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
#include <artemis/image/krl/KrlLib.h>

/**
* Empty Constructor.
* It's advised when you want to create
* a new KRL File.
* NOTE: For default all KRL Files are opened
* in gray scale mode with 12 bits per pixel.
*/
KRLImage::KRLImage(){
                
    setFilename("NO_FILE");
    setChannels(0);
    setImageID(0);
    setBitsPerPixel(12);
}

/**
* Constructor.
*
* @param filename The KRL file what will be opened.
*/
KRLImage::KRLImage(std::string filename){

    setFilename(filename);
    openImage(filename);
}

/**
* Destructor.
*/
KRLImage::~KRLImage(){

}

/**
* Open a KRL file.
*
* @param fileName The name of the KRL file what will be opened.
*/
void KRLImage::openImage(std::string filename) throw (std::runtime_error){
    
    KrlHeaderType header;
     
    try{
        FileHandler * img = new FileHandler(filename, img->FILE_BINARY);
        
        //==================== CABEcALHO DA IMAGEM ================================
        
        header.headerType = *((u_int16_t *)img->readFile(2));
        img->endianSwap(header.headerType);
    
        for(int i=0; i<32; i++)
            header.filename[i] = *(img->readFile(1));
    
    
        header.width = *((u_int16_t *) img->readFile(2));
        img->endianSwap(header.width);
    
        header.height = *((u_int16_t *) img->readFile(2));
        img->endianSwap(header.height);
     
        header.pixBytes = *(img->readFile(1));
        header.pixBits  = *(img->readFile(1));
        header.OS       = *(img->readFile(1));
        header.compress = *(img->readFile(1));

        for(int i=0;i<5;i++){     
           header.compParas[i] = *((u_int16_t *) img->readFile(2));
           img->endianSwap(header.compParas[i]);
        }
    
    for(int i=0; i<32; i++)
         header.hdtime[i] = *(img->readFile(1));
     
     header.imgType = *(img->readFile(1));
     header.black   = *(img->readFile(1));
     
     header.pixSize = *((u_int16_t*) img->readFile(2));
     img->endianSwap(header.pixSize);
     
     header.sampType = *((u_int16_t*)img->readFile(2));
     img->endianSwap(header.sampType);
     
     header.optDensity = *(img->readFile(1));
    
     for(int i=0; i<13; i++)
         header.dayNo[i] =  *(img->readFile(1));
     
     header.medHistNo = *((long*) img->readFile(4));
     
     for(int i=0; i<10; i++)
         header.studyType[i]   = *(img->readFile(1));
     for(int i=0; i<50; i++)
         header.patientName[i] = *(img->readFile(1));
     for(int i=0; i<216; i++)
         header.special[i]     = *(img->readFile(1));
     for(int i=0; i<128; i++)
         header.comment[i]     = *(img->readFile(1));   
     
     //========================= FIM CABEcALHO =================================
     //======================== CORPO DA IMAGEM ================================ 
   
     u_int16_t tmpValue;
     
     deletePixelMatrix();
     try{
        createPixelMatrix( (u_int32_t) header.width, (u_int32_t) header.height);
     } catch (...) {
        throw std::runtime_error("Cannot create pixel matrix on krl image.");
     }
     
     for(int x=0; x< header.width; x++){ 
         for(int y=0; y<header.height; y++){
             tmpValue = *((u_int16_t *) img->readFile(2));
             img->endianSwap(tmpValue);   
             Pixel aux;
             aux.setGrayPixelValue((float)tmpValue,pow((double)2,(int)header.pixBits));
             setPixel(x,y,aux);
         }
     }
    setFilename(filename);
    setImageID(2);
    setChannels(0);
    setBitsPerPixel(header.pixBits - 0);
         
     } catch (...) {
        throw std::runtime_error("Cannot open krl file");
    }
}

/**
* Clones a current Image.
* If, for any reason the Image cannot be copied,
* then NULL is returned.
*/
KRLImage* KRLImage::clone(){

    return new KRLImage(*this);
}
