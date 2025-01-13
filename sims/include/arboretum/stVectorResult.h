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
* This file defines the classes stResult and stResults.
*
* @version 1.0
* @author Enzo Seraphim (seraphim@unifei.edu.br)
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#ifndef __STVECTORRESULT_H
#define __STVECTORRESULT_H

#include <vector>


//----------------------------------------------------------------------------
// Class template stResult
//----------------------------------------------------------------------------
/**
* This class implements a query result. It will hold a set of pairs
* Object/Distance ordered by distance which constitutes the answer
* to a query. All query methods of all metric trees implemented by
* this library will return instances of this class.
*
* <P>In nearest neigbour queries and k-range queries, the result set may contain
* more than k results. It means that the greatest distance from sample to result
* has more than 1 object. In such cases, all objects whose distance from sample
* is equal to GetMaximumDistance() constitute the draw list.
*
* <P>As an extra capability, it can store information about the query but they
* will only be available if the query method supports this feature (this is an
* optional capability). See SetQueryInfo() for more details.
*
* <P> It also performs basic operations that allows the construction of
* the result set by the query procedures.
*
* @author Enzo Seraphim (seraphim@unifei.edu.br)
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @author Adriano Siqueira Arantes (arantes@icmc.usp.br)
* @version 1.1
* @ingroup struct
*/
template < class ObjectType >
class stResult: public stBasicResult < ObjectType > {

   public:

      /**
      * Type of the object.
      */
      typedef ObjectType tObject;

      /**
      * This type defines the stResult Pair used by this class.
      */
      typedef stResultPair< ObjectType > tPair;

      /**
      * This method will create a new instance of this class. The parameter hint
      * is used to prepare this instance to hold at least <i>hint</i> results
      * (it is not a upper bound limit).
      *
      * @param hint The projected number of results (default = 1).
      */
      stResult(unsigned int hint = 1){
         // Reserve results
         Pairs.reserve(hint);
         // No info
         SetQueryInfo();
      }//end stResult

      /**
      * This method disposes this instance and releases all allocated resources.
      */
      virtual ~stResult();

      /**
      * This operator allow the access to a pair.
      */
      tPair & operator [] (unsigned int idx){
         return (*Pairs[idx]);
      }//end operator []

      /**
      * This method returns the number of entries in this result.
      */
      unsigned int GetNumOfEntries(){
         return Pairs.size();
      }//end GetNumOfEntries

      /**
      * This method adds a pair Object/Distance to this result list.
      *
      * @param obj The object.
      * @param distance The distance from the sample object.
      * @warning There is no duplicate pair checking. All pairs will be added.
      */
      void AddPair(tObject * obj, double distance);

      /**
      * This method will remove the last object from this result list.
      */
      void RemoveLast(){

         if (Pairs.size() > 0){
            if (Pairs[Pairs.size() - 1] != NULL){
               delete Pairs[Pairs.size() - 1];
            }//end if
            Pairs.pop_back();
         }//end if
      }//end RemoveLast

      /**
      * This method will remove the first object from this result list.
      */
      void RemoveFirst(){

         if (Pairs.size() > 0){
            if (Pairs[0] != NULL){
               delete Pairs[0];
            }//end if
            Pairs.erase(Pairs.begin());
         }//end if
      }//end RemoveFirst

      /**
      * This method returns the minimum distance of the objects in this result
      * list. If this result is empty, it will return a negative value.
      */
      double GetMinimumDistance(){

         if (Pairs.size() > 0){
            return Pairs[0]->GetDistance();
         }else{
            return -1;
         }//end if
      }//end GetMinimumDistance

      /**
      * This method returns the maximum distance of the objects in this result
      * list. If this result is empty, it will return a negative value.
      */
      double GetMaximumDistance(){

         if (Pairs.size() > 0){
            return Pairs[Pairs.size() - 1]->GetDistance();
         }else{
            return -1;
         }//end if
      }//end GetMaximumDistance

      /**
      * This method will cut out undesired objects. It may be used
      * by k-Nearest Neighbour queries to control the number of results.
      *
      * <P>This implementation also treat...
      *
      * @param limit The desired number of results.
      * @todo Review of this explanation.
      */
      void Cut(unsigned int limit);

      /**
      * This method will cut out undesired objects. It may be used
      * by k-Farthest Neighbour queries to control the number of results.
      *
      * <P>This implementation also treat...
      *
      * @param limit The desired number of results.
      * @todo Review of this explanation.
      */
      void CutFirst(unsigned int limit);

      /**
      * Adds information about the query. It is used by Query methods to add
      * information about the query. Since it is optional, not all results will
      * provide meaningful information about it.
      *
      * @param sample The sample object (a copy of it).
      * @param querytype The query type (UNKNOWN, RANGEQUERY, NEARESTQUERY,
      * KANDRANGEQUERY, KORRANGEQUERY, CROWNQUERY, KANDRANGEQUERYESTIMATE or
      * KORRANGEQUERYESTIMATE)
      * @param k The value of k (if it makes sence).
      * @param radius The value of radius (if it makes sence).
      * @param innerRadius The value of inner radius (if it makes sence).
      * @param tie The tie list. Default false;
      *
      * @warning It do not changes the behavior of this result.
      * @see GetQueryType()
      * @see GetK()
      * @see GetRadius()
      * @see GetSample()
      * @see GetTie()
      */
      void SetQueryInfo(tObject * sample = NULL, int querytype = UNKNOWN,
                        u_int32_t k = 0, double radius = 0.0,
                        double innerRadius = 0.0, bool tie = false){
         this->QueryType = querytype;
         this->K = k;
         this->Radius = radius;
         this->InnerRadius = innerRadius;
         Sample = sample;
         this->Tie = tie;
      }//end SetQueryInfo

      /**
      * Adds information about the query. It is used by Query methods to add
      * information about the query. Since it is optional, not all results will
      * provide meaningful information about it.
      *
      * @param sample The sample object (a copy of it).
      * @param querytype The query type (UNKNOWN, RANGEQUERY, KRANGEQUERY or
      * KNEARESTQUERY)
      * @param k The value of k (if it makes sence).
      * @param radius The value of radius (if it makes sence).
      * @param tie The tie list. Default false;
      *
      * @warning It do not changes the behavior of this result.
      * @see GetQueryType()
      * @see GetK()
      * @see GetRadius()
      * @see GetSample()
      * @see GetTie()
      */
      void SetQueryInfo(tObject * sample, int querytype,
                        u_int32_t k, double radius, bool tie){
         this->SetQueryInfo(sample, querytype, k, radius, 0.0, tie);
      }//end SetQueryInfo

      /**
      * Gets the information about query type. I may assume the values
      * UNKNOWN, RANGEQUERY, KANDRANGEQUERY, KORRANGEQUERY or KNEARESTQUERY.
      */
      int GetQueryType(){
         return this->QueryType;
      }//end GetQueryType

      /**
      * Gets information about k. It makes sense only for KANDRANGEQUERY, KORRANGEQUERY
      * and KNEARESTQUERY.
      */
      unsigned int GetK(){
         return this->K;
      }//end GetK

      /**
      * Gets information about radius. It makes sense only for RANGEQUERY and
      * KRANGEQUERY.
      */
      double GetRadius(){
         return this->Radius;
      }//end GetRadius

      /**
      * Gets information about inner radius. It makes sense only for CROWNQUERY.
      */
      double GetInnerRadius(){
         return this->InnerRadius;
      }//end GetRadius

      /**
      * Gets the sample object if it is available. Since it is an optional
      * information it may not be available.
      *
      * @return The sample object or NULL if it is not available.
      */
      tObject * GetSample(){
         return Sample;
      }//end GetSample

      /**
      * This method tests if two results are equal.
      *
      * @param r1 The second result to be test.
      * @return True if is equal, False otherwise.
      */
      bool IsEqual(const stResult * r1);

      /**
      * This method tests the similarity between two results .
      *
      * @param r1 The second result to be test.
      * @return the percent-similarity of the two results.
      */
      double Precision(const stResult * r1);


      /**
      * This method implements the intersection operator between two results.
      * @param r1 The first result set.
      * @param r2 The second result set.
      */
      void Intersection(stResult * result1, stResult * result2);

      /**
      * This method implements the union operator between two results.
      * @param r1 The first result set.
      * @param r2 The second result set.
      */
      void Union(stResult * result1, stResult * result2);

   private:

      /**
      * The vector of pairs.
      */
      vector < tPair * > Pairs;

      /**
      * Sample object.
      */
      tObject * Sample;

      /**
      * This method locates the insertion position of an object.
      *
      * @param distance The desired distance.
      * @todo This code needs optimizations. I suggest a binary search
      * implementation.
      */
      unsigned int Find(double distance);

};//end stResult

//----------------------------------------------------------------------------
// Class template stResultPaged
//----------------------------------------------------------------------------
/**
* This class implements a query result. It will hold a set of pairs
* Object/Distance ordered by distance which constitutes the answer
* to a query. All query methods of all metric trees implemented by
* this library will return instances of this class.
*
* <P>In nearest neigbour queries and k-range queries, the result set may contain
* more than k results. It means that the greatest distance from sample to result
* has more than 1 object. In such cases, all objects whose distance from sample
* is equal to GetMaximumDistance() constitute the draw list.
*
* <P>As an extra capability, it can store information about the query but they
* will only be available if the query method supports this feature (this is an
* optional capability). See SetQueryInfo() for more details.
*
* <P> It also performs basic operations that allows the construction of
* the result set by the query procedures.
*
* @author Enzo Seraphim (seraphim@unifei.edu.br)
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @author Adriano Siqueira Arantes (arantes@icmc.usp.br)
* @version 1.1
* @ingroup struct
*/
template < class ObjectType >
class stResultPaged: public stBasicResult < ObjectType > {

   public:

      /**
      * Type of the object.
      */
      typedef ObjectType tObject;

      /**
      * This type defines the stResult Pair used by this class.
      */
      typedef stResultPair< ObjectType > tPair;

      /**
      * This method will create a new instance of this class. The parameter hint
      * is used to prepare this instance to hold at least <i>hint</i> results
      * (it is not a upper bound limit).
      *
      * @param hint The projected number of results (default = 1).
      */
      stResultPaged(unsigned int hint = 1){
         // Reserve results
         Pairs.reserve(hint);
         // No info
         SetQueryInfo();
      }//end stResultPaged

      /**
      * This method disposes this instance and releases all allocated resources.
      */
      virtual ~stResultPaged();

      /**
      * This operator allow the access to a pair.
      */
      tPair & operator [] (unsigned int idx){
         return (*Pairs[idx]);
      }//end operator []

      /**
      * This method returns the number of entries in this result.
      */
      unsigned int GetNumOfEntries(){
         return Pairs.size();
      }//end GetNumOfEntries

      /**
      * This method adds a pair Object/Distance to this result list.
      *
      * @param obj The object.
      * @param distance The distance from the sample object.
      * @warning There is no duplicate pair checking. All pairs will be added.
      */
      void AddPair(tObject * obj, double distance);

      /**
      * This method will remove the last object from this result list.
      */
      void RemoveLast(){

         if (Pairs.size() > 0){
            if (Pairs[Pairs.size() - 1] != NULL){
               delete Pairs[Pairs.size() - 1];
            }//end if
            Pairs.pop_back();
         }//end if
      }//end RemoveLast

      /**
      * This method will remove the first object from this result list.
      */
      void RemoveFirst(){

         if (Pairs.size() > 0){
            if (Pairs[0] != NULL){
               delete Pairs[0];
            }//end if
            Pairs.erase(Pairs.begin());
         }//end if
      }//end RemoveFirst

      /**
      * This method returns the minimum distance of the objects in this result
      * list. If this result is empty, it will return a negative value.
      */
      double GetMinimumDistance(){

         if (Pairs.size() > 0){
            return Pairs[0]->GetDistance();
         }else{
            return -1;
         }//end if
      }//end GetMinimumDistance

      /**
      * This method returns the maximum distance of the objects in this result
      * list. If this result is empty, it will return a negative value.
      */
      double GetMaximumDistance(){

         if (Pairs.size() > 0){
            return Pairs[Pairs.size() - 1]->GetDistance();
         }else{
            return -1;
         }//end if
      }//end GetMaximumDistance

      /**
      * This method will cut out undesired objects. It may be used
      * by k-Nearest Neighbour queries to control the number of results.
      *
      * <P>This implementation also treat...
      *
      * @param limit The desired number of results.
      * @todo Review of this explanation.
      */
      void Cut(unsigned int limit);

      /**
      * This method will cut out undesired objects. It may be used
      * by k-Farthest Neighbour queries to control the number of results.
      *
      * <P>This implementation also treat...
      *
      * @param limit The desired number of results.
      * @todo Review of this explanation.
      */
      void CutFirst(unsigned int limit);

      /**
      * Adds information about the query. It is used by Query methods to add
      * information about the query. Since it is optional, not all results will
      * provide meaningful information about it.
      *
      * @param sample The sample object (a copy of it).
      * @param querytype The query type (UNKNOWN, RANGEQUERY, NEARESTQUERY,
      * KANDRANGEQUERY, KORRANGEQUERY, CROWNQUERY, KANDRANGEQUERYESTIMATE or
      * KORRANGEQUERYESTIMATE)
      * @param k The value of k (if it makes sence).
      * @param radius The value of radius (if it makes sence).
      * @param innerRadius The value of inner radius (if it makes sence).
      * @param tie The tie list. Default false;
      *
      * @warning It do not changes the behavior of this result.
      * @see GetQueryType()
      * @see GetK()
      * @see GetRadius()
      * @see GetSample()
      * @see GetTie()
      */
      void SetQueryInfo(tObject * sample = NULL, int querytype = UNKNOWN,
                        u_int32_t k = 0, double radius = 0.0,
                        double innerRadius = 0.0, bool tie = false){
         this->QueryType = querytype;
         this->K = k;
         this->Radius = radius;
         this->InnerRadius = innerRadius;
         Sample = sample;
         this->Tie = tie;
      }//end SetQueryInfo

      /**
      * Adds information about the query. It is used by Query methods to add
      * information about the query. Since it is optional, not all results will
      * provide meaningful information about it.
      *
      * @param sample The sample object (a copy of it).
      * @param querytype The query type (UNKNOWN, RANGEQUERY, KRANGEQUERY or
      * KNEARESTQUERY)
      * @param k The value of k (if it makes sence).
      * @param radius The value of radius (if it makes sence).
      * @param tie The tie list. Default false;
      *
      * @warning It do not changes the behavior of this result.
      * @see GetQueryType()
      * @see GetK()
      * @see GetRadius()
      * @see GetSample()
      * @see GetTie()
      */
      void SetQueryInfo(tObject * sample, int querytype,
                        u_int32_t k, double radius, bool tie){
         this->SetQueryInfo(sample, querytype, k, radius, 0.0, tie);
      }//end SetQueryInfo

      /**
      * Gets the information about query type. I may assume the values
      * UNKNOWN, RANGEQUERY, KANDRANGEQUERY, KORRANGEQUERY or KNEARESTQUERY.
      */
      int GetQueryType(){
         return this->QueryType;
      }//end GetQueryType

      /**
      * Gets information about k. It makes sense only for KANDRANGEQUERY, KORRANGEQUERY
      * and KNEARESTQUERY.
      */
      unsigned int GetK(){
         return this->K;
      }//end GetK

      /**
      * Gets information about radius. It makes sense only for RANGEQUERY and
      * KRANGEQUERY.
      */
      double GetRadius(){
         return this->Radius;
      }//end GetRadius

      /**
      * Gets information about inner radius. It makes sense only for CROWNQUERY.
      */
      double GetInnerRadius(){
         return this->InnerRadius;
      }//end GetRadius

      /**
      * Gets the sample object if it is available. Since it is an optional
      * information it may not be available.
      *
      * @return The sample object or NULL if it is not available.
      */
      tObject * GetSample(){
         return Sample;
      }//end GetSample

      /**
      * This method tests if two results are equal.
      *
      * @param r1 The second result to be test.
      * @return True if is equal, False otherwise.
      */
      bool IsEqual(const stResultPaged * r1);

      /**
      * This method tests the similarity between two results .
      *
      * @param r1 The second result to be test.
      * @return the percent-similarity of the two results.
      */
      double Precision(const stResultPaged * r1);


      /**
      * This method implements the intersection operator between two results.
      * @param r1 The first result set.
      * @param r2 The second result set.
      */
      void Intersection(stResultPaged * result1, stResultPaged * result2);

      /**
      * This method implements the union operator between two results.
      * @param r1 The first result set.
      * @param r2 The second result set.
      */
      void Union(stResultPaged * result1, stResultPaged * result2);

   private:

      /**
      * The vector of pairs.
      */
      vector < tPair * > Pairs;

      /**
      * Sample object.
      */
      tObject * Sample;

      /**
      * This method locates the insertion position of an object.
      *
      * @param distance The desired distance.
      * @todo This code needs optimizations. I suggest a binary search
      * implementation.
      */
      unsigned int Find(double distance);

};//end stResultPaged

//----------------------------------------------------------------------------
// Class template stJoinedResult
//----------------------------------------------------------------------------
/**
* This class implements a query result. It will hold a set of pairs
* Object/Distance ordered by distance which constitutes the answer
* to a query. All query methods of all metric trees implemented by
* this library will return instances of this class.
*
* <P>In nearest neigbour queries and k-range queries, the result set may contain
* more than k results. It means that the greatest distance from sample to result
* has more than 1 object. In such cases, all objects whose distance from sample
* is equal to GetMaximumDistance() constitute the draw list.
*
* <P>As an extra capability, it can store information about the query but they
* will only be available if the query method supports this feature (this is an
* optional capability). See SetQueryInfo() for more details.
*
* <P> It also performs basic operations that allows the construction of
* the result set by the query procedures.
*
* @author Enzo Seraphim (seraphim@unifei.edu.br)
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @author Adriano Siqueira Arantes (arantes@icmc.usp.br)
* @version 1.1
* @ingroup struct
*/
template < class ObjectType >
class stJoinedResult: public stBasicResult < ObjectType > {

   public:

      /**
      * Type of the object.
      */
      typedef ObjectType tObject;

      /**
      * This type defines the stJoinedResult Triple used by this class.
      */
      typedef stResultTriple< ObjectType > tTriple;

      /**
      * This method will create a new instance of this class. The parameter hint
      * is used to prepare this instance to hold at least <i>hint</i> results
      * (it is not a upper bound limit).
      *
      * @param hint The projected number of results (default = 1).
      */
      stJoinedResult(unsigned int hint = 1){
         // Reserve results
         Triples.reserve(hint);
         // No info
         SetQueryInfo();
      }//end stJoinedResult

      /**
      * This method disposes this instance and releases all allocated resources.
      */
      virtual ~stJoinedResult();

      /**
      * This operator allow the access to a pair.
      */
      tTriple & operator [] (unsigned int idx){
         return (*Triples[idx]);
      }//end operator []

      /**
      * This method returns the number of entries in this result.
      */
      unsigned int GetNumOfEntries(){
         return Triples.size();
      }//end GetNumOfEntries

      /**
      * This method adds a joined pair Object/JoinedObject/distance
      * to this result list.
      *
      * @param obj The object.
      * @param joinedObj The joined object.
      * @param distance The distance from the sample object.
      * @warning There is no duplicate pair checking. All pairs will be added.
      */
      void AddJoinedTriple(tObject * obj, tObject * joinedObj, double distance){
         unsigned int pos;

         pos = this->Find(distance);
         Triples.insert(Triples.begin() + pos, new tTriple(obj, joinedObj, distance));
      }//end AddJoinedPair
      
      /**
      * AddLocalResult
      *
      * @param p The pairs.
      */
      void AddLocalResult(stResult<ObjectType> * local);

      /**
      * AddLocalResult
      *
      * @param obj The object.
      * @param p The pairs.
      */
      void AddLocalResultMaxDist(stResult<ObjectType> * local);

      void AddtJoinedResult(stJoinedResult<ObjectType> * res);
      
      /**
      * This method will remove the last object from this result list.
      */
      void RemoveLast(){

         if (Triples.size() > 0){
            if (Triples[Triples.size() - 1] != NULL){
               delete Triples[Triples.size() - 1];
            }//end if
            Triples.pop_back();
         }//end if
      }//end RemoveLast

      /**
      * This method will remove the first object from this result list.
      */
      void RemoveFirst(){

         if (Triples.size() > 0){
            if (Triples[0] != NULL){
               delete Triples[0];
            }//end if
            Triples.erase(Triples.begin());
         }//end if
      }//end RemoveFirst

      /**
      * This method returns the minimum distance of the objects in this result
      * list. If this result is empty, it will return a negative value.
      */
      double GetMinimumDistance(){

         if (Triples.size() > 0){
            return Triples[0]->GetDistance();
         }else{
            return -1;
         }//end if
      }//end GetMinimumDistance

      /**
      * This method returns the maximum distance of the objects in this result
      * list. If this result is empty, it will return a negative value.
      */
      double GetMaximumDistance(){

         if (Triples.size() > 0){
            return Triples[Triples.size() - 1]->GetDistance();
         }else{
            return -1;
         }//end if
      }//end GetMaximumDistance

      /**
      * This method will cut out undesired objects. It may be used
      * by k-Nearest Neighbour queries to control the number of results.
      *
      * <P>This implementation also treat...
      *
      * @param limit The desired number of results.
      * @todo Review of this explanation.
      */
      void Cut(unsigned int limit);

      /**
      * This method will cut out undesired objects. It may be used
      * by k-Farthest Neighbour queries to control the number of results.
      *
      * <P>This implementation also treat...
      *
      * @param limit The desired number of results.
      * @todo Review of this explanation.
      */
      void CutFirst(unsigned int limit);

      /**
      * Adds information about the query. It is used by Query methods to add
      * information about the query. Since it is optional, not all results will
      * provide meaningful information about it.
      *
      * @param sample The sample object (a copy of it).
      * @param querytype The query type (UNKNOWN, RANGEQUERY, NEARESTQUERY,
      * KANDRANGEQUERY, KORRANGEQUERY, CROWNQUERY, KANDRANGEQUERYESTIMATE or
      * KORRANGEQUERYESTIMATE)
      * @param k The value of k (if it makes sence).
      * @param radius The value of radius (if it makes sence).
      * @param innerRadius The value of inner radius (if it makes sence).
      * @param tie The tie list. Default false;
      *
      * @warning It do not changes the behavior of this result.
      * @see GetQueryType()
      * @see GetK()
      * @see GetRadius()
      * @see GetSample()
      * @see GetTie()
      */
      void SetQueryInfo(int querytype = UNKNOWN,
                        u_int32_t k = 0, double radius = 0.0,
                        double innerRadius = 0.0, bool tie = false){
         this->QueryType = querytype;
         this->K = k;
         this->Radius = radius;
         this->InnerRadius = innerRadius;
         this->Tie = tie;
      }//end SetQueryInfo

      /**
      * Adds information about the query. It is used by Query methods to add
      * information about the query. Since it is optional, not all results will
      * provide meaningful information about it.
      *
      * @param sample The sample object (a copy of it).
      * @param querytype The query type (UNKNOWN, RANGEQUERY, KRANGEQUERY or
      * KNEARESTQUERY)
      * @param k The value of k (if it makes sence).
      * @param radius The value of radius (if it makes sence).
      * @param tie The tie list. Default false;
      *
      * @warning It do not changes the behavior of this result.
      * @see GetQueryType()
      * @see GetK()
      * @see GetRadius()
      * @see GetSample()
      * @see GetTie()
      */
      void SetQueryInfo(int querytype, u_int32_t k, double radius, bool tie){
         this->SetQueryInfo(querytype, k, radius, 0.0, tie);
      }//end SetQueryInfo


      /**
      * This method tests if two results are equal.
      *
      * @param r1 The second result to be test.
      * @return True if is equal, False otherwise.
      */
      bool IsEqual(const stJoinedResult * r1);

      /**
      * This method tests the similarity between two results .
      *
      * @param r1 The second result to be test.
      * @return the percent-similarity of the two results.
      */
      double Precision(const stJoinedResult * r1);


      /**
      * This method implements the intersection operator between two results.
      * @param r1 The first result set.
      * @param r2 The second result set.
      */
      void Intersection(stJoinedResult * result1, stJoinedResult * result2);

      /**
      * This method implements the union operator between two results.
      * @param r1 The first result set.
      * @param r2 The second result set.
      */
      void Union(stJoinedResult * result1, stJoinedResult * result2);

   private:

      /**
      * The vector of triples.
      */
      vector < tTriple * > Triples;

      /**
      * This method locates the insertion position of an object.
      *
      * @param distance The desired distance.
      * @todo This code needs optimizations. I suggest a binary search
      * implementation.
      */
      unsigned int Find(double distance);
      
};//end stJoinedResult


#include "stVectorResult-inl.h"

#endif //__STVECTORRESULT_H
