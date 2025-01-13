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
* This file defines some useful struct layer utilities.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#ifndef __STSTRUCTUTILS_H
#define __STSTRUCTUTILS_H


/**
* Distance cache. It may be used by split algorithms to avoid
* unnecessary distance computations.
*
* <p>This class implements a sparse matrix, storing only the left side
* of the matrix. Because of that, it will allocate only
* @f$ {{(1 + n) * n} \over {2}} - 1 @f$ positions rather than @f$ n^2 @f$
* positions required by a standard matrix.
*
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
class doubleCache{
   public:
      /**
      * Creates a new distance cache.
      *
      * @param n Number of entries.
      */
      doubleCache(int n);
      
      /**
      * Disposes this instance and releases all associated resources.
      */
      virtual ~doubleCache(){
         delete[] distances;
      }//end ~doubleCache
   
      /**
      * Returns the distance between object i and j.
      *
      * @param i Object i.
      * @param j Object j.
      * @return The distance between the objects or -1 if it is not set.
      */
      double GetDistance(int i, int j){
         if (i < j){
            return distances[GetOffset(i, j)];
         }else if (i > j){
            return distances[GetOffset(j, i)];
         }else{
            return 0;
         }//end if           
      }//end GetDistance
      
      /**
      * Sets the distance between objects i and j.
      *
      * <p>It is possible to set all positions of the matrix. However,
      * due to optimization issues, it is not required to set the distances
      * when:
      *  - i == j [always 0]
      *  - (j, i) if you set (i, j) [reflexive].
      *
      * @param i Object i.
      * @param j Object j.
      * @param d The distance to be set.
      */
      void SetDistance(int i, int j, double d){
         if (i < j){
            distances[GetOffset(i, j)] = d;
         }else if (i > j){
            distances[GetOffset(j, i)] = d;
         //}else{
            // Do nothing
         }//end if           
      }//end SetDistance
   private:
      /**
      * Distance cache.
      */
      double * distances;
      
      /**
      * Returns the offset of the given position. The value of
      * i must be always greater then j.
      *
      * @param i First id
      * @param j Last id.
      */
      int GetOffset(int i, int j){
         return (((1 + i) * i) >> 1) + j - 1;
      }//end GetOffset
};//end doubleCache

#endif //__STSTRUCTUTILS_H
