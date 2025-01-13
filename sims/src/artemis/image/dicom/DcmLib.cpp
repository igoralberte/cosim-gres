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
 * This file contains the implementation of the DCMImage class.
 */

#include <artemis/image/dicom/DcmLib.h>

//------------------------------------------------------------------------------

DCMImage::DCMImage() {
    dcmFileFormat = NULL;
}

//------------------------------------------------------------------------------

DCMImage::DCMImage(std::string filename) {
    dcmFileFormat = NULL;
    openImage(filename);
}

//------------------------------------------------------------------------------

DCMImage::DCMImage(unsigned char *buffer, int length) {
    dcmFileFormat = NULL;
    openImage(buffer, length);
}

//------------------------------------------------------------------------------

DCMImage::DCMImage(DCMImage & i) : Image(i) {
    dcmFileFormat = new DcmFileFormat();
    *(this->dcmFileFormat) = *(i.dcmFileFormat);
}

//------------------------------------------------------------------------------

DCMImage::~DCMImage() {

    if (dcmFileFormat != NULL) {
        delete dcmFileFormat;
        dcmFileFormat = NULL;
    }
}
//------------------------------------------------------------------------------

void DCMImage::openImage(std::string filename) {

    E_TransferSyntax xfer;
    DicomImage * dcmImage = 0;
    EI_Status status;

    // Register codecs to support compressed JPEG DICOM images
    DJDecoderRegistration::registerCodecs();

    dcmFileFormat = new DcmFileFormat();
    dcmFileFormat->loadFile(filename.c_str(), EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength, ERM_autoDetect);

    xfer = dcmFileFormat->getDataset()->getOriginalXfer();
    dcmImage = new DicomImage(dcmFileFormat, xfer);

    // Verify conditions of DICOM
    status = dcmImage->getStatus();
    if (status != EIS_Normal) {
        delete dcmImage;
        dcmImage = NULL;
        delete dcmFileFormat;
        dcmFileFormat = NULL;
        DJDecoderRegistration::cleanup(); //unregister the support for compressed JPEG images
        stringstream ss;
        ss << status;
        throw dicom_error("Error reading DICOM image from file \"" + filename + "\". DicomImage status: " + ss.str());
    }

    setBitsPerPixel((int) dcmImage->getDepth());
    setFilename(filename);
    /*
     * @todo: Verify if the values below should be always these constants.
     */
    setChannels(3);
    setImageID(5);

    createPixelMatrix(dcmImage->getWidth(), dcmImage->getHeight());

    // Is the image color or monochrome?
    if (dcmImage->isMonochrome()) {

        dcmImage->setMinMaxWindow();
        Uint16 *pixelData = (Uint16 *) (dcmImage->getOutputData(getBitsPerPixel()));
        unsigned short grayLevels = pow(2, getBitsPerPixel());

        //
        //Recover gray scaled pixels
        //
        for (int x = 0; x < getWidth(); x++) {
            for (int y = 0; y < getHeight(); y++) {
                Pixel p;
                p.setGrayPixelValue(pixelData[x + (y * getHeight())], grayLevels);
                setPixel(x, y, p);
            }
        }
    } else {
        void * pDicomDibits;
        dcmImage->createWindowsDIB(pDicomDibits, 0, 0, 24, 0, 1);
        unsigned char * pd;
        pd = (unsigned char *) pDicomDibits;
        for (int y = 0; y < getHeight(); y++) {
            for (int x = 0; x < getWidth(); x++) {
                Pixel p((unsigned char) (*(pd + 3 * y * getWidth() + 3 * x + 2)),
                        (unsigned char) (*(pd + 3 * y * getWidth() + 3 * x + 1)),
                        (unsigned char) (*(pd + 3 * y * getWidth() + 3 * x)));
                setPixel(x, y, p);
            }
        }
    }

    delete dcmImage;
    dcmImage = NULL;
    DJDecoderRegistration::cleanup(); //unregister the support for compressed JPEG images
}

//------------------------------------------------------------------------------

void DCMImage::openImage(unsigned char *buffer, int length) {

    DcmInputBufferStream * dcmInBuffer = 0;
    E_TransferSyntax transferSyntax;
    DicomImage * dcmImage = 0;
    EI_Status status;

    // Register codecs to support compressed JPEG DICOM images
    DJDecoderRegistration::registerCodecs();

    // Set the dcmInputBuffer
    dcmInBuffer = new DcmInputBufferStream();
    dcmInBuffer->setBuffer((void *) buffer, (Uint32) length);
    dcmInBuffer->setEos();

    // Read the buffer and put it into the class member variable
    dcmFileFormat = new DcmFileFormat();
    dcmFileFormat->read(*dcmInBuffer, EXS_Unknown, EGL_withoutGL, DCM_MaxReadLength);

    // Open the image
    transferSyntax = dcmFileFormat->getDataset()->getOriginalXfer();
    dcmImage = new DicomImage(dcmFileFormat, transferSyntax, CIF_AcrNemaCompatibility);

    // Verify conditions of DICOM
    if (dcmImage->getStatus() != EIS_Normal) {
        dcmInBuffer->releaseBuffer();
        delete dcmImage;
        dcmImage = NULL;
        delete dcmFileFormat;
        dcmFileFormat = NULL;
        delete dcmInBuffer;
        dcmInBuffer = NULL;
        DJDecoderRegistration::cleanup(); //unregister the support for compressed JPEG images
        stringstream ss;
        ss << status;
        throw dicom_error("Error reading DICOM image from buffer. DicomImage status: " + ss.str());
    }

    // Set image properties
    setBitsPerPixel(dcmImage->getDepth());
    dcmImage->setMinMaxWindow();

    // Populate the generic Image pixel matrix
    uchar * pixelData = 0;
    float pixelValue;

    pixelData = (uchar *) dcmImage->getOutputData(getBitsPerPixel(), 0, 0);
    createPixelMatrix(dcmImage->getWidth(), dcmImage->getHeight());

    unsigned short grayLevels = pow(2, getBitsPerPixel());

    if (dcmImage->getDepth() <= 8) {
        /* read 1 byte per pixel */
        uchar pixelUChar;
        for (unsigned y = 0; y < getHeight(); y++) {
            for (unsigned x = getWidth(); x > 0; --x) {
                pixelUChar = *((uchar *) pixelData);
                pixelValue = (float) pixelUChar;
                Pixel pixel(pixelValue, grayLevels);
                setPixel((getWidth() - x), y, pixel);
                pixelData++;
            }
        }
    } else if (dcmImage->getDepth() < 16) {
        /* read 2 bytes per pixel */
        short pixelShort;
        for (unsigned y = 0; y < getHeight(); y++) {
            for (unsigned x = getWidth(); x > 0; --x) {
                pixelShort = *((short *) pixelData);
                pixelValue = (float) pixelShort;
                Pixel pixel(pixelValue, grayLevels);
                setPixel((getWidth() - x), y, pixel);
                pixelData += 2;
            }
        }
    } else if (dcmImage->getDepth() == 16) {
        /* read 2 bytes per pixel unsigned */
        unsigned short pixelUShort;
        for (unsigned y = 0; y < getHeight(); y++) {
            for (unsigned x = getWidth(); x > 0; --x) {
                pixelUShort = *((unsigned short *) pixelData);
                pixelValue = (float) pixelUShort;
                Pixel pixel(pixelValue, grayLevels);
                setPixel((getWidth() - x), y, pixel);
                pixelData += 2;
            }
        }
    } else {
        dcmInBuffer->releaseBuffer();
        delete dcmImage;
        dcmImage = NULL;
        delete dcmFileFormat;
        dcmFileFormat = NULL;
        delete dcmInBuffer;
        dcmInBuffer = NULL;
        DJDecoderRegistration::cleanup(); //unregister the support for compressed JPEG images
        throw dicom_error("Error reading DICOM image from buffer: image depth > 16 bits");
    }

    dcmInBuffer->releaseBuffer();
    delete dcmImage;
    dcmImage = NULL;
    delete dcmInBuffer;
    dcmInBuffer = NULL;
    DJDecoderRegistration::cleanup(); //unregister the support for compressed JPEG images
}

//------------------------------------------------------------------------------

void DCMImage::saveToFile(std::string filename) {

    if (!dcmFileFormat) {
        dcmFileFormat = new DcmFileFormat();
    }

    Uint16 *pixelData = new Uint16[getSize()];
    for (int x = 0; x < getWidth(); x++) {
        for (int y = 0; y < getHeight(); y++) {
            pixelData[x + (y * getHeight())] = getPixel(x, y).getGrayPixelValue();
        }
    }

    dcmFileFormat->getDataset()->putAndInsertUint16(DCM_Rows, (Uint16) getHeight());
    dcmFileFormat->getDataset()->putAndInsertUint16(DCM_Columns, (Uint16) getWidth());
    dcmFileFormat->getDataset()->putAndInsertUint16Array(DCM_PixelData, (const Uint16*) pixelData, getSize());

    OFCondition cond;
    cond = dcmFileFormat->saveFile(filename.c_str(), EXS_LittleEndianExplicit);
    if (cond.bad()) {
        stringstream ss;
        ss << cond.code();
        throw dicom_error("Error saving DICOM image to file \"" + filename + "\". DCMFileFormat.saveFile code: " + ss.str());
    }
}

//------------------------------------------------------------------------------

DCMImage * DCMImage::clone() {
    return new DCMImage(*this);
}

//------------------------------------------------------------------------------

void DCMImage::setStringTag(DCMImage::TagID tagId, std::string & tagValue) {
    /* DICOM tags have the form (gggg,eeee), thus, to compose a DcmTagKey:
     * tagId >> 16    : gets the first 16 bits (gggg) of tagId
     * tagId & 0xFFFF : gets the last 16 bits (eeee) of tagId
     */
    OFCondition cond;

    if (dcmFileFormat == NULL) {
        throw logic_error("DCMImage: Null DICOM header pointer.");
    }

    cond = dcmFileFormat->getDataset()->putAndInsertString(DcmTagKey(tagId >> 16, tagId & 0xFFFF), tagValue.c_str());
    if (cond.bad()) {
        stringstream ss;
        ss << hex << tagId << ". DcmDataset.putAndInsertString code: " << cond.code() << ".";
        throw dicom_error("Error writing DICOM tag 0x" + ss.str());
    }
}

//------------------------------------------------------------------------------

std::string DCMImage::getStringTag(DCMImage::TagID tagId) const {
    /* DICOM tags have the form (gggg,eeee), thus, to compose a DcmTagKey:
     * tagId >> 16    : gets the first 16 bits (gggg) of tagId
     * tagId & 0xFFFF : gets the last 16 bits (eeee) of tagId
     */
    const char * answer = 0;
    OFCondition cond;

    if (dcmFileFormat == NULL) {
        throw logic_error("DCMImage: Null DICOM header pointer.");
    }

    cond = dcmFileFormat->getDataset()->findAndGetString(DcmTagKey(tagId >> 16, tagId & 0xFFFF), answer);
    if (cond.bad()) {
        stringstream ss;
        ss << hex << tagId << ". DcmDataset.findAndGetString code: " << cond.code() << " (" << cond.text() << ").";
        throw dicom_error("Error reading DICOM tag 0x" + ss.str());
    }

    return answer;
}

//------------------------------------------------------------------------------

std::string DCMImage::getOFStringTag(DCMImage::TagID tagId, const unsigned long pos) const {
    /* DICOM tags have the form (gggg,eeee), thus, to compose a DcmTagKey:
     * tagId >> 16    : gets the first 16 bits (gggg) of tagId
     * tagId & 0xFFFF : gets the last 16 bits (eeee) of tagId
     */
    OFString answer;
    OFCondition cond;

    if (dcmFileFormat == NULL) {
        throw logic_error("DCMImage: Null DICOM header pointer.");
    }

    cond = dcmFileFormat->getDataset()->findAndGetOFString(DcmTagKey(tagId >> 16, tagId & 0xFFFF), answer, pos);
    if (cond.bad()) {
        stringstream ss;
        ss << hex << tagId << ". DcmDataset.findAndGetOFString code: " << cond.code() << " (" << cond.text() << ").";
        throw dicom_error("Error reading DICOM tag 0x" + ss.str());
    }

    return answer.c_str();
}//------------------------------------------------------------------------------