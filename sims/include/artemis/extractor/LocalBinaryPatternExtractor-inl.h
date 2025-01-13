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
template< class SignatureType, class DataObjectType >
LocalBinaryPatternExtractor<SignatureType, DataObjectType>::LocalBinaryPatternExtractor(u_int16_t value) {
    setNumFeatures(value);
    createLookUp();
}

template< class SignatureType, class DataObjectType >
LocalBinaryPatternExtractor<SignatureType, DataObjectType>::~LocalBinaryPatternExtractor() {
}

template< class SignatureType, class DataObjectType >
u_int16_t LocalBinaryPatternExtractor<SignatureType, DataObjectType>:: getNumFeatures() {
    return numFeatures;
}

template< class SignatureType, class DataObjectType >
void LocalBinaryPatternExtractor<SignatureType, DataObjectType>:: setNumFeatures(u_int16_t value) {
    numFeatures = value;
}

template< class SignatureType, class DataObjectType>
void LocalBinaryPatternExtractor<SignatureType, DataObjectType>::createLookUp(){
    u_int16_t code = 0;
    int32_t trans;
    for(size_t j=0; j<256; j++){
        u_char u = j;
        int32_t first = u%2;
        int32_t cur = first;
        int32_t next;
		trans=0;
        for(size_t i=0; i<8; i++){
			next = u%2;
			u /= 2;
			if(cur!=next){
				trans++;
			}
			cur = next;
		}
		if(next != first){
			trans++;
		}
		if(trans>2){
			lookUp[j] = 58;
		}else{
			lookUp[j] = code++;
		}
	}
}

/**
* Generates the Local Binary Pattern descriptor defined in the template from the
* image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template< class SignatureType, class DataObjectType>
void LocalBinaryPatternExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType & image, SignatureType & sign) {
    int32_t base[8]={1,2,4,8,16,32,64,128};
    int32_t moves[8][2] = {
						{0,1},
						{1,1},
						{1,0},
						{1,-1},
						{0,-1},
						{-1,-1},
						{-1,0},
						{-1,1}
						};
    size_t i;
    int32_t idxPixelValue;
    double * featureVector = NULL;

    featureVector = new double[3*getNumFeatures()];
    for (i = 0; i < 3*getNumFeatures(); i++) {
        featureVector[i] = 0;
    }

    //Start extracting the histogram.
    //Read each pixel in the image, summing the textures codes.
    for (size_t x = 1; x < image.getWidth() - 1; x++) {
        for (size_t y = 1; y < image.getHeight() - 1; y++) {
            //for red
            idxPixelValue = (int32_t) image.getPixel(x, y).getRedPixelValue();
            int text=0;
            for(size_t i=0; i<8; i++){
                int32_t newX = x+moves[i][0];
                int32_t newY = y+moves[i][1];
                int32_t newValue = (int32_t) image.getPixel(newX, newY).getRedPixelValue();
				if(newValue>=idxPixelValue){
					text+=base[i];
				}
			}
            
            featureVector[lookUp[text]]++;
            
            //for green
            idxPixelValue = (int32_t) image.getPixel(x, y).getGreenPixelValue();
            text = 0;
			for(int i=0; i<8; i++){
                int32_t newX = x+moves[i][0];
                int32_t newY = y+moves[i][1];
                int32_t newValue = (int32_t) image.getPixel(newX, newY).getGreenPixelValue();
				if(newValue>=idxPixelValue){
					text+=base[i];
				}
			}
            featureVector[lookUp[text]+getNumFeatures()]++;
            
            //for blue
            idxPixelValue = (int32_t) image.getPixel(x, y).getBluePixelValue();
            text = 0;
			for(int i=0; i<8; i++){
                int32_t newX = x+moves[i][0];
                int32_t newY = y+moves[i][1];
                int32_t newValue = (int32_t) image.getPixel(newX, newY).getBluePixelValue();
				if(newValue>=idxPixelValue){
					text+=base[i];
				}
			}
            featureVector[lookUp[text]+2*getNumFeatures()]++;
        }                
    }

    //Normalize the histogram in the range [0, 1], making the sum of all bin
    //values equals to 1.
    //Populate the sign object with the features computed
    sign.resize(3*getNumFeatures());
    for (i = 0; i < 3*getNumFeatures(); i++) {
            sign[i] = featureVector[i] / image.getSize();
    }
    delete[] featureVector;
}
