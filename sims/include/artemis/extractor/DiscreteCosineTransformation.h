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
 * This file defines a Class to DCT Transformation
 *
 * @version 1.0
 */
#ifndef DCTTRANSFORMATION_H
#define DCTTRANSFORMATION_H

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <stdexcept>
#include <cmath>

class DiscreteCosineTransformation {

    private:
        u_int8_t dimension;

	public:
        DiscreteCosineTransformation(u_int8_t dim = 8);
        virtual ~DiscreteCosineTransformation();

        void setDimension(u_int8_t value);
        u_int8_t getDimension();

        std::vector<int32_t> applyDCT(std::vector<int32_t> blocksValues) throw (std::range_error, std::runtime_error);
};

#endif
