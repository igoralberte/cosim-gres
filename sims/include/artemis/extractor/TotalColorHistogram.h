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
#ifndef TOTALCOLORHISTOGRAM_HPP
#define TOTALCOLORHISTOGRAM_HPP

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>

/**
 * This is a Template-Class. A template-parameter numFeatures is used
 * to define the lenght of the histogram.
 *
 * <P> A Total Clor Histogram is a simple sum of all pixel frequency
 * in the Image of each color band.
 *
 *
 * @brief Extractor of Normalized Histogram
 * @version 1.0
 */
template<class SignatureType, class DataObjectType = Image>
class TotalColorHistogramExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        u_int16_t numFeatures;

    public:

        TotalColorHistogramExtractor(u_int16_t numFeatures = 256);
        virtual ~TotalColorHistogramExtractor();

        u_int16_t getNumFeatures();

        void generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error, std::range_error);
};


#include "TotalColorHistogram-inl.h"
#endif
