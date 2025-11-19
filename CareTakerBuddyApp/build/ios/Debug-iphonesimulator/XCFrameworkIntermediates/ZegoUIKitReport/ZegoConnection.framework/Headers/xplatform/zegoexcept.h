#ifndef _ZEGOEXCEPT_INC_
#define _ZEGOEXCEPT_INC_
#pragma once

#include "zegotypes.h"
#include <assert.h>

#if defined(__cplusplus) && !defined(NEXCEPT)
#	include <exception>
#	include <stdexcept>
#	ifndef _EXCEPT_STD_
#		define _EXCEPT_STD_	std
#	endif
#	define	zegothrow(e)			throw _EXCEPT_STD_::e
#else
#	if defined(_OS_WIN_)
#		define zegothrow(e)		{__asm int 3}
#	else
#		ifdef __i386__
#			define zegothrow(e)	{__asm__ __volatile__("int3\nnop\nnop\n");}
#		elif defined(__arm__)
#			define zegothrow(e)	{__asm__ __volatile__("bkpt 1\nnop\n");}
#		else
#			define zegothrow(e)	((void)0)
#		endif
#	endif
#endif

#endif /*_ZEGOEXCEPT_INC_*/
