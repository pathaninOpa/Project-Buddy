#ifndef _ZEGOMM_INC_
#define _ZEGOMM_INC_
#pragma once

#ifndef	ZEGO_MMC_CONFIG_DISABLE

#include "mem_check_config.h"

#if defined(_WIN32)||defined(_WIN64)
#ifdef ZEGO_MEM_CHECK_ALL
#include  <stdexcept>
#include "debug_new.h"
#endif

#ifdef ZEGO_MEM_CHECK_PART
#include  <stdexcept>
#include "debug_new.h"
#endif

#ifdef ZEGO_MEM_CHECK_NONE
#include <new>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#endif
#endif

#endif

#ifndef zegofree
#	define	zegofree		free
#endif

#ifndef zegomalloc
#	define	zegomalloc	malloc
#endif

#ifndef zegorealloc
#	define	zegorealloc	realloc
#endif

#ifndef zegomemmove
#	define	zegomemmove	memmove
#endif

#endif /*_ZEGOMM_INC_*/
