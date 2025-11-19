#if !defined(_ZEGOMESS_INC_)
#define _ZEGOMESS_INC_
#pragma once
#include "zegotypes.h"
#ifdef _OS_WIN_
#include <stdio.h>
#include <windows.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#include <sys/times.h>
#endif
#include "zegoexcept.h"
#include <time.h>

#if defined(__aarch64__)
#else
#endif

#if	defined(_OS_WIN_)
#	if defined(_OS_WIN_DESKTOP_)
#		include <winsock2.h> /*for struct timeval*/
#       include <ws2tcpip.h>
#   else
#		undef WINAPI_FAMILY
#		define WINAPI_FAMILY	WINAPI_FAMILY_DESKTOP_APP
#		define WINSOCK_API_LINKAGE
#		include <winsock2.h>
#		undef WINAPI_FAMILY
#		define WINAPI_FAMILY	WINAPI_FAMILY_APP
#	endif
#endif

#if defined(__MACH__)
#include <mach/mach_time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
	/**
	 suspend execution for an interval of time.

	 @return	- return 0 if success, otherwise return error code(also set errno).
	 */
	_ZEGO_API int32  zego_msleep(uint32 milliseconds);
	
#ifdef _OS_WIN_
     struct timezone {
             int     tz_minuteswest; /* of Greenwich */
             int     tz_dsttime;     /* type of dst correction to apply */
     };
	/**
	get date and time.
	The system's notion of the current Greenwich time and the current time zone is obtained with the gettimeofday() call.
	The time is expressed in seconds and microseconds since midnight (0 hour), January 1, 1970.
	The resolution of the system clock is hardware dependent, and the time may be updated continuously or in 'ticks'.
	If tp is NULL and tzp is non-NULL, gettimeofday() will populate the timezone struct in tzp.
	If tp is non-NULL and tzp is NULL, then only the timeval struct in tp is populated. If both tp and tzp are NULL, nothing is returned.

	@return	- A 0 return value indicates that the call succeeded.  A -1 return value indicates an error occurred, and in this case an error code is stored into the global variable errno.
	*/
	_ZEGO_API	int gettimeofday(struct timeval* tp, struct timezone* tz);
#endif

    // _ZEGO_API int zego_gettimeofday(struct timeval* tp, struct timezone* tz);

    /**
     get time in second, since 0 hours, 0 minutes, 0 seconds, January 1, 1970
     */
    _ZEGO_API uint32 zego_gettimeofday_second();

    /**
     get time in millisecond, since 0 hours, 0 minutes, 0 seconds, January 1, 1970
     */
    _ZEGO_API uint64 zego_gettimeofday_millisecond();
    
    /**
     get time in microsecond, since 0 hours, 0 minutes, 0 seconds, January 1, 1970
     */
    _ZEGO_API uint64 zego_gettimeofday_microsecond();

    _ZEGO_API inline uint32 zego_gettickcount()
		{
#ifdef _OS_WIN8_
			return (uint32)GetTickCount64();
#elif defined(_OS_WIN_)
			return GetTickCount();
#elif defined(__MACH__)//for ios and macos
            static mach_timebase_info_data_t info = {0};
            static kern_return_t krv __attribute__((unused)) = mach_timebase_info(&info);
            static double r = 1.0 * info.numer / (info.denom * NSEC_PER_MSEC);
			// note: Convert directly from double to uint32 makes undefined behavior.
			// Because If the result lies outside the range of representable values by the type, the conversion causes undefined behavior.
			uint64 result = (uint64)(mach_absolute_time() * r);
            return (uint32)result;
#else
            uint32 result = 0;
            struct timespec ts = {0, 0};
            if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
                uint64 tc = ts.tv_sec;
                tc *= 1000;
                tc += ts.tv_nsec / 1000000;
                result = (uint32)tc;
            }
            return result;
#endif
		}
    
	
    _ZEGO_API inline uint64 zego_gettickcount64()
    {
#if defined(_OS_WIN_)
  #if _WIN32_WINNT <= 0x0501
	   return GetTickCount();
	#else
        return GetTickCount64();    // vista or above
  #endif
#elif defined(__MACH__)//for ios and macos
        static mach_timebase_info_data_t info = {0};
        static kern_return_t krv __attribute__((unused)) = mach_timebase_info(&info);
        static double r = 1.0 * info.numer / (info.denom * NSEC_PER_MSEC);
        return (uint64)(mach_absolute_time() * r);
#else
            uint64 result = 0;
            struct timespec ts = {0, 0};
            if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
                result = ts.tv_sec;
                result *= 1000;
                result += ts.tv_nsec / 1000000;
            }
            return result;
#endif
    }


	_ZEGO_API int64	zego_gettimeoffsetutc();
	
	_ZEGO_API int32  zego_rand();
	
	_ZEGO_API uint8* zego_rand16();
	
	_ZEGO_API boolean zego_isalldigit(const utf8* str);	
	_ZEGO_API boolean zego_str2time(const utf8* strTime,uint32 &dwTime);
	_ZEGO_API boolean zego_int642str(int64 i64Value,utf8** pputf8,uint32 *pulen);
	_ZEGO_API boolean zego_uint642str(uint64 ui64Value,utf8** pputf8,uint32 *pulen);
	_ZEGO_API boolean zego_str2int(const utf8* strValue, int32 & iValue);
	_ZEGO_API boolean zego_str2uint32(const utf8* strValue, uint32 & dwValue);
	_ZEGO_API boolean zego_str2int64(const utf8* strValue, int64 & i64Value);
	_ZEGO_API boolean zego_str2uint64(const utf8* strValue, uint64 & ui64Value);
    _ZEGO_API uint32  zego_strlen(const utf16* str);
	
#ifdef __cplusplus
};
#endif

#ifdef _OS_WIN_

#define ZEGODEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
extern const GUID name;// = {l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8}

#else

typedef struct
{
	uint8 Data1[16];
} GUID;

#define ZEGODEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
extern const GUID name __attribute__ ((weak))\
= {(l & 0xFF000000) >> 24, (l & 0x00FF0000) >> 16, (l & 0x0000FF00) >> 8, (l & 0x000000FF), (w1 & 0xFF00) >> 8, (w1 & 0x00FF), (w2 & 0xFF00) >> 8, (w2 & 0x00FF), b1, b2, b3, b4, b5, b6, b7, b8}
#endif

#ifndef MIN
#define MIN(x, y)	((x < y) ? x : y)
#endif

#ifndef MAX
#define MAX(x, y)	((x > y) ? x : y)
#endif

#ifndef ZEGOMAKEUINT64
#define ZEGOMAKEUINT64(a, b) ((uint64)(((uint32)(b)) | ((uint64)((uint32)(a))) << 32))
#endif

#endif /* _ZEGOMESS_INC_ */
