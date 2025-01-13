#include "distance_functions.h"

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
        Signature signature1 = ByteArrayToBasicArrayObject(byte_array1);                    \
        hermes_function_name<Signature> function_name;                                      \
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

#ifdef __cplusplus
}
#endif

Signature ByteArrayToBasicArrayObject(bytea *byte_array) {
    vector<SignatureElementType> signature;

    int byte_size = VARSIZE(byte_array) - VARHDRSZ;
    SignatureElementType *data = (SignatureElementType *)(palloc(byte_size));

    memcpy(data, VARDATA(byte_array), byte_size);

    int signature_size = byte_size / sizeof(SignatureElementType);
    for (int i = 0; i < signature_size; ++i) {
        signature.push_back(data[i]);
    }

    return Signature(0, signature);
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
