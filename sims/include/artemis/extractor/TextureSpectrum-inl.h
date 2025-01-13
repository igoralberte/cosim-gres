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
* This file implements the Texture Spectrum class.
*/

/**
* Empty class constructor.
*/
template< class SignatureType, class DataObjectType>
TextureSpectrumExtractor<SignatureType, DataObjectType>::TextureSpectrumExtractor(u_int8_t deltaRange, u_int16_t numFeatures) {

    setNumFeatures(numFeatures);
    setDeltaRange(deltaRange);
}

template< class SignatureType, class DataObjectType>
TextureSpectrumExtractor<SignatureType, DataObjectType>::~TextureSpectrumExtractor(){
}

template< class SignatureType, class DataObjectType>
void TextureSpectrumExtractor<SignatureType, DataObjectType>::setNumFeatures(u_int16_t numFeatures){

    this->numFeatures = numFeatures;
}

template< class SignatureType, class DataObjectType>
void TextureSpectrumExtractor<SignatureType, DataObjectType>::setDeltaRange(u_int8_t deltaRange){

    this->deltaRange = deltaRange;
}

template< class SignatureType, class DataObjectType>
u_int16_t TextureSpectrumExtractor<SignatureType, DataObjectType>::getNumFeatures(){

    return numFeatures;
}

template< class SignatureType, class DataObjectType>
u_int8_t TextureSpectrumExtractor<SignatureType, DataObjectType>::getDeltaRange(){

    return deltaRange;
}

/**
* Generates a Texture Spectrum from the image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated signature.
* @return The resulting ExtReturnCode.
*/
template< class SignatureType, class DataObjectType>
void TextureSpectrumExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error){

    size_t i;
    size_t j;
    u_int16_t centralPixelValue;
    u_int16_t neighborValues[8];

    double *featureVector = NULL;
    double spectrumTextureVector[8][MAX_LOOKUP_NTU];

    //To calculate the equal occurences
    double HM[MAX_LOOKUP_NTU];
    double VM[MAX_LOOKUP_NTU];
    double DM1[MAX_LOOKUP_NTU];
    double DM2[MAX_LOOKUP_NTU];
    double KCS[MAX_LOOKUP_NTU];

    //Create the feature vector with null values
    featureVector = new double[numFeatures];
    for (i = 0; i < numFeatures; i++) {
        featureVector[i] = 0;
    }

    //spectrumTextureVector = new double[8];
    for (j = 0; j < 8; j++) {
        //spectrumTextureVector[j] = new double[6561];
        for (i = 0; i < MAX_LOOKUP_NTU; i++) {
            spectrumTextureVector[j][i] = 0;
        }
    }

    //neighborValues = new stCount[8];
    for (i = 0; i < 8; i++) {
        neighborValues[i] = 0;
    }

    //Create and fill the vectors for the calculus of each distance
    for (i = 0; i < MAX_LOOKUP_NTU; i++) {
        HM[i] = 99;
        VM[i] = 99;
        DM1[i] = 99;
        DM2[i] = 99;
        KCS[i] = 99;
    }

    double imageSize = image.getWidth()*image.getHeight();


    //Start extracting the TextureSpectrum.
    //Read each pixel in the image, extracting the Texture Unit ocurrences.
    for (size_t x = 1; x < image.getWidth()-1; x++) {
        for (size_t y = 1; y < image.getHeight()-1; y++) {

            u_int32_t Ntu; //Each pixel will have a "Texture Unit Number"

            //Gather the pixel neighborhood
            centralPixelValue = (u_int16_t) ((image.getPixel(x, y).getGrayPixelValue()));
            neighborValues[0] = (u_int16_t) ((image.getPixel(x-1, y-1).getGrayPixelValue()));
            neighborValues[1] = (u_int16_t) ((image.getPixel(x-1, y).getGrayPixelValue()));
            neighborValues[2] = (u_int16_t) ((image.getPixel(x-1, y+1).getGrayPixelValue()));
            neighborValues[3] = (u_int16_t) ((image.getPixel(x, y+1).getGrayPixelValue()));
            neighborValues[4] = (u_int16_t) ((image.getPixel(x+1, y+1).getGrayPixelValue()));
            neighborValues[5] = (u_int16_t) ((image.getPixel(x+1, y).getGrayPixelValue()));
            neighborValues[6] = (u_int16_t) ((image.getPixel(x+1, y-1).getGrayPixelValue()));
            neighborValues[7] = (u_int16_t) ((image.getPixel(x, y-1).getGrayPixelValue()));

            //Obtain the pixel texture-unit
            for (i = 0; i < 8; i++){
                if (neighborValues[i] < ((u_int16_t) (centralPixelValue - (u_int16_t) deltaRange))){
                    neighborValues[i] = 0;
                } else if (neighborValues[i] > ((u_int16_t) (centralPixelValue + (u_int16_t) deltaRange))){
                    neighborValues[i] = 2;
                } else {
                    neighborValues[i] = 1;
                }
            }

            //Calculate texture ''label'' for each NTU
            for (j = 0; j < 8; j++){//for each sorting (8 possibles)

                Ntu = 0;

                for(i = 0; i < 8; i++){
                    Ntu = Ntu + neighborValues[(i+j)%8]*pow(3,i);
                }

                //Increments histograms according with the sorting j
                spectrumTextureVector[j][Ntu]++;

                if (j==0){

                    if (HM[Ntu]==99){
                        u_int16_t orientations[5];
                        getOrientations(neighborValues, orientations);
                        HM[Ntu] = orientations[0];
                        VM[Ntu] = orientations[1];
                        DM1[Ntu] = orientations[2];
                        DM2[Ntu] = orientations[3];
                        KCS[Ntu] =orientations[4];
                    }
                }
            }
        }
    }

    //Calculus of "Black-white symmetry" (BWS)
    double up_value = 0;
    double down_value = 0;
    double BWS;

    for (i = 0; i < 3280; i++){
        up_value = up_value + fabs(spectrumTextureVector[0][i] - spectrumTextureVector[0][3281+i]);
        down_value = down_value + spectrumTextureVector[0][i] + spectrumTextureVector[0][3281+i];
    }
    down_value = down_value + spectrumTextureVector[0][3280];

    BWS = (1-((double) up_value/down_value))*100;
    featureVector[0] = (double) BWS;

    //Calculus of "Geometric Symmetry" (GS)
    double GS = 0;
    for (j = 0; j < 4; j++){
        up_value = 0;
        down_value = 0;
        for(i=0; i < MAX_LOOKUP_NTU; i++){
            up_value = up_value + fabs(spectrumTextureVector[j][i] - spectrumTextureVector[j+4][i]);
            down_value = down_value + spectrumTextureVector[j][i];
        }
        down_value = 2*down_value;

        GS = GS + (double) up_value/down_value;
    }
    GS = GS/4;
    GS = (1 - GS)*100;

    featureVector[1] = GS;

    //Calculus of "Degree of Direction" (DD)
    double DD = 0;
    double sumResult;
    for (size_t m = 0; m < 3; m++){
        sumResult = 0;
        for (size_t n = m+1; n < 4; n++){
            up_value = 0;
            down_value = 0;
            for (i = 0; i < MAX_LOOKUP_NTU; i++){
                up_value = up_value + fabs(spectrumTextureVector[m][i] - spectrumTextureVector[n][i]);
                down_value = down_value + spectrumTextureVector[m][i];
            }
            down_value = 2*down_value;

            sumResult = sumResult + (double)(up_value/down_value);
        }
        DD = DD + sumResult;
    }
    DD = DD/6;
    DD = (1 - DD)*100;

    featureVector[2] = DD;


    //Calculus of "Micro-Horizontal Structure" (MHS)
    double MHS = 0;
    double MVS = 0;
    double MDS1 = 0;
    double MDS2 = 0;
    double CS = 0;
    for (i = 0; i < MAX_LOOKUP_NTU;i++){
        MHS = MHS + spectrumTextureVector[0][i]*HM[i];
        MVS = MVS + spectrumTextureVector[0][i]*VM[i];
        MDS1 = MDS1 + spectrumTextureVector[0][i]*DM1[i];
        MDS2 = MDS2 + spectrumTextureVector[0][i]*DM2[i];
        CS = CS + (spectrumTextureVector[0][i]*(KCS[i]*KCS[i]));
    }


    featureVector[3] = 100*MHS/imageSize;

    //Calculus of "Micro-Vertical Structure" (MVS)
    featureVector[4] = 100*MVS/imageSize;

    //Calculus of "Micro-Diagonal 1 Structure" (MDS1)
    featureVector[5] = 100*MDS1/imageSize;

    //Calculus of "Micro-Diagonal 2 Structure" (MDS2)
    featureVector[6] = 100*MDS2/imageSize;

    //Calculus of "Central symmetry"
    featureVector[7] = 100*CS/imageSize;
    
    //Populate the sign object with the computed features
    sign.resize(numFeatures, 0.0);
    double max = DBL_MIN;

    for (i = 0; i < numFeatures; i++) {
        sign[i] = featureVector[i];
        if (sign[i] > max){
            max = sign[i];
        }
    }

    for (i = 0; i < numFeatures; i++) {
        sign[i] = ((double) sign[i]) / ((double) max);
    }

    delete[] featureVector;
}

/**
* This is a function to compare 3 E_i values.
* Initially, it acts like a binary AND.
*
* @param elem1 The pixel P_1 mapped according its neighborhood
* @param elem2 The pixel P_2 mapped according its neighborhood
* @param elem3 The pixel P_3 mapped according its neighborhood
*
* @return A binary AND between elem1, elem2 and elem3
*/
template< class SignatureType, class DataObjectType>
u_int16_t TextureSpectrumExtractor<SignatureType, DataObjectType>::getSameValueNumber(u_int16_t elem1, u_int16_t elem2, u_int16_t elem3){

    if ((elem1 == elem2) && (elem2 == elem3)){
        return 1;
    }

    if ((elem1 == elem2)){
        return 0;
    }

    if ((elem2 == elem3)){
        return 0;
    }

    if ((elem1 == elem3)){
        return 0;
    }

    return 0;

}

/**
* Given a NTU, this function returns the values of each ''orientation''.
*
* @param u_int16_t neighborValues[8];
* @return The number of elements having the same value for each orientation;
*/
template< class SignatureType, class DataObjectType>
void TextureSpectrumExtractor<SignatureType, DataObjectType>::getOrientations(u_int16_t neighborValues[], u_int16_t orientations[]){

    // Obtain the number of elements that have the same value of E()
    // Regarding (HM), (VM), (DM1) and (DM2)
    u_int16_t auxHM = 0;
    u_int16_t auxVM = 0;
    u_int16_t auxDM1 = 0;
    u_int16_t auxDM2 = 0;
    u_int16_t auxKCS = 0;

    auxHM = getSameValueNumber(neighborValues[0], neighborValues[1], neighborValues[2]);
    auxHM = auxHM * getSameValueNumber(neighborValues[4], neighborValues[5], neighborValues[6]);

    auxVM = getSameValueNumber(neighborValues[2], neighborValues[3], neighborValues[4]);
    auxVM = auxVM * getSameValueNumber(neighborValues[0], neighborValues[6], neighborValues[7]);

    auxDM1 = getSameValueNumber(neighborValues[7], neighborValues[0], neighborValues[1]);
    auxDM1 = auxDM1 * getSameValueNumber(neighborValues[5], neighborValues[4], neighborValues[3]);

    auxDM2 = getSameValueNumber(neighborValues[1], neighborValues[2], neighborValues[3]);
    auxDM2 = auxDM2 * getSameValueNumber(neighborValues[5], neighborValues[6], neighborValues[7]);

    //Calculus for Central Simmetry
    auxKCS = 0;
    //(Ea,Ee) OR (Ea,Eh)
    if((neighborValues[0] == neighborValues[4])){auxKCS++;}
    //(Ea,Ef) OR (Eb,Eg)
    if((neighborValues[1] == neighborValues[5])){auxKCS++;}
    //(Ec,Eg) OR (Ec,Ef)
    if((neighborValues[2] == neighborValues[6])){auxKCS++;}
    //(Ed,Eh) OR (Ed,Ee)
    if((neighborValues[3] == neighborValues[7])){auxKCS++;}

    if (auxKCS == 4){
        auxKCS = 1;
    } else {
        auxKCS = 0;
    }

    orientations[0] = auxHM;
    orientations[1] = auxVM;
    orientations[2] = auxDM1;
    orientations[3] = auxDM2;
    orientations[4] = auxKCS;
}
