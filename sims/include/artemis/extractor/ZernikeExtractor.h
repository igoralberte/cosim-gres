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
#ifndef ZERNIKEEXTRACTOR_H
#define ZERNIKEEXTRACTOR_H

#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>
#include <cmath>
#include <vector>

/**
*
* @brief Extractor of Zernicke Moments
* @see Image hermes/util/BasicArrayObject
* @version 1.0
*/
template< class SignatureType, class DataObjectType = Image>
class ZernikeExtractor  : public Extractor<SignatureType, DataObjectType>{

    private:
        u_int8_t numMoments;

    public:
        ZernikeExtractor(u_int8_t numMoments = 10);
        ~ZernikeExtractor();

        void setNumMoments(u_int8_t numMoments);
        u_int8_t getNumMoments();

        double realZernikePolynom(char n, char m, double r2);
        std::pair< double, double> zernikePolynom(short n, short m, double x, double y);
        std::pair<double, double> zernikeMoment(Image image, short n, short m, short xc, short yc, double xscale, double yscale);
        std::vector< std::pair<double, double> > zernikeMoments(short n, Image image);

        virtual void generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error);
};

#include "ZernikeExtractor-inl.h"
#endif // ZERNIKEEXTRACTOR_H
