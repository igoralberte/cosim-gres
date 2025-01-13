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
* This file defines a generic array object that implements all methods required
* by the stObject interface. This object may be used in combination with the
* metric evaluators defined in the file stBasicMetricEvaluator.h.
*
* @version 2.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/

//----------------------------------------------------------------------------
// Class template dlGenericMatrix
//----------------------------------------------------------------------------
template < class Type >
void dlGenericMatrix<Type>::SetSize(int cols, int rows){
   int w;

 	if (Data != NULL){
      delete [] Data;
    	delete [] PRows;
   }//end if

   Rows = rows;
   Cols = cols;
   Data = new Type[cols * rows];
   PRows = new Type * [rows];
   PRows[0] = Data;
   for (w = 1; w < rows; w++){
      PRows[w] = PRows[w - 1] + cols;
   }//end for
}//end dlGenericMatrix<Type>::SetSize

//----------------------------------------------------------------------------
// Class template dlGenericMatrix
//----------------------------------------------------------------------------
template < class Type >
dlHugeGenericMatrix<Type>::~dlHugeGenericMatrix(){

   DisposeRows();
}//end stHugeGenericMatrix<Type>::~stHugeGenericMatrix

//----------------------------------------------------------------------------
template < class Type >
void dlHugeGenericMatrix<Type>::SetSize(int cols, int rows){
   int w;

   if ((cols != Cols) || (rows != Rows)){
      // Kill previows matrix
      DisposeRows();

      Rows = rows;
      Cols = cols;
      PRows = new Type * [rows];
      for (w = 0; w < rows; w++){
         PRows[w] = new Type[cols];
      }//end for      
   //}else{
      // Do nothing. He he he...
   }//end if
}//end dlGenericMatrix<Type>::SetSize

//----------------------------------------------------------------------------
template < class Type >
void dlHugeGenericMatrix<Type>::DisposeRows(){
   Type ** i;

 	if (PRows != NULL){
      for (i = PRows; i < PRows + Rows; i++){
         delete [] *i;
      }//end for
      delete [] PRows;
   }//end if
}//end dlGenericMatrix<Type>::DisposeRows 
