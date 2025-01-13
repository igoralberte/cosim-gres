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
#include <artemis/image/Pixel.h>

/**
* Constructor
*/
Pixel::Pixel() {

    setRGBPixelValue(0, 0, 0);
    setAlphaPixelValue(0);
    setGrayLevels(255);
    setGrayPixelValue(0, 255);
    setBlackAndWhitePixelValue(0);
}

/**
* Constructor
*
* @param r: Red value of the pixel.
* @param g: Green value of the pixel.
* @param b: Blue value of the pixel.
*/
Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b) {
    setRGBPixelValue(r, g, b);
    setGrayLevels(255);
    setAlphaPixelValue(0);
    toGrayScale();
}

/**
* Constructor for alpha value
*
* @param r: Red value of the pixel.
* @param g: Green value of the pixel.
* @param b: Blue value of the pixel.
* @param alpha: Alpha value of the pixel.
*/
Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha) {
    setRGBPixelValue(r, g, b);
    setAlphaPixelValue(alpha);
    setGrayLevels(255);
    toGrayScale();
}

/**
* Full Constructor for alpha value
*
* @param r: Red value of the pixel.
* @param g: Green value of the pixel.
* @param b: Blue value of the pixel.
* @param alpha: Alpha value of the pixel.
* @param grayValue: The gray value of the pixel.
*/
Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha, float grayPixel){

    setRGBPixelValue(r, g, b);
    setAlphaPixelValue(alpha);
    setGrayLevels(1024);
    setGrayPixelValue(grayPixel, 1024);
    toGrayScale();

}

/**
* Constructor of gray Pixel.
*
* @param grayValue: The gray value of the pixel.
*/
Pixel::Pixel(float grayValue, u_int16_t grayLevels) {
    setRGBPixelValue(0, 0, 0);
    setGrayPixelValue(grayValue, grayLevels);
    setAlphaPixelValue(0);
    setGrayLevels(grayLevels);
    toBlackAndWhite(getGrayLevels());
}

/**
* Clone-Constructor.
*/
Pixel::Pixel(const Pixel & pixel) {
    r = pixel.getRedPixelValue();
    g = pixel.getGreenPixelValue();
    b = pixel.getBluePixelValue();
    grayPixel = pixel.getGrayPixelValue();
    bw = pixel.getBlackAndWhitePixelValue();
    alpha = pixel.getAlphaPixelValue();
}

/**
* Destructor.
*/
Pixel::~Pixel() {
}

/**
* Sets a red value of the pixel.
*
* @param r: A red value of the pixel.
*/
void Pixel::setRedPixelValue(unsigned char r) {
    this->r = r;
    toGrayScale();
}

/**
* Sets a green value of the pixel.
*
* @param g: A green value of the pixel.
*/
void Pixel::setGreenPixelValue(unsigned char g) {
    this->g = g;
    toGrayScale();
}

/**
* Sets a blue value of the pixel.
*
* @param b: A blue value of the pixel.
*/
void Pixel::setBluePixelValue(unsigned char b) {
    this->b = b;
    toGrayScale();
}

/**
* Sets a RGB value of the pixel.
*
* @param r: Red value of the pixel.
* @param g: Green value of the pixel.
* @param b: Blue value of the pixel.
*/
void Pixel::setRGBPixelValue(unsigned char r, unsigned char g, unsigned char b) {
    this->r = r;
    this->g = g;
    this->b = b;
    toGrayScale();
}

/**
* Sets a gray pixel value and the leves of gray.
*
* @param grayValue: The gray value.
* @param grayLevels: The levels of gray.
*/
void Pixel::setGrayPixelValue(float grayValue, u_int16_t grayLevels) {

    grayPixel = grayValue;
    setGrayLevels(grayLevels);
}

/**
* Sets a alpha pixel value.
*
* @param alpha: A alpha value of the pixel.
*/
void Pixel::setAlphaPixelValue(unsigned char alpha) {

    this->alpha = alpha;
}

/**
* Sets a black and white pixel value.
*
* @param bw: A black-and-white value of the pixel.
*/
void Pixel::setBlackAndWhitePixelValue(unsigned char bw) {

    this->bw = bw;
}

/**
* Sets a gray leves of the pixel.
*
* @param grayLevels: A gray levels of the pixel.
*/
void Pixel::setGrayLevels(int grayLevels){

    this->grayLevels = grayLevels;
}

/**
* Converts a pixel to gray scale.
*/
void Pixel::toGrayScale() {

    if ((getRedPixelValue() == getBluePixelValue()) && (getRedPixelValue() == getGreenPixelValue()) && (getBluePixelValue() == getGreenPixelValue())){
        setGrayPixelValue(getRedPixelValue(), 255);
    } else {
        setGrayPixelValue((getBluePixelValue()*0.114) + (getGreenPixelValue()*0.587) + (getRedPixelValue()*0.299), 255);
    }
    toBlackAndWhite(getGrayLevels());
}

/**
* Converts a image to black-and-white.
*/
void Pixel::toBlackAndWhite(u_int16_t grayLevels) {
    if (getGrayPixelValue() > (grayLevels / 2))
        setBlackAndWhitePixelValue(255);
    else
        setBlackAndWhitePixelValue(0);
}

/**
* Gets a gray level.
*
* @return A gray level of the pixel.
*/
int Pixel::getGrayLevels() const{

    return grayLevels;
}

/**
* Gets a gray pixel value.
*
* @return A gray pixel value.
*/
float Pixel::getGrayPixelValue() const {

    return grayPixel;
}

/**
* Gets a red pixel value.
*
* @return A red pixel value.
*/
unsigned char Pixel::getRedPixelValue() const {

    return r;
}

/**
* Gets a green pixel value.
*
* @return A green pixel value.
*/
unsigned char Pixel::getGreenPixelValue() const {

    return g;
}

/**
* Gets blue pixel value.
*
* @return The blue value of the pixel.
*/
unsigned char Pixel::getBluePixelValue() const {

    return b;
}

/**
* Gets a alpha pixel of the value.
*
* @return A alpha pixel of the value.
*/
unsigned char Pixel::getAlphaPixelValue() const {

    return alpha;
}

/**
* Gets a black and white value of the pixel.
*
* @return A black and white pixel of the value.
*/
unsigned char Pixel::getBlackAndWhitePixelValue() const {

    return bw;
}

/**
* Clones a pixel.
*
* @return A cloned pixel.
*/
Pixel * Pixel::clone() const {
    return new Pixel(*this);
}

/**
* Makes a comparison between two pixels.
*
* @return True if the pixels are equal.
*/
bool Pixel::isEqual(const Pixel & p) const {
    return ((p.getRedPixelValue() == r) &&
            (p.getGreenPixelValue() == g) &&
            (p.getBluePixelValue() == b) &&
            (p.getGrayPixelValue() == grayPixel) &&
            (p.getAlphaPixelValue() == alpha));
}
