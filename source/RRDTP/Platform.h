#ifndef Platform_H
#define Platform_H

#if defined(_WIN32)
#define RRDTP_PLATFORM_WINDOWS
#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
#define RRDTP_PLATFORM_UNIX
#endif

#endif