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
* @version 1.0.
*/

/**
* Generates a Scalable Color descriptor from the image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
void ScalableColorExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType &image, SignatureType &sign) throw (std::runtime_error) {



    std::vector<int32_t> histogram;
    histogram.resize(getNumFeatures(), 0.0);

    //calculate the histogram
    histogram = computeNormalizedHistogram(image, histogram);


    //quantize the histogram
    histogram = quantizeHistogram(histogram);


    //apply haar transform in the histogram
    histogram = applyHaarTransform(histogram);


    std::vector<double> new_histogram;
    new_histogram.resize(getNumFeatures());

    for(size_t i = 0; i < getNumFeatures(); i++) {
        new_histogram[i] = static_cast<double>(histogram[i])/255;
    }


    //normalize vector
    double min, max;
    max = DBL_MIN;
    min = DBL_MAX;    

    for(size_t i = 0; i < getNumFeatures(); i++){
        if(new_histogram[i] > max)
            max = new_histogram[i];
        if(new_histogram[i] < min)
            min = new_histogram[i];
    }

    //Populate the sign object with the features computed
    sign.resize(getNumFeatures(),0.0);
    for(size_t i = 0; i < getNumFeatures(); i++){
            sign[i] = (new_histogram[i]+fabs(min))/(fabs(max)+fabs(min));
    }

}  //generateSignature

/**
* Normalize the pixel values in HSV for the MPEG7 pattern of this extractor.
*
* @param hsv_pixel The pixel values.
* @param[out] hsv_pixel The pixel values after normalization.
* @param[out] index The index of the pixel value into descriptor.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
void ScalableColorExtractor<SignatureType, DataObjectType>::quantizeScalableUniform(u_int16_t hsv_pixel[], int32_t *index) {

    hsv_pixel[0] = (hsv_pixel[0]*hue)/getNumFeatures();
    hsv_pixel[1] = (hsv_pixel[1]*saturation)/getNumFeatures();
    hsv_pixel[2] = (hsv_pixel[2]*vLuminance)/getNumFeatures();

    //0 <= index <= 255
    *index = static_cast<int32_t>(hsv_pixel[2]*saturation*hue + hsv_pixel[1]*hue + hsv_pixel[0]);

} //quantizeScalableUniform


/**
* Generates a Scalable Color descriptor from the image provided.
*
* @param normalizer The factor of normalization.
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
std::vector<int32_t> ScalableColorExtractor<SignatureType, DataObjectType>::computeNormalizedHistogram(const DataObjectType &image, std::vector<int32_t> histogram)
            throw (std::runtime_error) {

    //contain the pixel values
    u_int16_t *hsv_pixel;
    try {
        hsv_pixel = new u_int16_t[ScalableColorExtractor::HSV_CHANNELS];
    } catch(...) {
        throw std::runtime_error("Allocation error.");
    }


    for(size_t i = 0; i < getNumFeatures(); i++)
        histogram[i] = 0;

    //index of descriptor on the pixel value
    int32_t index;
    //reads the pixel values from image
    for(size_t x = 0; x < image.getWidth(); x++) {
        for(size_t y = 0; y < image.getHeight(); y++) {
            //converts from RGB to HSV
            try {
                HSVColorSystem conversion;
                u_int16_t hue, saturation, value;
                conversion.toHSV(image.getPixel(x,y), &hue, &saturation, &value);
                hsv_pixel[0] = hue;
                hsv_pixel[1] = saturation;
                hsv_pixel[2] = value;
            }
            catch(...) {
                throw std::runtime_error("Color Conversion error.");
                delete[] hsv_pixel;
            }

            //normalize the pixel values according the number of bins passed from template
            quantizeScalableUniform(hsv_pixel, &index);

            //put the results in the histogram
            histogram[index]++;
        }
    }


    //normalization of bins
    double factor = 2047.0; //NoBitsProBin=11
    double binwert, total = static_cast<double>(image.getWidth()*image.getHeight());

    for (size_t i = 0; i < getNumFeatures(); i++) {
        binwert = factor*static_cast<double>(histogram[i])/total;
        histogram[i] = static_cast<int32_t>(binwert + 0.49999);
    }

    delete[] hsv_pixel;
    return histogram;
} //computeNotmalizedHistogram

/**
* Quantize the histogram.
*
* @param histogram The histogram that will have your values quantized.
* @param histogram[out] The histogram with your values quantized.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
std::vector<int32_t> ScalableColorExtractor<SignatureType, DataObjectType>::quantizeHistogram(std::vector<int32_t> histogram) {

    int32_t factor = 15, iwert;
    double wert, potenz = 0.4, arg, maxwert;

    maxwert = 40.0*2047.0/100.0;

    for (size_t i = 0; i < getNumFeatures(); i++) {
        wert = static_cast<double>(histogram[i]);

        if(wert > maxwert)
            iwert = static_cast<int32_t>(factor);
        else {
           arg = wert/maxwert;
           wert = static_cast<double>(factor*pow(arg, potenz));
           iwert = static_cast<int32_t>(wert + 0.5);
        }

        if(iwert > factor)
            iwert = static_cast<int32_t>(factor);

        histogram[i] = iwert;
    }

    return histogram;
} //quantizeHistogram


/**
* Make a part of haar transform.
*
* @param tabale
* @param tabelle
* @param[out] histogram The histogram modified.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
std::vector<int32_t> ScalableColorExtractor<SignatureType, DataObjectType>::histo_3d_hirarch_5(const u_int16_t tablae, std::vector<int32_t> histogram)
    throw (std::runtime_error) {

    int32_t sum, dif, x1, y1, x2, y2, dimension = static_cast<int32_t>(sqrt(getNumFeatures()));
    int32_t **matrix;
    try {
        matrix = new int32_t*[dimension];
        for(size_t i = 0; i < dimension; i++) {
            matrix[i] = new int32_t[dimension];
        }
    }
    catch(...) {
        throw std::runtime_error("Allocation error on histo 3d");
    }

    for(size_t i = 0; i < getNumFeatures(); i++) {
        matrix[i%(hue)][i/(hue)] = histogram[i];
    }

    for(size_t i = 0; i < tablae; i++) {
        y1 = static_cast<int32_t> (tabelle[0][i]);
        x1 = static_cast<int32_t> (tabelle[1][i]);
        y2 = static_cast<int32_t> (tabelle[2][i]);
        x2 = static_cast<int32_t> (tabelle[3][i]);
        sum = static_cast<int32_t> (matrix[y1][x1] + matrix[y2][x2]);
        dif = static_cast<int32_t> (matrix[y2][x2] - matrix[y1][x1]);

        matrix[y1][x1] = sum;
        matrix[y2][x2] = dif;
    }

    for(size_t i = 0; i < getNumFeatures(); i++) {
        histogram[i] = matrix[i%(hue)][i/(hue)];
    }


    for(size_t i = 0; i < dimension; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    return histogram;
}  //histo_3d_hirarch_5


/**
* Make a part of haar transform.
*
* @param[out] histogram The histogram modified.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
void ScalableColorExtractor<SignatureType, DataObjectType>::hsv_hir_quant_lin_5(int32_t *histogram) {

    int32_t wert, maxwert;
    for(size_t i = 0; i < getNumFeatures(); i++) {
        maxwert = static_cast<int32_t>(scalableColorQuantValues[i][2]);
        wert = static_cast<int32_t>(histogram[i] - scalableColorQuantValues[i][0]);
        if(wert > maxwert)
            wert = maxwert;
        if(wert < -maxwert)
            wert = -maxwert;
        histogram[i] = wert;
    }

} //hsv_hir_quant_lin_5


/**
* Make a part of haar transform.
*
* @param[out] histogram The histogram modified.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
void ScalableColorExtractor<SignatureType, DataObjectType>::red_bits_pro_bin_5(int32_t *histogram) {

    int32_t wert, wert1, bits_pro_bin, bits_pro_bild = 0;
    int32_t max_bits_pro_bin = 0, anzkof = 0;

    if(getNumBitPlanesDiscarded() == 0)
        return;

    for(size_t i = 0; i < getNumFeatures(); ++i) {
        bits_pro_bin = static_cast<int32_t>(scalableColorQuantValues[i][1] - getNumBitPlanesDiscarded());
        if(bits_pro_bin < 2) {
            wert = histogram[i];
            if(wert >= 0)
                histogram[i] = 1;
            if(wert < 0)
                histogram[i] = 0;
            bits_pro_bild = bits_pro_bild + 1;
        } else {
            wert = histogram[i];
            wert1 = wert;
            if(wert < 0)
                wert = -wert;
            bits_pro_bild = bits_pro_bild + bits_pro_bin;
            if(bits_pro_bin > max_bits_pro_bin)
                max_bits_pro_bin = bits_pro_bin;
            anzkof++;

            for(size_t j = 0; j < getNumBitPlanesDiscarded(); j++)
                wert = wert/2;

            if(wert1 < 0)
                wert = -wert;

            histogram[i] = wert;
        }
   }

} //red_bits_pro_bin_5

/**
* Make a part of haar transform.
*
* @param[out] histogram The histogram after haar transform.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
std::vector<int32_t> ScalableColorExtractor<SignatureType, DataObjectType>::applyHaarTransform(std::vector<int32_t> histogram) throw (std::runtime_error) {

    u_int16_t tablae = 255;
    int32_t *histogram_out;
    try {
        histogram_out = new int32_t[getNumFeatures()];
    } catch(...) {
        throw std::runtime_error("Bad alloc when applying Haar transform.");
    }

    for(size_t i = 0; i < getNumFeatures(); i++) {
        histogram_out[i] = 0;
    }

    histogram = histo_3d_hirarch_5(tablae, histogram);

    int32_t index;
    for(size_t i = 0; i < getNumFeatures(); i++) {
        index = static_cast<int32_t>(sorttab[i]);
        histogram_out[i] = histogram[index];
    }

    hsv_hir_quant_lin_5(histogram_out);

    red_bits_pro_bin_5(histogram_out);

    for(size_t i = 0; i < getNumFeatures(); i++) {
        histogram[i] = (histogram_out[i]);
    }

    delete[] histogram_out;

    return histogram;
} //applyHaarTransform

/**
* Class constructor that sets the number of bins for each channel of color space HSV.
*
* @param histogram The histogram before normalization.
* @param new_histogramThe histogram after normalization.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
ScalableColorExtractor<SignatureType, DataObjectType>::ScalableColorExtractor(u_int16_t value1, u_int16_t value2) throw (std::runtime_error) {

    setNumFeatures(value1);
    setNumBitPlanesDiscarded(value2);

    u_int16_t numBins = value1;

    hue = 16;
    saturation = 4;
    vLuminance = 4;

    if (numBins == 128){
        hue = 8;
        saturation = 4;
        vLuminance = 4;
    }
    if (numBins == 64){
        hue = 8;
        saturation = 2;
        vLuminance = 4;
    }
    if (numBins == 32){
        hue = 8;
        saturation = 2;
        vLuminance = 2;
    }
    if (numBins == 16){
        hue = 4;
        saturation = 2;
        vLuminance = 2;
    }

}

template < class SignatureType, class DataObjectType >
ScalableColorExtractor<SignatureType, DataObjectType>::~ScalableColorExtractor(){
}

/**
* Set hue value
* @param Desired hue value.
*/
template < class SignatureType, class DataObjectType >
void ScalableColorExtractor<SignatureType, DataObjectType>::setHue(u_int16_t value){
    hue = value;
}

/**
* Get hue value.
*
* @return Hue value.
*/
template < class SignatureType, class DataObjectType >
u_int16_t ScalableColorExtractor<SignatureType, DataObjectType>::getHue() const{
     return hue;
}

/**
* Set saturation value
* @param Desired saturation value.
*/
template < class SignatureType, class DataObjectType >
void ScalableColorExtractor<SignatureType, DataObjectType>::setSaturation(u_int16_t value){
    saturation = value;
}

/**
* Get saturation value.
*
* @return Saturation value.
*/
template < class SignatureType, class DataObjectType >
u_int16_t ScalableColorExtractor<SignatureType, DataObjectType>::getSaturation() const{
     return saturation;
}

/**
* Set vLuminance value
* @param Desired vLuminance value.
*/
template < class SignatureType, class DataObjectType >
void ScalableColorExtractor<SignatureType, DataObjectType>::setVLuminance(u_int16_t value){
    vLuminance = value;
}

/**
* Get vLuminance value.
*
* @return VLuminance value.
*/
template < class SignatureType, class DataObjectType >
u_int16_t ScalableColorExtractor<SignatureType, DataObjectType>::getVLuminance() const{
     return vLuminance;
}

/**
* Set numFeatures value
* @param Desired vLuminance value.
*/
template < class SignatureType, class DataObjectType >
void ScalableColorExtractor<SignatureType, DataObjectType>::setNumFeatures(u_int16_t value){
    numFeatures = value;
}

/**
* Get numFeatures value.
*
* @return numFeatures value.
*/
template < class SignatureType, class DataObjectType >
u_int16_t ScalableColorExtractor<SignatureType, DataObjectType>::getNumFeatures() const{
     return numFeatures;
}

/**
* Set numBitPlanesDiscarded value
* @param Desired numBitPlanesDiscarded value.
*/
template < class SignatureType, class DataObjectType >
void ScalableColorExtractor<SignatureType, DataObjectType>::setNumBitPlanesDiscarded(u_int16_t value){
    numBitPlanesDiscarded = value;
}

/**
* Get numFeatures value.
*
* @return numFeatures value.
*/
template < class SignatureType, class DataObjectType >
u_int16_t ScalableColorExtractor<SignatureType, DataObjectType>::getNumBitPlanesDiscarded() const{
     return numBitPlanesDiscarded;
}
