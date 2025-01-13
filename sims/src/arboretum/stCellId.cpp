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
* This file implements the classes stCellIdLowDimensional and
* stCellIdHighDimensional.
*
* @version 2.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
*/

#include <arboretum/stCellId.h>


//---------------------------------------------------------------------------
// Class stCellIdHighDimensional
//---------------------------------------------------------------------------
void stCellIdHighDimensional::LeftShift(int indexTest){
   int pos = nPos - indexTest/8 - 1;  // set the position (of the array of char) where the bit number b is,
                              // considering bits from position nPos-1 to position 0  -
                              //  logical position 0 correponds to the last physical position

   bool addNext = 0, addNow = 0;

   //OBS: the bit values corresponding to the dimensions are defined from right to left)- it means that
   // the position of the array on the left side of the position pos (where b is...) is completely zeroed
   // because the dimensions higher than b haven't been processed yet

   if(pos>0) pos--; // points to the first zeroed position before a sequence containing at least one bit 1
   for (int i=nPos-1; i>=pos; i--){
      if (index[i]>=0x80){ //  first bit is 1
         addNext = 1;    // need to add 1 in the next step
      }
      else{
         addNext = 0;
         index[i] <<= 1;      // left shift
      }//end if
      if (addNow){
         index[i]+=1;  // put 1 in the last bit, coming from previous step
      }//end if
      addNow = addNext;
   }//end for
}//end stCellIdHighDimensional::LeftShift

//---------------------------------------------------------------------------
void stCellIdHighDimensional::operator >>=(int indexTest){
   int i,j;
   bool addNext = 0, addNow = 0;
   //j=0;
   //while (j<nPos && index[j]==0) j++;
   //if (j>0) j--; // deal with the first zeroed char before a sequence with 1
   for (i=0; i<nPos; i++){
      if (index[i]%2 == 1){ //  last bit is 1
         addNext = 1;     // need to add 1 in the next step
      }else{
         addNext = 0;
         index[i] >>= indexTest;      // right shift
      }//end if
      if (addNow){
         index[i]+=0x80;  // put 1 in the first bit, coming from previous step
      }//end if
      addNow = addNext;
   }//end for
}//end stCellIdHighDimensional::operator >>=

//---------------------------------------------------------------------------
/*void stCellId::operator <<= (int indexTest){
   int i,j;
   bool addNext = 0, addNow = 0;
   j=0;
   while (j<nPos && index[j]==0) j++;
   if (j>0) j--; // deal with the first zeroed char before a sequence with 1
   for (i=nPos-1; i>=j; i--){
      if (index[i]>=0x80) //  first bit is 1
         addNext = 1;    // need to add 1 in the next step
      else addNext = 0;
         index[i] <<= indexTest;      // left shift
      if (addNow) index[i]+=1;  // put 1 in the last bit, coming from previous step
         addNow = addNext;
   }//end for
   return;
}//end stCellId::operator <<=
*/
//---------------------------------------------------------------------------
void stCellIdHighDimensional::operator += (int valueAdd){
   int i;
   bool finished = 0;
   i=nPos-1;
   while (i>=0 && !finished){
      if (index[i]<0xFF){  //  index < 255 - at least one bit is 0, so the sum does not propagate to next char
         finished = 1;
      }//end if
      index[i] += valueAdd;      // add ValueAdd
      i--;
   }//end while
}//end stCellIdHighDimensional::operator +=

