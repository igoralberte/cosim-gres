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
 * This file defines the basic interfaces for feature extractors.
 *
 * @version 1.0
 */
#ifndef EXTRACTOR_h
#define EXTRACTOR_h

#include <iostream>
#include <cstdlib>
#include <stdexcept>

/**
*
* This class is the basic interface for feature extractors. It depends on
* datatypes for handling source data (e.g. Image) and datatypes for
* storing the extracted signature (e.g. stBasicArrayObject).
*
* @brief Base class for any extractor.
* @author 004
* @author 005
* @author 006
* @version 1.0
*/
template< class SignatureType, class DataObjectType >
class Extractor {
    public:

        /**
        * Builds a new uninitialized instance of this class.
        *
        * @note This constructor is required to fullfil this interface.
        */
        Extractor() {
        }

        /**
        * Class destructor.
        */
        virtual ~Extractor() {
        }

        /**
        * This is a pure virtual method to trigger the signature generation of the
        * feature extractors in the library.
        *
        * @param dataObj The data object to be processed.
        * @param[out] sign The object to store the generated signature.
        */
        virtual void generateSignature(const DataObjectType & dataObj, SignatureType & sign) = 0;
};

#endif

