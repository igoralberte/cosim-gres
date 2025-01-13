-- Para gerar o arquivo distance_functions.so:
--
-- g++ -I/home/ph/SimS/include -I/usr/pgsql-9.4/include/server distance_functions.cpp -c -fpic
-- g++ -shared -o distance_functions.so distance_functions.o
-- sudo cp distance_functions.so /var/lib/pgsql/9.4/
\c kiara
\set path '\'$libdir/distance_functions.so\''

CREATE OR REPLACE FUNCTION euclidean_distance(
    bytea,
    bytea)
  RETURNS double precision AS
:path, 'euclidean_distance'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;
ALTER FUNCTION euclidean_distance(bytea, bytea)
  OWNER TO postgres;

CREATE OR REPLACE FUNCTION manhattan_distance(
    bytea,
    bytea)
  RETURNS double precision AS
:path, 'manhattan_distance'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;
ALTER FUNCTION manhattan_distance(bytea, bytea)
  OWNER TO postgres;

CREATE OR REPLACE FUNCTION chebyshev_distance(
    bytea,
    bytea)
  RETURNS double precision AS
:path, 'chebyshev_distance'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;
ALTER FUNCTION chebyshev_distance(bytea, bytea)
  OWNER TO postgres;

CREATE OR REPLACE FUNCTION canberra_distance(
    bytea,
    bytea)
  RETURNS double precision AS
:path, 'canberra_distance'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;
ALTER FUNCTION canberra_distance(bytea, bytea)
  OWNER TO postgres;

CREATE OR REPLACE FUNCTION kullback_leibler_divergence_distance(
    bytea,
    bytea)
  RETURNS double precision AS
:path, 'kullback_leibler_divergence_distance'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;
ALTER FUNCTION kullback_leibler_divergence_distance(bytea, bytea)
  OWNER TO postgres;

CREATE OR REPLACE FUNCTION jeffrey_divergence_distance(
    bytea,
    bytea)
  RETURNS double precision AS
:path, 'jeffrey_divergence_distance'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;
ALTER FUNCTION jeffrey_divergence_distance(bytea, bytea)
  OWNER TO postgres;

CREATE OR REPLACE FUNCTION hamming_distance(
    bytea,
    bytea)
  RETURNS double precision AS
:path, 'hamming_distance'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;
ALTER FUNCTION hamming_distance(bytea, bytea)
  OWNER TO postgres;

CREATE OR REPLACE FUNCTION ledit_distance(
    bytea,
    bytea)
  RETURNS double precision AS
:path, 'ledit_distance'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;
ALTER FUNCTION ledit_distance(bytea, bytea)
  OWNER TO postgres;
-- select euclidean_distance(decode('10 20 30', 'escape'), decode('40 50 60', 'escape'));
-- select euclidean_distance((select features from test where id = 5), (select features from test where id = 6));

-- select euclidean_distance(decode('10 20 30 40', 'escape'), decode('50 60 70 80', 'escape'));
-- select manhattan_distance(decode('10 20 30 40', 'escape'), decode('50 60 70 80', 'escape'));
-- select chebyshev_distance(decode('10 20 30 40', 'escape'), decode('50 60 70 80', 'escape'));

-- insert into test (features) values (decode('1 2 3', 'escape'));

-- select * from test;


CREATE OR REPLACE FUNCTION text_to_signature(text) 
  RETURNS bytea AS 
  :path, 'text_to_signature' 
  LANGUAGE c IMMUTABLE STRICT 
  COST 1;
ALTER FUNCTION text_to_signature(text) OWNER TO postgres;

CREATE OR REPLACE FUNCTION text_to_signature_char(text) 
  RETURNS bytea AS 
  :path, 'text_to_signature_char' 
  LANGUAGE c IMMUTABLE STRICT 
  COST 1;
ALTER FUNCTION text_to_signature_char(text) OWNER TO postgres;

CREATE OR REPLACE FUNCTION signature_to_text(bytea) 
  RETURNS text AS 
  :path, 'signature_to_text' 
  LANGUAGE c IMMUTABLE STRICT 
  COST 1;
ALTER FUNCTION signature_to_text(bytea) OWNER TO postgres;

CREATE OR REPLACE FUNCTION signature_to_text_char(bytea) 
  RETURNS text AS 
  :path, 'signature_to_text_char' 
  LANGUAGE c IMMUTABLE STRICT 
  COST 1;
ALTER FUNCTION signature_to_text_char(bytea) OWNER TO postgres;
