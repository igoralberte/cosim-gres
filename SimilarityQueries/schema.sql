\c postgres
DROP DATABASE IF EXISTS KIARA;
CREATE DATABASE KIARA;
\c kiara

CREATE EXTENSION hstore;

-----------------------------------------------------------
-- BEGIN METATABLES
-----------------------------------------------------------

-----------------------------------------------------------
-- metatable_complex_type
-- This table must contain the supported complex types
-----------------------------------------------------------
DROP TABLE IF EXISTS metatable_complex_type CASCADE;
CREATE TABLE metatable_complex_type 
(
	id BIGSERIAL,
	name VARCHAR(60),
	UNIQUE (name),
	PRIMARY KEY(id)
);

-----------------------------------------------------------
-- metatable_fem
-- This table associates the extractors with complex types
-----------------------------------------------------------
DROP TABLE IF EXISTS metatable_fem CASCADE;
CREATE TABLE metatable_fem
(
	id BIGSERIAL,
	reserved_word VARCHAR(60),
	complex_type_id BIGINT REFERENCES metatable_complex_type(id) ON DELETE CASCADE ON UPDATE CASCADE,
	UNIQUE (reserved_word),
	PRIMARY KEY(id)
);

DROP TABLE IF EXISTS metatable_fem_instance CASCADE;
CREATE TABLE metatable_fem_instance 
(
	id BIGSERIAL,
	reserved_word VARCHAR(60),
	parameters TEXT DEFAULT '',
	fem_id BIGINT REFERENCES metatable_fem(id) ON DELETE CASCADE ON UPDATE CASCADE,
	UNIQUE (reserved_word),
	PRIMARY KEY(id)
);


-----------------------------------------------------------
-- metatable_df
-- This table must contain the supported distance functions
-----------------------------------------------------------
DROP TABLE IF EXISTS metatable_df CASCADE;
CREATE TABLE metatable_df
(
	id BIGSERIAL,
	reserved_word VARCHAR(60),
	UNIQUE (reserved_word),
	PRIMARY KEY(id)	
);

-----------------------------------------------------------
-- metatable_cdd
-- This table associates the extractors with distance functions
-- with a metric name
-----------------------------------------------------------
DROP TABLE IF EXISTS metatable_cdd CASCADE;
CREATE TABLE metatable_cdd
(
	id BIGSERIAL,
	fem_instance_id BIGINT REFERENCES metatable_fem_instance(id) ON DELETE CASCADE ON UPDATE CASCADE,
	df_id BIGINT REFERENCES metatable_df(id) ON DELETE CASCADE ON UPDATE CASCADE,
	reserved_word VARCHAR(60),
	UNIQUE (reserved_word),
	PRIMARY KEY(id)
);

-----------------------------------------------------------
-- metatable_complex_column
-- This table associates a complex column with a metric
-----------------------------------------------------------
DROP TABLE IF EXISTS metatable_complex_column CASCADE;
CREATE TABLE metatable_complex_column
(
	table_name VARCHAR(60),
	column_name VARCHAR(60),
	cdd_id BIGINT REFERENCES metatable_cdd(id) ON DELETE CASCADE ON UPDATE CASCADE,
	is_default BOOLEAN,
	PRIMARY KEY(table_name, column_name, cdd_id)
);
