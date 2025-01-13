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
* This file defines some useful struct layer utilities.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#include  <arboretum/stStructUtils.h>

//-----------------------------------------------------------------------------
// class doubleCache
//-----------------------------------------------------------------------------
doubleCache::doubleCache(int n){
   int size;
   double * p;

   // Remember the AP step 1...   
   size = (((1 + n) * n) >> 1) - 1;
   distances = new double[size];
   
   // Set all to -1
   for (p = distances; p != distances + size; p++){
      *p = -1;
   }//end for
}//end doubleCache::doubleCache
