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
MetricHistogramDistance<ObjectType>::MetricHistogramDistance() {
}

/**
* Destructor.
*/
template <class ObjectType>
MetricHistogramDistance<ObjectType>::~MetricHistogramDistance() {
}

/**
* @deprecated Use getDistance(ObjectType &obj1, ObjectType &obj2) instead.
*
* @copydoc getDistance(ObjectType &obj1, ObjectType &obj2) .
*/
template <class ObjectType>
double MetricHistogramDistance<ObjectType>::GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    return getDistance(obj1, obj2);
}

/**
* Calculates similariry between the two metric histograms.
* This calculus is based on the histograms.
*
* @param obj1: The first feature vector.
* @param obj2: The second feature vector.
* @return The distance between feature vector 1 and feature vector 2.
*/
template <class ObjectType>
double MetricHistogramDistance<ObjectType>::getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error){

    double *new_obj1_gray  = new double[(obj1.size()/2)];
    double *new_obj1_value = new double[(obj1.size()/2)];

    double *new_obj2_gray  = new double[(obj2.size()/2)];
    double *new_obj2_value = new double[(obj2.size()/2)];

    for (size_t x = 0; x < (obj1.size()/2); x++){
        new_obj1_gray[x] = obj1[x];
        new_obj1_value[x] = obj1[(obj1.size()/2) + x];
    }

    for (size_t x = 0; x < (obj2.size()/2); x++){
        new_obj2_gray[x] = obj2[x];
        new_obj2_value[x] = obj2[(obj2.size()/2) + x];
    }

    double d1, d2;
    double m;
    d1 = d2 = 0.0;

    for (size_t x = 0; x < ((obj1.size()/2)-1); x++){
        m = (new_obj1_value[x+1] - new_obj1_value[x])/(new_obj1_gray[x+1] - new_obj1_gray[x]);
        d1 += ((m*new_obj1_gray[x+1]*new_obj1_gray[x+1])/2.0) + (new_obj1_value[x]*new_obj1_gray[x+1]);
        d1 -= ((m*new_obj1_gray[x]*new_obj1_gray[x])/2.0) + (new_obj1_value[x]*new_obj1_gray[x]);
    }

    for (size_t x = 0; x < ((obj2.size()/2)-1); x++){
        m = (new_obj2_value[x+1] - new_obj2_value[x])/(new_obj2_gray[x+1] - new_obj2_gray[x]);
        d2 += ((m*new_obj2_gray[x+1]*new_obj2_gray[x+1])/2.0) + (new_obj2_value[x]*new_obj2_gray[x+1]);
        d2 -= ((m*new_obj2_gray[x]*new_obj2_gray[x])/2.0) + (new_obj2_value[x]*new_obj2_gray[x]);
    }

    this->updateDistanceCount();

    return fabs(d1 - d2);
}
