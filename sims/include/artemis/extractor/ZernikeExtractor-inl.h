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
template<class SignatureType, class DataObjectType>
ZernikeExtractor<SignatureType, DataObjectType>::ZernikeExtractor(u_int8_t numMoments){
    setNumMoments(numMoments);
}

/**
* Sets the number of Zernike moments.
*
* @param numMoments The number of Zernike moments.
*/
template<class SignatureType, class DataObjectType>
void ZernikeExtractor<SignatureType, DataObjectType>::setNumMoments(u_int8_t numMoments){

    this->numMoments = numMoments;
}

/**
* Gets the number of Zernike moments.
*
* @return The number of Zernike moments.
*/
template<class SignatureType, class DataObjectType>
u_int8_t ZernikeExtractor<SignatureType, DataObjectType>::getNumMoments(){

    return numMoments;
}

/**
* Destructor.
*/
template<class SignatureType, class DataObjectType>
ZernikeExtractor<SignatureType, DataObjectType>::~ZernikeExtractor(){
}

/**
* Calculates de Zernike Polynom.
*
* @param n The n polynom entrance.
* @param m The m polynom entrance.
* @param r2 The r2 polynom entrance.
* @return The Zernike Polynom result.
*/
template<class SignatureType, class DataObjectType>
double ZernikeExtractor<SignatureType, DataObjectType>::realZernikePolynom(char n, char m, double r2) {

    char i, s, sign, lim;
    double  a,     // (n-s)!
            b,     //   s!
            c,     // [(n+|m|)/2-s]!
            d;     // [(n-|m|)/2-s]!

    double result = 0.0;
    double ep;

    m = abs(m);

    sign = a = b = c = d = 1;
    lim = (n-m) >> 1;

    // Set initial values for s=0
    for (i = n; i > 1; i--)
        a *= i;         // a = n!  &   b = 0! = 1
    for (i = (n+m) >> 1; i > 1; i--)
        c *= i; // c = [(n+|m|)/2]!
    for (i = lim; i > 1; i--)
        d *= i;        // d = [(n-|m|)/2-s]!

    result = ((double)a/(double)(b*c*d)) * (!n?1:pow(r2, (double)n/2.0));

    for (s=1; s <= lim; s++)  {
        sign *= -1;
        a /= n - (s - 1);
        b *= s;
        c /= ((n+m) >> 1) - (s - 1);
        d /= ((n-m) >> 1) - (s - 1);
        ep = ((double)n/2.0) - s;
        result += (double)sign * ((double)a/(double)(b*c*d)) * (!ep?1:pow(r2, ep));
    }

    return result;
}

/**
* Gets Zernike Polynom.
*
* @param n The n polynom entrance.
* @param m The m polynom entrance.
* @param x The x space position.
* @param y The y space position.
* @return The Zernike Polynom coordinates.
*/
template<class SignatureType, class DataObjectType>
std::pair< double, double> ZernikeExtractor<SignatureType, DataObjectType>::zernikePolynom(short n, short m, double x, double y) {

    std::pair< double, double> result;
    double  R, arg;
    double r2;

    r2 = (x*x) + (y*y);

    if (r2 == 0.0) {
        result.first = 0;
        result.second = 0;
        return result;
    }
    R = realZernikePolynom(n, m , r2);
    arg = m * atan2(y, x);
    result.first = R * cos(arg);
    result.second = R * sin(arg);

    return result;
}

/**
* Gets a Zernike space momentum.
*
* @param image The image to be processed
* @param n The n polynom entrance.
* @param m The m polynom entrance.
* @param xc The x center space position.
* @param yc The y center space position.
* @param xscale The xscale center space position.
* @param yscale The yscale center space position.
* @return The Zernike polynom coordinates momentum.
*/
template<class SignatureType, class DataObjectType>
std::pair<double, double> ZernikeExtractor<SignatureType, DataObjectType>::zernikeMoment(Image image, short n, short m, short xc, short yc, double xscale, double yscale){

    std::pair< double, double> result;
    double x, y;
    std::pair<double, double> zerp;
    unsigned char *p;
    double z;

    for (size_t l = 0; l < image.getHeight(); l++) {
        p = new unsigned char[image.getWidth()];
        for (size_t k = 0; k < image.getWidth(); k++){
            p[k] = image.getPixel(k, l).getGrayPixelValue();
        }

        for (size_t c = 0; c < image.getWidth(); c++) {
            y = (double)(l-yc)/yscale;
            x = (double)(c-xc)/xscale;

            if (((x*x) + (y*y)) <= 1) {
                zerp = zernikePolynom(n, m, x, y);
                result.first += p[c] *  zerp.first;
                result.second += p[c] * (-zerp.second);
            }
        }
    }

    if (p != NULL)
        delete[] p;

    z = (double)(n+1)/(double)2.1314;

    result.first *= z;
    result.second *= z;
    return result;
}

/**
* Gets the Zernike.
*
* @param image The image to be processed
* @param n The n polynom entrance.
* @return The Zernike polynom coordinates momentum.
*/
template<class SignatureType, class DataObjectType>
std::vector< std::pair<double, double> > ZernikeExtractor<SignatureType, DataObjectType>::zernikeMoments(short n, Image image){

    short m, _n;
    short num_moments;
    double xc, yc, xscale, yscale;
    std::vector< std::pair<double, double> > result;
    std::pair<double, double> fill;

    // The center of the circle/elipse
    xc = image.getWidth()  >> 1;
    yc = image.getHeight() >> 1;

    // The radii of the circle/elipse
    xscale = (image.getWidth()  -1) >> 1;
    yscale = (image.getHeight() -1) >> 1;

    // Determines the number of moments of n_th order
    num_moments = 0;
    for (m = n; m >= 0; m--)
        num_moments += (m >> 1) + 1;

    for (size_t x = 0; x < num_moments; x++)
        result.push_back(fill);

    size_t pos = 0;

    for (_n = 0; ((_n <=(n-1)) && (pos < result.size())); _n++){
        for (m = _n; ((m >= 0) && (pos < result.size())); m-=2){
            fill = zernikeMoment(image, _n, m, xc, yc, xscale, yscale);
            result[pos] = fill;
            pos++;
        }
    }

    return result;
}


/**
* Generates the Zenike descriptor defined in the template from the
* image provided.
*
* @param image The image to be processed.
* @param[out] sign The object to store the generated normalized histogram.
* @return The resulting ExtReturnCode.
*/
template<class SignatureType, class DataObjectType>
void ZernikeExtractor<SignatureType, DataObjectType>::generateSignature(const DataObjectType & image, SignatureType & sign) throw (std::runtime_error){

    std::vector< std::pair<double, double> > moments;
    double z, max;

    moments = zernikeMoments(getNumMoments(), image);

    z = moments[0].first;
    max = DBL_MIN;

    sign.resize(moments.size(), 0.0);

    for (size_t i = 0; i < moments.size(); i++){
        sign[i] = (sqrt(((moments[i].first)*(moments[i].first)) + ((moments[i].second)*(moments[i].second)))/z);
        if (sign[i] > max){
            max = sign[i];
        }
    }

    moments.clear();

    for (size_t i = 0; i < sign.size(); i++){
        sign[i] = ((double) sign[i]) / ((double) max);
    }
}

