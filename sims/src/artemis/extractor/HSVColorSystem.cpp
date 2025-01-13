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
#include <artemis/extractor/HSVColorSystem.h> 

HSVColorSystem::HSVColorSystem() {
}

HSVColorSystem::~HSVColorSystem() {
}

void HSVColorSystem::toHSV(Pixel p, u_int16_t *hue, u_int16_t *saturation, u_int16_t *value) throw (std::runtime_error){

    int32_t red, green, blue;

    red = static_cast<int32_t> (p.getRedPixelValue());
    green = static_cast<int32_t> (p.getGreenPixelValue());
    blue = static_cast<int32_t> (p.getBluePixelValue());

    u_int16_t maxrgb = 0, minrgb;
    int32_t order;
    double floath;

    //Establishes an order between the RGB pixels
    //0 = rgb, 1=gbr, 2=grb, 3=rbg, 4=bgr, 5=brg
    //the value will be put into variable order

    /*sort r g and b and get max and min value*/
    if(green > blue) {
        if(red > green) {
            maxrgb = red;
            minrgb = blue;
            order = 0;
        }
        else {
            if(blue > red) {
                maxrgb = green;
                minrgb = red;
                order = 1;
            }
            else {
                maxrgb = green;
                minrgb = blue;
                order = 2;
            }
        }
    }
    else {
        if(red > blue) {
            maxrgb = red;
            minrgb = green;
            order = 3;
        }
        else {
            if(green > red) {
                maxrgb = blue;
                minrgb = red;
                order = 4;
            }
            else {
                maxrgb = blue;
                minrgb = green;
                order = 5;
            }
        }
    }

    if(maxrgb == 0) {
        *value = 0;
        *saturation = 0;
        *hue = 0;
        return;
    }

    *value = static_cast<u_int16_t>(maxrgb);
    *saturation = static_cast<u_int16_t>(((maxrgb - minrgb)*255)/maxrgb);

    if(maxrgb == minrgb) {
      *hue = 0;
      return;
    }

    switch(order) {
        case 0:
            floath = 1.0 - static_cast<double>(red - green)/static_cast<double>(red - blue);
            break;
        case 1:
            floath = 3.0 - static_cast<double>(green - blue)/static_cast<double>(green - red);
            break;
        case 2:
            floath = 1.0 + static_cast<double>(green - red)/static_cast<double>(green - blue);
            break;
        case 3:
            floath = 5.0 + static_cast<double>(red - blue)/static_cast<double>(red - green);
            break;
        case 4:
            floath = 3.0 + static_cast<double>(blue - green)/static_cast<double>(blue - red);
            break;
        case 5:
            floath = 5.0 - static_cast<double>(blue - red)/static_cast<double>(blue - green);
            break;
        default:
            throw  std::runtime_error("Cannot convert from RGB to HSV.");
    }

    *hue = static_cast<u_int16_t>(floath/6*255);
}
