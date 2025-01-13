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
#include <artemis/extractor/XYZColorSystem.h> 

/**
* Constructor
*/
XYZColorSystem::XYZColorSystem() {
}

/**
* Destructor.
*/
XYZColorSystem::~XYZColorSystem() {
}

/**
* Converts a pixel in RGB to X channel.
*/
double XYZColorSystem::getXOfXYZ(Pixel p){

    SRGBColorSystem aux;
    return mXYZConversion[0][0]*aux.getROfSRGB(p)+mXYZConversion[0][1]*aux.getGOfSRGB(p)+mXYZConversion[0][2]*aux.getBOfSRGB(p);
}

/**
* Converts a pixel in RGB to Y channel.
*/
double XYZColorSystem::getYOfXYZ(Pixel p){

    SRGBColorSystem aux;
    return mXYZConversion[1][0]*aux.getROfSRGB(p)+mXYZConversion[1][1]*aux.getGOfSRGB(p)+mXYZConversion[1][2]*aux.getBOfSRGB(p);
}

/**
* Converts a pixel in RGB to Z channel.
*/
double XYZColorSystem::getZOfXYZ(Pixel p){

    SRGBColorSystem aux;
    return mXYZConversion[2][0]*aux.getROfSRGB(p)+mXYZConversion[2][1]*aux.getGOfSRGB(p)+mXYZConversion[2][2]*aux.getBOfSRGB(p);
}
