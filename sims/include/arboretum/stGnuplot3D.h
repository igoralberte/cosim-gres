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
* This file defines the class stGHTree.
* $Author: seraphim $
*
* @author Enzo Seraphim (seraphim@unifei.edu.br)
*/

#ifndef __STGNUPLOT3D_H
#define __STGNUPLOT3D_H


#include <stdexcept>
#include <arboretum/stGnuplot.h>

class stGnuplot3D: public stGnuplot {
   public:
   
      /**
      *
      */
      stGnuplot3D(int xvaluesCount, int barsCount);

      virtual ~stGnuplot3D();

      void SetYLabel(const char * label);

      void SetYRange(const char * label);

      void SetLogScaleX(bool log = true) { xlogscale = log; };
      void SetLogScaleY(bool log = true) { ylogscale = log; };
      void SetLogScaleXY(bool log = true) { xlogscale = log; ylogscale = log; };

      void SetBarTexture(int bar, int texture) {
         #ifdef __stDEBUG__
         if ((bar < 0) && (bar >= bars)){
            throw range_error("bar is out of range.");
         }//end if
         #endif //__stDEBUG__
         barsTextures[bar] = texture;
      };

      void SetBarLabel(int pos, const char * label);

      void SetXValueLabel(int pos, const char * label);

      void SetValue(int pos, int bar, long double value) {
   	   #ifdef __stDEBUG__
         if ((pos < 0) && (pos >= xvalues) && (bar < 0) && (bar >= bars)){
            throw range_error("pos or bar are out of range.");
         }//end if
         #endif //__stDEBUG__
         xrealvalues[pos][bar] = value;
      };

      long double GetValue(int pos, int bar) {
         #ifdef __stDEBUG__
         if ((pos < 0) && (pos >= xvalues) && (bar < 0) && (bar >= bars)){
            throw range_error("pos or bar are out of range.");
         }//end if
         #endif //__stDEBUG__
         return xrealvalues[pos][bar];
      };

      void SaveTo(char * filename, tOutputPlot type = plotWindows);

   private:
      bool xlogscale;
      bool ylogscale;
      int xvalues;
      int bars;
      char ** barsLabels;
      char ** xvaluesLabels;
      char * ylabel;
      char * yrange;
      int * barsTextures;
      long double **xrealvalues;
};

#endif // __STGNUPLOT3D_H
