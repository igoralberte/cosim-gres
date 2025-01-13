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
* This file defines a Class to extract a EdgeHistogram of the image. This extractor is defined by MPEG7 pattern.
*
* @version 1.0
* @date 01-08-2014
*/

#ifndef EDGEHISTOGRAM_H
#define EDGEHISTOGRAM_H

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>
#include <artemis/extractor/EdgeHistogram.h>

#include <cmath>
#include <vector>

/**
* @brief Edge Histogram Extractor
* @author 009
* @author 006
* @version 1.0
*/

template<class SignatureType, class DataObjectType = Image >
class EdgeHistogramExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        u_int16_t numBlocks;

        static const int NUM_CHANNELS = 3;
        static constexpr double NUM_DIVISIONS_IMAGE = 4.0;

        static const u_int16_t NUM_DIVISIONS_IMAGE_INT = 4;
        static const u_int16_t NUM_FEATURES = 5;
        static const u_int16_t NUM_MIN_OF_MAX_EDGE = 11;
        static const u_int16_t HORIZONTAL_EDGE = 4441;
        static const u_int16_t VERTICAL_EDGE = 4442;
        static const u_int16_t DIAGONAL_45_DEGREE_EDGE = 4443;
        static const u_int16_t DIAGONAL_135_DEGREE_EDGE = 4444;
        static const u_int16_t NO_EDGE = 4445;
        static const u_int16_t NON_DIRECIONAL_EDGE = 4446;

    private:
        u_int16_t calculateBlockSize(const u_int16_t width, const u_int16_t height);
        u_int16_t calculateEgdeFeature(const DataObjectType &image, const u_int16_t width, const u_int16_t blockSize, const u_int16_t x, const u_int16_t y);
        std::vector<EdgeHistogram> extractLocalEdgeHistogramFromImage(const u_int16_t blockSize, const u_int16_t width, const u_int16_t height, const DataObjectType &image, std::vector<EdgeHistogram> histogram) throw (std::runtime_error);
        std::vector<EdgeHistogram> extractSemiLocalGlobalHistogram(std::vector<EdgeHistogram> histogram);

    public:
        EdgeHistogramExtractor(u_int16_t numBlocks = 1100);
        virtual ~EdgeHistogramExtractor();

        void setNumBlocks(u_int16_t numBlocks);
        u_int16_t getNumBlocks();

        virtual void generateSignature(const DataObjectType &image, SignatureType &sign) throw (std::runtime_error);
};

#include "EdgeHistogramExtractor-inl.h"
#endif // EDGEHISTOGRAM_H
