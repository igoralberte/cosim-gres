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
* @file This file contains the Mahalanobis distance implementation.
*
* @version 1.0
*/

#ifndef MAHALANOBISDISTANCE_H
#define MAHALANOBISDISTANCE_H


#include "DistanceFunction.h"
#include <cmath>
#include <stdexcept>

#include <boost/assign/std/vector.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/covariance.hpp>
#include <boost/accumulators/statistics/variates/covariate.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

//using namespace boost::accumulators;
//using namespace boost::assign;
//using namespace boost::numeric::ublas;


typedef boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::mean> > localMean;
typedef boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::covariance<double, boost::accumulators::tag::covariate1> > > covariance_XY;
typedef boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::mean, boost::accumulators::tag::variance> > zscore;



/**
* Invert matrix through Boost.
*
* @param input The matrix to be inverted.
* @param inverse The inverted matrix.
*/
template<class T>
bool InvertMatrix (const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse) {

    typedef boost::numeric::ublas::permutation_matrix<std::size_t> pmatrix;
    boost::numeric::ublas::matrix<T> A(input);
    pmatrix pm(A.size1());

    int res = lu_factorize(A,pm);
    if( res != 0 )
        return false;

    inverse.assign(boost::numeric::ublas::identity_matrix<T>(A.size1()));
    lu_substitute(A, pm, inverse);

    return true;
}

/**
* @brief Mahalanobis Distance class.
* @author 012
* @author 006
* @date 10-13-2014
* @version 1.0.
*/
template <class ObjectType>
class MahalanobisDistance : public DistanceFunction <ObjectType>{

    private:
        size_t dim;
        boost::numeric::ublas::matrix<double> *inverted_matrix2;
        std::vector<double> mean_vector;

    public:

        MahalanobisDistance(size_t dim = 0);
        virtual ~MahalanobisDistance();

        void init(std::vector<ObjectType> &FVs) throw (std::length_error);

        double GetDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, ObjectType &obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1, std::vector<ObjectType> obj2) throw (std::length_error);
        double getDistance(ObjectType &obj1)throw (std::length_error);
};


#include "MahalanobisDistance-inl.h"
#endif // MAHALANOBISDISTANCE_H
