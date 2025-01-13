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
 * This file defines a Class to extract a Color Structure histogram.
 *
 * @version 1.0
 */

#ifndef COLORTEMPERATURE_H
#define COLORTEMPERATURE_H

#include <artemis/extractor/XYZColorSystem.h>
#include <artemis/extractor/Extractor.h>

#include <vector>
#include <cmath>
#include <artemis/image/ImageBase.h>

template< class SignatureType, class DataObjectType = Image>
class ColorTemperatureExtractor : public Extractor<SignatureType, DataObjectType> {

    private:
        u_int16_t threshold, multiplierCoeficient, delta;
        const double ROBERTSON_ISOTEMPERATURE_LINES_DATA[31][4] = {
            {0.0, 0.18006, 0.26352, -0.24341},
            {10.0, 0.18066, 0.26589, -0.25479},
            {20.0, 0.18133, 0.26846, -0.26876},
            {30.0, 0.18208, 0.27119, -0.28539},
            {40.0, 0.18293, 0.27407, -0.30470},
            {50.0, 0.18388, 0.27709, -0.32675},
            {60.0, 0.18494, 0.28021, -0.35156},
            {70.0, 0.18611, 0.28342, -0.37915},
            {80.0, 0.18740, 0.28668, -0.40955},
            {90.0, 0.18880, 0.28997, -0.44278},
            {100.0, 0.19032, 0.29326, -0.47888},
            {125.0, 0.19462, 0.30141, -0.58204},
            {150.0, 0.19962, 0.30921, -0.70471},
            {175.0, 0.20525, 0.31647, -0.84901},
            {200.0, 0.21142, 0.32312, -1.0182},
            {225.0, 0.21807, 0.32909, -1.2168},
            {250.0, 0.22511, 0.33439, -1.4512},
            {275.0, 0.23247, 0.33904, -1.7298},
            {300.0, 0.24010, 0.34308, -2.0637},
            {325.0, 0.24792, 0.34655, -2.4681},
            {350.0, 0.25591, 0.34951, -2.9641},
            {375.0, 0.26400, 0.35200, -3.5814},
            {400.0, 0.27218, 0.35407, -4.3633},
            {425.0, 0.28039, 0.35577, -5.3762},
            {450.0, 0.28863, 0.35714, -6.7262},
            {475.0, 0.29685, 0.35823, -8.5955},
            {500.0, 0.30505, 0.35907, -11.324},
            {525.0, 0.31320, 0.35968, -15.628},
            {550.0, 0.32129, 0.36011, -23.325},
            {575.0, 0.32931, 0.36038, -40.770},
            {600.0, 0.33724, 0.36051, -116.45}
        };

    private:
        std::vector < std::vector <double> > convertRGB2XYZ(const DataObjectType &image);
        u_int8_t discardPixels(double colorY);
        void averageProcedure(const DataObjectType &image, std::vector < std::vector <double> >, double *xA, u_int8_t channel);
        double colorTemperature(double xA, double yA, double zA, double *uS, double *vS);

    public:
        ColorTemperatureExtractor();
        virtual ~ColorTemperatureExtractor() ;

        void setThreshold(u_int16_t value);
        u_int16_t getThreshold();

        void setMultiplierCoeficient(u_int16_t value);
        void setDelta(u_int16_t value);

        u_int16_t getMultiplierCoeficient();
        u_int16_t getDelta();

        virtual void generateSignature(const DataObjectType &image, SignatureType &sign);
};


//include implementation (template class)
#include "ColorTemperature-inl.h"
#endif // COLORTEMPERATURE_H
