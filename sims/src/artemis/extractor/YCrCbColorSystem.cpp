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
#include <artemis/extractor/YCrCbColorSystem.h> 

/**
* Constructor
*/
YCrCbColorSystem::YCrCbColorSystem() {
}

/**
* Destructor.
*/
YCrCbColorSystem::~YCrCbColorSystem() {
}

/**
* Converts a pixel in YCCrCb to gray scale.
*/
u_int8_t YCrCbColorSystem::toGrayScale(u_int16_t y, u_int16_t u, u_int16_t v){

    return y;
}

/**
* Converts a pixel in RGB to Y channel.
*/
u_int8_t YCrCbColorSystem::getYOfYCrCb(Pixel p){

    u_int8_t aux = 16 + ((65.738*p.getRedPixelValue()/256) + (129.057*p.getGreenPixelValue()/256) + (25.064*p.getBluePixelValue()/256));

    // limit value range from 0 to 255
    if(aux < 0) {
        aux = 0;
    }
    if(aux > 255) {
        aux = 255;
    }
    return aux;
}

/**
* Converts a pixel in RGB to Cr channel.
*/
u_int8_t YCrCbColorSystem::getCrOfYCrCb(Pixel p){

    u_int8_t aux = 128 + ((112.439*p.getRedPixelValue()/256) - (94.154*p.getGreenPixelValue()/256) - (18.285*p.getBluePixelValue()/256));
    // limit value range from 0 to 255
    if(aux < 0) {
        aux = 0;
    }
    if(aux > 255) {
        aux = 255;
    }
    return aux;
}

/**
* Converts a pixel in RGB to V channel.
*/
u_int8_t YCrCbColorSystem::getCbOfYCrCb(Pixel p){

    u_int8_t aux = 128 + (112*p.getRedPixelValue() - 93.786*p.getGreenPixelValue() + 18.214*p.getBluePixelValue());
    // limit value range from 0 to 255
    if(aux < 0) {
        aux = 0;
    }
    if(aux > 255) {
        aux = 255;
    }
    return aux;
}
