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
*  Creates the big matrix what will be relationed with co-ocurrence matrix.
*
* @throw std::runtime_error If the memory space are full.
*/
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::createCoocurrenceMatrix() throw (std::runtime_error) {

    try {
        cm = new double***[getNumDistances()];
        for (u_int16_t i = 0; i < getNumDistances(); i++) {
            cm[i] = new double**[getNumAngles()];
            for (u_int16_t j = 0; j < getNumAngles(); j++) {
                cm[i][j] = new double*[getNumBins()];
                for (u_int16_t k = 0; k < getNumBins(); k++) {
                    cm[i][j][k] = new double[getNumBins()];
                    for (u_int16_t l = 0; l < getNumBins(); l++) {
                        cm[i][j][k][l] = 0;
                    }
                }
            }
        }

    } catch (std::bad_alloc&) {
        throw std::runtime_error("Insuficient heap size to allocate Fetures Matrix");
    } catch (...) {
        throw std::runtime_error( "Error into allocate memory size");
    }
}

/**
* Unlock the memory space utilized by Co-ocurrence Matrix.
*/
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::deleteCoocurrenceMatrix() {

    if (cm != NULL){
        for (u_int16_t i = 0; i < getNumDistances(); i++) {
            for (u_int16_t j = 0; j < getNumAngles(); j++) {
                for (u_int16_t k = 0; k < getNumBins(); k++) {
                    delete[] cm[i][j][k];
                }
                delete[] cm[i][j];
            }
            delete[] cm[i];
        }
        delete cm;

        cm = NULL;
    }
}

/**
* Sets the values on the big co-ocurrence matrix.
*
* @param distance    The distance of the pixels (First value of the four).
* @param angle       The angle between the pixels (Second value of the four).
* @param numBins_x   The value numBins_x of the co-ocurrence matrix (Third value of the four).
* @param numBins_y   The value numBins_x of the co-ocurrence matrix (Fourthy value of the four).
*
* @throw If the position (distance, angle, numBins_x, numBins_y) in not valid.
*/
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::setCoocurrenceValue(u_int16_t distance, u_int16_t angle, u_int16_t numBins_x, u_int16_t numBins_y, double value) throw (std::range_error) {

    if ((angle > getNumAngles()) || (distance > getNumDistances())
            || (numBins_x > getNumBins()) || (numBins_y > getNumBins())
            || (angle < 0) || (distance < 0) || (numBins_x < 0) || (numBins_y < 0)){
        throw std::range_error("The requested (distance, angle, x, y) is out of bounds");        
    }

    cm[distance][angle][numBins_x][numBins_y] = value;
}



//
/**
 * Normalize the image gray values.
 *
 * @param height   The height of the Image
 * @param width    The width of the Image
 */
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::normalizeCoocurrenceMatrix(u_int16_t height, u_int16_t width) {

    double total;
    // normalization
    for (u_int16_t line = 0; line < getNumBins(); line++) {
        for (u_int16_t column = 0; column < getNumBins(); column++) {
            for (u_int16_t distance = 1; distance <= getNumDistances(); distance++) {
                // 0
                total = 2 * height * (width - distance);
                cm[distance - 1][0][line][column] /= total;
                // 45 and 135
                total = 2 * (height - distance)*(width - distance);
                cm[distance - 1][1][line][column] /= total;
                cm[distance - 1][3][line][column] /= total;
                // 90
                total = 2 * (height - distance) * width;
                cm[distance - 1][2][line][column] /= total;
            }
        }
    }
}


/**
 * Construtor
 */
template<class SignatureType, class DataObjectType>
HaralickExtractor<SignatureType, DataObjectType>::HaralickExtractor(u_int16_t numDistances, u_int16_t numAngles, u_int16_t typeCharacteristics, u_int16_t typeHaralickExtractor, u_int16_t numBins) {
	cm = NULL;
    setNumDistances(numDistances);
    setNumAngles(numAngles);
    setTypeCharacteristics(typeCharacteristics);
    setTypeHaralickExtractor(typeHaralickExtractor);
    setNumBins(numBins);

    varianceMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
    entropyMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
    uniformityMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
    homogeneityMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
    moment3thMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
    reverseVarianceMatrix = new HaralickFeaturesMatrix(getNumDistances(), getNumAngles());
}

/**
* Sets the used num of bins.
*
* @param numBins The used number of bins.
*/
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::setNumBins(u_int16_t numBins) {
    this->numBins = numBins;
}

/**
* Sets the Haralick Type - Angle or distance.
*
* @param typeHaralickExtractor The Haralick Type extractor.
*/
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::setTypeHaralickExtractor(u_int16_t typeHaralickExtractor) {
    this->typeHaralickExtractor = typeHaralickExtractor;
}

/**
* Sets the number of valid characteristics.
*
* @param numBins The valid characteristics.
*/
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::setTypeCharacteristics(u_int16_t typeCharacteristics) {
    this->typeCharacteristics = typeCharacteristics;
}

/**
* Sets the used num of angles.
*
* @param numBins The used number of angles.
*/
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::setNumAngles(u_int16_t numAngles) {
    this->numAngles = numAngles;
}

/**
* Sets the used num of distances.
*
* @param numBins The used number of distances.
*/
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::setNumDistances(u_int16_t numDistances) {
    this->numDistances = numDistances;
}

/**
* Destructor
*/
template<class SignatureType, class DataObjectType>
HaralickExtractor<SignatureType, DataObjectType>::~HaralickExtractor() {

    deleteCoocurrenceMatrix();

    if (varianceMatrix != NULL)
        delete (varianceMatrix);
    if (entropyMatrix != NULL)
        delete (entropyMatrix);
    if (uniformityMatrix != NULL)
        delete (uniformityMatrix);
    if (homogeneityMatrix != NULL)
        delete (homogeneityMatrix);
    if (moment3thMatrix != NULL)
        delete (moment3thMatrix);
    if (reverseVarianceMatrix != NULL)
        delete (reverseVarianceMatrix);
}


/**
* Gets the utilized number of distances.
*
* @return The number of the utilized distances.
*/
template<class SignatureType, class DataObjectType>
u_int16_t HaralickExtractor<SignatureType, DataObjectType>::getNumDistances() {

    return numDistances;
}

/**
* Gets the utilized number of angles.
*
* @return The number of utilized angles.
*/
template<class SignatureType, class DataObjectType>
u_int16_t HaralickExtractor<SignatureType, DataObjectType>::getNumAngles() {

    return numAngles;
}

/**
* Gets the number of the utilized characteristics utilized to compose the signature.
*
* @return The utilized characteristics.
*/
template<class SignatureType, class DataObjectType>
u_int16_t HaralickExtractor<SignatureType, DataObjectType>::getTypeCharacteristics() {

    return typeCharacteristics;
}

/**
* Gets the type of the extractor.
*
* @return The type of the extractor.
*/
template<class SignatureType, class DataObjectType>
u_int16_t HaralickExtractor<SignatureType, DataObjectType>::getTypeHaralickExtractor() {

    return typeHaralickExtractor;
}

/**
* Gets the number of the utilized bins.
*
* @return The number of the utilized bins.
*/
template<class SignatureType, class DataObjectType>
u_int16_t HaralickExtractor<SignatureType, DataObjectType>::getNumBins() {

    return numBins;
}

/**
* Gets the value on the big matrix.
*
* @param distance                The distance (First param of the matrix)
* @param angle                   The angle (Second param of the matrix)
* @param numBins_x               The numBins_x (Third param of the matrix)
* @param numBins_y               The numBins_y (Fourthy param of the matrix)
*
* @throw std::range_error    If the (distance, angle, numBins_x, numBins_y) are not valid.
*
* @return The value on the big co-ocurrence matrix.
*/
template<class SignatureType, class DataObjectType>
double HaralickExtractor<SignatureType, DataObjectType>::getCoocurrenceValue(u_int16_t distance, u_int16_t angle, u_int16_t numBins_x, u_int16_t numBins_y) throw (std::range_error) {
    if ((angle > getNumAngles()) || (distance > getNumDistances())
            || (numBins_x > getNumBins()) || (numBins_y > getNumBins())
            || (angle < 0) || (distance < 0) || (numBins_x < 0) || (numBins_y < 0)){
        throw std::range_error("The requested (distance, angle, x, y) is out of bounds");
    }
    return cm[distance][angle][numBins_x][numBins_y];
}

/**
* Load the co-ocurrence matrix
*
* @param *image The image that will be loaded u_int16_to co-ocurrence matrix
*/
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::loadImageValues(const Image & image) {

    //Build a clone image with gray value depth reduced
    Image img;
    img.createPixelMatrix(image.getWidth(), image.getHeight());

    double max = pow(2, (double) image.getBitsPerPixel());

    for (u_int16_t x = 0; x < image.getWidth(); x++) {
        for (u_int16_t y = 0; y < image.getHeight(); y++) {
            Pixel p(image.getPixel(x, y));
            p.setGrayPixelValue((image.getPixel(x, y).getGrayPixelValue() * getNumBins()) / max, max / getNumBins());
            img.setPixel(x, y, p);
        }
    }
    //Compute the values of the co-ocurrence matrix
    for (u_int16_t y = 0; y < img.getHeight(); y++) {
        for (u_int16_t x = 0; x < img.getWidth(); x++) {
            for (u_int16_t distance = 1; distance <= getNumDistances(); distance++) {
                if ((((int32_t)(x - distance)) >= 0) && ((x + distance) < img.getWidth())) {
                    setCoocurrenceValue(distance - 1, 0,
                                        (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                        (u_int16_t) (img.getPixel(x + distance, y).getGrayPixelValue()),
                                        getCoocurrenceValue(distance - 1, 0,
                                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (u_int16_t) (img.getPixel(x + distance, y).getGrayPixelValue())) + 1);
                    setCoocurrenceValue(distance - 1, 0,
                                        (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                        (u_int16_t) (img.getPixel(x - distance, y).getGrayPixelValue()),
                                        getCoocurrenceValue(distance - 1, 0,
                                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (u_int16_t) (img.getPixel(x - distance, y).getGrayPixelValue())) + 1);

                } else {
                    if (((int32_t)(x - distance)) < 0) {
                        setCoocurrenceValue(distance - 1, 0,
                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                            (u_int16_t) (img.getPixel(x + distance, y).getGrayPixelValue()),
                                            getCoocurrenceValue(distance - 1, 0,
                                                                (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (u_int16_t) (img.getPixel(x + distance, y).getGrayPixelValue())) + 1);

                    } else {
                        setCoocurrenceValue(distance - 1, 0,
                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                            (u_int16_t) (img.getPixel(x - distance, y).getGrayPixelValue()),
                                            getCoocurrenceValue(distance - 1, 0,
                                                                (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (u_int16_t) (img.getPixel(x - distance, y).getGrayPixelValue())) + 1);
                    }
                }// End 0
                //Starts 90o
                if ((((int32_t)(y - distance)) >= 0) && ((y + distance) < img.getHeight())) {
                    setCoocurrenceValue(distance - 1, 2,
                                        (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                        (u_int16_t) (img.getPixel(x, y + distance).getGrayPixelValue()),
                                        getCoocurrenceValue(distance - 1, 2,
                                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (u_int16_t) (img.getPixel(x, y + distance).getGrayPixelValue())) + 1);
                    setCoocurrenceValue(distance - 1, 2,
                                        (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                        (u_int16_t) (img.getPixel(x, y - distance).getGrayPixelValue()),
                                        getCoocurrenceValue(distance - 1, 2,
                                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (u_int16_t) (img.getPixel(x, y - distance).getGrayPixelValue())) + 1);
                } else {
                    if (((int32_t)(y - distance)) < 0) {
                        setCoocurrenceValue(distance - 1, 2,
                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                            (u_int16_t) (img.getPixel(x, y + distance).getGrayPixelValue()),
                                            getCoocurrenceValue(distance - 1, 2,
                                                                (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (u_int16_t) (img.getPixel(x, y + distance).getGrayPixelValue())) + 1);
                    } else {
                        setCoocurrenceValue(distance - 1, 2,
                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                            (u_int16_t) (img.getPixel(x, y - distance).getGrayPixelValue()),
                                            getCoocurrenceValue(distance - 1, 2,
                                                                (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (u_int16_t) (img.getPixel(x, y - distance).getGrayPixelValue())) + 1);
                    }
                }//End 90o

                // Start 135o
                if ((((int32_t)(x - distance)) >= 0) && ((x + distance) < img.getWidth()) &&
                        (((int32_t)(y - distance)) >= 0) && ((y + distance) < img.getHeight())) {
                    setCoocurrenceValue(distance - 1, 3,
                                        (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                        (u_int16_t) (img.getPixel(x - distance, y - distance).getGrayPixelValue()),
                                        getCoocurrenceValue(distance - 1, 3,
                                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (u_int16_t) (img.getPixel(x - distance, y - distance).getGrayPixelValue())) + 1);
                    setCoocurrenceValue(distance - 1, 3,
                                        (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                        (u_int16_t) (img.getPixel(x + distance, y + distance).getGrayPixelValue()),
                                        getCoocurrenceValue(distance - 1, 3,
                                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (u_int16_t) (img.getPixel(x + distance, y + distance).getGrayPixelValue())) + 1);
                } else {
                    if (((((int32_t)(x - distance)) < 0) && ((y + distance) < img.getHeight())) ||
                            ((((int32_t)(y - distance)) < 0) && ((x + distance) < img.getWidth()))) {
                        setCoocurrenceValue(distance - 1, 3,
                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                            (u_int16_t) (img.getPixel(x + distance, y + distance).getGrayPixelValue()),
                                            getCoocurrenceValue(distance - 1, 3,
                                                                (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (u_int16_t) (img.getPixel(x + distance, y + distance).getGrayPixelValue())) + 1);
                    }
                    if ((((x + distance) >= img.getWidth()) && (((int32_t)(y - distance)) >= 0)) ||
                            (((y + distance) >= img.getHeight()) && (((int32_t)(x - distance)) >= 0))) {
                        setCoocurrenceValue(distance - 1, 3,
                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                            (u_int16_t) (img.getPixel(x - distance, y - distance).getGrayPixelValue()),
                                            getCoocurrenceValue(distance - 1, 3,
                                                                (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (u_int16_t) (img.getPixel(x - distance, y - distance).getGrayPixelValue())) + 1);
                    }
                }// End 135o

                // Start 45o
                if ((((int32_t)(x - distance)) >= 0) && ((x + distance) < img.getWidth()) &&
                        (((int32_t)(y - distance)) >= 0) && ((y + distance) < img.getHeight())) {
                    setCoocurrenceValue(distance - 1, 1,
                                        (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                        (u_int16_t) (img.getPixel(x + distance, y - distance).getGrayPixelValue()),
                                        getCoocurrenceValue(distance - 1, 1,
                                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (u_int16_t) (img.getPixel(x + distance, y - distance).getGrayPixelValue())) + 1);
                    setCoocurrenceValue(distance - 1, 1,
                                        (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                        (u_int16_t) (img.getPixel(x - distance, y + distance).getGrayPixelValue()),
                                        getCoocurrenceValue(distance - 1, 1,
                                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                            (u_int16_t) (img.getPixel(x - distance, y + distance).getGrayPixelValue())) + 1);
                } else {
                    if (((((int32_t)(x - distance)) < 0) && (((int32_t)(y - distance)) >= 0)) ||
                            (((y + distance) >= img.getHeight()) && ((x + distance) < img.getWidth()))) {
                        setCoocurrenceValue(distance - 1, 1,
                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                            (u_int16_t) (img.getPixel(x + distance, y - distance).getGrayPixelValue()),
                                            getCoocurrenceValue(distance - 1, 1,
                                                                (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (u_int16_t) (img.getPixel(x + distance, y - distance).getGrayPixelValue())) + 1);
                    }
                    if ((((x + distance) >= img.getWidth()) && ((y + distance) < img.getHeight())) ||
                            ((((int32_t)(y - distance)) < 0) && (((int32_t)(x - distance)) >= 0))) {
                        setCoocurrenceValue(distance - 1, 1,
                                            (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                            (u_int16_t) (img.getPixel(x - distance, y + distance).getGrayPixelValue()),
                                            getCoocurrenceValue(distance - 1, 1,
                                                                (u_int16_t) (img.getPixel(x, y).getGrayPixelValue()),
                                                                (u_int16_t) (img.getPixel(x - distance, y + distance).getGrayPixelValue())) + 1);
                    }
                }// End 45o
            }// End
        }// End
    }// End
}

/**
 * Gets the signature based on characteristic defined by the user: Entropy or/and variancy, etc....
 *
 * @return The selected signature.
 */
template<class SignatureType, class DataObjectType>
double* HaralickExtractor<SignatureType, DataObjectType>::getSelectedSignature() {

    double *result = NULL;

    if (getTypeCharacteristics() == VARIANCE) {
        if (getTypeHaralickExtractor() == 0) {
            result = varianceMatrix->getDistanceMeanFeatures();
        } else {
            result = varianceMatrix->getAngleMeanFeatures();
        }
    }

    if (getTypeCharacteristics() == ENTROPY) {
        if (getTypeHaralickExtractor() == 0) {
            result = entropyMatrix->getDistanceMeanFeatures();
        } else {
            result = entropyMatrix->getAngleMeanFeatures();
        }
    }

    if (getTypeCharacteristics() == UNIFORMITY) {
        if (getTypeHaralickExtractor() == 0) {
            result = uniformityMatrix->getDistanceMeanFeatures();
        } else {
            result = uniformityMatrix->getAngleMeanFeatures();
        }
    }

    if (getTypeCharacteristics() == HOMOGEINITY) {
        if (getTypeHaralickExtractor() == 0) {
            result = homogeneityMatrix->getDistanceMeanFeatures();
        } else {
            result = homogeneityMatrix->getAngleMeanFeatures();
        }
    }

    if (getTypeCharacteristics() == MOMENT_3TH) {
        if (getTypeHaralickExtractor() == 0) {
            result = moment3thMatrix->getDistanceMeanFeatures();
        } else {
            result = moment3thMatrix->getAngleMeanFeatures();
        }
    }

    if (getTypeCharacteristics() == REVERSE_VARIANCE) {
        if (getTypeHaralickExtractor() == 0) {
            result = reverseVarianceMatrix->getDistanceMeanFeatures();
        } else {
            result = reverseVarianceMatrix->getAngleMeanFeatures();
        }
    }

    if (getTypeCharacteristics() == ALL_CHARACTERISTICS) {

        if (getTypeHaralickExtractor() == 0) {
            u_int16_t distanceMeanSize = varianceMatrix->getDistanceMeanSize();
            u_int16_t sizeVector = 6 * distanceMeanSize;

            result = new double[sizeVector];
            double* variance = varianceMatrix->getDistanceMeanFeatures();
            double* entropy = entropyMatrix->getDistanceMeanFeatures();
            double* uniformity = uniformityMatrix->getDistanceMeanFeatures();
            double* homogeneity = homogeneityMatrix->getDistanceMeanFeatures();
            double* moment3th = moment3thMatrix->getDistanceMeanFeatures();
            double* reverseVariance = reverseVarianceMatrix->getDistanceMeanFeatures();

            for (u_int16_t i = 0; i < distanceMeanSize; i++) {
                result[i + 0 * distanceMeanSize] = variance[i];
                result[i + 1 * distanceMeanSize] = entropy[i];
                result[i + 2 * distanceMeanSize] = uniformity[i];
                result[i + 3 * distanceMeanSize] = homogeneity[i];
                result[i + 4 * distanceMeanSize] = moment3th[i];
                result[i + 5 * distanceMeanSize] = reverseVariance[i];
            }
        } else {
            u_int16_t angleMeanSize = varianceMatrix->getAngleMeanSize();
            u_int16_t sizeVector = 6 * angleMeanSize;

            result = new double[sizeVector];
            double* variance = varianceMatrix->getAngleMeanFeatures();
            double* entropy = entropyMatrix->getAngleMeanFeatures();
            double* uniformity = uniformityMatrix->getAngleMeanFeatures();
            double* homogeneity = homogeneityMatrix->getAngleMeanFeatures();
            double* moment3th = moment3thMatrix->getAngleMeanFeatures();
            double* reverseVariance = reverseVarianceMatrix->getAngleMeanFeatures();

            for (u_int16_t i = 0; i < angleMeanSize; i++) {
                result[i + 0 * angleMeanSize] = variance[i];
                result[i + 1 * angleMeanSize] = entropy[i];
                result[i + 2 * angleMeanSize] = uniformity[i];
                result[i + 3 * angleMeanSize] = homogeneity[i];
                result[i + 4 * angleMeanSize] = moment3th[i];
                result[i + 5 * angleMeanSize] = reverseVariance[i];
            }
        }
    }

    return result;

}

/**
 * Generates the Haralick's descriptor defined in the template from the
 * image provided.
 *
 * @param image The image to be processed.
 * @param[out] sign The object to store the generated normalized histogram.
 * @return The resulting ExtReturnCode.
 */
template<class SignatureType, class DataObjectType>
void HaralickExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error){
    try {
        createCoocurrenceMatrix();
    } catch (...) {
        throw std::runtime_error("Error into allocate memory size");
    }

    //
    // The original image are not affected
    //
    loadImageValues(image);
    for (u_int16_t distance = 0; distance < getNumDistances(); distance++) {
        for (u_int16_t angle = 0; angle < getNumAngles(); angle++) {

            // Sum without princ diag
            for (u_int16_t line = 0; line < getNumBins() - 1; line++) {
                for (u_int16_t column = (line + 1); column < getNumBins(); column++) {

                    varianceMatrix->setHaralickFeatureValue(distance, angle,
                                                            varianceMatrix->getHaralickFeatureValue(distance, angle) +
                                                            2 * (cm[distance][angle][line][column]*(line - column)*(line - column)));
                    if (cm[distance][angle][line][column] != 0) {
                        entropyMatrix->setHaralickFeatureValue(distance, angle,
                                                               entropyMatrix->getHaralickFeatureValue(distance, angle) -
                                                               2 * (cm[distance][angle][line][column] * log10(cm[distance][angle][line][column])));
                    }
                    uniformityMatrix->setHaralickFeatureValue(distance, angle,
                                                              uniformityMatrix->getHaralickFeatureValue(distance, angle) +
                                                              2 * (cm[distance][angle][line][column] * cm[distance][angle][line][column]));
                    homogeneityMatrix->setHaralickFeatureValue(distance, angle,
                                                               homogeneityMatrix->getHaralickFeatureValue(distance, angle) +
                                                               2 * (cm[distance][angle][line][column] / (1 + abs(line - column))));
                    moment3thMatrix->setHaralickFeatureValue(distance, angle,
                                                             moment3thMatrix->getHaralickFeatureValue(distance, angle) +
                                                             2 * (cm[distance][angle][line][column]*(line - column)*(line - column) * abs(line - column)));
                    reverseVarianceMatrix->setHaralickFeatureValue(distance, angle,
                                                                   reverseVarianceMatrix->getHaralickFeatureValue(distance, angle) +
                                                                   2 * (cm[distance][angle][line][column] / ((line - column)*(line - column))));
                }
            }
            // Sum diag princ
            for (u_int16_t line = 0; line < getNumBins(); line++) {
                if (cm[distance][angle][line][line] != 0) {
                    entropyMatrix->setHaralickFeatureValue(distance, angle,
                                                           entropyMatrix->getHaralickFeatureValue(distance, angle) -
                                                           cm[distance][angle][line][line] * log10(cm[distance][angle][line][line]));
                }
                uniformityMatrix->setHaralickFeatureValue(distance, angle,
                                                          uniformityMatrix->getHaralickFeatureValue(distance, angle) +
                                                          cm[distance][angle][line][line] * cm[distance][angle][line][line]);
                homogeneityMatrix->setHaralickFeatureValue(distance, angle,
                                                           homogeneityMatrix->getHaralickFeatureValue(distance, angle) +
                                                           cm[distance][angle][line][line]);
            }
        }
    }

    u_int16_t size = 0;
    if (getTypeHaralickExtractor() == 0) {
        size = varianceMatrix->getDistanceMeanSize();
    } else {
        size = varianceMatrix->getAngleMeanSize();
    }
    if (getTypeCharacteristics() == ALL_CHARACTERISTICS) {
        size = 6 * size;
    }

    if (sign.size() != size)
        sign.resize(size);

    for (u_int16_t i = 0; i < size; i++) {
        sign[i] = getSelectedSignature()[i];
    }

    double max = DBL_MIN;
    for (u_int16_t i = 0; i < size; i++) {
        if (sign[i] > max)
            max = sign[i];
    }

    if (max == 0.0)
        max = 1.0;

    for (u_int16_t i = 0; i < size; i++) {
        sign[i] = ((double) sign[i]) / ((double) max);
    }

    try{
        deleteCoocurrenceMatrix();
    } catch (...) {
        throw std::runtime_error("Unable to unlock mem at Haralick Extractor");
    }
}
