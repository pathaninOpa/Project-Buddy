#if !defined(_ZEGOISOCKET_INC_)
#define _ZEGOISOCKET_INC_

#include "zegotypes.h"
#include "zegonet.h"

typedef enum ZEGOSOCKET_EVNET 
{
	ZEGOSOCKET_EVENT_READ		= 0x02,
	ZEGOSOCKET_EVENT_WRITE	= 0x04,
	ZEGOSOCKET_EVENT_READWRITE= ZEGOSOCKET_EVENT_READ|ZEGOSOCKET_EVENT_WRITE
} ZEGOSOCKET_EVNET;


//******************************************************************//
//******************** All Socket Interface ************************//
//******************************************************************//

//******connect tcp*********//

typedef enum ZEGOProxyType
{
	zegoproxy_none,
	zegoproxy_http,
	zegoproxy_socks5
} ZEGOProxyType;

class CZEGOITCPCnnSocket;

class _ZEGO_CLS CZEGOITCPCnnSocketSink
{
public:	
	virtual ~CZEGOITCPCnnSocketSink(){};
	virtual void OnConnected(boolean bSuccess,CZEGOITCPCnnSocket* pCnnSocket,boolean bIsBeClosed=false) = 0;
	virtual	void SetIpAndPort2Bind(const utf8* addr, uint16 port){}
};

class _ZEGO_CLS CZEGOITCPCnnSocket
{
public:	
	virtual				~CZEGOITCPCnnSocket(){};
	virtual	void	    SetIpAndPort2Bind(const utf8* addr, uint16 port){}
	virtual void		SetSink(CZEGOITCPCnnSocketSink* pSink) = 0;
    virtual void        SetIsNat64(bool nat64) = 0;
	virtual boolean		Connect(const utf8* addr, uint16 port,uint32 timeout_ms) = 0;
	virtual zegosocket	Detach() = 0;
	virtual	ZEGOProxyType GetProxyType() { return zegoproxy_none; }
	virtual void        GetProxyInfo(zego::strutf8& ip, uint16& port) {};
	virtual void        GetOuterIPInProxy(zego::strutf8& ip) {};
};


//******listen tcp*********//

class CZEGOITCPListenSocket;

class _ZEGO_CLS CZEGOITCPListenSocketSink
{
public:
	virtual ~CZEGOITCPListenSocketSink(){};
	virtual void OnBind(uint16 uListenPort,CZEGOITCPListenSocket* pListen) = 0;
	virtual void OnAccept(CZEGOITCPListenSocket* pListen) = 0;
	virtual void OnClose(CZEGOITCPListenSocket* pListen)  = 0;
};

class _ZEGO_CLS CZEGOITCPListenSocket
{
public:
	virtual ~CZEGOITCPListenSocket(){};
	virtual void	 SetSink(CZEGOITCPListenSocketSink* pSink) = 0;
	virtual boolean  Listen(const char* bindIP,uint16 uBindPort,uint32 uListenTimeout_ms = 5000 ,int32 nbackbog = 5) = 0;
	virtual boolean  GetSocketName(uint16 &uListenPort) = 0;
	virtual boolean  Attach(zegosocket s,uint32 uListenTimeout_ms) = 0;
	virtual zegosocket Accept() = 0;
	virtual void	 Close() = 0;
};


//******connect socks5 proxy tcp*********//

class IZEGOSock5ProxyTCP;

///SOCKS5代理信息结构
typedef struct tagSock5ProxyInfo
{
	tagSock5ProxyInfo()
	{
		pInerProxyAddr = NULL;			
		uInerProxyPort = 0;				
		pProxyUserName = NULL;			
		pProxyPasswd   = NULL;			
		pDestAddr	   = NULL;			
		uDestPort	   = 0;		
		customMethod	   = NULL;
		customMethodLen = 0;
		customDataLen = 0;
		customData = NULL;
		nProxyType = zegoproxy_none;
		udpMode = 0;
	}

	~tagSock5ProxyInfo()
	{
		if( pInerProxyAddr ) free(pInerProxyAddr);
		if( pProxyUserName ) free(pProxyUserName);
		if( pProxyPasswd )	 free(pProxyPasswd);
		if( pDestAddr )		 free(pDestAddr);
		if (customData)		 free(customData);
		if (customMethod)	 free(customMethod);
	}
	
	utf8*		pInerProxyAddr;	//代理服务地址	
	uint16		uInerProxyPort;	//代理服务端口	
	utf8*		pProxyUserName;	//认证用户名	
	utf8*		pProxyPasswd;	//认证密码	
	utf8*		pDestAddr;		//目标地址	
	uint16		uDestPort;		//目标端口
	utf8*       customMethod;	//认证方式 0x02 默认 （自定义认证 目前只适用于 zego 使用。）
	uint16		customMethodLen;
	uint16		customDataLen;
	utf8* customData;
	ZEGOProxyType nProxyType;
	uint16		udpMode ;
} Sock5ProxyInfo;


//******UDP*********//

class CZEGOIUDPSocket;

class _ZEGO_CLS CZEGOIUDPSocketSink
{
public:
	virtual ~CZEGOIUDPSocketSink(){};
	virtual void OnBind(boolean bSuccess,uint16 uBindPort,CZEGOIUDPSocket* pUdpSocket) = 0;
	virtual void OnRecv(const uint8* pData,uint32 uDataLen,const utf8* uFromIP,uint16 uFromPort,CZEGOIUDPSocket* pUdpSocket) = 0;
};

class _ZEGO_CLS CZEGOIUDPSocket
{
public:
	virtual ~CZEGOIUDPSocket(){};
	
    virtual boolean  Create(uint32 uTimeout_ms,const char* bindIP = "",uint16 uBindPort = 0) = 0;
	virtual void	 SetSink(CZEGOIUDPSocketSink* pSink) = 0;
	virtual void	 Attach(zegosocket s, int type) = 0;
	virtual zegosocket Detach() = 0;
	virtual zegosocket NativeSocket() = 0;
	virtual int32	 SendTo(const utf8 *IP, uint16 wPort, const uint8 *pData,uint32 uBufLen) = 0;
    virtual int32	SendToV6(const utf8 *IP, int nIpType, uint16 uPort, const uint8 *pData,uint32 uBufLen, bool bIsNat64 = true) = 0;
	//virtual int32	 SendTo(uint32 uIP, uint16 wPort, const uint8 *pData,uint32 uBufLen) = 0;
	virtual int32	 RecvFrom(uint8 *pData,uint32 ulen,const utf8* uFromIP,uint16 &uFromPort) = 0;
	
	virtual boolean  SetSendBufferSize(int32 size) = 0;
	virtual boolean  SetRecvBufferSize(int32 size) = 0;
	virtual boolean  GetSocketName(uint16 &port) = 0;
	virtual boolean  SelectEvent(ZEGOSOCKET_EVNET e,boolean bIsToClear) = 0;
	virtual void	 Close() = 0;	
};

class CZEGOIUDPSocketICMP;
class _ZEGO_CLS CZEGOIUDPSocketICMPSink
{
public:
	virtual ~CZEGOIUDPSocketICMPSink() {};
	virtual void OnRecv(const utf8* uFromIP, uint16 uFromPort, int ttl,CZEGOIUDPSocketICMP* pUdpSocket) = 0;
};

class _ZEGO_CLS CZEGOIUDPSocketICMP
{
public:
	virtual ~CZEGOIUDPSocketICMP() {};

	virtual boolean  Create(uint32 uTimeout_ms, int nType = PF_INET) = 0;
	virtual void	 SetSink(CZEGOIUDPSocketICMPSink* pSink) = 0;
	virtual int32	SendICMP(const utf8 *ip, uint16 uPort,uint32 uDataLen = 0) = 0;
	virtual bool SetTTL(int ttl) = 0;

	virtual void	 Close() = 0;
};

//******Communication TCP*********//

class CZEGOITCPSocket;

class _ZEGO_CLS CZEGOITCPSocketSink
{
public:	
	virtual ~CZEGOITCPSocketSink(){};
	virtual void OnRecv(CZEGOITCPSocket* pTCPSocket)	= 0;
	virtual void OnSend(CZEGOITCPSocket* pTCPSocket)	= 0;
	virtual void OnClose(CZEGOITCPSocket* pTCPSocket)	= 0;
};

class _ZEGO_CLS CZEGOITCPSocket
{
public:
	
	virtual ~CZEGOITCPSocket(){};
	
	virtual void	 SetSink(CZEGOITCPSocketSink* pSink) = 0;
	virtual void	 Attach(zegosocket s) = 0;
	virtual zegosocket Detach() = 0;
    virtual zegosocket NativeSocket() = 0;
	
	virtual int32	Send(const void *pData,uint32 ulen) = 0;
	virtual int32	Recv(void *pData,uint32 ulen) = 0;
	
	virtual uint32  GetUnReadDataLen() = 0;
	virtual boolean SetSendBufferSize(int32 size) = 0;
    virtual boolean GetSendBufferSize(int32 &size) = 0;
	virtual boolean SetRecvBufferSize(int32 size) = 0;
    virtual boolean GetRecvBufferSize(int32 &size) = 0;
	virtual boolean SetNoDelay(boolean bIsToSet = true) = 0;
	virtual boolean GetSocketName(uint16 &port) = 0;
	virtual boolean GetPeerName(uint16 &port) = 0;
	virtual boolean SelectEvent(ZEGOSOCKET_EVNET e,boolean bIsToClear = false) = 0;
	virtual void	Close() = 0;
};


//******Communication TCP For Combine Logic Packet*********//

typedef boolean (*zegocombinetcp_getpklenfun)(const uint8* pData,uint32 len,uint32 &uPacketTotalLen);

class CZEGOICombineTCPSocket;

class _ZEGO_CLS CZEGOICombineTCPSocketSink
{
public:	
	virtual ~CZEGOICombineTCPSocketSink(){};
	virtual void OnRecv(const uint8* pPacket,uint32 uPacketlen,CZEGOICombineTCPSocket* pTCPSocket)	= 0;
	virtual void OnSend(CZEGOICombineTCPSocket* pTCPSocket)	= 0;
	virtual void OnClose(CZEGOICombineTCPSocket* pTCPSocket)	= 0;
};

class _ZEGO_CLS CZEGOICombineTCPSocket
{
public:
	virtual ~CZEGOICombineTCPSocket(){};
	
	virtual void	 SetSink(CZEGOICombineTCPSocketSink* pSink) = 0;
	virtual boolean	 SetCombineInfo(uint32 uPacketHeadLen,zegocombinetcp_getpklenfun fun,uint32 maxPacketSize = 2048) = 0;
	virtual void	 Attach(zegosocket s) = 0;
	virtual zegosocket Detach() = 0;
	
	virtual int32	Send(const void *pData,uint32 ulen) = 0;
	virtual uint32  GetUnReadDataLen() = 0;
	virtual boolean SetSendBufferSize(int32 size) = 0;
	virtual boolean SetRecvBufferSize(int32 size) = 0;
	virtual boolean SetNoDelay(boolean bIsToSet = true) = 0;
	virtual boolean GetSocketName(uint16 &port) = 0;
	virtual boolean GetPeerName(uint16 &port) = 0;
	virtual boolean SelectEvent(ZEGOSOCKET_EVNET e,boolean bIsToClear = false) = 0;
	virtual void	Close() = 0;
};




//****************************************************************************//
//******Help API To Create the Socket Interface and Set Network Proxy*********//
//****************************************************************************//


	
//set global proxy
_ZEGO_API void ZEGOSetGlobalProxyInfo(ZEGOProxyType type, const utf8* proxyip, uint16 proxyport, const utf8* usename, const utf8* password);

//get golbal proxy , notice: the utf8** direct refence the golbal data,unneed to free
_ZEGO_API void ZEGOGetGlobalProxyInfo(ZEGOProxyType &type,utf8** proxyip = NULL,uint16* proxyport = NULL,utf8** usename = NULL,utf8** password = NULL);

//******Create Cnn TCP******/

//create connet socket by global proxy
_ZEGO_API CZEGOITCPCnnSocket*		ZEGOCreateCnnTCPSocket();

//appiont create a none proxy cnn socket
_ZEGO_API CZEGOITCPCnnSocket*		ZEGOCreateNoneProxyCnnTCPSocket();

//appiont create a proxy cnn socket
_ZEGO_API CZEGOITCPCnnSocket*		ZEGOCreateProxyCnnTCPSocket(ZEGOProxyType type, const utf8* proxyip, uint16 proxyport, const utf8* usename, const utf8* password);


//******Create Socks5 Proxy TCP******/

//appiont create a socks5 proxy cnn socket
_ZEGO_API CZEGOITCPCnnSocket*		ZEGOCreateCnnSock5ProxyTCPSocket(Sock5ProxyInfo& proxyInfo, boolean bForUDP = false);


//******Create Listen TCP******/

//create listen socket by global proxy
_ZEGO_API CZEGOITCPListenSocket*	ZEGOCreateTCPListenSocket();

//appiont create a none proxy listen socket
_ZEGO_API CZEGOITCPListenSocket*	ZEGOCreateNoneProxyTCPListenSocket();

//appiont create a proxy listen socket
_ZEGO_API CZEGOITCPListenSocket*	ZEGOCreateProxyTCPListenSocket(ZEGOProxyType type,const utf8* proxyip,uint16 proxyport, const utf8* usename,const utf8* password);



//******Create UDP ******/

//create udp socket by global proxy
_ZEGO_API CZEGOIUDPSocket*			ZEGOCreateUDPSocket();

//appiont create a none proxy udp socket
_ZEGO_API CZEGOIUDPSocket*			ZEGOCreateNoneProxyUDPSocket();

//appiont create a proxy udp socket
_ZEGO_API CZEGOIUDPSocket*			ZEGOCreateProxyUDPSocket(ZEGOProxyType type,const utf8* proxyip,uint16 proxyport, const utf8* usename,const utf8* password);

_ZEGO_API CZEGOIUDPSocket*			ZEGOCreateSocks5ProxyUDPSocket(CZEGOITCPCnnSocket* dependTcp);
//appiont create a none proxy udp socket for icmp
_ZEGO_API CZEGOIUDPSocketICMP*			ZEGOCreateNoneProxyUDPSocketICMP();

//******Create Communication TCP******/

//create a communication tcp
_ZEGO_API CZEGOITCPSocket*			ZEGOCreateTCPSocket();

//create a communication tcp by combine logic packet( send or recv always a complete logic packet)
_ZEGO_API CZEGOICombineTCPSocket*	ZEGOCreateCombineTCPSocket();

//create a communication tcp by combine logic packet( send or recv always a complete logic packet)
//and support mutilthread to send
_ZEGO_API CZEGOICombineTCPSocket*	ZEGOCreateCombineTCPSocket_MutilThread();


#endif
