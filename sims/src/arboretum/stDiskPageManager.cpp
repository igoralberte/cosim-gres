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

#include <arboretum/stDiskPageManager.h>

/**
* Number of instances in the page cache.
*/
#define STDISKPAGEMANAGER_INSTANCECACHESIZE 16

//------------------------------------------------------------------------------
// class stDiskPageManager
//------------------------------------------------------------------------------
stDiskPageManager::stDiskPageManager(const std::string & fName, u_int32_t pagesize,
      u_int32_t userHeaderSize,
      int cacheNPages):fileName(fName){

   myStorage = new CStorage;
   this->pageSize = pagesize;
   myStorage->Create(fileName.c_str(), pageSize, userHeaderSize, cacheNPages);

   // Instance cache with
   pageInstanceCache = new stPageInstanceCache(STDISKPAGEMANAGER_INSTANCECACHESIZE,
         new stPageAllocator(pageSize));
}//end stDiskPageManager::stDiskPageManager

//------------------------------------------------------------------------------
stDiskPageManager::stDiskPageManager(const std::string & fName):fileName(fName){

    this->myStorage = new CStorage();
    this->myStorage->Open(fileName.c_str());
    this->pageSize =  myStorage->GetPageSize();
    // Instance cache with
    pageInstanceCache = new stPageInstanceCache(
            STDISKPAGEMANAGER_INSTANCECACHESIZE, new stPageAllocator(pageSize));
}//end stDiskPageManager::stDiskPageManager

//------------------------------------------------------------------------------
stDiskPageManager::~stDiskPageManager(){

   if (myStorage != 0){
      if (myStorage->IsOpened()){
         myStorage->FlushCache();
         myStorage->Close();
      }//end if
   }//end if
   
   delete pageInstanceCache;
   pageInstanceCache = 0;
   delete myStorage;
   myStorage = 0;
}//end stDiskPageManager::~stDiskPageManager

//------------------------------------------------------------------------------
bool stDiskPageManager::IsEmpty(){

   // Error checking
   return myStorage->GetTotalPagesInUse() == 0;
}//end stDiskPageManager::IsEmpty

//------------------------------------------------------------------------------
stPage * stDiskPageManager::GetHeaderPage(){
   stPage *hPage = new stPage(this->GetHeaderPageSize());

   myStorage->ReadUserHeader(hPage->GetData());

   // Update Counters
   UpdateReadCounter();

   return hPage;
}//end stDiskPageManager::GetHeaderPage()

//------------------------------------------------------------------------------
stPage * stDiskPageManager::GetPage(u_int32_t pageid){
   stPage * myPage;

   if ((int)pageid <= myStorage->GetTotalPagesIncludingDisposed()){
      // Get from cache
      myPage = pageInstanceCache->Get();
      
      myStorage->ReadPage(pageid-1, myPage->GetData());
      myPage->SetPageID(pageid);
   
      // Update Counters
      UpdateReadCounter();
      return myPage;
   }else{
      // Error!!!
      return NULL;
   }//end if
}//end stDiskPageManager::GetPage()

//------------------------------------------------------------------------------
void stDiskPageManager::ReleasePage(stPage * page){

   // Put it back
   if (page->GetPageSize() == this->pageSize){
      pageInstanceCache->Put(page);
   }else{
      delete page;
	  page = 0;
   }//end if   
}//end stDiskPageManager::ReleasePage()

//------------------------------------------------------------------------------
stPage * stDiskPageManager::GetNewPage(){
   stPage * currentPage;
   
   // Get from cache
   currentPage = pageInstanceCache->Get();
   
   u_int32_t newID = myStorage->InsertNewPage((void *)currentPage->GetData())+1;
   currentPage->SetPageID(newID);

   // Update Counters
   UpdateWriteCounter();

   return currentPage;
}//end stDiskPageManager::GetNewPage()

//------------------------------------------------------------------------------
void stDiskPageManager::WritePage(stPage * page){

   // Write it but do not delete. It is not equal to ReleasePage.
   myStorage->WritePage(page->GetPageID()-1, (void *) page->GetData());

   // Update Counters
   UpdateWriteCounter();
}//end stDiskPageManager::WritePage

//------------------------------------------------------------------------------
void stDiskPageManager::WriteHeaderPage(stPage * headerpage){

   myStorage->WriteUserHeader((void *) headerpage->GetData());

   // Update Counters
   UpdateWriteCounter();
}//end stDiskPageManager::WriteHeaderPage

//------------------------------------------------------------------------------
void stDiskPageManager::DisposePage(stPage * page){

   myStorage->FreePage(page->GetPageID()-1);

   // Put it back
   pageInstanceCache->Put(page);
}//end stDiskPageManager::DisposePage()

//------------------------------------------------------------------------------
void stDiskPageManager::Create(const char *fName, int pagesize, int userHeaderSize, int cacheNPages){

   this->pageSize = pagesize;
   this->myStorage = new CStorage;
   myStorage->Create(fName, pageSize, userHeaderSize, cacheNPages);
   ResetStatistics();

   // Instance cache with 
   pageInstanceCache = new stPageInstanceCache(STDISKPAGEMANAGER_INSTANCECACHESIZE,
         new stPageAllocator(pageSize));
}//end stDiskPageManager::Create()

//------------------------------------------------------------------------------
void stDiskPageManager::Open(char *fname){
   if (this->myStorage == NULL)
      this->myStorage = new CStorage;

   this->myStorage->Open(fname);
   this->pageSize =  myStorage->GetPageSize();
   ResetStatistics();

   // Instance cache with
   pageInstanceCache = new stPageInstanceCache(STDISKPAGEMANAGER_INSTANCECACHESIZE,
         new stPageAllocator(pageSize));
}//end stDiskPageManager::Open()

//------------------------------------------------------------------------------
void stDiskPageManager::ResetFile(){

   myStorage->Drop();
}//end stDiskPageManager::Reset()

//------------------------------------------------------------------------------
long stDiskPageManager::GetHeaderPageSize(){

   return myStorage->GetUserHeaderSize();
}//end stDiskPageManager::GetHeaderPageSize()

//------------------------------------------------------------------------------
void stDiskPageManager::Flush(){

   myStorage->FlushCache();
}//end stDiskPageManager::Flush

//------------------------------------------------------------------------------
void stDiskPageManager::ResetStatistics(){
   stPageManager::ResetStatistics();

   myStorage->ResetStatistics();
}//end stDiskPageManager::ResetStatistics()

//------------------------------------------------------------------------------
inline u_int32_t stDiskPageManager::GetDiskReadCount(){

   return myStorage->counters.diskRead;
}//end stDiskPageManager::GetDiskReadCount()

//------------------------------------------------------------------------------
inline u_int32_t stDiskPageManager::GetDiskWriteCount(){

   return myStorage->counters.diskWrite;
}//end stDiskPageManager::GetDiskWriteCount()


