#ifndef DISTANCE_FUNCTIONS_H
#define DISTANCE_FUNCTIONS_H

#include <hermes/EuclideanDistance.h>
#include <hermes/ManhattanDistance.h>
#include <hermes/ChebyshevDistance.h>
#include <hermes/CanberraDistance.h>
#include <hermes/KullbackLeiblerDivergenceDistance.h>
#include <hermes/JeffreyDivergenceDistance.h>
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
typedef float SignatureElementType;
typedef BasicArrayObject<SignatureElementType> Signature;

Signature ByteArrayToBasicArrayObject(bytea *byte_array);

double hammingDistance(bytea *x, bytea *y);

#endif // DISTANCE_FUNCTIONS_H
