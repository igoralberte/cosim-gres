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
//---------------------------------------------------------------------------
// stHistogram.h ...
//
// Author: Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
//---------------------------------------------------------------------------
#ifndef stHistogramH
#define stHistogramH

#include <vector>

//---------------------------------------------------------------------------
// class stHistogram
//---------------------------------------------------------------------------
template <class ObjectType, class EvaluatorType>
class stHistogram {

   public:

      /**
      * Type of the object.
      */
      typedef ObjectType tObject;

      /**
      * Creates a new instance of this class.
      */
      stHistogram(unsigned int numberOfBins = 100);

      /**
      * Destruct of this class.
      */
      ~stHistogram();

      int Size(){
         return NumberOfBins;
      }//end Size

      int GetNumberOfObjects();

      void Build(EvaluatorType * metricEvaluator);

      void Add(ObjectType * obj);

      double GetBin(unsigned int idx);

      double GetValue(unsigned int idx);

      double GetValueOfBin(double value);

   private:

      std::vector <tObject *> Objects;

      double * Values;

      double * Bins;

      unsigned int NumberOfBins;

};//end stHistogram

#include "stHistogram-inl.h"

#endif //end stHistogramH
