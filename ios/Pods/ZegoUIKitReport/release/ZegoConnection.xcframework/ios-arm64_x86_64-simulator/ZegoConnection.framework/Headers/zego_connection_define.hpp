#ifndef zego_connection_define_hpp
#define zego_connection_define_hpp

#include <functional>
#include <memory>
#include <string>
#include <map>
#include <mutex>
#include <vector>

#include <zegotypes.h>

#define ZEGOCONNECTION_API // * nothing

/*
#ifdef WIN32
	#ifdef ZEGOCONNECTION_EXPORTS
		#define ZEGOCONNECTION_API __declspec(dllexport)
	#elif defined(ZEGOCONNECTION_STATIC)
		#define ZEGOCONNECTION_API // * nothing
	#else
		#define ZEGOCONNECTION_API __declspec(dllimport)
	#endif
#else
	#define ZEGOCONNECTION_API __attribute__((visibility("default")))
#endif
*/

namespace ZEGO
{

namespace CONNECTION
{

 
	using ZCSeq = uint32;
	using ZCError = uint32;
	
	////////////////////////////////错误码//////////////////////////////////////////
    const ZCError   kZCSuccess            = 0;                            //执行成功
    const ZCError   kZCInvalidCode        = 1;                            //占位错误码，理论上不会上报这个错误码

    //Http errorCode
    const ZCError   kZCEHttpInnerBase        = 1100000;
    const ZCError   kZCEHttpRequestBase      = 1200000;
    const ZCError   kZCEHttpRespondBase      = 1300000;
    
    const ZCError   kZCENotInited        = kZCEHttpInnerBase + 1;                //未初始化
    const ZCError   kZCEInvalidMulitPart = kZCEHttpInnerBase + 2;
    const ZCError   kZCEHttpReqTooMany   = kZCEHttpInnerBase + 1001;             //请求过于频繁
    const ZCError   kZCEInvalidParams    = kZCEHttpInnerBase + 2001;                //无效输入参数
    const ZCError   kZCEHttpInvalidUrl   = kZCEHttpInnerBase + 2002;
    const ZCError   kZCEHttpRequestDnsFailNoIP = kZCEHttpInnerBase + 2003; //请求无ip 一般dns失败，且没有保底ip
	const ZCError   kZCEOpenFileFailed	 = kZCEHttpInnerBase + 3001;				//打开文件失败
        
    //net detector errorCode ,和trace 错误码 net worok 会调用detector
    const ZCError   kZCEDNSResolve              = 1000404;
    const ZCError   kZCENetworkConnect          = 1001001;
    const ZCError   kZCETCPDetectTimeout        = 1001002;    //TCP connect 超时
    const ZCError   kZCETCPDetectLaunchFailed   = 1001003;    //调用TCP Detect 失败
    const ZCError   kZCETCPDetectConnectFailed  = 1001004;    //TCP connect 失败
    const ZCError   kZCEHTTPDetectRepeat        = 1001005;
    const ZCError   kZCEHTTPDetectLaunchFailed  = 1001006;    //调用HTTP Detect 失败
	const ZCError   kZCEDetectParamError = 1001007;    //探测模块探测错误
	const ZCError   kZCEDetectSendDataError = 1001008;
	const ZCError   kZCETCPDetectRepeat = 1001009;
	const ZCError   kZCEUDPDetectRepeat = 1001010;

	const ZCError kZCNetworkTraceParamError = 1002001; //启动network trace错误
	const ZCError kZCNetworkTraceNetBroken = 1002002; //net work trace 由于检测到网络中断而断开
	const ZCError kZCNetworkTraceRouteSocketError = 1002020; //socket error
	const ZCError kZCNetworkTraceRouteDnsError = 1002021; //dns error
	const ZCError kZCNetworkTraceSendICMPError = 1002022; //dns error
	const ZCError kZCNetworkTraceRouteMaxJumpTimeout = 1002023;

 

    const ZCError kZCNtpServerParamError =  1003001; //启动ZegoNS请求ntp服务器地址错误
    const ZCError kZCNtpSyncFailError    =  1003002; //没有可靠的ntp数据可同步
    const ZCError kZCNtpHighPLRError     =  1003003; //ntp服务器丢包率过大
    const ZCError kZCNtpSyncTimeOutError =  1003004; //ntp同步超时
    const ZCError kZCNtpDNSResolveError  =  1003404; //DNS解析ntp服务器地址错误

    const ZCError kZCAgentProbeNotAllow       = 1004000;  //不允许探测
    const ZCError kZCZegoNoZegoNSAddressError = 1004001;  //使用zegons获取服务地址参数错误
    const ZCError kZCZegoZegoNSRequestError = 1004002; //使用zegons 发起请求错误
    const ZCError kZCAgentProbeRecvActionStop = 1004003;  //探测节点收到服务关闭连接通知
    const ZCError kZCLocalDNSOneSecondsTimeOut = 1004004;  //本地域名解析超时 默认1s 超过会启用zegons
    const ZCError kZCLocalDnsTwoSecondsTimeout = 1004005;  //local dns 本地域名解析默认2s超时
    const ZCError kZCZegoNSTwoSecondsTimeout = 1004006;  //zegons 本地域名解析超时
    const ZCError kZCDNSResolveFailed = 1004007; // DNS 解析失败，单次解析结果为空
    
    const ZCError kZCZegoAgentHttpDNSTimeoutError = 1005002; //

    const ZCError kZCZegoAgentCreateReportObjectError = 1006001; //
    const ZCError kZCZegoAgentRtcReportTimeout = 1006002;  //60s 超时

    //net agent errorCode
    const ZCError kZCEAgentInnerBase     =  5000000;
    const ZCError kZCEAgentDispatchBase  =  5100000;
    const ZCError kZCEAgentQuicBase      =  5200000;
    const ZCError kZCEAgentTcpBase      =  5300000;
    const ZCError kZCEAgentTaskBase      =  5500000;
    const ZCError kZCEAgentHttpCodeBase  =  5600000;
    const ZCError kZCEAgentServerCodeBase = 5700000;


    const ZCError kZCZegoInitConfigServerInternalError = kZCEAgentServerCodeBase + 20008;  //server 内部错误
    const ZCError kZCZegoInitConfigInvaildAppid = kZCEAgentServerCodeBase + 20012;  //无效appid
    const ZCError kZCZegoInitConfigInvaildAppSign = kZCEAgentServerCodeBase + 20014;  //签名错误
    const ZCError kZCZegoUnsupportMsgType = kZCEAgentServerCodeBase + 20003;
    const ZCError kZCZegoUnsupportService = kZCEAgentServerCodeBase + 20010;
    const ZCError kZCZegoAccessDenied = kZCEAgentServerCodeBase + 20018; // 访问被拒绝，开启了域外 ip 拒绝服务的限制
    const ZCError kZCZegoSpecialCustomerReuse = kZCEAgentServerCodeBase + 20019;  //拒绝服务，特殊客户，需使用特殊版本appid

    const ZCError kZCEAgentReqFrequency         = kZCEAgentInnerBase + 1001;
    const ZCError kZCEAgentInvalidParam         = kZCEAgentInnerBase + 2001;
    const ZCError kZCEAgentConnectAbort         = kZCEAgentInnerBase + 1;
    const ZCError kZCEAgentConnectLaunchFailed  = kZCEAgentInnerBase + 2;
    const ZCError kZCEAgentConnectCancel        = kZCEAgentInnerBase + 3;
    const ZCError kZCEAgentConnectFailed        = kZCEAgentInnerBase + 4;
    const ZCError kZCEAgentLogicError1          = kZCEAgentInnerBase + 3001;
    const ZCError kZCEAgentLogicError2          = kZCEAgentInnerBase + 3002;
    const ZCError kZCEAgentLogicError3          = kZCEAgentInnerBase + 3003;
    
    const ZCError kZCEAgentConnctServerAbort    = kZCEAgentInnerBase + 10;
    const ZCError kZCEAgentConnctRetryCurrentNode = kZCEAgentConnctServerAbort + 2;
    const ZCError kZCEAgentConnctRetryNextNode= kZCEAgentConnctServerAbort + 3;
    const ZCError kZCEAgentConnctRetryNextGroup = kZCEAgentConnctServerAbort + 4;
    const ZCError kZCEAgentConnctAbort = kZCEAgentConnctServerAbort + 5;
    const ZCError kZCEAgentConnctIdlePushClose = kZCEAgentConnctServerAbort + 6; //服务推送空闲链接

    const ZCError kZCEAgentDispatchDNSResolveFailed           = kZCEAgentDispatchBase + 404;
    const ZCError kZCEAgnetDispatchQuicNoMoreIPAddress        = kZCEAgentDispatchBase + 8;
    const ZCError kZCEAgnetDispatchRequestTimeout             = kZCEAgentDispatchBase + 9;
    const ZCError kZCEAgentDispatchHttpResponseData           = kZCEAgentDispatchBase + 10;
    const ZCError kZCEAgentDispatchLaunchFailed               = kZCEAgentDispatchBase + 11;
    const ZCError kZCEAgentDispatchBeyondMaxRetry             = kZCEAgentDispatchBase + 12;
    const ZCError kZCEAgentDispatchCanceled                   = kZCEAgentDispatchBase + 13;
    const ZCError kZCEAgentDispatchAbort                      = kZCEAgentDispatchBase + 14;
    
    const ZCError kZCEAgentQUICInternalError                  = kZCEAgentQuicBase + 1; // Connection has reached an invalid state.
    const ZCError kZCEAgentQUICStreamTermination                = kZCEAgentQuicBase + 2; // There were data frames after the a fin or reset.
    const ZCError kZCEAgentQUICInvalidPacketHeader            = kZCEAgentQuicBase + 3;// Control frame is malformed.
    const ZCError kZCEAgentQUICInvalidFrameData               = kZCEAgentQuicBase + 4; // Frame data is malformed.
    const ZCError kZCEAgentQUICInvalidFecData                 = kZCEAgentQuicBase + 5;   // FEC data is malformed.
    const ZCError kZCEAgentQUICInvalidRstStreamData           = kZCEAgentQuicBase + 6; // RST_STREAM frame data is malformed.
    const ZCError kZCEAgentQUICInvalidConnectionCloseData     = kZCEAgentQuicBase + 7;  // CONNECTION_CLOSE frame data is malformed.
    const ZCError kZCEAgentQUICInvalidGoawayData              = kZCEAgentQuicBase + 8;// GOAWAY frame data is malformed.
    const ZCError kZCEAgentQUICInvalidAckData                 = kZCEAgentQuicBase + 9; // ACK frame data is malformed.
    const ZCError kZCEAgentQUICInvalidVersionNegotiation                 = kZCEAgentQuicBase + 10;  // Version negotiation packet is malformed
    const ZCError kZCEAgentQUICPublicRstPacket                 = kZCEAgentQuicBase + 11;  // Public RST packet is malformed.
    const ZCError kZCEAgentQUICDecryptionFailure                 = kZCEAgentQuicBase + 12; // There was an error decrypting.
    const ZCError kZCEAgentQUICEecryptionFailure                 = kZCEAgentQuicBase + 13; // There was an error encrypting.
    const ZCError kZCEAgentQUICPeerGoingAway                  = kZCEAgentQuicBase + 16;
    const ZCError kZCEAgentQUICInvalidStreamID                = kZCEAgentQuicBase + 17;
    const ZCError kZCEAgentQUICPublicReset                    = kZCEAgentQuicBase + 19;
    const ZCError kZCEAgentQUICInvalidVersion                    = kZCEAgentQuicBase + 20;  //  Invalid protocol version.
    const ZCError kZCEAgentQUICNetworkIdleTimeout             = kZCEAgentQuicBase + 25;
    const ZCError kZCEAgentQUICPacketWriteError               = kZCEAgentQuicBase + 27;
    const ZCError kZCEAgentQUICHandshakeFailed                = kZCEAgentQuicBase + 28;
    const ZCError kZCEAgentQUICInvalidStreamData              = kZCEAgentQuicBase + 46;
    const ZCError kZCEAgentQUICUnencryptedStreamData          = kZCEAgentQuicBase + 61;
    const ZCError kZCEAgentQuicHandshakeTimeout                    = kZCEAgentQuicBase+67;  // The connection timed out waiting for the handshake to complete.
    const ZCError kZCEAgentQUICTooManyAvailableStreams        = kZCEAgentQuicBase + 76;
    const ZCError kZCEAgentQUICVerifyCertTimeError                    = kZCEAgentQuicBase+119;
    //自定义错误码从1001 开始
    const ZCError kZCEAgentQUICConnectHBTimeout               = kZCEAgentQuicBase + 1001;
    const ZCError kZCEAgentQUICConnectIdleTimeout             = kZCEAgentQuicBase + 1002;
    const ZCError kZCEAgentQUICNeedSwitchBetterLink              = kZCEAgentQuicBase + 1003;
    const ZCError kZCEAgentQUICCallConnectFail                         = kZCEAgentQuicBase + 1004;
    const ZCError kZCEAgentQUICSend0RTTDataFail                        = kZCEAgentQuicBase + 1005;
    const ZCError kZCEAgentQUICDefaultIPNoResponseFiveSeconds = kZCEAgentQuicBase + 1006;

    const ZCError kZCEAgentTCPConnectTimeout                      = kZCEAgentTcpBase + 1;
    const ZCError kZCEAgentTCPConnectClose                        = kZCEAgentTcpBase + 2;
    const ZCError kZCEAgentTCPConnectHBTimeout                    = kZCEAgentTcpBase + 3;
    const ZCError kZCEAgentTCPConnectIdleTimeout                  = kZCEAgentTcpBase + 4;
    const ZCError kZCEAgentTCPNeedSwitchBetterLink                = kZCEAgentTcpBase + 5;    
    const ZCError kZCEAgentTCPCallConnectFail                     = kZCEAgentTcpBase + 6;

    const ZCError kZCEAgentTCPWSocketConnectFail                    = kZCEAgentTcpBase + 30;
    const ZCError kZCEAgentTCPWSocketClose                              = kZCEAgentTcpBase + 31;
    const ZCError kZCEAgentTCPWSocketSendFail                         = kZCEAgentTcpBase + 32;
    const ZCError kZCEAgentTCPWSocketRecvError                          = kZCEAgentTcpBase + 33;
    const ZCError kZCEAgentTCPWSHandShakeError                         = kZCEAgentTcpBase + 34;
    const ZCError kZCEAgentTCPWSHandShakeTimeout                    = kZCEAgentTcpBase + 35;
    const ZCError kZCEAgentTCPWSCloseFrame                                  = kZCEAgentTcpBase + 36;
    const ZCError kZCEAgentTCPWSFrameError                                   = kZCEAgentTcpBase + 37;
    const ZCError kZCEAgentTCPWSCallConnectApiFail                        = kZCEAgentTcpBase + 38;
    const ZCError kZCEAgentTCPWSocketNoAddressError                    = kZCEAgentTcpBase + 39;

    const ZCError kZCEAgentTCPSSLCreateCTXError                     = kZCEAgentTcpBase + 50;
    const ZCError kZCEAgentTCPSSLCreateSSLError                      = kZCEAgentTcpBase + 51;
    const ZCError kZCEAgentTCPSSLBindSocketError                     = kZCEAgentTcpBase + 52;
    const ZCError kZCEAgentTCPSSLHandShakeError                     = kZCEAgentTcpBase + 53;
    const ZCError kZCEAgentTCPSSLHandShakeTimeout                = kZCEAgentTcpBase + 54;
    const ZCError kZCEAgentTCPSSLReadDataError                        = kZCEAgentTcpBase + 55;
    const ZCError kZCEAgentTCPSSLWriteDataError                       = kZCEAgentTcpBase + 56;

    const ZCError kZCEAgentTCPWSocketNoInstanceID                 = kZCEAgentTcpBase + 600;
    const ZCError kZCEAgentTCPWSocketUrlError                          = kZCEAgentTcpBase + 601;
    const ZCError kZCEAgentTCPWSocketNetTypeChanged           = kZCEAgentTcpBase + 602;
    const ZCError kZCEAgentTCPWSocketProxyChanged                = kZCEAgentTcpBase + 603;

    const ZCError kZCEAgentTCPNetReportNoRequestToken          = kZCEAgentTcpBase + 700;
    const ZCError kZCEAgentTCPNetReportHttpEncodeError           = kZCEAgentTcpBase + 701;
    const ZCError kZCEAgentTCPNetReportWssEncodeError            = kZCEAgentTcpBase + 702;
    const ZCError kZCEAgentTCPNetReportWssDecodeError            = kZCEAgentTcpBase + 703;
    const ZCError kZCEAgentTCPNetReportWssHBTimeout              = kZCEAgentTcpBase + 704;
    const ZCError kZCEAgentTCPNetReportWssIdleTimeout             = kZCEAgentTcpBase + 705;
    const ZCError kZCEAgentTCPNetReportFrameError                    = kZCEAgentTcpBase + 706;
    const ZCError kZCEAgentTCPNetReportUpdateRtc                     = kZCEAgentTcpBase + 707;
    const ZCError kZCEAgentTCPNetReportNotConnectInMaxTimer = kZCEAgentTcpBase + 708;
    const ZCError kZCEAgentTCPNetReportNotReportInMaxTimer    = kZCEAgentTcpBase + 709;
    const ZCError kZCEAgentTCPNetReportHttpInnerError                = kZCEAgentTcpBase + 710;

    const ZCError kZCEAgentTCPWSUnkownError                         = kZCEAgentTcpBase + 999;

    const ZCError kZCEAgentTaskPackDataError                  = kZCEAgentTaskBase + 1;
    const ZCError kZCEAgentTaskSendStreamDataError            = kZCEAgentTaskBase + 3;
    const ZCError kZCEAgentTaskConnectClose                   = kZCEAgentTaskBase + 4;
    const ZCError kZCEAgentTaskUnpackDataError                = kZCEAgentTaskBase + 5;
    const ZCError kZCEAgentTaskAbort                          = kZCEAgentTaskBase + 6;
    const ZCError kZCEAgentTaskProxyConnectTimeout            = kZCEAgentTaskBase + 7;
    const ZCError kZCEAgentTaskProxyConnectClose              = kZCEAgentTaskBase + 8;
    const ZCError kZCEAgentTaskBackendConnectFaild            = kZCEAgentTaskBase + 9;
    const ZCError kZCEAgentTaskBackendConnectClose            = kZCEAgentTaskBase + 10;
    const ZCError kZCEAgentTaskNetworkChanged                 = kZCEAgentTaskBase + 11;
    const ZCError kZCEAgnetTaskConnectDisconnected            = kZCEAgentTaskBase + 12;
    const ZCError kZCEAgentTaskTimeout                        = kZCEAgentTaskBase + 13;
	const ZCError kZCAgentTaskProxyServicesTempBroken = kZCEAgentTaskBase + 14;
    const ZCError kZCEAgnetTaskConnectRediretDisconnected = kZCEAgentTaskBase + 15;  //由于重定向导致的断开链接
    const ZCError kZCEAgnetTaskProxyChanged                   = kZCEAgentTaskBase + 16;//由于代理模式变更导致的断开连接
    const ZCError kZCEAgentTaskIPStackChanged = kZCEAgentTaskBase + 17;
    const ZCError kZCEAgentTaskHaveByPassDispatchNeedClose = kZCEAgentTaskBase + 18;
    
    //ZegoNS errorCode
    const ZCError   kZCENSInnerBase          = 6100000;
    const ZCError   kZCENSRespondBase        = 6200000;

    const ZCError   kZCENSDecodeError       = kZCENSInnerBase + 1;             //解包失败
    const ZCError   kZCENSRequestNotMatch   = kZCENSInnerBase + 2;             //请求ID不符
    const ZCError   kZCENSRedirectIpEmpty   = kZCENSInnerBase + 3;             //Redirect Ip为空
    const ZCError   kZCENSRequestTimeout    = kZCENSInnerBase + 4;             //请求超时
    const ZCError   kZCENSConnectTimeout    = kZCENSInnerBase + 5;             //连接超时
    const ZCError   kZCENSSocketClosed      = kZCENSInnerBase + 6;             //socket closed
    const ZCError   kZCENSSendDataFailed    = kZCENSInnerBase + 7;             //socket send data error
    const ZCError   kZCENSInvalidParams     = kZCENSInnerBase + 8;             //invalid params

    //tcp errorCode
    const ZCError   kZCETCPConnectFailed        = 7000001;    //TCP connect 失败
    const ZCError   kZCETCPDisconnectFailed     = 7000002;    //TCP disconnect 失败
    const ZCError   kZCETCPSendDataFailed       = 7000003;    //TCP send data 失败
    
	//////////////////////////////////////////////////////////////////////////


	enum class ZegoLogLevel
	{
		error = 1,
		warning = 2,
		notice = 3,
		debug = 4
	};

    enum class ZegoConnectionNetType
    {
        ZEGO_NT_NONE = 0,
        ZEGO_NT_LINE = 1,
        ZEGO_NT_WIFI = 2,
        ZEGO_NT_2G = 3,
        ZEGO_NT_3G = 4,
        ZEGO_NT_4G = 5,
        ZEGO_NT_5G = 6,
        ZEGO_NT_UNKNOWN = 32
    };

    enum class ReachableState
    {
        UNKNOWN = -1,
        REACHABLE = 0,
        UNREACHABLE = 1
    };

    //transport layer protocol
    enum class TLProtocolType
    {
        UDP = 0,
        TCP = 1,
    };

    enum class IPStack{
        IPV4 = 0,   
        IPV6,
        IPV64       //nat64
    };

    // IP协议栈模式
    enum class IPStackMode{
        IPV4Only = 0,
        IPV4Prefer,
        IPV6Prefer,
        IPV6Only
    };

    /// \brief geo fencing type
    enum GeoFencingType
    {
        // not use geo fencing
        GeoFencingTypeNone = 0,
        // include
        GeoFencingTypeInclude = 1,
        // exclude
        GeoFencingTypeExclude = 2
    };

    /// \brief geo fencing area code id
    enum GeoFencingArea
    {
        GeoFencingAreaCN = 2, // 中国大陆（不包含港澳台）
        GeoFencingAreaNA = 3, // 北美
        GeoFencingAreaEU = 4, // 欧洲，包括英国
        GeoFencingAreaAS = 5, // 亚洲，不包括中国大陆、印度
        GeoFencingAreaIN = 6, // 印度
        GeoFencingAreaNCN = 7, // 不包含任何中国元素（排除中国大陆、港澳台，以及所有中国供应商）
    };

    //ip地址和ip类型
    using IP2Type = std::pair<std::string, int>;  ///< type: AF_INET/AF_INET6

	//日志内容
	struct ZegoLogContent
	{
		ZegoLogLevel	level = ZegoLogLevel::notice;
		std::string		file_name;
		std::string		module_name;
		std::string		func_name;
		uint32			line = 0;
		std::string		text;
	};

    struct HttpCollectedData;
    struct NetAgentCollectedData;

    struct HttpRequestConfig
    {
        bool useCurlDnsResolve = true; //是否使用curl 内部的dns解析，设置为false 时上层未传递ip，且没有保底ip 请求 会直接失败
		bool lowPriorityRequest = false; //是否是低优先级任务请求
        bool isNeedHeaderInfo = false;      //是否关注HTTP header信息
        uint32  maxTryCount = 6;          //最大重试次数
    };
    
	//http执行结果
	struct HttpContext
	{
		ZCSeq seq = 0;				//执行序列号
		ZCError error = kZCSuccess;	//错误码
		long statusCode = 200;		//状态码
        std::string errorMessage;   //错误信息
        
		std::shared_ptr<std::string> header = nullptr;			//回包头
		std::shared_ptr<std::string> content = nullptr;			//回包主体
        
        uint64 startTime = 0;            //请求开始时间
        uint64 finishTime = 0;          //请求完成时间
        
        std::shared_ptr<HttpCollectedData> httpCollectedData = nullptr;         //收集的http数据
        std::shared_ptr<NetAgentCollectedData> netAgentCollectedData = nullptr; //收集的NetAgent数据
	};

    enum NetAgentNetMode
    {
        PublicCloud = 0,     //0 默认公有云模式
        PrivateOuterNet = 1, //1 私有化外网模式
        PrivateIntranet = 2, //私有化内网模式
    };
    
	struct ZegoAppInfo
    {
        uint32  appID = 0;
        int     bizType = 0;    //live:0, rtv:2
        int     mode = 1;       //test:0, online:1, alpha:2
        std::string userID = "";
        std::string deviceID = "";
        int geo_fencing_type = 0; // GeoFencingType
        std::vector<int> geo_fencing_area_list; // GeoFencingArea
        bool is_special_cusomer_sdk  = false;
        std::string appSign; // appsign 必须传
        NetAgentNetMode na_net_mode = NetAgentNetMode::PublicCloud; 
        uint32 net_agent_dispatch_tag = 0; //个性化统一接入调度使用 默认0    
    };

    enum NetAgentAddressGetType
    {
        Normal = 1,//正常请求
        Refresh = 2,//旁路刷新
    };

    struct IPConfig
    {
        std::string ip;
        uint32 port = 0;
    };

    struct NetAgentAddressGroup
    {
        std::string groupName;
        int weight = 0; //权重值
        uint32 gid = 0; //用于表征节点的优先级，值越大，优先级越高
        std::vector<IPConfig> quicV4Nodes;    //ipv4 nodes
        std::vector<IPConfig> wssV4Nodes;    //ipv4 nodes
        std::vector<IPConfig> quicV6Nodes;    //ipv6 nodes
        std::vector<IPConfig> wssV6Nodes;    //ipv6 nodes
    };

    struct ZegoNetAgentAddressInfo
    {
        uint64 uTxID = 0;
        std::string rand;
        std::string secret;
        std::string httpStick;
        std::string tcpStick;
        std::string locationID;
        std::vector<NetAgentAddressGroup> groups;

         uint32 ttl = 0; //cache ttl 
         uint32 recv_time = 0; //recv address time

         bool HaveV6Address() 
         { 
             for (const auto& group : groups) {
                 if (!group.quicV6Nodes.empty() || !group.wssV6Nodes.empty())
                     return true;
             }

             return false;
         }

         bool HaveV4Address()
         {
             for (const auto &group : groups) {
                 if (!group.quicV4Nodes.empty() || !group.wssV4Nodes.empty())
                     return true;
             }

             return false;
         }
    };

    struct Quic0RttVerifyInfo 
    {
        std::string domain;

        uint64 time_shamp = 0;
        std::string serverConfig;
        std::string chloHash;
        std::string signature;
        std::string sourceAddressToken;
        std::string cert;
    };

    //SOCKS5代理配置
    struct Socks5ProxyConfig
    {
        std::string proxyAddr;			//代理服务地址	
        uint16		proxyPort = 0;		//代理服务端口	
        std::string userName;			//认证用户名	
        std::string passwd;				//认证密码	
        uint32		timeout = 10 * 1000;//超时时间， 默认 10 s
        std::string customMethod ;      //云代理自定义认证方式，默认为0x02
        std::string customData;         //云代理自定义验证数据
        uint16 udpMode = 0;             //udp 转发模式 0：标准socks5， 1：udp bnd.addr 强制使用代理ip

        void Reset() 
        {
             proxyAddr.clear();      //代理服务地址
             proxyPort = 0;       //代理服务端口
             userName.clear();       //认证用户名
             passwd.clear();         //认证密码
             timeout = 10 * 1000; //超时时间， 默认 10 s
             customMethod.clear();   //云代理自定义认证方式，默认为0x02
             customData.clear();     //云代理自定义验证数据
             udpMode = 0; //udp 转发模式 0：标准socks5， 1：udp bnd.addr 强制使用代理ip
        }
    };

    enum ZEGOGlobalProxyType
    {
        zego_global_proxy_none = 0,
        zego_global_proxy_http = 1,
        zego_global_proxy_socks5 = 2
    } ;

    struct Socks5ProxyConfigList
    {
        std::vector<Socks5ProxyConfig> proxyList;
        bool bCloud = false;
        bool bEnable = false;
        ZEGOGlobalProxyType proxyType = zego_global_proxy_none;
    };

    class CZegoCACertData
    {
    public:
        ~CZegoCACertData() {}

    public:
        std::string GetRootCert();
        uint32 GetRootCertSize();
    protected:
        void SetRootCert(const std::string& cert_data);
        void SetRootCert(std::string&& cert_data);
    
    private:
        std::mutex cert_data_mtx_;
        std::string cert_data_;
    };    

}// CONNECTION
}// ZEGO


#endif /* zego_connection_define_hpp */
