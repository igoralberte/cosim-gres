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
* This file implements the Dummy Tree node.
*
* @version 1.0
* $Revision: 1.9 $
* $Date: 2005-03-08 19:44:35 $
* $Author: marcos $
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#include <arboretum/stDummyNode.h>

//-----------------------------------------------------------------------------
// class stDummyNode
//-----------------------------------------------------------------------------
stDummyNode::stDummyNode(stPage * page, bool create){

   this->Page = page;

   // Will I create it ?
   if (create){
      Page->Clear();
   }//end if

   // Set elements
   this->Header = (stDummyHeader *) Page->GetData();
   this->Entries = (u_int32_t *)(Page->GetData() + sizeof(stDummyHeader));
}//end stDummyNode::stDummyNode

//------------------------------------------------------------------------------
int stDummyNode::AddEntry(u_int32_t size, const unsigned char * object){
   u_int32_t totalsize;
   u_int32_t offs;

   totalsize = size + sizeof(u_int32_t);
   if (totalsize <= GetFree()){
      // Object offset
      if (Header->Occupation == 0){
         offs = Page->GetPageSize() - size;
      }else{
         offs = Entries[Header->Occupation - 1] - size;
      }//end if

      // Write object
      Page->Write((unsigned char*)object, size, offs);

      // Update entry
      Entries[Header->Occupation] = offs;

      // Update header
      Header->Occupation++;
      return Header->Occupation - 1;
   }else{
      return -1;
   }//end if
}//end stDummyNode::AddEntry

//------------------------------------------------------------------------------
const unsigned char * stDummyNode::GetObject(int idx){

   return Page->GetData() + Entries[idx];
}//end stDummyNode::GetObject

//------------------------------------------------------------------------------
u_int32_t stDummyNode::GetObjectSize(int idx){

   if (idx == 0){
      return Page->GetPageSize() - Entries[0];
   }else{
      return Entries[idx - 1] - Entries[idx];
   }//end if
}//end stDummyNode::GetObjectSize

//------------------------------------------------------------------------------
void stDummyNode::RemoveEntry(u_int32_t idx){
   u_int32_t i;
   u_int32_t lastID;
   u_int32_t rObjSize;

   // Programmer's note: This procedure is simple but tricky! See the
   // stDBMNode structure documentation for more details.

   #ifdef __stDEBUG__
   if (idx >= (int )GetNumberOfEntries()){
      // Oops! This id doesn't exists.
      throw range_error("Invalid idx!");
   }//end if
   #endif //__stDEBUG__

   // Let's remove
   lastID = Header->Occupation - 1; // The id of the last object. This
                                    // value will be very useful.
   // Do I need to move something ?
   if (lastID != idx){
      // Yes, I do. Save the removed object size
      rObjSize = GetObjectSize(idx);

      // Let's move objects first. We will use memmove() from stdlib because
      // it handles the overlap between src and dst. Remember that src is the
      // offset of the last object and the dst is the offset of the last
      // object plus removed object size.
      memmove(Page->GetData() + Entries[lastID] + rObjSize,
              Page->GetData() + Entries[lastID],
              Entries[idx] - Entries[lastID]);
      // Let's move entries...
      for (i = idx; i < lastID; i++){
         // Copy all fields with memcpy (it's faster than field copy).
         memcpy(Entries + i, Entries + i + 1, sizeof(u_int32_t));
         // Update offset by adding the removed object size to it. It will
         // reflect the previous move operation.
         Entries[i] += rObjSize;
      }//end for
   }//end if

   // Update counter...
   Header->Occupation--;
}//end stDummyNode::RemoveEntry

//------------------------------------------------------------------------------
u_int32_t stDummyNode::GetFree(){

   if (Header->Occupation == 0){
      return Page->GetPageSize() - sizeof(stDummyHeader);
   }else{
      return Page->GetPageSize() - sizeof(stDummyHeader) -
            (sizeof(u_int32_t) * Header->Occupation) -
            (Page->GetPageSize() - Entries[Header->Occupation - 1]);
   }//end if
}//end stDummyNode::GetFree
