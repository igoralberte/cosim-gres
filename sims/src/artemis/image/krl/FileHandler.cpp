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
#include <artemis/image/krl/FileHandler.h>

/**
* Empty constructor for File Handler.
*/
FileHandler::FileHandler(){
}

/**
* Constructor for File Handler.
*
* @param name The name of the file what will be opened.
* @param type Type of open (e.g.: Only to read, write, etc.).
* @exception FileException Throwed when the file does not exists.
*/
FileHandler::FileHandler(std::string name, int type) throw (std::runtime_error){

    if (!this->fileExists(name))
       throw std::runtime_error("The krl image file cannot be opened or the file does not exists");
            
    switch(type){
        case 0: //FILE_IN:
            this->file.open(name.c_str(),std::fstream::in);
            break;
        case 1: //FILE_OUT:
            this->file.open(name.c_str(),std::fstream::out);
            break;
        case 2: //FILE_OUT_APP:
            this->file.open(name.c_str(),std::fstream::app);
            break;
        case 3: //FILE_BINARY
            this->file.open(name.c_str(),std::fstream::in | std::fstream::binary);
            break;
    }
    
    this->type = type;
    
    file.seekg(0, std::ios::end);
    setSize(file.tellg());
    file.seekg(0, std::ios::beg);
}

/**
* Destructor of File Handler
*/
FileHandler::~FileHandler(){

}

/**
* Sets the size of the file.
*
* @param size The size of the file.
*/
void FileHandler::setSize(int size){

    this->size = size;
}

/**
* Gets the size of the file.
*
* @return The size of the file.
*/
int FileHandler::getSize(){

    return size;
}

/**
* Gets a next line from file.
*/
char* FileHandler::getNextLine(){
    
    char *charLine;
    std::string line;
    getline(this->file,line);
    charLine = new char[line.size()+1];
    strcpy(charLine, line.c_str());
    
    return charLine;
}

/**
* Gets a next word from recovered line.
*
* @param line The recovered line from file.
*/
char * FileHandler::getNextWord(char * &line){

    char * begin = line;
    while( *line != ' ' && *line != '\n' && *line != '\0' ){
           
        line++;
    }
    *line = '\0';
    line++;
    return begin;     
}

/**
* Read size bytes from file.
*
* @param size The size to be read.
*/
char * FileHandler::readFile(int size){
    
    char * tmp = new char(size); 
    this->file.read(tmp,size);
    return tmp;     
}

/**
* Read bytes from Buffer.
*
* @param buffer The in buffer.
* @param numByte The number of bytes to be read.
*/
char * FileHandler::readBuffer( char* & buffer, int numByte){
        
    char* tmp = new char[numByte];
    for(int i=0;i<numByte;i++)
        tmp[i] = buffer[i];
    buffer = buffer + numByte;
    return tmp;  
}

/**
* Endian Swap.
*
* @param x.
*/
void FileHandler::endianSwap(u_int16_t& x){
       
    x = (x>>8) | (x<<8);
}

/**
* Close the opened file.
*/
void FileHandler::close(){

    this->file.close();
}

/**
* Checks if the file exists.
*
* @param fileName The name of the file.
* @return If the file exists.
**/
bool FileHandler::fileExists(std::string fileName){
     
    std::fstream tmpFile(fileName.c_str());
    if(tmpFile.is_open()){
        tmpFile.close();
        return true;
    } else {
        return false;       
    }   
     
}

/**
* Checks if the file is over.
*
* @return If the file is ended.
*/
bool FileHandler::endOfFile(){
     
    return(file.eof());
}

