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
ManhattanDistance<ObjectType>::ManhattanDistance(){
}

/**
        * Destructor.
        */
template <class ObjectType>
ManhattanDistance<ObjectType>::~ManhattanDistance(){
}

/**
        * @deprecated Use getDistance(ObjectType &obj1, ObjectType &obj2) instead.
        *
        * @copydoc getDistance(ObjectType &obj1, ObjectType &obj2) .
        */
template <class ObjectType>
double ManhattanDistance<ObjectType>::GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    return getDistance(obj1, obj2);
}

/**
        * Calculates the Manhattan distance between two feature vectors.
        * This calculus is based on the math form sum(feature_1[i] - feature_2[i]).
        * To make this computations both feature vectors should have the same size().
        *
        * @param obj1: The first feature vector.
        * @param obj2: The second feature vector.
        * @throw Exception If the computation is not possible.
        * @return The Manhattan distance between feature vector 1 and feature vector 2.
        */
template <class ObjectType>
double ManhattanDistance<ObjectType>::getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    if (obj1.size() != obj2.size()){
        throw std::length_error("The feature vectors do not have the same size.");
    }

    double d = 0;
    double tmp;

    for (size_t i = 0; i < obj1.size(); i++){
        tmp = fabs(obj1[i] - obj2[i]);
        d = d + tmp;
    }

    // Statistic support
    this->updateDistanceCount();

    return d;
}
