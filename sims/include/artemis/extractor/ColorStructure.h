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
* This file defines a Class to extract a Color Structure histogram.
*
* @version 1.0
*/

#ifndef COLORSTRUCTURE_H
#define COLORSTRUCTURE_H

#include <cmath>
#include <vector>

#include <artemis/extractor/HMMDColorSystem.h>
#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>

template< class SignatureType, class DataObjectType = Image >
class ColorStructureExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        u_int8_t numFeatures;

        //Range of suspaces values
        const u_int32_t nSubSpaces[5] = {7,21,61,111,256};

        //Quantization Level
        const u_int32_t nQuantization[4] = {32,64,128,256};

        // numbers relative to hue level
        const u_int8_t nHueLevels[4][5] = {
            {1,4,4,4,4},
            {1,4,4,8,8},
            {1,4,8,8,8},
            {1,4,16,16,16}};

        // numbers relative to sum level
        const u_int8_t nSumLevels[4][5] = {
            {8,4,4,1,1},
            {8,4,4,2,1},
            {16,4,4,4,4},
            {32,8,4,4,4}};

        void structureElements(int32_t height, int32_t width, int32_t *factorE, int32_t *factorK);
        std::vector<double> acumulationHistogram(const DataObjectType &image, int32_t factorE, int32_t factorK);
        void quantizeValue(u_int16_t hue, u_int16_t sum, u_int8_t diff, int32_t *subspace, int32_t *quantizedHue, int32_t *quantizedSum);
        std::vector<int32_t> incrementHistogram(std::vector<int32_t> localHistogram, int32_t subspace, int32_t hue, int32_t sum);

    public:
        ColorStructureExtractor(u_int8_t value = 128) throw (std::runtime_error);
        virtual ~ColorStructureExtractor() {}

        void setNumFeatures(u_int8_t value) throw (std::runtime_error);
        u_int8_t getNumFeatures();

        virtual void generateSignature(const DataObjectType &image, SignatureType &sign);
};

//include implementation (template class)
#include "ColorStructure-inl.h"
#endif // COLORSTRUCTURE_H
