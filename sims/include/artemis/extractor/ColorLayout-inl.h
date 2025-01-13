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
* This file implements the complex methods of the ColorLayoutExtractor class.
*/
template<class SignatureType, class DataObjectType >
void ColorLayoutExtractor<SignatureType, DataObjectType>::setNumFeatures(u_int16_t value){
    numFeatures = value;
}

template<class SignatureType, class DataObjectType >
u_int16_t ColorLayoutExtractor<SignatureType, DataObjectType>::getNumFeatures(){
    return numFeatures;
}

template<class SignatureType, class DataObjectType >
void ColorLayoutExtractor<SignatureType, DataObjectType>::setNumBlocks(u_int16_t value){
    numBlocks = value;
}

template<class SignatureType, class DataObjectType >
u_int16_t ColorLayoutExtractor<SignatureType, DataObjectType>::getNumBlocks(){
    return numBlocks;
}

template<class SignatureType, class DataObjectType >
ColorLayoutExtractor<SignatureType, DataObjectType>::ColorLayoutExtractor(u_int16_t value1, u_int16_t value2){
    setNumFeatures(value1);
    setNumBlocks(value2);
}

template<class SignatureType, class DataObjectType >
ColorLayoutExtractor<SignatureType, DataObjectType>::~ColorLayoutExtractor() { }

/**
* Get the dimension of the square matrix that represent the image divided.
* If the dimension value is invalid, in other words, if numBlocks isn't a perfect square number
* a NotAvaliableException is raised.
*
* @return The dimension of the square matrix.
*/
template<class SignatureType, class DataObjectType >
u_int16_t ColorLayoutExtractor<SignatureType, DataObjectType>::getDimension() throw (std::bad_alloc*) {

    if(getNumBlocks() <= 0) {
        throw std::bad_alloc();
    }

    //verify whether the number of blocks passed by template is valid,
    //this number should be a perfect square.
    double fractpart, intpart, value = sqrt(getNumBlocks());

    fractpart = modf (value , &intpart);

    if(fractpart != 0) {
        throw std::bad_alloc();
    }

    return static_cast<u_int16_t>(value);
}

/**
* Generates the Color Layout from the image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated color layout descriptor.
* @return The resulting ExtReturnCode.
*/
template<class SignatureType, class DataObjectType >
void ColorLayoutExtractor<SignatureType, DataObjectType>::generateSignature(
    const DataObjectType &image, SignatureType &sign) throw (std::runtime_error) {

    //Dimension of square matrix in which the image will be divided
    u_int16_t dimension = getDimension();

    //Number of image channels
    u_int16_t channels = image.getChannels();

    //Matrix that will store the data blocks of all channels
    std::vector<std::vector<int32_t> > blocks(channels, std::vector<int32_t>(getNumBlocks()));

    for(size_t x = 0; x < channels; x++)
        for(size_t y = 0; y < getNumBlocks(); y++)
            blocks[x][y] = 0;                

    //Part 1: Division of image into blocks. Each image channel will have its own set of blocks. The number of blocks is
    //setted in the ColorLayoutExtractor::BLOCKS constant.
    //a 3x64 matrix  -> the three image channels will each have their own block set.
    //blocks[0][x] -> block on the luminance (Y)    
    //blocks[1][x] -> block on the red color (Cr)
    //blocks[2][x] -> block on the blue color (Cb)
    //Part 2: for each image color channel choose the color representative of each block (the color representative will the average
    //color of pixels of each block)
    try {
        blocks = imageSplitter(channels, dimension, image, blocks);
    }
    catch(...) {
        for(size_t i = 0; i < blocks.size(); i++) {
            blocks[i].clear();
        }
        blocks.clear();
        throw std::runtime_error("Cannot splitt the image");
    }


    //Part 3: DCT applied on blocks in each image color channel
    //Otimization: will be used the matrix form of the DCT
    DiscreteCosineTransformation *DCT = new DiscreteCosineTransformation();
    try {
        blocks[0] = DCT->applyDCT(blocks[0]);
		blocks[1] = DCT->applyDCT(blocks[1]);
        blocks[2] = DCT->applyDCT(blocks[2]);
    }
    catch(...) {
        for(size_t i = 0; i < blocks.size(); i++) {
            blocks[i].clear();
        }
        blocks.clear();
        delete(DCT);
        throw std::runtime_error("Cannot apply DCT on Color Layout");
    }


    //matrices that will store the data obtain in this extractor
    double *signAux;
    int32_t **descriptor;
    try {
        descriptor = new int32_t*[channels];
        signAux = new double[channels*getNumBlocks()];
        for(size_t i = 0; i < channels; i++) {
            descriptor[i] = new int32_t[getNumBlocks()];
        }
    }
    catch(...) {
        for(size_t i = 0; i < blocks.size(); i++) {
            blocks[i].clear();
        }
        blocks.clear();
        throw std::runtime_error("Cannot allocate mem on Color Layout.");
    }

    for(size_t x = 0; x < channels; x++)
        for(size_t y = 0; y < getNumBlocks(); y++) {
            descriptor[x][y] = 0;
            signAux[x+y] = 0.0;
        }
    //Part 4: quantization of the low frequency coefficients using the zigzag algorithm for scanning
    try {
        sumQuantify(dimension, blocks, descriptor);
    }
    catch(...) {
        for(size_t i = 0; i < channels; i++) {
            delete[] descriptor[i];
            blocks[i].clear();
        }
        blocks.clear();
        delete[] descriptor;
        throw std::runtime_error("Cannot quantify the sum on Color Layout");
    }

    //populate signature vector with values 8x4x4 (8Y,4Cr,4Cb)
    //values for Y
    sign.resize(16, 0.0);
    for(size_t i = 0; i < 8; i++){
        sign[i] = (double) descriptor[0][i] / (double) (numFeatures-1);
    }
    //Values for Cr
    for(size_t i = 0; i < 4; i++){
        sign[8+i] = (double) descriptor[1][i] / (double) (numFeatures-1);
    }
    //Values for Cb
    for(size_t i = 0; i < 4; i++){
        sign[12+i] = (double) descriptor[2][i] / (double) (numFeatures-1);
    }

    for(size_t i = 0; i < channels; i++) {
        delete[] descriptor[i];
		blocks[i].clear();
    }
    blocks.clear();
    delete[] descriptor;

} //generate signature

/**
* Performs the division of then image into blocks e calculates the color value representative of each block already calculating
* the normalized pixel value.
*
* @param channels The number of image channels
* @param dimension The dimension of square matrix in which the image will be divided
* @param normalizer The normalization factor
* @param image The imagem that will be divided
* @param[out] blocks The datas obtain of image division and the calculating of the colors representative for each
* @return The resulting ExtReturnCode.
*/
template<class SignatureType, class DataObjectType >
std::vector<vector<int32_t> > ColorLayoutExtractor<SignatureType, DataObjectType>::imageSplitter(const u_int16_t channels,
        const u_int16_t dimension,const DataObjectType &image, std::vector<std::vector<int32_t> > blocks)
        throw (std::runtime_error) {

    //matrix that will store the sum of the values of the pixels of each image color channel
    int32_t **sum;
    //pixel counter of each block
    int32_t *count;

    //The number of blocks is setted in the constant ColorLayoutExtractor::BLOCKS
    try {
        sum = new int32_t*[channels];
        for(size_t i = 0; i < channels; i++)
            sum[i] = new int32_t[getNumBlocks()];
        count = new int32_t[getNumBlocks()];
    } catch(...) {
        throw std::runtime_error("Cannot splitt image on Color Layout");
    }

	//Fill matrix with 0
    for(size_t x = 0; x < getNumBlocks(); x++) {
        count[x] = 0;
        for(size_t y = 0; y < channels; y++)
            sum[y][x] = 0;
    }   

    //Part 1: image division into ColorLayoutExtractor::BLOCKS blocks
    int32_t y_axis, x_axis, blockPosition;
    for(size_t y = 0; y < image.getHeight(); y++){
        for(size_t x = 0; x < image.getWidth(); x++){
            //calculation of pixel index in the block
            y_axis = static_cast<int32_t>((y*dimension)/(image.getHeight()));
            x_axis = static_cast<int32_t>((x*dimension)/(image.getWidth()));
			
            //pixel index in the block
            blockPosition = y_axis*dimension + x_axis;

            YCrCbColorSystem conversion;
            //RGB -> YCrCb conversion / calculation of pixels sum of each block
            sum[0][blockPosition] += static_cast<int32_t> (conversion.getYOfYCrCb(image.getPixel(x,y))); //Y
            sum[1][blockPosition] += static_cast<int32_t> (conversion.getCrOfYCrCb(image.getPixel(x,y))); //Cr
            sum[2][blockPosition] += static_cast<int32_t> (conversion.getCbOfYCrCb(image.getPixel(x,y))); //Cb

            //pixel counter
            count[blockPosition]++;
        }
    }

    //Part 2: For each image color channel choose the representative color of each block (average color)
    blocks = averageRepresentativeColorDefault(channels, dimension, count, sum, blocks);

    delete[] count;
    for(size_t i = 0; i < channels; i++)
        delete[] sum[i];
    delete[] sum;
	
    return blocks;

} //imageSplitter


/**
* Calculates the color value representative of each block using the default average
*
* @param channels The number of image channels
* @param dimension The dimension of square matrix in which the image will be divided
* @param count Pixel counter for each block
* @param sum Matrix that store the sum of the values of the pixels of each image color channel
* @param[out] blocks The datas obtain calculating the representative color for each block
* @return The resulting ExtReturnCode.
*/
template<class SignatureType, class DataObjectType >
std::vector<vector<int32_t> > ColorLayoutExtractor<SignatureType, DataObjectType>::averageRepresentativeColorDefault(const u_int16_t channels,
        const u_int16_t dimension, int32_t *count,  int32_t **sum, std::vector<std::vector<int32_t> > blocks){
		
    for(size_t x = 0; x < dimension; x++) {
        for(size_t y = 0; y < dimension; y++) {
            for(size_t k = 0; k < channels; k++) {
                if(count[x*dimension + y] != 0)
                    blocks[k][x*dimension + y] = sum[k][x*dimension + y]/count[x*dimension + y];
                else
                    blocks[k][x*dimension + y] = 0;
            }
        }
    }
	
	return blocks;
	
}


/**
* Performs the quantization of the 0 block of the Y color channel.
* Executa a quantização do bloco 0 do canal de cor Y
*
* @param block_value The input value of the block
*                  O valor de entrada do bloco
* @param[out] The input value of the block after quantization
*              O valor de entrada do bloco após a quantização
* @return The resulting ExtReturnCode.
*/
template<class SignatureType, class DataObjectType >
void ColorLayoutExtractor<SignatureType, DataObjectType>::quantifyYChannel(
        const int32_t block_value, int32_t &new_value) {

    if(block_value > 191)
        new_value = 112 + (block_value - 192)/4;
    else if(block_value > 159)
        new_value = 96 + (block_value - 160)/2;
    else if(block_value > 95)
        new_value = 32 + (block_value - 96);
    else if(block_value > 63)
        new_value = 16 + (block_value - 64)/2;
    else
        new_value = block_value/4;

} //quantifyYChannel

/**
* Performs the quantization of the 0 block of the C color channels (Cr and Cb).
* Executa a quantização do bloco 0 dos canais de cor C (Cr e Cb)
*
* @param block_value The input value of the block
*                  O valor de entrada do bloco
* @param[out] The input value of the block after quantization
*              O valor de entrada do bloco após a quantização
* @return The resulting ExtReturnCode.
*/
template<class SignatureType, class DataObjectType >
void ColorLayoutExtractor<SignatureType, DataObjectType>::quantifyCChannel(
        const int32_t block_value, int32_t &new_value) {

    if(block_value > 191)
        new_value = 63;
    else if(block_value > 159)
        new_value = 56 + (block_value - 160)/4;
    else if(block_value > 143)
        new_value = 48 + (block_value - 144)/2;
    else if(block_value > 111)
        new_value = 16 + (block_value - 112);
    else if(block_value > 95)
        new_value = 8 + (block_value - 96)/2;
    else if(block_value > 63)
        new_value = (block_value - 64)/4;
    else
        new_value = 0;

} //quantifyCChannel

/**
* Performs the quantization of the others blocks.
* Executa a quantização dos demais blocos
*
* @param block_value The input value of the block
*                  O valor de entrada do bloco
* @param[out] The input value of the block after quantization
*              O valor de entrada do bloco após a quantização
* @return The resulting ExtReturnCode.
*/
template<class SignatureType, class DataObjectType >
void ColorLayoutExtractor<SignatureType, DataObjectType>::quantify(const int32_t block_value,
        int32_t &new_value) {

    int temp = block_value;
    if(temp > 239)
        temp = 239;
    if(temp < -256)
        temp = -256;
    if ((abs(temp)) > 127)
        new_value = 64 + (abs(temp))/4;
    else if ((abs(temp)) > 63)
        new_value = 32 + (abs(temp))/2;
    else
        new_value = abs(temp);
    new_value = (temp < 0 ) ? -(new_value) : new_value;
    new_value += 132;

} //quantify

/**
* Calculate the scanning pattern utilized in the zigzag algorithm for a given the dimension. In other words, calculate the order
* of visitation of the image blocks in the execution of the zigzag algorithm. Code retired from: http://rosettacode.org/wiki/Zig_Zag.
*
* @param dimension The dimension of the matrix that represents the image divided.
* @param[out] zigzag The array that contains the visitation pattern of the zigzag algorithm.
*
*/
template<class SignatureType, class DataObjectType >
void ColorLayoutExtractor<SignatureType, DataObjectType>::calculateZigZagPattern(
        const u_int16_t dimension, int32_t *zigzag) {

    u_int16_t lastValue = dimension * dimension - 1;
    int currNum = 0, currDiag = 0, loopFrom, loopTo, row, col, count = 0;

    do {
        if (currDiag < dimension) {
            loopFrom = 0;
            loopTo = currDiag;
        }
        else {
            loopFrom = currDiag - dimension + 1;
            loopTo = dimension - 1;
        }

        for (size_t i = loopFrom; i <= loopTo; i++) {
            if ( currDiag % 2 == 0 ) {
                row = loopTo - i + loopFrom;
                col = i;
            }
            else {
                row = i;
                col = loopTo - i + loopFrom;
            }
            zigzag[currNum++] = row*dimension + col;
        }
        count++;
        currDiag++;
    }
    while (currNum <= lastValue);

} //calculateZigZagPattern

/**
 * Performs the complete process of the quantization
 * Executa o processo completo de quantização
 *
 * @param dimension The dimension of the matrix that represents the image divided.
 *                 A dimensão da matriz que representa a imagem dividida
 * @param blocks The values to be quantized of the all blocks
 *                  Os valores a serem quantizados de todos os blocos
 * @param[out] descriptor The values of all blocks after the quantization
 *                  Os valores de todos os blocos após a quantização
 * @return The resulting ExtReturnCode.
 */
template<class SignatureType, class DataObjectType >
void ColorLayoutExtractor<SignatureType, DataObjectType>::sumQuantify(const u_int16_t dimension,
        std::vector<std::vector<int32_t> > blocks, int32_t **descriptor)
        throw (std::runtime_error){

    //scanning zigzag pattern
    int32_t *zigzag;
    try {
        zigzag = new int32_t[getNumBlocks()];
    } catch(...) {
        std::runtime_error("Cannot allocate the sum on Color Layout");
    }

    calculateZigZagPattern(dimension, zigzag);

    quantifyYChannel(blocks[0][0]/dimension, descriptor[0][0]);
    quantifyCChannel(blocks[1][0]/dimension, descriptor[1][0]);
    quantifyCChannel(blocks[2][0]/dimension, descriptor[2][0]);
    for(size_t i = 1; i < getNumBlocks(); i++) {
        quantify(blocks[0][zigzag[i]]/2, descriptor[0][i]);
        quantify(blocks[1][zigzag[i]], descriptor[1][i]);
        quantify(blocks[2][zigzag[i]], descriptor[2][i]);
    }

    delete[] zigzag;

} //sumQuantify
