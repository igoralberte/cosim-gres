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
* This file defines the class stStorageGrid.
*
* @version 2.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
*
* @ingroup fastmap
*/
#ifndef __STPOINTSET_H
#define __STPOINTSET_H

#include <assert.h>

//----------------------------------------------------------------------------
// class stPointSet
//----------------------------------------------------------------------------
/**
* This class is used to store the points from the database.
*
* @version 2.0
* @author Caetano Traina Jr (caetano@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Elaine Parros Machado de Sousa (parros@icmc.usp.br)
* @author Ana Carolina Riekstin (anacarol@grad.icmc.usp.br)
* @ingroup fastmap
*/
//---------------------------------------------------------------------------
class stPointSet {
   
   public:
   
      /**
      * Creates a new set of NumberOfPoints points
      *
      * @param stNumberOfPoints Number of points in the dataset.
      */
      stPointSet(int NumberOfPointsP);

      /**
      * Creates a new set of NumberOfPoints points copying it.
      */
      stPointSet(const stPointSet &);

      /**
      * Disposes this set of points
      */
      ~stPointSet();

      /**
      * Insert a set of point in the end.
      *
      * @param stPointSet.
      * @return stPointSet.
      */
      stPointSet & operator += (const stPointSet &);

      /**
      * Remove a set of points from the end.
      *
      * @param stPointSet.
      * @return stPointSet.
      */
      stPointSet & operator -= (const stPointSet & );

      /**
      * Copy the set of points.
      *
      * @param stPointSet.
      * @return stPointSet.
      */
      stPointSet & operator = (const stPointSet & );

      /**
      * Clear the point set.
      */
      void Clear();

      /**
      * Vector that will store the values of log(R) - R are the values of r
      * (the lenght of the side of a grid cell) to plot the graph to find out
      * de intrinsic dimension.
      */
      double * logR;

      /**
      * Vector that will store the values of log(SqR) - SqR is the sum of
      * squared occupancy for the grid of cell side r.
      */
      double * logSqR;

      /**
      * Value of Alpha, that is the Slope of the result line of the graph.
      */
      double Alpha;

      /**
      * Value of Beta, that is the Y increment of the result line of the graph.
      ,0

      */
      double Beta;

      /**
      * Value of the fitting error.
      */
      double error;

      /**
      * Index of the first point of the set.
      */
      int firstPoint;

      /**
      * Index of the last point of the set.
      */
      int lastPoint;

      /**
      * Current number of points in the set.
      */
      int count;

      /**
      * Maximum number of points in the set.
      */
      int maxCount;

};//end stPointSet
//---------------------------------------------------------------------------
#endif //__STPOINTSET_H 
