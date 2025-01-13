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
#include <artemis/extractor/SRGBColorSystem.h> 

/**
* Constructor
*/
SRGBColorSystem::SRGBColorSystem() {
}

/**
* Destructor.
*/
SRGBColorSystem::~SRGBColorSystem() {
}


/**
* Converts a pixel in RGB to R channel.
*/
double SRGBColorSystem::getROfSRGB(Pixel p){

    double aux;

    if (p.getRedPixelValue() < 10.0164)
        aux = (double)p.getRedPixelValue()/(255*12.92);
    else
        aux = pow(((((double)p.getRedPixelValue())/255)+0.055)/1.055,2.4);

	return aux;
}

/**
* Converts a pixel in RGB to G channel.
*/
double SRGBColorSystem::getGOfSRGB(Pixel p){

    double aux;

    if(p.getGreenPixelValue() < 10.0164)
        aux = (double)p.getGreenPixelValue()/(255*12.92);
    else
        aux = pow(((((double)p.getGreenPixelValue())/255)+0.055)/1.055,2.4);
	return aux;
}

/**
* Converts a pixel in RGB to B channel.
*/
double SRGBColorSystem::getBOfSRGB(Pixel p){

    double aux;

    if(p.getBluePixelValue() < 10.0164)
        aux = (double)p.getBluePixelValue()/(255*12.92);
    else
        aux = pow((((double)(p.getBluePixelValue())/255)+0.055)/1.055,2.4);
	return aux;
}
