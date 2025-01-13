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
#include <artemis/extractor/DiscreteCosineTransformation.h> 


/**
* Sets the dimension.
*
* @param value The dimension value.
*/
void DiscreteCosineTransformation::setDimension(u_int8_t value){

    dimension = value;
}

/**
* Gets the dimension.
*
* @return The DCT dimension.
*/
u_int8_t DiscreteCosineTransformation::getDimension(){

    return dimension;
}

/**
* Constructor.
*
* @param dim The DCT dimension.
*/
DiscreteCosineTransformation::DiscreteCosineTransformation(u_int8_t dim){

    setDimension(dim);
}

/**
* Destructor.
*/
DiscreteCosineTransformation::~DiscreteCosineTransformation() {
}

/**
* Apply DCT Transformation
*
* @param blocksValues A vector that have dimension*dimension positions to store the DCT transformation for only ONE channel.
* @param[out] blocks The transformation result for each block.
*/
std::vector<int32_t> DiscreteCosineTransformation::applyDCT(std::vector<int32_t> blocksValues) throw (std::range_error, std::runtime_error){
	
	//compare if the size of vector is equal to the dimension
    if (blocksValues.size() != dimension*dimension){
        throw std::range_error("The informed dimension is invalid for the number of blocks.");
    }


    //Would contain the calculation values of the c matrix
    double **c;
    u_int8_t dimension = getDimension();
    try {
        c = new double*[dimension];
        for(size_t i = 0; i < dimension; i++)
            c[i] = new double[dimension];
    } catch (...) {
        throw std::runtime_error("Cannot alloc mem for the matrix on discrete transformation.");
    }


    double v;
    //calculation of the c matrix
    for(size_t x = 0; x < dimension; x++) {
        if (x == 0) {
            v = sqrt(0.125);
        } else {
            v = 0.5;
        } for (size_t y = 0; y < dimension; y++) {
            c[x][y] = v*cos((M_PI/dimension)*x*(y + 0.5));
        }
    }


    double s = 0.0;
    double *temp;

    try {
        temp = new double[blocksValues.size()];//Size of total blocks        
    } catch(...) {
        throw std::runtime_error("Cannot alloc mem for the vector discrete transformation.");
    }

    for (size_t x = 0; x < dimension; x++) {
        for (size_t y = 0; y < dimension; y++) {
            s = 0.0;
            for (size_t k = 0; k < dimension; k++)
                s += c[y][k] * blocksValues[dimension*x + k];
            temp[dimension*x + y] = s;
        }
    }
    for (size_t y = 0; y < dimension; y++){
        for (size_t x = 0; x < dimension; x++){
            s = 0.0;
            for (size_t k = 0; k < dimension; k++)
                s += c[x][k] * temp[dimension*k + y];
            blocksValues[dimension*x + y] = static_cast<int32_t>(floor(s + 0.499999));
        }
    }

    delete[] temp;

    for(size_t i = 0; i < dimension; i++)
        delete[] c[i];
    delete[] c;
	
	return blocksValues;
}
