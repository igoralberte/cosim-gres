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
 * This file defines co-ocurrences Matrix for Haralick use.
 *
 * @version 1.0
 */
#ifndef HARALICKEXTRACTOR_h
#define HARALICKEXTRACTOR_h


#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cfloat>
#include <artemis/extractor/HaralickFeature.h>
#include <artemis/extractor/Extractor.h>
#include <artemis/image/ImageBase.h>


/*
  Comments about the data structure:

                Co-ocurrence Matrix
                 x = numDistances
             ______________
            |_1|__|__|__|__|   y = numAngles 
            |__|__|__|__|__|
            |__|__|__|__|__|
            |__|__|__|__|__|
            |__|__|__|__|__|

               Where is writed 1 we have the matrix numBins x numBins associed
                 ...
            |__|__|__|__|__|
            |__|__|__|__|__|
            |__|__|__|__|__|
                 ...
 */

/**
 * This is a Template-Class. A template-parameter numDistances is used
 * to define the number of the distance what will be utilized on the
 * co-ocurrence matrix. The template-parameter numAngles is utilized
 * like numDistances. The template-parameter typeCharacteristics defines
 * how will be created the signature. The others parameters typeHaralickExtractor
 * and numBins defines the behavior of the extractor and the reescale for
 * the gray-scaled image.
 * <P>A haralick Extractor has six main features: Variance, Entropy, Uniformity,
 * homogeinity, moment2thMatrix and Reverse Variance. </P>
 *
 * @author 005
 * @author 006
 * @see ImageBase
 * @see ImageException
 * @see HaralickFeatures.h
 * @brief Extractor of Haralick Features.
 */

template< class SignatureType, class DataObjectType = Image >
class HaralickExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        u_int16_t numDistances;
        u_int16_t numAngles;
        u_int16_t typeCharacteristics;
        u_int16_t typeHaralickExtractor;
        u_int16_t numBins;

        double ****cm;
        HaralickFeaturesMatrix *varianceMatrix;
        HaralickFeaturesMatrix *entropyMatrix;
        HaralickFeaturesMatrix *uniformityMatrix;
        HaralickFeaturesMatrix *homogeneityMatrix;
        HaralickFeaturesMatrix *moment3thMatrix;
        HaralickFeaturesMatrix *reverseVarianceMatrix;

    public:
        static const u_int16_t VARIANCE = 0;
        static const u_int16_t ENTROPY = 1;
        static const u_int16_t UNIFORMITY = 2;
        static const u_int16_t HOMOGEINITY = 3;
        static const u_int16_t MOMENT_3TH = 4;
        static const u_int16_t REVERSE_VARIANCE = 5;
        static const u_int16_t ALL_CHARACTERISTICS = 6;
        static const u_int16_t DISTANCE = 0;
        static const u_int16_t ANGLE = 1;
        static const u_int16_t ZERO_PI = 0;
        static const u_int16_t PI_UNDER_FOUR = 1;
        static const u_int16_t PI_UNDER_TWO = 2;
        static const u_int16_t THREE_PI_UNDER_FOUR = 3;

    private:
        void createCoocurrenceMatrix() throw (std::runtime_error);
        void deleteCoocurrenceMatrix();
        void setCoocurrenceValue(u_int16_t distance, u_int16_t angle, u_int16_t numBins_x, u_int16_t numBins_y, double value) throw (std::range_error);
        void normalizeCoocurrenceMatrix(u_int16_t height, u_int16_t width);


    public:
        HaralickExtractor(u_int16_t numDistances = 5, u_int16_t numAngles = 4, u_int16_t typeCharacteristics = 6, u_int16_t typeHaralickExtractor = 0, u_int16_t numBins = 16);
        ~HaralickExtractor();

        void setNumDistances(u_int16_t numDistances);
        void setNumAngles(u_int16_t numAngles);
        void setTypeCharacteristics(u_int16_t typeCharacteristics);
        void setTypeHaralickExtractor(u_int16_t typeHaralickExtractor);
        void setNumBins(u_int16_t numBins);

        u_int16_t getNumDistances();
        u_int16_t getNumAngles();
        u_int16_t getTypeCharacteristics();
        u_int16_t getTypeHaralickExtractor();
        u_int16_t getNumBins();
        double getCoocurrenceValue(u_int16_t distance, u_int16_t angle, u_int16_t numBins_x, u_int16_t numBins_y) throw (std::range_error);

        void loadImageValues(const Image & image);

        double* getSelectedSignature();

        virtual void generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error);
};

#include "HaralickExtractor-inl.h"
#endif

