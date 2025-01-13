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

#ifndef __STGNUPLOT_H
#define __STGNUPLOT_H

class stGnuplot {
   public:

 	  enum tOutputPlot{
	     plotWindows,
	     plotX11,
	     plotEps,
        plotPs
     };

      stGnuplot() {
         fontsize = -1;
         legend = true;
         xlabel = NULL;
         title = NULL;
      };

      virtual ~stGnuplot();

      void SetFontSize(int size) { fontsize = size; };

      void SetTitle(const char * str);

      void SetXLabel(const char * label);

      void SetLegend(bool leg = true) { legend = leg; };

      int GetFontSize() { return fontsize; };

      char * GetTitle() { return title; };

      char * GetXLabel() { return xlabel; };

      bool GetLegend() { return legend; };

      virtual void SaveTo(char * filename, tOutputPlot type = plotWindows) = 0;

   private:
      char * xlabel;
      char * title;
      int fontsize;
      bool legend;
};

#endif // __STGNUPLOT_H
