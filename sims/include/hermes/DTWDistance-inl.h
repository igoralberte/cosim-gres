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
DTWDistance<ObjectType>::DTWDistance(){
}

/**
* Destructor.
*/
template <class ObjectType>
DTWDistance<ObjectType>::~DTWDistance(){
}

template <class ObjectType>
double DTWDistance<ObjectType>::GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    return getDistance(obj1, obj2);
}

/**
* Calculates the Dynamic Time Warping distance between two feature vectors.
*
* @param obj1: The first feature vector.
* @param obj2: The second feature vector.
* @throw Exception If the computation is not possible.
* @return The DTW distance between feature vector 1 and feature vector 2.
*/
template <class ObjectType>
double DTWDistance<ObjectType>::getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    size_t rows = obj1.size();
    size_t cols = obj2.size();
    double d = 0.0;
    double **dtw; //Matrix to be used.

    // Allocates the memory for matrix M
    dtw = new double*[rows + 1];
    for (size_t i = 0; i < rows + 1; i++){
        dtw[i] = new double[cols + 1];
    }

    // Initialize
    dtw[0][0] = pow(obj1[0] - obj2[0], 2.0);
    // Initialize first col
    for (size_t i = 1; i < rows; i++)
        dtw[i][0] = pow(obj1[i] - obj2[0], 2.0) + dtw[i-1][0];
    // Initialize first row
    for (size_t i = 1; i < cols; i++)
        dtw[0][i] = pow(obj1[0] - obj2[i], 2.0) + dtw[0][i-1];

    // Execute the recurrence
    for (size_t i = 1; i < rows; i++) {
        for (size_t j = 1; j < cols; j++) {
            dtw[i][j] = pow(obj1[i] - obj2[j], 2.0) + fmin(dtw[i-1][j-1], fmin(dtw[i][j-1], dtw[i-1][j]));
        }
    }

    // stores the result
    d = sqrt(dtw[rows-1][cols-1]);

    // Statistic support
    this->updateDistanceCount();

    return d;
}

