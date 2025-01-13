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
* This file defines the Dummy Tree node.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#ifndef __STDUMMYNODE_H
#define __STDUMMYNODE_H

#include <arboretum/stPage.h>
#include <stdexcept>

/**
* This class implements the node of the Dummy Tree. Since the Dummy Tree is a
* sequential list, this node holds only objects and a link to the next node.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @ingroup dummy
*/
// +-----------------------------------------------------------------------------+
// | Occupation | Next | Offset0 | ... | OffsetN |... | ObjectN | ... | Object 0 |
// +-----------------------------------------------------------------------------+
class stDummyNode{
   public:
      /**
      * Creates a new instance of this class. The parameter <i>page</i> is an
      * instance of stPage that hold the node data.
      *
      * <P>The parameter <i>create</i> tells to stIndexPage what operation will
      * be performed. True means that the page will be initialized and false
      * means that the page will be used as it is. The default value is false.
      *
      * @param page The page that hold the data of this node.
      * @param create The operation to be performed.
      */
      stDummyNode(stPage * page, bool create = false);

      /**
      * Returns the associated page.
      *
      * @return The associated page.
      */
      stPage * GetPage(){
         return Page;
      }//end GetPage

      /**
      * Returns the ID of the associated page.
      *
      * @return The ID of the associated page.
      */
      u_int32_t GetPageID(){
         return Page->GetPageID();
      }//end GetPage

      /**
      * Returns the number of entries in this node.
      *
      * @return the number of entries.
      * @see GetEntry()
      * @see GetObject()
      * @see GetObjectSize()
      */
      u_int32_t GetNumberOfEntries(){
         return this->Header->Occupation;
      }//end GetNumberOfEntries

      /**
      * This method adds an object to this node.
      *
      * @param size The size of the object in bytes.
      * @param object The object data.
      * @return The position in the vector Entries or a negative value for failure.
      * @see RemoveObject()
      */
      int AddEntry(u_int32_t size, const unsigned char * object);

      /**
      * Gets the serialized object. Use GetObjectSize to determine the size of
      * the object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return A pointer to the serialized object.
      * @see GetObjectSize()
      */
      const unsigned char * GetObject(int idx);

      /**
      * Returns the size of the object in bytes. Use GetObject() to get the
      * object data.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      u_int32_t GetObjectSize(int idx);

      /**
      * Removes an entry from this object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      * unless __stDEBUG__ is defined at compile time.
      * @see GetObjectSize()
      * @todo This method is not implemented yet.
      */
      void RemoveEntry(u_int32_t idx);

      /**
      * Gets the next node linked to this node.
      */
      u_int32_t GetNextNode(){

         return this->Header->Next;
      }//end GetNextNode

      /**
      * Sets the next node linked to this node.
      *
      * @param id The ID of the next node.
      */
      void SetNextNode(u_int32_t idx){
         this->Header->Next = idx;
      }//end SetNextNode

   private:

      /**
      * Header of the stDummyNode.
      */
      typedef struct DummyHeader{
         /**
         * Occupation of this node.
         */
         u_int32_t Occupation;

         /**
         * The ID of the next page.
         */
         u_int32_t Next;
      }stDummyHeader;//end

      /**
      * The associated page.
      */
      stPage * Page;

      /**
      * Header of this node.
      */
      stDummyHeader * Header;

      /**
      * Entries.
      */
      u_int32_t * Entries;

      /**
      * Returns the free space available in this node.
      */
      u_int32_t GetFree();
      
};//end stDummyNode

#endif //__STDUMMYNODE_H
