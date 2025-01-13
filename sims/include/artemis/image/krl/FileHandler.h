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
* This file defines a Class to open a single File from disk.
*
* @version 1.0
*/
#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <stdexcept>

/**
* This Class is a Handler Class for files.
*
* @brief Handler Class for Files
* @version 1.0 
*/

class FileHandler {
      
    private :
        int size;
        int type;
		
    public :
        std::fstream file;
        static const int FILE_IN = 0;
        static const int FILE_OUT = 1;
        static const int FILE_OUT_APP = 2;
        static const int FILE_BINARY = 3;
		
	public:
	    FileHandler();
            FileHandler(std::string name, int type) throw (std::runtime_error);
            ~FileHandler();
		
            void setSize(int size);
		
            int getSize();
            char * getNextLine();
            char * getNextWord(char * &line);
        
            char * readFile(int size);
            char * readBuffer(char * &buffer, int numByte);

            void endianSwap(u_int16_t& x);
            void close();
       
            static bool fileExists( std::string fileName);
            bool endOfFile();
		
};

#endif
