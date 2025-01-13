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
* Constructor.
*/
template< class SignatureType, class DataObjectType >
HaarExtractor<SignatureType, DataObjectType>::HaarExtractor() {
}

/**
* Destructor.
*/
template< class SignatureType, class DataObjectType >
HaarExtractor<SignatureType, DataObjectType>::~HaarExtractor() {
}

/**
* Generates the Haar Wavelets descriptor defined in the template from the
* image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template< class SignatureType, class DataObjectType >
void HaarExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType & img, SignatureType & sign) throw (std::runtime_error){


    DataObjectType* image = img.clone();
    DataObjectType* aux = image->clone();
    DataObjectType* aux2 = image->clone();

    size_t widthFull, heightFull;
    size_t width = image->getWidth()-1;
    size_t height = image->getHeight()-1;

    Pixel *p;
    for (u_int16_t level = 0; level < this->getLevels(); level++) {

        widthFull = width;
        heightFull = height;
        width = (size_t) width / 2;
        height = (size_t) height / 2;

        //
        // Makes the the addition or subtration in the y axis.
        // Use the Haar theory to generate Pixel Gray Scale Value
        //
        for (size_t y = 0; y < heightFull; y++) {
            for (size_t x = 0, low = 0, high = width; x < widthFull; x = x + 2, low++, high++) {
                if (x != (widthFull - 1)) {
                    p = new Pixel();
                    p->setGrayPixelValue(((image->getPixel(x, y).getGrayPixelValue() +
                                           image->getPixel(x + 1, y).getGrayPixelValue()) * sqrt(2) / 2), 256);
                    aux->setPixel(low, y, (*p));
                    delete(p);

                    p = new Pixel();
                    p->setGrayPixelValue((image->getPixel(x, y).getGrayPixelValue() -
                                          image->getPixel(x + 1, y).getGrayPixelValue()) * sqrt(2) / 2, 256);
                    aux->setPixel(high, y, (*p));
                    delete(p);
                } else {
                    p = new Pixel;
                    p->setGrayPixelValue((image->getPixel(x, y).getGrayPixelValue() -
                                          image->getPixel(x, y).getGrayPixelValue()) * sqrt(2) / 2, 256);
                    aux->setPixel(high, y, (*p));
                    delete(p);
                }
            }
        }

        //
        // Makes the the addition or subtration in the x axis.
        // Use the Haar theory to generate Pixel Gray Scale Value
        //
        for (size_t x = 0; x < widthFull; x++) {
            for (size_t y = 0, low = 0, high = height; y < heightFull; y = y + 2, low++, high++) {
                if (y != (heightFull - 1)) {
                    p = new Pixel();
                    p->setGrayPixelValue(((aux->getPixel(x, y).getGrayPixelValue() +
                                           aux->getPixel(x, y + 1).getGrayPixelValue()) * sqrt(2) / 2), 256);
                    aux2->setPixel(x, low, (*p));
                    delete(p);

                    p = new Pixel();
                    p->setGrayPixelValue(((aux->getPixel(x, y).getGrayPixelValue() -
                                           aux->getPixel(x, y + 1).getGrayPixelValue()) * sqrt(2) / 2), 256);
                    aux2->setPixel(x, high, (*p));
                    delete(p);

                } else {
                    p = new Pixel();
                    p->setGrayPixelValue(((aux->getPixel(x, y).getGrayPixelValue() - aux->getPixel(x, y).getGrayPixelValue()) * sqrt(2) / 2), 256);
                    image->setPixel(x, high, (*p));
                    delete(p);

                }
            }
        }
    }

    if (aux != NULL)
        delete(aux);
    if (aux2 != NULL)
        delete(aux2);
    sign = this->extractSignature(image);

    if (image != NULL)
        delete (image);
}


