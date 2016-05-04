/*==============================================================================
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson & The RRDTP Team

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
==============================================================================*/
#ifndef Platform_H
#define Platform_H

//Platform detection
#if defined(_WIN32)
#define RRDTP_PLATFORM_WINDOWS
#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
#define RRDTP_PLATFORM_UNIX

#endif

//JNI Wrapper - comment this out to disable building the JNI wrapper
#define BUILD_JAVA_INTERFACE

//hton/ntoh
#if defined(RRDTP_PLATFORM_WINDOWS)
#include <Winsock2.h>
#elif defined(RRDTP_PLATFORM_UNIX)
#include <arpa/inet.h>
#define ntohll(x) htobe64(x)
#define htonll(x) be64toh(x)
#define htonf(x) htobe64(x)
#define ntohf(x) be64toh(x)
#endif

#endif
