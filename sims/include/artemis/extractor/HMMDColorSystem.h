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
#ifndef HMMDCOLORSYSTEM_H
#define HMMDCOLORSYSTEM_H

#include <artemis/image/Pixel.h>

/**
* @author 009
* @author 006
* @version 1.0.
*/
class HMMDColorSystem {

    public:
        HMMDColorSystem();
        ~HMMDColorSystem();

        void toHMMD(Pixel p, u_int16_t *hue, u_int8_t *max, u_int8_t *min, u_int8_t *diff);
};

#endif
