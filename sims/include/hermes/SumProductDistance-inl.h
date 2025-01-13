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
template <class ObjectType>
SumProductDistance<ObjectType>::SumProductDistance() {
    numEval = 0;
}

template <class ObjectType>
SumProductDistance<ObjectType>::~SumProductDistance() {
    clear();
}

template <class ObjectType>
void SumProductDistance<ObjectType>::clear(){

    for (size_t i = 0; i < numEval; i++) {
        delete Evaluator[i];
    }
    Evaluator.clear();
    numEval = 0;
}

template <class ObjectType>
void SumProductDistance<ObjectType>::addEval(myDistance *e, double w) {

    Evaluator.insert(Evaluator.end(), e);
    max.insert(max.end(), 0);
    weight.insert(weight.end(), w);
    numEval++;
}

template <class ObjectType>
double SumProductDistance<ObjectType>::getPartialDistance(ObjectType &o1, ObjectType &o2, u_int32_t i) {

    double tmp;
    double sum = 0;
    size_t NumObjs = Evaluator.size();

    if (i < NumObjs) {
        tmp = Evaluator[i]->getDistance(o1, o2);
        sum += (weight[i] * tmp);
    }
    this->updateDistanceCount();

    return sum;
}

/**
* @copydoc DistanceFunction::getDistance().
*/
template <class ObjectType>
double SumProductDistance<ObjectType>::GetDistance(ObjectType &o1, ObjectType &o2) {

    return getDistance(o1, o2);
}

/**
* @copydoc DistanceFunction::getDistance().
*/
template <class ObjectType>
double SumProductDistance<ObjectType>::getDistance(ObjectType &o1, ObjectType &o2) {

    double sum = 0.0;
    double tmp = 0.0;
    int numObjs = Evaluator.size();

    for (size_t i = 0; i < numObjs && i < numEval; i++) {
        //cout << typeid(*(o1->GetObj(i))).name() << endl;
        tmp = Evaluator[i]->getDistance(o1, o2);
        if (weight[i] * tmp > max[i])
            max[i] = weight[i] * tmp;
        sum += (weight[i] * tmp);
    }

    this->updateDistanceCount();

    return sum;
}

/**
* Gets the maximum distance.
*
* @return The maximum distance.
*/
template <class ObjectType>
double SumProductDistance<ObjectType>::getMax(size_t i){

    return max[i];
}
