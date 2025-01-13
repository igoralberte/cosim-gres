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
* Constructor.
*/


template< class SignatureType, class DataObjectType >
Wavelets<SignatureType, DataObjectType>::Wavelets(u_int16_t levels, u_int16_t vectorComposition) {
    multiSquare = NULL;
    
    setLevels(levels);
    setVectorComposition(vectorComposition);

    mean.clear();
    entropy.clear();
    energy.clear();
    variance.clear();
}

/**
* Destructor.
*/
template< class SignatureType, class DataObjectType >
Wavelets<SignatureType, DataObjectType>::~Wavelets() {
    if (multiSquare != NULL){
        delete (multiSquare);
    }
    clearFeatures();
}

/**
* Clear the characteristics vector.
*/
template< class SignatureType, class DataObjectType >
void Wavelets<SignatureType, DataObjectType>::clearFeatures() {

    mean.clear();
    entropy.clear();
    energy.clear();
    variance.clear();
}


/**
* Sets a image what will be processed.
*
* @param *image  The image what will be cloned.
*/
template< class SignatureType, class DataObjectType >
void Wavelets<SignatureType, DataObjectType>::setMultiSquareImage(DataObjectType *image) {

    multiSquare = image->clone();
}

/**
* Gets a MeanFeatures vector.
*
* @return The mean features vector.
*/
template< class SignatureType, class DataObjectType >
std::vector<double> Wavelets<SignatureType, DataObjectType>::getMeanFeatures() {

    return mean;
}

/**
* Gets a EntropyFeatures vector.
*
* @return The entropy features vector.
*/
template< class SignatureType, class DataObjectType >
std::vector<double> Wavelets<SignatureType, DataObjectType>::getEntropyFeatures() {

    return entropy;
}

/**
* Gets a EnergyFeatures vector.
*
* @return The energy features vector.
*/
template< class SignatureType, class DataObjectType >
std::vector<double> Wavelets<SignatureType, DataObjectType>::getEnergyFeatures() {

    return energy;
}

/**
* Gets a VarianceFeatures vector.
*
* @return The variance features vector.
*/
template< class SignatureType, class DataObjectType >
std::vector<double> Wavelets<SignatureType, DataObjectType>::getVarianceFeatures() {

    return variance;
}

/**
* Gets the multi squared image.
*
* @return The processed image.
*/
template< class SignatureType, class DataObjectType >
DataObjectType* Wavelets<SignatureType, DataObjectType>::getMultiSquareImage() {

    return multiSquare;
}

/**
* Gets the number of levels.
*
* @return The number of levels.
*/
template< class SignatureType, class DataObjectType >
u_int16_t Wavelets<SignatureType, DataObjectType>::getLevels() {

    return levels;
}

/**
* Gets the composition of the signature.
*
* @return The composition of the signature.
*/
template< class SignatureType, class DataObjectType >
u_int16_t Wavelets<SignatureType, DataObjectType>::getComposition() {

    return vectorComposition;
}

template< class SignatureType, class DataObjectType >
void Wavelets<SignatureType, DataObjectType>::setVectorComposition(u_int16_t vectorComposition) {

    this->vectorComposition = vectorComposition;
}

template< class SignatureType, class DataObjectType >
void Wavelets<SignatureType, DataObjectType>::setLevels(u_int16_t levels) {

    this->levels = levels;
}


/**
 * Mount the full signature based on wanted characteristics.
 *
 * @param *image The image trated by Wavelets
 *
 * @return The wanted signature.
 */
template< class SignatureType, class DataObjectType >
SignatureType Wavelets<SignatureType, DataObjectType>::extractSignature(DataObjectType *image) {

    u_int32_t sumPixels;
    size_t x0, x1, y0, y1;
    size_t i = 0;

    for (size_t level = 1; level <= levels; level++) {
        for (size_t width = 1; width <= 3; width++) {
            switch (width) {
            case 1:
                y0 = 0;
                y1 = (u_int16_t)((u_int16_t)image->getHeight() / (u_int16_t)(2 * level));
                x0 = (u_int16_t)((u_int16_t)image->getWidth() / (u_int16_t)(2 * level));
                x1 = (u_int16_t)((u_int16_t)image->getWidth() / (u_int16_t)level);
                break;
            case 2:
                y0 = (u_int16_t)((u_int16_t)image->getHeight() / (u_int16_t)(2 * level));
                y1 = (u_int16_t)((u_int16_t)image->getHeight() / (u_int16_t)(level));
                x0 = (u_int16_t)((u_int16_t)image->getWidth() / (u_int16_t)(2 * level));
                x1 = (u_int16_t)((u_int16_t)image->getWidth() / (u_int16_t)level);
                break;
            case 3:
                y0 = (u_int16_t)((u_int16_t)image->getHeight() / (u_int16_t)(2 * level));
                y1 = (u_int16_t)((u_int16_t)image->getHeight() / (u_int16_t)(level));
                x0 = 0;
                x1 = (u_int16_t)((u_int16_t)image->getWidth() / (u_int16_t)(2 * level));
                break;
            }

            sumPixels = 0;
            mean.push_back(0);
            entropy.push_back(0);
            energy.push_back(0);
            variance.push_back(0);

            for (size_t y = y0; y < y1-1; y++) {
                for (size_t x = x0; x < x1-1; x++) {
                    if (image->getPixel(x, y).getGrayPixelValue() != 0) {
                        entropy[i] = entropy[i] + (image->getPixel(x, y).getGrayPixelValue()) * log(image->getPixel(x, y).getGrayPixelValue()) / log(2);
                    }
                    energy[i] = energy[i] + image->getPixel(x, y).getGrayPixelValue() * image->getPixel(x, y).getGrayPixelValue();
                    mean[i] = mean[i] + image->getPixel(x, y).getGrayPixelValue();
                    variance[i] = variance[i] + (image->getPixel(x, y).getGrayPixelValue() - mean[i])*(image->getPixel(x, y).getGrayPixelValue() - mean[i]);
                    sumPixels++;
                }
            }

            mean[i] = mean[i] / sumPixels;
            entropy[i] = entropy[i] / sumPixels;
            energy[i] = energy[i] / sumPixels;
            variance[i] = variance[i] / sumPixels;

            i++;
        }

        if (level == (levels)) {
            y0 = 0;
            y1 = (image->getHeight() / (2 * level));
            x0 = 0;
            x1 = (image->getWidth() / (2 * level));

            sumPixels = 0;
            mean.push_back(0);
            entropy.push_back(0);
            energy.push_back(0);
            variance.push_back(0);

            for (size_t y = y0; y < y1; y++) {
                for (size_t x = x0; x < x1; x++) {
                    if (image->getPixel(x, y).getGrayPixelValue() != 0) {
                        entropy[i] = entropy[i] + (image->getPixel(x, y).getGrayPixelValue()) * log(image->getPixel(x, y).getGrayPixelValue()) / log(2);
                    }
                    energy[i] = energy[i] + image->getPixel(x, y).getGrayPixelValue() * image->getPixel(x, y).getGrayPixelValue();
                    mean[i] = mean[i] + image->getPixel(x, y).getGrayPixelValue();
                    variance[i] = variance[i] + (image->getPixel(x, y).getGrayPixelValue() - mean[i])*(image->getPixel(x, y).getGrayPixelValue() - mean[i]);
                    sumPixels++;
                }
            }
            mean[i] = mean[i] / sumPixels;
            entropy[i] = entropy[i] / sumPixels;
            energy[i] = energy[i] / sumPixels;
            variance[i] = variance[i] / sumPixels;
            i++;

        }
    }
    std::vector<double> result;

    for (size_t j = 0; j < mean.size(); j++) {
        if (!std::isnan(mean[j])){
            result.push_back(mean[j]);
        } else {
            result.push_back(0.0);
        }
    }

    for (size_t j = 0; j < entropy.size(); j++) {
        if (!std::isnan(entropy[j])){
            result.push_back(entropy[j]);
        } else {
            result.push_back(0.0);
        }
    }

    for (size_t j = 0; j < energy.size(); j++) {
        if (!std::isnan(energy[j])){
            result.push_back(energy[j]);
        } else {
            result.push_back(0.0);
        }
    }

    for (size_t j = 0; j < variance.size(); j++) {
        if (!std::isnan(variance[j])){
            result.push_back(variance[j]);
        } else {
            result.push_back(0.0);
        }
    }


    SignatureType sign;
    double max = DBL_MIN;

    sign.resize(result.size(), 0.0);

    for (size_t x = 0; x < result.size(); x++){
        sign[x] = result[x];
        if (sign[x] > max){
            max = sign[x];
        }
    }

    for (size_t x = 0; x < sign.size(); x++){
        sign[x] = ((double) sign[x]) / ((double) max);
    }

    mean.clear();
    entropy.clear();
    energy.clear();
    variance.clear();
    result.clear();

    return sign;
}

/**
* Normalize the imagens between 0 and 1.
* It's needed for each new square, delimited by (x0, y0) and (x1, y1)
*
* @param image: A pointer to divided image
* @param x0: Start position x of the square
* @param x1: End position x of the square
* @param y0: Start position y of the square
* @param y1: End position y of the square
* @throw std::range_error : If the coordinates (x0, y0) e (x1, y1) are incorrects.
*/
template< class SignatureType, class DataObjectType >
void Wavelets<SignatureType, DataObjectType>::NormalizeImage(DataObjectType *image, size_t x0, size_t x1, size_t y0, size_t y1) throw (std::range_error) {

    if ((x0 < 0) || (x1 >= image->getWidth()) || (y0 < 0) || (y1 >= image->getHeight())) {
        throw std::range_error("Incorret Values for normalization");
    }

    double maxValue = DBL_MIN;
    double minValue = DBL_MAX;

    for (size_t y = y0; y < y1; y++) {
        for (size_t x = x0; x < x1; x++) {
            if (maxValue < image->getPixel(x, y).getGrayPixelValue()) {
                maxValue = image->getPixel(x, y).getGrayPixelValue();
            }
            if (minValue > image->getPixel(x, y).getGrayPixelValue()) {
                minValue = image->getPixel(x, y).getGrayPixelValue();
            }
        }
    }

    double dif = maxValue - minValue;

    //Is another scale depending of the max and the min values of the square. The difference between
    //the max and the min creates the rule.
    for (size_t y = y0; y < y1; y++) {
        for (size_t x = x0; x < x1; x++) {
            Pixel pp(image->getPixel(x, y));
            pp.setGrayPixelValue(abs(255 * fabs((image->getPixel(x, y).getGrayPixelValue() - minValue)) / dif), 256);
            image->setPixel(x, y, pp);
        }
    }
}

