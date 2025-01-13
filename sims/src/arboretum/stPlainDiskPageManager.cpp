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
* This file implements the stPlainDiskPageManager.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @todo Implementation not tested.
*/
#include <arboretum/stPlainDiskPageManager.h>

/**
* Number of instances in the page cache.
*/
#define STDISKPAGEMANAGER_INSTANCECACHESIZE 16

//==============================================================================
// stPlainDiskPageManager
//------------------------------------------------------------------------------
stPlainDiskPageManager::stPlainDiskPageManager(const char * fName, u_int32_t pagesize){

   // Open file
   fd = open(fName, O_CREAT|O_TRUNC|O_RDWR|O_BINARY, S_IREAD|S_IWRITE); // New file with 0 bytes
   if (fd < 0){
      throw std::logic_error("Unable to create file.");
   }//Igor----
   /*else {
      if(remove(fName) != 0) //erase file to overwrite its content
         throw std::logic_error("Unable to reinitialize file.");
   }*///-------//end if

   //Igor----
   /*fd = open(fName, O_CREAT|O_TRUNC|O_RDWR|O_BINARY, S_IREAD|S_IWRITE); // New file with 0 bytes
   if (fd < 0){
      throw std::logic_error("Unable to create file.");
   }*/
   //---------

   // Initialize fields.
   this->headerPage = new stLockablePage(pagesize, sizeof(tHeader), 0);
   this->header = (tHeader *)(this->headerPage->GetTrueData());
   NewHeader(this->header, pagesize);
   WriteHeaderPage(headerPage);

   // Page cache
   pageInstanceCache = new stPageInstanceCache(STDISKPAGEMANAGER_INSTANCECACHESIZE,
         new stPageAllocator(pagesize));    
}//end stPlainDiskPageManager::stPlainDiskPageManager
//------------------------------------------------------------------------------

stPlainDiskPageManager::stPlainDiskPageManager(const char * fName){
   tHeader tmpHeader;
   
   // Open file
   fd = open(fName, O_RDWR|O_BINARY); // Open file
   if (fd < 0){
      throw std::logic_error("Unable to open file.");
   }//end if
   
   // Validate file
   if ((read(fd, &tmpHeader, sizeof(tmpHeader)) != sizeof(tmpHeader)) ||
         (!IsValidHeader(&tmpHeader))){
      throw std::logic_error("invalid file.");
   }//end if

   // WARNING!!! If you don't undertand what is going on here, see the
   // stLockablePage documentation for further info.
      
   // Ok, now I must reload the header again. I can not use
   this->headerPage = new stLockablePage(tmpHeader.PageSize, sizeof(tHeader), 0);
   this->header = (tHeader *)(this->headerPage->GetTrueData());

   // Transfer tmpHeader to header page because I don't like seeks...
   memcpy((void*)this->headerPage->GetTrueData(), &tmpHeader, sizeof(tHeader));
   // Load rest of the header.
   if (read(fd, (void *)this->headerPage->GetData(), (int)this->headerPage->GetPageSize())
         != (int)this->headerPage->GetPageSize()){
      delete headerPage;
      throw std::logic_error("invalid file.");
   }//end if

   // Page cache   
   pageInstanceCache = new stPageInstanceCache(STDISKPAGEMANAGER_INSTANCECACHESIZE,
         new stPageAllocator(header->PageSize));      
}//end stPlainDiskPageManager::stPlainDiskPageManager

//------------------------------------------------------------------------------
stPlainDiskPageManager::~stPlainDiskPageManager(){
   
   // Free resources
   delete pageInstanceCache;
   // Save header page info.
   WriteHeaderPage(headerPage);
   // Delete header page.
   delete this->headerPage;
   // Close file
   close(fd);
}//end stPlainDiskPageManager::~stPlainDiskPageManager

//------------------------------------------------------------------------------
bool stPlainDiskPageManager::IsEmpty(){
   
   return header->UsedPages == 0;
}//end stPlainDiskPageManager::IsEmpty
//------------------------------------------------------------------------------

stPage * stPlainDiskPageManager::GetHeaderPage(){
   
   // Update read count
   UpdateReadCounter();
   
   // Effective read
   lseek(fd, 0, SEEK_SET);  
   read(fd, (void *)this->headerPage->GetTrueData(), header->PageSize);
    
   return this->headerPage;
}//end stPlainDiskPageManager::GetheaderPage

//------------------------------------------------------------------------------
stPage * stPlainDiskPageManager::GetPage(u_int32_t pageid){
   stPage * myPage;

   // Do not allow users to load header page from this file.
   if ((pageid != 0) && (pageid <= header->PageCount)){
      
      // Get from cache
      myPage = pageInstanceCache->Get();
      
      // Read data...
      lseek(fd, PageID2Offset(pageid), SEEK_SET);
      read(fd, myPage->GetData(), header->PageSize);
      myPage->SetPageID(pageid);
   
      // Update Counters
      UpdateReadCounter();
      return myPage;
   }else{
      // Error!!!
      #ifdef __stDEBUG__
      throw invalid_argument("Invalid page ID.");
      #else
      return NULL;
      #endif //__stDEBUG__
   }//end if
}//end stPlainDiskPageManager::GetPage

//------------------------------------------------------------------------------
void stPlainDiskPageManager::ReleasePage(stPage * page){
   
   // Put it back
   if (page->GetPageSize() == header->PageSize){
      pageInstanceCache->Put(page);
   }else if (page->GetPageID() != 0){
      delete page;
   //}else{
      // Do nothing because it is the header page.
   }//end if      
}//end stPlainDiskPageManager::ReleasePage

//------------------------------------------------------------------------------
stPage * stPlainDiskPageManager::GetNewPage(){
   u_int32_t * next;
   stPage * page;
   
   if (header->Available == 0){
      // Get instance from cache
      page = pageInstanceCache->Get();
      
      // Creating the new page
      header->PageCount++;
      page->SetPageID(header->PageCount);
   }else{
      // Remove from free list
      page = GetPage(header->Available);
      next = (u_int32_t *)(page->GetData());
      header->Available = * next;
   }//end if
   
   // Update header
   header->UsedPages++;
   WriteHeaderPage(headerPage);
   
   return page;   
}//end stPlainDiskPageManager::GetNewPage

//------------------------------------------------------------------------------
void stPlainDiskPageManager::WritePage(stPage * page){

   #ifdef __stDEBUG__
   if (page->GetPageID() == 0){
      throw invalid_argument("Do not use WritePage to write header pages.");
   }//end if
   #endif //__stDEBUG__

   lseek(fd, PageID2Offset(page->GetPageID()), SEEK_SET);
   write(fd, page->GetData(), header->PageSize);      
   UpdateWriteCounter();
}//end stPlainDiskPageManager::WritePage

//------------------------------------------------------------------------------
void stPlainDiskPageManager::WriteHeaderPage(stPage * headerpage){
   
   #ifdef __stDEBUG__
   if (headerpage->GetPageID() != 0){
      throw invalid_argument("Do not use WriteHeaderPage to write standard pages.");
   }//end if
   #endif //__stDEBUG__
   
   lseek(fd, 0, SEEK_SET);
   write(fd, this->headerPage->GetTrueData(), header->PageSize);
   UpdateWriteCounter();
}//end stPlainDiskPageManager::WriteHeaderPage

//------------------------------------------------------------------------------
void stPlainDiskPageManager::DisposePage(stPage * page){
   u_int32_t * next;
   
   // Append to free list
   next = (u_int32_t *)page->GetData();
   *next = header->Available;
   header->Available = page->GetPageID();
   WritePage(page);

   // Update header   
   header->UsedPages--;
   WriteHeaderPage(headerPage);
   
   // Free resources
   ReleasePage(page);
}//end stPlainDiskPageManager::DisposePage

//------------------------------------------------------------------------------
void stPlainDiskPageManager::NewHeader(tHeader * header, u_int32_t pagesize){
   
   // Magic header. Always "DPM1".
   header->Magic[0] = 'D';
   header->Magic[1] = 'P';
   header->Magic[2] = 'M';
   header->Magic[3] = '1';

   // Organization
   header->PageSize = pagesize;
   
   // Page control
   header->PageCount = 0;
   header->UsedPages = 0;
   header->Available = 0;
}//end stPlainDiskPageManager::NewHeader

//------------------------------------------------------------------------------
bool stPlainDiskPageManager::IsValidHeader(tHeader * header){
   
   return (header->Magic[0] == 'D') &&
         (header->Magic[1] == 'P') &&
         (header->Magic[2] == 'M') &&
         (header->Magic[3] == '1'); 
}//end stPlainDiskPageManager::IsValidHeader
//------------------------------------------------------------------------------
