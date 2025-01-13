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
* Constructor.
*/
template <class ObjectType>
ColorLayoutDistance<ObjectType>::ColorLayoutDistance(){
}

/**
* Destructor.
*/
template <class ObjectType>
ColorLayoutDistance<ObjectType>::~ColorLayoutDistance(){
}

/**
* @deprecated Use getDistance(ObjectType &obj1, ObjectType &obj2) instead.
*
* @copydoc getDistance(ObjectType &obj1, ObjectType &obj2) .
*/
template <class ObjectType>
double ColorLayoutDistance<ObjectType>::GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    return getDistance(obj1, obj2);
}

/**
* Calculates the Color Layout distance between two feature vectors.
* This calculus is based on the original paper using the same weight values.
* To make this computations both feature vectors should have the same size().
*
* @param obj1: The first feature vector.
* @param obj2: The second feature vector.
* @throw Exception If the computation is not possible.
* @return The Color Layout distance between feature vector 1 and feature vector 2.
*/
template <class ObjectType>
double ColorLayoutDistance<ObjectType>::getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    if (obj1.size() != obj2.size())
        throw std::length_error("The feature vectors do not have the same size.");

    double d = 0;
    double tmp, tmp2 = 0;
    int k = 0;

    const u_int8_t weight[12] = {2,2,2,1,1,1,2,1,1,4,2,2};//constant values
    double squares[12];

    //square first channel
    for (size_t i = 0; i < 6; i++){
        tmp = (obj1[i] - obj2[i]);
        squares[k] = (tmp * tmp) * weight[k];
        tmp2 = tmp2 + squares[k];
        k++;
    }

    d = d + sqrt(tmp2);
    tmp2 = 0;

    //square second channel
    for (size_t i = 64; i < 67; i++){
        tmp = (obj1[i] - obj2[i]);
        squares[k] = (tmp * tmp) * weight[k];
        tmp2 = tmp2 + squares[k];
        k++;
    }

    d = d + sqrt(tmp2);
    tmp2 = 0;

    //square third channel
    for (size_t i = 128; i < 131; i++){
        tmp = (obj1[i] - obj2[i]);
        squares[k] = (tmp * tmp) * weight[k];
        tmp2 = tmp2 + squares[k];
        k++;
    }

    d = d + sqrt(tmp2);

    // Statistic support
    this->updateDistanceCount();

    return d;
}
