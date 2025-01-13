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
* This file implements the complex methods of the TextureBrowsingExtractor class.
*/


/**
* Empty class constructor.
*/
template< class SignatureType, class DataObjectType >
TextureBrowsingExtractor<SignatureType, DataObjectType>::TextureBrowsingExtractor(u_int16_t sigmaSize, u_int16_t thetaSize) {

    setSigmaSize(sigmaSize);
    setThetaSize(thetaSize);
}

template< class SignatureType, class DataObjectType >
void TextureBrowsingExtractor<SignatureType, DataObjectType>::setKernelSize(u_int16_t value){

    kernelSize = value;
}

template< class SignatureType, class DataObjectType >
u_int16_t TextureBrowsingExtractor<SignatureType, DataObjectType>::getKernelSize(){

    return kernelSize;
}

template< class SignatureType, class DataObjectType >
void TextureBrowsingExtractor<SignatureType, DataObjectType>::setSigmaSize(u_int16_t value){

    sigmaSize = value;
}

template< class SignatureType, class DataObjectType >
u_int16_t TextureBrowsingExtractor<SignatureType, DataObjectType>::getSigmaSize(){

    return sigmaSize;
}

template< class SignatureType, class DataObjectType >
void TextureBrowsingExtractor<SignatureType, DataObjectType>::setThetaSize(u_int16_t value){

    thetaSize = value;
}

template< class SignatureType, class DataObjectType >
u_int16_t TextureBrowsingExtractor<SignatureType, DataObjectType>::getThetaSize(){

    return thetaSize;
}

template< class SignatureType, class DataObjectType >
void TextureBrowsingExtractor<SignatureType, DataObjectType>::setThreshold(double value){
    threshold = value;
}

template< class SignatureType, class DataObjectType >
double TextureBrowsingExtractor<SignatureType, DataObjectType>::getThreshold(){

    return threshold;
}

/**
 * Class destructor.
 */
template< class SignatureType, class DataObjectType >
TextureBrowsingExtractor<SignatureType, DataObjectType>::~TextureBrowsingExtractor() {
}


/**
* Generates the Texture Browsing from the image provided.
*
* The feature vector is composed by:
* First 5 positions: The PBC values;
*
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated color layout descriptor.
*/
template< class SignatureType, class DataObjectType >
void TextureBrowsingExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType &image, SignatureType &sign){    

    //set initial values
    u_int16_t pos_lm = 11;
    u_int16_t pos_psi = 90;
    double lm = 0.5 + ((double) pos_lm/ (double) 100.0);
    double ps = pos_psi;

    //create std::vector of results (gama, vertical contrast and horizontal contrast)
    std::vector<double> results;
    //create std::vector of DIS and STD
    std::vector<std::pair<double,double> > coordinates;
    std::vector<double> means, stds;

    //For each S,K configuration
    for (size_t x = 0; x < sigmaSize; x++){
        for (size_t y = 0; y < thetaSize; y++){
            //get table values
            double sig = (double) sigmaS[x];
            double th =  (double) thetaK[y];

            //generate kernel
            std::vector <std::vector<double> > resultImage, kernel = mkKernel(kernelSize, sig, th, lm, ps);

            //normalize kernel
            double min, max;
            max = DBL_MIN;
            min = DBL_MAX;
            for (size_t i = 0; i < kernelSize; i++){
                for (size_t j = 0; j < kernelSize;j++){
                    if (kernel[i][j] > max)
                        max = kernel[i][j];
                    if (kernel[i][j]< min)
                        min = kernel[i][j];
                }
            }
            for (size_t i = 0; i < kernelSize; i++){
                for (size_t j = 0; j < kernelSize;j++){
                    kernel[i][j] = (kernel[i][j]+fabs(min))/(fabs(max)+fabs(min));
                }
            }

            //apply the kernel on image with convolution
            resultImage = convolution(image, kernel);

            //calculate mean of projection
            means.push_back(calculateMean(resultImage));
            //calculate standart deviation of projection
            stds.push_back(calculateDeviation(resultImage, means[means.size()-1]));

            //calculate contrast of projection
            double gama, verticalContrast, horizontalContrast;
            calculateConstrastandGama(resultImage, &gama, &verticalContrast, &horizontalContrast, &coordinates);

            results.push_back(gama);
            results.push_back(verticalContrast);
            results.push_back(horizontalContrast);

            for(u_int16_t x = 0; x < resultImage.size(); x++)
                resultImage[x].clear();
            resultImage.clear();
            for(u_int16_t x = 0; x < kernel.size();x++)
                kernel[x].clear();
            kernel.clear();
        }
    }

    //calculate caracteristics
    std::vector<u_int16_t> clusteringPositions = agglomerativeClustering(coordinates);
    std::vector<double> PBC = calculatePBC2to5(results, clusteringPositions);
    double v1 = candidateClassification(results, clusteringPositions);

    //populate signature vector
    sign.resize(PBC.size() + means.size() + stds.size() + 1);
    sign[0] = v1;

    for(size_t i = 0; i < PBC.size(); i++){
        sign[i+1] = PBC[i];
    }
    for(size_t i = 0; i < means.size(); i++){
        sign[i + 5] = means[i];
    }
    for(size_t i = 0; i < stds.size(); i++){
        sign[i + 5 + means.size()] = stds[i];
    }

    double max = DBL_MIN;
    for(size_t i = 0; i < sign.size(); i++){
        if (sign[i] > max){
            max = sign[i];
        }
    }
    for(size_t i = 0; i < sign.size(); i++){
        sign[i] = ((double) sign[i]) / ((double) max);
    }

    PBC.clear();
    means.clear();
    stds.clear();
    coordinates.clear();

}//generate signature

template< class SignatureType, class DataObjectType >
std::vector<std::vector<double> > TextureBrowsingExtractor<SignatureType, DataObjectType>::mkKernel(u_int16_t ks, double sig, double th, double lm, double ps){

    int32_t hks = (ks-1)/2;
    double theta = th*M_PI/180.0;
    double psi = ps*M_PI/180.0;
    double del = 2.0/((double) (ks-1));
    double lmbd = lm;
    double sigma = (double) (sig / (double) ks);
    double x_theta;
    double y_theta;

    //initiate matrix
    std::vector<std::vector<double> > kernel(ks);
    for(size_t i = 0; i < ks; i++){
        kernel[i].resize(ks);
    }

    for (int32_t y = (int32_t) (-1*hks); y <= hks; y++){
        for (int32_t x = (int32_t) (-1*hks); x <= hks; x++){
            x_theta = x*del*cos(theta)+y*del*sin(theta);
            y_theta = -x*del*sin(theta)+y*del*cos(theta);
            kernel[hks+y][hks+x] = (double) exp( ((-0.5)*((x_theta*x_theta) + (y_theta*y_theta))/(sigma*sigma))*(cos(((2*M_PI*x_theta)/lmbd)+psi)) );
        }
    }

    return kernel;
}

template< class SignatureType, class DataObjectType >
std::vector<std::vector<double> > TextureBrowsingExtractor<SignatureType, DataObjectType>::convolution(const DataObjectType &image, std::vector<std::vector<double> > kernel){

    //matrix which stores the result
    std::vector< std::vector<double> > result(image.getWidth());
    for(size_t i = 0; i < image.getWidth();i++){
        result[i].resize(image.getHeight());
    }

    u_int16_t ks = ((kernel.size()-1)/2);

    //walk through image
    for(size_t i = 0; i < image.getWidth(); i++){
        for(size_t j = 0; j < image.getHeight(); j++){
            //apply filter
            double aux = 0.0;
            u_int16_t xx, yy;
            xx = 0;
            for(int16_t x = (int16_t) (-1*ks); x <= ks; x++){
                yy = 0;
                for(int16_t y = (int16_t) (-1*ks); y <= ks; y++){
                    //if kernel are beyond the border
                    if( (i + x < 0) || (i + x >= image.getWidth()) || (j + y < 0) || ((j + y) >= image.getHeight())){
                        aux += 0.0;
                    }else{
                        aux += (image.getPixel(i + x, j + y).getGrayPixelValue()/255)*kernel[xx][yy];
                    }
                    yy++;
                }
                xx++;
            }
            if (aux > 255.0)
                result[i][j] = 255.0;
            else
                result[i][j] = aux;
        }
    }


    return result;
}

template< class SignatureType, class DataObjectType >
double TextureBrowsingExtractor<SignatureType, DataObjectType>::calculateMean(std::vector<std::vector<double> > matrix){

    double aux = 0;

    for(size_t i = 0; i < matrix.size();i++){
        for(size_t j = 0; j < matrix[0].size();j++){
            aux += matrix[i][j];
        }
    }

    aux = aux/(matrix.size()*matrix[0].size());

    return aux;
}

template< class SignatureType, class DataObjectType >
double TextureBrowsingExtractor<SignatureType, DataObjectType>::calculateDeviation(std::vector<std::vector<double> > matrix, double mean){
    double aux = 0;

    for(size_t i = 0; i < matrix.size();i++){
        for(size_t j = 0; j < matrix[0].size();j++){
            aux += pow(matrix[i][j]-mean,2);
        }
    }

    aux = sqrt(aux/(matrix.size()*matrix[0].size()));
    return aux;
}

template< class SignatureType, class DataObjectType >
void TextureBrowsingExtractor<SignatureType, DataObjectType>::calculateConstrastandGama(std::vector<std::vector<double> > matrix, double *gama, double *verticalContrast, double *horizontalContrast, std::vector<std::pair<double, double> > *coordinates){
    //calculate horizontal means
    std::vector<double> horizontal;

    for (size_t i = 0; i < matrix.size();i++){
        horizontal.push_back(0);
        //for each column
        for (size_t j = 0; j < matrix[0].size();j++){
            horizontal[i] += matrix[i][j];
        }
        horizontal[i] /= matrix[0].size();//mean of each column
    }

    //calculate vertical means
    std::vector<double> vertical;

    for(size_t i = 0; i < matrix[0].size();i++){
        vertical.push_back(0);
        for(size_t j = 0; j< matrix.size();j++){
            vertical[i] += matrix[j][i];
        }
        vertical[i] /= matrix.size();//mean of each row
    }

    //calculate the autocorrelation of horizontal elements
    std::vector<double> aCHorizontal;
    double auxUp, auxDown,down1,down2;
    auxUp = auxDown =  down1 = down2 = 0.0;


    for(size_t k = 0; k < matrix.size(); k++){
        for (size_t m = k; m < (matrix.size() - 1); m++){
            auxUp += horizontal[m-k]*horizontal[m];
            down1 += pow(horizontal[m-k], 2.0);
            down2 += pow(horizontal[m], 2.0);
        }
        aCHorizontal.push_back(auxUp/sqrt(down1*down2));
    }

    //calculate the autocorrelation of vertical elements
    std::vector<double> aCVertical;
    for (size_t k = 0; k < matrix[0].size(); k++){
        auxUp = auxDown =  down1 = down2 = 0;
        for (size_t m = k; m < matrix[0].size() - 1; m++){
            auxUp += vertical[m-k]*vertical[m];
            down1 += pow(vertical[m-k],2);
            down2 += pow(vertical[m],2);
        }
        aCVertical.push_back(auxUp/sqrt(down1*down2));
    }

    //detection of peaks an valleys of horizontal values
    std::vector<std::pair<u_int16_t,double> > horizontalPeaks;
    std::vector<std::pair<u_int16_t,double> > horizontalValleys;

    for(size_t i = 1; i < matrix.size()-1;i++){
        //detection of peak
        if((aCHorizontal[i-1] <= aCHorizontal[i]) && (aCHorizontal[i+1] <= aCHorizontal[i])){
            //save position and magnitude
            horizontalPeaks.push_back(make_pair(i, aCHorizontal[i]));
        }
        //detection of valley
        if((aCHorizontal[i-1] >= aCHorizontal[i]) && (aCHorizontal[i+1] >= aCHorizontal[i])){
            //save position and magnitude
            horizontalValleys.push_back(make_pair(i, aCHorizontal[i]));
        }
    }

    //detection of peaks an valleys of vertical values
    std::vector<std::pair<u_int16_t, double> > verticalPeaks;
    std::vector<std::pair<u_int16_t, double> > verticalValleys;

    for(size_t i = 1; i < matrix[0].size()-1;i++){
        //detection of peak
        if((aCVertical[i-1] <= aCVertical[i]) && (aCVertical[i+1] <= aCVertical[i])){
            //save position and magnitude
            verticalPeaks.push_back(make_pair(i, aCVertical[i]));
        }
        //detection of valley
        if((aCVertical[i-1] >= aCVertical[i]) && (aCVertical[i+1] >= aCVertical[i])){
            //save position and magnitude
            verticalValleys.push_back(make_pair(i, aCVertical[i]));
        }
    }


    //calculate gama
    double auxDIS = 0.;

    //sum of horizontal peaks
    for(size_t i = 0; i < horizontalPeaks.size();i++){
        auxDIS += horizontalPeaks[i].second;
    }
    //sum of vertical peaks
    for (size_t i = 0; i < verticalPeaks.size();i++){
        auxDIS += verticalPeaks[i].second;
    }
    //calculate the mean
    auxDIS /= (horizontalPeaks.size()+verticalPeaks.size());

    double auxSTD = 0.0;
    //difference between horizontal peaks
    for (size_t i = 0; i < horizontalPeaks.size();i++){
        auxSTD += pow(horizontalPeaks[i].second-auxDIS,2);
    }
    //difference between vertical peaks
    for (size_t i = 0; i < verticalPeaks.size();i++){
        auxSTD += pow(verticalPeaks[i].second-auxDIS,2);
    }
    //calculate standard deviation
    auxSTD = sqrt(auxSTD/(horizontalPeaks.size()+verticalPeaks.size()));

    //save gama value
    *gama = auxSTD/auxDIS;

    coordinates->push_back(make_pair(auxDIS, auxSTD));

    //calculate contrast of horizontal value
    double  auxPeak, auxValley;

    auxPeak = auxValley = 0;
    for(size_t i = 0; i < horizontalPeaks.size(); i++){
        auxPeak += horizontalPeaks[i].second;
    }
    auxPeak /= horizontalPeaks.size();
    for(size_t i = 0; i < horizontalValleys.size(); i++){
        auxValley += horizontalValleys[i].second;
    }
    auxValley /= horizontalValleys.size();

    *horizontalContrast = auxPeak - auxValley;

    //calculate contrast of vertical value
    auxPeak = auxValley = 0;
    for(size_t i = 0; i < verticalPeaks.size(); i++){
        auxPeak += verticalPeaks[i].second;
    }
    auxPeak /= verticalPeaks.size();
    for(size_t i = 0; i < verticalValleys.size(); i++){
        auxValley += verticalValleys[i].second;
    }
    auxValley /= verticalValleys.size();

    *verticalContrast = auxPeak - auxValley;
}

template< class SignatureType, class DataObjectType >
std::vector<double> TextureBrowsingExtractor<SignatureType, DataObjectType>::calculatePBC2to5(std::vector<double> results, std::vector<u_int16_t> clusteringPoints){

    u_int16_t posH, posV;
    std::vector<double> PBC;
    double maxH, maxV;
    posH = posV = maxH =  maxV = DBL_MIN;

    std::vector<u_int16_t> candidatesPosition;

    for(u_int16_t i = 0; i < results.size()/3; i++)
        candidatesPosition.push_back(0);

    for(u_int16_t i = 0; i < clusteringPoints.size(); i++)
        candidatesPosition[clusteringPoints[i]] = 1;

    //check gama values using a threshold
    for(u_int16_t i = 0; i < results.size(); i = i+3){
        if((results[i] < getThreshold()) && (candidatesPosition[i/3] == 1)){
            //check max vertical contrast
            if(results[i+1] > maxV){
                maxV = results[i+1];
                posV = i/3;
            }
            //check max horizontal contrast
            if(results[i+2] > maxH){
                maxH = results[i+2];
                posH = i/3;
            }
        }
    }

    if(posH == DBL_MIN || posV == DBL_MIN){
        //no candidates pass threshold
        PBC.push_back(0.0);
        PBC.push_back(0.0);
        PBC.push_back(0.0);
        PBC.push_back(0.0);
    }else{
        //save result values
        if(posH == 0){
            PBC.push_back((double)thetaK[0]/thetaK[thetaSize-1]);
            PBC.push_back((double)sigmaS[0]/sigmaS[sigmaSize-1]);
        }else{
            PBC.push_back((double)thetaK[(posH-1)%thetaSize]/thetaK[thetaSize-1]);
            PBC.push_back((double)sigmaS[(posH-1)/thetaSize]/sigmaS[sigmaSize-1]);
        }

        if(posV == 0){
            PBC.push_back((double)thetaK[0]/thetaK[thetaSize-1]);
            PBC.push_back((double)sigmaS[0]/sigmaS[sigmaSize-1]);
        }else{
            PBC.push_back((double)thetaK[(posV-1)%thetaSize]/thetaK[thetaSize-1]);
            PBC.push_back((double)sigmaS[(posV-1)/thetaSize]/sigmaS[sigmaSize-1]);
        }
    }

    return PBC;
}

template< class SignatureType, class DataObjectType >
double TextureBrowsingExtractor<SignatureType, DataObjectType>::candidateClassification(std::vector<double> results, std::vector<u_int16_t> clusteringPoints){

    std::vector<u_int16_t> values;
    u_int16_t candidates = 0;
    bool C1, C2;
    C1 = C2 = false;

    //initialize std::vector
    values.push_back(0);
    values.push_back(0);
    values.push_back(0);

    std::vector<u_int16_t> candidatesPosition;

    for(u_int16_t i = 0; i < results.size()/3; i++)
        candidatesPosition.push_back(0);

    for(u_int16_t i = 0; i < clusteringPoints.size(); i++)
        candidatesPosition[clusteringPoints[i]] = 1;

    for(u_int16_t i = 0; i < results.size(); i = i + 3){
        for(u_int16_t j = 0; j < results.size(); j = j + 3){
            if((results[i] < getThreshold()) && (results[j] < getThreshold()) && (i != j) && (candidatesPosition[i/3] == 1) && (candidatesPosition[j/3] == 1)){
                candidates++;
                u_int16_t scaleI, scaleJ, orientationI, orientationJ;
                //get scale and orientation of I
                if(i == 0){
                    scaleI = orientationI = 0;
                }else{
                    scaleI = (i-1)/thetaSize;
                    orientationI = (i-1)%thetaSize;
                }
                //get scale and orientation of J
                if(j == 0){
                    scaleJ = orientationJ = 0;
                }else{
                    scaleJ = (j-1)/thetaSize;
                    orientationJ = (j-1)%thetaSize;
                }
                //verify C1 condition
                if((fabs(scaleI - scaleJ) == 1) || (fabs(orientationI - orientationJ) == 1)){
                    C1 = true;
                }
                //verify C2 condition
                if(scaleI == scaleJ || orientationI == orientationJ){
                    C2 = true;
                }

                if(C1){
                    values[0]++;
                }else{
                    if(C2){
                        values[1]++;
                    }
                    else{
                        values[2]++;
                    }

                }
            }
        }
    }
    //calculate M value
    double mValue = 0;
    std::vector<double> cValues;
    cValues.push_back(1.0);
    cValues.push_back(0.5);
    cValues.push_back(0.2);
    for(u_int16_t i = 0; i < 3; i++){
        mValue += values[i]*cValues[i];
    }
    if(candidates == 0)
        return 0;
    else
        return (double) mValue/candidates;
}


template< class SignatureType, class DataObjectType >
std::vector<u_int16_t> TextureBrowsingExtractor<SignatureType, DataObjectType>::agglomerativeClustering(std::vector<std::pair<double,double> > coordinates){

    std::vector< std::vector<double> > distances(coordinates.size());
    std::vector<u_int16_t> position;
    std::vector<u_int16_t> clusteringPoints;


    //calculate euclidean distance for all coordinates
    for (size_t i = 0; i < coordinates.size();i++){
        position.push_back(i);
        for(size_t j = 0; j < coordinates.size();j++){
            distances[i].push_back( sqrt( pow((coordinates[i].first-coordinates[j].first), 2.0) + pow((coordinates[i].second - coordinates[j].second), 2.0)));
        }
    }

    //verify the lowest distance between 2 coordinates
    double minDistance = DBL_MAX;
    size_t point1, point2;
    point1 = point2 = 0;

    for (size_t x = 0; x < distances.size(); x++){
        for (size_t y = 0; y < distances.size(); y++){
            //if true save the position of Points
            if ((x != y) && (distances[x][y] <= minDistance)){
                minDistance = distances[x][y];
                point1 = x;
                point2 = y;
            }
        }
    }

    //save which projection is near
    clusteringPoints.push_back(point1);
    clusteringPoints.push_back(point2);

    //update coordinate with the mean value of both Points
    coordinates[point1].first = (coordinates[point1].first + coordinates[point2].first)/2;
    coordinates[point1].second = (coordinates[point1].second + coordinates[point2].second)/2;

    //remove one point
    position.erase(position.begin()+point2);
    coordinates.erase(coordinates.begin()+point2);

    //search for another 10 nearest Points
    for(u_int16_t k = 0; k < 10; k++){

        //discover the new position of cluster on std::vector
        u_int16_t newPos = 0;
        while(position[newPos] != clusteringPoints[0])
            newPos++;

        //calculate euclidean distance for all coordinates
        std::vector<double> newDistances;
        for(u_int16_t i = 0; i < coordinates.size();i++){
            newDistances.push_back(sqrt(pow(coordinates[newPos].first-coordinates[i].first,2) + pow(coordinates[newPos].second - coordinates[i].second,2)));
        }

        //verify the lowest distance between the cluster and coordinates
        minDistance = DBL_MAX;
        for(u_int16_t x = 0; x < newDistances.size(); x++){
            if((newDistances[x] <= minDistance) && (newPos != x)){
                minDistance = newDistances[x];
                point1 = x;
            }
        }

        //save which projection is near
        clusteringPoints.push_back(position[point1]);

        //update coordinate with the mean value of both Points
        coordinates[newPos].first = (coordinates[newPos].first + coordinates[point1].first)/2;
        coordinates[point1].second = (coordinates[newPos].second + coordinates[point1].second)/2;

        //remove one point
        position.erase(position.begin()+point1);
        coordinates.erase(coordinates.begin()+point1);
    }
    return clusteringPoints;
}

