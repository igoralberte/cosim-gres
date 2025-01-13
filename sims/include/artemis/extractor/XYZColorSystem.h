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
* This file contains the definition of the XYZColorSystem class.
*
* @version 1.0
*/
#ifndef XYZCOLORSYSTEM_HPP
#define XYZCOLORSYSTEM_HPP

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <artemis/image/Pixel.h>
#include <artemis/extractor/SRGBColorSystem.h>

/**
* @author 009
* @author Marcos Vinicius Naves Bedo (marcosivni@grad.icmc.usp.br)
* @version 1.0.
*/
class XYZColorSystem {

    private:
        const double mXYZConversion[3][3] = {
            {0.4124,0.3576,0.1805},
            {0.2126,0.7152,0.0722},
            {0.0193,0.1192,0.9505}
        };

    public:
        XYZColorSystem();
        ~XYZColorSystem();

        double getXOfXYZ(Pixel p);
        double getYOfXYZ(Pixel p);
        double getZOfXYZ(Pixel p);

};

#endif
