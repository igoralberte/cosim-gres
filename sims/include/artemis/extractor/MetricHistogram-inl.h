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
template< class SignatureType, class DataObjectType>
MetricHistogramExtractor<SignatureType, DataObjectType>::MetricHistogramExtractor(u_int16_t numFeatures, u_int16_t threshTangent, u_int16_t threshZero, u_int16_t bucketLenght) {
    setNumFeatures(numFeatures);
    setThreshTangent(threshTangent);
    setThreshZero(threshZero);
    setBucketLenght(bucketLenght);

    tradHistogram.clear();
}

/**
* Destructor.
*/
template< class SignatureType, class DataObjectType>
MetricHistogramExtractor<SignatureType, DataObjectType>::~MetricHistogramExtractor() {

    tradHistogram.clear();
}

/**
* Verify if the direction of the vector is bigger than a threshold.
*
* @param value The new value of the direction.
*
* @return A int what defines the direction of the vector.
*/
template< class SignatureType, class DataObjectType>
u_int16_t MetricHistogramExtractor<SignatureType, DataObjectType>::verifyDirection(double value) {
    if (fabs(value) < (threshZero * 0.001)) {
        return 0;
    } else if (value < 0) {
        return -1;
    } else {
        return 1;
    }
}

/**
* Verify if the inclination of the vector between two posize_ts is bigger than a threshold.
*
* @param angle 1     The angle of the first posize_t.
* @param angle 2     The angle of the second posize_t.
*
* @return True if the difference between two angles are bigger than a threshold or false.
*/
template< class SignatureType, class DataObjectType>
bool MetricHistogramExtractor<SignatureType, DataObjectType>::verifyTangent(double angle1, double angle2) {

    double a = fabs(atan(angle1) - atan(angle2));

    if (a > M_PI) {
        a = 2 * M_PI - a;
    }

    return (a < getThreshTangent());
}

/**
* Generates the MHD's descriptor defined in the template from the
* image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template< class SignatureType, class DataObjectType>
void MetricHistogramExtractor<SignatureType, DataObjectType>::generateSignature(const Image & image, SignatureType & sign) throw (std::runtime_error) {

    std::vector<double> result;

    tradHistogram.clear();

    //Starts the sum of the traditional histogram
    for (size_t i = 0; i < getNumFeatures(); i++) {
        tradHistogram.push_back(0);
    }

    //Calculates the Traditional Histogram
    for (size_t x = 0; x < image.getWidth(); x++) {
        for (size_t y = 0; y < image.getHeight(); y++) {
            u_int16_t reescaled = (((u_int16_t) image.getPixel(x, y).getGrayPixelValue())*(numFeatures / (u_int16_t) pow(2, (double) image.getBitsPerPixel())));
            if (reescaled > tradHistogram.size()){
                throw new std::runtime_error("Invalid relation Number of Features / Bits per pixel");
            }
            tradHistogram[(u_int16_t) reescaled]++;
        }
    }

    //Normalizes the Histogram
    double max = DBL_MIN;
    for (size_t i = 0; i < tradHistogram.size(); i++) {
        if (tradHistogram[i] > max)
            max = tradHistogram[i];
    }
    for (size_t i = 0; i < tradHistogram.size(); i++) {
        tradHistogram[i] = tradHistogram[i] / max;
    }

    //Calculates Metric Histogram
    u_int16_t brigthness = 0; // b Brightness value
    double ll; // Left limit of h'(x)
    double rl; // Right limit of h'(x)
    double threshold = 5E-5; // Threshold number to eliminate little variations
    double dir; // m (derivate)
    bool accept; // Accept bin

    // Add First entry if it is bigger than the th value
    while (tradHistogram[brigthness] < threshold)
        brigthness++;

    result.push_back(0.0);
    result.push_back(tradHistogram[brigthness]);

    // Process entries
    for (size_t w = brigthness + 1; w < getNumFeatures() - 2; w++) {
        // w < numFeatures-2 to ignore the last bin of normalized histogram
        accept = false;
        if (tradHistogram[w] < threshold) {
            brigthness++;
        } else {
            // Left and right limits
            ll = tradHistogram[w] - tradHistogram[w - 1];
            rl = tradHistogram[w + 1] - tradHistogram[w];

            // Get direction
            dir = verifyDirection(ll);

            // Lets evaluate the posize_t
            if (dir != verifyDirection(rl)) {
                // Inversion of direction - Local min or max
                accept = true;
            } else if (dir != 0) {
                // Same direction. Test angent limit
                accept = verifyTangent(ll, rl);
            }//end if

            if (accept) {
                // Add bin
                result.push_back(w - brigthness);
                result.push_back(tradHistogram[w]);
            } else {
                if (verifyDirection(result[result.size()-1] - tradHistogram[w]) != 0) {
                    if (((w - brigthness) - result[result.size()-2]) > bucketLenght) {
                        // Add bin because bucket len is too large
                        result.push_back(w - brigthness);
                        result.push_back(tradHistogram[w]);
                    }//end if
                }//end if
            }//end if
        }//end if
    }//end for

    sign.resize(result.size(), 0.0);
    for (size_t x = 0; x < result.size()-1; x += 2){
        sign[x] = result[x];
    }
    for (size_t x = 1; x < result.size(); x += 2){
        sign[x] = result[x];
    }

    result.clear();
}

template< class SignatureType, class DataObjectType>
void MetricHistogramExtractor<SignatureType, DataObjectType>::setBucketLenght(u_int16_t bucketLenght) {

    this->bucketLenght = bucketLenght;
}

template< class SignatureType, class DataObjectType>
void MetricHistogramExtractor<SignatureType, DataObjectType>::setThreshZero(u_int16_t threshZero) {

    this->threshZero = threshZero;
}

template< class SignatureType, class DataObjectType>
void MetricHistogramExtractor<SignatureType, DataObjectType>::setThreshTangent(u_int16_t threshTangent) {

    this->threshTangent = threshTangent;
}

template< class SignatureType, class DataObjectType>
void MetricHistogramExtractor<SignatureType, DataObjectType>::setNumFeatures(u_int16_t numFeatures) {

    this->numFeatures = numFeatures;
}

template< class SignatureType, class DataObjectType>
u_int16_t MetricHistogramExtractor<SignatureType, DataObjectType>::getBucketLenght() const {
    return bucketLenght;
}

template< class SignatureType, class DataObjectType>
u_int16_t MetricHistogramExtractor<SignatureType, DataObjectType>::getThreshZero() const {

    return threshZero;
}

template< class SignatureType, class DataObjectType>
u_int16_t MetricHistogramExtractor<SignatureType, DataObjectType>::getThreshTangent() const {

    return threshTangent;
}

template< class SignatureType, class DataObjectType>
u_int16_t MetricHistogramExtractor<SignatureType, DataObjectType>::getNumFeatures() const {

    return numFeatures;
}



