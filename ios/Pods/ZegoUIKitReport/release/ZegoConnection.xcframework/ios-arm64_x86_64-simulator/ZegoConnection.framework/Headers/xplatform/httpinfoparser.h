#pragma once


/** 
 @file      
 @brief	    Http	
 @version	2012/08/02 Gavinhuang Create
 */

#ifndef _HTTP_INFO_PARSER_
#define _HTTP_INFO_PARSER_

#include "zegostream.h"
#include "zegostring.h"

class CHttpInfoParser
{
public:
    
    // 返回 -1 表示没有找到结束点
    _ZEGO_API static boolean      GetHttpHeadInfo(const utf8* pSrc,const uint32 nSrcLen,uint32 &nBeginPos,uint32 &uHeadLen,uint64 &uContentLen,zego::strutf8 &strboundary);
    
    //返回true，表示得到strFileName
    _ZEGO_API static boolean      GetDispositionFileName(const zego::strutf8 &strSrc,zego::strutf8 &strFileName);
    
    //返回true，表示得到uRange
    _ZEGO_API static boolean      GetRange(const zego::strutf8 &strSrc,uint64 &uRange);
    
    //返回true，表示得到strInfoValue
    _ZEGO_API static boolean      QueryInfo(const zego::strutf8 &strSrc1,const utf8* strQueryInfo,zego::strutf8 &strQueryValue);
    
    //返回true，表示得到uValue
    _ZEGO_API static boolean      QueryInfo(const zego::strutf8 &strSrc,const utf8* strQueryInfo,uint32 &uValue);
    
    //组装Server回包头
    _ZEGO_API static zego::strutf8  MakeSvrResHead(uint32 uRetCode,uint64 uContentLength,boolean bKeepalive = true,boolean bgzip = false,int64 Range = 0);
    
    _ZEGO_API static zego::strutf8  MakeSvrResHead(uint32 uRetCode,const zegostl::string& sCustomHead, uint64 uContentLength,boolean bKeepalive = true,boolean bgzip = false,int64 Range = 0);
    //返回true，表示得到strPureFileName
    _ZEGO_API static boolean GetPureFileName(const zego::strutf16 &strFileName,zego::strutf16 &strPureFileName);
};

#endif