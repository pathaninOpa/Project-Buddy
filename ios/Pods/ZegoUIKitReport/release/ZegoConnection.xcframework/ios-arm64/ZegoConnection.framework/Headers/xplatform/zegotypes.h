#ifndef _ZEGOTYPES_INC_
#define _ZEGOTYPES_INC_
#pragma once

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/*system detecting*/
#if defined(__ANDROID__) || defined (ANDROID)
#	define _OS_ANDROID_
#elif (defined(_WIN32) || defined(_WIN64))

#if defined BUILD_XP  //BUILD_XP macro is predefined for the project
#	if !defined(_WIN32_WINNT) || (_WIN32_WINNT <= 0x0501)
#		define _WIN32_WINNT	0x0501 /*support xp*/
#		define NTDDI_VERSION NTDDI_WINXP
#	endif
#else
#	if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0600)
#		define _WIN32_WINNT	0x0600 /*here we only support Windows Vista and above*/
#	endif
#endif


#	define _OS_WIN_				1
#   if defined(_MSC_VER)  && !defined(__clang__)
#       pragma warning(disable:4355) /*we don't need such warning C4355: 'this' : used in base member initializer list*/
#		if _MSC_VER >= 1300		/*MS VC++ 7.0 _MSC_VER = 1300*/
#			define	__attribute__(...)
#		else
#			define	__attribute__(a)
#		endif
#   endif
#	define _WINSOCKAPI_
#ifndef WINBASE_DECLARE_GET_MODULE_HANDLE_EX
#define WINBASE_DECLARE_GET_MODULE_HANDLE_EX
#endif
#	include <windows.h>
#	undef _WINSOCKAPI_
#	if defined(WINAPI_FAMILY_PARTITION)
#		if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#			define _OS_WIN_DESKTOP_	1
#		else
#			define _OS_WIN8_		1
#			define _OS_WIN_APP_		1
#		endif
#	else
#		define _OS_WIN_DESKTOP_		1
#	endif
#elif defined(__APPLE__) || defined(__MACH__)
#	include	<TargetConditionals.h>
#	if TARGET_OS_IPHONE
#		define _OS_IOS_			1
#	elif TARGET_IPHONE_SIMULATOR
#		define _OS_IOS_			1
#		define _OS_IOS_SIMULATOR_	1
#	elif TARGET_OS_MAC
#		define _OS_MAC_			1
#	else
#		error Unsupported platform
#	endif
#elif defined(__linux__) || defined(__CYGWIN__)
#   if defined(ARM_LINUX)
#       define _ARM_            1
#   endif
#	define _OS_LINUX_			1
#elif (!defined(_OS_MAC_) && !defined(_OS_IOS_) && !defined(_OS_ANDROID_) && !defined(_OS_WIN_) && !defined(_OS_LINUX_)) && !defined(_OS_OHOS_)
#	error current system is not supported
#endif

/*make stdio/string/stdlib happy*/
#ifndef _OS_WIN_
#	define strnicmp				strncasecmp
#	define stricmp				strcasecmp
#	define _atoi64				atoll
#	define putch				putchar
#else
#	pragma   warning( disable : 4996)//warning silas
#	ifndef _CRT_SECURE_NO_DEPRECATE
#	define _CRT_SECURE_NO_DEPRECATE	//warning silas
#	endif
#	define snprintf				_snprintf
#	define vsnprintf			_vsnprintf
#	define atoll				_atoi64
#endif /*_OS_WIN_*/

#undef _I64_
#undef _i64_
#undef _64u_
#undef _I64uw_
#undef _i64uw_
#if defined(_MSC_VER) || defined(__BORLANDC__)
#	if defined(_ZEGO_EXPORT_)
#		define _ZEGO_API			__declspec(dllexport)
#		define _ZEGO_CLS			__declspec(dllexport)
#	elif defined(_ZEGO_IMPORT_)
#		define _ZEGO_API			__declspec(dllimport)
#		define _ZEGO_CLS			__declspec(dllimport)
#	else
#		define _ZEGO_API		
#		define _ZEGO_CLS		
#	endif
#	define _I64_				"I64"
#	define _i64_				"i64"
#	define _64u_				"%I64u"
#	define _I64uw_				L"%llu" L
#	define _i64uw_				L"%llu" L
#else
#	define _ZEGO_API				__attribute__((visibility("default")))
#	define _ZEGO_CLS				__attribute__((visibility("default")))
#	define _I64_				"ll"
#	define _i64_				"ll"
#	define _64u_				"%llu"
#	define _I64uw_				L"%llu" L
#	define _i64uw_				L"%llu" L
#	undef __int64
#	define __int64				long long	
#endif

#ifndef __cplusplus
#	define	false				0
#	define	true				1
#endif

#if defined(DEBUG) || defined(_DEBUG) || defined(_DEBUG_)
#	ifndef DEBUG
#		define DEBUG
#	endif
#	ifndef _DEBUG
#		define _DEBUG
#	endif
#	ifndef _DEBUG_
#		define _DEBUG_
#	endif
#endif

#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86_64 1
#define ARCH_CPU_64_BITS 1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86 1
#define ARCH_CPU_32_BITS 1
#elif defined(__ARMEL__) || defined(_M_ARM)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARMEL 1
#define ARCH_CPU_32_BITS 1
#define WCHAR_T_IS_UNSIGNED 1
#elif defined(__powerpc64__)
#define ARCH_CPU_PPC64 1
#define ARCH_CPU_64_BITS 1
#elif defined(__ppc__) || defined(__powerpc__)
#define ARCH_CPU_PPC 1
#define ARCH_CPU_32_BITS 1
#elif defined(__sparc64__)
#define ARCH_CPU_SPARC 1
#define ARCH_CPU_64_BITS 1
#elif defined(__sparc__)
#define ARCH_CPU_SPARC 1
#define ARCH_CPU_32_BITS 1
#elif defined(__mips__)
#define ARCH_CPU_MIPS 1
#define ARCH_CPU_32_BITS 1
#elif defined(__hppa__)
#define ARCH_CPU_HPPA 1
#define ARCH_CPU_32_BITS 1
#elif defined(__ia64__)
#define ARCH_CPU_IA64 1
#define ARCH_CPU_64_BITS 1
#elif defined(__s390x__)
#define ARCH_CPU_S390X 1
#define ARCH_CPU_64_BITS 1
#elif defined(__s390__)
#define ARCH_CPU_S390 1
#define ARCH_CPU_32_BITS 1
#elif defined(__alpha__)
#define ARCH_CPU_ALPHA 1
#define ARCH_CPU_64_BITS 1
#elif defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARMEL 1
#define ARCH_CPU_64_BITS 1
#else
#error Please add support for your architecture in zegotypes.h
#endif

/*common type*/
typedef unsigned char			boolean;

typedef float					float32;
typedef double					float64;

typedef signed char				int8;
typedef signed short			int16;
typedef signed int				int32;
typedef signed __int64			int64;

typedef unsigned char			uint8;
typedef unsigned short			uint16;
typedef unsigned int			uint32;
typedef unsigned __int64		uint64;

typedef	char					utf8;
typedef	char					gbk;
#ifdef _MSC_VER
typedef	wchar_t					utf16;
#else /*_MSC_VER*/
typedef	unsigned short			utf16;
#endif /*_MSC_VER*/

#ifndef _OS_WIN_
typedef uint8 BYTE;
typedef unsigned short WORD;
typedef signed int LONG;
typedef short SHORT;
typedef unsigned int UINT;
typedef long long INT64;
#endif

typedef void*					t_hcontext;

#if defined(_OS_WIN_)

typedef SIZE zegosize;

#else

typedef struct _tagZEGOSIZE
{
	int32 cx;
	int32 cy;
}zegosize;

#endif/*_OS_WIN_*/

#ifdef __cplusplus
typedef const zegosize & zegosize_cref;
#endif

#ifdef ARCH_CPU_32_BITS
typedef int32						FD;
#else
typedef int64						FD;
#endif

//add by silas eg: #pragma message WARN("your warning message here")
#define STRINGISE_IMPL(x) #x
#define STRINGISE(x) STRINGISE_IMPL(x)
#if _MSC_VER
#   define FILE_LINE_LINK __FILE__ "(" STRINGISE(__LINE__) ") : "
#   define WARN(exp) (FILE_LINE_LINK "WARNING: " exp)
#else//__GNUC__ - may need other defines for different compilers
#   define WARN(exp) ("WARNING: " exp)
#endif
#if defined(_OS_WIN_)

typedef RECT zegorect;

#else

typedef struct _tagZEGORect
{
	uint32 left;
	uint32 top;
	uint32 right;
	uint32 bottom;
}zegorect;

#endif/*_OS_WIN_*/

typedef  zegorect  zegorect_cref;


#endif /*_ZEGOTYPES_INC_*/
