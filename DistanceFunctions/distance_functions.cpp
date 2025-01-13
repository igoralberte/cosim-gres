#include "distance_functions.h"
#include <typeinfo>


bytea * Signature2ByteArray(Signature& imgSignature){
    // allocs space for byte array
    bytea * outputBytes = (bytea *) palloc(imgSignature.getSerializedSize() * sizeof(bytea) + VARHDRSZ);
    //Set the bytea size value, which is requeried by postgres bytea type
    SET_VARSIZE(outputBytes, VARHDRSZ + imgSignature.getSerializedSize());

    //copy from local variable memory to the returning varible memory
    memcpy(VARDATA(outputBytes), imgSignature.serialize(), imgSignature.getSerializedSize());

    // Return final bytea
    return outputBytes;
}

bytea * CharSignature2ByteArray(SignatureChar& imgSignature){
    // allocs space for byte array
    bytea * outputBytes = (bytea *) palloc(imgSignature.getSerializedSize() * sizeof(bytea) + VARHDRSZ);
    //Set the bytea size value, which is requeried by postgres bytea type
    SET_VARSIZE(outputBytes, VARHDRSZ + imgSignature.getSerializedSize());

    //copy from local variable memory to the returning varible memory
    memcpy(VARDATA(outputBytes), imgSignature.serialize(), imgSignature.getSerializedSize());

    // Return final bytea
    return outputBytes;
}

#ifdef __cplusplus
extern "C" {
#endif

    #ifdef PG_MODULE_MAGIC
        PG_MODULE_MAGIC;
    #endif

    PG_FUNCTION_INFO_V1(hamming_distance);

    Datum hamming_distance(PG_FUNCTION_ARGS) {
        if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) {
            ereport(ERROR, (errmsg("Null parameters are not accepted")));
        }

        bytea *x = PG_GETARG_BYTEA_P(0);
        bytea *y = PG_GETARG_BYTEA_P(1);

        float8 result = hammingDistance(x, y);

        PG_RETURN_FLOAT8(result);
    }

    // This macro is a skelleton function to support hermes functions
    #define SIMILARITY_SUPPORT_HERMES_FUNCTION(sql_function_name,hermes_function_name);     \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                 \
    Datum sql_function_name(PG_FUNCTION_ARGS) {                                             \
        if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) {                                           \
            ereport(ERROR, (errmsg("Null parameters are not accepted.")));                  \
        }                                                                                   \
        /* get args */                                                                      \
        bytea *byte_array0 = PG_GETARG_BYTEA_P(0);                                          \
        bytea *byte_array1 = PG_GETARG_BYTEA_P(1);                                          \
        /* convert bytea to signature */                                                    \
        Signature signature0 = ByteArrayToBasicArrayObject(byte_array0);                    \
                                                                                            \
                                                                                            \
        Signature signature1 = ByteArrayToBasicArrayObject(byte_array1);                    \
                                                                                            \
        SignatureElementType aux;                                                           \
                                                                                            \
        string type = typeid(aux).name();                                                   \
                                                                                            \
                                                                                            \
        hermes_function_name<Signature> function_name;                                      \
        float8 result;                                                                      \
        try {                                                                               \
            result = function_name.getDistance(signature0, signature1);                     \
        } catch (std::length_error) {                                                       \
            ereport(ERROR, (errmsg("Signatures must have the same length.")));              \
        }                                                                                   \
        PG_RETURN_FLOAT8(result);                                                           \
    }   

    #define SIMILARITY_SUPPORT_HERMES_FUNCTION_CHAR(sql_function_name,hermes_function_name);     \
    PG_FUNCTION_INFO_V1(sql_function_name);                                                 \
    Datum sql_function_name(PG_FUNCTION_ARGS) {                                             \
        if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) {                                           \
            ereport(ERROR, (errmsg("Null parameters are not accepted.")));                  \
        }                                                                                   \
        /* get args */                                                                      \
        bytea *byte_array0 = PG_GETARG_BYTEA_P(0);                                          \
        bytea *byte_array1 = PG_GETARG_BYTEA_P(1);                                          \
        /* convert bytea to signature */                                                    \
        SignatureChar signature0 = CharByteArrayToBasicArrayObject(byte_array0);            \
                                                                                            \
                                                                                            \
        SignatureChar signature1 = CharByteArrayToBasicArrayObject(byte_array1);                    \
                                                                                            \
        SignatureElementTypeChar aux;                                                           \
                                                                                            \
        string type = typeid(aux).name();                                                   \
                                                                                            \
                                                                                            \
        hermes_function_name<SignatureChar> function_name;                                      \
        float8 result;                                                                      \
        try {                                                                               \
            result = function_name.getDistance(signature0, signature1);                     \
        } catch (std::length_error) {                                                       \
            ereport(ERROR, (errmsg("Signatures must have the same length.")));              \
        }                                                                                   \
        PG_RETURN_FLOAT8(result);                                                           \
    }  

    SIMILARITY_SUPPORT_HERMES_FUNCTION(manhattan_distance,ManhattanDistance);
    SIMILARITY_SUPPORT_HERMES_FUNCTION(euclidean_distance,EuclideanDistance);
    SIMILARITY_SUPPORT_HERMES_FUNCTION(chebyshev_distance,ChebyshevDistance);
    SIMILARITY_SUPPORT_HERMES_FUNCTION(canberra_distance,CanberraDistance);
    SIMILARITY_SUPPORT_HERMES_FUNCTION(kullback_leibler_divergence_distance,KullbackLeiblerDivergenceDistance);
    SIMILARITY_SUPPORT_HERMES_FUNCTION(jeffrey_divergence_distance,JeffreyDivergenceDistance);
    SIMILARITY_SUPPORT_HERMES_FUNCTION_CHAR(ledit_distance, LEditDistance);



 PG_FUNCTION_INFO_V1(text_to_signature);                                                         
    Datum text_to_signature(PG_FUNCTION_ARGS) {                                                     
        if(PG_ARGISNULL(0)) {                                                                       
            ereport(ERROR, (errmsg("[ERROR] Null array not accepted.\n")));                         
        }                                                                                           
        try                                                                                         
        {   
                        
            /*VERSÃO FLOAT -----------------------*/
            //----------------------------------VERSÃO ATUAL                                                                                        
            Signature signature;
            SignatureElementType n;                                                                 
            //artemis_extractor_name<Signature,Image> *descriptor;                                    
            //// ByteArray to image                                                                 
            //bytea * inputByteArray = PG_GETARG_BYTEA_P(0);                                          
            text* inputByteArray = PG_GETARG_TEXT_PP(0);                                          
            ////Image * image = ByteArray2Image(inputByteArray);                                        
            //// Extract                                                                           
            //descriptor = new artemis_extractor_name<Signature, Image>();                            
            //descriptor->generateSignature(*image, imgSignature);

            //text *destination = (text *) palloc(VARHDRSZ + 40);
            //destination->length = VARHDRSZ + 40;
            //memcpy(destination->data, buffer, 40);

            //VARDATA(inputByteArray)
            //double n = 0;
            //SignatureElementType n = 0;

            //char* str = VARDATA(inputByteArray);
            //char * str = (char *)palloc(VARSIZE(inputByteArray) - VARHDRSZ);
            char * str = (char *)palloc(VARSIZE_ANY_EXHDR(inputByteArray) - VARHDRSZ + 1);
            //SET_VARSIZE(str, VARSIZE(inputByteArray)-VARHDRSZ);
            SET_VARSIZE(str, VARSIZE_ANY_EXHDR(inputByteArray) - VARHDRSZ + 1);

            //memcpy(str, VARDATA(inputByteArray), VARSIZE(inputByteArray) - VARHDRSZ);
            memcpy(str, (void *)VARDATA(inputByteArray), VARSIZE_ANY_EXHDR(inputByteArray));
            str[VARSIZE_ANY_EXHDR(inputByteArray)] = '\0';
            ////elog(WARNING, "str: %s", str);i

            //----------------------------------FIM VERSÃO ATUAL
            char * pch = 0;
            pch = strtok (str," \t");
            signature.SetOID(0);
            while (pch != NULL)
            {
                //elog(WARNING, "x: %s",str);
                n = atof(pch);
                //elog(WARNING,"b %f ***%s***",n, pch);
                signature.set(n);
                //pch = strtok(NULL," \t");
                pch = strtok(NULL," ");
            }

            //// Signature to ByteArray                                                             
            bytea * characteristics = Signature2ByteArray(signature);

            pfree(str);                            
            free(pch);
            //delete signature;

            PG_RETURN_BYTEA_P(characteristics);                                                             
            
            ////ereport(WARNING,(errcode(ERRCODE_WARNING), errmsg("This is a warning message")));

        }                                                                                           
        catch(exception& e)                                                                         
        {                                                                                           
            ereport(ERROR,                                                                          
                (errmsg("[ERROR] in %s: %s\n", "text_to_signature", e.what() )));              
        }                                                                                           
    }

    PG_FUNCTION_INFO_V1(text_to_signature_char);                                                         
    Datum text_to_signature_char(PG_FUNCTION_ARGS) {                                                     
        if(PG_ARGISNULL(0)) {                                                                       
            ereport(ERROR, (errmsg("[ERROR] Null array not accepted.\n")));                         
        }                                                                                           
        try                                                                                         
        {   
            
            /*VERSÃO CHAR ------------------------*/
            SignatureChar signature;
            text* inputByteArray = (text*) PG_GETARG_TEXT_PP(0);

            int32 byte_size = VARSIZE_ANY_EXHDR(inputByteArray);
            text * str = (text *)palloc(255 + VARHDRSZ);
            //SET_VARSIZE(str, VARSIZE(inputByteArray)-VARHDRSZ);
                            

            //memcpy(str, VARDATA(inputByteArray), VARSIZE(inputByteArray) - VARHDRSZ);
            //memcpy(str, (void *)VARDATA(inputByteArray), VARSIZE_ANY_EXHDR(inputByteArray));
            //-----VERSÃO IGOR

            SET_VARSIZE(str, 255 + VARHDRSZ);
            
            memcpy(VARDATA(str), VARDATA_ANY(inputByteArray), byte_size);
            
            string pch(VARDATA(str));
            
            // copy the input string to a vector
            vector<char> v;
            for(int i=0; i<255; i++) {
                if(i < byte_size) 
                    v.push_back(pch[i]);
                else {
                    if (i == byte_size)
                        v.push_back('\0'); //'=' indicates the end of the string
                    else
                        v.push_back('=');
                }
            }
            
            // copy the characters to the signature array
            signature.SetOID(0);
            for(int i=0; i<v.size(); i++) {
                //elog(WARNING,"b ***%c***",v[i]);
                signature.set(v[i]);
            }
            
            //// Signature to ByteArray                                                             
            bytea * characteristics = CharSignature2ByteArray(signature);

            pfree(str);
            v.clear();
            //delete signature;
            
            PG_RETURN_BYTEA_P(characteristics);                                                     
            
            
            ////ereport(WARNING,(errcode(ERRCODE_WARNING), errmsg("This is a warning message")));

        }                                                                                           
        catch(exception& e)                                                                         
        {                                                                                           
            ereport(ERROR,                                                                          
                (errmsg("[ERROR] in %s: %s\n", "text_to_signature", e.what() )));              
        }                                                                                           
    }


    PG_FUNCTION_INFO_V1(signature_to_text);                                                         
    Datum signature_to_text(PG_FUNCTION_ARGS) {                                                     
        if(PG_ARGISNULL(0)) {                                                                       
            ereport(ERROR, (errmsg("[ERROR] Null array not accepted.\n")));                         
        }                                                                                           
        try                                                                                         
        {                                                                                           
            Signature signature;                                                                 
            

            bytea *byte_array0 = PG_GETARG_BYTEA_P(0);  
            Signature signature0 = ByteArrayToBasicArrayObject(byte_array0);

            //Serve para conversão signature -> text
            std::ostringstream strs;


            std::string outputStr = "";
            //double value;
            SignatureElementType value;

            for(int i = 0; i < signature0.GetSize(); i++){
                if(i) outputStr += " ";
                //if(i) outputStr += "\t";

                strs.str("");
                strs << signature0.getData()[i];

                //outputStr += std::to_string(signature0.get(i));
                outputStr += strs.str();
                //ereport(INFO, (errmsg("%s", strs.str().c_str())));

            }

            //text *destination = (text *) palloc(VARHDRSZ + outputStr.length()+1);
            text *destination = (text *) palloc(VARHDRSZ + outputStr.length());
            //SET_VARSIZE(destination, VARHDRSZ + outputStr.length()+1);
            SET_VARSIZE(destination, VARHDRSZ + outputStr.length());
            //memcpy(destination, outputStr.c_str(), outputStr.length()+1);
            memcpy(VARDATA(destination), outputStr.c_str(), outputStr.length());

            outputStr.clear();
            PG_RETURN_TEXT_P(destination);                                                     
        }                                                                                           
        catch(exception& e)                                                                         
        {                                                                                           
            ereport(ERROR,                                                                          
                (errmsg("[ERROR] in %s: %s\n", "signature_to_text", e.what() )));              
        }                                                                                           
    }


    PG_FUNCTION_INFO_V1(signature_to_text_char);                                                         
    Datum signature_to_text_char(PG_FUNCTION_ARGS) {                                                     
        if(PG_ARGISNULL(0)) {                                                                       
            ereport(ERROR, (errmsg("[ERROR] Null array not accepted.\n")));                         
        }                                                                                           
        try                                                                                         
        {                                                                                           
            SignatureChar signature;                                                                 
            

            bytea *byte_array0 = PG_GETARG_BYTEA_P(0);  
            SignatureChar signature0 = CharByteArrayToBasicArrayObject(byte_array0);

            //Serve para conversão signature -> text
            std::ostringstream strs;

            //ereport(INFO, (errmsg("Tamanho do sig0: %d", signature0.GetSize())));

            std::string outputStr = "";
            //double value;
            SignatureElementTypeChar value;

            for(int i = 0; i < signature0.GetSize(); i++){

                //if(i) outputStr += " ";
                //if(i) outputStr += "\t";

                strs.str("");
                // '=' indicates the end of the string
                if (signature0.getData()[i] == '\0') {
                    break ;
                }
                strs << signature0.getData()[i];

                //outputStr += std::to_string(signature0.get(i));
                outputStr += strs.str();
                //ereport(INFO, (errmsg("%s", strs.str().c_str())));
                

            }

            //text *destination = (text *) palloc(VARHDRSZ + outputStr.length()+1);
            text *destination = (text *) palloc(VARHDRSZ + outputStr.length());
            //SET_VARSIZE(destination, VARHDRSZ + outputStr.length()+1);
            SET_VARSIZE(destination, VARHDRSZ + outputStr.length());
            //memcpy(destination, outputStr.c_str(), outputStr.length()+1);
            memcpy(VARDATA(destination), outputStr.c_str(), outputStr.length());

            outputStr.clear();

            PG_RETURN_TEXT_P(destination);                                                     
        }                                                                                           
        catch(exception& e)                                                                         
        {                                                                                           
            ereport(ERROR,                                                                          
                (errmsg("[ERROR] in %s: %s\n", "signature_to_text_char", e.what() )));              
        }                                                                                           
    }



#ifdef __cplusplus
}
#endif

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

    //return Signature(0, signature);
    pfree(vetor_completo);
    return s;
}

double hammingDistance(bytea *x, bytea *y) {
    double dist = 0;
    unsigned long long int pHash1 = 0, pHash2 = 0, val = 0;

    memcpy(&pHash1, VARDATA(x), 8);
    memcpy(&pHash2, VARDATA(y), 8);

    val = pHash1 ^ pHash2;

    while (val != 0) {
        dist++;
        val &= val - 1;
    }

    return dist;
}

