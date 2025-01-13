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
 * This file implements the complex methods of the EdgeHistogramExtractor class.
 */

template<class SignatureType, class DataObjectType>
EdgeHistogramExtractor<SignatureType, DataObjectType>::EdgeHistogramExtractor(u_int16_t numBlocks){

    setNumBlocks(numBlocks);
}

template<class SignatureType, class DataObjectType>
EdgeHistogramExtractor<SignatureType, DataObjectType>::~EdgeHistogramExtractor(){
}

template<class SignatureType, class DataObjectType>
void EdgeHistogramExtractor<SignatureType, DataObjectType>::setNumBlocks(u_int16_t numBlocks){

    this->numBlocks = numBlocks;
}

template<class SignatureType, class DataObjectType>
u_int16_t EdgeHistogramExtractor<SignatureType, DataObjectType>::getNumBlocks(){

    return numBlocks;
}

/**
 * Generates the Edge Histogram from the image provided.
 *
 * @param image The image to be processed.
 * @param[out] sign The object to store the generated color layout descriptor.
 *
 * @return The resulting ExtReturnCode.
*/
template<class SignatureType, class DataObjectType>
void EdgeHistogramExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType &image, SignatureType &sign) throw (std::runtime_error) {

    //vector that will store in each postion the features of a given block
    std::vector<EdgeHistogram> histogram;


    //extracts local edge histogram
    histogram = extractLocalEdgeHistogramFromImage(calculateBlockSize(image.getWidth(), image.getHeight()),
                                                   image.getWidth(),
                                                   image.getHeight(),
                                                   image,
                                                   histogram);

    //extracts local and global edge histogram
    histogram = extractSemiLocalGlobalHistogram(histogram);

    //fills sign vector
    size_t aux = 0;
    sign.resize(150, 0.0);
    for (size_t i = 0; i < histogram.size()*NUM_FEATURES; i += 5){
        sign[i] = histogram[i-4*aux].getVerticalEdge();
        sign[i+1] = histogram[i-4*aux].getHorizontalEdge();
        sign[i+2] = histogram[i-4*aux].getDegree45Edge();
        sign[i+3] = histogram[i-4*aux].getDegree135Edge();
        sign[i+4] = histogram[i-4*aux].getNonDirectionalEdge();
        aux++;
    }

    double max = DBL_MIN;
    for (size_t x = 0; x < sign.size(); x++){
        if (sign[x] > max){
            max = sign[x];
        }
    }

    for (size_t x = 0; x < sign.size(); x++){
        sign[x] = ((double) sign[x]) / ((double) max);
    }


} //generate signature

/**
* Calculates the size of a block of the image.
*
* @param width The width of the image.
* @param height The height of the image.
*
* @return The block size.
*/
template<class SignatureType, class DataObjectType>
u_int16_t EdgeHistogramExtractor<SignatureType, DataObjectType>::calculateBlockSize(const u_int16_t width, const u_int16_t height) {

    // return static_cast<u_int16_t>(sqrt((width*height)/getNumBlocks()));
    
    u_int16_t blockSize = static_cast<u_int16_t>(sqrt((width*height)/getNumBlocks()));
    
    if(blockSize != 0) {
        return blockSize;
    } else { /* in case of block with 0 pixels, readjust the image blocks */
        setNumBlocks(width*height);
        return 1;
    }

} //calculateBlockSzie

/**
* Calculates the type of the edge for a block of the image.
*
* @param serializedGrayImage The serialized gray image.
* @param width The width of the image.
* @param blockSize The block size of the image.
* @param offset The shift that represents a block size.
*
* @return The type of edge for a block of the image.
*/
template<class SignatureType, class DataObjectType>
u_int16_t EdgeHistogramExtractor<SignatureType, DataObjectType>::calculateEgdeFeature(const DataObjectType &image, const u_int16_t width, const u_int16_t blockSize, const u_int16_t x, const u_int16_t y) {

    double d1, d2, d3, d4, eH, eV, e45, e135, eM, eMax;
    u_int16_t edgeIndex;

    d1 = d2 = d3 = d4 = 0.0;

    for (size_t j = 0; j < blockSize; j++) {
        for (size_t i = 0; i < blockSize; i++) {
            if (j < blockSize/2) {
                if(i < blockSize/2) {
                    // d1 += serializedGrayImage[i + width*j + offset];
                    d1 += image.getPixel(i+x,j+y).getGrayPixelValue();
                } else {
                    //d2 += serializedGrayImage[i + width*j + offset];
                    d2 += image.getPixel(i+x,j+y).getGrayPixelValue();
                }
            }
            else {
                if (i < blockSize/2) {
                    //d3 += serializedGrayImage[i + width*j + offset];
                    d3 += image.getPixel(i+x,j+y).getGrayPixelValue();
                } else {
                    //d4 += serializedGrayImage[i + width*j + offset];
                    d4 += image.getPixel(i+x,j+y).getGrayPixelValue();
                }
            }
        }
    }

    d1 = d1/(static_cast<double>(blockSize*blockSize)/NUM_DIVISIONS_IMAGE);
    d2 = d2/(static_cast<double>(blockSize*blockSize)/NUM_DIVISIONS_IMAGE);
    d3 = d3/(static_cast<double>(blockSize*blockSize)/NUM_DIVISIONS_IMAGE);
    d4 = d4/(static_cast<double>(blockSize*blockSize)/NUM_DIVISIONS_IMAGE);

    eH = fabs(d1 + d2 - (d3 + d4));
    eV = fabs(d1 + d3 - (d2 + d4));
    e45 = sqrt(2)*fabs(d1 - d4);
    e135 = sqrt(2)*fabs(d2 - d3);
    eM = 2*fabs(d1 - d2 - d3 + d4);
    eMax = eV;

    edgeIndex = EdgeHistogramExtractor::VERTICAL_EDGE;

    if(eH > eMax) {
        eMax = eH;
        edgeIndex = EdgeHistogramExtractor::HORIZONTAL_EDGE;
    }
    if(e45 > eMax) {
        eMax = e45;
        edgeIndex = EdgeHistogramExtractor::DIAGONAL_45_DEGREE_EDGE;
    }
    if(e135 > eMax) {
        eMax = e135;
        edgeIndex = EdgeHistogramExtractor::DIAGONAL_135_DEGREE_EDGE;
    }
    if(eM > eMax) {
        eMax = eM;
        edgeIndex = NON_DIRECIONAL_EDGE;
    }
    if(eMax < NUM_MIN_OF_MAX_EDGE) {
        edgeIndex = NO_EDGE;
    }

    return edgeIndex;

} //calculateEgdeFeature

/**
* Extracts the edge histogram from a serialized gray image.
* @param blockSize The block size of the image.
* @param width The width of the image.
* @param height The height of the image.
* @param serializedGrayImage The serialized gray image.
* @param[out] sign The descriptor storing the extracted datas.
*
* @return The resulting ExtReturnCode.
*/
template<class SignatureType, class DataObjectType>
std::vector<EdgeHistogram> EdgeHistogramExtractor<SignatureType, DataObjectType>::extractLocalEdgeHistogramFromImage(
        const u_int16_t blockSize, const u_int16_t width, const u_int16_t height, const DataObjectType &image, std::vector<EdgeHistogram> histogram)
throw (std::runtime_error) {

    u_int16_t subLocalIndex, *countSubLocalIndex, edgeTypeOfBlock, offset;
    double *extractedData, *normalizedData;

    try {
        countSubLocalIndex = new u_int16_t[static_cast<u_int16_t>(NUM_DIVISIONS_IMAGE*NUM_DIVISIONS_IMAGE)];
        extractedData = new double[static_cast<u_int16_t>(NUM_DIVISIONS_IMAGE*NUM_DIVISIONS_IMAGE*NUM_FEATURES)];
        normalizedData = new double[static_cast<u_int16_t>(NUM_DIVISIONS_IMAGE*NUM_DIVISIONS_IMAGE*NUM_FEATURES)];
    } catch(...) {
        throw std::runtime_error("Cannot allocate mem on local edges on Edge Histogram Extractor");
    }

    for (size_t i = 0; i < NUM_DIVISIONS_IMAGE*NUM_DIVISIONS_IMAGE; i++) {
        countSubLocalIndex[i] = 0;
    }

    for (size_t i = 0; i < NUM_DIVISIONS_IMAGE*NUM_DIVISIONS_IMAGE*NUM_FEATURES; i++) {
        extractedData[i] = 0.0;
        normalizedData[i] = 0.0;
    }
    for (size_t j = 0 ; j < height - blockSize; j += blockSize) {
        for(size_t i = 0; i < width - blockSize; i += blockSize) {
            subLocalIndex = static_cast<u_int16_t>(i*NUM_DIVISIONS_IMAGE_INT/width) +
                    static_cast<u_int16_t>(j*NUM_DIVISIONS_IMAGE_INT/height)*NUM_DIVISIONS_IMAGE_INT;
            countSubLocalIndex[subLocalIndex]++;
            offset = width*j + i;
            edgeTypeOfBlock = calculateEgdeFeature(image, width, blockSize, i, j);
            switch(edgeTypeOfBlock) {
                case NO_EDGE: {
                    break;
                }
                case EdgeHistogramExtractor::VERTICAL_EDGE: {
                    extractedData[subLocalIndex*NUM_FEATURES]++;
                    break;
                }
                case EdgeHistogramExtractor::HORIZONTAL_EDGE: {
                    extractedData[subLocalIndex*NUM_FEATURES + 1]++;
                    break;
                }
                case EdgeHistogramExtractor::DIAGONAL_45_DEGREE_EDGE: {
                    extractedData[subLocalIndex*NUM_FEATURES + 2]++;
                    break;
                }
                case EdgeHistogramExtractor::DIAGONAL_135_DEGREE_EDGE: {
                    extractedData[subLocalIndex*NUM_FEATURES + 3]++;
                    break;
                }
                case NON_DIRECIONAL_EDGE: {
                    extractedData[subLocalIndex*NUM_FEATURES + 4]++;
                    break;
                }
            }
        }
    }

    // normalization
    for(size_t i = 0; i < NUM_DIVISIONS_IMAGE_INT*NUM_DIVISIONS_IMAGE_INT*NUM_FEATURES; i++) {
        subLocalIndex = static_cast<u_int16_t>(i/NUM_FEATURES);
        normalizedData[i] = extractedData[i]/countSubLocalIndex[subLocalIndex];
    }

    // puts the extracted data u_int16_to result
    for(size_t i = 0; i < NUM_DIVISIONS_IMAGE_INT*NUM_DIVISIONS_IMAGE_INT*NUM_FEATURES; i = i+5) {
        EdgeHistogram aux(normalizedData[i], normalizedData[i+1], normalizedData[i+2], normalizedData[i+3], normalizedData[i+4]);
        histogram.push_back(aux);
    }

    delete[] countSubLocalIndex;
    delete[] extractedData;
    delete[] normalizedData;

    return histogram;

} //extractEdgeHistogramFromSerializedGrayImage

/**
* Extracts the local and global edge histogram from the clusters of original paper.
*
* @param histogram The local histogram of all 16 blocks
*
* @return The resulting histogram with 30 differents clulsters (150 bins).
*/
template<class SignatureType, class DataObjectType>
std::vector<EdgeHistogram> EdgeHistogramExtractor<SignatureType, DataObjectType>::extractSemiLocalGlobalHistogram(std::vector<EdgeHistogram> histogram){

    double auxVertical, auxHorizontal, aux45, aux135, auxNonDirectional;

    //Cluster 1,2,3 and 4 (like original paper)
    auxVertical = auxHorizontal = aux45 = aux135 = auxNonDirectional = 0;
    for(size_t j = 0; j < 4;j++){
        for(size_t i = 0; i < 4; i++){
            auxVertical += histogram[j+i+3*i].getVerticalEdge();
            auxHorizontal += histogram[j+i+3*i].getHorizontalEdge();
            aux45 += histogram[j+i+3*i].getDegree45Edge();
            aux135 += histogram[j+i+3*i].getDegree135Edge();
            auxNonDirectional += histogram[j+i+3*i].getNonDirectionalEdge();
        }
        //store the mean
        EdgeHistogram aux(auxVertical/4, auxHorizontal/4, aux45/4, aux135/4, auxNonDirectional/4);
        histogram.push_back(aux);
    }

    //Cluster 5,6,7 and 8 (like original paper)
    auxVertical = auxHorizontal = aux45 = aux135 = auxNonDirectional = 0;
    for(size_t j = 0; j < 4;j++){
        for(size_t i = 0; i < 4; i++){
            auxVertical += histogram[i+j].getVerticalEdge();
            auxHorizontal += histogram[i+j].getHorizontalEdge();
            aux45 += histogram[i+j].getDegree45Edge();
            aux135 += histogram[i+j].getDegree135Edge();
            auxNonDirectional += histogram[i+j].getNonDirectionalEdge();
        }
        //store the mean
        EdgeHistogram aux(auxVertical/4, auxHorizontal/4, aux45/4, aux135/4, auxNonDirectional/4);
        histogram.push_back(aux);
    }

    //Cluster 9 (like original paper)
    auxVertical = auxHorizontal = aux45 = aux135 = auxNonDirectional = 0;
    auxVertical += histogram[0].getVerticalEdge() + histogram[1].getVerticalEdge() +
            histogram[4].getVerticalEdge() + histogram[5].getVerticalEdge();
    auxHorizontal += histogram[0].getHorizontalEdge() + histogram[1].getHorizontalEdge() +
            histogram[4].getHorizontalEdge() + histogram[5].getHorizontalEdge();
    aux45 += histogram[0].getDegree45Edge() + histogram[1].getDegree45Edge() +
            histogram[4].getDegree45Edge() + histogram[5].getDegree45Edge();
    aux135 += histogram[0].getDegree135Edge() + histogram[1].getDegree135Edge() +
            histogram[4].getDegree135Edge() + histogram[5].getDegree135Edge();
    auxNonDirectional += histogram[0].getNonDirectionalEdge() + histogram[1].getNonDirectionalEdge() +
            histogram[4].getNonDirectionalEdge() + histogram[5].getNonDirectionalEdge();
    //store the mean
    EdgeHistogram aux(auxVertical/4, auxHorizontal/4, aux45/4, aux135/4, auxNonDirectional/4);
    histogram.push_back(aux);

    //Cluster 10 (like original paper)
    auxVertical = auxHorizontal = aux45 = aux135 = auxNonDirectional = 0;
    auxVertical += histogram[2].getVerticalEdge() + histogram[3].getVerticalEdge() +
            histogram[6].getVerticalEdge() + histogram[7].getVerticalEdge();
    auxHorizontal += histogram[2].getHorizontalEdge() + histogram[3].getHorizontalEdge() +
            histogram[6].getHorizontalEdge() + histogram[7].getHorizontalEdge();
    aux45 += histogram[2].getDegree45Edge() + histogram[3].getDegree45Edge() +
            histogram[6].getDegree45Edge() + histogram[7].getDegree45Edge();
    aux135 += histogram[2].getDegree135Edge() + histogram[3].getDegree135Edge() +
            histogram[6].getDegree135Edge() + histogram[7].getDegree135Edge();
    auxNonDirectional += histogram[2].getNonDirectionalEdge() + histogram[3].getNonDirectionalEdge() +
            histogram[6].getNonDirectionalEdge() + histogram[7].getNonDirectionalEdge();
    //store the mean
    EdgeHistogram aux1(auxVertical/4, auxHorizontal/4, aux45/4, aux135/4, auxNonDirectional/4);
    histogram.push_back(aux1);

    //Cluster 11 (like original paper)
    auxVertical = auxHorizontal = aux45 = aux135 = auxNonDirectional = 0;
    auxVertical += histogram[8].getVerticalEdge() + histogram[9].getVerticalEdge() +
            histogram[12].getVerticalEdge() + histogram[13].getVerticalEdge();
    auxHorizontal += histogram[8].getHorizontalEdge() + histogram[9].getHorizontalEdge() +
            histogram[12].getHorizontalEdge() + histogram[13].getHorizontalEdge();
    aux45 += histogram[8].getDegree45Edge() + histogram[9].getDegree45Edge() +
            histogram[12].getDegree45Edge() + histogram[13].getDegree45Edge();
    aux135 += histogram[8].getDegree135Edge() + histogram[9].getDegree135Edge() +
            histogram[12].getDegree135Edge() + histogram[13].getDegree135Edge();
    auxNonDirectional += histogram[8].getNonDirectionalEdge() + histogram[9].getNonDirectionalEdge() +
            histogram[12].getNonDirectionalEdge() + histogram[13].getNonDirectionalEdge();
    //store the mean
    EdgeHistogram aux2(auxVertical/4, auxHorizontal/4, aux45/4, aux135/4, auxNonDirectional/4);
    histogram.push_back(aux2);

    //Cluster 12 (like original paper)
    auxVertical = auxHorizontal = aux45 = aux135 = auxNonDirectional = 0;
    auxVertical += histogram[10].getVerticalEdge() + histogram[11].getVerticalEdge() +
            histogram[14].getVerticalEdge() + histogram[15].getVerticalEdge();
    auxHorizontal += histogram[10].getHorizontalEdge() + histogram[11].getHorizontalEdge() +
            histogram[14].getHorizontalEdge() + histogram[15].getHorizontalEdge();
    aux45 += histogram[10].getDegree45Edge() + histogram[11].getDegree45Edge() +
            histogram[14].getDegree45Edge() + histogram[15].getDegree45Edge();
    aux135 += histogram[10].getDegree135Edge() + histogram[11].getDegree135Edge() +
            histogram[14].getDegree135Edge() + histogram[15].getDegree135Edge();
    auxNonDirectional += histogram[10].getNonDirectionalEdge() + histogram[14].getNonDirectionalEdge() +
            histogram[11].getNonDirectionalEdge() + histogram[15].getNonDirectionalEdge();
    //store the mean
    EdgeHistogram aux3(auxVertical/4, auxHorizontal/4, aux45/4, aux135/4, auxNonDirectional/4);
    histogram.push_back(aux3);

    //Cluster 13 (like original paper)
    auxVertical = auxHorizontal = aux45 = aux135 = auxNonDirectional = 0;
    auxVertical += histogram[5].getVerticalEdge() + histogram[6].getVerticalEdge() +
            histogram[9].getVerticalEdge() + histogram[10].getVerticalEdge();
    auxHorizontal += histogram[5].getHorizontalEdge() + histogram[6].getHorizontalEdge() +
            histogram[9].getHorizontalEdge() + histogram[10].getHorizontalEdge();
    aux45 += histogram[5].getDegree45Edge() + histogram[6].getDegree45Edge() +
            histogram[9].getDegree45Edge() + histogram[10].getDegree45Edge();
    aux135 += histogram[5].getDegree135Edge() + histogram[6].getDegree135Edge() +
            histogram[9].getDegree135Edge() + histogram[10].getDegree135Edge();
    auxNonDirectional += histogram[5].getNonDirectionalEdge() + histogram[6].getNonDirectionalEdge() +
            histogram[9].getNonDirectionalEdge() + histogram[10].getNonDirectionalEdge();
    //store the mean
    EdgeHistogram aux4(auxVertical/4, auxHorizontal/4, aux45/4, aux135/4, auxNonDirectional/4);
    histogram.push_back(aux4);

    //Global bins
    auxVertical = auxHorizontal = aux45 = aux135 = auxNonDirectional = 0;
    auxVertical += histogram[25].getVerticalEdge() + histogram[26].getVerticalEdge() +
            histogram[27].getVerticalEdge() + histogram[28].getVerticalEdge();
    auxHorizontal += histogram[25].getHorizontalEdge() + histogram[26].getHorizontalEdge() +
            histogram[27].getHorizontalEdge() + histogram[28].getHorizontalEdge();
    aux45 += histogram[25].getDegree45Edge() + histogram[26].getDegree45Edge() +
            histogram[27].getDegree45Edge() + histogram[28].getDegree45Edge();
    aux135 += histogram[25].getDegree135Edge() + histogram[26].getDegree135Edge() +
            histogram[27].getDegree135Edge() + histogram[28].getDegree135Edge();
    auxNonDirectional += histogram[25].getNonDirectionalEdge() + histogram[26].getNonDirectionalEdge() +
            histogram[27].getNonDirectionalEdge() + histogram[28].getNonDirectionalEdge();
    //store the mean
    EdgeHistogram aux5(auxVertical/4, auxHorizontal/4, aux45/4, aux135/4, auxNonDirectional/4);
    histogram.push_back(aux5);

    return histogram;
}//extractSemiLocalGlobalHistogram
