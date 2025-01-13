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
* Empty class constructor.
*/
template< class SignatureType, class DataObjectType>
ColorTemperatureExtractor<SignatureType, DataObjectType>::ColorTemperatureExtractor() {

    setThreshold(5);
    setMultiplierCoeficient(3);
    setDelta(20);
}

template< class SignatureType, class DataObjectType>
void ColorTemperatureExtractor<SignatureType, DataObjectType>::setThreshold(u_int16_t value){
    threshold = value;
}

template< class SignatureType, class DataObjectType>
u_int16_t ColorTemperatureExtractor<SignatureType, DataObjectType>::getThreshold(){
    return threshold;
}

template< class SignatureType, class DataObjectType>
void ColorTemperatureExtractor<SignatureType, DataObjectType>::setMultiplierCoeficient(u_int16_t value){
    multiplierCoeficient = value;
}

template< class SignatureType, class DataObjectType>
u_int16_t ColorTemperatureExtractor<SignatureType, DataObjectType>::getMultiplierCoeficient(){
    return multiplierCoeficient;
}

template< class SignatureType, class DataObjectType>
void ColorTemperatureExtractor<SignatureType, DataObjectType>::setDelta(u_int16_t value){
    delta = value;
}

template< class SignatureType, class DataObjectType>
u_int16_t ColorTemperatureExtractor<SignatureType, DataObjectType>::getDelta(){
    return delta;
}

/**
* Class destructor.
*/
template< class SignatureType, class DataObjectType>
ColorTemperatureExtractor<SignatureType, DataObjectType>::~ColorTemperatureExtractor() {
}


/**
* Generates a Color Temperature descriptor from the image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template< class SignatureType, class DataObjectType>
void ColorTemperatureExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType &image, SignatureType &sign) {

    //vector to store final image
    std::vector< std::vector<double> > imageXYZ;

    //Define structure parameters
    imageXYZ = convertRGB2XYZ(image);

    double xA,yA,zA;

    //apply averaging procedure
    averageProcedure(image, imageXYZ, &xA, 0);
    averageProcedure(image, imageXYZ, &yA, 1);
    averageProcedure(image, imageXYZ, &zA, 2);

    //calculate color temperature
    double uS, uV;
    double temperature = colorTemperature(xA,yA,zA,&uS, &uV);

    //Fill signature vector
    sign.resize(3, 0.0);
    sign[0] = temperature;
    sign[1] = uS;
    sign[2] = uV;

    for (size_t i = 0; i < image.getWidth()*image.getHeight();i++){
        imageXYZ[i].clear();
    }
    imageXYZ.clear();
}

template< class SignatureType, class DataObjectType >
std::vector < std::vector<double> > ColorTemperatureExtractor<SignatureType, DataObjectType>::convertRGB2XYZ(const DataObjectType &image){

    //Matrix to store converted image
    std::vector< std::vector<double> > imageXYZ(image.getWidth()*image.getHeight());

    XYZColorSystem conversionXYZ;

    //analyzes and convert all image to XYZ
    for(size_t y = 0; y < image.getHeight(); y++){
        for(size_t x = 0; x < image.getWidth(); x++){

            //Map the coordinate to a vector
            imageXYZ[y*image.getWidth()+x].resize(3);

            //convert RGB -> XYZ
            double colorX = conversionXYZ.getXOfXYZ(image.getPixel(x,y));
            double colorY = conversionXYZ.getYOfXYZ(image.getPixel(x,y));
            double colorZ = conversionXYZ.getZOfXYZ(image.getPixel(x,y));

            //discard pixels with Y luminance value bellow threshold
            if (discardPixels(colorY) == 0){
                imageXYZ[y*image.getWidth()+x][0] = 0.0;
                imageXYZ[y*image.getWidth()+x][1] = 0.0;
                imageXYZ[y*image.getWidth()+x][2] = 0.0;
            }else{
                imageXYZ[y*image.getWidth()+x][0] = colorX;
                imageXYZ[y*image.getWidth()+x][1] = colorY;
                imageXYZ[y*image.getWidth()+x][2] = colorZ;
            }
        }
    }
    return imageXYZ;
}


template< class SignatureType, class DataObjectType >
u_int8_t ColorTemperatureExtractor<SignatureType, DataObjectType>::discardPixels(double colorY){
    if (colorY < (((double) threshold)/100.0))
        return 0;
    else
        return 1;
}

template< class SignatureType, class DataObjectType >
void ColorTemperatureExtractor<SignatureType, DataObjectType>::averageProcedure(const DataObjectType &image, std::vector< std::vector<double> > imageXYZ, double *xA, u_int8_t channel){

    double xTOld, xTNew, xAux;
    bool cond = true;
    xTOld = 0;

    while(cond){
        xAux = xTNew = 0;
        //calculate average
        for (size_t j = 0; j < image.getHeight(); j++){
            for (size_t i = 0; i < image.getWidth(); i++){
                xAux += imageXYZ[j*image.getWidth()+i][channel];
            }
        }
        //last steps of average
        xAux *= (1/(double)(image.getHeight()*image.getWidth()));
        xTNew = multiplierCoeficient * xAux;

        //check if threshold value is the same of last iteration
        if(xTNew == xTOld){
            //if true the process is over
            cond = false;
            *xA = xAux;
        } else {
            for (size_t j = 0; j < image.getHeight(); j++){
                for (size_t i = 0; i < image.getWidth(); i++){
                    if (imageXYZ[j*image.getWidth()+i][0] > xTNew + delta){
                        //discard pixel
                        imageXYZ[j*image.getWidth()+i][0] = 0;
                        imageXYZ[j*image.getWidth()+i][1] = 0;
                        imageXYZ[j*image.getWidth()+i][2] = 0;
                    }
                }
            }
        }
        xTOld = xTNew;
    }
}

template< class SignatureType, class DataObjectType >
double ColorTemperatureExtractor<SignatureType, DataObjectType>::colorTemperature(double xA, double yA, double zA, double *uS, double *vS){

    double xS, yS;

    //calculate uS, uV
    if ((xA+yA+zA) != 0){
        xS = xA/(xA+yA+zA);
        yS = yA/(xA+yA+zA);

        *uS = (4*xS)/(-2*xS+12*yS+3);
        *vS = (6*yS)/(-2*xS+12*yS+3);
    } else {
        *uS = 0;
        *vS = 0;
    }

    //calculate distance from 33 isotemperature lines
    std::vector<double> distance;
    for(size_t i = 0;i < 31; i++){
        double value = ((*vS-ROBERTSON_ISOTEMPERATURE_LINES_DATA[i][1])-
                ROBERTSON_ISOTEMPERATURE_LINES_DATA[i][3]*(*uS-ROBERTSON_ISOTEMPERATURE_LINES_DATA[i][2]))/
                sqrt(1+pow(ROBERTSON_ISOTEMPERATURE_LINES_DATA[i][3],2));
        distance.push_back(value);
    }

    //get 2 distances where one is nearest from 0 negative and another is nearest from 0 positive
    u_int16_t posD1, posD2,i;
    bool negativesValues = false;
    posD1 = 0;
    posD2 = 1;
    i = 2;

    if (distance[i] > 0){
        while((distance[i] > 0) && (i < 31)){
            posD1++;
            posD2++;
            i++;
        }
        posD1++;
        posD2++;
    } else {
        negativesValues = true;
    }

    //calculate temperature
    double temperature;
    if (!negativesValues){
        temperature = ROBERTSON_ISOTEMPERATURE_LINES_DATA[posD1][0] + (distance[posD1]/(distance[posD1]-distance[posD2]))*
                (ROBERTSON_ISOTEMPERATURE_LINES_DATA[posD2][0]-ROBERTSON_ISOTEMPERATURE_LINES_DATA[posD1][0]);
    } else {
        temperature = 0;
    }

    return temperature;
}

