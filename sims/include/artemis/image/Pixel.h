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
* This file contains the definition of the Pixel class.
*
* @version 1.0
*/
#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>


/**
* Pixel is a basic structure for processing and visualization of the used Images.
* The class Pixel  * supports Black And White scale, Gray Scale, RGB Scale and
* Alpha Transparency.
*
* @brief Base class of coordinate of an image.
* @version 1.0.
*/
class Pixel {

    private:
        unsigned char r, g, b;
        unsigned char alpha;
        float grayPixel;
        unsigned char bw;
        int grayLevels;

    public:
        Pixel();
        Pixel(unsigned char r, unsigned char g, unsigned char b);
        Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha);
        Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha, float grayPixel);
        Pixel(float grayValue, u_int16_t grayLevels);
        Pixel(const Pixel & pixel);
        ~Pixel();

        void setRedPixelValue(unsigned char r);
        void setGreenPixelValue(unsigned char g);
        void setBluePixelValue(unsigned char b);
        void setRGBPixelValue(unsigned char r, unsigned char g, unsigned char b);
        void setGrayPixelValue(float grayValue, u_int16_t grayLevels);
        void setAlphaPixelValue(unsigned char alpha);
        void setBlackAndWhitePixelValue(unsigned char bw);
        void setGrayLevels(int grayLevels);

        void toGrayScale();

        void toBlackAndWhite(u_int16_t grayLevels);

        float getGrayPixelValue() const;
        unsigned char getRedPixelValue() const;
        unsigned char getGreenPixelValue() const;
        unsigned char getBluePixelValue() const;
        unsigned char getAlphaPixelValue() const;
        unsigned char getBlackAndWhitePixelValue() const;
        int getGrayLevels() const;

        Pixel * clone() const;
        bool isEqual(const Pixel & p) const;
};

#endif
