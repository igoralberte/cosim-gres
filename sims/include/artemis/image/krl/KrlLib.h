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
* This file contains a class to manipulate KRL Images. KRL images are
* scanned medical images with a little header.
*
* This header is represented here by a struct, but your attributes are
* relationed only with the image.
*
* @version 1.0
*/
#ifndef KRLLIB_HPP
#define KRLLIB_HPP

#include <artemis/image/ImageBase.h>
#include "FileHandler.h"

/**
* This struct is a header for KRL images.
*
* @brief Header for KRL Images.
* @author 005
* @author 006
* @version 1.0
*/
struct KrlHeaderType {
       
    u_int16_t headerType;
    char filename[32];
    u_int16_t width;
    u_int16_t height;
    char pixBytes;
    char pixBits;
    char OS;
    char compress;
    u_int16_t compParas[5];
    char hdtime[32];
    char imgType;
    char black;
    u_int16_t pixSize;
    u_int16_t sampType;
    char optDensity;
    char dayNo[13];
    long medHistNo;
    char studyType[10];
    char patientName[50];
    char special[216];
    char comment[128];
};

/**
* @brief Handler Class to KRL Images
*
* @author 005
* @author 006
* @see ImageBase
* @see ImageException
* @see krlsample.cpp
* @version 1.0
*/
class KRLImage : public Image{

    private:            
        KrlHeaderType header;
        
    public:
        KRLImage();
        KRLImage(std::string filename);
        virtual ~KRLImage();
        
        void openImage(std::string filename) throw (std::runtime_error);
        
        KRLImage* clone();
};

#endif
