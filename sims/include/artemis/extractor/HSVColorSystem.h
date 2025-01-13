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
#ifndef HSVCOLORSYSTEM_HPP
#define HSVCOLORSYSTEM_HPP

#include <artemis/image/Pixel.h>
#include <stdexcept>

/**
* @author 009
* @author 006
* @version 1.0.
*/
class HSVColorSystem {

    public:
        HSVColorSystem();
        ~HSVColorSystem();

        void toHSV(Pixel p, u_int16_t *hue, u_int16_t *saturation, u_int16_t *value) throw (std::runtime_error);
};

#endif
