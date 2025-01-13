-- Para gerar o arquivo distance_functions.so:
--
-- g++ -I/home/ph/SimS/include -I/usr/pgsql-9.4/include/server distance_functions.cpp -c -fpic
-- g++ -shared -o distance_functions.so distance_functions.o
-- sudo cp distance_functions.so /var/lib/pgsql/9.4/
\c kiara
\set path '\'$libdir/metric_access.so\''

-----------------------------------------------------------
-- Configura a linguagem C como linguagem confiável
UPDATE pg_language SET lanpltrusted = true WHERE lanname LIKE 'c';
CREATE EXTENSION cube;


-- Cria tabela de índices criados
CREATE TABLE IF NOT EXISTS similarql_indexes(
    name text NOT NULL,
    idx_table text NOT NULL,
    attrib_complex text NOT NULL,
    distance_function text NOT NULL,
    attrib_included text,
    pred text,
    root text,
    flags integer,
    pagesize integer,
    PRIMARY KEY(name, idx_table, attrib_complex, distance_function),
    UNIQUE(idx_table, attrib_complex, distance_function)
);

-- CREATE FUNCTIONS
CREATE OR REPLACE FUNCTION slim_euclidean_distance_create(
    text, text, text, text, integer)
  RETURNS text AS
:path, 'slim_euclidean_distance_create'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;

CREATE OR REPLACE FUNCTION slim_manhattan_distance_create(
    text, text, text, text, integer)
  RETURNS text AS
:path, 'slim_manhattan_distance_create'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;

CREATE OR REPLACE FUNCTION slim_chebyshev_distance_create(
    text, text, text, text, integer)
  RETURNS text AS
:path, 'slim_chebyshev_distance_create'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;

CREATE OR REPLACE FUNCTION slim_ledit_distance_create(
    text, text, text, text, integer)
  RETURNS text AS
:path, 'slim_ledit_distance_create'
  LANGUAGE c IMMUTABLE STRICT
  COST 1;

-----------------------------------------------------------
-- ADD FUNCTIONS
CREATE OR REPLACE FUNCTION slim_euclidean_distance_add(
      bytea, integer, text, text, text, text)
    RETURNS boolean AS
  :path, 'slim_euclidean_distance_add'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;


CREATE OR REPLACE FUNCTION slim_manhattan_distance_add(
      bytea, integer, text, text, text, text)
    RETURNS boolean AS
  :path, 'slim_manhattan_distance_add'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;


CREATE OR REPLACE FUNCTION slim_chebyshev_distance_add(
      bytea, integer, text, text, text, text)
    RETURNS boolean AS
  :path, 'slim_chebyshev_distance_add'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;

CREATE OR REPLACE FUNCTION slim_ledit_distance_add(
      bytea, integer, text, text, text, text)
    RETURNS boolean AS
  :path, 'slim_ledit_distance_add'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;

-----------------------------------------------------------
-- PRINT STATISTICS
CREATE OR REPLACE FUNCTION print_statistics(
      text, text, text, text)
    RETURNS void AS
  :path, 'print_statistics'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;


-----------------------------------------------------------
-- RANGE FUNCTIONS
CREATE OR REPLACE FUNCTION slim_euclidean_distance_range(
    bytea, real, text, text, text, text, 
    OUT id integer, OUT distance double precision)
    RETURNS SETOF RECORD AS
  :path, 'slim_euclidean_distance_range'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;

CREATE OR REPLACE FUNCTION slim_manhattan_distance_range(
    bytea, real, text, text, text, text, 
    OUT id integer, OUT distance double precision)
    RETURNS SETOF RECORD AS
  :path, 'slim_manhattan_distance_range'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;

CREATE OR REPLACE FUNCTION slim_chebyshev_distance_range(
    bytea, real, text, text, text, text,
    OUT id integer, OUT distance double precision)
    RETURNS SETOF RECORD AS
  :path, 'slim_chebyshev_distance_range'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;

CREATE OR REPLACE FUNCTION slim_ledit_distance_range(
    bytea, real, text, text, text, text,
    OUT id integer, OUT distance double precision)
    RETURNS SETOF RECORD AS
  :path, 'slim_ledit_distance_range'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;


-----------------------------------------------------------
-- KNN FUNCTIONS
CREATE OR REPLACE FUNCTION slim_euclidean_distance_knn(
	bytea, integer, text, text, text, text,
    OUT id integer, OUT distance double precision, OUT korder integer)
	RETURNS SETOF RECORD AS
	:path, 'slim_euclidean_distance_knn'
	LANGUAGE c IMMUTABLE STRICT
	COST 1;

CREATE OR REPLACE FUNCTION slim_manhattan_distance_knn(
    bytea, integer, text, text, text, text,
    OUT id integer, OUT distance double precision, OUT korder integer)
    RETURNS SETOF RECORD AS
    :path, 'slim_manhattan_distance_knn'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;

CREATE OR REPLACE FUNCTION slim_chebyshev_distance_knn(
    bytea, integer, text, text, text, text,
    OUT id integer, OUT distance double precision, OUT korder integer)
    RETURNS SETOF RECORD AS
    :path, 'slim_chebyshev_distance_knn'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;

CREATE OR REPLACE FUNCTION slim_ledit_distance_knn(
    bytea, integer, text, text, text, text,
    OUT id integer, OUT distance double precision, OUT korder integer)
    RETURNS SETOF RECORD AS
    :path, 'slim_ledit_distance_knn'
    LANGUAGE c IMMUTABLE STRICT
    COST 1;
-----------------------------------------------------------


-- Colocar no arquivo de criação de tabelas script inicial
/*    CREATE TABLE similarql_indexes(
        name TEXT NOT NULL,
        idx_table TEXT NOT NULL,
        Attrib_complex TEXT NOT NULL,
        distance_function TEXT NOT NULL,
        Attrib_included TEXT,
        Pred TEXT,
        Root TEXT,
        Flags INTEGER,
        PageSize INTEGER,
        UNIQUE(idx_table, attrib_complex, distance_function),
        PRIMARY KEY (Name, idx_Table, Attrib_complex, Distance_function));
*/

DROP TABLE pontos3d;
CREATE TABLE pontos3d AS SELECT n, (100*random())::int x, (100*random())::int y, 
 (100*random())::int z FROM generate_series(1, 1000) n;
ALTER TABLE pontos3d ADD COLUMN pontos_vector DOUBLE PRECISION[];
ALTER TABLE pontos3d ADD COLUMN pontos_bytea BYTEA;
UPDATE pontos3d SET pontos_vector = ARRAY[x, y, z];
UPDATE pontos3d SET pontos_bytea = text_to_signature(array_to_string(pontos_vector, ' '));


-- CRIA A SLIM-TREE E ADICIONA OS DADOS DA TABELA
-- pname: nome do índice
-- pidx_Table: nome da tabela indexada
-- pattrib_id: atributo de identificação da tabela (id)
-- pattrib_complex: atributo sobre o qual está o índice
-- pdistance_function: função de distância
-- ppagesize: tamanho, em bytes, da página da slim-tree
-- pattrib_included: atributo incluído
-- ppred: predicado WHERE para criação do índice
DROP FUNCTION IF EXISTS simql_create_index;
CREATE OR REPLACE FUNCTION simql_create_index(
	pname text, pidx_table text, pattrib_id text, pattrib_complex text, pdistance_function text, 
    ppagesize INTEGER, pattrib_included text default '', ppred text default '')
	RETURNS VOID AS $$
	
    DECLARE
        resultado TEXT = '';
        cmd TEXT = '';

	BEGIN

        -- Insert Slim-tree in similarql_indexes table
        INSERT INTO similarql_indexes(
                name, idx_table, attrib_complex, distance_function, pagesize, attrib_included, pred)
                VALUES (pname, pidx_table, pattrib_complex, pdistance_function, ppagesize,
                    pattrib_included, ppred);

        CASE pdistance_function
            WHEN 'euclidean_distance' THEN
                
        		SELECT slim_euclidean_distance_create(pname, pidx_Table, pattrib_complex, 
                    pdistance_function, ppagesize) INTO resultado;

                cmd := 'SELECT slim_euclidean_distance_add(' || pattrib_complex ||  ', ' || 
                            pattrib_id || ', ''' || pname || ''', ''' || pidx_table ||
                            ''', ''' || pattrib_complex || ''', ''' || pdistance_function  || ''') FROM ' ||
                            pidx_Table;

                EXECUTE cmd;

            WHEN 'manhattan_distance' THEN
                
                SELECT slim_manhattan_distance_create(pname, pidx_Table, pattrib_complex, 
                    pdistance_function, ppagesize) INTO resultado;

                cmd := 'SELECT slim_manhattan_distance_add(' || pattrib_complex ||  ', ' || 
                            pattrib_id || ', ''' || pname || ''', ''' || pidx_table ||
                            ''', ''' || pattrib_complex || ''', ''' || pdistance_function  || ''') FROM ' ||
                            pidx_Table;

                EXECUTE cmd;

            WHEN 'chebyshev_distance' THEN
                
                SELECT slim_chebyshev_distance_create(pname, pidx_Table, pattrib_complex, 
                    pdistance_function, ppagesize) INTO resultado;

                cmd := 'SELECT slim_chebyshev_distance_add(' || pattrib_complex ||  ', ' || 
                            pattrib_id || ', ''' || pname || ''', ''' || pidx_table ||
                            ''', ''' || pattrib_complex || ''', ''' || pdistance_function  || ''') FROM ' ||
                            pidx_Table;

                EXECUTE cmd;                

            WHEN 'ledit_distance' THEN
                SELECT slim_ledit_distance_create(pname, pidx_Table, pattrib_complex, 
                    pdistance_function, ppagesize) INTO resultado;

                cmd := 'SELECT slim_ledit_distance_add(' || pattrib_complex ||  ', ' || 
                            pattrib_id || ', ''' || pname || ''', ''' || pidx_table ||
                            ''', ''' || pattrib_complex || ''', ''' || pdistance_function  || 
                            ''') FROM ' ||
                            pidx_Table;

                EXECUTE cmd;                

            ELSE
                RAISE EXCEPTION 'Distance Function does not exist!';
        END CASE;

            --RAISE NOTICE 'Resultado: %', resultado;

        EXCEPTION
            WHEN unique_violation THEN
                RAISE EXCEPTION 'Slim-tree already exists!';
		
	END;
$$ LANGUAGE plpgsql;


-- APENAS ADICIONA DADOS A UMA SLIM-TREE JÁ EXISTENTE
-- pname: nome do índice
-- pidx_Table: nome da tabela indexada
-- pattrib_id: atributo de identificação da tabela (id)
-- pattrib_complex: atributo sobre o qual está o índice
-- pdistance_function: função de distância
-- pattrib_included: atributo incluído
-- ppred: predicado WHERE para criação do índice
DROP FUNCTION IF EXISTS simql_add_data_index;
CREATE OR REPLACE FUNCTION simql_add_data_index(
    pname text, pidx_table text, pattrib_id text, pattrib_complex text, pdistance_function text, 
    pattrib_included text default '', ppred text default '')
    RETURNS VOID AS $$
    
    DECLARE
        resultado TEXT = '';
        cmd TEXT = '';
        qtd INTEGER = 0;

    BEGIN

        -- Search for Slim-tree in similarql_indexes table
        SELECT count(*) INTO qtd FROM similarql_indexes WHERE name = pname AND idx_table = pidx_table 
            AND attrib_complex = pattrib_complex AND distance_function = pdistance_function;

        -- Não há Slim-trees com as informações recebidas
        IF (qtd < 1) THEN 
            RAISE EXCEPTION 'The Slim-tree does not exist!' 
                USING HINT = 'Please check the parameters!';
            RETURN; 
        END IF;

        CASE pdistance_function
            WHEN 'euclidean_distance' THEN
                
                cmd := 'SELECT slim_euclidean_distance_add(' || pattrib_complex ||  ', ' || 
                            pattrib_id || ', ''' || pname || ''', ''' || pidx_table ||
                            ''', ''' || pattrib_complex || ''', ''' || pdistance_function  || ''') FROM ' ||
                            pidx_Table;

                EXECUTE cmd;

            WHEN 'manhattan_distance' THEN
                
                cmd := 'SELECT slim_manhattan_distance_add(' || pattrib_complex ||  ', ' || 
                            pattrib_id || ', ''' || pname || ''', ''' || pidx_table ||
                            ''', ''' || pattrib_complex || ''', ''' || pdistance_function  || ''') FROM ' ||
                            pidx_Table;

                EXECUTE cmd;

            WHEN 'chebyshev_distance' THEN
                
                cmd := 'SELECT slim_chebyshev_distance_add(' || pattrib_complex ||  ', ' || 
                            pattrib_id || ', ''' || pname || ''', ''' || pidx_table ||
                            ''', ''' || pattrib_complex || ''', ''' || pdistance_function  || ''') FROM ' ||
                            pidx_Table;

                EXECUTE cmd;                

            WHEN 'ledit_distance' THEN
                
                cmd := 'SELECT slim_ledit_distance_add(' || pattrib_complex ||  ', ' || 
                            pattrib_id || ', ''' || pname || ''', ''' || pidx_table ||
                            ''', ''' || pattrib_complex || ''', ''' || pdistance_function  || 
                            ''') FROM ' ||
                            pidx_Table;

                EXECUTE cmd;                

            ELSE
                RAISE EXCEPTION 'Distance Function does not exist!';
        END CASE;

            --RAISE NOTICE 'Resultado: %', resultado;
        
    END;
$$ LANGUAGE plpgsql;

-- APAGA SLIM-TREE DA TABELA SIMILARQL_INDEXES
-- pname: nome do índice
-- pidx_Table: nome da tabela indexada
-- pattrib_complex: atributo sobre o qual está o índice
-- pdistance_function: função de distância
DROP FUNCTION IF EXISTS simql_drop_index;
CREATE OR REPLACE FUNCTION simql_drop_index(
    pname text, pidx_table text, pattrib_complex text, pdistance_function text)
    RETURNS VOID AS $$
    
    DECLARE
        resultado TEXT = '';
        cmd TEXT = '';

    BEGIN
        DELETE FROM similarql_indexes WHERE
                name = pname AND idx_table = pidx_table AND attrib_complex = pattrib_complex AND 
                distance_function = pdistance_function;
            
    END;
$$ LANGUAGE plpgsql;

---------------------------------------------------------------------------------------------------------
-- REALIZA UMA CONSULTA RANGE UTILIZANDO UMA SLIM-TREE OU DE FORMA SEQUENCIAL
-- center (bytea): vetor de características em formato bytea do centro de consulta
-- radius (numeric): valor do raio
-- pname (text): nome do índice a ser utilizado
-- pidx_table (text): nome da tabela a ser consultada
-- pattrib_id (text): nome do atributo identificador da tabela consultada
-- pattrib_complex (text): nome do atributo complexo que contém valores bytea
-- pdistance_function (text): nome da função de distância
-- pattrib_included (text): permite inclusão de atributos (NÃO IMPLEMENTADO)
-- ppred (text): permite avaliação de predicados (NÃO IMPLEMENTADO)
-- RETORNO: tabela com tuplas do tipo (Id, Distância do centro)
DROP FUNCTION IF EXISTS slim_range;
CREATE OR REPLACE FUNCTION slim_range(
    center bytea, radius numeric, pname text, pidx_table text, pattrib_id text, 
    pattrib_complex text, pdistance_function text, 
    pattrib_included text default '', ppred text default '')
    RETURNS TABLE(id INTEGER, distance DOUBLE PRECISION) AS $$
    
    DECLARE
        resultado TEXT = '';
        cmd TEXT = '';

    BEGIN

        CASE pdistance_function
            WHEN 'euclidean_distance' THEN
                RETURN QUERY (SELECT * FROM slim_euclidean_distance_range(center, radius, pname, pidx_Table, 
                    pattrib_complex, pdistance_function));

            WHEN 'manhattan_distance' THEN
                RETURN QUERY (SELECT * FROM slim_manhattan_distance_range(center, radius, pname, pidx_Table, 
                    pattrib_complex, pdistance_function));

            WHEN 'chebyshev_distance' THEN
                RETURN QUERY (SELECT * FROM slim_chebyshev_distance_range(center, radius, pname, pidx_Table, 
                    pattrib_complex, pdistance_function));

            WHEN 'ledit_distance' THEN
                RETURN QUERY (SELECT * FROM slim_ledit_distance_range(center, radius, pname, pidx_Table, 
                    pattrib_complex, pdistance_function));            

            ELSE
                RAISE EXCEPTION 'Função de distância não existe!';
        END CASE;
        
    END;
$$ LANGUAGE plpgsql;

-- REALIZA UMA CONSULTA KNN UTILIZANDO UMA SLIM-TREE
-- center (bytea): vetor de características em formato bytea do centro de consulta
-- k (integer): valor de k
-- pname (text): nome do índice a ser utilizado
-- pidx_table (text): nome da tabela a ser consultada
-- pattrib_id (text): nome do atributo identificador da tabela consultada
-- pattrib_complex (text): nome do atributo complexo que contém valores bytea
-- pdistance_function (text): nome da função de distância
-- pattrib_included (text): permite inclusão de atributos (NÃO IMPLEMENTADO)
-- ppred (text): permite avaliação de predicados (NÃO IMPLEMENTADO)
-- RETORNO: tabela com tuplas do tipo (Id, Distância do centro, Ordem k)
DROP FUNCTION IF EXISTS slim_knn;
CREATE OR REPLACE FUNCTION slim_knn(
    center bytea, k integer, pname text, pidx_table text, pattrib_id text, 
    pattrib_complex text, pdistance_function text, 
    pattrib_included text default '', ppred text default '')
    RETURNS TABLE(id INTEGER, distance DOUBLE PRECISION) AS $$
    --, korder INTEGER) AS 
    
    DECLARE
        resultado TEXT = '';
        cmd TEXT = '';

    BEGIN

        CASE pdistance_function
            WHEN 'euclidean_distance' THEN
                RETURN QUERY (SELECT ans.id, ans.distance FROM slim_euclidean_distance_knn(center, k, pname, pidx_Table, 
                    pattrib_complex, pdistance_function) as ans);

            WHEN 'manhattan_distance' THEN
                RETURN QUERY (SELECT ans.id, ans.distance FROM slim_manhattan_distance_knn(center, k, pname, pidx_Table, 
                    pattrib_complex, pdistance_function) as ans);

            WHEN 'chebyshev_distance' THEN
                RETURN QUERY (SELECT ans.id, ans.distance FROM slim_chebyshev_distance_knn(center, k, pname, pidx_Table, 
                    pattrib_complex, pdistance_function) as ans);

            WHEN 'ledit_distance' THEN      
                RETURN QUERY (SELECT ans.id, ans.distance FROM slim_ledit_distance_knn(center, k, pname, pidx_Table, 
                    pattrib_complex, pdistance_function) as ans);


            ELSE
                RAISE EXCEPTION 'Função de distância não existe!';
        END CASE;
        
    END;
$$ LANGUAGE plpgsql;

--==================================================================================================
------ Definição dos parâmetros de execução ---------------- =======================================
--==================================================================================================
DROP TABLE  IF EXISTS SimilarQL_Settings;
CREATE TABLE SimilarQL_Settings(
    Param  TEXT PRIMARY KEY,
    Value  TEXT);
INSERT INTO SimilarQL_Settings VALUES ('similarql_d_trace', 'false');
INSERT INTO SimilarQL_Settings VALUES ('similarql_GCD_Unit', 'k');        -- k=Km, m=Miles, n=Nautical Miles, y=Yards, everything else=meters (case insensitive)
INSERT INTO SimilarQL_Settings VALUES ('similarql_p_norm', '2.0');
INSERT INTO SimilarQL_Settings VALUES ('similarql_FDGPowPow', '2.0');
INSERT INTO SimilarQL_Settings VALUES ('similarql_FDGQuantile', '0.25');  -- First quantile (25%)

DROP FUNCTION IF EXISTS    SimilarQL_SetParam (P TEXT, V TEXT);
CREATE OR REPLACE FUNCTION SimilarQL_SetParam (P TEXT, V TEXT) RETURNS INTEGER AS $$ BEGIN
    UPDATE SimilarQL_Settings SET Value=V WHERE Lower(Param)=Lower(P);
    RETURN 0;
END; $$ LANGUAGE 'plpgsql';

DROP FUNCTION IF EXISTS    SimilarQL_ShowParam (P TEXT);
CREATE OR REPLACE FUNCTION SimilarQL_ShowParam (P TEXT) RETURNS TEXT AS $$
DECLARE V TEXT;
BEGIN
    SELECT Value INTO V FROM SimilarQL_Settings WHERE Lower(Param)=Lower(P);
    RETURN V;
END; $$ LANGUAGE 'plpgsql';

--==================================================================================================
-- SELECT RANGE  ------ ============================================================================
--==================================================================================================
-- REALIZA CONSULTA RANGE UTILIZANDO A SLIM-TREE, CASO HAJA, OU DE FORMA SEQUENCIAL
-- Relation: relation's name
-- Atr: name of the complex attribute (type of the attribute: BYTEA)
-- DF: distance function name. Available: euclidean_distance, manhattan_distance, chebyshev_distance
-- Center: bytea vector which represents the query center
-- Radius: query radius
-- ID_p: name of the identification attribute of the relation. Default: 'id'
-- seqscan: caso seja true, executa consulta sequencial, mesmo caso haja índice
-- RETORNO: tabela com tuplas (Id, Distância do centro)
DROP FUNCTION IF EXISTS Select_SimRange;
CREATE OR REPLACE FUNCTION Select_SimRange (Relation VARCHAR, Atr VARCHAR, DF VARCHAR, 
    Center BYTEA, Radius NUMERIC DEFAULT -1.0, ID_p VARCHAR DEFAULT 'id', seqscan BOOLEAN DEFAULT FALSE)
    RETURNS TABLE(id INTEGER, distance DOUBLE PRECISION) AS $$
DECLARE
    Var_r RECORD; Var_Cmd TEXT; Direction TEXT;  F VARCHAR; ParamV TEXT;  DFT VARCHAR;
    var_idx_search TEXT; idx_tuple similarql_indexes%ROWTYPE;
    var_idx_name TEXT; var_idx_table TEXT; var_idx_attrib_complex TEXT; var_distance_function TEXT;
BEGIN
    
    SELECT Value INTO ParamV FROM SimilarQL_Settings WHERE Param='similarql_d_trace';
    -- Try to find an available Slim-tree for the query
    SELECT name, idx_table, attrib_complex, distance_function INTO
        var_idx_name, var_idx_table, var_idx_attrib_complex, var_distance_function
        FROM similarql_indexes 
        WHERE idx_table = relation AND attrib_complex = atr AND distance_function = DF;

    -- If the Slim-tree was not found, it executes Sequential Scan
    IF (var_idx_name = '') IS NOT FALSE OR seqscan = TRUE THEN
        IF ParamV='true' THEN 
            IF seqscan = TRUE THEN RAISE NOTICE 'Forced Sequential Scan!'; 
            ELSE RAISE NOTICE 'Sequential Scan!'; END IF; END IF;
        
        --IF Filter IS NULL THEN F:=' '; ELSE F:=' AND '|| Filter; END IF;
        IF Radius<0 THEN Radius:=0;  END IF;
        --IF Exist THEN F:=F||' LIMIT 1'; END IF;
        DFT:=DF||'($1, REL.'|| Atr ||')';
        --IF Farthest THEN Direction:='>$2'; ELSE Direction:='<=$2';  END IF;
        Direction:='<=$2'; 
        
        --CONSULTA VERSÃO KNN
        --Var_Cmd:='SELECT ID, Distance FROM (SELECT ' || ID_p || ' ID, ROUND((' || DFT || ')::numeric,6)::DOUBLE PRECISION Distance,
        --    RANK() OVER (ORDER BY '||DFT||' ASC) AS kOrd 
        --    FROM '||Relation||' REL) AS ids_distances
        --    WHERE Distance ' ||Direction;

        --CONSULTA RANGE NATURAL
        Var_Cmd:='SELECT ID, Distance FROM (SELECT ' || ID_p || ' ID, ROUND((' || DFT || ')::numeric,6)::DOUBLE PRECISION Distance 
            FROM '||Relation||' REL) AS ids_distances
            WHERE Distance ' ||Direction;
        
        IF ParamV='true' THEN RAISE NOTICE E'Select_SimRange Statement:\n%', Var_Cmd; END IF;
        
        FOR Var_r IN EXECUTE Var_Cmd USING Center, Radius LOOP
            id:=Var_r.id;
            distance:=Var_r.distance;
            RETURN NEXT;
        END LOOP;


        --RETURN QUERY EXECUTE var_cmd USING center, radius;
            --END LOOP;

    ELSE
        IF ParamV='true' THEN RAISE NOTICE 'Slim Scan!'; END IF;

        RETURN QUERY(SELECT * FROM slim_range(Center, Radius, var_idx_name, var_idx_table, ID_p, atr, df));
    END IF;
END; $$ LANGUAGE 'plpgsql' VOLATILE;


--==================================================================================================
------ SELECT KNN ------ ===========================================================================
-- Select_KNN (Relation, Atr, DF, Center, K, Tie, Untie, NoCenter, CountTuples, Farthest, Filter) --
--==================================================================================================
DROP FUNCTION IF EXISTS Select_KNN;

-- REALIZA CONSULTA KNN UTILIZANDO A SLIM-TREE, CASO HAJA, OU DE FORMA SEQUENCIAL
-- Relation: relation's name
-- Atr: name of the complex attribute (type of the attribute: BYTEA)
-- DF: distance function name. Available: euclidean_distance, manhattan_distance, chebyshev_distance
-- Center: bytea vector which represents the query center
-- k: number of neighbors
-- ID_p: name of the identification attribute of the relation. Default: 'id'
-- seqscan: caso seja true, executa consulta sequencial, mesmo caso haja índice
-- RETORNO: tabela com tuplas (Id, Distância do centro, Ordem k)
CREATE OR REPLACE FUNCTION Select_KNN (Relation VARCHAR, Atr VARCHAR, DF VARCHAR, Center BYTEA, 
        K INTEGER DEFAULT 1, ID_p VARCHAR DEFAULT 'id', seqscan BOOLEAN DEFAULT FALSE)
    RETURNS TABLE (ID INTEGER, Distance DOUBLE PRECISION) AS $$ 
    --, KOrder INTEGER) AS 
DECLARE
    Var_r RECORD; Var_Cmd TEXT;  Var_Func TEXT; Direction TEXT; Var_Distinct TEXT;  
    UnT VARCHAR; CC VARCHAR; DFT VARCHAR; F VARCHAR;
    R1 RegType;   ParamV TEXT;
    var_idx_name TEXT; var_idx_table TEXT; var_idx_attrib_complex TEXT; var_distance_function TEXT;
BEGIN
    IF DF IS NULL OR Center IS NULL THEN RETURN; END IF;
    CC:=Relation||'.'||Atr;
    SELECT Value INTO ParamV FROM SimilarQL_Settings WHERE Param='similarql_d_trace';

    -- Try to find an available Slim-tree for the query
    SELECT name, idx_table, attrib_complex, distance_function INTO
        var_idx_name, var_idx_table, var_idx_attrib_complex, var_distance_function
        FROM similarql_indexes 
        WHERE idx_table = relation AND attrib_complex = atr AND distance_function = DF;

    -- If the Slim-tree was not found, it executes Sequential Scan
    IF (var_idx_name = '') IS NOT FALSE OR seqscan = TRUE THEN
        IF ParamV='true' THEN 
            IF seqscan = TRUE THEN RAISE NOTICE 'Forced Sequential Scan!'; 
            ELSE RAISE NOTICE 'Sequential Scan!'; END IF; END IF;


        Direction:='::FLOAT ASC';
        
        DFT:=DF||'($1, '||CC;

        Var_Cmd:='SELECT * FROM (SELECT ';  
        

        --VERSÃO ORIGINAL
        Var_Cmd:=Var_Cmd ||  Relation || '.' || ID_p || ' ID, ROUND((' || DFT || '))::numeric,6)::DOUBLE PRECISION Dist,
            RANK() OVER (ORDER BY '||DFT||')'||Direction ||') AS kOrd FROM '||Relation||') AS ids_distances
            WHERE kOrd <= ' || K;

        --VERSÃO KNN NATURAL
        --Var_Cmd:=Var_Cmd ||  Relation || '.' || ID_p || ' ID, ROUND((' || DFT || '))::numeric,6)::DOUBLE PRECISION Dist
        --     FROM '||Relation||') AS ids_distances) as ans_table
        --    WHERE kOrd <= ' || K;

        --VERSÃO SEM O KORD
        --Var_Cmd:=Var_Cmd ||  Relation || '.' || ID_p || ' ID, ROUND((' || DFT || '))::numeric,6)::DOUBLE PRECISION Dist
        --    FROM '||Relation||') AS ids_distances ORDER BY Dist LIMIT ' || K;
            

        IF ParamV='true' THEN RAISE NOTICE E'Select_KNN k:= %. Statement:%', K, Var_Cmd; END IF;
        FOR Var_r IN EXECUTE Var_Cmd USING Center LOOP
                ID:=Var_r.ID;
                Distance:=Var_r.Dist;
                --KOrder:=Var_r.kOrd;
                RETURN NEXT;
            END LOOP;
    ELSE
        IF ParamV='true' THEN RAISE NOTICE 'Slim Scan!'; END IF;

        RETURN QUERY(SELECT * FROM slim_knn(Center, K, var_idx_name, var_idx_table, ID_p, atr, df) as ans);
    END IF;
END; $$ LANGUAGE 'plpgsql' VOLATILE;


ALTER FUNCTION slim_euclidean_distance_create(text, text, text, text)
  OWNER TO postgres;

ALTER FUNCTION slim_euclidean_distance_add(bytea, integer, text, text, text, text)
    OWNER TO postgres;

ALTER FUNCTION slim_euclidean_distance_range(bytea, real, text, text, text, text)
    OWNER TO postgres;

ALTER FUNCTION slim_euclidean_distance_knn(bytea, integer, text, text, text, text)
	OWNER TO postgres;

ALTER FUNCTION print_statistics(text, text, text, text)
  OWNER TO postgres;

ALTER FUNCTION simql_create_index(text, text, text, text, text, text)
  OWNER TO postgres;

ALTER FUNCTION Select_SimRange OWNER TO postgres;
