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
* This file implements all basic metric evaluators templates.
* Last modification 10-29-2014.
*
* @version 1.0
*/

#ifndef SUMPRODUCTDISTANCE_H
#define SUMPRODUCTDISTANCE_H

#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>


template <class ObjectType>
class SumProductDistance : public DistanceFunction <ObjectType>{

    private:
        typedef DistanceFunction<ObjectType> myDistance;
        int numEval;
        std::vector <double> max;
        std::vector <double> weight;
        std::vector <myDistance *> Evaluator;

    public:
        SumProductDistance();
        virtual ~SumProductDistance();
        void clear();

        void addEval(myDistance *e, double w);
        double getPartialDistance(ObjectType &o1, ObjectType &o2, u_int32_t i);
        double getDistance(ObjectType &o1, ObjectType &o2);
        double GetDistance(ObjectType &o1, ObjectType &o2);
        double getMax(size_t i);
};

#include "SumProductDistance-inl.h"
#endif //SUMPRODUCTDISTANCE_H

