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
#include "Extractor.h"

/**
* Empty class constructor.
*/
template<class SignatureType, class DataObjectType>
TotalColorHistogramExtractor<SignatureType, DataObjectType>::TotalColorHistogramExtractor(u_int16_t numFeatures) {

    this->numFeatures = numFeatures;
}

/**
* Class destructor.
*/
template<class SignatureType, class DataObjectType>
TotalColorHistogramExtractor<SignatureType, DataObjectType>::~TotalColorHistogramExtractor() {
}

/**
* Get the number of features.
*
* @return The number of features.
*/
template<class SignatureType, class DataObjectType>
u_int16_t TotalColorHistogramExtractor<SignatureType, DataObjectType>::getNumFeatures() {
    return numFeatures;
}

/**
* Generates the TotalColorHistogram descriptor defined in the template from the
* image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template< class SignatureType, class DataObjectType>
void TotalColorHistogramExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error, std::range_error){

    u_int16_t idxPixelValue;
    double * featureVector = NULL;

    //Get the rate to normalize each pixel value
    double normalizer = pow(2, (float) image.getBitsPerPixel()) / getNumFeatures();

    //The number of bins exceed the number of colors?
    if (normalizer < 1.0)
        throw std::runtime_error("Incorrect number of bins in bits per pixel from Color Histogram");

    featureVector = new double[3*numFeatures];
    for (size_t i = 0; i < 3*numFeatures; i++) {
        featureVector[i] = 0;
    }

    //Start extracting the histogram.
    //Read each pixel in the image, summing the color ocurrences.
    for (size_t x = 0; x < image.getWidth(); x++) {
        for (size_t y = 0; y < image.getHeight(); y++) {
            //for red
            //Resize the pixel value according with the Histogram size.
            idxPixelValue = (u_int16_t) ((image.getPixel(x, y).getRedPixelValue() / normalizer));
            if (idxPixelValue >= numFeatures) {
                delete[] featureVector;
                throw std::range_error("Out of bounds (red) on Color Histogram");
            }
            featureVector[idxPixelValue]++;
            //for green
            //Resize the pixel value according with the Histogram size.
            idxPixelValue = (u_int16_t) ((image.getPixel(x, y).getGreenPixelValue() / normalizer));
            if (idxPixelValue >= numFeatures) {
                delete[] featureVector;
                throw std::range_error("Out of bounds (green) on Color Histogram");
            }
            featureVector[idxPixelValue + 256]++;
            //for blue
            //Resize the pixel value according with the Histogram size.
            idxPixelValue = (u_int16_t) ((image.getPixel(x, y).getBluePixelValue() / normalizer));
            if (idxPixelValue >= numFeatures) {
                delete[] featureVector;
                throw std::range_error("Out of bounds (blue) on Color Histogram");
            }
            featureVector[idxPixelValue + 512]++;
        }
    }


    sign.resize(3*numFeatures, 0.0);
    double max = DBL_MIN;
    for (size_t i = 0; i < 3*numFeatures; i++) {
        sign[i] = ((double) featureVector[i]) / ((double) (numFeatures-1));
        if (sign[i] > max){
            max = sign[i];
        }
    }

    for (size_t i = 0; i < 3*numFeatures; i++) {
        sign[i] = ((double) sign[i]) / ((double) max);
    }

    delete[] featureVector;
}
