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
#include <arboretum/stLevelDiskAccess.h>

//---------------------------------------------------------------------------
// class stLevelDiskAccess
//---------------------------------------------------------------------------
stLevelDiskAccess::stLevelDiskAccess(unsigned int height){
   Height = height;
   Radius = new double [Height];
   Nodes = new unsigned int [Height];
   Entries = new unsigned int [Height];
   for (unsigned int i = 0; i < Height; i++){
      Radius[i] = 0;
      Nodes[i] = 0;
      Entries[i] = 0;
   }//end if
}//end stLevelDiskAccess

//---------------------------------------------------------------------------
stLevelDiskAccess::~stLevelDiskAccess(){
   // Delete the resources.
   delete[] Radius;
   delete[] Nodes;
   delete[] Entries;
}//end ~stLevelDiskAccess

//---------------------------------------------------------------------------
void stLevelDiskAccess::AddEntry(double radius, unsigned int height){
   if (height < Height){
      Radius[height] += radius;
      Entries[height]++;
   }//end if
}//end AddEntry

//---------------------------------------------------------------------------
double stLevelDiskAccess::GetAvgRadius(unsigned int idx){
   if (idx < Height){
      return Radius[idx];
   }//end if
   return 0;
}//end GetAvgRadius

//---------------------------------------------------------------------------
long stLevelDiskAccess::GetNumberOfNodes(unsigned int idx){
   if (idx < Height){
      return Nodes[idx];
   }//end if
   return 0;
}//end GetNumberOfNodes

//---------------------------------------------------------------------------
void stLevelDiskAccess::AddNumberOfNodes(unsigned int height){
   if (height < Height){
      Nodes[height]++;
   }//end if
}//end GetNumberOfNodes

//---------------------------------------------------------------------------
void stLevelDiskAccess::Sumarize(){
   unsigned int i;
   
   for (i = 0; i < Height; i++){
      if (Entries[i] != 0){
         Radius[i] = Radius[i] / Entries[i];
      }else{
         Radius[i] = 0; 
      }//end if
   }//end if
}//end Sumarize
