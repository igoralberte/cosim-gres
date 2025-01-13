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
* This file defines the abstract class stPageServer.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#ifndef __STPAGESERVER_H
#define __STPAGESERVER_H

#include  <arboretum/stPage.h>

/**
* This class defines the abstract class stPageServer. A page server is a
* low-level block device interface that is used by stClientPageManager.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @see stClientPageManager
* @ingroup storage
*/
class stPageServer{
   public:
      /**
      * Reads a page from a page server.
      *
      * @param pageid The id of the required page.
      * @return The given page.
      */
      virtual stPage * GetPage(int clientID, u_int32_t pageid) = 0;

      /**
      * Writes a page into a page server.
      *
      * @param page The page.
      */
      virtual void WritePage(int clientID, stPage * page) = 0;

      /**
      * Creates a new page.
      *
      * @return The new page.
      */
      virtual stPage * GetNewPage(int clientID) = 0;

      /**
      * Disposes a page.
      *
      * @param page The page.
      */
      virtual void DisposePage(int clientID, stPage * page) = 0;

      /**
      * Releases this page.
      *
      * @param page The page.
      */
      virtual void ReleasePage(int clientID, stPage * page) = 0;

      virtual u_int32_t GetHeaderPageID(int clientID);

      virtual u_int32_t GetPageCount(int clientID);

      virtual u_int32_t GetPageSize();
};//end stPageServer

#endif //__STPAGESERVER_H
