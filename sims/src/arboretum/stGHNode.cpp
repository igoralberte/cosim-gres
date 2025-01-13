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
* This file implements the GH Tree node.
*
* @version 1.0
* @author Ives Renê Venturini Pola (ives@icmc.sc.usp.br)
*/
#include <arboretum/stGHNode.h>

//-----------------------------------------------------------------------------
// class stGHNode
//-----------------------------------------------------------------------------

stGHNode::stGHNode(stPage * page, bool create){

   // Set elements
   this->Page = page;
   this->Header = (stGHNodeHeader *) this->Page->GetData();
   this->Entries = (stGHNodeEntry *)(this->Page->GetData() +
                   sizeof(stGHNodeHeader));
   // if create is true, we must to zero fill the page
   if (create){
      #ifdef __stDEBUG__
      Page->Clear();
      #endif //__stDEBUG__
      this->Header->Occupation = 0;
   }//end if
}//end stGHNode::stGHNode

//------------------------------------------------------------------------------
int stGHNode::AddEntry(u_int32_t size, const unsigned char * object){
   u_int32_t entrySize;

   #ifdef __stDEBUG__
   if (size == 0){
      throw invalid_argument("The object size is 0.");
   }//end if
   #endif //__stDEBUG__

   // Does it fit ?
   entrySize = size + sizeof(stGHNodeEntry);
   if (entrySize > this->GetFree()){
      // No, it doesn't.
      return -1;
   }//end if

   // Ok. I can put it. Lets put it in the last position.

   // Adding the object. Take care with these pointers or you will destroy the
   // node. The idea is to put the object of an entry in the reverse order
   // in the data array.
   if (Header->Occupation == 0){
      Entries[Header->Occupation].Offset = Page->GetPageSize() - size;
   }else{
      if (Header->Occupation == 1){
         Entries[Header->Occupation].Offset = Entries[Header->Occupation - 1].Offset - size;
      }else{
         return -1; // It cannot store more than two objects per node
      }//end if
   }//end if
   // Set the pageID.
   this->Entries[Header->Occupation].PageID = 0;
   memcpy((void *)(Page->GetData() + Entries[Header->Occupation].Offset),
          (void *)object, size);
   // Update # of entries
   Header->Occupation++; // One more!

   return Header->Occupation - 1;
}//end stGHNode::AddEntry

//------------------------------------------------------------------------------
const unsigned char * stGHNode::GetObject(u_int32_t idx){

   #ifdef __stDEBUG__
   if (idx >= GetNumberOfEntries()){
      throw invalid_argument("idx value is out of range.");
   }//end if
   #endif //__stDEBUG__

   return Page->GetData() + Entries[idx].Offset;
}//end stGHNode::GetObject

//------------------------------------------------------------------------------
u_int32_t stGHNode::GetObjectSize(u_int32_t idx){
   #ifdef __stDEBUG__
   if (idx >= GetNumberOfEntries()){
      throw invalid_argument("idx value is out of range.");
   }//end if
   #endif //__stDEBUG__

   if (idx == 0){
      // First object
      return Page->GetPageSize() - Entries[0].Offset;
   }else{
      // Any other
      return Entries[idx - 1].Offset - Entries[idx].Offset;
   }//end if
}//end stGHNode::GetObjectSize

//------------------------------------------------------------------------------
void stGHNode::RemoveEntry(u_int32_t idx){
   u_int32_t lastID;
   u_int32_t i;
   u_int32_t rObjSize;

   // Programmer's note: This procedure is simple but tricky! See the
   // stGHNode structure documentation for more details.

   #ifdef __stDEBUG__
   if (idx >= GetNumberOfEntries()){
      // Oops! This idx doesn't exists.
      throw range_error("idx value is out of range.");
   }//end if
   #endif //__stDEBUG__

   // Let's remove
   lastID = Header->Occupation - 1; // The idx of the last object. This
                                    // value will be very useful.
   // Do I need to move something ?
   if (idx != lastID){
      // Yes, I do.
      rObjSize = GetObjectSize(idx);    // Save the removed object size

      // Let's move objects first. We will use memmove() from stdlib because
      // it handles the overlap between src and dst. Remember that src is the
      // offset of the last object and the dst is the offset of the last
      // object plus removed object size.
      memmove(Page->GetData() + Entries[lastID].Offset + rObjSize,
              Page->GetData() + Entries[lastID].Offset,
              Entries[idx].Offset - Entries[lastID].Offset);

      // Let's move entries...
      for (i = idx; i < lastID; i++){
         // Copy all fields with memcpy (it's faster than field copy).
         memcpy(Entries + i, Entries + i + 1, sizeof(stGHNodeEntry));

         // Update offset by adding the removed object size to it. It will
         // reflect the previous move operation.
         Entries[i].Offset += rObjSize;
      }//end for
   }//end if

   // Update counter...
   Header->Occupation--;
}//end stGHNode::RemoveEntry
                        
//------------------------------------------------------------------------------
u_int32_t stGHNode::GetFree(){                                                     
   u_int32_t usedSize;

   // Fixed size
   usedSize = sizeof(stGHNodeHeader);

   // Entries
   if (GetNumberOfEntries() > 0){

      usedSize +=
         // Total size of entries
         (sizeof(stGHNodeEntry) * GetNumberOfEntries()) +
         // Total object size
         (Page->GetPageSize() - Entries[GetNumberOfEntries() - 1].Offset);
   }//end if

   return Page->GetPageSize() - usedSize;
}//end stGHNode::GetFree
