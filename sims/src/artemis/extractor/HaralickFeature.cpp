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
 Explanation:
             
          HaralickFeatureMatrix
                    |       Distance Vector
                    |             |
              x = distances       |
              ______________      __
  y = angles |__|__|__|__|__|    |__|
             |__|__|__|__|__|    |__|
             |__|__|__|__|__|    |__|
             |__|__|__|__|__|    |__|

              ______________
             |__|__|__|__|__| -- Angle Vector

  Use (x , y) = (distance, angle) to set and get value.
  The "default" values to (x, y) are (5, 4).
  The direct access to the vectores are not allowed.
*/

#include <artemis/extractor/HaralickFeature.h> 

/**
* Constructor.
*
* @param numDistances The number of valid distances.
* @param numAngles The number of valid angles.
**/
HaralickFeaturesMatrix::HaralickFeaturesMatrix(u_int16_t numDistances, u_int16_t numAngles){

    setNumAngles(numAngles);
    setNumDistances(numDistances);
    createFeaturesMatrix();
    createDistanceMeanVector();
    createAngleMeanVector();
}

/**
* Creates the Co-ocurrence Matrix.
*
* @throw runtime_error: If the memory are full.
*/
void HaralickFeaturesMatrix::createFeaturesMatrix() throw (std::runtime_error){

     try{
        featuresMatrix = new double*[getNumDistances()];
        for (u_int16_t i = 0; i < getNumDistances(); i++)
	        featuresMatrix[i] = new double[getNumAngles()];
	        
        for(u_int16_t x = 0; x < getNumDistances(); x++){
            for(u_int16_t y = 0; y< getNumAngles(); y++){
                 setHaralickFeatureValue(x, y, 0);
            }
        }
    }catch (std::bad_alloc&){
        throw std::runtime_error("Insuficient heap size to allocate Fetures Matrix");
    }catch (...){
        throw std::runtime_error("Error into allocate memory size");
    }     
}

/**
* Create the characteristics vector of mean by distance.
*
* @throw runtime_error If the memory size is not sufficient.
*/
void HaralickFeaturesMatrix::createDistanceMeanVector() throw (std::runtime_error){

     try{
        distanceMeanFeatures = new double[getNumAngles()];
        for(u_int16_t y = 0; y< getNumAngles(); y++){
            setDistanceMeanValue(y, 0);   
        }
     }catch (std::bad_alloc&){
        throw std::runtime_error("Insuficient heap size to allocate distance feature vector");
     }catch (...){
        throw std::runtime_error("Error u_int16_to allocate memory size");
     } 
     
}

/**
* Create the characteristics vector of mean by angle.
*
* @throw runtime_error If the memory size is not sufficient.
*/
void HaralickFeaturesMatrix::createAngleMeanVector() throw (std::runtime_error){
     
     try{
        angleMeanFeatures = new double[getNumDistances()];
        for(u_int16_t y = 0; y< getNumDistances(); y++){
                setAngleMeanValue(y, 0);   
        }
     }catch (std::bad_alloc&){
        throw std::runtime_error("Insuficient heap size to allocate distance feature vector");
     }catch (...){
        throw std::runtime_error("Error u_int16_to allocate memory size");
     } 
}

/**
* Unlock memory space utilized by FeaturesMatrix.
*/
void HaralickFeaturesMatrix::deleteFeaturesMatrix(){

     if(featuresMatrix != NULL){
	for (u_int16_t i = 0; i < getNumDistances(); i++){
		delete[] featuresMatrix[i];
	}
	delete[] featuresMatrix;
     }
}

/**
* Unlock memory space utilized by DistanceMeanVector.
*/
void HaralickFeaturesMatrix::deleteDistanceMeanVector(){

     if(distanceMeanFeatures != NULL){
        delete[] distanceMeanFeatures;
    }
}

/**
* Unlock memory space utilized by AngleMeanVector.
*/
void HaralickFeaturesMatrix::deleteAngleMeanVector(){
     
     if(angleMeanFeatures != NULL){
        delete[] angleMeanFeatures;
    }
}

/**
* Destructor method.
*/
HaralickFeaturesMatrix::~HaralickFeaturesMatrix() throw (std::runtime_error){

        deleteFeaturesMatrix();
        deleteDistanceMeanVector();
        deleteAngleMeanVector();
}

/**
* Sets the value of the position on co-ocurrence matrix.
*
* @param distance The distance (X Axis) of the value.
* @param angle    The angle (Y axis) of the value.
* @param value    The value to be inserted on co-ocurrence matrix.
* @throw std::range_error If the pair (distance, angle) is not a valid position in the co-ocurrence matrix.
*/
void HaralickFeaturesMatrix::setHaralickFeatureValue(u_int16_t distance, u_int16_t angle, double value) throw (std::range_error){

     if ((distance >= getNumDistances()) || (distance < 0))
        throw std::range_error("The requested (distance, angle) is out of bounds");
     if ((angle >= getNumAngles()) || (angle < 0))
        throw std::range_error("The requested (distance, angle) is out of bounds");
     
     featuresMatrix[distance][angle] = value;
}

/**
* Sets a value in a valid position of the de DistanceMeanVector
*
* @param pos    The position to be inserted
* @param value  The value what will be inserted
*
* @throw std::range_error If the pos is a not valid position in the DistanceMean vector.
*/
void HaralickFeaturesMatrix::setDistanceMeanValue(u_int16_t pos, double value) throw (std::range_error){

     if ((pos >= getNumAngles()) || (pos < 0))
        throw std::range_error("The requested (distance, angle) is out of bounds");

     distanceMeanFeatures[pos] = value;
}

/**
* Sets a value in a valid position of the de DistanceMeanVector
*
* @param pos    The position to be inserted
* @param value  The value what will be inserted
*
* @throw std::range_error If the pos is a not valid position in the AngleMean vector.
*/
void HaralickFeaturesMatrix::setAngleMeanValue(u_int16_t pos, double value) throw (std::range_error){

     if ((pos >= getNumDistances()) || (pos < 0))
        throw std::range_error("The requested (distance, angle) is out of bounds");

     angleMeanFeatures[pos] = value;
}

/**
* Sets the number of angles utilized.
*
* @param numAngles The number of the angles utilized.
*/
void HaralickFeaturesMatrix::setNumAngles(u_int16_t numAngles){

     this->numAngles = numAngles;
}

/**
* Sets the number of the distances utilized.
*
* @param numDistances The number of the distances utilized.
*/
void HaralickFeaturesMatrix::setNumDistances(u_int16_t numDistances){

     this->numDistances = numDistances;
}
        
/**
* Gets a value by pair (distance, angle) of the co-ocurrence matrix.
*
* @param distance    The distance value (X axis).
* @param angle       The angle value (Y axis).
* @return The value of the Haralick feature on the position (distance, angle) of the co-ocurrence matrix.
*
* @throw std::range_error If the pair (distance, angle) is not a valid position.
*/
double HaralickFeaturesMatrix::getHaralickFeatureValue(u_int16_t distance, u_int16_t angle) throw (std::range_error){

     if ((distance >= getNumDistances()) || (distance < 0))
        throw std::range_error("The requested (distance, angle) is out of bounds");
     if ((angle >= getNumAngles()) || (angle < 0))
        throw std::range_error("The requested (distance, angle) is out of bounds");
     
     return featuresMatrix[distance][angle];
}

/**
* Gets a value of the DistanceMean vector.
*
* @param pos The position what will be recovered.
*
* @throw std::range_error If the pos is not a valid position.
*
* @return A value of the Distance Mean vector on the position pos.
*/
double HaralickFeaturesMatrix::getDistanceMeanValue(u_int16_t pos) throw (std::range_error){

      if ((pos >= getNumAngles()) || (pos < 0))
        throw std::range_error("The requested (distance, angle) is out of bounds");

      return distanceMeanFeatures[pos];
}

/**
* Gets a value of the AngleMean vector.
*
* @param pos The position what will be recovered.
*
* @throw std::range_error If the pos is not a valid position.
*
* @return A value of the Angle Mean vector on the position pos.
*/
double HaralickFeaturesMatrix::getAngleMeanValue(u_int16_t pos) throw (std::range_error){

      if ((pos >= getNumDistances()) || (pos < 0))
        throw std::range_error("The requested (distance, angle) is out of bounds");

      return angleMeanFeatures[pos];
}

/**
* Gets the utilized number of angles.
*
* @return The utilized number of angles.
*/
u_int16_t HaralickFeaturesMatrix::getNumAngles(){

      return numAngles;
}

/**
* Gets the utilized number of distances.
*
* @return The utilized number of distances.
*/
u_int16_t HaralickFeaturesMatrix::getNumDistances(){

      return numDistances;
}

/**
* Gets the size of the DistanceMean vector.
*
* @return The size of the DistanceMean.
*/
u_int16_t HaralickFeaturesMatrix::getDistanceMeanSize(){

    return getNumAngles();
}

/**
* Gets the size of the AngleMean vector.
*
* @return The size of the AngleMean.
*/
u_int16_t HaralickFeaturesMatrix::getAngleMeanSize(){

    return getNumDistances();
}
        
/**
* This method catch a "nature" identification of the co-ocurrence matrix based on distance.
*
* @return The characteristics of the co-ocurrence matrix.
*/
double* HaralickFeaturesMatrix::getDistanceMeanFeatures(){

     for(u_int16_t y = 0; y< getNumAngles(); y++){
          setDistanceMeanValue(y, 0);   
     };
     
     for(u_int16_t y = 0; y< getNumAngles(); y++){
        for(u_int16_t x=0; x < getNumDistances(); x++){
            setDistanceMeanValue(y, (getDistanceMeanValue(y) + getHaralickFeatureValue(x, y)));
        }
        setDistanceMeanValue(y, (getDistanceMeanValue(y)/getNumDistances()));
     } 
    
     return distanceMeanFeatures;  
}

/**
* This method catch a "nature" identification of the co-ocurrence matrix based on angle.
*
* @return The characteristics of the co-ocurrence matrix.
*/
double* HaralickFeaturesMatrix::getAngleMeanFeatures(){

     for(u_int16_t y = 0; y< getNumDistances(); y++){
          setAngleMeanValue(y, 0);   
     };
     
     for(u_int16_t x=0; x < getNumDistances(); x++){
        for(u_int16_t y = 0; y< getNumAngles(); y++){
            setAngleMeanValue(x, (getAngleMeanValue(x) + getHaralickFeatureValue(x, y)));
        }
        setAngleMeanValue(x, (getAngleMeanValue(x)/getNumAngles()));
     }
     
     return angleMeanFeatures;
}

/**
* Clone the co-ocurrence matrix.
*
* @return A cloned co-ocurrence matrix.
*/
HaralickFeaturesMatrix* HaralickFeaturesMatrix::clone(){

     HaralickFeaturesMatrix *h = new HaralickFeaturesMatrix(this->getNumDistances(), 
                                                            this->getNumAngles());
     
     for(u_int16_t x=0; x < getNumDistances(); x++){
        for(u_int16_t y = 0; y< getNumAngles(); y++){
              h->setHaralickFeatureValue(x, y, this->getHaralickFeatureValue(x, y)); 
        }
        h->setAngleMeanValue(x, this->getAngleMeanValue(x));
     }
     
     for(u_int16_t y = 0; y< getNumAngles(); y++){
             h->setDistanceMeanValue(y, this->getDistanceMeanValue(y));
     }
     
     return h;
}

/**
* Compare two co-ocurrence matrix.
*
* @return True if the matrix are equal or false if are distinct.
*/
bool HaralickFeaturesMatrix::isEqual(HaralickFeaturesMatrix *h){

     if (this->getNumDistances() != h->getNumDistances())
        return false;
     if (this->getNumAngles() != h->getNumAngles())
        return false;
     
     for(u_int16_t x=0; x < getNumDistances(); x++){
        for(u_int16_t y = 0; y< getNumAngles(); y++){
              if (h->getHaralickFeatureValue(x, y) != this->getHaralickFeatureValue(x, y)){
                  return false;
              } 
        }
        if (h->getAngleMeanValue(x) != this->getAngleMeanValue(x))
           return false;
     }
     
     for(u_int16_t y = 0; y< getNumAngles(); y++){
             if (h->getDistanceMeanValue(y) != this->getDistanceMeanValue(y))
                return false;
     }
     
     return true;
}
