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
* This file defines the extractor for Metric Histogram.
*
* @version 1.0
*/
#ifndef METRICHISTOGRAM_h
#define METRICHISTOGRAM_h

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cfloat>
#include <vector>

#include <artemis/extractor/Extractor.h>
#include <artemis/image/ImageBase.h>


/**
* This is a template class. The template parameter numFeatures define the deepth of the image.
* The template threshTangent and threshZero define the thresholds to valide variations on traditional
* histogram. The template parameter bucketLenght define the max size of the buckets.
*
* @brief Defines the correct behavior of Metric Histogram
* @author 001
* @author 005
* @version 1.0
*/
template< class SignatureType, class DataObjectType = Image >
class MetricHistogramExtractor : public Extractor<SignatureType, DataObjectType> {
private:
    std::vector<double> tradHistogram;
    u_int16_t numFeatures;
    u_int16_t threshTangent;
    u_int16_t threshZero;
    u_int16_t bucketLenght;

public:

    MetricHistogramExtractor(u_int16_t numFeatures = 256, u_int16_t threshTangent = 2, u_int16_t threshZero = 1, u_int16_t bucketLenght = 2);
    ~MetricHistogramExtractor() ;

    u_int16_t verifyDirection(double value);  
    bool verifyTangent(double angle1, double angle2);

    virtual void generateSignature(const Image & image, SignatureType & sign) throw (std::runtime_error);

    void setBucketLenght(u_int16_t bucketLenght);
    void setThreshZero(u_int16_t threshZero);
    void setThreshTangent(u_int16_t threshTangent);
    void setNumFeatures(u_int16_t numFeatures) ;

    u_int16_t getBucketLenght() const;
    u_int16_t getThreshZero() const;
    u_int16_t getThreshTangent() const;
    u_int16_t getNumFeatures() const;

};

#include "MetricHistogram-inl.h"
#endif // METRICHISTOGRAM_h
