#ifndef DISTANCE_FUNCTIONS_H
#define DISTANCE_FUNCTIONS_H

#include <hermes/EuclideanDistance.h>
#include <hermes/ManhattanDistance.h>
#include <hermes/ChebyshevDistance.h>
#include <hermes/CanberraDistance.h>
#include <hermes/KullbackLeiblerDivergenceDistance.h>
#include <hermes/JeffreyDivergenceDistance.h>
#include <hermes/LEdit.h>
#include <util/BasicArrayObject.h>

#include <string>
#include <sstream>
#include <vector>

using namespace std;


#ifdef __cplusplus
extern "C" {
#endif

    #include <postgres.h>
    #include <fmgr.h>

#ifdef __cplusplus
}
#endif

typedef char SignatureElementTypeChar;
typedef BasicArrayObject<SignatureElementTypeChar> SignatureChar;

typedef float SignatureElementType;
typedef BasicArrayObject<SignatureElementType> Signature;

extern Signature ByteArrayToBasicArrayObject(bytea *byte_array);
bytea * Signature2ByteArray(Signature& imgSignature);
double hammingDistance(bytea *x, bytea *y);

extern SignatureChar CharByteArrayToBasicArrayObject(bytea *byte_array);
bytea * CharSignature2ByteArray(SignatureChar& imgSignature);

#endif // DISTANCE_FUNCTIONS_H
