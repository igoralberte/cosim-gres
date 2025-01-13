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
* This file contains the definition of the YUVColorSystem class.
*
* @version 1.0
*/
#ifndef YCRCBCOLORSYSTEM_H
#define YCRCBCOLORSYSTEM_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <artemis/image/Pixel.h>

using namespace std;

/**
* @author 009
* @author 006
* @version 1.0.
*/
class YCrCbColorSystem {

    public:
        YCrCbColorSystem();
        ~YCrCbColorSystem();

        u_int8_t toGrayScale(u_int16_t y, u_int16_t u, u_int16_t v);
        u_int8_t getYOfYCrCb(Pixel p);
        u_int8_t getCrOfYCrCb(Pixel p);
        u_int8_t getCbOfYCrCb(Pixel p);
};

#endif //YCRCBCOLORSYSTEM_H
