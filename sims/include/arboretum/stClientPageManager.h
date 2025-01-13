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
* This file defines the class stClientPageManager.
*
* @version 1.0
* $Revision: 1.4 $
* $Date: 2005-03-08 19:43:09 $
* $Author: marcos $
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#ifndef __STCLIENTPAGEMANAGER_H
#define __STCLIENTPAGEMANAGER_H

#include  <arboretum/stPageServer.h>
#include  <arboretum/stPage.h>

/**
* This class defines the class stClientPageManager. It uses a page server
* to provide access to a block device.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @see stPageServer
* @ingroup storage
* @todo Tests!
*/
class stClientPageManager: public stPageManager{
   public:
      /**
      * Creates a new instance of a client page manager. Do not call this
      * constructor if you are not a page server creator.
      *
      * @param clientID Client ID.
      * @param server The server to be used.
      */
      stClientPageManager(int clientID, stPageServer * server){
         myID = clientID;
         this->server = server;
      }//end stClientPageManager

      /**
      * This method will checks if this page manager is empty.
      * If this method returns true, the stSlimTree will create a
      * new tree otherwise it will continue to use the existing tree.
      *
      * @return True if the page manager is empty or false otherwise.
      */
      virtual bool IsEmpty(){
         return server->GetPageCount(myID) == 1;
      }//end IsEmpty

      /**
      * Returns the header page. This method will return a special
      * page that will be used by SlimTree to write some information
      * about the tree itself.
      *
      * <P>Since this page must always exist, this method will always
      * return a valid page for reading/writing. Use WritePage() to
      * write this page.
      *
      * <P>The returning instance of stPage will be locked to prevent
      * its reuse by this page manager. Use ReleasePage() to unlock
      * this instance.
      *
      * @return The header page.
      * @see WritePage()
      * @see ReleasePage()
      */
      virtual stPage * GetHeaderPage(){
         return GetPage(headerID);
      }//end GetHeaderPage

      /**
      * Returns the page with the given page ID. This method will
      * return a valid page for reading/writing. Use WritePage() to
      * write this page.
      *
      * <P>The returning instance of stPage will be locked to prevent
      * its reuse by this page manager. Use ReleasePage() to unlock
      * this instance.
      *
      * @param pageid The desired page id.
      * @return The page or NULL for an invalid page ID.
      * @see WritePage()
      * @see ReleasePage()
      */
      virtual stPage * GetPage(u_int32_t pageid){
         UpdateReadCounter();
         return server->GetPage(myID, pageid);
      }//end GetPage

      /**
      * Releases this instace for reuse by this page manager.
      * Since some implementations of page manager will reuse
      * instances of stPage to avoid unnecessary resource
      * reallocations, each page instance must be locked until
      * it becomes unecessary.
      *
      * <P>The stSlimTree will always call this method when a
      * stPage instance will not be used in a near future.
      *
      * @param page The locked page.
      * @see GetPage()
      * @see GetHeaderPage()
      */
      virtual void ReleasePage(stPage * page){
         return server->ReleasePage(myID, page);
      }//end ReleasePage

      /**
      * Allocates a new page for use. As GetPage() and
      * GetHeaderPage(), the returning instance will be
      * locked to prevent reuse by this page manager.
      *
      * <P>To dispose this page (make it free), use DisposePage().
      *
      * @return A new page or NULL for errors.
      * @see ReleasePage()
      * @see WritePage()
      * @see DisposePage()
      */
      virtual stPage * GetNewPage(){
         return server->GetNewPage(myID);
      }//end GetNewPage

      /**
      * Writes the given page to the disk. This method
      * will write the page but will not release it. Use
      * ReleasePage() to do it.
      *
      * @param page The page to be written.
      * @see ReleasePage()
      * @see WriteHeaderPage()
      */
      virtual void WritePage(stPage * page){
         server->WritePage(myID, page);
      }//end WritePage

      /**
      * Disposes the given page. This method will make the page
      * available (not allocated) for the next calls of GetNewPage().
      *
      * <P>Since this page will not be used anymore, this method will
      * release the lock for this page instance.
      *
      * @param page The page to be disposed.
      * @see GetNewPage()
      */
      virtual void DisposePage(stPage * page){
         server->DisposePage(myID, page);
      }//end DisposePage

      /**
      * Returns the minimum size of a page. The size of the header page is
      * always ignored since it may be smaller than others.
      */
      virtual u_int32_t GetMinimumPageSize(){
         return server->GetPageSize();
      }//end GetMinimumPageSize

      /**
      * Returns the client ID.
      */
      int GetClientID(){
         return myID;
      }//end GetClientID

      /**
      * Returns the server used by this client.
      */
      stPageServer * GetServer(){
         return server;
      }//end GetServer
   private:
      /**
      * The header page of this client page manager.
      */
      u_int32_t headerID;

      /**
      *
      */
      int myID;

      /**
      * Page server of this client page manager.
      */
      stPageServer * server;
};//end stPageServer

#endif //__STPAGESERVER_H
