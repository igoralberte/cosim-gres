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
* This file include all files needed by artemis.
*
* @version 1.0
*/

#ifndef ARTEMIS_H
#define ARTEMIS_H

#include <math.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstring>


#include <artemis/extractor/Extractor.h>
#include <artemis/extractor/HaralickExtractor.h>
#include <artemis/extractor/HaralickFeature.h>
#include <artemis/extractor/MetricHistogram.h>
#include <artemis/extractor/TotalColorHistogram.h>
#include <artemis/extractor/NormalizedHistogram.h>
#include <artemis/extractor/Wavelets.h>
#include <artemis/extractor/ZernikeExtractor.h>
#include <artemis/extractor/TextureSpectrum.h>
#include <artemis/extractor/DiscreteCosineTransformation.h>
#include <artemis/extractor/YCrCbColorSystem.h>
#include <artemis/extractor/ColorLayout.h>
#include <artemis/extractor/ColorStructure.h>
#include <artemis/extractor/ScalableColor.h>
#include <artemis/extractor/EdgeHistogram.h>
#include <artemis/extractor/EdgeHistogramExtractor.h>
#include <artemis/extractor/SRGBColorSystem.h>
#include <artemis/extractor/XYZColorSystem.h>
#include <artemis/extractor/ColorTemperature.h>
#include <artemis/extractor/DaubechiesExtractor.h>
#include <artemis/extractor/HaarExtractor.h>
#include <artemis/extractor/TextureBrowsing.h>
#include <artemis/extractor/LocalBinaryPatternExtractor.h>
#include <artemis/extractor/BICHistogramExtractor.h>
#include <artemis/extractor/RotationInvariantLBPExtractor.h>

#include <artemis/image/Pixel.h>
#include <artemis/image/ImageBase.h>
#include <artemis/image/bmp/BmpLib.h>
//#include <artemis/image/dicom/DcmLib.h>
#include <artemis/image/jpg/JpgLib.h>
//#include <artemis/image/krl/FileHandler.h>
//#include <artemis/image/krl/KrlLib.h>
#include <artemis/image/png/PngLib.h>

#endif
