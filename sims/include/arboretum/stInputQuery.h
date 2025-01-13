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
* This file defines the classes stInputQuery and stInputQueryPair.
*
* @version 1.0
* $Revision: 1.6 $
* $Date: 2005-03-08 19:43:09 $
* $Author: marcos $
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#ifndef __STINPUTQUERY_H
#define __STINPUTQUERY_H

#include <vector>


//----------------------------------------------------------------------------
// Class template stInputQueryPair
//----------------------------------------------------------------------------
/**
* This class defines the pair Object/Distance returned as the result of a query.
*
* @author Adriano Siqueira Arantes (arantes@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @version 1.0
* @ingroup struct
*/
template < class ObjectType >
class stInputQueryPair{
   public:

      /**
      * Type of the object.
      */
      typedef ObjectType tObject;

      /**
      * Creates a new pair Object/Distance.
      *
      * <P>This instance will claim the ownership of the object. In other words,
      * it will dispose the object when it is no loger necessary.
      *
      * @param object The object.
      * @param radius The radius of the query object.
      * @param distance The distance from the representative query.
      */
      stInputQueryPair (tObject * object, double radius,
                        double distance = 0.0){
         this->Object = object;
         this->Radius = radius;
         this->Distance = distance;
      }//end stInputQueryPair

      /**
      * Disposes this instance and release the associated objects.
      */
      ~stInputQueryPair(){

         if (Object != NULL){
            delete this->Object;
         }//end if
      }//end ~stInputQueryPair

      /**
      * This method returns the object query.
      */
      tObject * GetObject(){
         return this->Object;
      }//end GetObject

      /**
      * This method gets the Radius of this entry.
      */
      double GetRadius(){
         return this->Radius;
      }//end GetRadius

      /**
      * This method gets the distance of this entry from the
      * representative query.
      */
      double GetDistance(){
         return this->Distance;
      }//end GetDistance

      /**
      * This method sets the distance of this entry from the
      * representative query.
      */
      void SetDistance(double distance){
         this->Distance = distance;
      }//end SetDistance

   private:

      /**
      * The object query.
      */
      tObject * Object;

      /**
      * The distance from the representative query.
      */
      double Distance;

      /**
      * The radius of this query object.
      */
      double Radius;
};//end stInputQueryPair

//----------------------------------------------------------------------------
// Class template stInputQuery
//----------------------------------------------------------------------------
/**
* This class implements a query input. It will hold a set of pairs
* Object/Radius/Distance.
*
* @author Adriano Siqueira Arantes (arantes@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @version 1.0
* @ingroup struct
*/
template < class ObjectType, class EvaluatorType>
class stInputQuery{
   public:

      /**
      * Type of the object.
      */
      typedef ObjectType tObject;

      /**
      * This type defines the stInputQueryPair Pair used by this class.
      */
      typedef stInputQueryPair < ObjectType > tPair;

      /**
      * This method will create a new instance of this class. The parameter hint
      * is used to prepare this instance to hold at least <i>hint</i> inputs
      * (it is not a upper bound limit).
      *
      * @param hint The projected number of inputs (default = 1).
      */
      stInputQuery(int hint = 1){
         // Reserve results
         Pairs.reserve(hint);
      }//end stInputQuery

      /**
      * This method disposes this instance and releases all allocated resources.
      */
      virtual ~stInputQuery();

      /**
      * This operator allow the access to a pair.
      */
      tPair & operator [] (int idx){
         return (*Pairs[idx]);
      }//end operator []

      /**
      * This method sets the EvaluatorType of this input query.
      * This method do not do a copy.
      * @param evaluator The EvaluatorType.
      */
      void BorrowMetricEvaluator(EvaluatorType * evaluator){
         Evaluator = evaluator;
      }//end BorrowMetricEvaluator

      /**
      * This method release the evaluator of this input query.
      */
      void ReleaseMetricEvaluator(){
         Evaluator = NULL;
      }//end ReleaseMetricEvaluator

      /**
      * This method returns the number of entries in this input query.
      */
      int GetNumOfEntries(){
         return Pairs.size();
      }//end GetNumOfEntries

      /**
      * This method get the index of the representative object.
      */
      u_int32_t GetRepresentativeIndex(){
         return RepIndex;
      }//end GetRepresentativeIndex

      /**
      * This method sets the index of the representative object.
      *
      * @param repIndex the index of the representative object.
      */
      void SetRepresentativeIndex(u_int32_t repIndex){
         RepIndex = repIndex;
      }//end SetRepresentativeIndex

      /**
      * This method adds a pair Object/Distance to this result list.
      *
      * @param object The object.
      * @param distance The distance from the .
      * @warning There is no duplicate pair checking. All pairs will be added.
      */
      void AddPair(tObject * object, double radius){
         Pairs.insert(Pairs.end(), new tPair(object, radius));
      }//end AddPair

      /**
      * This method returns the distance of this object from the representative
      * object.
      * If this result is empty, it will return a negative value.
      *
      * @param idx the index of the object.
      */
      double GetDistance(int idx){

         if (Pairs.size() > 0){
            return Pairs[Pairs.begin() + idx]->GetDistance();
         }else{
            return -1;
         }//end if
      }//end GetDistance

      /**
      * Sets information about the query radius.
      *
      * @param queryRadius the value of the new radius query.
      */
      void SetQueryRadius(double queryRadius){
         QueryRadius = queryRadius;
      }//end SetQueryRadius

      /**
      * Gets information about the query radius.
      * Returns 0 if it is not defined yet!
      */
      double GetQueryRadius(){
         return QueryRadius;
      }//end GetQueryRadius

      /**
      * This method choose a new representative object for this query.
      * The representative is the center object that generate the minimum
      * cover radius.
      *
      * @return the index of the new representative object.
      */
      u_int32_t ChooseRepresentative();

      /**
      * This method update the distance of the object from the new
      * representative object.
      *
      * @param repObj the represetative object.
      * @param repObjIdx the index of the represetative object.
      */
      void UpdateDistances(ObjectType * repObj,
                           u_int32_t repObjIdx);

      /**
      * This method returns the minimum radius query of this query.
      */
      double GetMinimumRadius();

      /**
      * This method ...
      */
      u_int32_t ChooseRepresentativeSmallRadius();
      
   private:

      /**
      * The vector of pairs.
      */
      vector < tPair * > Pairs;

      /**
      * Information about ...
      */
      u_int32_t RepIndex;

      /**
      * Information about radius for the inputQuery.
      */
      double QueryRadius;

      /**
      * The evaluator type. It is used to evaluate the distances.
      */
      EvaluatorType * Evaluator;

};//end stInputQuery

#include <arboretum/stInputQuery-inl.h>

#endif //__STINPUTQUERY
