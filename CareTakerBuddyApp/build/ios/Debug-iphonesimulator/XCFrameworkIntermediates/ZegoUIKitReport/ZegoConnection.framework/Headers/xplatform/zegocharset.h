#if !defined(_ZEGOCHARSET_INC_)
#define _ZEGOCHARSET_INC_
#pragma once

#include "zegotypes.h"
#include "zegoexcept.h"
#include "zegostream.h"
/*
#ifdef __cplusplus
extern "C" {
#endif
*/
_ZEGO_API boolean zegogbk2utf16(const gbk* str,uint32 len,zego::strutf16 &strutf16);

_ZEGO_API boolean zegoutf162gbk(const utf16* pstr, uint32 ulen,zego::strutf8 &strgbk);
	
_ZEGO_API boolean zegoutf82utf16(const utf8* str, uint32 len,zego::strutf16 &strutf16);

_ZEGO_API boolean zegoutf162utf8(const utf16* str, uint32 len,zego::strutf8 &strutf8);

_ZEGO_API
uint32 zegogbk2utf16(const gbk* str,uint32 len,utf16 **pputf16);

_ZEGO_API
uint32 zegoutf162gbk(const utf16* pstr, uint32 ulen,gbk **pgbk);
	
_ZEGO_API
uint32 zegoutf82utf16(const utf8* str, uint32 len,utf16 **pputf16);

_ZEGO_API
uint32 zegoutf162utf8(const utf16* str, uint32 len,utf8** pputf8);

/*	
#ifdef __cplusplus
};
#endif
*/
#endif /*_ZEGOCHARSET_INC_*/
