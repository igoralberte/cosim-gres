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
 * This file defines the basis for for Daubechies and Haar Wavelets Extractor.
 *
 * @version 1.0
 */
#ifndef WAVELETEXTRACTOR_h
#define WAVELETEXTRACTOR_h

#include <cmath>
#include <vector>
#include <cfloat>
#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>

/**
* This is a Template-Class. A template-parameter levels defines the levels
* utilized on the Wavelet. The template parameter vectorComposition defines
* a type of signature what will be utilized.
*
* @brief Basic of Wavelet Extractor
* @author 005
* @author 006
* @see ImageBase
* @see ImageException
* @version 1.0
*/
template< class SignatureType, class DataObjectType = Image >
class Wavelets {
    private:
        u_int16_t levels;
        u_int16_t vectorComposition;

        std::vector<double> mean;
        std::vector<double> entropy;
        std::vector<double> energy;
        std::vector<double> variance;

        Image *multiSquare;

    protected:
        SignatureType extractSignature(DataObjectType *image);

    public:
        Wavelets(u_int16_t levels = 1, u_int16_t vectorComposition = 4);
        virtual ~Wavelets();

        void clearFeatures();
        void NormalizeImage(DataObjectType *image, size_t x0, size_t x1, size_t y0, size_t y1) throw (std::range_error);
        void setMultiSquareImage(DataObjectType *image);
    
        std::vector<double> getMeanFeatures() ;
        std::vector<double> getEntropyFeatures();
        std::vector<double> getEnergyFeatures();
        std::vector<double> getVarianceFeatures();
        DataObjectType* getMultiSquareImage();
        u_int16_t getLevels() ;
        u_int16_t getComposition();

        void setVectorComposition(u_int16_t vectorComposition);
        void setLevels(u_int16_t levels) ;
};


#include "Wavelets-inl.h"
#endif
