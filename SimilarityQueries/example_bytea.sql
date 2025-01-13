-- Just making sure we are in the right schema/database
\c kiara

-- EXAMPLE OF DISTANCE FUNCTION DEFINITION
CREATE OR REPLACE FUNCTION metric1(v1 bytea, v2 bytea) RETURNS DECIMAL AS
$$
BEGIN
    RETURN (V1::text::int - V2::text::int);
 END;
$$
LANGUAGE plpgsql;

-- EXAMPLE OF DISTANCE FUNCTION DEFINITION
CREATE OR REPLACE FUNCTION sum_images(v1 bytea, v2 bytea) RETURNS DECIMAL AS
$$
BEGIN
    RETURN abs(V1::text::int - V2::text::int);
 END;
$$
LANGUAGE plpgsql;


-- EXAMPLE OF EXTRACTOR DEFINITION
CREATE OR REPLACE FUNCTION extractor1(v1 bytea) RETURNS BYTEA AS
$$
BEGIN
    RETURN v1;
 END;
$$
LANGUAGE plpgsql;


-- CLEAN METATABLES
DELETE FROM metatable_complex_type;
DELETE FROM metatable_df;
DELETE FROM metatable_fem;
DELETE FROM metatable_cdd;
DELETE FROM metatable_complex_column;

-- POPULATES METATABLE
INSERT INTO metatable_complex_type (name) VALUES ('IMAGE');
INSERT INTO metatable_df (reserved_word) VALUES ('sum_images');
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('extractor1', 
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));
-- INSERT INTO metatable_cdd (fem_id, df_id, reserved_word) VALUES (1, 1, 'metric1');

-- INSERT INTO metatable_complex_column VALUES ('t1', 'c1', 1, true);
-- INSERT INTO metatable_complex_column VALUES ('t2', 'c2', 1, true);

-- INSERT INTO metatable_complex_column VALUES ('t1', 'c1', 2, true);
-- INSERT INTO metatable_complex_column VALUES ('t2', 'c2', 2, true);




CREATE METRIC metric1 USING sum_images FOR ( extractor1 );
-- THIS QUERY IS TRANSLATE TO THIS
-- SELECT create_metric('metric1', 'sum_images', 'extractor1');

CREATE METRIC metric2 USING sum_images FOR ( extractor1 );
-- THIS QUERY IS TRANSLATE TO THIS
-- SELECT create_metric('metric2', 'sum_images', 'extractor1');

CREATE METRIC metric3 USING sum_images FOR ( extractor1 );
-- THIS QUERY IS TRANSLATE TO THIS
-- SELECT create_metric('metric3', 'sum_images', 'extractor1');

DROP TABLE IF EXISTS t1;
CREATE TABLE t1(id int);

-- THIS ACTION SHOULD BE PERFORMED BY TRIGGER
DROP TABLE IF EXISTS metatable_t1_c;

ALTER TABLE t1 ADD METRIC (c) USING (metric1) DEFAULT;
-- THIS QUERY IS TRANSLATE TO THIS
-- SELECT add_table_metric('t1', 'c', 'metric1', true);

ALTER TABLE t1 ADD METRIC (c) USING (metric2);
-- THIS QUERY IS TRANSLATE TO THIS
-- SELECT add_table_metric('t1', 'c', 'metric2', false); 

ALTER TABLE t1 ADD METRIC (c) USING (metric3);
-- THIS QUERY IS TRANSLATE TO THIS
-- SELECT add_table_metric('t1', 'c', 'metric3', false); 

DROP TABLE IF EXISTS t2;
CREATE TABLE t2(id int, c BYTEA);

-- THIS ACTION SHOULD BE PERFORMED BY TRIGGER
DROP TABLE IF EXISTS metatable_t2_c;

ALTER TABLE t2 ADD METRIC (c) USING (metric1) DEFAULT;
-- THIS QUERY IS TRANSLATE TO THIS
-- SELECT add_table_metric('t1', 'c', 'metric1', true);

ALTER TABLE t2 ADD METRIC (c) USING (metric2);
-- THIS QUERY IS TRANSLATE TO THIS
-- SELECT add_table_metric('t1', 'c', 'metric2', false); 

ALTER TABLE t2 ADD METRIC (c) USING (metric3);
-- THIS QUERY IS TRANSLATE TO THIS
-- SELECT add_table_metric('t1', 'c', 'metric3', false); 



-- INSERT VALUES ON TABLES
-- these inserts should be performed by trigger
-- INSERT INTO metatable_t1_c VALUES (1, 0::text::bytea,0::text::bytea,0::text::bytea);
-- INSERT INTO metatable_t1_c VALUES (2, 1::text::bytea,1::text::bytea,1::text::bytea);
-- INSERT INTO metatable_t1_c VALUES (3, 2::text::bytea,2::text::bytea,2::text::bytea);
-- INSERT INTO metatable_t2_c VALUES (1, 0::text::bytea,0::text::bytea,0::text::bytea);
-- INSERT INTO metatable_t2_c VALUES (2, 1::text::bytea,1::text::bytea,1::text::bytea);
-- INSERT INTO metatable_t2_c VALUES (3, 2::text::bytea,2::text::bytea,2::text::bytea);

-- visualize the data in metatable
-- SELECT * FROM metatable_t1_c; 
-- SELECT * FROM metatable_t2_c;

-- the last column should be performed by trigger
INSERT INTO t1 VALUES (1, 0::text::bytea);
INSERT INTO t1 VALUES (2, 1::text::bytea);
INSERT INTO t1 VALUES (3, 2::text::bytea);
INSERT INTO t2 VALUES (1, 0::text::bytea);
INSERT INTO t2 VALUES (2, 1::text::bytea);
INSERT INTO t2 VALUES (3, 2::text::bytea);


-- TO QUERY METATABLES
-- select get_feature('t1', 'c', 'metric1', 1);
-- select get_feature('t2', 'c', 'metric1', 1);


-- EXAMPLES TO USE OUR SYNTAX 
SELECT * FROM t1 RANGE JOIN t2 ON t1.c NEAR t2.c RANGE 5;
SELECT * FROM t1 NEAREST JOIN t2 ON t1.c NEAR t2.c STOP AFTER 5;
SELECT * FROM t1 CLOSEST JOIN t2 ON t1.c NEAR t2.c STOP AFTER 5;
SELECT * FROM t1,t2 WHERE t1.c NEAR t2.c STOP AFTER 5;
SELECT * FROM t1,t2 WHERE t1.c NEAR t2.c RANGE 5;
SELECT * FROM t1 WHERE t1.c NEAR (SELECT c from t1 WHERE id=1) STOP AFTER 5;
SELECT * FROM t1 WHERE t1.c NEAR (SELECT c from t1 WHERE id=1) RANGE 5;
