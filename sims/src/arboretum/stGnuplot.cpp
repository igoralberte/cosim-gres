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
* This file implements the blocks of ParitionGlobal.
*
* @version 1.0
* @author Enzo Seraphim(seraphim@icmc.usp.br)
*/

#include <arboretum/stGnuplot3D.h>
#include <cstring>
#include <fstream>

//------------------------------------------------------------------------------
// Class stGnuplot
//------------------------------------------------------------------------------
stGnuplot::~stGnuplot(){
   if (title){
      delete [] title;
   }
   if (xlabel){
      delete [] xlabel;
   }
}// ~stGnuplot

//------------------------------------------------------------------------------
void stGnuplot::SetTitle(const char * str) {
   if (title){
      delete [] title;
   }
   title = new char[strlen(str)+1];
   strcpy(title, str);
}// SetTitle

//------------------------------------------------------------------------------
void stGnuplot::SetXLabel(const char * label) {
   if (xlabel){
      delete [] xlabel;
   }
   xlabel = new char[strlen(label)+1];
   strcpy(xlabel, label);
}// SetXLabel
//------------------------------------------------------------------------------

