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
* @file
*
* This file defines the Euclidean distance.
*
* @version 1.0
* @date 10-29-2014
*/

#ifndef EUCLIDEANDISTANCE_H
#define EUCLIDEANDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the Euclidean (or geometric) Distance
*
* @brief L2 distance class.
* @author 006.
* @version 1.0.
*/
template <class ObjectType>
class EuclideanDistance : public DistanceFunction <ObjectType>{

    public:

        EuclideanDistance();
        virtual ~EuclideanDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "EuclideanDistance-inl.h"
#endif // EUCLIDEANDISTANCE_H
