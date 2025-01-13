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
* This file defines the class stVPTree.
*
* @version 1.0
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/


#ifndef __STVPTREE_H
#define __STVPTREE_H

#include <math.h>
#include <algorithm>
#include <arboretum/stCommon.h>
#include <arboretum/stMetricTree.h>
#include <arboretum/stVPNode.h>

/**
* This class template implements a VP-tree. The VP-tree has the same
* external interface and behavior of a Metric Tree.
*
* <P>It can perform both range and k-nearest neighbour queries, and so its
* combinations and variations.
*
* This is a binary static memory metric tree
*
* <P>This class was developed to generate perfect answers to queries. It
* allows the build of automated test programs for other metric trees
* implemented by this library.
*
* @author Ives RenÃª Venturini Pola (ives@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @todo Documentation review.
* @version 1.0
* @ingroup VP
*/
template <class ObjectType, class EvaluatorType>
class stVPTree: public stMetricTree<ObjectType, EvaluatorType>{

   public:

      /**
      * This type defines the header of the VPTree.
      */
      typedef struct VPTreeHeader{
         /**
         * Magic number. This is a short string that must contains the magic
         * string "VP-3". It may be used to validate the file (this feature
         * is not implemented yet).
         */
         char Magic[4];
      
         /**
         * The root.
         */
         u_int32_t Root;

         /**
         * The height of the tree
         */
         u_int32_t Height;

         /**
         * Total number of objects.
         */
         u_int32_t ObjectCount;

         /**
         * The number of the nodes
         */
         u_int32_t NodeCount;
      } stVPTreeHeader;

      /**
      * This is the class that abstracts the object.
      */
      typedef ObjectType tObject;

      /**
      * This is the class that abstracts the metric evaluator.
      */
      typedef EvaluatorType tMetricEvaluator;

      /**
      * This is the class that abstracts an result set.
      */
      typedef stResult <ObjectType> tResult;

      /**
      * Creates a new instance of the VPTree.
      *
      * @param pageman The Page Manager to be used.
      */
      stVPTree(stPageManager * pageman);

      /**
      * Disposes this tree and releases all associated resources.
      */
      virtual ~stVPTree();

      /**
      * This method adds an object to the VPTree.
      * This method may fail it he object size exceeds the page size.
      *
      * @param obj The object to be added.
      * @return True for success or false otherwise.
      */
      bool Add(tObject ** objects, long listSize);

      /**
      * This method implements the Add method to insert object one-by-one in
      * the VP-Tree.
      *
      * @return success for the VPTree build.
      */
      bool Add(tObject * obj);

      /**
      * This method build the VP-tree after some Add calls.
      *
      * @return success for the VPTree build.
      */
      bool MakeVPTree();

      #ifdef __stDEBUG__
         /**
         * Get root page id.
         *
         * @warning This method is public only if __stDEBUG__ is defined at compile
         * time.
         */
         u_int32_t GetRoot(){
            return this->Header->Root;
         }//end GetRoot
      #endif //__stDEBUG__

      /**
      * Returns the height of the tree.
      */
      virtual u_int32_t GetHeight(){
         return Header->Height;
      }//end GetHeight

      /**
      * Returns the number of objetcs of this tree.
      */
      virtual long GetNumberOfObjects(){
         return Header->ObjectCount;
      }//end GetNumberOfObjects

      /**
      * This method will perform a range query. The result will be a set of
      * pairs object/distance.
      *
      * @param sample The sample object.
      * @param range The range of the results.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      tResult * RangeQuery(tObject * sample, double range);

      /**
      * This method will perform a k nearest neighbor query.
      *
      * @param sample The sample object.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      tResult * NearestQuery(tObject * sample, u_int32_t k, bool tie = false);

   private:

      /**
      * The header page. It will be kept in memory all the time to avoid
      * reads.
      */
      stPage * HeaderPage;

      /**
      * The header of the "tree".
      */
      stVPTreeHeader * Header;

      /**
      * If true, the header mus be written to the page manager.
      */
      bool HeaderUpdate;

      /**
      * To add object one-by-one.
      */
      tObject ** Objects;

      /**
      * The Used Size.
      */
      long Size;

      /**
      * The Maximum Capacity.
      */
      long Capacity;

		/**
		* Increment.
		*/
      long Increment;

      #ifndef __stDEBUG__
         /**
         * Get root page id.
         */
         u_int32_t GetRoot(){
            return this->Header->Root;
         }//end GetRoot
      #endif // __stDEBUG__

      /**
      * Sets a new root.
      */
      void SetRoot(u_int32_t root){
         this->Header->Root = root;
         this->HeaderUpdate = true;
      }//end SetRoot

      /**
      * Loads the header from the page manager.
      */
      void LoadHeader();

      /**
      * Sets all header's fields to default values.
      *
      * @warning This method will destroy the tree.
      */
      void DefaultHeader();

      /**
      * Updates the header in the file if required.
      */
      void WriteHeader(){
         if (this->HeaderUpdate){
            this->myPageManager->WriteHeaderPage(HeaderPage);
            this->HeaderUpdate = false;
         }//end if
      }//end WriteHeader

      /**
      * Disposes the header page if it exists. It also updates its contents
      * before destroy it.
      *
      * <P>This method is called by the destructor.
      */
      void FlushHeader(){
         if (HeaderPage != NULL){
            if (Header != NULL){
               this->WriteHeader();
            }//end if
            this->myPageManager->ReleasePage(HeaderPage);
         }//end if
      }//end FlushHeader
      
      /**
      * Creates a new empty page and updates the node counter.
      */
      stPage * NewPage(){
         this->Header->NodeCount++;
         return this->myPageManager->GetNewPage();
         this->HeaderUpdate = true;
      }//end NewPage

      /**
      * Disposes a given page and updates the page counter.
      */
      void DisposePage(stPage * page){
         Header->NodeCount--;
         this->myPageManager->DisposePage(page);
         this->HeaderUpdate = true;
      }//end DisposePage

      /**
      * This method recursively build the VP-tree based on the object list
      *
      * @return success for the VPTree build.
      */
      u_int32_t MakeVPTree(tObject ** objects, doubleIndex * & selected,
                          long size);

      /**
      * Used by MakeVPTree, selects a vantage point from a dataset from the
      * corner of the space.
      */
      long SelectVP(tObject ** objects, doubleIndex * selected, long size);

      /**
      *  This method calculates the median element, sorting the "sample" using
      *  the "vantage" as vantage point (index of the object in "obj".
      */
      long GetMedian(tObject ** objects, doubleIndex * sample, long sampleSize,
                    long vantage);

      /**
      *  Calculates the 2nd=Momento of the sample
      */
      double GetSecondMoment(doubleIndex * sample, long sampleSize, long median);

      /**
      *  This method calculate the sample size given the actual size
      */
      long PickSampleSize(long size){
         long sampleSize;

         // Try to get 10%
         sampleSize = (long) ceil(0.09 * (double )size);
         if (sampleSize < 50){
            sampleSize = 50;
            if (size < sampleSize){
               sampleSize = size;
            }//end if
         }//end if
         if (sampleSize > 500){
            sampleSize = 500;
            if (size < sampleSize){
               sampleSize = size;
            }//end if
         }//end if
         return sampleSize;
      }//end PickSampleSize

      /**
      *  Used to make a sample of the vector "selected"
      */
      void MakeSample(doubleIndex * sample, long sampleSize,
                      doubleIndex * selected, long size);

		/**
		* Expands the capacity of the object vector by adding increment
		* entries to the current capacity.
		*/
      void Resize();
      
      /**
      * Support for the RangeQuery, recursive code for searching.
      * @param sample The sample object.
      * @param range The range of the results.
      * @param result The result object to add the pairs <object, distance> found
      * @param page The page (node) to search in.
      */
      void RangeQuery(tObject * sample, double range,
                      tResult * result, u_int32_t page);

      /**
      * Support for the NearestQuery, recursive code for searching.
      * @param sample The sample object.
      * @param k The number of nearest neighbors to retrieve.
      * @param result The result object to add the pairs <object, distance> found
      * @param page The page (node) to search in.
      */
      void NearestQuery(tObject * sample, u_int32_t k, tResult * result,
                        u_int32_t page);

};//end stVPTree

#include <arboretum/stVPTree-inl.h>

#endif //__STVPTREE_H

