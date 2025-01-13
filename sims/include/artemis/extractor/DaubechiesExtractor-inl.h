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
*Constructor.
*/
template< class SignatureType, class DataObjectType >
DaubechiesExtractor<SignatureType, DataObjectType>::DaubechiesExtractor() {
}

/**
*Destructor.
*/
template< class SignatureType, class DataObjectType >
DaubechiesExtractor<SignatureType, DataObjectType>::~DaubechiesExtractor() {
}

template< class SignatureType, class DataObjectType >
void DaubechiesExtractor<SignatureType, DataObjectType>::generateSignature(const Image & img, SignatureType & sign)  throw (std::runtime_error){

    Image* image = img.clone();
    Image* aux = image->clone();
    Image* aux2 = image->clone();

    size_t widthFull, heightFull;
    size_t width = image->getWidth()-1;
    size_t height = image->getHeight()-1;

    for (u_int16_t level = 0; level < this->getLevels(); level++) {
        widthFull = width;
        heightFull = height;
        width = (size_t) width / 2;
        height = (size_t) height / 2;

        Pixel *p = new Pixel();
                //
        // Makes the the addition or subtration in the x axis.
        // Use the Daubechies theory to generate Pixel Gray Scale Value
        //
        for (size_t y = 0; y < heightFull; y++) {
            p = new Pixel();
            p->setGrayPixelValue((image->getPixel(widthFull - 2, y).getGrayPixelValue()*(1 + sqrt(3)) +
                                  image->getPixel(widthFull - 1, y).getGrayPixelValue()*(3 + sqrt(3)) +
                                  image->getPixel(0, y).getGrayPixelValue()*(3 - sqrt(3)) +
                                  image->getPixel(1, y).getGrayPixelValue()*(1 - sqrt(3))) * sqrt(2) / 8, 256);
            aux->setPixel(0, y, (*p));
            delete(p);

            p = new Pixel();
            p->setGrayPixelValue((image->getPixel(widthFull - 2, y).getGrayPixelValue()*(1 - sqrt(3)) +
                                  image->getPixel(widthFull - 1, y).getGrayPixelValue()*(sqrt(3) - 3) +
                                  image->getPixel(0, y).getGrayPixelValue()*(3 + sqrt(3)) -
                                  image->getPixel(1, y).getGrayPixelValue()*(1 + sqrt(3))) * sqrt(2) / 8, 256);
            aux->setPixel(width, y, (*p));
            delete(p);

            for (size_t x = 2, low = 1, high = width + 1; x < widthFull;
                 x = x + 2, low++, high++) {
                if (x != (widthFull - 1)) {

                    p = new Pixel();
                    p->setGrayPixelValue(((image->getPixel(x - 2, y).getGrayPixelValue()*(1 + sqrt(3)) +
                                           image->getPixel(x - 1, y).getGrayPixelValue()*(3 + sqrt(3)) +
                                           image->getPixel(x, y).getGrayPixelValue()*(3 - sqrt(3)) +
                                           image->getPixel(x + 1, y).getGrayPixelValue()*(1 - sqrt(3))) * sqrt(2) / 8), 256);
                    aux->setPixel(low, y, (*p));
                    delete(p);

                    p = new Pixel();
                    p->setGrayPixelValue(((image->getPixel(x - 2, y).getGrayPixelValue()*(1 - sqrt(3)) +
                                           image->getPixel(x - 1, y).getGrayPixelValue()*(sqrt(3) - 3) +
                                           image->getPixel(x, y).getGrayPixelValue()*(3 + sqrt(3)) -
                                           image->getPixel(x + 1, y).getGrayPixelValue()*(1 + sqrt(3))) * sqrt(2) / (8)), 256);
                    aux->setPixel(high, y, (*p));
                    delete(p);

                } else {

                    p = new Pixel();
                    p->setGrayPixelValue(((image->getPixel(x - 2, y).getGrayPixelValue()*(1 - sqrt(3)) +
                                           image->getPixel(x - 1, y).getGrayPixelValue()*(sqrt(3) - 3) +
                                           image->getPixel(x, y).getGrayPixelValue()*(3 + sqrt(3)) -
                                           image->getPixel(0, y).getGrayPixelValue()*(1 + sqrt(3))) * sqrt(2) / (8)), 256);
                    aux->setPixel(high, y, (*p));
                    delete(p);
                }
            }
        }

        for (size_t x = 0; x < widthFull; x++) {

            p = new Pixel();
            p->setGrayPixelValue(((aux->getPixel(x, heightFull - 2).getGrayPixelValue()*(1 + sqrt(3)) +
                                   aux->getPixel(x, heightFull - 1).getGrayPixelValue()*(3 + sqrt(3)) +
                                   aux->getPixel(x, 0).getGrayPixelValue()*(3 - sqrt(3)) +
                                   aux->getPixel(x, 1).getGrayPixelValue()*(1 - sqrt(3))) * sqrt(2) / 8), 256);
            image->setPixel(x, 0, (*p));
            delete(p);


            p = new Pixel();
            p->setGrayPixelValue(((aux->getPixel(x, heightFull - 2).getGrayPixelValue()*(1 - sqrt(3)) +
                                   aux->getPixel(x, heightFull - 1).getGrayPixelValue()*(sqrt(3) - 3) +
                                   aux->getPixel(x, 0).getGrayPixelValue()*(3 + sqrt(3)) -
                                   aux->getPixel(x, 1).getGrayPixelValue()*(1 + sqrt(3))) * sqrt(2) / 8), 256);
            image->setPixel(x, height, (*p));
            delete(p);

            for (size_t y = 2, low = 1, high = height + 1; y < heightFull; y = y + 2, low++, high++) {
                if (y != (heightFull - 1)) {

                    p = new Pixel();
                    p->setGrayPixelValue(((aux->getPixel(x, y - 2).getGrayPixelValue()*(1 + sqrt(3)) +
                                           aux->getPixel(x, y - 1).getGrayPixelValue()*(3 + sqrt(3)) +
                                           aux->getPixel(x, y).getGrayPixelValue()*(3 - sqrt(3)) +
                                           aux->getPixel(x, y + 1).getGrayPixelValue()*(1 - sqrt(3))) * sqrt(2) / 8), 256);
                    image->setPixel(x, low, (*p));
                    delete(p);

                    p = new Pixel();
                    p->setGrayPixelValue(((aux->getPixel(x, y - 2).getGrayPixelValue()*(1 - sqrt(3)) +
                                           aux->getPixel(x, y - 1).getGrayPixelValue()*(sqrt(3) - 3) +
                                           aux->getPixel(x, y).getGrayPixelValue()*(3 + sqrt(3)) -
                                           aux->getPixel(x, y + 1).getGrayPixelValue()*(1 + sqrt(3))) * sqrt(2) / 8), 256);
                    image->setPixel(x, high, (*p));
                    delete(p);

                } else {

                    p = new Pixel();
                    p->setGrayPixelValue(((aux->getPixel(x, y - 2).getGrayPixelValue()*(1 - sqrt(3)) +
                                           aux->getPixel(x, y - 1).getGrayPixelValue()*(sqrt(3) - 3) +
                                           aux->getPixel(x, y).getGrayPixelValue()*(3 + sqrt(3)) -
                                           aux->getPixel(x, 0).getGrayPixelValue()*(1 + sqrt(3))) * sqrt(2) / 8), 256);
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
