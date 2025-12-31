
#ifndef _IUDPCHANNEL_INCLUDE_
#define _IUDPCHANNEL_INCLUDE_

#include "zegorefc.h"
#include "zegostream.h"

//udpchn通道命令
enum
{
	ZEGOUDPCHN_CMD_DISCOVER = 1,//探测包,用于破防火墙
	ZEGOUDPCHN_CMD_FILE	  = 100,//传文件命令字
	//其他命令最好在这里登记下，方式冲突
};

//=====Udp通道协议编解码=============
class _ZEGO_CLS CZEGOUdpChnPacket : public CRefCountSafe
{
public:    
	uint32		m_uCmd;		 //命令
	uint32		m_uSeq;		 //seq
	uint32		m_uSubCmd;	 //子命令
	uint64		m_uSessionId;//回话ID

	uint8		m_isNeedAck; //需要应答
	uint8		m_isAckPK;	 //是应答包

	//Chn层进行快速的应答，提高大数据传输速度,普通的信令不需要关心
	uint8		m_isNeedChnAutoAck; //需要Chn底层自动应答
	uint8		m_isChnAutoAck;		//该包是Chn底层的自动应答
	uint64		m_uChnAutoAckExData;//通道快速应答附带数据

	zego::buffer	m_bufBody;	 //数据本身
public:
	CZEGOUdpChnPacket();
	~CZEGOUdpChnPacket();
};

class _ZEGO_CLS CZEGOUdpChnRetryInfo
{
public:
	CZEGOUdpChnRetryInfo(int32 uSendTimesPer,int32 uRetryCount,int32 uRetryIntervalms);
	int32	m_uSendTimesPer;	//每次重复发送几个包
	int32	m_uRetryCount;		//总共重复多少次
	int32	m_uRetryInterval;	//重复的时间间隔，单位为(ms)
};

class IZEGOUdpChannel;

//注册收到数据
class _ZEGO_CLS IZEGOUdpChnRecvSink
{
public:
	virtual void OnReceiveData(CZEGOUdpChnPacket* pPack,const uint8* pData,uint32 uDataLen,const utf8* uFromIP,uint16 uFromPort,IZEGOUdpChannel* fromChn) = 0;
	virtual ~IZEGOUdpChnRecvSink();
};


//对于需要应答包的回包通知
class _ZEGO_CLS IZEGOUdpChnResponseSink
{
public:
	virtual void OnResponse(CZEGOUdpChnPacket* pSendPack,CZEGOUdpChnPacket* pResponsePack,IZEGOUdpChannel* fromChn) = 0;	
	virtual void OnSendTimeOut(CZEGOUdpChnPacket* pSendPack,IZEGOUdpChannel* fromChn)	= 0;	
	virtual ~IZEGOUdpChnResponseSink();
};

class _ZEGO_CLS IZEGOUdpChannel
{
public:
	virtual boolean Create(uint16 nBindPort=0,const char* bindIP="") = 0;
	virtual void	Close() = 0;
	//virtual void	GetSocketInfo(uint32 &uIP,uint16 &nPort) = 0;
	virtual boolean	Register(IZEGOUdpChnRecvSink *pSink,uint32 uCmd,uint32 uSubCmd=-1, uint64 uSessionId=-1) = 0;
	virtual void	Unregister(IZEGOUdpChnRecvSink *pSink) = 0;
	virtual boolean	Send(const char* uDestIP,uint16 uDestPort,CZEGOUdpChnPacket* pSendPack,IZEGOUdpChnResponseSink* pSink = NULL,CZEGOUdpChnRetryInfo* pRetryInfo = NULL) = 0;
	virtual void    DeleteResponseSink(IZEGOUdpChnResponseSink* pSink) = 0;
	virtual void	SendDiscover(const utf8* uDestIP,uint16 uPort) = 0;
    
    //减少copy的高效率自编码发送，用于大数据传输
    virtual  uint32  GetPacketHeadLen() = 0;
    virtual  boolean CodePacketHead(CZEGOUdpChnPacket *pack,zego::buffer &buf,uint32 uBodyLen) = 0;
    virtual  boolean SendBySelfCode(const zego::strutf8 &strDestIP,uint16 uDestPort,const zego::buffer &bufPacket) = 0;
    
	virtual ~IZEGOUdpChannel();
};

IZEGOUdpChannel* CreateNewUdpChannel();

#endif