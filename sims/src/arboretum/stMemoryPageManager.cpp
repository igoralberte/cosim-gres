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
* This file implements the stMemoryPageManager.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @author Enzo Seraphim (seraphim@icmc.usp.br)
* @todo Review of documentation.
*/

#include <arboretum/stMemoryPageManager.h>

// Include exception class
#ifdef __stDEBUG__
   #include <stdexcept>
   #ifdef __BORLANDC__
      using namespace std;
   #endif //__BORLANDC__
#endif //__stDEBUG__

//------------------------------------------------------------------------------
// Class stMemoryPageManager
//------------------------------------------------------------------------------
stMemoryPageManager::stMemoryPageManager(u_int32_t pagesize){

   // Size of the page
   this->PageSize = pagesize;

   // Clear Statistics
   this->ResetStatistics();
}//end stMemoryPageManager::stMemoryPageManager

//------------------------------------------------------------------------------
stMemoryPageManager::~stMemoryPageManager(){
   u_int32_t i;

   for (i = 0; i < Pages.size(); i++){
      if (Pages.at(i) != NULL){
         delete Pages.at(i);
      }//end if
   }//end for
}//end stMemoryPageManager::stMemoryPageManager

//------------------------------------------------------------------------------
bool stMemoryPageManager::IsEmpty(){

   return Pages.size() - FreePages.size() < 2;
}//end stMemoryPageManager::IsEmpty

//------------------------------------------------------------------------------
stPage * stMemoryPageManager::GetHeaderPage(){

   // Create the page if required.
   if (Pages.size() == 0){
      Pages.insert(Pages.end(), new stPage(this->PageSize, 0));
   }//end if

   // Update Counters
   UpdateReadCounter(); 

   // It will always be the first page.
   return Pages.at(0);
}//end stMemoryPageManager::GetHeaderPage

//------------------------------------------------------------------------------
stPage * stMemoryPageManager::GetPage(u_int32_t pageid){

   #ifdef __stDEBUG__
   if ((pageid < 1) || (pageid >= Pages.size())){
      throw invalid_argument("Invalid page id.");
   }//end if
   #endif //__stDEBUG__

   // Update statistics
   UpdateReadCounter(); 

   return Pages.at(pageid);
}//end stMemoryPageManager::GetPage

//------------------------------------------------------------------------------
void stMemoryPageManager::ReleasePage(stPage * page){

   // Nothing to do!
}//end stMemoryPageManager::ReleasePage

//------------------------------------------------------------------------------
stPage * stMemoryPageManager::GetNewPage(){
   stPage * newpage;
   u_int32_t freepage;

   // Update Counters
   UpdateWriteCounter(); 

   // Check stack
   if (FreePages.empty()){
      // New!
      newpage = new stPage(this->PageSize, Pages.size());
      // Add
      Pages.insert(Pages.end(), newpage);
      // Return
      return newpage;
   }else{
      // From stack
      freepage = FreePages.top();
      FreePages.pop();
      return Pages.at(freepage);
   }//end if
}//end stMemoryPageManager::GetNewPage

//------------------------------------------------------------------------------
void stMemoryPageManager::WritePage(stPage * page){

   // Update statistics
   UpdateWriteCounter(); 
}//end stMemoryPageManager::WritePage

//------------------------------------------------------------------------------
void stMemoryPageManager::DisposePage(stPage * page){

   #ifdef __stDEBUG__
   if (page==NULL){
      throw invalid_argument("Are you nuts ? \"page\" can not be NULL.");
   }//end if
   #endif //__stDEBUG__

   FreePages.push(page->GetPageID());
}//end stMemoryPageManager::DisposePage

