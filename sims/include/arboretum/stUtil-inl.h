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
* This file has the implementaiton of all template utility classes.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
// Copyright (c) 2002-2003 GBDI-ICMC-USP

//----------------------------------------------------------------------------
// Class template stGenericMatrix
//----------------------------------------------------------------------------
template < class Type >
void stGenericMatrix<Type>::SetSize(int cols, int rows){
   int i;
   int newSize;

   // New data size
   newSize = cols * rows;

   // Reallocate Data if required
   if (DataSize < newSize){
      if (Data != NULL){
         delete[] Data;
      }//end if
      Data = new Type[cols * rows];
      DataSize = newSize;
   }//end if

   if (PRowsSize < rows){
      if (PRows != NULL){
         delete[] PRows;
      }//end if
      PRows = new Type * [rows];
      PRowsSize = rows;
   }//end if

   Rows = rows;
   Cols = cols;
   PRows[0] = Data;
   for (i = 1; i < rows; i++){
      PRows[i] = PRows[i - 1] + cols;
   }//end for
}//end stGenericMatrix<Type>::SetSize
   
//----------------------------------------------------------------------------
// Class template stGenericMatrix
//----------------------------------------------------------------------------
template < class Type >
stHugeGenericMatrix<Type>::~stHugeGenericMatrix(){

   DisposeRows();
}//end stHugeGenericMatrix<Type>::~stHugeGenericMatrix

//----------------------------------------------------------------------------
template < class Type >
void stHugeGenericMatrix<Type>::SetSize(int cols, int rows){
   int i;

   if ((cols != Cols) || (rows != Rows)){
      // Kill previows matrix
      DisposeRows();

      Rows = rows;
      Cols = cols;
      PRows = new Type * [rows];
      for (i = 0; i < rows; i++){
         PRows[i] = new Type[cols];
      }//end for
   //}else{
      // Do nothing. He he he...
   }//end if
}//end stGenericMatrix<Type>::SetSize

//----------------------------------------------------------------------------
template < class Type >
void stHugeGenericMatrix<Type>::DisposeRows(){
   Type ** i;

   if (PRows != NULL){
      for (i = PRows; i < PRows + Rows; i++){
         delete[] *i;
      }//end for
      delete[] PRows;
   }//end if
}//end stGenericMatrix<Type>::DisposeRows

//----------------------------------------------------------------------------
// template class stRPriorityQueue
//----------------------------------------------------------------------------
template < class TKey, class TValue >
bool stRPriorityQueue < TKey, TValue >::Get(TKey & key, TValue & value){
   int child;
   int parent;
   
   if (size > 0){
      // Remove first
      key = entries[0].key;
      value = entries[0].value;
      size --;
      
      // Reinsert last.
      parent = 0;
      child = GetMinChild(parent);
      // Do not enable complete boolena evaluation or it will fail.
      while ((child >= 0) && (entries[size].key > entries[child].key)){
         // Move child up
         entries[parent].key = entries[child].key; 
         entries[parent].value = entries[child].value;
         parent = child;
         child = GetMinChild(parent);
      }//end while
      // Put it in place.
      entries[parent].key = entries[size].key; 
      entries[parent].value = entries[size].value;          
      return true;
   }else{
      // Empty!
      return false;
   }//end if         
}//end stRPriorityQueue::Get

//----------------------------------------------------------------------------
template < class TKey, class TValue >
void stRPriorityQueue < TKey, TValue >::Add(const TKey & key, const TValue & value){
   int child;
   int parent;
   
   // No error checking when __stDEBUG__ is not defined!
   #ifdef __stDEBUG__
   if (size < maxSize){
   #endif //__stDEBUG__    
      child = size;
      parent = (child - 1) / 2;
      // Do not enable complete boolena evaluation or it will fail.
      while ((child > 0) && (entries[parent].key > key)){
         // Move parent down.
         entries[child].key = entries[parent].key; 
         entries[child].value = entries[parent].value;
         // Next...
         child = parent;
         parent = (child - 1) / 2;
      }//end while
      
      // Add in the proper position
      entries[child].key = key;
      entries[child].value = value;
      size++;
   #ifdef __stDEBUG__
   }else{
      throw logic_error("Unable to add more entries.");
   }//end if         
   #endif //__stDEBUG__
}//end stRPriorityQueue::Add
      
//----------------------------------------------------------------------------
// template class stDynamicRPriorityQueue
//----------------------------------------------------------------------------
template < class TKey, class TValue >
bool stDynamicRPriorityQueue < TKey, TValue>::Get(
   TKey & key, TValue & value){
   int child;
   int parent;
   
   if (size > 0){
      // Remove first
      key = entries[0].key;
      value = entries[0].value;
      size--;
      
      // Reinsert last.
      parent = 0;
      child = GetMinChild(parent);
      // Do not enable complete boolena evaluation or it will fail.
      while ((child >= 0) && (entries[size].key > entries[child].key)){
         // Move child up
         entries[parent].key = entries[child].key; 
         entries[parent].value = entries[child].value;
         parent = child;
         child = GetMinChild(parent);
      }//end while
      // Put it in place.
      entries[parent].key = entries[size].key; 
      entries[parent].value = entries[size].value;
      return true;
   }else{
      // Empty!
      return false;
   }//end if         
}//end stDynamicRPriorityQueue::Get

//----------------------------------------------------------------------------
template < class TKey, class TValue >
void stDynamicRPriorityQueue < TKey, TValue>::Add(
   const TKey & key, const TValue & value){
   int child;
   int parent;

   // Resize me if required.
   if (size == maxSize){
      Resize();
   }//end if
   
   child = size;
   parent = (child - 1) / 2;
   // Do not enable complete boolena evaluation or it will fail.
   while ((child > 0) && (entries[parent].key > key)){
      // Move parent down.
      entries[child].key = entries[parent].key; 
      entries[child].value = entries[parent].value;
      // Next...
      child = parent;
      parent = (child - 1) / 2;
   }//end while
   
   // Add in the proper position
   entries[child].key = key;
   entries[child].value = value;
   size++;
}//end stDynamicRPriorityQueue::Add

//----------------------------------------------------------------------------
template < class TKey, class TValue >
void stDynamicRPriorityQueue < TKey, TValue >::Resize(){
   tEntry * newEntries;
   
   // New entry vector
   newEntries = new tEntry[maxSize + increment];
   memcpy(newEntries, entries, sizeof(tEntry) * size);
   
   // Delete old vector
   delete[] entries;
   entries = newEntries;
   this->maxSize += increment;
}//end stDynamicRPriorityQueue::Resize

//----------------------------------------------------------------------------
// template class stDynamicRReversedPriorityQueue
//----------------------------------------------------------------------------
template < class TKey, class TValue >
bool stDynamicRReversedPriorityQueue < TKey, TValue>::Get(
   TKey & key, TValue & value){
   int child;
   int parent;
   
   if (size > 0){
      // Remove first
      key = entries[0].key;
      value = entries[0].value;
      size--;
      
      // Reinsert last.
      parent = 0;
      child = GetMinChild(parent);
      // Do not enable complete boolena evaluation or it will fail.
      while ((child >= 0) && (entries[size].key < entries[child].key)){
         // Move child up
         entries[parent].key = entries[child].key;
         entries[parent].value = entries[child].value;
         parent = child;
         child = GetMinChild(parent);
      }//end while
      // Put it in place.
      entries[parent].key = entries[size].key; 
      entries[parent].value = entries[size].value;
      return true;
   }else{
      // Empty!
      return false;
   }//end if         
}//end stDynamicRReversedPriorityQueue::Get

//----------------------------------------------------------------------------
template < class TKey, class TValue >
void stDynamicRReversedPriorityQueue < TKey, TValue>::Add(
   const TKey & key, const TValue & value){
   int child;
   int parent;

   // Resize me if required.
   if (size == maxSize){
      Resize();
   }//end if
   
   child = size;
   parent = (child - 1) / 2;
   // Do not enable complete boolena evaluation or it will fail.
   while ((child > 0) && (entries[parent].key < key)){
      // Move parent down.
      entries[child].key = entries[parent].key; 
      entries[child].value = entries[parent].value;
      // Next...
      child = parent;
      parent = (child - 1) / 2;
   }//end while
   
   // Add in the proper position
   entries[child].key = key;
   entries[child].value = value;
   size++;
}//end stDynamicRReversedPriorityQueue::Add

//----------------------------------------------------------------------------
template < class TKey, class TValue >
void stDynamicRReversedPriorityQueue < TKey, TValue >::Resize(){
   tEntry * newEntries;
   
   // New entry vector
   newEntries = new tEntry[maxSize + increment];
   memcpy(newEntries, entries, sizeof(tEntry) * size);
   
   // Delete old vector
   delete[] entries;
   entries = newEntries;
   this->maxSize += increment;
}//end stDynamicRReversedPriorityQueue::Resize

//----------------------------------------------------------------------------
// Class template stInstanceCache
//----------------------------------------------------------------------------
template < class ClassType, class AllocatorType>
stInstanceCache<ClassType, AllocatorType>::~stInstanceCache(){
   int i;

   for (i = 0; i < size; i++){
      allocator->Dispose(instances[i]);
	  instances[i] = 0;
   }//end for

   delete[] instances;
   instances = 0;
   delete allocator;
   allocator = 0;
}//end stInstanceCache< class ClassType, class AllocatorType>::~stInstanceCache()
