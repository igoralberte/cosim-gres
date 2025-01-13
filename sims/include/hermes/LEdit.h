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
* This file defines the distance functions of the LEdit.
*
* @version 1.0
*/

#ifndef LEDIT_H
#define LEDIT_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

/**
* Class to obtain the LEdit distance.
*
* @brief L-Edit class.
* @author 006.
* @version 1.0.
*/
template <class ObjectType = std::string >
class LEditDistance : public DistanceFunction <ObjectType> {

    public:

        LEditDistance();
        virtual ~LEditDistance();

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
};

#include "LEdit-inl.h"
#endif // LEDIT_H
