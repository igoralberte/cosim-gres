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
//---------------------------------------------------------------------------
// stHistogram.h ...
//
// Author: Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
//---------------------------------------------------------------------------
#ifndef stLevelDiskAccessH
#define stLevelDiskAccessH

//---------------------------------------------------------------------------
// class stHistogram
//---------------------------------------------------------------------------
class stLevelDiskAccess {

   public:

      /**
      * Creates a new instance of this class.
      */
      stLevelDiskAccess(unsigned int height);

      /**
      * Destruct of this class.
      */
      ~stLevelDiskAccess();

      int GetHeight(){
         return Height;
      }//end GetHeight

      void AddEntry(double radius, unsigned int height);

      double GetAvgRadius(unsigned int idx);

      long GetNumberOfNodes(unsigned int idx);

      void AddNumberOfNodes(unsigned int height);

      void Sumarize();

   private:

      double * Radius;

      unsigned int * Nodes;

      unsigned int * Entries;

      unsigned int Height;

};//end stLevelDiskAccess

#endif //end stLevelDiskAccessH
