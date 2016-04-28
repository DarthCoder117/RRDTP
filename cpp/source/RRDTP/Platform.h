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

#endif