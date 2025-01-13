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
 * This file defines a Class to extract a Normalized Histogram.
 *
 * @version 1.0
 */
#ifndef NORMALIZEDHISTOGRAM_HPP
#define NORMALIZEDHISTOGRAM_HPP

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>

/**
 * This is a Template-Class. A template-parameter numFeatures is used
 * to define the lenght of the histogram.
 *
 * <P> A Normalized Histogram is a simple sum of all pixel frequency
 * in the Image. In this case the image must be in gray scale.
 *
 * NOTE:
 * <ul>
 * <li>DataObjectType must satisfy the interface Image;</li>
 * <li>SignatureType must be an array object satisfying interface stObject
 * (e.g. stBasicArrayObject <double>);</li>
 * <li>numFeatures is the number of bins of the histogram (defaults to 256);</li>
 * <li>normalizationRange is the maximum value of a bin in the normalization,
 * i.e., every bin is in the range [0, normalizationRange]. It defaults to 1.<br/>
 * The normalization in the range [0, 1] means that the sum of all bin values
 * is equal to 1. The normalization in the range [0, normalizationRange] first
 * normalize the bin values in the range [0, 1] and, thereafter, divides every
 * value is divided by the maximum value found, making the maximum equals to
 * normalizationRange.</li>
 * </ul>
 *
 * @brief Extractor of Normalized Histogram
 * @author Marcelo Ponciano da Silva (ponciano@icmc.usp.br)
 * @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
 * @author Daniel dos Santos Kaster (dskaster@uel.br)
 * @see Image BasicArrayObject
 * @version 1.0
 */
template< class SignatureType, class DataObjectType = Image >
class NormalizedHistogramExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        size_t histogramSize;
        bool normalization;

    public:
        NormalizedHistogramExtractor(){
            setNormalization(true);
            setHistogramSize(256);
        }

        NormalizedHistogramExtractor(size_t histogramSize, bool normalization){
            setNormalization(normalization);
            setHistogramSize(histogramSize);
        }

        virtual ~NormalizedHistogramExtractor(){
        }

        void setHistogramSize(size_t histogramSize){
            this->histogramSize = histogramSize;
        }

        void setNormalization(bool normalization){
            this->normalization =  normalization;
        }

        size_t getHistogramSize(){
            return histogramSize;
        }

        bool isNormalized(){
            return normalization;
        }

        virtual void generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error);

};

//include implementation (template class)
#include "NormalizedHistogram-inl.h"

#endif
