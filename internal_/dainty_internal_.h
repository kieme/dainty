/******************************************************************************

 MIT License

 Copyright (c) 2018 kieme, frits.germs@gmx.net

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

******************************************************************************/

#ifndef DAINTY_INTERNAL_H_
#define DAINTY_INTERNAL_H_

#define DAINTY_CONSTEXPR constexpr
#define DAINTY_INLINE    inline
#define DAINTY_NOEXCEPT  noexcept

#endif

///////////////////////////////////////////////////////////////////////////////

#ifdef DAINTY_INTERNAL_INCLUDE_C_STDINT
  #ifndef _DAINTY_INTERNAL_C_STDINT_INCLUDED_
  #define _DAINTY_INTERNAL_C_STDINT_INCLUDED_
    #include <cstdint>
  #endif
#endif


#ifdef DAINTY_INTERNAL_INCLUDE_C_STDARG
  #ifndef _DAINTY_INTERNAL_C_STDARG_INCLUDED_
  #define _DAINTY_INTERNAL_C_STDARG_INCLUDED_
    #include <cstdarg>
  #endif
#endif


#ifdef DAINTY_INTERNAL_INCLUDE_C_EXECINFO
  #ifndef _DAINTY_INTERNAL_C_EXECINFO_INCLUDED_
  #define _DAINTY_INTERNAL_C_EXECINFO_INCLUDED_
    #include <execinfo.h>
  #endif
#endif


#ifdef DAINTY_INTERNAL_INCLUDE_C_STDLIB
  #ifndef _DAINTY_INTERNAL_C_STDLIB_INCLUDED_
  #define _DAINTY_INTERNAL_C_STDLIB_INCLUDED_
    #include <cstdlib>
  #endif
#endif


#ifdef DAINTY_INTERNAL_INCLUDE_C_UNISTD
  #ifndef _DAINTY_INTERNAL_C_UNISTD_INCLUDED_
  #define _DAINTY_INTERNAL_C_UNISTD_INCLUDED_
    #include <unistd.h>
  #endif
#endif


#ifdef DAINTY_INTERNAL_INCLUDE_C_ASSERT
  #ifndef _DAINTY_INTERNAL_C_ASSERT_INCLUDED_
  #define _DAINTY_INTERNAL_C_ASSERT_INCLUDED_
    #include <cassert>
  #endif
#endif


#ifdef DAINTY_INTERNAL_INCLUDE_C_STDIO
  #ifndef _DAINTY_INTERNAL_C_STDIO_INCLUDED_
  #define _DAINTY_INTERNAL_C_STDIO_INCLUDED_
    #include <cstdio>
  #endif
#endif

#ifdef DAINTY_INTERNAL_INCLUDE_C_STRING
  #ifndef _DAINTY_INTERNAL_C_STRING_INCLUDED_
  #define _DAINTY_INTERNAL_C_STRING_INCLUDED_
    #include <cstring>
  #endif
#endif

#ifdef DAINTY_INTERNAL_INCLUDE_IOSTREAM
  #ifndef _DAINTY_INTERNAL_IOSTREAM_INCLUDED_
  #define _DAINTY_INTERNAL_IOSTREAM_INCLUDED_
    #include <iostream>
  #endif
#endif

#ifdef DAINTY_INTERNAL_INCLUDE_NEW
  #ifndef _DAINTY_INTERNAL_NEW_INCLUDED_
  #define _DAINTY_INTERNAL_NEW_INCLUDED_
    #include <new>
  #endif
#endif

///////////////////////////////////////////////////////////////////////////////
