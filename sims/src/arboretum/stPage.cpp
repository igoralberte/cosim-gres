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
* This file implements stPage and stLockablePage.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
// Copyright (c) 2002-2003 GBDI-ICMC-USP

#include <arboretum/stPage.h>

//------------------------------------------------------------------------------
// class stPage
//------------------------------------------------------------------------------
stPage::stPage (u_int32_t size, u_int32_t pageid){

   this->BufferSize = size;
   this->Buffer = new unsigned char[size];
   this->SetPageID(pageid);
}//end stPage::stPage

//------------------------------------------------------------------------------
stPage::~stPage(){

   if (Buffer != 0){
      delete[] Buffer;
      Buffer = 0;
   }//end if
}//end stPage::~stPage

//------------------------------------------------------------------------------
void stPage::Write(unsigned char * buff, u_int32_t n, u_int32_t offset){

   #ifdef __stDEBUG__
   if ((offset + n) > this->BufferSize){
      throw invalid_argument("Offset out of bounds.");
   }//end if
   #endif //__stDEBUG__

   memcpy((void *)(this->Buffer + offset), (void*)buff, n);
}//end stPage::Write

//------------------------------------------------------------------------------
u_int32_t stPage::GetPageSize(){

   return this->BufferSize;
}//end stPage::GetPageSize

//------------------------------------------------------------------------------
unsigned char * stPage::GetData(){

   return this->Buffer;
}//end stPage::GetData

//------------------------------------------------------------------------------
void stPage::Copy(stPage * page){

   #ifdef __stDEBUG__
   if (page->GetPageSize() != GetPageSize()){
      throw invalid_argument("Both pages must have the same size.");
   }//end if
   #endif //__stDEBUG__

   memcpy(this->GetData(), page->GetData(), this->GetPageSize());
}//end stPage::Copy

//------------------------------------------------------------------------------
void stPage::Clear(){

   memset(this->GetData(), 0, this->GetPageSize());
}//end stPage::Clear

//------------------------------------------------------------------------------
// class stLockablePage
//------------------------------------------------------------------------------
void stLockablePage::Write(unsigned char * buff, u_int32_t n, u_int32_t offset){

   #ifdef __stDEBUG__
   if ((LockSize + offset + n) > this->BufferSize){
      throw invalid_argument("Offset out of bounds.");
   }//end if
   #endif //__stDEBUG__

   memcpy((void *)(this->Buffer + offset + LockSize),
         (void*)buff, n);
}//end stLockablePage::Write

//------------------------------------------------------------------------------
u_int32_t stLockablePage::GetPageSize(){

   return this->BufferSize - this->LockSize;
}//end stLockablePage::GetPageSize

//------------------------------------------------------------------------------
unsigned char * stLockablePage::GetData(){

   return this->Buffer + this->LockSize;
}//end stLockablePage::GetData

