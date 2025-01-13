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
* This file defines the class stMetricAccessMethod.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#ifndef __STMETRICACCESSMETHOD_H
#define __STMETRICACCESSMETHOD_H

#include <arboretum/stResult.h>
#include <arboretum/stUtil.h>
#include <stdexcept>
#include <arboretum/stQueryHint.h>
#include <arboretum/stTreeInformation.h>


//-----------------------------------------------------------------------------
// Class template stMetricAccess
//-----------------------------------------------------------------------------
/**
* This abstract class defines the basic interface used by all metric access
* methods implemented by this library.
*
* <P>Developers must use this class as the base class for all MAM
* implementations. Memory MAM must extend this class directly but disk
* MAM must extend stMetricTree instead.
*
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @version 1.0
* @since Release 2
* @ingroup struct
* @see stMetricTree
*/
template <class ObjectType, class EvaluatorType>
class stMetricAccessMethod{

   public:

      /**
      * This is the class that abstracts the object used by this metric tree.
      */
      typedef ObjectType tObject;

      /**
      * This is the class that abstracts the metric evaluator used by
      * this metric tree.
      */
      typedef EvaluatorType tMetricEvaluator;

      /**
      * This is the class that abstracs an result set for simple queries.
      */
      typedef stResult <ObjectType> tResult;

      /**
      * This is the class that abstracs an result set for joined queries.
      */
      typedef stJoinedResult <ObjectType> tJoinedResult;

      /**
      * Disposes this instance and release all associated resources. The
      * page manager will never be disposed by this destructor.
      */
      virtual ~stMetricAccessMethod(){
      }//end ~stMetricAccessMethod

      /**
      * This method adds an object to the metric tree.
      * <P>The object pointed by <b>obj</b> will not be destroyed by this
      * method.
      *
      * @param obj The object to be added.
      * @return True for success or false otherwise.
      * @exception std::logic_error If this method is not supported
      * by this tree.
      */
      virtual bool Add(tObject * obj){
         throw std::logic_error("Unsupported method! Contact the tree \
               author for more details.");
      }//end Add

      /**
      * Returns the number of objetcs of this metric access method.
      *
      * @return The number of objects indexed by the tree or -1 if this
      * information is not available.
      * @exception std::logic_error If this method is not supported
      * by this tree.
      */
      virtual long GetNumberOfObjects(){
         throw std::logic_error("Unsupported method! Contact the tree \
               author for more details.");
      }//end GetNumberOfObjects

      /**
      * This method will perform a range query. The result will be a set of
      * pairs object/distance.
      *
      * <P>The object pointed by <b>sample</b> will not be destroyed by this
      * method.
      *
      * @param sample The sample object.
      * @param range The range of the results.
      * @return The result.
      * @warning The instance of tResult returned must be destroied by user.
      * @exception std::logic_error If this method is not supported
      * by this tree.
      */
      virtual tResult * RangeQuery(tObject * sample, double range){
         throw std::logic_error("Unsupported method! Contact the tree \
               author for more details.");
      }//end RangeQuery

      /**
      * This method will perform a k nearest neighbour query. It is a new
      * version that do a range query with a dynamic radius.
      *
      * <P>The object pointed by <b>sample</b> will not be destroyed by this
      * method.
      *
      * @param sample The sample object.
      * @param k The number of neighbours.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * NearestQuery(tObject * sample, u_int32_t k,
            bool tie = false){
         throw std::logic_error("Unsupported method! Contact the tree \
               author for more details.");
      }//end NearestQuery

      /**
      * This method will return the object in the tree that has the distance 0
      * to the query object. In other words, the query object itself.
      *
      * <P>If there are more elements that has distance 0 to the query
      * object, this method will return the first found.
      *
      * <P>The object pointed by <b>sample</b> will not be destroyed by this
      * method.
      *
      * @param sample The sample object.
      * @return The result or NULL if this method is not implemented.
      * @warning This method return only one object that has distance 0 to the
      * query object.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * PointQuery(tObject * sample){
         throw std::logic_error("Unsupported method! Contact the tree \
               author for more details.");
      }//end PointQuery

      /**
      * This method will perform a range query with a limited number of results.
      *
      * <P>This query is a combination of the standard range query and the
      * standard k-nearest neighbour query. All objects which matches both
      * conditions will be included in the result.
      *
      * <P>The object pointed by <b>sample</b> will not be destroyed by this
      * method.
      *
      * @param sample The sample object.
      * @param range The range of the results.
      * @param k The maximum number of results.
      * @param tie The tie list. Default false.
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * KAndRangeQuery(tObject * sample, double range,
            u_int32_t k, bool tie = false){
         throw std::logic_error("Unsupported method! Contact the tree \
               author for more details.");
      }//end KAndRangeQuery

      /**
      * This method will perform range query with a limited number of results.
      *
      * <P>This query is a combination of the standard range query and the
      * standard k-nearest neighbor query. All objects which matches with one
      * of two conditions will be included in the result.
      *
      * @param sample The sample object.
      * @param range The range of the results.
      * @param k The maximum number of results.
      * @param tie The tie list. This parameter is optional. Default false;
      * @return The result or NULL if this method is not implemented.
      * @warning The instance of tResult returned must be destroied by user.
      */
      virtual tResult * KOrRangeQuery(tObject * sample, double range,
            u_int32_t k, bool tie = false){
         throw std::logic_error("Unsupported method! Contact the tree \
               author for more details.");
      }//end KOrRangeQuery

      /**
      * This method will perform a ring query. The result will be a set of
      * pairs object/distance.
      *
      * <P>The object pointed by <b>sample</b> will not be destroyed by this
      * method.
      *
      * @param sample The sample object.
      * @param inRange The inner range of the results.
      * @param outRange The outter range of the results.
      * @return The result.
      * @warning The instance of tResult returned must be destroied by user.
      * @warning The inRange must be less than the outRange.
      * @exception std::logic_error If this method is not supported
      * by this tree.
      */
      virtual tResult * RingQuery(tObject * sample, double inRange,
                  double outRange){
         throw std::logic_error("Unsupported method! Contact the tree \
               author for more details.");
      }//end RangeQuery

      /**
      * This method will perform a ring query. The result will be a set of
      * pairs object/distance.
      *
      * <P>The object pointed by <b>sample</b> will not be destroyed by this
      * method.
      *
      * @param sample The sample object.
      * @param inRange The inner range of the results.
      * @param outRange The outter range of the results.
      * @param k The number of nearest neighbor.
      * @param tie The tie list. This parameter is optional. Default false;
      * @return The result.
      * @warning The instance of tResult returned must be destroied by user.
      * @warning The inRange must be less than the outRange.
      * @exception std::logic_error If this method is not supported
      * by this tree.
      */
      virtual tResult * KRingQuery(tObject * sample, double inRange,
                  double outRange, u_int32_t k, bool tie = false){
         throw std::logic_error("Unsupported method! Contact the tree \
               author for more details.");
      }//end RangeKQuery

      /**
      * Deletes an object from the tree. If the structure doesn't support
      * this operation, this method will always return false.
      *
      * @param obj The object to be deleted.
      * @return True for sucess or false otherwise.
      */
      virtual bool Delete(tObject * obj){
         return false;
      }//end Delete

      /**
      * Optimizes the structure of this tree. If the structure doesn't
      * support this operation, this method will do nothing.
      */
      virtual void Optimize(){
      }//end Optimize

      /**
      * Returns the metric evaluator used by this metric tree. This method
      * may be used to modify the configuration of the metric evaluator used
      * by this tree.
      */
      EvaluatorType * GetMetricEvaluator(){
         return myMetricEvaluator;
      }//end GetMetricEvaluator
      
      /**
      * This enumeration defines the common query hint types that all
      * metric trees must implement.
      *
      * @warning These values are always between 0 and 1023. Values outside
      * this range are used by other metric trees to define their private
      * set of query hint types.
      * @todo The set of query hint types must be defined.
      */
      enum stCommonQueryHintType{
         /**
         * This query hint type forces CreateQueryHint() to return NULL.  This
         * type will be removed in future versions.
         */
         QHT_NONE = 0
      };//end stQueryHintType

      /**
      * Creates the proper query hint for a given operation type. A query hint
      * instance may be required to perform certain types of operations.
      *
      * <p>The returned instance must be disposed by the application when it
      * is no longer necessary.
      *
      * @param type The operation type.
      * @return An instance of the proper query hint of a given type for NULL
      * for fail.
      * @see stCommonQueryHintType
      * @warning Each metric tree has its own implementation for each query
      * hint type. DO NOT USE a hint created by a tree as a parameter for 
      */
      virtual stQueryHint * CreateQueryHint(int type){
         return NULL;
      }//end CreateQueryHint

   protected:
   
      /**
      * The metric evaluator used by this metric tree.
      */
      tMetricEvaluator * myMetricEvaluator;
      
      /**
      * Creates a new metric access method.
      *
      * <p>The <i>eval</i> is the metric evaluator used by this class. 
      * This class will not claim the ownership of the given evaluator
      * instance. In other words, it WILL NEVER BE DISPOSED by this class.
      *
      * @param eval The metric evaluator used by this MAM.
      */
      stMetricAccessMethod(EvaluatorType * eval){
         myMetricEvaluator = eval;
      }//end stMetricAccessMethod
};//end stMetricAccessMethod

#endif //__STMETRICACCESSMETHOD_H
