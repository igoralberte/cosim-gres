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

//----------------------------------------------------------------------------
// class stListPriorityQueue
//----------------------------------------------------------------------------
/**
* This class defines node type of stListPriorityQueue.
*
*
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @version 1.0
* @ingroup util
* @todo Documentation update.
*/
#include <arboretum/stListPriorityQueue.h>

//------------------------------------------------------------------------------
stListPriorityQueue::~stListPriorityQueue(){
   stEntry * temp = head;
   while(head){
      head = head->GetNext();
      delete temp;
      temp = head;
   }//end while
}//end ~stListPriorityQueue

//------------------------------------------------------------------------------
void stListPriorityQueue::Add(u_int32_t pageID, double distance, double radius){
    stEntry * newEntry;
    stEntry * priorEntry = NULL;
    stEntry * currEntry = head;

    while ((currEntry!=NULL) && (distance > currEntry->GetDistance())){
       priorEntry = currEntry;
       currEntry = currEntry->GetNext();
    }//end while

    newEntry = new stEntry();
    newEntry->SetPageID(pageID);
    newEntry->SetDistance(distance);
    newEntry->SetRadius(radius);
    newEntry->SetNext(currEntry);
    capacity++;

    if (priorEntry==NULL){
       head = newEntry;
    }else{
       priorEntry->SetNext(newEntry);
    }//end if

}//end stListPriorityQueue::Add

//------------------------------------------------------------------------------
stEntry * stListPriorityQueue::Get(){
   stEntry * returnEntry = head;
   if(head){
      head = head->GetNext();
      capacity--;
   }//end if
   return returnEntry;
}//end stListPriorityQueue::Get
