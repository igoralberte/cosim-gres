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
 * This file defines a Class to extract a Color Layout of the image. This extractor is defined by MPEG7 pattern.
 *
 * @version 1.0
 * @see Image stBasicArrayObject
 */

#ifndef COLORLAYOUT_H
#define COLORLAYOUT_H

#include <artemis/extractor/DiscreteCosineTransformation.h>
#include <artemis/extractor/YCrCbColorSystem.h>
#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>


template< class SignatureType, class DataObjectType = Image >
class ColorLayoutExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        u_int16_t numFeatures;
        u_int16_t numBlocks;

    private:
        std::vector<vector<int32_t> > imageSplitter(const u_int16_t channels, const u_int16_t dimension, const DataObjectType &image,
                                                std::vector<std::vector<int32_t> > blocks) throw (std::runtime_error);
        std::vector<std::vector<int32_t> > averageRepresentativeColorDefault(const u_int16_t channels, const u_int16_t dimension, int32_t *count, int32_t **sum, std::vector<std::vector<int32_t> > blocks);
        void quantifyYChannel(const int32_t block_value, int32_t &new_value);
        void quantifyCChannel(const int32_t block_value, int32_t &new_value);
        void quantify(const int32_t block_value, int32_t &new_value);
        void calculateZigZagPattern(const u_int16_t dimension, int32_t *zigzag);
        void sumQuantify(const u_int16_t dimension, std::vector<std::vector<int32_t> > blocks, int32_t **descriptor) throw (std::runtime_error);

    public:
        ColorLayoutExtractor(u_int16_t value1 = 256, u_int16_t value2 = 64);
        virtual ~ColorLayoutExtractor();

        void setNumFeatures(u_int16_t value);
        void setNumBlocks(u_int16_t value);

        u_int16_t getNumFeatures();
        u_int16_t getNumBlocks();

        u_int16_t getDimension() throw (std::bad_alloc*) ;

        virtual void generateSignature(const DataObjectType &image, SignatureType &sign) throw (std::runtime_error);
};


#include "ColorLayout-inl.h"
#endif
