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
* This file implements the VPTree node.
*
* @version 1.0
* @author Ives Renê Venturini Pola (ives@icmc.usp.br)
*/
#include <arboretum/stVPNode.h>

//-----------------------------------------------------------------------------
// class stVPNode
//-----------------------------------------------------------------------------
stVPNode::stVPNode(stPage * page, bool create){

   this->Page = page;

   // if create is true, we must to zero fill the page
   if (create){
      Page->Clear();
   }//end if

   // Set elements
   this->Header = (stVPNodeHeader *) this->Page->GetData();
   this->Entry = (u_int32_t *)(this->Page->GetData() + sizeof(stVPNodeHeader));
}//end stVPNode::stVPNode

//------------------------------------------------------------------------------
bool stVPNode::AddEntry(u_int32_t size, const unsigned char * object){
   u_int32_t totalsize;
   u_int32_t offs;

   totalsize = size + sizeof(u_int32_t);
   if (totalsize <= GetFree()){

      offs = Page->GetPageSize() - size;

      // Update entry offset
      *Entry = offs;
      // Write object
      memcpy( (void *) (Page->GetData() + *Entry), (void *) object, size);

      return true;
   }else{
      // there is no room for the object
      return false;
   }//end if
}//end stVPNode::AddEntry

//------------------------------------------------------------------------------
const unsigned char * stVPNode::GetObject(){
   return (unsigned char *) Page->GetData() + *Entry;
}//end stVPNode::GetObject

//------------------------------------------------------------------------------
u_int32_t stVPNode::GetObjectSize(){
   return (u_int32_t) Page->GetPageSize() - *Entry;
}//end stVPNode::GetObjectSize

//------------------------------------------------------------------------------
u_int32_t stVPNode::GetFree(){
   return Page->GetPageSize() - sizeof(stVPNodeHeader);
}//end stVPNode::GetFree
