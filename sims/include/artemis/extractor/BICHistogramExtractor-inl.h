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
 * This file implements the Normalized BIC Histogram class.
 */


template< class SignatureType, class DataObjectType >
BICHistogramExtractor<SignatureType, DataObjectType>::BICHistogramExtractor(u_int16_t value) {
    setNumFeatures(value);
}

template< class SignatureType, class DataObjectType >
BICHistogramExtractor<SignatureType, DataObjectType>::~BICHistogramExtractor() {
}

template< class SignatureType, class DataObjectType >
u_int16_t BICHistogramExtractor<SignatureType, DataObjectType>::getNumFeatures() {
    return numFeatures;
}

template< class SignatureType, class DataObjectType >
void BICHistogramExtractor<SignatureType, DataObjectType>::setNumFeatures(u_int16_t value) {
    numFeatures = value;
}

/**
* Generates a Normalized BIC Histogram from the image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template< class SignatureType, class DataObjectType>
void BICHistogramExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error){
	
    int8_t moves[4][2] = { {0,1},
						{0,-1},
						{1,0},
						{-1,0}
						};
    size_t i;
    int32_t idxPixelValue;
    double normalizer;
    double * featureVector = NULL;

    //Get the rate to normalize each pixel value
    normalizer = pow(2, (float) image.getBitsPerPixel()) / getNumFeatures();

    //The number of bins exceed the number of colors?
    if (normalizer < 1)
        throw std::runtime_error("Incorrect number of bins.");

    featureVector = new double[2*getNumFeatures()];
    for (i = 0; i < 2*getNumFeatures(); i++) {
        featureVector[i] = 0;
    }

    //Start extracting the histogram.
    //Read each pixel in the image, summing the color ocurrences.
    for (size_t x = 0; x < image.getWidth(); x++) {
        for (size_t y = 0; y < image.getHeight(); y++) {
            //Resize the pixel value according with the Histogram size.
            idxPixelValue = (int32_t) ((image.getPixel(x, y).getGrayPixelValue() / normalizer));
            int border = 0;
            for(int i=0; !border && i<4; i++){
                int32_t newX = x+moves[i][0];
                int32_t newY = y+moves[i][1];
                if(newX<0 || newY<0 || newX >= image.getWidth() || newY>=image.getHeight()){
					continue;
				}
                int32_t newValue = (int32_t) ((image.getPixel(newX, newY).getGrayPixelValue() / normalizer));
				if(idxPixelValue != newValue){
					border = 1;
					break;
				}
			}
            
            if (border*getNumFeatures() + idxPixelValue >= 2*getNumFeatures()) {
                delete[] featureVector;
                throw std::runtime_error("OUT_OF_BOUNDS");
            }
            featureVector[border*getNumFeatures() + idxPixelValue]++;
        }
    }

    //Normalize the histogram in the range [0, 1], making the sum of all bin
    //values equals to 1.
    //Populate the sign object with the features computed
    sign.resize(2*getNumFeatures());
    for (i = 0; i < 2*getNumFeatures(); i++) {
            sign[i] = featureVector[i] / image.getSize();
        }

    delete[] featureVector;
}
