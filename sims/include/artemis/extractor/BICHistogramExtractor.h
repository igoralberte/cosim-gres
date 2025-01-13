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
 * This file defines a Class to extract a Normalized BIC Histogram (Border/Interior pixel Classification Histogram).
 *
 * @version 1.0
 * @author Daniel Yoshinobu Takada Chino (chinodyt@icmc.usp.br)
 * @date 15-11-2014
 */
#ifndef BICHISTOGRAMEXTRACTOR_H
#define BICHISTOGRAMEXTRACTOR_H

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>
#include <stdexcept>

/**
 * This is a Template-Class. A template-parameter numFeatures is used
 * to define the lenght of the histogram.
 *
 * <P> A Normalized BIC Histogram is a simple sum of all border and interior pixels frequency
 * in the Image. In this case the image must be in gray scale.
 *
 * NOTE:
 * <ul>
 * <li>DataObjectType must satisfy the interface Image;</li>
 * <li>SignatureType must be an array object satisfying interface stObject
 * (e.g. stBasicArrayObject <double>);</li>
 * <li>numFeatures is the number of bins of the BIC histogram for each pixel classification,
 * the feature vector will have size == 2*numFeatures (defaults to 256);</li>
 * <li>normalizationRange is the maximum value of a bin in the normalization,
 * i.e., every bin is in the range [0, normalizationRange]. It defaults to 1.<br/>
 * The normalization in the range [0, 1] means that the sum of all bin values
 * is equal to 1. The normalization in the range [0, normalizationRange] first
 * normalize the bin values in the range [0, 1] and, thereafter, divides every
 * value is divided by the maximum value found, making the maximum equals to
 * normalizationRange.</li>
 * </ul>
 *
 * @brief Extractor of Normalized BIC Histogram
 * @author 014
 * @see Image BasicArrayObject
 * @version 1.0
 */
template< class SignatureType, class DataObjectType = Image >
class BICHistogramExtractor : public Extractor<SignatureType, DataObjectType> {

private:
    u_int16_t numFeatures;

public:
    BICHistogramExtractor(u_int16_t value = 256);
    virtual ~BICHistogramExtractor();
    u_int16_t getNumFeatures();
    void setNumFeatures(u_int16_t value);

    virtual void generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error);

};

#include "BICHistogramExtractor-inl.h"

#endif
