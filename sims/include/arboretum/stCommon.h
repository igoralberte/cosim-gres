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
* This file includes all common headers used by this library. It will help the portability
* of this code.
*
* @version 1.0
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
*/                                     
#ifndef __STCOMMON_H
#define __STCOMMON_H

// All compilers
#include <stdlib.h>

// GNU GCC
#ifdef __GNUG__
   #include <string.h> // malloc & cia
   
   // This is an useful function extracted from Windows C Runtime library.
   #define random(x) ((int)((((double)rand()) / ((double)RAND_MAX)) * (x)))
   
	// Mingw does not provide these values so, here they are.
	#ifdef __MINGW32_VERSION
	   #define MAXSHORT    0x7fff
	   #define MAXINT      0x7fffffff
	   #define MAXLONG     0x7fffffff
	   #define MAXDOUBLE   1.7976931348623158E+308
	   #define MAXFLOAT    3.40282347E+38F
	   #define MINDOUBLE   2.2250738585072014E-308
	   #define MINFLOAT    1.17549435E-38F
	   #define MAXLDOUBLE  1.1897314953572317649E+4932L
	   #define MINLDOUBLE  3.362103143112094E-4917L
	#else	
		#ifdef __CYGWIN__
	   	#define MAXSHORT    0x7fff
	   	#define MAXINT      0x7fffffff
	   	#define MAXLONG     0x7fffffff
	   	#define MAXDOUBLE   1.7976931348623158E+308
	   	#ifndef MAXFLOAT
            #define MAXFLOAT    3.40282347E+38F
         #endif
	   	#define MINDOUBLE   2.2250738585072014E-308
	   	#define MINFLOAT    1.17549435E-38F
		   #define MAXLDOUBLE  1.1897314953572317649E+4932L
		   #define MINLDOUBLE  3.362103143112094E-4917L
		#else
			// Other gccs
		   #include <limits.h> 
         #include <float.h>
         #include <limits>
         #define MAXSHORT    0x7fff
         #define MAXINT      0x7fffffff
         #define MAXLONG     0x7fffffff
         #define MAXDOUBLE   1.7976931348623158E+308
         #ifndef MAXFLOAT
            #define MAXFLOAT    3.40282347E+38F
         #endif
         #define MINDOUBLE   2.2250738585072014E-308
         #define MINFLOAT    1.17549435E-38F
         #define MAXLDOUBLE  1.1897314953572317649E+4932L
         #define MINLDOUBLE  3.362103143112094E-4917L
		#endif
	#endif //__MINGW32_VERSION
#endif //__GNUG__

// Microsoft Visual C++
#ifdef _MSC_VER

   #include <limits.h>
   #define MAXLONG LONG_MAX
   #define MAXINT INT_MAX
   #define MAXDOUBLE 1.7E308

   // malloc & cia
   #include <memory.h>  

   // Avoid the deprecated POSIX names. 
   // TODO In the future we must replace the POSIX names by the ISO C++.
   #include <io.h>
   #include <fcntl.h>
   #include <sys/stat.h>
   #define lseek _lseek
   #define read _read
   #define write _write
   #define open _open

   #define O_CREAT _O_CREAT
   #define O_TRUNC _O_TRUNC
   #define O_RDWR _O_RDWR 
   #define O_BINARY _O_BINARY
   // The following constants are not supported by Win32
   #ifndef S_IREAD
      #define S_IREAD 0
   #endif
   #ifndef S_IWRITE
      #define S_IWRITE 0
   #endif

   // M_PI has been deprecated so I'll define it again.
   #ifndef M_PI
      #define M_PI           3.14159265358979323846
   #endif
#endif //_MSC_VER

// Borland C++
#ifdef __BORLANDC__
   #include <mem.h> // malloc & cia
   #include <limits.h> 
   #include <float.h>
#endif //__BORLANDC__

#endif //__STCOMMON_H
