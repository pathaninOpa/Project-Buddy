//
//  Created by gavin huang on 12-08-02.
//  Copyright (c) 2012骞?tencent. All rights reserved.
//

#ifndef _ZEGOIHTTPSERVER_INCLUDE_
#define _ZEGOIHTTPSERVER_INCLUDE_

#include "zegostream.h"
#include "zegostreamreader.h"

enum 
{
    HTTP_ERR_SUCCESS         = 0,
    HTTP_ERR_TIMEOUT         = 1,
    HTTP_ERR_BECLOSE         = 2,
    HTTP_ERR_HEAD            = 3,
    HTTP_ERR_WRITEFAIL       = 4,
    HTTP_ERR_READFAIL        = 5,
    HTTP_ERR_OPENFILEFAIL    = 6,
    HTTP_ERR_CREATEFILEFAIL  = 7,
};

class _ZEGO_CLS CHttpListenSocketNotify
{
public:
    virtual ~CHttpListenSocketNotify() {};
    virtual void OnClose(uint16 uListenPort) = 0;
};


class _ZEGO_CLS CHttpRequestNotify
{
public:
    virtual ~CHttpRequestNotify() {};
    virtual void OnRequest(uint64 uSessionId, const zego::strutf8 &strRequestType, const zego::strutf8 &strRequest,const zego::strutf8 strLocalIP,const zego::strutf8 strFromIP,uint16 uFromPort) = 0;
};

class _ZEGO_CLS CHttpNotify
{
public:
    virtual ~CHttpNotify(){};
    virtual void OnProgress(uint64 uSessionId, uint64 uCurLen, uint64 uTotalLen, uint32 uSpeed_Byte_S,uint64 uTransferIncrementLen)= 0;
    virtual void OnComplete(uint64 uSessionId, const zego::strutf8 &strHead, const zego::strutf8 &strContent, int32 nErrCode, uint64 fileLen = 0)= 0;
};

class _ZEGO_CLS IHttpServer
{
public:
	virtual ~IHttpServer(){};
    
    //listen mgr,permit start multi listen
    virtual boolean IsStart(uint16 uBindPort) = 0;
    virtual boolean StartListen(uint16 uBindPortBegin,uint16 uBindPortEnd,uint16 &uBindPort) = 0;
    virtual void    StopListen(uint16 uBindPort, boolean bStopAllConnections = true) = 0;
    
    //register notify
    virtual void RegisterListenSocketNotify(uint16 uBindPort,CHttpListenSocketNotify* pListenSocketNotify) = 0;
    virtual void RegisterRequest(const zego::strutf8& strRequestType,uint16 uBindPort,CHttpRequestNotify* pNotify) = 0;
    virtual void UnRegisterRequest(const zego::strutf8& strRequestType,uint16 uBindPort) = 0;
    
    //session operator
    virtual void Refuse(uint64 uSessionId, const zego::strutf8& strResponse, uint32 uHttpErrorCode = 400) = 0;
    
    virtual void Accept_SendBuf(uint64 uSessionId, const zego::strutf8& strResBuf, 
                                CHttpNotify* pNotify = NULL, boolean bNotifyProgress = false) = 0;
    
    virtual void Accept_RecvFile(uint64 uSessionId, const zego::strutf8& strResponse, const zego::strutf8& strRecvFilePath,
                                 CHttpNotify* pNotify = NULL, boolean bNotifyProgress = false) = 0;
    
    virtual void Accept_RecvBuff(uint64 uSessionId, const zego::strutf8& strResponse,
                                 CHttpNotify* pNotify = NULL, boolean bNotifyProgress = false) = 0;
    
	virtual void Accept_SendFile(uint64 uSessionId, const zego::strutf8& strSendFilePath,
                                 CHttpNotify* pNotify = NULL, boolean bNotifyProgress = false,IZEGOStreamReader* pStreamRead = NULL) = 0;
    
    virtual void Post(uint64 uSessionId, const zego::strutf8 &strHead, const zego::strutf8 &strContent, CHttpNotify* pNotify = NULL, boolean bNotifyProgress = false) = 0;
    
    virtual void Cancel(uint64 uSessionId) = 0;
    
    virtual void GetLocalIP(uint64 uSessionId, zego::strutf8 &strLocalIP) = 0;
    virtual void GetPeerIP(uint64 uSessionId, zego::strutf8 &strPeerIP) = 0;
    virtual uint64 GetRequestContentLen(uint64 uSessionId) = 0;

	virtual void AddHead(uint64 uSessionId, const zego::strutf8& strHead) = 0;
};

_ZEGO_API IHttpServer *GetHttpServerInstance();

#endif
