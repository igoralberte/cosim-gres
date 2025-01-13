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
#ifndef HARALICKFEATURE_h
#define HARALICKFEATURE_h


#include <artemis/image/ImageBase.h>
#include <artemis/extractor/Extractor.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

/**
* This is single class that describes a coocurrence matrix with
* Haralick features. For each pair (distance, angle) six Haralick
* characteristics are extracted.
*
* <P> The co-ocurrence matrix are based o texture parameters. </P>
*
* @brief Generic co-ocurrence Matrix.
*
* @author 005
* @author 006
* @see ImageBase
* @see ImageException
* @see HaralickFeature.cpp
* @version 1.00
*/
class HaralickFeaturesMatrix {

    private:
        u_int16_t numDistances;
        u_int16_t numAngles;
        
        double **featuresMatrix; //[NUM_ANGLES][NUM_DISTANCE];
        double *distanceMeanFeatures;
        double *angleMeanFeatures;
    
    private:
        void createFeaturesMatrix() throw (std::runtime_error);
        void createDistanceMeanVector() throw (std::runtime_error);
        void createAngleMeanVector() throw (std::runtime_error);
        
        void deleteFeaturesMatrix();
        void deleteDistanceMeanVector();
        void deleteAngleMeanVector();
        
    public:
        HaralickFeaturesMatrix(u_int16_t numDistances = 5, u_int16_t numAngles = 4);
        ~HaralickFeaturesMatrix() throw (std::runtime_error);
        
        void setHaralickFeatureValue(u_int16_t distance, u_int16_t angle, double value) throw (std::range_error);
        void setDistanceMeanValue(u_int16_t pos, double value) throw (std::range_error);
        void setAngleMeanValue(u_int16_t pos, double value) throw (std::range_error);
        
        double getHaralickFeatureValue(u_int16_t distance, u_int16_t angle) throw (std::range_error);
        double getDistanceMeanValue(u_int16_t pos) throw (std::range_error);
        double getAngleMeanValue(u_int16_t pos) throw (std::range_error);
        u_int16_t getDistanceMeanSize();
        u_int16_t getAngleMeanSize();

        void setNumAngles(u_int16_t numAngles);
        void setNumDistances(u_int16_t numDistances);
        u_int16_t getNumAngles();
        u_int16_t getNumDistances();
        
        double* getDistanceMeanFeatures();
        double* getAngleMeanFeatures();

        HaralickFeaturesMatrix* clone();
        bool isEqual(HaralickFeaturesMatrix *h);
};

#endif
