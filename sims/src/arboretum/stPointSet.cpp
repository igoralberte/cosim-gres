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
* This file implements the class stStorageGrid.
*
* @version 2.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
*/

#include <arboretum/stPointSet.h>

//---------------------------------------------------------------------------
// Class stPointSet
//---------------------------------------------------------------------------
stPointSet::stPointSet(int NumberOfPoints) {
   count = firstPoint = lastPoint = 0;
   logR = new double[NumberOfPoints]; //The values of X in the graphic
   logSqR = new double[NumberOfPoints]; //the values of Y in the graph
   for (int i=0; i<NumberOfPoints; i++){
      logR[i] = 0;
      logSqR[i] = 0;
   }//end for
   maxCount = NumberOfPoints;
}//end stPointSet::stPointSet

//---------------------------------------------------------------------------
stPointSet::stPointSet(const stPointSet & set) {
   int i;
   count = set.count;
   maxCount = set.maxCount;
   logR = new double[maxCount]; //The values of X in the graphic
   logSqR = new double[maxCount]; //the values of Y in the graph
   for (i=0; i<count; i++){ //Setting the values
      logR[i] = set.logR[i];
      logSqR[i] = set.logSqR[i];
   }//end for
   for (i < count; i < maxCount; i++){
      logR[i] = 0;
      logSqR[i] = 0;
   }//end for
   firstPoint = set.firstPoint;
   lastPoint = set.lastPoint;
   Alpha = set.Alpha;
   Beta = set.Beta;
   error = set.error;
}//end stPointSet::stPointSet

//---------------------------------------------------------------------------
stPointSet::~stPointSet() {
   delete[] logR;
   delete[] logSqR;
}//end stPointSet::~stPointSet

//---------------------------------------------------------------------------
stPointSet & stPointSet::operator += (const stPointSet & set){
   int i, j, aux;
   aux = this->count;
   assert((this->count += set.count -1) <= maxCount); //verifies if the sum is less than maxCount
   for (i = aux, j=1; i < this->count; i++, j++){ //Setting the values to add the set
      this->logR[i] = set.logR[j];
      this->logSqR[i] = set.logSqR[j];
   }//end for
   this->lastPoint = set.lastPoint;
   this->Alpha = 0;
   this->Beta = 0;
   this->error = 0;
   return *this;
}//end stPointSet & stPointSet::operator +=

//---------------------------------------------------------------------------
stPointSet & stPointSet::operator -= (const stPointSet & set){
   int i,j,aux;
   bool equal;
   assert(this->maxCount >= set.count); //Verifies if maxCount is bigger than count
   aux = this->count - set.count + 1;
   for (i = aux-1 , j=0, equal = 1; j < set.count && equal == 1; i++, j++) {//Setting values
      equal = (this->logR[i] == set.logR[j] && this->logSqR[i] == set.logSqR[j]);
   }//end for
   if (equal){ //If equal, it will be removed from the graph
      aux = this->count;
      this->count -= set.count -1;
      for (i = this->count; i < aux; i++){
         this->logR[i] = 0;
         this->logSqR[i] = 0;
      }//end for
   }//end if

   this->lastPoint = set.firstPoint;
   this->Alpha = 0;
   this->Beta = 0;
   this->error = 0;
   return *this;
}//end stPointSet & stPointSet::operator -=

//---------------------------------------------------------------------------
stPointSet & stPointSet::operator = (const stPointSet & set){
   int i;
   // assert (this->maxCount >= set.count);
   this->count = set.count;
   for (i = 0; i < this->count; i ++){ //Until count, it will receive the current values, just copying
      this->logR[i] = set.logR[i];
      this->logSqR[i] = set.logSqR[i];
   }//end for
   for (i = this->count; i < maxCount; i ++){ //The others are zero yet
      this->logR[i] = 0;
      this->logSqR[i] = 0;
   }//end for
   this->firstPoint = set.firstPoint;
   this->lastPoint = set.lastPoint;
   this->Alpha = set.Alpha;
   this->Beta = set.Beta;
   this->error = set.error;
   return *this;
}//end stPointSet & stPointSet::operator =

//---------------------------------------------------------------------------
void stPointSet::Clear( ){
   for (int i = 0; i< count; i++){
      logR[i] = 0;
      logSqR[i] = 0;
   }//end for
   lastPoint = lastPoint = count = 0;
   Alpha = Beta = error = 0.0;
}//end stPointSet::Clear
