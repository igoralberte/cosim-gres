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
* This file defines a Class to manipulate bitmaps images. 
* All methods required by ImageBase are implemented.
*
* @version 1.0
*/

#ifndef BMPLIB_H
#define BMPLIB_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <artemis/image/ImageBase.h>

/**
*
* This Class provide resources to manipulate Bitmap Images.
* All methods required by ImageBase are implemented.
* This class can be used with the Extractors defined in the directory 
* Extractor.
*
*
* @brief Handler Class to BMP Image
* @author 006
* @see ImageBase
* @version 1.0
*/
class BMPImage : public Image{

    private:
        void loadPixelMatrix() throw (std::runtime_error);

    public:
        BMPImage();
        BMPImage(std::string filename);
        virtual ~BMPImage();

        void openImage(std::string filename) throw (std::runtime_error);
             
        void saveToFile(std::string filename) throw (std::runtime_error);
        
        BMPImage* clone();
};

#endif
