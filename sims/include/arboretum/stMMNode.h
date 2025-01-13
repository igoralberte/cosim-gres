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
* This file defines the MMTree node.
*
* @version 1.0
*/

#ifndef __STMMNODE_H
#define __STMMNODE_H

#include <arboretum/stPage.h>

#include <stdexcept>

/**
* This class implements the node of the MMtree. The MMtree is built by
* partitioning of the space in 4 disjoint regions, using two objects as
* representatives and the distance between them as the radius of both.
* @version 1.0
* @author Ives Rene Venturini Pola (ives@icmc.sc.usp.br)
* @todo Documentation review.
* @ingroup MM
*/

//-----------------------------------------------------------------------------
// Class stMMNode
//-----------------------------------------------------------------------------
/**
*
* +--------------------------------------------------------------------------------------------------+
* | Occupation | Distance |  ptr1 | ptr2 | ptr3 | ptr4 | Offset0 | Offset1 | ..blank.. | Obj1 | obj0 |
* +--------------------------------------------------------------------------------------------------+
*
* <p>The structure of MMtree Node follows:
* @image html mmnode.png "MMtree node structure"
*
*/
class stMMNode{

   public:
      /**
      * Creates a new instance of this class. The parameter <i>page</i> is an
      * instance of stPage that hold the node data.
      *
      * <P>The parameter <i>create</i> tells to stPage what operation will
      * be performed. True means that the page will be initialized and false
      * means that the page will be used just as it is. The default value is
      * false.
      *
      * @param page The page that hold the data of this node.
      * @param create The operation to be performed.
      */
      stMMNode(stPage * page, bool create = false);

      /**
      * Gets the associated page.
      *
      * @return The associated page.
      */
      stPage * GetPage(){
         return Page;
      }//end GetPage

      /**
      * Gets the ID of the associated page.
      *
      * @return The ID of the associated page.
      */
      u_int32_t  GetPageID(){
         return Page->GetPageID();
      }//end GetPageID

      /**
      * Gets the ID of a child page.
      *
      * @return The ID of the child page.
      */
      u_int32_t  GetChildPageID(int reg){
         return Header->RegionPageID[reg];
      }//end GetPageID

      /**
      * Sets the ID of a child page.
      *
      * @return nothing.
      */
      void SetChildPageID(int reg, u_int32_t ID){
         Header->RegionPageID[reg] = ID;
      }//end SetChildPageID
      
      /**
      * Gets the Distance of the objects.
      *
      * @return The Distance.
      */
      double GetDistance(){
         return this->Header->Distance;
      }//end GetDistance

      /**
      * Sets the Distance of the objects.
      *
      * @return nothing.
      */
      void SetDistance(double dist){
         Header->Distance = dist;
      }//end SetDistance

      /**
      * Returns the number of entries in this node.
      *
      * @return the number of entries.
      * @see GetEntry()
      * @see GetObject()
      * @see GetObjectSize()
      */
      unsigned char GetNumberOfEntries(){
         return this->Header->Occupation;
      }//end GetNumberOfEntries

      /**
      * This method adds an object to this node.
      *
      * @param size The size of the object in bytes.
      * @param object The object data.
      * @return The position in the vector Entries or a negative value for failure.
      */
      int AddEntry(u_int32_t size, const unsigned char * object);

      /**
      * Gets the serialized object. Use GetObjectSize to determine the size of
      * the object.
      *
      * @param id The id of the entry.
      * @warning The parameter id is not verified by this implementation
      *  unless __stDEBUG__ is defined at compile time.
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
      *  unless __stDEBUG__ is defined at compile time.
      * @return The size of the serialized object.
      * @see GetObject()
      */
      u_int32_t GetObjectSize(int idx);

      /**
      * Empty the node
      */
      void Clear(){
         this->Header->Occupation = 0;
      }//end Clear

   private:
   
      /**
      * Header of the stMMNode.
      */
      typedef struct MMNodeHeader{
         /**
         * Occupation of this node.
         */
         unsigned char Occupation;

         /**
         * The Distance between the two objects
         */
         double Distance;

         /**
         * The Page IDs of the respective regions
         */
         u_int32_t RegionPageID[4];
      } stMMNodeHeader;//end       

      /**
      * The associated page.
      */
      stPage * Page;

      /**
      * Header of this node.
      */
      stMMNodeHeader * Header;

      /**
      * Offset of the Entries.
      */
      u_int32_t * Entries;

      /**
      * Returns the free space available in this node.
      */
      u_int32_t GetFree();
};//end stMMNode

#endif //__STMMNODE_H
