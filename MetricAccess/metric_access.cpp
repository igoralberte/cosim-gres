#include "metric_access.h"

#ifdef __cplusplus
extern "C" {
#endif

    #ifdef PG_MODULE_MAGIC
        PG_MODULE_MAGIC;
    #endif

    // This macro is a skelleton function to support hermes functions
    #define SIMILARITY_SUPPORT_SLIM_CREATE(sql_function_name,hermes_function_name);                                                                         \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                                                                                 \
    Datum sql_function_name(PG_FUNCTION_ARGS) {                                                                                                             \
        if (PG_ARGISNULL(0) || PG_ARGISNULL(1)|| PG_ARGISNULL(2)|| PG_ARGISNULL(3)) {                                                                       \
            ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                                  \
        }                                                                                                                                                   \
        /* get args */                                                                                                                                      \
        /*nome índice, nome tabela indexada, atributo indexado, função de distância.*/                                                                      \
        /* for slim -> table,attrib,func_distance,extractor*/                                                                                               \
        text *arg1 = (text *)PG_GETARG_TEXT_PP(0);                                                                                                          \
        text *arg2 = (text *)PG_GETARG_TEXT_PP(1);                                                                                                          \
        text *arg3 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                          \
        text *arg4 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                          \
        int32 pagesize = PG_GETARG_INT32(4);                                                                                                                \
                                                                                                                                                            \
        /*testar se algum dos argumentos é vazio - AINDA É UM PROBLEMA*/                                                                                    \
        /*if(*arg1 == "" or *arg2 == "" or *arg3 == "" or *arg4 == "") { ereport(ERROR, (errmsg("Empty parameters are not accepted."))); }*/                \
                                                                                                                                                            \
        /* convert bytea to signature */                                                                                                                    \
        /*ereport(ERROR, (errmsg("APRESENTAR FILENAME")));*/                                                                                                \
        text *new_text = concat4(arg1, arg2, arg3, arg4);                                                                                                   \
        /* Criamos a Slim */                                                                                                                                \
        DiskManagerSingleton::getInstance().setPageManager(dup_pgtext(new_text), pagesize);                                                                 \
        /* Testamos utilizando o pageManager criado*/                                                                                                       \
        stSlimTree < Signature,  hermes_function_name<Signature>  >* tree =                                                                                 \
                new stSlimTree < Signature,  hermes_function_name <Signature> >                                                                             \
                (DiskManagerSingleton::getInstance().getPageManager(dup_pgtext(new_text)));                                                                 \
        delete tree;                                                                                                                                        \
        /*PG_RETURN_NULL();*/                                                                                                                               \
        /*PG_RETURN_TEXT_P(new_text);*/                                                                                                                     \
        /*new_text = concat4("", "", "", ""); ereport(NOTICE, (errmsg("Nome salvo %s", new_text)));*/                                                       \
        PG_RETURN_TEXT_P(new_text);                                                                                                                         \
    }
    
    // This macro is a skelleton function to support hermes functions
    #define SIMILARITY_SUPPORT_SLIM_CREATE_CHAR(sql_function_name,hermes_function_name);                                                                    \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                                                                                 \
    Datum sql_function_name(PG_FUNCTION_ARGS) {                                                                                                             \
        if (PG_ARGISNULL(0) || PG_ARGISNULL(1)|| PG_ARGISNULL(2)|| PG_ARGISNULL(3)) {                                                                       \
            ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                                  \
        }                                                                                                                                                   \
        /* get args */                                                                                                                                      \
        /*nome índice, nome tabela indexada, atributo indexado, função de distância.*/                                                                      \
        /* for slim -> table,attrib,func_distance,extractor*/                                                                                               \
        text *arg1 = (text *)PG_GETARG_TEXT_PP(0);                                                                                                          \
        text *arg2 = (text *)PG_GETARG_TEXT_PP(1);                                                                                                          \
        text *arg3 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                          \
        text *arg4 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                          \
        int32 pagesize = PG_GETARG_INT32(4);                                                                                                                \
                                                                                                                                                            \
        /*testar se algum dos argumentos é vazio - AINDA É UM PROBLEMA*/                                                                                    \
        /*if(*arg1 == "" or *arg2 == "" or *arg3 == "" or *arg4 == "") { ereport(ERROR, (errmsg("Empty parameters are not accepted."))); }*/                \
                                                                                                                                                            \
        /* convert bytea to signature */                                                                                                                    \
        /*ereport(ERROR, (errmsg("APRESENTAR FILENAME")));*/                                                                                                \
        text *new_text = concat4(arg1, arg2, arg3, arg4);                                                                                                   \
        /* Criamos a Slim */                                                                                                                                \
        DiskManagerSingleton::getInstance().setPageManager(dup_pgtext(new_text), pagesize);                                                                 \
        /* Testamos utilizando o pageManager criado*/                                                                                                       \
        stSlimTree < SignatureChar,  hermes_function_name<SignatureChar>  >* tree =                                                                         \
                new stSlimTree < SignatureChar,  hermes_function_name <SignatureChar> >                                                                     \
                (DiskManagerSingleton::getInstance().getPageManager(dup_pgtext(new_text)));                                                                 \
        delete tree;                                                                                                                                        \
        /*PG_RETURN_NULL();*/                                                                                                                               \
        /*PG_RETURN_TEXT_P(new_text);*/                                                                                                                     \
        /*new_text = concat4("", "", "", ""); ereport(NOTICE, (errmsg("Nome salvo %s", new_text)));*/                                                       \
        PG_RETURN_TEXT_P(new_text);                                                                                                                         \
    }

    // PH: Precisa de apenas um PG_ARGISNULL (somente um vetor de características vem por parâmetro)
    // PH: Deixar apenas byte_array0 e signature0 (apagar os que têm 1 no final do nome)

    #define SIMILARITY_SUPPORT_SLIM_ADD(sql_function_name,hermes_function_name);                                                                            \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                                                                                 \
    Datum sql_function_name(PG_FUNCTION_ARGS) {                                                                                                             \
        if (PG_ARGISNULL(0) || PG_ARGISNULL(1)|| PG_ARGISNULL(2) || PG_ARGISNULL(3) || PG_ARGISNULL(4) || PG_ARGISNULL(5))  {                               \
            ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                                  \
        }                                                                                                                                                   \
        /* get args */                                                                                                                                      \
        /* signature, OID, nome tabela, nome coluna, função de distância, extrator de características.*/                                                    \
        /* for slim -> table,attrib,func_distance,extractor*/                                                                                               \
        bytea *byte_array = PG_GETARG_BYTEA_P(0);                                                                                                           \
        int32 image_id = PG_GETARG_INT32(1);                                                                                                                \
        text *arg1 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                          \
        text *arg2 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                          \
        text *arg3 = (text *)PG_GETARG_TEXT_PP(4);                                                                                                          \
        text *arg4 = (text *)PG_GETARG_TEXT_PP(5);                                                                                                          \
        text *new_text = concat4(arg1, arg2, arg3, arg4);                                                                                                   \
        /*ereport(INFO, (errmsg("Construiu new_text: %s", VARDATA(new_text))));*/                                                                           \
                                                                                                                                                            \
        /* Criamos o vetor de caracteristica e setamos seu ID para adicionar na Slim*/                                                                      \
        Signature signature = ByteArrayToBasicArrayObject(byte_array);                                                                                      \
        signature.setOID(image_id);                                                                                                                         \
        /* slim */                                                                                                                                          \
        /*NAMEBASEEDINtable,attrib,extractor*/                                                                                                              \
        /*Antigo*/                                                                                                                                          \
        /*stSlimTree < Signature,  hermes_function_name >* tree = new stSlimTree < Signature,  hermes_function_name >(pageManager);*/                       \
        /*Novo*/                                                                                                                                            \
        stSlimTree <Signature, hermes_function_name<Signature>  >* tree = new stSlimTree < Signature,                                                       \
                        hermes_function_name <Signature> >(DiskManagerSingleton::getInstance().                                                             \
                            getPageManager(dup_pgtext(new_text)));                                                                                          \
        /* Adicionamos o elemento na Slim e retornamos para a funcao o retorno do mesmo*/                                                                   \
        bool ret = tree->Add(&signature);                                                                                                                   \
        delete tree;                                                                                                                                        \
        pfree(new_text);                                                                                                                                    \
        PG_RETURN_BOOL(ret);                                                                                                                                \
        /*PG_RETURN_TEXT_P(new_text);*/                                                                                                                     \
    }


    #define SIMILARITY_SUPPORT_SLIM_ADD_CHAR(sql_function_name,hermes_function_name);                                                                       \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                                                                                 \
    Datum sql_function_name(PG_FUNCTION_ARGS) {                                                                                                             \
        if (PG_ARGISNULL(0) || PG_ARGISNULL(1)|| PG_ARGISNULL(2) || PG_ARGISNULL(3) || PG_ARGISNULL(4) || PG_ARGISNULL(5))  {                               \
            ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                                  \
        }                                                                                                                                                   \
        /* get args */                                                                                                                                      \
        /* signature, OID, nome tabela, nome coluna, função de distância, extrator de características.*/                                                    \
        /* for slim -> table,attrib,func_distance,extractor*/                                                                                               \
        bytea *byte_array = PG_GETARG_BYTEA_P(0);                                                                                                           \
        int32 image_id = PG_GETARG_INT32(1);                                                                                                                \
        text *arg1 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                          \
        text *arg2 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                          \
        text *arg3 = (text *)PG_GETARG_TEXT_PP(4);                                                                                                          \
        text *arg4 = (text *)PG_GETARG_TEXT_PP(5);                                                                                                          \
        text *new_text = concat4(arg1, arg2, arg3, arg4);                                                                                                   \
        /*ereport(INFO, (errmsg("Construiu new_text: %s", VARDATA(new_text))));*/                                                                           \
                                                                                                                                                            \
        /* Criamos o vetor de caracteristica e setamos seu ID para adicionar na Slim*/                                                                      \
        SignatureChar signature = CharByteArrayToBasicArrayObject(byte_array);                                                                              \
        signature.setOID(image_id);                                                                                                                         \
        /* slim */                                                                                                                                          \
        /*NAMEBASEEDINtable,attrib,extractor*/                                                                                                              \
        /*Antigo*/                                                                                                                                          \
        /*stSlimTree < Signature,  hermes_function_name >* tree = new stSlimTree < Signature,  hermes_function_name >(pageManager);*/                       \
        /*Novo*/                                                                                                                                            \
        stSlimTree <SignatureChar, hermes_function_name<SignatureChar>  >* tree = new stSlimTree < SignatureChar,                                           \
                        hermes_function_name <SignatureChar> >(DiskManagerSingleton::getInstance().                                                         \
                            getPageManager(dup_pgtext(new_text)));                                                                                          \
        /* Adicionamos o elemento na Slim e retornamos para a funcao o retorno do mesmo*/                                                                   \
        bool ret = tree->Add(&signature);                                                                                                                   \
        delete tree;                                                                                                                                        \
        pfree(new_text);                                                                                                                                    \
        PG_RETURN_BOOL(ret);                                                                                                                                \
        /*PG_RETURN_TEXT_P(new_text);*/                                                                                                                     \
    }


    // Importante verificar que essa funcao eh um callback, ou seja, ela fica se chamando 
    // sempre para resolver o problema de retornar multiplas linhas da busca
    #define SIMILARITY_SUPPORT_SLIM_RANGE(sql_function_name,hermes_function_name);                                                                          \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                                                                                 \
    Datum sql_function_name(PG_FUNCTION_ARGS) {                                                                                                             \
        /* Estrutura do Postgres, verificar documentacao*/                                                                                                  \
        FuncCallContext *funcctx;                                                                                                                           \
        myResult* result = nullptr;                                                                                                                         \
        /* myResult aponta para o resultado da busca*/                                                                                                      \
        bytea *ret_array = PG_GETARG_BYTEA_P(0);                                                                                                            \
        TupleDesc tupdesc;                                                                                                                                  \
        AttInMetadata *attinmeta;                                                                                                                           \
                                                                                                                                                            \
        char **values;                                                                                                                                      \
        HeapTuple tuple;                                                                                                                                    \
        Datum resultdatum;                                                                                                                                  \
                                                                                                                                                            \
        /* Verifica se o callback entrou pela primeira vez*/                                                                                                \
        if (SRF_IS_FIRSTCALL()) {                                                                                                                           \
            /* Inicializa o contexto da funcao */                                                                                                           \
            MemoryContext oldcontext;                                                                                                                       \
            funcctx = SRF_FIRSTCALL_INIT();                                                                                                                 \
            oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);                                                                             \
            if (PG_ARGISNULL(0) || PG_ARGISNULL(1)|| PG_ARGISNULL(2) || PG_ARGISNULL(3) || PG_ARGISNULL(4) || PG_ARGISNULL(5)) {                            \
                ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                              \
            }                                                                                                                                               \
                                                                                                                                                            \
            bytea *byte_array = PG_GETARG_BYTEA_P(0);                                                                                                       \
            float4 radius = PG_GETARG_FLOAT4(1);                                                                                                            \
            text *arg1 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                      \
            text *arg2 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                      \
            text *arg3 = (text *)PG_GETARG_TEXT_PP(4);                                                                                                      \
            text *arg4 = (text *)PG_GETARG_TEXT_PP(5);                                                                                                      \
                                                                                                                                                            \
            text *new_text = concat4(arg1, arg2, arg3, arg4);                                                                                               \
            Signature signature = ByteArrayToBasicArrayObject(byte_array);                                                                                  \
                                                                                                                                                            \
            /* Cria a Slim utilizando o pageManager e convertendo o texto do postgres para string em c(char*) */                                            \
            stSlimTree <Signature, hermes_function_name<Signature>  >* tree = new stSlimTree                                                                \
                    < Signature,  hermes_function_name <Signature> >(DiskManagerSingleton::getInstance().                                                   \
                        getPageManager(dup_pgtext(new_text)));                                                                                              \
                                                                                                                                                            \
            /* Realiza a busca com o raio determinado */                                                                                                    \
            result = tree->RangeQuery(&signature, ((float) radius));                                                                                        \
            /* Seta quantas vezes a funcao vai ser chamada = numero de objetos retornados na query*/                                                        \
            funcctx->max_calls = result->GetNumOfEntries();                                                                                                 \
                                                                                                                                                            \
            /* Build a tuple descriptor for our result type */                                                                                              \
            if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)                                                                         \
                ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),                                                                                     \
                                errmsg("Function returning record called in context "                                                                       \
                                        "that cannot accept type record!")));                                                                               \
                                                                                                                                                            \
            /*generate attribute metadata needed later to produce tuples from raw C strings*/                                                               \
            attinmeta = TupleDescGetAttInMetadata(tupdesc);                                                                                                 \
            funcctx->attinmeta = attinmeta;                                                                                                                 \
                                                                                                                                                            \
            /* Itera sobre todos os elementos e armazena na estrutura criada anteriormente*/                                                                \
            /* importante ver aqui que so fazemos a busca na primeira vez que a funcao roda*/                                                               \
            for(auto elem : *(result->GetPairs())) {                                                                                                        \
                answer_oids.push_back(elem->GetObject()->GetOID());                                                                                         \
                answer_dists.push_back(elem->GetDistance());                                                                                                \
            }                                                                                                                                               \
            delete result;                                                                                                                                  \
            delete tree;                                                                                                                                    \
            pfree(new_text);                                                                                                                                \
            result = 0;                                                                                                                                     \
            tree = 0;                                                                                                                                       \
            /* Postgres*/                                                                                                                                   \
            MemoryContextSwitchTo(oldcontext);                                                                                                              \
        }                                                                                                                                                   \
        /* retorna os elementos de fato, e quando acaba o numero de elementos que                                                                           \
        tem q retornar, ele avisa que acabou ao contexto*/                                                                                                  \
        funcctx = SRF_PERCALL_SETUP();                                                                                                                      \
        attinmeta = funcctx->attinmeta;                                                                                                                     \
                                                                                                                                                            \
        if (funcctx->call_cntr < funcctx->max_calls) {                                                                                                      \
                                                                                                                                                            \
            /*Prepare a values array for building the returned tuple.                                                                                       \
            * This should be an array of C strings which will                                                                                               \
            * be processed later by the type input functions.*/                                                                                             \
            values = (char **) palloc(2 * sizeof(char *));                                                                                                  \
            values[0] = (char *) palloc(16 * sizeof(char));                                                                                                \
            values[1] = (char *) palloc(16 * sizeof(char));                                                                                                \
                                                                                                                                                            \
            snprintf(values[0], 16, "%d", answer_oids[funcctx->call_cntr]);                                                                                \
            snprintf(values[1], 16, "%lf", answer_dists[funcctx->call_cntr]);                                                                              \
                                                                                                                                                            \
            /* build a tuple */                                                                                                                             \
            tuple = BuildTupleFromCStrings(attinmeta, values);                                                                                              \
                                                                                                                                                            \
            /* make the tuple into a datum */                                                                                                               \
            resultdatum = HeapTupleGetDatum(tuple);                                                                                                         \
                                                                                                                                                            \
            pfree(values[0]); pfree(values[1]); pfree(values);                                                            \
            SRF_RETURN_NEXT(funcctx, resultdatum);                                                                                                          \
        } else {                                                                                                                                            \
            answer_oids.clear();                                                                                                                            \
            answer_dists.clear();                                                                                                                           \
            SRF_RETURN_DONE(funcctx);                                                                                                                       \
        }                                                                                                                                                   \
    }


    // Importante verificar que essa funcao eh um callback, ou seja, ela fica se chamando 
    // sempre para resolver o problema de retornar multiplas linhas da busca
    #define SIMILARITY_SUPPORT_SLIM_RANGE_CHAR(sql_function_name,hermes_function_name);                                                                     \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                                                                                 \
    Datum sql_function_name(PG_FUNCTION_ARGS) {                                                                                                             \
        /* Estrutura do Postgres, verificar documentacao*/                                                                                                  \
        FuncCallContext *funcctx;                                                                                                                           \
        myResultChar* result = nullptr;                                                                                                                         \
        /* myResult aponta para o resultado da busca*/                                                                                                      \
        bytea *ret_array = PG_GETARG_BYTEA_P(0);                                                                                                            \
        TupleDesc tupdesc;                                                                                                                                  \
        AttInMetadata *attinmeta;                                                                                                                           \
                                                                                                                                                            \
        char **values;                                                                                                                                      \
        HeapTuple tuple;                                                                                                                                    \
        Datum resultdatum;                                                                                                                                  \
                                                                                                                                                            \
        /* Verifica se o callback entrou pela primeira vez*/                                                                                                \
        if (SRF_IS_FIRSTCALL()) {                                                                                                                           \
            /* Inicializa o contexto da funcao */                                                                                                           \
            MemoryContext oldcontext;                                                                                                                       \
            funcctx = SRF_FIRSTCALL_INIT();                                                                                                                 \
            oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);                                                                             \
            if (PG_ARGISNULL(0) || PG_ARGISNULL(1)|| PG_ARGISNULL(2) || PG_ARGISNULL(3) || PG_ARGISNULL(4) || PG_ARGISNULL(5)) {                            \
                ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                              \
            }                                                                                                                                               \
                                                                                                                                                            \
            bytea *byte_array = PG_GETARG_BYTEA_P(0);                                                                                                       \
            float4 radius = PG_GETARG_FLOAT4(1);                                                                                                            \
            text *arg1 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                      \
            text *arg2 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                      \
            text *arg3 = (text *)PG_GETARG_TEXT_PP(4);                                                                                                      \
            text *arg4 = (text *)PG_GETARG_TEXT_PP(5);                                                                                                      \
                                                                                                                                                            \
            text *new_text = concat4(arg1, arg2, arg3, arg4);                                                                                               \
            SignatureChar signature = CharByteArrayToBasicArrayObject(byte_array);                                                                          \
                                                                                                                                                            \
            /* Cria a Slim utilizando o pageManager e convertendo o texto do postgres para string em c(char*) */                                            \
            stSlimTree <SignatureChar, hermes_function_name<SignatureChar>  >* tree = new stSlimTree                                                        \
                    < SignatureChar,  hermes_function_name <SignatureChar> >(DiskManagerSingleton::getInstance().                                           \
                        getPageManager(dup_pgtext(new_text)));                                                                                              \
                                                                                                                                                            \
            /* Realiza a busca com o raio determinado */                                                                                                    \
            result = tree->RangeQuery(&signature, ((float) radius));                                                                                        \
            /* Seta quantas vezes a funcao vai ser chamada = numero de objetos retornados na query*/                                                        \
            funcctx->max_calls = result->GetNumOfEntries();                                                                                                 \
                                                                                                                                                            \
            /* Build a tuple descriptor for our result type */                                                                                              \
            if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)                                                                         \
                ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),                                                                                     \
                                errmsg("Function returning record called in context "                                                                       \
                                        "that cannot accept type record!")));                                                                               \
                                                                                                                                                            \
            /*generate attribute metadata needed later to produce tuples from raw C strings*/                                                               \
            attinmeta = TupleDescGetAttInMetadata(tupdesc);                                                                                                 \
            funcctx->attinmeta = attinmeta;                                                                                                                 \
                                                                                                                                                            \
            /* Itera sobre todos os elementos e armazena na estrutura criada anteriormente*/                                                                \
            /* importante ver aqui que so fazemos a busca na primeira vez que a funcao roda*/                                                               \
            for(auto elem : *(result->GetPairs())) {                                                                                                        \
                answer_oids.push_back(elem->GetObject()->GetOID());                                                                                         \
                answer_dists.push_back(elem->GetDistance());                                                                                                \
            }                                                                                                                                               \
            delete result;                                                                                                                                  \
            delete tree;                                                                                                                                    \
            pfree(new_text);                                                                                                                                \
            result = 0;                                                                                                                                     \
            tree = 0;                                                                                                                                       \
            /* Postgres*/                                                                                                                                   \
            MemoryContextSwitchTo(oldcontext);                                                                                                              \
        }                                                                                                                                                   \
                                                                                                                                                            \
                                                                                                                                                            \
        /* retorna os elementos de fato, e quando acaba o numero de elementos que                                                                           \
        tem q retornar, ele avisa que acabou ao contexto*/                                                                                                  \
        funcctx = SRF_PERCALL_SETUP();                                                                                                                      \
        attinmeta = funcctx->attinmeta;                                                                                                                     \
                                                                                                                                                            \
        if (funcctx->call_cntr < funcctx->max_calls) {                                                                                                      \
                                                                                                                                                            \
            /*Prepare a values array for building the returned tuple.                                                                                       \
            * This should be an array of C strings which will                                                                                               \
            * be processed later by the type input functions.*/                                                                                             \
            values = (char **) palloc(2 * sizeof(char *));                                                                                                  \
            values[0] = (char *) palloc(16 * sizeof(char));                                                                                                \
            values[1] = (char *) palloc(16 * sizeof(char));                                                                                                \
                                                                                                                                                            \
            snprintf(values[0], 16, "%d", answer_oids[funcctx->call_cntr]);                                                                                \
            snprintf(values[1], 16, "%lf", answer_dists[funcctx->call_cntr]);                                                                              \
                                                                                                                                                            \
            /* build a tuple */                                                                                                                             \
            tuple = BuildTupleFromCStrings(attinmeta, values);                                                                                              \
                                                                                                                                                            \
            /* make the tuple into a datum */                                                                                                               \
            resultdatum = HeapTupleGetDatum(tuple);                                                                                                         \
                                                                                                                                                            \
            pfree(values[0]); pfree(values[1]); pfree(values);                                                                                              \
            SRF_RETURN_NEXT(funcctx, resultdatum);                                                                                                          \
        } else {                                                                                                                                            \
            answer_oids.clear();                                                                                                                            \
            answer_dists.clear();                                                                                                                           \
            SRF_RETURN_DONE(funcctx);                                                                                                                       \
        }                                                                                                                                                   \
    }

    #define SIMILARITY_SUPPORT_SLIM_KNN(sql_function_name,hermes_function_name);                                                                            \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                                                                                 \
        Datum sql_function_name(PG_FUNCTION_ARGS){                                                                                                          \
        FuncCallContext *funcctx;                                                                                                                           \
        myResult* result = nullptr;                                                                                                                         \
        bytea *ret_array = PG_GETARG_BYTEA_P(0);                                                                                                            \
        TupleDesc tupdesc;                                                                                                                                  \
        AttInMetadata *attinmeta;                                                                                                                           \
                                                                                                                                                            \
        char **values;                                                                                                                                      \
        HeapTuple tuple;                                                                                                                                    \
        Datum resultdatum;                                                                                                                                  \
                                                                                                                                                            \
        if(SRF_IS_FIRSTCALL()){                                                                                                                             \
            /* Inicializa o contexto da funcao */                                                                                                           \
            MemoryContext oldcontext;                                                                                                                       \
            funcctx = SRF_FIRSTCALL_INIT();                                                                                                                 \
            oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);                                                                             \
                                                                                                                                                            \
            if(PG_ARGISNULL(0) || PG_ARGISNULL(1) || PG_ARGISNULL(2) || PG_ARGISNULL(3) || PG_ARGISNULL(4) || PG_ARGISNULL(5)){                             \
                ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                              \
            }                                                                                                                                               \
            bytea *byte_array = PG_GETARG_BYTEA_P(0);                                                                                                       \
            int32 qtdK = PG_GETARG_INT32(1);                                                                                                                \
            text *arg1 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                      \
            text *arg2 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                      \
            text *arg3 = (text *)PG_GETARG_TEXT_PP(4);                                                                                                      \
            text *arg4 = (text *)PG_GETARG_TEXT_PP(5);                                                                                                      \
            text *new_text = concat4(arg1, arg2, arg3, arg4);                                                                                               \
            Signature signature = ByteArrayToBasicArrayObject(byte_array);                                                                                  \
            /*ereport(INFO, (errmsg("Criando Slim KNN"))); */                                                                                               \
            stSlimTree <Signature, hermes_function_name<Signature> >* tree = new stSlimTree <Signature,                                                     \
                hermes_function_name <Signature> >(DiskManagerSingleton::getInstance().                                                                     \
                getPageManager(dup_pgtext(new_text)));                                                                                                      \
            /*ereport(INFO, (errmsg("Slim criada. Iniciando KNN"))); */                                                                                     \
            /*elog(INFO, "Numero de objetos na Slim: %d\n", tree->GetNumberOfObjects()); */                                                                 \
            result = tree->NearestQuery(&signature, ((int32) qtdK));                                                                                        \
            funcctx->max_calls = result->GetNumOfEntries();                                                                                                 \
                                                                                                                                                            \
            /* Build a tuple descriptor for our result type */                                                                                              \
            if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)                                                                         \
                ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),                                                                                     \
                                errmsg("Function returning record called in context "                                                                       \
                                        "that cannot accept type record!")));                                                                               \
                                                                                                                                                            \
            /*generate attribute metadata needed later to produce tuples from raw C strings*/                                                               \
            attinmeta = TupleDescGetAttInMetadata(tupdesc);                                                                                                 \
            funcctx->attinmeta = attinmeta;                                                                                                                 \
                                                                                                                                                            \
            for(auto elem : *(result->GetPairs())){                                                                                                         \
                answer_oids.push_back(elem->GetObject()->GetOID());                                                                                         \
                answer_dists.push_back(elem->GetDistance());                                                                                                \
                answer_korder.push_back(answer_oids.size());                                                                                                \
            }                                                                                                                                               \
                                                                                                                                                            \
            delete result;                                                                                                                                  \
            delete tree;                                                                                                                                    \
            pfree(new_text);                                                                                                                                \
            result = 0;                                                                                                                                     \
            tree = 0;                                                                                                                                       \
                                                                                                                                                            \
            MemoryContextSwitchTo(oldcontext);                                                                                                              \
        }                                                                                                                                                   \
        funcctx = SRF_PERCALL_SETUP();                                                                                                                      \
        attinmeta = funcctx->attinmeta;                                                                                                                     \
        if(funcctx->call_cntr < funcctx->max_calls){                                                                                                        \
            /*Prepare a values array for building the returned tuple.                                                                                       \
            * This should be an array of C strings which will                                                                                               \
            * be processed later by the type input functions.*/                                                                                             \
            values = (char **) palloc(3 * sizeof(char *));                                                                                                  \
            values[0] = (char *) palloc(16 * sizeof(char));                                                                                                \
            values[1] = (char *) palloc(16 * sizeof(char));                                                                                                \
            values[2] = (char *) palloc(16 * sizeof(char));                                                                                                \
                                                                                                                                                            \
            snprintf(values[0], 16, "%d", answer_oids[funcctx->call_cntr]);                                                                                \
            snprintf(values[1], 16, "%lf", answer_dists[funcctx->call_cntr]);                                                                              \
            snprintf(values[2], 16, "%d", answer_korder[funcctx->call_cntr]);                                                                              \
                                                                                                                                                            \
            /* build a tuple */                                                                                                                             \
            tuple = BuildTupleFromCStrings(attinmeta, values);                                                                                              \
                                                                                                                                                            \
            /* make the tuple into a datum */                                                                                                               \
            resultdatum = HeapTupleGetDatum(tuple);                                                                                                         \
                                                                                                                                                            \
            pfree(values[0]); pfree(values[1]); pfree(values[2]); pfree(values);                                                                                              \
            SRF_RETURN_NEXT(funcctx, resultdatum);                                                                                                          \
        }else {                                                                                                                                             \
            answer_oids.clear();                                                                                                                            \
            answer_dists.clear();                                                                                                                           \
            answer_korder.clear();                                                                                                                          \
            SRF_RETURN_DONE(funcctx);                                                                                                                       \
        }                                                                                                                                                   \
    }   


    #define SIMILARITY_SUPPORT_SLIM_KNN_CHAR(sql_function_name,hermes_function_name);                                                                       \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                                                                                 \
        Datum sql_function_name(PG_FUNCTION_ARGS){                                                                                                          \
        FuncCallContext *funcctx;                                                                                                                           \
        myResultChar* result = nullptr;                                                                                                                         \
        bytea *ret_array = PG_GETARG_BYTEA_P(0);                                                                                                            \
        TupleDesc tupdesc;                                                                                                                                  \
        AttInMetadata *attinmeta;                                                                                                                           \
                                                                                                                                                            \
        char **values;                                                                                                                                      \
        HeapTuple tuple;                                                                                                                                    \
        Datum resultdatum;                                                                                                                                  \
                                                                                                                                                            \
        if(SRF_IS_FIRSTCALL()){                                                                                                                             \
            /* Inicializa o contexto da funcao */                                                                                                           \
            MemoryContext oldcontext;                                                                                                                       \
            funcctx = SRF_FIRSTCALL_INIT();                                                                                                                 \
            oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);                                                                             \
                                                                                                                                                                \
            if(PG_ARGISNULL(0) || PG_ARGISNULL(1) || PG_ARGISNULL(2) || PG_ARGISNULL(3) || PG_ARGISNULL(4) || PG_ARGISNULL(5)){                                 \
                ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                                  \
            }                                                                                                                                                   \
            bytea *byte_array = PG_GETARG_BYTEA_P(0);                                                                                                           \
            int32 qtdK = PG_GETARG_INT32(1);                                                                                                                    \
            text *arg1 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                          \
            text *arg2 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                          \
            text *arg3 = (text *)PG_GETARG_TEXT_PP(4);                                                                                                          \
            text *arg4 = (text *)PG_GETARG_TEXT_PP(5);                                                                                                          \
            text *new_text = concat4(arg1, arg2, arg3, arg4);                                                                                                   \
            SignatureChar signature = CharByteArrayToBasicArrayObject(byte_array);                                                                              \
            /*ereport(INFO, (errmsg("Criando Slim KNN"))); */                                                                                                   \
            stSlimTree <SignatureChar, hermes_function_name<SignatureChar> >* tree = new stSlimTree <SignatureChar,                                             \
                hermes_function_name <SignatureChar> >(DiskManagerSingleton::getInstance().                                                                     \
                getPageManager(dup_pgtext(new_text)));                                                                                                          \
            /*ereport(INFO, (errmsg("Slim criada. Iniciando KNN"))); */                                                                                         \
            /*elog(INFO, "Numero de objetos na Slim: %d\n", tree->GetNumberOfObjects()); */                                                                     \
            result = tree->NearestQuery(&signature, ((int32) qtdK));                                                                                            \
            funcctx->max_calls = result->GetNumOfEntries();                                                                                                     \
                                                                                                                                                                \
            /* Build a tuple descriptor for our result type */                                                                                                  \
            if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)                                                                             \
                ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),                                                                                         \
                                errmsg("Function returning record called in context "                                                                           \
                                        "that cannot accept type record!")));                                                                                   \
                                                                                                                                                                \
            /*generate attribute metadata needed later to produce tuples from raw C strings*/                                                                   \
            attinmeta = TupleDescGetAttInMetadata(tupdesc);                                                                                                     \
            funcctx->attinmeta = attinmeta;                                                                                                                     \
                                                                                                                                                                \
            for(auto elem : *(result->GetPairs())){                                                                                                             \
                answer_oids.push_back(elem->GetObject()->GetOID());                                                                                             \
                answer_dists.push_back(elem->GetDistance());                                                                                                    \
                answer_korder.push_back(answer_oids.size());                                                                                                    \
            }                                                                                                                                                   \
                                                                                                                                                                \
            delete result;                                                                                                                                      \
            delete tree;                                                                                                                                        \
            pfree(new_text);                                                                                                                                    \
            result = 0;                                                                                                                                         \
            tree = 0;                                                                                                                                           \
                                                                                                                                                                \
            MemoryContextSwitchTo(oldcontext);                                                                                                                  \
        }                                                                                                                                                       \
        funcctx = SRF_PERCALL_SETUP();                                                                                                                          \
        attinmeta = funcctx->attinmeta;                                                                                                                         \
        if(funcctx->call_cntr < funcctx->max_calls){                                                                                                            \
            /*Prepare a values array for building the returned tuple.                                                                                           \
            * This should be an array of C strings which will                                                                                                   \
            * be processed later by the type input functions.*/                                                                                                 \
            values = (char**) palloc(3 * sizeof(char*));                                                                                                        \
            values[0] = (char *) palloc(16 * sizeof(char));                                                                                                     \
            values[1] = (char *) palloc(16 * sizeof(char));                                                                                                     \
            values[2] = (char *) palloc(16 * sizeof(char));                                                                                                     \
                                                                                                                                                                \
            snprintf(values[0], 16, "%d", answer_oids[funcctx->call_cntr]);                                                                                     \
            snprintf(values[1], 16, "%lf", answer_dists[funcctx->call_cntr]);                                                                                   \
            snprintf(values[2], 16, "%d", answer_korder[funcctx->call_cntr]);                                                                                   \
                                                                                                                                                                \
            /* build a tuple */                                                                                                                                 \
            tuple = BuildTupleFromCStrings(attinmeta, values);                                                                                                  \
                                                                                                                                                                \
            /* make the tuple into a datum */                                                                                                                   \
            resultdatum = HeapTupleGetDatum(tuple);                                                                                                             \
                                                                                                                                                                \
            pfree(values[0]); pfree(values[1]); pfree(values[2]); pfree(values);                                                                                \
            SRF_RETURN_NEXT(funcctx, resultdatum);                                                                                                              \
        }else {                                                                                                                                                 \
            answer_oids.clear();                                                                                                                                \
            answer_dists.clear();                                                                                                                               \
            answer_korder.clear();                                                                                                                              \
            SRF_RETURN_DONE(funcctx);                                                                                                                           \
        }                                                                                                                                                       \
    } 



    // Exemplo 1 SPI: https://www.highgo.ca/2020/07/24/the-spi-feature-under-postgresql-kernel/
    // Exemplo 2 SPI: https://web.mit.edu/cygwin/cygwin_v1.3.2/usr/doc/postgresql-7.1.2/html/spi-examples.html
    // Referência código Postgres: https://doxygen.postgresql.org/struct__SPI__plan.html
    // Documentação SPI: https://www.postgresql.org/docs/13/spi.html

    // Basicamente imprime as informacoes sobre a Slim
    // Basicamente imprime as informacoes sobre a Slim
    //PG_FUNCTION_INFO_V1(print_statistics);                                                                                                                 
    //Datum print_statistics(PG_FUNCTION_ARGS) {                                                                                                             
    /*#define PRINT_STATISTICS_MODEL(sql_function_name,hermes_function_name);                                                                         \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                                                                         \
    Datum sql_function_name(PG_FUNCTION_ARGS) {                                                                                                     \
        if (PG_ARGISNULL(0) || PG_ARGISNULL(1)|| PG_ARGISNULL(2) || PG_ARGISNULL(3)) {                                                              \
            ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                          \
        }                                                                                                                                           \
        text *arg1 = (text *)PG_GETARG_TEXT_PP(0);                                                                                                  \
        text *arg2 = (text *)PG_GETARG_TEXT_PP(1);                                                                                                  \
        text *arg3 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                  \
        text *arg4 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                  \
                                                                                                                                                    \
        char * str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));                                                                                        \
        char * str2 = text_to_cstring(PG_GETARG_TEXT_PP(1));                                                                                        \
        char * str3 = text_to_cstring(PG_GETARG_TEXT_PP(2));                                                                                        \
        char * str4 = text_to_cstring(PG_GETARG_TEXT_PP(3));                                                                                        \
                                                                                                                                                    \
        text *new_text = concat4(arg1, arg2, arg3, arg4);                                                                                           \
                                                                                                                                                    \
        //tests if the tree exists in fact - query on similarql_indexes table                                                                     \
        char query[1000] = "\0";                                                                                                                    \
        strcat(query, "SELECT * FROM similarql_indexes WHERE name ='");                                                                             \
        strcat(query, str1);                                                                                                                        \
        strcat(query, "' AND idx_table = '");                                                                                                       \
        strcat(query, str2);                                                                                                                        \
        strcat(query, "' AND attrib_complex = '");                                                                                                  \
        strcat(query, str3);                                                                                                                        \
        strcat(query, "' AND  distance_function = '");                                                                                              \
        strcat(query, str4);                                                                                                                        \
        strcat(query, "'");                                                                                                                         \
                                                                                                                                                    \
        SPI_connect();                                                                                                                              \
                                                                                                                                                    \
        int result = SPI_execute(query, true, 1);                                                                                                   \
                                                                                                                                                    \
        int proc = SPI_processed;                                                                                                                   \
                                                                                                                                                    \
        if ( result > 0 && SPI_tuptable != NULL ) {                                                                                                 \
            SPITupleTable *tuptable = SPI_tuptable;                                                                                                 \
            //TupleDesc tupdesc = tuptable->tupdesc;                                                                                                \
                                                                                                                                                    \
            //There are at least one tuple on similarql_indexes                                                                                     \
            if(tuptable->numvals > 0) {                                                                                                             \
                stSlimTree <Signature, hermes_function_name<Signature>  >* tree = new stSlimTree < Signature, hermes_function_name<Signature> >(DiskManagerSingleton::getInstance().getPageManager(dup_pgtext(new_text)));                       \
                                                                                                                                                    \
                elog(NOTICE, "Number of objects on Slim-tree: %d", tree->GetNumberOfObjects());                                                     \
                elog(NOTICE, "Number of Slim-tree nodes: %d", tree->GetNodeCount());                                                                \
                elog(NOTICE, "Number of leaves on Slim-tree: %d", tree->GetLeafNodeCount());                                                        \
                elog(NOTICE, "Number of index nodes on Slim-tree: %d", tree->GetIndexNodeCount());                                                  \
                elog(NOTICE, "Height of the Slim-tree: %d", tree->GetHeight());                                                                     \
                                                                                                                                                    \
                                                                                                                                                    \
                delete tree;                                                                                                                        \
            }                                                                                                                                       \
            //There are not Slim-trees on similarql_indexes with the arguments received                                                             \
            else {                                                                                                                                  \
                elog(NOTICE, "There are not Slim-trees with these parameters!");                                                                    \
                                                                                                                                                    \
            }                                                                                                                                       \
        }                                                                                                                                           \
                                                                                                                                                    \
        SPI_finish()                                                                                                                               \
                                                                                                                                                    \
        PG_RETURN_NULL();                                                                                                                           \
    }*/


    PG_FUNCTION_INFO_V1(print_statistics);                                                                                                                 
    Datum print_statistics(PG_FUNCTION_ARGS) {                                                                                                             
        if (PG_ARGISNULL(0) || PG_ARGISNULL(1)|| PG_ARGISNULL(2) || PG_ARGISNULL(3)) {                                                   
            ereport(ERROR, (errmsg("Null parameters are not accepted.")));                                                                                  
        }                                                                                                                                                   
        text *arg1 = (text *)PG_GETARG_TEXT_PP(0);                                                                                                           
        text *arg2 = (text *)PG_GETARG_TEXT_PP(1);                                                                                                           
        text *arg3 = (text *)PG_GETARG_TEXT_PP(2);                                                                                                           
        text *arg4 = (text *)PG_GETARG_TEXT_PP(3);                                                                                                           
        
        char * str1 = text_to_cstring(PG_GETARG_TEXT_PP(0));
        char * str2 = text_to_cstring(PG_GETARG_TEXT_PP(1));
        char * str3 = text_to_cstring(PG_GETARG_TEXT_PP(2));
        char * str4 = text_to_cstring(PG_GETARG_TEXT_PP(3));

        text *new_text = concat4(arg1, arg2, arg3, arg4); 
        
        /*queries if the tree exists in fact - query on similarql_indexes table*/
        char query[1000] = "\0";
        strcat(query, "SELECT * FROM similarql_indexes WHERE name ='");
        strcat(query, str1);
        strcat(query, "' AND idx_table = '");
        strcat(query, str2);
        strcat(query, "' AND attrib_complex = '");
        strcat(query, str3);
        strcat(query, "' AND  distance_function = '");
        strcat(query, str4);
        strcat(query, "'");
        
        SPI_connect();

        int result = SPI_execute(query, true, 1);

        int proc = SPI_processed;

        if ( result > 0 && SPI_tuptable != NULL )
        {
            SPITupleTable *tuptable = SPI_tuptable;
            
            //There are at least one tuple on similarql_indexes
            if(tuptable->numvals > 0) 
            {
                //It uses the EuclideanDistance as default, but it does not matter, because the 
                //important thing is to get informations about the file.
                //So, it works with other distance functions as well
                stSlimTree <Signature, EuclideanDistance<Signature>  >* tree = new stSlimTree < Signature,  
                    EuclideanDistance<Signature> >(DiskManagerSingleton::getInstance().getPageManager(dup_pgtext(new_text)));

                elog(NOTICE, "Number of objects on Slim-tree: %d", tree->GetNumberOfObjects());
                elog(NOTICE, "Number of Slim-tree nodes: %d", tree->GetNodeCount());
                elog(NOTICE, "Number of leaves on Slim-tree: %d", tree->GetLeafNodeCount());
                elog(NOTICE, "Number of index nodes on Slim-tree: %d", tree->GetIndexNodeCount());
                elog(NOTICE, "Height of the Slim-tree: %d", tree->GetHeight());

 
                delete tree;                                                                                                                                        
            }
            //There are not Slim-trees on similarql_indexes with the arguments received
            else
                elog(NOTICE, "There are not Slim-trees with these parameters!");
            
        }
        
        SPI_finish();
        
        pfree(str1); pfree(str2); pfree(str3); pfree(str4);
        //elog(NOTICE, "Passou dos free");

        PG_RETURN_NULL();                                                                                                                                
    }
    
/**
 Descomentar as linhas para dar suporte as outras funcoes de Distancia 
 **/

    SIMILARITY_SUPPORT_SLIM_CREATE(slim_euclidean_distance_create, EuclideanDistance);
//    SIMILARITY_SUPPORT_SLIM_CREATE(slim_create, EuclideanDistance);
    SIMILARITY_SUPPORT_SLIM_CREATE(slim_manhattan_distance_create,ManhattanDistance);
    SIMILARITY_SUPPORT_SLIM_CREATE(slim_chebyshev_distance_create,ChebyshevDistance);
    SIMILARITY_SUPPORT_SLIM_CREATE_CHAR(slim_ledit_distance_create,LEditDistance);
//    SIMILARITY_SUPPORT_SLIM_CREATE(slim_canberra_distance_create,CanberraDistance);
//    SIMILARITY_SUPPORT_SLIM_CREATE(slim_kullback_leibler_divergence_distance_create,KullbackLeiblerDivergenceDistance);
//    SIMILARITY_SUPPORT_SLIM_CREATE(slim_jeffrey_divergence_distance_create,JeffreyDivergenceDistance);

    SIMILARITY_SUPPORT_SLIM_ADD(slim_manhattan_distance_add,ManhattanDistance);
    SIMILARITY_SUPPORT_SLIM_ADD(slim_euclidean_distance_add,EuclideanDistance);
    SIMILARITY_SUPPORT_SLIM_ADD(slim_chebyshev_distance_add,ChebyshevDistance);
    SIMILARITY_SUPPORT_SLIM_ADD_CHAR(slim_ledit_distance_add,LEditDistance);
//    SIMILARITY_SUPPORT_SLIM_ADD(slim_canberra_distance_add,CanberraDistance);
//    SIMILARITY_SUPPORT_SLIM_ADD(slim_kullback_leibler_divergence_distance_add,KullbackLeiblerDivergenceDistance);
//    SIMILARITY_SUPPORT_SLIM_ADD(slim_jeffrey_divergence_distance_add,JeffreyDivergenceDistance);

    SIMILARITY_SUPPORT_SLIM_KNN(slim_euclidean_distance_knn, EuclideanDistance);
    SIMILARITY_SUPPORT_SLIM_KNN(slim_manhattan_distance_knn, ManhattanDistance);
    SIMILARITY_SUPPORT_SLIM_KNN(slim_chebyshev_distance_knn, ChebyshevDistance);
    SIMILARITY_SUPPORT_SLIM_KNN_CHAR(slim_ledit_distance_knn, LEditDistance);
    SIMILARITY_SUPPORT_SLIM_RANGE(slim_chebyshev_distance_range,ChebyshevDistance);
    SIMILARITY_SUPPORT_SLIM_RANGE(slim_manhattan_distance_range,ManhattanDistance);
    SIMILARITY_SUPPORT_SLIM_RANGE(slim_euclidean_distance_range,EuclideanDistance);
    SIMILARITY_SUPPORT_SLIM_RANGE_CHAR(slim_ledit_distance_range,LEditDistance);
//    SIMILARITY_SUPPORT_SLIM_RANGE(slim_canberra_distance_range,CanberraDistance);
//    SIMILARITY_SUPPORT_SLIM_RANGE(slim_kullback_leibler_divergence_distance_range,KullbackLeiblerDivergenceDistance);
//    SIMILARITY_SUPPORT_SLIM_RANGE(slim_jeffrey_divergence_distance_range,JeffreyDivergenceDistance);

    //PRINT_STATISTICS_MODEL(print_statistics_euclidean_distance, EuclideanDistance);

#ifdef __cplusplus
}
#endif

// Converte um bytea, que eh o binario do vetor de caracteristicas no Postgres para o vetor de caracteristicas que utilizamos em C++
// bytea -> BasicArrayObject
/*Signature ByteArrayToBasicArrayObject(bytea *byte_array) {
    vector<SignatureElementType> signature;

    int byte_size = VARSIZE(byte_array) - VARHDRSZ;
    SignatureElementType *data = (SignatureElementType *)(palloc(byte_size));

    memcpy(data, VARDATA(byte_array), byte_size);

    int signature_size = byte_size / sizeof(SignatureElementType);
    for (int i = 0; i < signature_size; ++i) {
        signature.push_back(data[i]);
    }

    return Signature(0, signature);
}*/
Signature ByteArrayToBasicArrayObject(bytea *byte_array) {
    //vector<SignatureElementType> signature;
    Signature s;

    int byte_size = VARSIZE(byte_array) - VARHDRSZ;
    //SignatureElementType *data = (SignatureElementType *)(palloc(byte_size));
    u_char *vetor_completo = (u_char *)(palloc(byte_size));
    SET_VARSIZE(vetor_completo, byte_size+VARHDRSZ);
    //memcpy(data, VARDATA(byte_array), byte_size);
    memcpy(vetor_completo, VARDATA(byte_array), byte_size);

    /*int signature_size = byte_size / sizeof(SignatureElementType);
    for (int i = 0; i < signature_size; ++i) {
        signature.push_back(data[i]);
    }*/
    s.unserialize(vetor_completo);
    pfree(vetor_completo);
    
    //return Signature(0, signature);
    return s;
}

SignatureChar CharByteArrayToBasicArrayObject(bytea *byte_array) {
    //vector<SignatureElementType> signature;
    SignatureChar s;

    int byte_size = VARSIZE(byte_array) - VARHDRSZ;
    //SignatureElementType *data = (SignatureElementType *)(palloc(byte_size));
    u_char *vetor_completo = (u_char *)(palloc(byte_size));
    SET_VARSIZE(vetor_completo, byte_size+VARHDRSZ);
    //memcpy(data, VARDATA(byte_array), byte_size);
    memcpy(vetor_completo, VARDATA(byte_array), byte_size);

    /*int signature_size = byte_size / sizeof(SignatureElementType);
    for (int i = 0; i < signature_size; ++i) {
        signature.push_back(data[i]);
    }*/
    s.unserialize(vetor_completo);
    pfree(vetor_completo);

    //return Signature(0, signature);
    return s;
}


// Concatena 4 text do Postgres em um novo text
// Precisamos disso para criacao do pagemanager da slim
// importante ver que cada vez que fazemos o calculo com VARHDRSZ precisamos recalcular com o mesmo
// VARHDRSZ eh um header que todo objeto do postgres no c++ tem, entao se quisermos somente o conteudo do objeto, precisamos retirar no memcpy
text* concat4(text *arg1, text *arg2, text *arg3, text *arg4) {
    int32 arg1_size = VARSIZE_ANY_EXHDR(arg1);
    int32 arg2_size = VARSIZE_ANY_EXHDR(arg2);
    int32 arg3_size = VARSIZE_ANY_EXHDR(arg3);
    int32 arg4_size = VARSIZE_ANY_EXHDR(arg4);

    int32 new_text_size =  arg1_size + arg2_size + arg3_size + arg4_size + VARHDRSZ;
    // palloc eh um malloc do postgres
    text *new_text = (text *) palloc(new_text_size);

    SET_VARSIZE(new_text, new_text_size);

    memcpy(VARDATA(new_text), VARDATA_ANY(arg1), arg1_size);
    memcpy(VARDATA(new_text) + arg1_size, VARDATA_ANY(arg2), arg2_size);
    memcpy(VARDATA(new_text) + arg1_size + arg2_size, VARDATA_ANY(arg3), arg3_size);
    memcpy(VARDATA(new_text) + arg1_size + arg2_size + arg3_size, VARDATA_ANY(arg4), arg4_size);

    return new_text;
}


// // Concatena 4 text do Postgres em um novo text
// // Precisamos disso para criacao do pagemanager da slim
// // importante ver que cada vez que fazemos o calculo com VARHDRSZ precisamos recalcular com o mesmo
// // VARHDRSZ eh um header que todo objeto do postgres no c++ tem, entao se quisermos somente o conteudo do objeto, precisamos retirar no memcpy
// text* concat4(text *arg1, text *arg2, text *arg3, text *arg4) {
//     int32 new_text_size = VARSIZE(arg1) + VARSIZE(arg2) + VARSIZE(arg3) + VARSIZE(arg4) - 3 * VARHDRSZ;
//     // palloc eh um malloc do postgres
//     text *new_text = (text *) palloc(new_text_size);\

//     SET_VARSIZE(new_text, new_text_size);

//     memcpy(VARDATA(new_text), VARDATA(arg1), VARSIZE(arg1) - VARHDRSZ);
//     memcpy(VARDATA(new_text) + (VARSIZE(arg1) - VARHDRSZ),  VARDATA(arg2), VARSIZE(arg2) - VARHDRSZ);
//     memcpy(VARDATA(new_text) + (VARSIZE(arg1) + VARSIZE(arg2) - 2*VARHDRSZ), VARDATA(arg3), VARSIZE(arg3) - VARHDRSZ);
//     memcpy(VARDATA(new_text) + (VARSIZE(arg1) + VARSIZE(arg2) + VARSIZE(arg3) - 3*VARHDRSZ), VARDATA(arg4), VARSIZE(arg4) - VARHDRSZ);
//     return new_text;
// }

// Converte o text do Postgres para um tipo de string em C, isso se da pelo fato de haver lixo se tentarmos fazer somente um cast do tipo text
char *dup_pgtext(text *what)
{
    size_t len = VARSIZE(what)-VARHDRSZ;
    char *dup = (char *) palloc(len + 1);
    memcpy(dup, VARDATA(what), len);
    //dup[len] = 0;
    dup[len] = '\0';
    return dup;
}
