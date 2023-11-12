#pragma once


#include <stdint.h>


/************************************/
/*        Platform detection        */
/************************************/
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define PLATFORM_WINDOWS 1
//#ifndef _WIN64
//#error "64-bit is required on Windows"
//#endif

#elif defined(__linux__) || defined(__gnu_linux)
#define PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#endif

#elif defined(__unix__)
#define PLATFORM_UNIX 1

#elif defined(_POSIX_VERSION)
#define PLATFORM_UNIX 1

#elif __APPLE__
#define PLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define PLATFORM_IOS 1
#define PLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define PLATFORM_IOS 1

#elif TARGET_OS_MAC

#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform"
#endif


// Exports
#ifdef GEXPORT

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
//#define GAPI __declspec(dllexport)
#define GAPI __attribute__((visibility("default")))
#else
#define GAPI
#endif

// Imports
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define GAPI __declspec(dllimport)
#define GCALL __stdcall
#else
#define GAPI
#define GCALL
#endif
