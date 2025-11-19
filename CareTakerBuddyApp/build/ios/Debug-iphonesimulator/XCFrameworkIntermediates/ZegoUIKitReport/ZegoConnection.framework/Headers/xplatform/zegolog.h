#if !defined(_ZEGOLOG_INC_)
#define _ZEGOLOG_INC_
#pragma once

#include "zegotypes.h"
#include "zegoexcept.h"

#define _MAX_EVT_LEN	10240

#ifndef __MODULE__
#	define	__MODULE__	"unnamed"
#endif

/*
 *	system log level
 */
#define	_SLT_GRIEVOUS	0
#define	_SLT_ERROR		1
#define	_SLT_WARNING	2
#define	_SLT_GENERIC	3
#define	_SLT_DEBUG		4

#ifndef _SLT_DEFAULT
#	if defined(_DEBUG) || defined(DEBUG) || defined(_DEBUG_) || !defined(NDEBUG)
#		define _SLT_DEFAULT	_SLT_ERROR
#	else
#		define _SLT_DEFAULT	_SLT_ERROR
#	endif
#endif

typedef	enum {
    log_output_type_a,
    log_output_type_v,
	log_output_type_s, // 日志精简模式枚举标记, 目的是减小包大小, 方式是在编译构建之前, 使用脚本抽取工程里hardcode的日志字符串信息到map文件里, 然后使用脚本将目前工程调用日志打印的接口替换成如下的接口
    log_output_type_x  //日志工程 用于sdk 上层写明文日志
} log_output_type;


typedef	enum {
	/*this type is used for app panic, it most like some app crash or data loss will happen.*/
	esyslog_grievous	= _SLT_GRIEVOUS,
	
	/*this type is used for app logic error*/
	esyslog_error		= _SLT_ERROR,
	
	/*this type is used for app logic warning, 
	 in this case, something happened but we really don't want it be, so just throw a warning
	 */
	esyslog_warning		= _SLT_WARNING,
	
	/*this type is used to make some tips to user or developer for further purpose. 
	 there very important thing, like login/out/audit/security operation, is happening. 
	 */
	esyslog_generic		= _SLT_GENERIC,
	
	/*this type is used to make some tips to developer for debuging purpose only.*/
	esyslog_debug		= _SLT_DEBUG
} esyslog_type;

typedef enum {

    lg_char_type 	= 0x01,			// %c
    lg_int_type 	= 0x02,			// %d
	lg_uint_type	= 0x03,			// %u
    lg_long_type	= 0x04,			// %ld
	lg_ulong_type	= 0x05,			// %lu
	lg_hexint_type	= 0x06,			// %x
    lg_double_type	= 0x07,			// %f
    lg_string_type	= 0x08,			// %s
    lg_ptr_type		= 0x09			// %p

}replaced_logfunc_params_type;

typedef enum{
	for_liveroom_sdk = 0x01,
	for_express_sdk
}log_bus_type;


#if (!defined(V_LOG) && !defined(A_LOG))

#if	defined(_OS_WIN_) && (_MSC_VER < 1300) //MS VC++ 7.0 _MSC_VER = 1300, __VA_ARGS__ is unsupported if _MSC_VER < 1300
#	pragma message ("syslog is unsupported under current compiler!")
/*
 we do nothing here,
 just make such compiler happy,
 so he can skip all 'undefined error'!
 */
inline void log_panic(...) {}
inline void log_error(...) {}
inline void log_warning(...) {}
inline void log_notice(...) {}
inline void log_debug(...) {}
inline void log_suc(fmt, ...){}
inline void log_fail(fmt, ...){}

#else

#   define  log_suc(fmt,...) log_notice("<<<<<<<<" #fmt ">>>>>>>>",##__VA_ARGS__)
#   define  log_fail(fmt,...) log_error("!!!!!!!!" #fmt "!!!!!!!!",##__VA_ARGS__)

#	define	log_panic(...)	syslog(esyslog_grievous,__MODULE__, __LINE__,  __VA_ARGS__)

//# if		_SLT_DEFAULT >= _SLT_ERROR
#	define	log_error(...)	syslog(esyslog_error,	__MODULE__, __LINE__,  __VA_ARGS__)
//# else
//#	define	log_error(...)
//# endif

//# if		_SLT_DEFAULT >= _SLT_WARNING
#	define	log_warning(...)syslog(esyslog_warning, __MODULE__, __LINE__,  __VA_ARGS__)
//# else
//#	define	log_warning(...)
//# endif

//# if		_SLT_DEFAULT >= _SLT_GENERIC
#	define	log_notice(...)	syslog(esyslog_generic, __MODULE__, __LINE__,  __VA_ARGS__)
//# else
//#	define	log_notice(...)
//# endif

//# if		_SLT_DEFAULT >= _SLT_DEBUG
#	define	log_debug(...)	syslog(esyslog_debug,	__MODULE__, __LINE__,  __VA_ARGS__)
//# else
//#	define	log_debug(...)
//# endif

#endif

#endif


#if (defined(A_LOG))
#if	defined(_OS_WIN_) && (_MSC_VER < 1300) //MS VC++ 7.0 _MSC_VER = 1300, __VA_ARGS__ is unsupported if _MSC_VER < 1300
#	pragma message ("syslog is unsupported under current compiler!")
/*
 we do nothing here,
 just make such compiler happy,
 so he can skip all 'undefined error'!
 */
inline void log_a_panic(...) {}
inline void log_a_error(...) {}
inline void log_a_warning(...) {}
inline void log_a_notice(...) {}
inline void log_a_debug(...) {}
inline void log_a_suc(fmt, ...){}
inline void log_a_fail(fmt, ...){}

#else
#   define  log_a_suc(fmt,...) log_a_notice("<<<<<<<<" #fmt ">>>>>>>>",##__VA_ARGS__)
#   define  log_a_fail(fmt,...) log_a_error("!!!!!!!!" #fmt "!!!!!!!!",##__VA_ARGS__)
#   define	log_a_panic(...)      syslog_ex(log_output_type_a, esyslog_grievous,__MODULE__, __LINE__,  __VA_ARGS__)
#   define	log_a_error(...)      syslog_ex(log_output_type_a, esyslog_error,	__MODULE__, __LINE__,  __VA_ARGS__)
#   define	log_a_warning(...)    syslog_ex(log_output_type_a, esyslog_warning, __MODULE__, __LINE__,  __VA_ARGS__)
#   define	log_a_notice(...)     syslog_ex(log_output_type_a, esyslog_generic, __MODULE__, __LINE__,  __VA_ARGS__)
#   define	log_a_debug(...)      syslog_ex(log_output_type_a, esyslog_debug,	__MODULE__, __LINE__,  __VA_ARGS__)

#endif

#endif

#if (defined(V_LOG))
#if	defined(_OS_WIN_) && (_MSC_VER < 1300) //MS VC++ 7.0 _MSC_VER = 1300, __VA_ARGS__ is unsupported if _MSC_VER < 1300
#	pragma message ("syslog is unsupported under current compiler!")
/*
 we do nothing here,
 just make such compiler happy,
 so he can skip all 'undefined error'!
 */
inline void log_v_panic(...) {}
inline void log_v_error(...) {}
inline void log_v_warning(...) {}
inline void log_v_notice(...) {}
inline void log_v_debug(...) {}
inline void log_v_suc(fmt, ...){}
inline void log_v_fail(fmt, ...){}

#else
#   define  log_v_suc(fmt,...)   log_v_notice("<<<<<<<<" #fmt ">>>>>>>>",##__VA_ARGS__)
#   define  log_v_fail(fmt,...)  log_v_error("!!!!!!!!" #fmt "!!!!!!!!",##__VA_ARGS__)
#	define	log_v_panic(...)     syslog_ex(log_output_type_v, esyslog_grievous,__MODULE__, __LINE__,  __VA_ARGS__)
#	define	log_v_error(...)     syslog_ex(log_output_type_v, esyslog_error,	__MODULE__, __LINE__,  __VA_ARGS__)
#	define	log_v_warning(...)   syslog_ex(log_output_type_v, esyslog_warning, __MODULE__, __LINE__,  __VA_ARGS__)
#	define	log_v_notice(...)    syslog_ex(log_output_type_v, esyslog_generic, __MODULE__, __LINE__,  __VA_ARGS__)
#	define	log_v_debug(...)     syslog_ex(log_output_type_v, esyslog_debug,	__MODULE__, __LINE__,  __VA_ARGS__)

//明文日志接口调用函数
#	define	log_x_error(...)     syslog_ex(log_output_type_x, esyslog_error,	__MODULE__, __LINE__,  __VA_ARGS__)
#	define	log_x_warning(...)   syslog_ex(log_output_type_x, esyslog_warning, __MODULE__, __LINE__,  __VA_ARGS__)
#	define	log_x_notice(...)    syslog_ex(log_output_type_x, esyslog_generic, __MODULE__, __LINE__,  __VA_ARGS__)
#	define	log_x_debug(...)     syslog_ex(log_output_type_x, esyslog_debug,	__MODULE__, __LINE__,  __VA_ARGS__)

#endif

// 日志精简模式的对外暴露接口的宏, 目的是减小包大小, 方式是在编译构建之前, 使用脚本抽取工程里hardcode的日志字符串信息到map文件里, 然后使用脚本将目前工程调用日志打印的接口替换成如下的接口
// 为了不修改构建脚本，将新增的日志打印方式放在 #if (defined(V_LOG)) 宏里
#if	defined(_OS_WIN_) && (_MSC_VER < 1300) //MS VC++ 7.0 _MSC_VER = 1300, __VA_ARGS__ is unsupported if _MSC_VER < 1300
#	pragma message ("syslog is unsupported under current compiler!")
/*
 we do nothing here,
 just make such compiler happy,
 so he can skip all 'undefined error'!
 */
inline void log_s_panic(logid, params_num, ...) {}
inline void log_s_error(logid, params_num, ...) {}
inline void log_s_warning(logid, params_num, ...) {}
inline void log_s_notice(logid, params_num, ...) {}
inline void log_s_debug(logid, params_num, ...) {}

#else
#	define	log_s_panic(logid, params_num, ...)   syslog_ex_s(log_output_type_s, esyslog_grievous, logid, params_num, ##__VA_ARGS__)
#	define	log_s_error(logid, params_num, ...)	  syslog_ex_s(log_output_type_s, esyslog_error, logid, params_num, ##__VA_ARGS__)
#	define	log_s_warning(logid, params_num, ...) syslog_ex_s(log_output_type_s, esyslog_warning, logid, params_num, ##__VA_ARGS__)
#	define	log_s_notice(logid, params_num, ...)  syslog_ex_s(log_output_type_s, esyslog_generic, logid, params_num, ##__VA_ARGS__)
#	define	log_s_debug(logid, params_num, ...)   syslog_ex_s(log_output_type_s, esyslog_debug, logid, params_num, ##__VA_ARGS__)



#endif

#endif

#	define	log_s_panic(logid, params_num, ...)	syslog_ex_s(log_output_type_s, esyslog_grievous, logid, params_num, ##__VA_ARGS__)
#	define	log_s_error(logid, params_num, ...)	syslog_ex_s(log_output_type_s, esyslog_error, logid, params_num, ##__VA_ARGS__)
#	define	log_s_warning(logid, params_num, ...)	syslog_ex_s(log_output_type_s, esyslog_warning, logid, params_num, ##__VA_ARGS__)
#	define	log_s_notice(logid, params_num, ...)	syslog_ex_s(log_output_type_s, esyslog_generic, logid, params_num, ##__VA_ARGS__)
#	define	log_s_debug(logid, params_num, ...)	syslog_ex_s(log_output_type_s, esyslog_debug, logid, params_num, ##__VA_ARGS__)




typedef void (*fnsyslog_hook)(esyslog_type level, const char* module, int line, const char* message, int contentoffset);
typedef void (*fnsyslog_hook_s)(esyslog_type level, const char* message, int contentoffset);

typedef uint32(*fnsysregulartime_hook)();

#ifdef __cplusplus
extern "C" {
#endif
    _ZEGO_API int32 syslog_hook_ex_s(log_output_type output_type, fnsyslog_hook_s hook);
	_ZEGO_API int32 syslog_hook_ex_sa(fnsyslog_hook_s hook);
    _ZEGO_API int32	syslog_hook_ex(log_output_type output_type, fnsyslog_hook hook);
	_ZEGO_API fnsyslog_hook get_syslog_hook_ex();
	_ZEGO_API fnsyslog_hook_s get_syslog_hook_ex_s();
	_ZEGO_API fnsyslog_hook_s get_syslog_hook_ex_sa();
    

    _ZEGO_API int32	syslog_setlevel_ex(log_output_type output_type, esyslog_type level);
    _ZEGO_API void	syslog_ex(log_output_type output_type, esyslog_type level, const char* module, int line, const char* format, ...);
	_ZEGO_API void 	syslog_ex_s(log_output_type output_type, esyslog_type level, int16 logid, int params_num, ...);

	/**
	 init syslog hook to store message.
	 
	 @hook		- hook entry for further use to store message;
	 */
	_ZEGO_API int32 syslog_hook(fnsyslog_hook hook, bool only_output_xplatform_log = false);
    
	_ZEGO_API uint32	sysregulartime_hook(fnsysregulartime_hook hook);

	/**
	 init syslog hook to store message.
	 
	 @hook		- hook entry for further use to store message;
	 */
	_ZEGO_API int32	syslog_setlevel(esyslog_type level);

	/**
	 syslog is a system log interface to send log messages to zegolatform.
	 
	 notice: here we don't store any data to disk, we only redirect message to a callback entry previous initialized by syslog_hook call.
	 
	 @level		- event type, please see 'esyslog_type' above for more detail;
	 @module	- indicate who is calling 'syslog'
	 @format	- event description
	 */
	_ZEGO_API void	syslog(esyslog_type level, const char* module, int line, const char* format, ...)

#ifndef _MSC_VER
	__attribute__ ((format (printf, 4, 5)))
#endif
	;
	
	/*
	 va_list version of syslog
	 
	 @args		- argument list
	 
	 */
	_ZEGO_API void syslogv(fnsyslog_hook func, esyslog_type level, const char* module, int line, const char* format, va_list args);
	_ZEGO_API void syslogv_s(log_bus_type bus_type, esyslog_type level, int16 logid, int params_num, va_list args);
	_ZEGO_API void StopLog(void);
	
#ifdef __cplusplus
};
#endif

#endif /*_ZEGOLOG_INC_*/
