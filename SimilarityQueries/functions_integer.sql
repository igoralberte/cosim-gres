-- Just making sure we are in the right schema/database
\c kiara

-- DIST FUNCTION
CREATE OR REPLACE FUNCTION get_feature(tblname1 VARCHAR(60), colname1 VARCHAR(60), fem_instance VARCHAR(60), colname1_id BIGINT) RETURNS INTEGER AS
$$
DECLARE 
v_feature INTEGER;
BEGIN
	
	EXECUTE 'SELECT mt1.'|| fem_instance ||'
				FROM metatable_' || tblname1 ||'_'|| colname1 ||' mt1 
				WHERE mt1.id = ' || colname1_id 
		INTO v_feature;
	RETURN v_feature;
 END;
$$
LANGUAGE plpgsql STABLE;


CREATE OR REPLACE FUNCTION get_fem(tblname1 VARCHAR(60), colname1 VARCHAR(60), INOUT cdd VARCHAR(80), OUT fem VARCHAR(60), OUT fem_parameters TEXT, OUT fem_instance VARCHAR(60)) AS
$$
DECLARE 
	 v_cdd VARCHAR(80);
BEGIN
    SELECT metatable_cdd.reserved_word AS metric, 
		metatable_fem.reserved_word AS fem,
		metatable_fem_instance.parameters AS fem_parameters,
		metatable_fem_instance.reserved_word AS fem_instance 
		INTO v_cdd, fem, fem_parameters, fem_instance 
	 FROM metatable_complex_column 
		INNER JOIN metatable_cdd 
		ON metatable_complex_column.cdd_id = metatable_cdd.id
		INNER JOIN metatable_fem_instance 
		ON metatable_cdd.fem_instance_id = metatable_fem_instance.id 
		INNER JOIN metatable_fem 
		ON metatable_fem_instance.fem_id = metatable_fem.id 
		WHERE metatable_complex_column.table_name = tblname1 
			AND metatable_complex_column.column_name = colname1 
			AND (metatable_cdd.reserved_word = cdd 
			OR (metatable_complex_column.is_default = true AND (cdd IS NULL OR cdd = '')))
		ORDER BY metatable_complex_column.is_default
		LIMIT 1;
	IF v_cdd is NULL THEN
		RAISE EXCEPTION 'Metric "%" not exist for table "%" and column "%" or the column has not default metric.', cdd, tblname1, colname1;
	END IF;
	cdd = v_cdd;
	IF NOT (fem_parameters = '' OR fem_parameters IS NULL) THEN
		fem_parameters = ',' || fem_parameters ;
	END IF;
 END;
$$
LANGUAGE plpgsql STABLE;

CREATE OR REPLACE FUNCTION get_df(tblname1 VARCHAR(60), colname1 VARCHAR(60), cdd VARCHAR(80)) RETURNS VARCHAR(60) AS 
$$
DECLARE 
	v_df VARCHAR(60);
BEGIN
    SELECT metatable_df.reserved_word AS df 
		INTO v_df 
	 FROM metatable_complex_column 
		INNER JOIN metatable_cdd
		ON metatable_complex_column.cdd_id = metatable_cdd.id
		INNER JOIN metatable_df 
		ON metatable_cdd.df_id = metatable_df.id 
		WHERE metatable_complex_column.table_name = tblname1 
			AND metatable_complex_column.column_name = colname1 
			AND (metatable_cdd.reserved_word = cdd 
			OR (metatable_complex_column.is_default = true AND (cdd IS NULL OR cdd = '')))
		ORDER BY metatable_complex_column.is_default
		LIMIT 1;
	IF v_df is NULL THEN
		RAISE EXCEPTION 'Metric "%" not exist for table "%" and column "%" or the column has not default metric.', cdd, tblname1, colname1;
	END IF;
	RETURN v_df;
 END;
$$
LANGUAGE plpgsql STABLE;


CREATE OR REPLACE FUNCTION fem_extraction(tblname1 VARCHAR(60), colname1 VARCHAR(60), cdd VARCHAR(60), complex_data INTEGER) RETURNS INTEGER AS
$$
DECLARE 
    v_fem VARCHAR(60);
    v_fem_instance VARCHAR(60);
    v_fem_parameters TEXT;
    v_feature1 INTEGER;
    time_bef TIMESTAMP;
BEGIN
    SELECT * FROM get_fem(tblname1, colname1, cdd) INTO cdd, v_fem, v_fem_parameters, v_fem_instance;


				
    --RAISE NOTICE '%', v_query_cdd;
    SELECT clock_timestamp() INTO time_bef;
    EXECUTE 'SELECT ' || v_fem ||'($1 ' || v_fem_parameters || ')' INTO v_feature1 USING complex_data;
    --RAISE NOTICE 'EXTRACTION % % % %', tblname1, colname1, cdd, (SELECT EXTRACT(EPOCH FROM (clock_timestamp() - time_bef)));
    RETURN v_feature1; 
 END;
$$
LANGUAGE plpgsql STABLE COST 1;


CREATE OR REPLACE FUNCTION dist(tblname1 VARCHAR(60), colname1 VARCHAR(60), tblname2 VARCHAR(60), colname2 VARCHAR(60), cdd VARCHAR(80), colname1_id BIGINT, colname2_id BIGINT) RETURNS FLOAT8 AS
$$
DECLARE 
    v_fem VARCHAR(60);
    v_fem_instance1 VARCHAR(60);
    v_fem_instance2 VARCHAR(60);
    v_fem_parameters TEXT;
    v_df VARCHAR(60);
    v_distance FLOAT8; 
    v_feature1 INTEGER;	
    v_feature2 INTEGER;	
    v_time_bef TIMESTAMP;
	v_record RECORD;
BEGIN 
	SELECT * FROM get_fem(tblname1, colname1, cdd) INTO cdd, v_fem, v_fem_parameters, v_fem_instance1;
	SELECT * FROM get_fem(tblname2, colname2, cdd) INTO cdd, v_fem, v_fem_parameters, v_fem_instance2;
	--RAISE NOTICE '% %', cdd, v_fem;
    IF v_fem_instance1 <> v_fem_instance2 THEN
		RAISE EXCEPTION 'Default metrics are different for table "%" with column "%" and table "%" with column "%". You must define an explicit metric using BY USING on NEAR clausule', tblname1, colname1, tblname2, colname2;
    END IF;
	v_df = get_df(tblname2, colname2, cdd);
    --RAISE NOTICE '%', v_df;
	v_feature1 = get_feature(tblname1, colname1, v_fem_instance1, colname1_id);
	v_feature2 = get_feature(tblname2, colname2, v_fem_instance2, colname2_id);
				

    SELECT clock_timestamp() INTO v_time_bef;
    EXECUTE 'SELECT ' || v_df ||'($1, $2)'
		INTO v_distance USING v_feature1, v_feature2;
    --RAISE NOTICE 'DISTANCE % % % % %', tblname1, tblname2, v_df, v_fem, (SELECT EXTRACT(EPOCH FROM (clock_timestamp() - v_time_bef)));
    RETURN v_distance; 
 END;
$$
LANGUAGE plpgsql STABLE;



CREATE OR REPLACE FUNCTION dist(tblname1 VARCHAR(60), colname1 VARCHAR(60), cdd VARCHAR(80), colname1_id BIGINT, feature2 INTEGER) RETURNS FLOAT8 AS
$$
DECLARE 
    v_query_cdd TEXT;
    v_df VARCHAR(60);
    v_fem VARCHAR(60);
    v_fem_instance VARCHAR(60);
    v_fem_parameters TEXT;
    v_feature1 INTEGER;
    v_distance FLOAT8;
    time_bef TIMESTAMP;
BEGIN
	SELECT * FROM get_fem(tblname1, colname1, cdd) INTO cdd, v_fem, v_fem_parameters, v_fem_instance;
	v_feature1 = get_feature(tblname1, colname1, v_fem_instance, colname1_id);	
	v_df = get_df(tblname1, colname1, cdd);

    SELECT clock_timestamp() INTO time_bef;
    EXECUTE 'SELECT ' || v_df ||'($1, $2)'
		INTO v_distance USING v_feature1, feature2;
    --RAISE NOTICE 'DISTANCE % % % %', tblname1, v_df, v_fem, (SELECT EXTRACT(EPOCH FROM (clock_timestamp() - time_bef)));
    RETURN v_distance; 
 END;
$$
LANGUAGE plpgsql STABLE;



CREATE OR REPLACE FUNCTION add_table_metric(RELATION TEXT, ATTRIBUTE TEXT, METRIC TEXT, IS_DEFAULT BOOLEAN)
 RETURNS void AS
$BODY$
DECLARE
   v_exists_complex_column_id BOOLEAN;
   v_id_cdd   BIGINT; 
   v_complex_column_id_name TEXT;
   v_complex_metatable TEXT;
   v_exists_complex_metatable_column BOOLEAN;
   v_complex_trigger TEXT;
   v_fem_instance_name TEXT;
   v_attribute_type TEXT;
 BEGIN
   IF NOT EXISTS (SELECT 1 
                    FROM pg_tables
                    WHERE TRIM(tablename) = TRIM(RELATION)) THEN
        RAISE EXCEPTION 'Relation "%" does not exist.', RELATION;
   END IF; 

  SELECT lower(data_type) 
    INTO v_attribute_type 
    FROM information_schema.columns 
    WHERE TRIM(table_name) = TRIM(RELATION) 
    AND TRIM(column_name) = TRIM(ATTRIBUTE);

   IF v_attribute_type IS NULL THEN
        RAISE EXCEPTION 'Column "%" does not exist in the relation "%".', ATTRIBUTE, RELATION;
   END IF; 
   
   IF v_attribute_type <> lower('INTEGER') THEN
        RAISE EXCEPTION 'Column "%" must be INTEGER type in the relation "%".', ATTRIBUTE, RELATION;
   END IF;

   SELECT id INTO v_id_cdd
     FROM metatable_cdd
     WHERE TRIM(reserved_word) = TRIM(METRIC);
    
   IF v_id_cdd IS NULL THEN
        RAISE EXCEPTION 'Metric "%" does not exist.', METRIC;
   END IF;

   IF NOT EXISTS (SELECT 1
                    FROM information_schema.columns 
                    WHERE TRIM(table_name) = TRIM(RELATION)
                      AND TRIM(column_name) = TRIM(ATTRIBUTE)) THEN
	EXECUTE 'ALTER TABLE ' || RELATION ||' ADD COLUMN ' || ATTRIBUTE ||' INTEGER';
   ELSE
	--RAISE NOTICE 'Column % exists.', ATTRIBUTE; 
   END IF;
   -- creating metatable
   v_complex_metatable = 'metatable_' || RELATION || '_' || ATTRIBUTE;


   IF NOT EXISTS (SELECT 1
                    FROM pg_tables 
                    WHERE TRIM(tablename) = TRIM(v_complex_metatable)) THEN
		EXECUTE 'CREATE TABLE ' || v_complex_metatable ||' (id BIGSERIAL PRIMARY KEY)';
   END IF;

   -- creating complex id
   v_complex_column_id_name = ATTRIBUTE||'_id'; 
   EXECUTE 'SELECT 1  
		FROM information_schema.columns 
		WHERE TRIM(table_name) = TRIM($1) 
			AND TRIM(column_name) = TRIM($2) ' 
	INTO v_exists_complex_column_id 
	USING RELATION, v_complex_column_id_name;

   IF v_exists_complex_column_id IS NULL THEN 
	EXECUTE 'ALTER TABLE ' || RELATION ||' 
				ADD COLUMN ' || v_complex_column_id_name ||' 
				BIGINT REFERENCES ' || v_complex_metatable || '(id)' ;
   END IF;  

   -- getting metric_reserved_word
   SELECT metatable_fem_instance.reserved_word 
		INTO v_fem_instance_name 
		FROM metatable_cdd  
		INNER JOIN metatable_fem_instance 
		ON metatable_cdd.fem_instance_id = metatable_fem_instance.id 
		WHERE metatable_cdd.id = v_id_cdd;

   -- creating complex_column
   EXECUTE 'SELECT 1 
		FROM information_schema.columns 
		WHERE TRIM(table_name) = TRIM($1) 
			AND TRIM(column_name) = TRIM($2) ' 
	INTO v_exists_complex_metatable_column 
	USING v_complex_metatable, v_fem_instance_name;

   IF v_exists_complex_metatable_column IS NULL THEN 
	EXECUTE 'ALTER TABLE ' || v_complex_metatable ||' ADD COLUMN ' || v_fem_instance_name ||' INTEGER';
   END IF;
   
   IF EXISTS (SELECT 1
                    FROM metatable_complex_column
                    WHERE TRIM(table_name) = TRIM(RELATION)
                      AND TRIM(column_name) = TRIM(ATTRIBUTE)
                      AND cdd_id = v_id_cdd) THEN 
	
       RAISE EXCEPTION 'The metric % is already associated to column % of table %.', METRIC, ATTRIBUTE, RELATION; 
   END IF;

   IF IS_DEFAULT THEN
	UPDATE metatable_complex_column SET is_default = FALSE 
		WHERE TRIM(table_name) = TRIM(RELATION)
                      AND TRIM(column_name) = TRIM(ATTRIBUTE);
   END IF;


   -- Creating trigger
   v_complex_trigger = 'trigger_' || v_complex_metatable;	
   EXECUTE 'DROP TRIGGER IF EXISTS ' || v_complex_trigger ||' ON ' || RELATION;
   EXECUTE 'CREATE TRIGGER ' || v_complex_trigger ||' 
		BEFORE INSERT OR UPDATE 
		ON ' || RELATION ||' 
		FOR EACH ROW EXECUTE PROCEDURE feature_extractor( ''' || RELATION ||''' , '''|| ATTRIBUTE ||''')';	

   INSERT INTO metatable_complex_column (table_name, column_name, cdd_id, is_default)
          VALUES (RELATION, ATTRIBUTE, v_id_cdd, IS_DEFAULT);

 END;
$BODY$
 LANGUAGE 'plpgsql' VOLATILE;


CREATE OR REPLACE FUNCTION feature_extractor() RETURNS trigger AS
$feature_extractor$
DECLARE 
	v_complex_value INTEGER;
    v_feature INTEGER;
    v_metatable_name TEXT;
    v_complex_column_id_name TEXT;
    v_exists_metatable_row TEXT;
    v_fem_parameters TEXT;
    v_metatable_id BIGINT;
    v_seq_name TEXT;
    v_cursor CURSOR (param1 TEXT, param2 TEXT) IS SELECT DISTINCT(metatable_fem_instance.reserved_word) AS femi_instance,
							metatable_fem_instance.parameters AS femi_parameters,
								metatable_fem.reserved_word AS femi
							FROM metatable_complex_column 
							JOIN metatable_cdd 
							ON metatable_complex_column.cdd_id = metatable_cdd.id 
							JOIN metatable_fem_instance 
							ON metatable_cdd.fem_instance_id = metatable_fem_instance.id 
							JOIN metatable_fem 
							ON metatable_fem_instance.fem_id = metatable_fem.id 
							WHERE table_name = param1 and column_name = param2;

    v_record RECORD;
    tblname VARCHAR(60);
    colname VARCHAR(60);
    time_bef TIMESTAMP;
    time_bef_all TIMESTAMP;
BEGIN
	tblname = TG_ARGV[0]; 
	colname = TG_ARGV[1];
    v_metatable_name = 'metatable_' || tblname || '_' || colname;
    v_complex_column_id_name =  colname || '_id';
    v_seq_name = v_metatable_name || '_id_seq';


	SELECT clock_timestamp() INTO time_bef_all;

	IF TG_OP = 'UPDATE' THEN
		EXECUTE 'SELECT $1.' || v_complex_column_id_name INTO v_metatable_id USING OLD;
	END IF;

	--setting complex value
	EXECUTE 'SELECT $1.' || colname INTO v_complex_value USING NEW;

	IF v_complex_value IS NULL THEN
        IF TG_OP = 'INSERT' THEN
            RAISE EXCEPTION 'Column "%" must be different to null.', colname;    
        END IF;
        EXECUTE 'SELECT $1.' || colname INTO v_complex_value USING OLD;
	END IF;
	
	-- check if colname_id exists
	EXECUTE 'SELECT 1 FROM ' || v_metatable_name || ' WHERE id = $1'
	INTO v_exists_metatable_row USING v_metatable_id;

	IF TG_OP = 'INSERT'	OR v_exists_metatable_row IS NULL THEN
		EXECUTE 'SELECT nextval($1)' INTO v_metatable_id USING v_seq_name;

		-- Inserting row
		EXECUTE 'INSERT INTO ' || v_metatable_name || '(id) VALUES (' || v_metatable_id || ')';

		--RAISE NOTICE 'NEW= %', NEW;
		EXECUTE 'SELECT (r).* FROM (SELECT $1 #= ''"'||v_complex_column_id_name || '"=>"'||v_metatable_id||'"'' AS r ) s '
			INTO NEW USING NEW;
		--EXECUTE 'SELECT (r).* FROM (SELECT t #= ''"'||v_complex_column_id_name || '"=>"'||v_metatable_id||'"'' AS r FROM $1 t) '
		--	INTO NEW USING NEW;
		--RAISE NOTICE 'NEW= %', NEW;
		--RAISE NOTICE 'NEW ASSIGNMENTS SUCCESFULLY';
		--EXECUTE 'SELECT $1.' || v_complex_column_id_name INTO v_metatable_id USING NEW;
	END IF;
	
--    EXECUTE 'SELECT hstore($1) -> $2' INTO v_complex_data USING NEW, colname;
--    EXECUTE 'SELECT hstore($1) -> $2' INTO v_metatable_id  USING NEW, v_complex_column_id_name;

	--RAISE NOTICE 'valueSD= %', v_metatable_id;
	
    -- Get all metrics 
	FOR v_record IN v_cursor(tblname, colname) LOOP
			SELECT clock_timestamp() INTO time_bef;
			v_fem_parameters = v_record.femi_parameters;

			IF NOT (v_fem_parameters = '' OR v_fem_parameters IS NULL) THEN
				v_fem_parameters = ',' || v_fem_parameters;
			END IF;

			EXECUTE 'SELECT ' || v_record.femi || '($1 ' || v_fem_parameters || ')' 
				   INTO v_feature USING v_complex_value;

			EXECUTE 'UPDATE ' || v_metatable_name || ' SET ' || v_record.femi_instance || ' = $1 WHERE id = ' || v_metatable_id
					USING v_feature;
			--RAISE NOTICE 'EXTRACTION % % %', tblname, v_record.femi_instance, (SELECT EXTRACT(EPOCH FROM (clock_timestamp() - time_bef)));
	END LOOP;

--	RAISE NOTICE 'INSERTION % % %', tblname, colname, (SELECT EXTRACT(EPOCH FROM (clock_timestamp() - time_bef_all)));
    RETURN NEW; 
 END;
$feature_extractor$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION create_metric(METRIC_NAME TEXT, DF_KEYWORD TEXT, FEM_INSTANCE_KEYWORD TEXT)
 RETURNS integer AS
$BODY$
DECLARE

   v_return int4 = 0;
   v_id_fem_instance BIGINT;
   v_id_df  BIGINT;

 BEGIN

   SELECT id INTO v_id_fem_instance
     FROM metatable_fem_instance
     WHERE TRIM(reserved_word) = TRIM(FEM_INSTANCE_KEYWORD);

   SELECT id INTO v_id_df
     FROM metatable_df
     WHERE TRIM(reserved_word) = TRIM(DF_KEYWORD);
   
   IF v_id_fem_instance IS NULL THEN
        RAISE EXCEPTION 'Feature extractor instance "%" does not exist.', FEM_INSTANCE_KEYWORD;
   END IF;

   IF v_id_df IS NULL THEN
        RAISE EXCEPTION 'Distance function "%" does not exist.', DF_KEYWORD;
   END IF;

   INSERT INTO metatable_cdd (fem_instance_id, df_id, reserved_word)
        VALUES (v_id_fem_instance, v_id_df, METRIC_NAME);
 
   RETURN v_return;
 
 END;
$BODY$
 LANGUAGE 'plpgsql' VOLATILE;


CREATE OR REPLACE FUNCTION create_fem_instance(FEM_INSTANCE_KEYWORD TEXT, FEM_KEYWORD TEXT, FEM_PARAMETERS TEXT='')
 RETURNS integer AS
$BODY$
DECLARE

   v_return int4 = 0;
   v_id_fem BIGINT;
   
 BEGIN

   SELECT id INTO v_id_fem
     FROM metatable_fem
     WHERE TRIM(reserved_word) = TRIM(FEM_KEYWORD);

   IF v_id_fem IS NULL THEN
        RAISE EXCEPTION 'Feature extractor "%" does not exist.', FEM_KEYWORD;
   END IF;

   INSERT INTO metatable_fem_instance (reserved_word,parameters,fem_id)
        VALUES (FEM_INSTANCE_KEYWORD, FEM_PARAMETERS, v_id_fem);
 
   RETURN v_return;
 
 END;
$BODY$
 LANGUAGE 'plpgsql' VOLATILE;

CREATE OR REPLACE FUNCTION drop_metric(METRIC_NAME VARCHAR)
 RETURNS integer AS
$BODY$
DECLARE

   v_return int4 = 0;

 BEGIN

   IF EXISTS (SELECT 1
                FROM metatable_cdd
                WHERE TRIM(reserved_word) = METRIC_NAME) THEN

       DELETE 
         FROM metatable_cdd
         WHERE TRIM(reserved_word) = METRIC_NAME;

   ELSE 

       RAISE EXCEPTION 'Metric "%" does not exist.', METRIC_NAME;

   END IF;
 
   RETURN v_return;
 
 END;
$BODY$
 LANGUAGE 'plpgsql' VOLATILE;

--EXAMPLE FOR POPULATION ON TABLES 
--SCHEMA
 CREATE OR REPLACE FUNCTION euclidean(v1 int, v2 int) RETURNS FLOAT8 AS
 $$
 BEGIN
     RETURN sqrt(V1 * V1 + V2 * V2);
  END;
 $$
 LANGUAGE plpgsql;

 CREATE OR REPLACE FUNCTION manhattan(v1 int, v2 int) RETURNS FLOAT8 AS
 $$
 BEGIN
     RETURN abs(V1 - V2);
  END;
 $$
 LANGUAGE plpgsql;

 CREATE OR REPLACE FUNCTION fem1(v1 int) RETURNS INT AS
 $$
 BEGIN
     RETURN v1 * 2;
  END;
 $$
 LANGUAGE plpgsql;

 CREATE OR REPLACE FUNCTION fem2(v1 int, parameter1 int) RETURNS INT AS
 $$
 BEGIN
     RETURN v1 * 3 + parameter1;
  END;
 $$
 LANGUAGE plpgsql;


 INSERT INTO metatable_complex_type (name) VALUES ('IMAGE');

 INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('fem1', 1); 
 INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('fem2', 1); 

 INSERT INTO metatable_df (reserved_word) VALUES ('euclidean');
 INSERT INTO metatable_df (reserved_word) VALUES ('manhattan');  

-- INSERT INTO metatable_fem_instance (reserved_word, fem_id, parameters) VALUES ('fem1_1', 1, ''); 
-- INSERT INTO metatable_fem_instance (reserved_word, fem_id, parameters) VALUES ('fem2_1', 2, '1'); 
SELECT create_fem_instance('fem1', 'fem1', '');
SELECT create_fem_instance('fem2', 'fem2', '1');

INSERT INTO metatable_cdd (fem_instance_id, df_id, reserved_word) VALUES (1, 1, 'fem1_euclidean'); 
INSERT INTO metatable_cdd (fem_instance_id, df_id, reserved_word) VALUES (2, 2, 'fem2_manhathan');

--CREATE TABLE T1(ID BIGSERIAL PRIMARY KEY, image1 INTEGER);
--CREATE TABLE T2(ID BIGSERIAL PRIMARY KEY, image2 INTEGER);

--SELECT add_table_metric('t1', 'image1', 'fem1_euclidean', true);
--SELECT add_table_metric('t2', 'image2', 'fem2_manhathan', true);


--INSERT INTO t1(image1) values(2);
--INSERT INTO t1(image1) values(13);
--INSERT INTO t2(image2) values(13);
-- SELECT get_feature('t1', 'c1', 'fem1_1_euclidean', 1);

-- SELECT add_table_metric('t1', 'c1', 'fem1_2_manhathan', true);
-- SELECT add_table_metric('t1', 'c1', 'fem2_1_euclidean', true);
-- SELECT add_table_metric('t1', 'c1', 'fem2_1_manhathan', true);


-- INSERT INTO t1 (c1) VALUES (3);

-- SELECT * FROM t1;
-- SELECT * FROM metatable_t1_c1;

-- UPDATE t1 set c1 = 4 where id = 1;

-- SELECT * FROM t1;
-- SELECT * FROM metatable_t1_c1;

-- DELETE FROM metatable_t1_c1;

-- SELECT * FROM metatable_t1_c1;

-- UPDATE t1 set c1 = 4 where id = 1;

-- SELECT * FROM metatable_t1_c1;
