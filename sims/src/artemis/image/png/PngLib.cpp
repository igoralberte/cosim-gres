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
#include <artemis/image/png/PngLib.h>

/**
* Load the PNG Image in the Pixel Matrix.
* A new matrix of Pixels are allocated and the
* PNG File are loaded in.
* NOTE: OpenCv use.
*
* @exception FullHeapException Insufficient space for memory allocation.
*/
void PNGImage::loadPixelMatrix() throw (std::runtime_error){


    //Structures of OpenCv
    CvScalar s;
    IplImage *image;

    image = cvLoadImage(getFilename().c_str(), -1);

    deletePixelMatrix();
    try{
        createPixelMatrix( (u_int32_t) image->width, (u_int32_t) image->height);
    } catch (...) {
        throw std::runtime_error("Cannot create pixel matrix on png image");
    }


    //Converts the IpLImage OpenCVStructure to Pixel Matrix
    for(size_t x = 0; x < getWidth(); x++){
      for(size_t y = 0; y < getHeight(); y++){
          //Catch the RGB from opened Image and tranfer to the matrix of pixels

          //s = cvGet2D(image, y, x);
          Pixel aux;
          /*aux.setRedPixelValue((const char) s.val[2]);
          aux.setGreenPixelValue((const char) s.val[1]);
          aux.setBluePixelValue((const char) s.val[0]);
          aux.setAlphaPixelValue((const char) s.val[3]);*/

          aux.setRedPixelValue(((uchar *)(image->imageData + y*image->widthStep))[x*image->nChannels +2]);
          aux.setGreenPixelValue(((uchar *)(image->imageData + y*image->widthStep))[x*image->nChannels +1]);
          aux.setBluePixelValue(((uchar *)(image->imageData + y*image->widthStep))[x*image->nChannels +0]);

          setPixel(x, y, aux);
      }
    }

    //Destroy the OPenCv structure
    cvReleaseImage(&image);
}

/**
* Empty constructor of PNGImage Class.
* It's advised when you want to create
* a new PNG File.
* NOTE: For default all PNG Files are opened
* in colored mode with Alpha Transparency
* and 8 bits per pixel.
*/
PNGImage::PNGImage(){

    setFilename("NO_FILE");
    setChannels(0);
    setImageID(0);

    setBitsPerPixel(1);
}

/**
* Constructor of PNGImage Class.
* It's advised when you want to open
* a PNG File.
* NOTE: For default all PNG Files are opened
* in colored mode with 8 bits per pixel.
*
* @param filename: The name of the file.
*/
PNGImage::PNGImage(std::string filename){

    openImage(filename);
    setBitsPerPixel(8);
}

PNGImage::~PNGImage(){

}

/**
* Open the PNG images.
* If file exists, the image are loaded in the pixel matrix.
*
* @param filename: Name of the Image to open.
* @see Pixel.
*/
void PNGImage::openImage(std::string filename) throw (std::runtime_error){

    IplImage *image;

    setFilename(filename);

    image = cvLoadImage(this->getFilename().c_str(), -1);
    if (!image)
       throw std::runtime_error("The png file cannot be open.");


    setImageID(0);

    //PNG has alpha transparency
    setChannels(image->nChannels);
    //Properties of the Object
    //Transfer RGB from IplImage to Matrix of Pixels
    //The Matrix of Pixels can be accessed now
    //Full Load of the jpg image
    //This function load the JPEG image by OpenCV
    try{
        loadPixelMatrix();
    }catch(...){
        throw std::runtime_error("Cannot create pixel matrix on png image");
    }
    cvReleaseImage(&image);
}


/**
* Save the current PNG into a file.
*
* @param filename The name of the file.
*/
void PNGImage::saveToFile(std::string filename) throw (std::runtime_error){

    CvScalar s;
    IplImage *image = cvCreateImage(cvSize(getWidth(), getHeight()), IPL_DEPTH_8U, 0);

    u_int8_t div;
    if ((getBitsPerPixel() > 8) && (getBitsPerPixel() <= 16))
        div = 16;
    else
        div = 1;

    for(size_t x = 0; x < getWidth(); x++){
        for(size_t y = 0; y < getHeight(); y++){
            s.val[0] = (getPixel(x, y).getGrayPixelValue()/div);
            cvSet2D(image, y, x, s);
        }
    }

    filename.append(".png");

    if (!cvSaveImage(filename.c_str(), image)){
        throw std::runtime_error("The png file cannot be created");
    }
}

/**
* Clones a current Image.
* If, for any reason the Image cannot be copied,
* then NULL is returned.
*/
PNGImage* PNGImage::clone(){

      return new PNGImage(*this);
}
