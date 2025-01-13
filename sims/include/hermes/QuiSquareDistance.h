/* Copyright 2003-2017 GBDI-ICMC-USP <caetano@icmc.usp.br>
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*   http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/**
* @QuiSquareDistance.hpp
*
* This file defines the distance functions
*
* @version 1.0
*/

#ifndef QUISQUAREDISTANCE_H
#define QUISQUAREDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>

/**
* Class to obtain the Qui-Square distance. The chi-squared distance is useful when comparing histograms.
*
* @brief Qui-Square distance class.
* @author 13.
* @version 1.0.
*/
template <class ObjectType>
class QuiSquareDistance : public DistanceFunction<ObjectType> {

    public:

        QuiSquareDistance();
        ~QuiSquareDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error) ;
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "QuiSquareDistance-inl.h"
#endif // QUISQUAREDISTANCE_H
