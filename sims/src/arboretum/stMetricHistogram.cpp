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
* This file defines a generic array object that implements all methods required
* by the stObject interface. This object may be used in combination with the
* metric evaluators defined in the file stBasicMetricEvaluator.h.
*
* @version 2.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/

#include <arboretum/stMetricHistogram.h>

//==============================================================================
// Classe tMetricHistogram
//------------------------------------------------------------------------------
tMetricHistogram::tMetricHistogram(int size, tMetricHistogramBin * bins){
   Size = size;
   Bins = new tMetricHistogramBin[size];
   std::copy(bins, bins + size, Bins);
   this->tag = 0;
   
   // Serialized data
   Serialized = NULL;
}//end tMetricHistogram::tMetricHistogram

//------------------------------------------------------------------------------
tMetricHistogram::tMetricHistogram(unsigned char * SerializedData){

   unsigned char *tmp;
   Bins = NULL;
   this->tag = 0;
   this->Size = 0;
   this->Unserialize(SerializedData);
}//end tMetricHistogram::tMetricHistogram

//------------------------------------------------------------------------------
tMetricHistogram::tMetricHistogram(tMetricHistogram & mhistogram){

   Size = mhistogram.Size;
   Bins = new tMetricHistogramBin[Size];
   std::copy(mhistogram.Bins, mhistogram.Bins + Size, Bins);
   // Serialized data
   Serialized = NULL;
}//end tMetricHistogram::tMetricHistogram

//------------------------------------------------------------------------------
bool tMetricHistogram::IsEqual(tMetricHistogram * obj){
   // If the size of objects is diferent.
   if (this->GetSize() != obj->GetSize())
      return false;

   // For each bin compare the its contents.
   for (int idx=0; (idx<this->GetSize()) && (idx<obj->GetSize()); idx++){
      if ((this->Get(idx).GetX() != obj->Get(idx).GetX()) &&
          (this->Get(idx).GetY() != obj->Get(idx).GetY())){
          // If the content is diferent, then return false.
         return false;
      }//end if
   }//end for
   // return true if is equal.
   return true;
}//end IsEqual

//------------------------------------------------------------------------------
const unsigned char * tMetricHistogram::Serialize(){
   unsigned char * tmp;
   int w;

   // Attention: This is a modified version that supports tags. This
   // modification will not affect previous versions since the tag is the last
   // value and will be ignored by the old unserialization procedure.
   //
   // Note: I do love pointers! (Just kidding!)
   //                                                                     FJTC

   // Is there a serialized version ?
   if (Serialized == NULL){
      // We must create it.
      Serialized = new unsigned char[GetSerializedSize()];

      //Write Buckets Count
      tmp = Serialized;
      *((int *)tmp) = GetSize();
      tmp += sizeof(int);

      for(w = 0; w < Size; w++){
        *((int *)tmp) = this->Bins[w].GetGray();
        tmp += sizeof(int);
        *((double *)tmp) = this->Bins[w].GetValue();
        tmp += sizeof(double);
      }//end for

      // Append the tag value at the end of file.
      *((u_int32_t *)tmp) = this->tag;
   }//end if

   return Serialized;
}//end tMetricHistogram::Serialize()

//------------------------------------------------------------------------------
void tMetricHistogram::Unserialize(const unsigned char * data){
   int w;
   int oldSize;
   unsigned char * tmp;

   // Save the old size
   oldSize = Size;

   // Read the new size
   tmp = (unsigned char *)data;
   Size = *((int*) tmp);
   tmp += sizeof(int);

   // Will reallocate things ?
   if (Size != oldSize){
      if (Bins != NULL){
         delete [] Bins;
      }//end if
      Bins = new tMetricHistogramBin[Size];
      
      if (Serialized != NULL){
         delete [] Serialized;
      }//end if
      Serialized = new unsigned char[GetSerializedSize()];
   }//end if

   for(w = 0; w < Size; w++){
      this->Bins[w].SetGray(*((int *)tmp));
      tmp += sizeof(int);
      this->Bins[w].SetValue(*((double *)tmp));
      tmp += sizeof(double);
   }//end for

   // It has no tag, so, ignore it!
   this->tag = 0;

   // Put all but the tag.
   memcpy(Serialized, data, GetSerializedSize() - sizeof(tag));

   // Put the tag.
   *((unsigned int *)(Serialized + GetSerializedSize() - sizeof(tag))) =
         this->tag;
}//end tMetricHistogram::Unserialize

//------------------------------------------------------------------------------
void tMetricHistogram::Unserialize(const unsigned char * data, u_int32_t datasize){
   int w;
   int oldSize;
   unsigned char * tmp;

   // Save the old size
   oldSize = Size;
   
   // Read the new size
   tmp = (unsigned char *)data;
   Size = *((int*) tmp);
   tmp += sizeof(int);

   // Will reallocate things ?
   if (Size != oldSize){
      if (Bins != NULL){
         delete [] Bins;
      }//end if
      Bins = new tMetricHistogramBin[Size];
      
      if (Serialized != NULL){
         delete [] Serialized;
      }//end if
      Serialized = new unsigned char[GetSerializedSize()];
   }//end if

   for(w = 0; w < Size; w++){
      this->Bins[w].SetGray(*((int *)tmp));
      tmp += sizeof(int);
      this->Bins[w].SetValue(*((double *)tmp));
      tmp += sizeof(double);
   }//end for

   // Has the tag ?
   if (tmp != data + datasize){
      // Yes, it has a tag!
      this->tag = *((u_int32_t *)tmp);
   }else{
      // No, it doesn't.
      this->tag = 0;
   }//end if

   // Put all but the tag.
   memcpy(Serialized, data, GetSerializedSize() - sizeof(tag));

   // Put the tag.
   *((unsigned int *)(Serialized + GetSerializedSize() - sizeof(tag))) =
         this->tag;
}//end tMetricHistogram::Unserialize

//------------------------------------------------------------------------------
void tMetricHistogram::Refresh(){

   if (this->Serialized != NULL){
      delete [] this->Serialized;
      this->Serialized = NULL;
   }//end if
}//end tMetricHistogram::Refresh

//------------------------------------------------------------------------------
// tMetricHistogramEvaluator Class
//==============================================================================

double tMetricHistogramEvaluator::GetMetricDistance(tMetricHistogram * one,
                                                        tMetricHistogram * other){
   double d=0.0;
   int    a=0, i1=1, io=1, base;
   double a1, ao, y1, yo, w;
   a1 = one->Get(0).GetValue();
   ao = other->Get(0).GetValue();

   //enquanto existir buckets...
   while (i1 < one->GetSize() && io < other->GetSize()) {

      if (one->Get(i1).GetGray() < other->Get(io).GetGray() ) {
         base = one->Get(i1).GetGray() - a;
         yo = ao + base*(other->Get(io).GetValue() - ao)/(double)(other->Get(io).GetGray()-a);
         y1 = one->Get(i1++).GetValue();
      }else{
         base = other->Get(io).GetGray() - a;
         y1 = a1 + base*(one->Get(i1).GetValue() - a1)/(double)(one->Get(i1).GetGray() - a);
         yo = other->Get(io).GetValue();
         if (one->Get(i1).GetGray() == other->Get(io++).GetGray())
            i1++;
      }//end if
      if ( ((ao<a1) && (y1<yo)) || ((ao>a1) && (y1>yo)) ) { // lines cross
         w = (ao-a1)*base/(y1-a1-yo+ao);
         d += fabs(w*(ao-a1) + (base-w)*(y1-yo))/2.;
      }else{
         d += base*fabs(a1+y1-ao-yo);
      }//end if
      a1 = y1;
      ao = yo;
      a += base;
   }//end while
   return d/2.;

} //end tMetricHistogramEvaluator::GetMetricDistance

//------------------------------------------------------------------------------
double tMetricHistogramEvaluator::GetWarpDistance(tMetricHistogram * one,
                                                  tMetricHistogram * other)
{
   int  i, j, lin, col, w;
   float distfinal;
   dlGenericMatrix<float> dist(256, 256), distacum(256,256);

//------vetq recebe os valores de one e vetc recebe os valores de other --------
    lin = one->GetSize();
    col = other->GetSize();
    float vetq[256], vetc[256];
    for (i=0; i<lin; i++)
      vetq[i] = one->Get(i).GetValue();
    for (j=0; j<col; j++)
      vetc[j] = other->Get(j).GetValue();

//------carga da matriz de distâncias ------------------------------------------
    for(i=0; i<lin; i++){
      for(j=0; j<col; j++){
         dist[i][j] = (fabs((vetq[i]-vetc[j])) * fabs(vetq[i]-vetc[j]));
      }//end for
    }//end for
//-------matriz de distâncias acumuladas------------------------------
    float d1, d2, d3;
    for(i=0; i<lin; i++){
      for(j=0; j<col; j++){
         if(i == 0){
            if(j == 0)
               distacum[i][j] = dist[i][j];
            else
               distacum[i][j] = dist[i][j] + distacum[i][j-1];
         }else{
            if(j == 0)
               distacum[i][j] = dist[i][j] + distacum[i-1][j];
            else{
               d1 = distacum[i-1][j-1];
               d2 = distacum[i-1][j];
               d3 = distacum[i][j-1];
               if (d1<=d2 && d1<=d3) distacum[i][j] = dist[i][j] + d1;
               else if (d2<=d3) distacum[i][j] = dist[i][j] + d2;
               else distacum[i][j] = dist[i][j] + d3;
            }//end else j!=0
         }//end else (i!=0)
      }//end for
    }//end for

//------quantidade de elementos do caminho (w) ---------------------------------
    w = 1;
    i = lin-1;
    j = col-1;
    do
    {   if(i == 0)
        {   if(j != 0)  j--;           
        }
        else
        {   if(j == 0)  i--;            
            else
            {   d1 = distacum[i-1][j-1];
                d2 = distacum[i-1][j];
                d3 = distacum[i][j-1];
                if (d1<=d2 && d1<=d3) { i--; j--; }
                else if (d2<=d3) i--;
                else j--;
            }
        }
        w++;
        //printf(" w = %d", w);

    }while(i>0 || j>0);

//------distância final --------------------------------------------------------
    distfinal = distacum[lin-1][col-1]/w;
    return distfinal;
} //end tMetricHistogramEvaluator::GetWarpDistance
//------------------------------------------------------------------------------
