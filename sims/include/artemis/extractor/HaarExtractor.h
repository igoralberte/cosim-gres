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
#ifndef HAAREXTRACTOR_H
#define HAAREXTRACTOR_H

#include <vector>
#include <cmath>
#include <cfloat>

#include <artemis/extractor/Wavelets.h>
#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>

/**
* This is a Template-Class. A template-parameter levels defines the levels
* utilized on the Wavelet. The template parameter vectorComposition defines
* a type of signature what will be utilized.
*
* @brief Implementation of Haar Wavelet Extractor
* @author 005
* @author 006
* @see ImageBase
* @see ImageException
* @version 1.0
*/
template< class SignatureType, class DataObjectType = Image >
class HaarExtractor : public Extractor<SignatureType, DataObjectType> , public Wavelets <SignatureType, DataObjectType>{
    public:
        HaarExtractor();
        virtual ~HaarExtractor();

        virtual void generateSignature(const DataObjectType & img, SignatureType & sign) throw (std::runtime_error);
};

#include "HaarExtractor-inl.h"
#endif
