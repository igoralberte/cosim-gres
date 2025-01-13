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
* This file defines a Class to extract a Dominant Color.
*
* @version 1.0
*/

/**
* Generates a Scalable Color descriptor from the image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template < class SignatureType, class DataObjectType >
void ColorStructureExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType &image, SignatureType &sign){

    int32_t E,K;

    //Define structure parameters
    structureElements(image.getHeight(), image.getWidth(), &E, &K);

    //Calculate structure
    std::vector<double> histogram;
    histogram = acumulationHistogram(image, E, K);

    //Fill signature vector
    sign.resize(getNumFeatures(), 0.0);
    double max = DBL_MIN;

    for (size_t i = 0; i < getNumFeatures();i++){
        sign[i] = histogram[i];
        if (sign[i] > max){
            max = sign[i];
        }
    }

    for (size_t i = 0; i < getNumFeatures();i++){
        sign[i] = ((double) sign[i]) / ((double) max);
    }

    histogram.clear();
}

/**
* Define the number of structuring elements (E and K)
*
* @param height Value of image.
* @param width Value of image.
* @param[out] Parameter E.
* @param[out] Parameter K
*/
template < class SignatureType, class DataObjectType >
void ColorStructureExtractor<SignatureType, DataObjectType>::structureElements(int32_t height, int32_t width, int32_t *factorE, int32_t *factorK){

    int32_t p;

    p = std::max(0,(int32_t)(0.5*log2(height*width)-8));
    *factorK = pow(2,p);
    *factorE = 8*(*factorK);
}

/**
* Calculate acumulation histogram of structure
*
* @param height Value of image.
* @param width Value of image.
* @param factorE Value of E (like paper)
* @param factorK Value of K (like paper)
*/
template < class SignatureType, class DataObjectType >
std::vector<double> ColorStructureExtractor<SignatureType, DataObjectType>::acumulationHistogram(const DataObjectType &image, int32_t factorE, int32_t factorK){

    int32_t step;

    //store total values of all structures
    std::vector<double> globalHistogram;

    for(size_t i = 0; i < getNumFeatures(); i++){
        globalHistogram.push_back(0.0);
    }

    //Detect which dimension is lower
    if((image.getHeight()/factorE) > (image.getWidth()/factorE)){
        step = image.getWidth()/factorE;
    }else{
        step = image.getHeight()/factorE;
    }

    //First two fors to "slide" structural element on image
    for(size_t i = 0; i < image.getHeight()-factorE; i = i+step){
        for(size_t j = 0; j < image.getWidth()-factorE; j = j+step){

            //store local values (0 or 1)
            std::vector<int32_t> localHistogram;
            for(size_t z = 0; z < getNumFeatures(); z++){
                localHistogram.push_back(0);
            }

            //Calculate each 8x8 structure
            for(size_t y = i; y < i+8*factorK; y+=factorK){
                for(size_t x = j; x < j+8*factorK; x+=factorK){
                    //int32_t hue, max, min, sum, diff;
                    u_int16_t hue,sum;
                    u_int8_t max,min,diff;

                    //Color Conversion
                    //rgb2hmmd(red,green,blue,&hue,&sum,&diff);
                    HMMDColorSystem conversion;
                    conversion.toHMMD(image.getPixel(x,y),&hue,&max,&min,&diff);                    
                    sum = static_cast<u_int16_t>((max + min)/2);

                    //Quantize Element
                    int32_t quantizedHue, quantizedSum, subspace;
                    quantizeValue(hue,sum,diff, &subspace, &quantizedHue, &quantizedSum);

                    //increment local histogram
                    localHistogram = incrementHistogram(localHistogram,subspace,quantizedHue,quantizedSum);                    
                }
            }
            for(size_t z = 0; z < getNumFeatures();z++){
                globalHistogram[z]+=localHistogram[z];
            }
            localHistogram.clear();
        }
    }

    //normalize global histogram
    for(size_t z = 0; z < getNumFeatures();z++){
        globalHistogram[z]/=(double)(((double)factorE)*((double)factorE));
    }
    return globalHistogram;
}

/**
* Calculate the quantized values
*
* @param localHistogram The local histogram.
* @param subspace The position of actual subspace.
* @param hue Value of quantized hue
* @param sum Valur of quantized sum
*/
template < class SignatureType, class DataObjectType >
std::vector<int32_t> ColorStructureExtractor<SignatureType, DataObjectType>::incrementHistogram(std::vector<int32_t> localHistogram, int32_t subspace, int32_t hue, int32_t sum){

    //Find the correct position and set the value to 1
    int32_t position = 0;

    //Discover the position of total bins
    int32_t totalBinsPosition = 0;
    while(getNumFeatures() != nQuantization[totalBinsPosition]){
        totalBinsPosition++;
    }

    switch(subspace)
    {
        case 0:
        {
            position = sum;
            localHistogram[position] = 1;
            break;
        }
        case 1:
        {
            //Bins of subspace 0 + displacement of hue + value of sum
            position = (nHueLevels[totalBinsPosition][0]*nSumLevels[totalBinsPosition][0])+
                    (hue*nSumLevels[totalBinsPosition][subspace]);
            localHistogram[position] = 1;
            break;
        }
        case 2:
        {
        if(getNumFeatures() == 32){
                position = (nHueLevels[totalBinsPosition][0]*nSumLevels[totalBinsPosition][0])+
                        (hue*nSumLevels[totalBinsPosition][subspace]);
                localHistogram[position] = 1;
                break;
            }else{
                //Bins of subspace 0 + bins of subspace 1 + displacement of hue + value of sum
                position = (nHueLevels[totalBinsPosition][0]*nSumLevels[totalBinsPosition][0])+
                        (nHueLevels[totalBinsPosition][1]*nSumLevels[totalBinsPosition][1]) +
                        (hue*nSumLevels[totalBinsPosition][subspace]);
                localHistogram[position] = 1;
                break;
            }

        }
        case 3:
        {
        if(getNumFeatures() == 32){
                position = (nHueLevels[totalBinsPosition][0]*nSumLevels[totalBinsPosition][0])+
                    (nHueLevels[totalBinsPosition][2]*nSumLevels[totalBinsPosition][2]) +
                    (hue*nSumLevels[totalBinsPosition][subspace])+sum;
            }else{
                //Bins of subspace 0 + bins of subspace 1 + bins of subspace 2 + displacement of hue + value of sum
                position = (nHueLevels[totalBinsPosition][0]*nSumLevels[totalBinsPosition][0])+
                    (nHueLevels[totalBinsPosition][1]*nSumLevels[totalBinsPosition][1]) +
                    (nHueLevels[totalBinsPosition][2]*nSumLevels[totalBinsPosition][2]) +
                    (hue*nSumLevels[totalBinsPosition][subspace])+sum;
            }
            localHistogram[position] = 1;
            break;
        }
        case 4:
        {
        if(getNumFeatures() == 32){
                position = (nHueLevels[totalBinsPosition][0]*nSumLevels[totalBinsPosition][0])+
                    (nHueLevels[totalBinsPosition][2]*nSumLevels[totalBinsPosition][2]) +
                    (nHueLevels[totalBinsPosition][3]*nSumLevels[totalBinsPosition][3]) +
                    (hue*nSumLevels[totalBinsPosition][subspace])+sum;
            }else{
                //Bins of subspace 0 + bins of subspace 1 + bins of subspace 2 + bins of subspace 3 + displacement of hue + value of sum
                position = (nHueLevels[totalBinsPosition][0]*nSumLevels[totalBinsPosition][0])+
                    (nHueLevels[totalBinsPosition][1]*nSumLevels[totalBinsPosition][1]) +
                    (nHueLevels[totalBinsPosition][2]*nSumLevels[totalBinsPosition][2]) +
                    (nHueLevels[totalBinsPosition][3]*nSumLevels[totalBinsPosition][3]) +
                    (hue*nSumLevels[totalBinsPosition][subspace])+sum;
            }
            localHistogram[position] = 1;
            break;
        }
    }
    return localHistogram;
}



/**
* Calculate the quantized values
*
* @param height Value of image.
* @param width Value of image.
* @param factorE Value of E (like paper)
* @param factorK Value of K (like paper)
*/
template < class SignatureType, class DataObjectType >
void ColorStructureExtractor<SignatureType, DataObjectType>::quantizeValue(
        u_int16_t hue, u_int16_t sum, u_int8_t diff, int32_t *subspace, int32_t *quantizedHue, int32_t *quantizedSum){

    //change range of hue from [0,360] to [0,255]
    hue = (hue*255)/360;

    //Discover the subspace of HMMD color
    bool condition = false;
    int32_t subspacePosition = -1;
    do{
        subspacePosition++;
        if(diff < nSubSpaces[subspacePosition]){
            condition = true;
        }
    }while(condition == false);

    //Discover the position of total bins
    int32_t totalBinsPosition = 0;

    while(getNumFeatures() != nQuantization[totalBinsPosition]){
        totalBinsPosition++;
    }

    //Quantize hue
    int32_t step, testValue;
    step = 256/nHueLevels[totalBinsPosition][subspacePosition];
    testValue = step;

    *quantizedHue = 0;
    while(hue > testValue){
        testValue+=step;
        (*quantizedHue)++;
    }

    //Quantize sum
    step = 256/nSumLevels[totalBinsPosition][subspacePosition];
    testValue = step;
    *quantizedSum = 0;
    while(sum > testValue){
        testValue+=step;
        (*quantizedSum)++;
    }

    *subspace = subspacePosition;
}

/**
* Verify and set the num of features (size of signature vector)
*
* @param Desired value of numFeatures.
*/
template < class SignatureType, class DataObjectType >
void ColorStructureExtractor<SignatureType, DataObjectType>::setNumFeatures(u_int8_t value) throw (std::runtime_error){

    if( value == 256 || value == 128 || value == 64 || value == 32)
        numFeatures = value;
    else
        throw std::runtime_error("Cannot set number of features Color Structure");
}

/**
* Return the number of features
* @return numFeatures
*/
template < class SignatureType, class DataObjectType >
u_int8_t ColorStructureExtractor<SignatureType, DataObjectType>:: getNumFeatures(){

    return numFeatures;
}

/**
*  Class constructor
* @param Desired value of numFeatures.
*/
template < class SignatureType, class DataObjectType>
ColorStructureExtractor<SignatureType, DataObjectType>::ColorStructureExtractor(u_int8_t value) throw (std::runtime_error) {

    setNumFeatures(value);
}
