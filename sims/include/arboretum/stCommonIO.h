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
* This file includes all common I/O headers used by this library. It will help the portability
* of this code.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/
#ifndef __STCOMMONIO_H
#define __STCOMMONIO_H

// GNU GCC
#ifdef __GNUG__
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <fcntl.h>
   #include <unistd.h>
   
   #ifndef O_BINARY
   	#define O_BINARY 0
   #endif // O_BINARY
#endif //__GNUG__

// Microsoft Visual C++
#ifdef _MSC_VER
   
#endif //_MSC_VER


// Borland C++
#ifdef __BORLANDC__
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <fcntl.h>
   #include <io.h>
#endif //__BORLANDC__


#endif //__STCOMMONIO_H
