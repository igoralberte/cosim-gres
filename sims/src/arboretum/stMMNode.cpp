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
* This file implements the MMTree node.
*
* @version 1.0
* @author Ives Renê Venturini Pola (ives@icmc.sc.usp.br)
*/
#include <arboretum/stMMNode.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// class stMMNode
//-----------------------------------------------------------------------------
stMMNode::stMMNode(stPage * page, bool create){

   this->Page = page;

   // if create is true, we must to zero fill the page
   if (create){
      Page->Clear();
   }//end if

   // Set elements
   this->Header = (stMMNodeHeader *) this->Page->GetData();
   this->Entries = (u_int32_t *)(this->Page->GetData() + sizeof(stMMNodeHeader));

}//end stMMNode::stMMNode

//------------------------------------------------------------------------------
int stMMNode::AddEntry(u_int32_t size, const unsigned char * object){
   u_int32_t totalsize;
   u_int32_t offs;

   totalsize = size + sizeof(u_int32_t);
   if (totalsize <= GetFree()){

      // Object offset
      if (Header->Occupation == 0){
         offs = Page->GetPageSize() - size;
      }else{
         if (Header->Occupation == 1){
            offs = Entries[Header->Occupation-1] - size;
         }else{
            return -1; //there can be no more than two objects per node
         }//end if
      }//end if

      // Update entry
      this->Entries[Header->Occupation] = offs;

      // Write object
      memcpy((void *)(Page->GetData() + Entries[Header->Occupation]),
             (void *)object, size);

      // Update header
      this->Header->Occupation++;
      return ((int )this->Header->Occupation) - 1;
   }else{
      return -1;  // there is no room for the object
   }//end if
}//end stMMNode::AddEntry

//------------------------------------------------------------------------------
const unsigned char * stMMNode::GetObject(int id){

   return (unsigned char *) Page->GetData() + Entries[id];
}//end stMMNode::GetObject

//------------------------------------------------------------------------------
u_int32_t stMMNode::GetObjectSize(int id){

   if (id == 0){
      return Page->GetPageSize() - Entries[0];
   }else{
      return Entries[id-1] - Entries[id];
   }//end if
}//end stMMNode::GetObjectSize

//------------------------------------------------------------------------------
u_int32_t stMMNode::GetFree(){

   if (Header->Occupation == 0){
      return Page->GetPageSize() - sizeof(stMMNodeHeader);
   }else{
      return Page->GetPageSize() - sizeof(stMMNodeHeader) -
             (sizeof(u_int32_t) * Header->Occupation) -
             (Page->GetPageSize() - Entries[Header->Occupation-1]);
   }//end if
}//end stMMNode::GetFree
