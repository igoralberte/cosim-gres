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
#include <artemis/extractor/HMMDColorSystem.h> 

HMMDColorSystem::HMMDColorSystem() {
}

HMMDColorSystem::~HMMDColorSystem() {
}

void HMMDColorSystem::toHMMD(Pixel p, u_int16_t *hue, u_int8_t *max, u_int8_t *min, u_int8_t *diff){
    //rgb to hmmd
    int32_t red, green, blue;

    red = static_cast<int32_t> (p.getRedPixelValue());
    green = static_cast<int32_t> (p.getGreenPixelValue());
    blue = static_cast<int32_t> (p.getBluePixelValue());

    double maxAux = std::max(std::max(red,green), std::max(green,blue));
    double minAux = std::min(std::min(red,green), std::min(green,blue));
    double hueAux = 0;
    double diffAux = (maxAux - minAux);

    if (diffAux == 0)
        hueAux = 0;
    else if ((red == maxAux) && ((green - blue) > 0))
        hueAux = 60*(green-blue)/(maxAux-minAux);
    else if ((red == maxAux) && ((green - blue) < 0))
        hueAux = 60*(green-blue)/(maxAux-minAux) + 360;
    else if (green == maxAux){
        hueAux = (double) (60*(2+(blue-red)/((maxAux)-(minAux))));}
    else if (blue == maxAux)
        hueAux = (double) (60*(4+(red-green)/(maxAux-minAux)));

    *hue = static_cast<u_int16_t> (hueAux);             //range [0,360]
    *max = static_cast<u_int8_t> (maxAux);              //range [0,255]
    *min = static_cast<u_int8_t> (minAux);              //range [0,255]
    *diff = static_cast<u_int8_t> (*max - *min);		//range [0,255]
}
