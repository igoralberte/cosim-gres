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
* This file defines the class stMMTree.
*
* @version 1.0
*/

#ifndef __STMMTREE_H
#define __STMMTREE_H

#include <math.h>
#include <arboretum/stCommon.h>

#include <arboretum/stMetricTree.h>
#include <arboretum/stMMNode.h>

#define NUMBEROFREGIONS 4
#define NUMBEROFUNKNOWN 9

/**
* This class template implements a MMTree. The MMTree has the same
* external interface and behavior of a Metric Tree.
*
* <P>It can perform both range and k-nearest neighbour queries, and so its
* combinations and variations.
*
* This is a memory metric tree, fast and light weight structure, that can be
* persistent !
*
* <P>This class was developed to generate perfect answers to queries. It
* allows the build of automated test programs for other metric trees
* implemented by this library.
*
* @author Ives Renï¿½ Venturini Pola (ives@icmc.usp.br)
* @todo Finish the implementation (some queries).
* @version 1.0
* @ingroup MM
*/
template <class ObjectType, class EvaluatorType>
class stMMTree: public stMetricTree<ObjectType, EvaluatorType>{
   public:

      enum NearestMethod {
         /*
         * Normal method for nearest neighbor queries. It just visit the regions
         * which intercept the radius of the knn always in the same order
         */
         nmNORMAL = 0,

         /*
         *  This is an optimized method for nearest neighbors queries. This
         *  method visit the regions wich intercept the radius query in a
         *  specific order, depending on the position of the sample object
         *  to query.
         */
         nmGUIDED = 1
      };

      enum InsertionMethod {
         /*
         * Normal method for insertion algorithm. It will not try to balance the
         * nodes after the insertion.
         */
         imNOBALANCE = 0,

         /*
         * Normal method for insertion algorithm. It will not try to balance the
         * nodes after the insertion.
         */
         imTRY2BALANCE = 1
      };

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
      * Creates a new instance of the MMTree.
      *
      * @param pageman The Page Manager to be used.
      */
      stMMTree(stPageManager * pageman);

      /**
      * Disposes this tree and releases all associated resources.
      */
      virtual ~stMMTree(){
         if (HeaderPage != NULL){
            if(Header != NULL){
               WriteHeader();
            }//end if
            // Release the header page.
            this->myPageManager->ReleasePage(HeaderPage);
         }//end if
      }//end ~stMMTree

      /*
      *   Chooses the Nearest Neighbor Query Method
      *   Default = nmNORMAL
      */
      void SetNearestMethod(enum NearestMethod method){
         Header->NearestMethod = method;
         WriteHeader();
      }//end SetNearestMethod

      /*
      *   Chooses the Insert Method
      *   Default = imNOBALANCE (0)
      */
      void SetInsertionMethod(enum InsertionMethod method){
         Header->InsertionMethod = method;
         WriteHeader();
      }//end SetInsertionMethod

      /**
      * This method adds an object to the MMTree.
      * This method may fail it he object size exceeds .
      *
      * @param obj The object to be added.
      * @return True for success or false otherwise.
      */
      virtual bool Add(tObject * obj);

      /**
      *  .....
      */
      void GetHeights(int &min, int &max);

      /**
      *  .....
      */
      void MinMax(u_int32_t page, int alt, int & altMin, int & altMax);

      /**
      *  .....
      */
      int GetNumberOfNodes(){
         return this->Header->NodeCount;
      }//end GetNumberOfNodes

      /**
      *  This method tries to balance the leaf nodes, avoiding the need for
      *  creating new nodes, consequently keeping small the height of that sub-tree
      */
      bool ExaustiveBalance(stMMNode * Parent, stMMNode * & curr, tObject * obj);

      /**
      *  This method returns true if the node associated with the attribute page
      *  have any children nodes, otherwise return false.
      */
      bool HaveChild(u_int32_t page);

      /**
      *   This method counts the number of objects in next level under the
      *   node Parent, including the objects in the node Parent.
      */
      int CountObjs(stMMNode * Parent);

      /**
      *  This method stores in the vector "objects" the objects in the parent node
      *  and all objects in its children nodes (only the next level down). Note
      *  that the vector needs to be alocated first.
      *
      *  Return:
      *     The number of objects stored.
      */
      int Associate(stMMNode * Parent, tObject ** objects);

      /**
      *  This method tries to distribute the entries of the vector "objects",
      *  in the "regions", therefore these objects will be contained in only
      *  one level (plus the parents in the parent node).
      */
      bool Distribute(tObject ** objects, int numObjs, int * regions);

      /**
      *  This method realocate each "objects[i]" in the proper "regions[i]"
      *  (child nodes) of the "Parent" node.
      */
      void Realocate(tObject ** objects, int numObjs, stMMNode * Parent,
                     int * regions);

      /**
      * This method will perform a range query. The result will be a set of
      * pairs object/distance.
      *
      * @param sample The sample object.
      * @param range The range of the results.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * RangeQuery(tObject * sample, double range);

      /**
      * Support for the RangeQuery, recursive code for searching.
      * @param sample The sample object.
      * @param range The range of the results.
      * @param result The result object to add the pairs <object, distance>
      *  found
      * @param page The page (node) to search in.
      */
      void RangeQuery(tObject * sample, double range, tResult * result,
                      u_int32_t page);
      
      /**
      * This method will perform a k nearest neighbor query.
      *
      * @param sample The sample object.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * NearestQuery(tObject * sample, u_int32_t k, bool tie = false);

      /**
      * Support for the NearestQuery, recursive code for searching.
      * @param sample The sample object.
      * @param k The number of nearest neighbors to retrieve.
      * @param result The result object to add the pairs <object, distance> found
      * @param page The page (node) to search in.
      */
      void NearestQuery(tObject * sample, u_int32_t k, tResult * result,
                        u_int32_t page, bool tie);

   private:
      /**
      * This type defines the header of the MMTree.
      */
      typedef struct MMTreeHeader{
         /**
         * The root.
         */
         u_int32_t Root;

         /**
         * The Nearest Neighbor Query Method.
         */
         int NearestMethod;

         /**
         * The Insert Method (changing this method the tree will try to balance).
         */
         int InsertionMethod;

         /**
         * The number of the objects
         */
         u_int32_t ObjectCount;

         /**
         * The number of the nodes
         */
         u_int32_t NodeCount;
      } stMMTreeHeader;

      /**
      * The header page. It will be kept in memory all the time to avoid
      * reads.
      */
      stPage * HeaderPage;

      /**
      * The header of the "tree".
      */
      stMMTreeHeader * Header;

      /**
      * ...
      */
      double distances[NUMBEROFUNKNOWN][NUMBEROFUNKNOWN];

      /**
      * Creates the header for an empty tree.
      */
      void Create(){
         LoadHeader();
         Header->Root = 0;
         Header->ObjectCount = 0;
         Header->NodeCount = 0;
         Header->NearestMethod = 0;
         Header->InsertionMethod = 0;
         WriteHeader();
      }//end Create

      /**
      * Loads the header from the page manager.
      */
      void LoadHeader(){
         HeaderPage = this->myPageManager->GetHeaderPage();
         Header = (stMMTreeHeader *) HeaderPage->GetData();
      }//end LoadHeader

      /**
      * Writes the header into the Page Manager.
      */
      void WriteHeader(){
         this->myPageManager->WriteHeaderPage(HeaderPage);
      }//end WriteHeader
      
};//end stMMTree

#include <arboretum/stMMTree-inl.h>
#endif //__STMMTREE_H
