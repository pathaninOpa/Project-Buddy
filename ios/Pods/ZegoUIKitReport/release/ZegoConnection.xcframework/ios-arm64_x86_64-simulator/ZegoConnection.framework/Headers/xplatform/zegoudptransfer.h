#pragma once

#include "zegoudpchanneli.h"
#include "zegostreamreader.h"

class CZEGOUdpTransferPostRequest : public CRefCountSafe
{
public:
	uint64		uFileSize;
	zego::buffer  bufPostReq;
	uint64		uSessionId;
	uint32		uSeq;
    zego::strutf8		uFromIP;
	uint16		uFromPort;
};

class CZEGOUdpTransferGetRequest : public CRefCountSafe
{
public:
	zego::buffer  bufGetReq;
	uint64		uSessionId;
	uint32		uSeq;
	uint64		uRange;
    zego::strutf8		uFromIP;
	uint16		uFromPort;
};


class IZEGOUdpTransferSender;

class  IZEGOUdpTransferSenderSink  
{
public:
	IZEGOUdpTransferSenderSink();
	virtual ~IZEGOUdpTransferSenderSink();
	virtual void OnSendProgress(IZEGOUdpTransferSender *pSender, uint64 dwProgress, uint64 dwProgressMax,uint32 uSpeed_Byte_S){};
	virtual void OnSendComplete(IZEGOUdpTransferSender *pSender, uint32 dwErrorCode,const zego::strutf16 &strErrorDisc) = 0;
};

class IZEGOUdpTransferSender
{
public:
	virtual ~IZEGOUdpTransferSender();
	virtual boolean Init(IZEGOUdpTransferSenderSink* pSink,const char* uDestIP,uint16 uDestPort,IZEGOUdpChannel* pChn,uint64 uSessionId) = 0;
	virtual boolean SendBySelfPost(zego::buffer &bufPostReq,zego::strutf16 *pstrSendFileName,IZEGOStreamReader* pFileReader = NULL) = 0;
	virtual boolean AcceptByPeerGet(CZEGOUdpTransferGetRequest* pReq,zego::strutf16 *pstrSendFileName,IZEGOStreamReader* pStreamReader = NULL) = 0;
	virtual void	RefuseByPeerGet(uint32 nErrCode,zego::strutf16 &strErr) = 0;
	virtual uint64  GetSessionId() = 0;
	virtual void	Cancel() = 0;
};


class IZEGOUdpTransferReceiver;

class  IZEGOUdpTransferReceiverSink 
{
public:
	IZEGOUdpTransferReceiverSink();
	virtual ~IZEGOUdpTransferReceiverSink();
	virtual void OnRecvProgress(IZEGOUdpTransferReceiver *pReceiver, uint64 dwProgress, uint64 dwProgressMax,uint32 uSpeed_Byte_S){};
	virtual void OnRecvComplete(IZEGOUdpTransferReceiver *pReceiver, uint32 dwErrorCode,const zego::strutf16 &strErrorDisc) = 0;
};

class IZEGOUdpTransferReceiver
{
public:
	virtual ~IZEGOUdpTransferReceiver();
	virtual boolean Init(IZEGOUdpTransferReceiverSink *pSink,IZEGOUdpChannel* pChn,const char* uDestIP,uint16 uDestPort,uint64 uSessionId) = 0;
	virtual boolean ReceiveByPeerPost(CZEGOUdpTransferPostRequest* pReq,const zego::strutf16 &strSaveFileName) = 0;
	virtual boolean RefuseByPeerPost(uint32 uCode,zego::strutf16 &strDisc) = 0;
	virtual boolean ReceiveSelfGet(zego::buffer &bufGetReq,const zego::strutf16 &strSaveFileName) = 0;
	virtual boolean GetReceiveFilePath(zego::strutf16 &strFileName) = 0;
	virtual uint64  GetSessionId() = 0;
	virtual void	Cancel() = 0;
};

class  IXUdpTransferServiceSink
{
public:
	virtual ~IXUdpTransferServiceSink();
	virtual void OnTransferPostRequest(CZEGOUdpTransferPostRequest* pReq) = 0;
	virtual void OnTransferGetRequest(CZEGOUdpTransferGetRequest* pReq)   = 0;
};

class IZEGOUdpTransferService
{
public:
	virtual ~IZEGOUdpTransferService();
	virtual void Init(IZEGOUdpChannel* pChn,IXUdpTransferServiceSink* pSink) = 0;
    
    //用于发送滑动窗口快速启动的参数记录和获取
    virtual void AddParamForSliderWinQuickStart(const char* uDestIP,uint16 uDestPort,CRefCountSafe* pParam)   = 0;
    virtual void GetParamForSliderWinQuickStart(const char* uDestIP,uint16 uDestPort,CRefCountSafe** ppParam) = 0;
};


IZEGOUdpTransferService*  GetUdpTransferServiceInstance();
void                    DestoryUdpTransferServiceInstance();

IZEGOUdpTransferSender*   CreateZEGOUdpTransferSender();
IZEGOUdpTransferReceiver* CreateZEGOUdpTransferReceiver();

