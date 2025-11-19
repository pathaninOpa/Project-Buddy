#ifndef zego_connection_agent_define_hpp
#define zego_connection_agent_define_hpp

#include <map>
#include <vector>
#include <string>

#include "zego_connection_define.hpp"
#include "zego_connection_http.hpp"
#include "zego_connection_dns.hpp"

namespace ZEGO
{

namespace CONNECTION
{
    struct ZegoNSDispatchRequestInfo
    {
        ZCError error = kZCSuccess;
    };
    
    struct LocalDNSDispatchRequestInfo
    {
        ZCError error = kZCSuccess;
        std::string domain;  //解析的默认域名
    };

     struct AnyCastDispatchRequestInfo
     {
        ZCError error = kZCSuccess;
        std::string fromip; //解析的默认域名
        uint32 cost = 0; //从开始发送 到成功接收的时间 ms
    };

    struct AutoRefreshDispatchRequestInfo
    {
        ZCError error = kZCSuccess;
    };

    enum class DispatchInfoLaunchReason
    {
        NoDispatchResult = 0,
        NoUsableNodes = 1,
        SwitchProxyMode = 2,
        DispacthResultTTLExpire = 3,
        DispatchAgentNotConnectFinish = 4,
        NoIPv6Address = 5, //一定是旁路调度
    };
    
    enum class NetAgentPortocolStrategy 
    {
        Default = 0,  //内部默认策略
        QuicFirst = 1,  //链接时quic 协议优先，quic异常或者链接慢可能会尝试到wss
        WssFirst = 2, //链接时wss 协议优先，wss 链接慢或者异常时会尝试到quic
        QuicOnly = 3, 
        WssOnly = 4, 
    };

    struct NetAgentDispatchResult
    {
        DispatchInfoLaunchReason reason;
        
        ZCError error = kZCSuccess;
        
        uint64 startTime = 0;
        uint64 finishTime = 0;
        uint64 connectID = 0;  //用于匹配match 当前connect 事件 仅 连上之后的自调度有效 autoRefreshRequestInfo != nullptr

        bool isByPass = false;
        std::string zegons_return_server_sni_host;
        std::shared_ptr<ZegoNetAgentAddressInfo> result = nullptr;
        std::shared_ptr<LocalDNSDispatchRequestInfo> localDnsRequestInfo = nullptr;
        std::shared_ptr<ZegoNSDispatchRequestInfo> zegoNSRequestInfo = nullptr;
        std::shared_ptr<AutoRefreshDispatchRequestInfo> autoRefreshRequestInfo = nullptr;  //链接上统一接入自动刷新调度neagent 地址
        std::shared_ptr<AnyCastDispatchRequestInfo> zegoAnyCastRequestInfo = nullptr;
    };
    
    struct NetAgentWSSConnectInfo 
    {
        uint64 sslHandShakeStartTime = 0;
        uint64 sslHandShakeEndTime   = 0;

        uint64 wsHandShakeStartTime = 0;
        uint64 wsHandShakeEndTime   = 0;
    };

	struct NetAgentConnectDetailData
    {
        uint64 startTime = 0;
        uint64 finishTime = 0;
        uint64 connectID  = 0;  // 当前表示唯一连接的ID
        uint32 connectType;  //1: quic,3:websocket
        uint32 ip_stack;        //0:ipv4, 1:ipv6
        ZCError error = kZCSuccess;
        std::string ip;
        uint32 port;

        std::shared_ptr<NetAgentWSSConnectInfo> wssConnectInfo = nullptr;
    };
    
    struct NetAgentConnectData
    {
        uint64 startTime = 0;
        uint64 finishTime = 0;
 
        ZCError error = kZCSuccess;
        uint64  connectID = 0;
        int success_connect_type = 1; //成功时链接使用的 类型
        //成功时所使用的服务器IP的协议栈类型，0:ipv4, 1:ipv6
        uint32 success_ip_stack = 0;
        int dispatch_address_index = 0; //成功时是调度的第几个ip
        int ip_source = 0;//ip来源，0:default, 1:dispatch
        std::string success_ip; //成功时的ip
        std::vector<std::shared_ptr<NetAgentConnectDetailData>> connectDetailData;
        int connect_reason = 0; 
    };
    
    struct NetAgentDisconnectData
    {
        ZCError error = kZCSuccess;
        uint64 connectSuccessTime = 0;
        uint64 finishTime = 0;
        
        uint64 connectID = 0;
        std::string ip;
        uint32 port;
    };
    
    enum class NetAgentHttpMethod
    {
        Post = 0,
        Get = 1,
        Put = 2,
        Head = 3,
        Delete = 4,
    };

	/*后端服务类型（统一代理层支持的服务）
	"liveroom":	房间服务
	"zpush":		消息推送服务
	"zeus":		老zeus服务
	"mix":		连麦dispatch及其他连麦http接口
	"mediagw":	统一接入服务
	"accesshub":调度服务
	*/

	//http请求内容
    struct NetAgentHttpRequestBase 
    {
        bool is_better_http_request = false;
        std::string userID;                                   // 用户名
        NetAgentHttpMethod method = NetAgentHttpMethod::Post; // 请求方法
        std::map<std::string, std::string> headers;           // http headers
        std::string body;                                     // http body

        bool is_send_after_connect_created = false; //是否在链接创建后就立即发送
    };

    struct NetAgentBetterHttpRequest : public NetAgentHttpRequestBase
    {
        /*信令精简请求 http://service_interface_id/res_id?/param   服务端根据service_id 去映射domain/path
        */
        NetAgentBetterHttpRequest() 
        {
            is_better_http_request = true; 
        }

        uint32 service_interface_id = 0;  
        std::string token;   //当用户有使用Token 时传入
        std::string liteSign; // 用于 当没有appsign 时，传入litesign 发起云控请求。
        std::string res_id;                               //resid 
        std::string url_param;                             //url 后面带的param 参数
    };
    
    struct NetAgentHttpRequest : public NetAgentHttpRequestBase
    {
        //http 请求   http://service/api  
        std::string service;                                  // 后端服务类型
        std::string api;                                      // 请求api
    };

    struct NetAgentDetailData
    {
        uint64 startTime = 0;
        uint64 finishTime = 0;
        
        ZCError error = kZCSuccess;
        
        uint64 dispatchStartTime = 0;
        uint64 dispatchFinishTime = 0;
        
        uint64 connectStartTime = 0;
        uint64 connectFinishTime = 0;
        uint64 connectID = 0;
        std::string connectIP = "";
        uint32 connectPort = 0;
        
        uint64 requestID = 0;
        
        uint64 sendDataTime = 0;
        uint64 recvDataTime = 0;
    };

    struct NetAgentCollectedData
    {
        std::vector<NetAgentDetailData> detailList;
    };

    struct NetAgentNetworkInfo
    {
        ZegoConnectionNetType last_net_type;
        ZegoConnectionNetType current_net_type;
        std::string last_client_ip;
        std::string current_client_ip;
    };
    
    enum class NetAgentBusinessType
    {
        None = 0,
        InitConfig =1,  //柔性配置关键项业务请求
        ByPass = 2,     //柔性配置非关键项业务请求
        RefreshNetAgentAddress = 3,  //quic 刷新统一接入请求
        IMConfig = 4,   //IM请求配置请求
        HttpDNS = 5,    //HTTP dns 请求
        RootCert = 6,   //获取根证书
        ServerAddress = 7,  //获取服务地址
        RootQuicEcc = 8, //获取ECC地址
        CheckClientRegion = 9, //发送客户端区域信息
    };

    struct NetAgentInitConfigResult
    {
        uint32 uCode = 0;
        uint64 serverTime = 0; //服务器的linux时间戳
        NetAgentBusinessType type = NetAgentBusinessType::InitConfig;
        std::string data;
        uint32 version = 0; //版本

        bool isQuic0Rtt = false;  //是否是0RTT获取的
        std::string quic0RttFailReason;  //0RTT 失败时的reason
    };
    //长连接回调类
    class INetAgentProxyCallback
    {
    public:
        virtual ~ INetAgentProxyCallback(){}
		//统一接入链接成功
		virtual void OnRecvNetAgentProxyConnect(ZCSeq proxyID, bool bConnected, const uint32 errorCode) = 0 ;
		//统一接入的ack 回报 uTxID 为SendProxyData是的数据 
		virtual void OnRecvNetAgentProxyAck(ZCSeq proxyID, uint64 uTxID) = 0;
		//收到统一接入下发的数据, 收到数据后，切换到您的业务线程中处理
		virtual void OnRecvNetAgentProxyData(ZCSeq proxyID, const std::string& data) = 0;
		//链接被关闭
		virtual void OnRecvNetAgentProxyClosed(ZCSeq proxyID, const uint32 errorCode) = 0;
    };

    struct NetAgentProxyConnectRequest
    {
        std::string service;
        std::string userID;
        std::string targetIP;
        uint32 targetPort;
        std::weak_ptr<INetAgentProxyCallback> callback;
    };

    using OnNetAgentProxyDataAckDeleagte = std::function<void(const ZCSeq proxyID, uint64 uTxID)>;
    struct NetAgentProxyDataExtra
    {
        std::string service; //后端服务类型 目前只有zpush使用长连接
        bool ack = false;
    };

	enum  ProxyServicesConnectEvent
	{
		PROXY_SERVICES_CONNECTED = 1, //统一接入到后台服务链接成功 
        PROXY_SERVICES_TEMP_BROKEN = 2,  //统一接入到后台服务链接临时中断
		PROXY_SERVICES_DISCONNECTED = 3,  //统一接入到后台服务链接中断
	};

    enum NetAgentConnectEvent 
    {
        NetAgentConnecting = 1, 
        NetAgentConnected = 2,
        NetAgentDisconnected = 3
    };

    enum BetterLinkReason 
    {
        QuicNotFinish = 0,
        ProbHaveBetterLink = 1
    };

    struct ProxyServicesConnectInfo
    {
        uint32 uCode = 0;                    //错误码
        int tempBrokenTimeOut = 0;  //PROXY_SERVICES_TEMP_BROKEN 时有效 收到broken后 可等待的时间
        uint32 proxyConnetTime = 0; //event 为PROXY_SERVICES_CONNECTED 有效 统一接入 与后端链接消耗时间
        uint32 proxyLinkSources = 0;  //event 为PROXY_SERVICES_CONNECTED 有效 统一接入 与后端链接来源 0: 从连接池获取；1: 从连接池获取，但需要等待；2: 新建连接
        uint64 uClientRecvTime = 0; //sdk 触发该事件的起始时间点
        uint64 connectID    = 0;  //用于匹配连接的ID
    };

    //-------------------ZIM专用-------------------------
    enum NetAgentConfigRequestType
    {
        IM = 0
    };

    struct NetAgentConfigRequest
    {
        NetAgentConfigRequestType type = NetAgentConfigRequestType::IM;
        uint32 timeout = 10*1000;  //请求超时时间
        std::string litesign;  //鉴权litesign   
        std::string token;   //鉴权token    
    };
     //-------------------ZIM-------------------------

    struct NetAgentRequestBase 
    {
        NetAgentBusinessType type = NetAgentBusinessType::None;
        std::string token; //token 
        std::string liteSign; //litesign
    };

    struct NetAgentInitConfigRequestInfo : NetAgentRequestBase
    {
        NetAgentInitConfigRequestInfo() 
        {
            type = NetAgentBusinessType::InitConfig;
        }

        int version = 0;  //统一接入版本
        int timeout = 10*1000;
    };

    struct NetAgentConfigResponse
    {
        ZCSeq seq = 0;
        uint32 uCode = 0;
        uint64 serverTime = 0;
        std::string data;
    };

    struct NetAgentHttpDNSRequestInfo : NetAgentRequestBase
    {
        NetAgentHttpDNSRequestInfo() 
        {
            type = NetAgentBusinessType::HttpDNS;
        }
        
        std::string cdnVendor; //cdn 运营商
        std::string domain; //请求的域名
    };

    struct NetAgentHttpDNSResponse 
    {
        uint32 code = 0;
        bool fromCache = false;
        std::vector<std::string> ipV4List;
        std::vector<std::string> ipV6List;
    };

    struct NetAgentHttpDNSReportData 
    {
        uint32 code = 0;
        std::string domain;
        std::string cdnVendor;
        int ttl = 0;
        bool preReslove = false;
        std::vector<std::string> ipV4List;
        std::vector<std::string> ipV6List;
        uint64 startTime = 0;
        uint64 endTime = 0;
    };

    struct NetAgentConnectProbeConfig
    {
        int badMinRtt = 0;  //触发探测的最小 rtt 值，单位毫秒。默认为 0，表示关闭探测。如需探测，推荐设置为 1500
        int goodMinRtt = 1000; //探测结果切换链路的阈值，当探测的结果<= 1000ms 时，会触发链路切换  
        int closedAfterConnectedFailCount = 3;  //当一个ip 链接成功后，链接又被断开。 上述出现次数大于closedAfterConnectedFailCount 时，会切换下一个ip = 0 则关闭。断掉之后重试当前ip
    };

    struct NetAgentRegionInfo
    {
        std::string language;
        std::string timezone;
        std::string carrier;
        std::string accesscube_domain;
        IP2Type accesscube_ip;
    };

    enum NetAgentWeightSelectStrategy
    {
        Mode0 = 0,
        Mode1 = 1,
        Mode2 = 2,
    };

    struct NetAgentConfig 
    {
        bool enbale0Rtt = false;  //是否开启0RTT功能
        NetAgentConnectProbeConfig connectProbe;
        NetAgentPortocolStrategy agentProtocolStrategy = NetAgentPortocolStrategy::Default; //统一计入链接时使用得策略
        bool enbaleAgentHttpDNS = false;  //是否开启agent httpdns 功能支持。  
        bool enableQuicTransferDataEncrypt = false;  //quic 传输是否会加密, 只有在数据加密情况下enableClientVerifyServerCert 开关才生效
        bool enableClientVerifyServerCert = false; //是否开启 客户端校验服务端证书，如开启 必须调用SetNetAgentRootCertificate 设置证书 否则将会链接异常
        int quic_client_hello_min_padding = 0;
        int quic_client_hello_max_padding = 0;
        int quic_server_hello_min_padding = 0;
        int quic_server_hello_max_padding = 0;
        
        int max_link_count = 2; //最大并发链接数
        int quic_weight = 2;
        int wss_weight = 1;
        int bypass_disaptch_time_ms = 5000; 
        int link_interval = 500; //链接并发的时间间隔 单位ms
        int bypass_disaptch_wait_zegons_time = 10000; //旁路dispatch 时等待zegons 的时间

        bool use_wss = true; //tcp 链接时默认使用wss  为false 时使用ws 链接
        bool enable_anycast_dispatch = true; //调度时使用使用anycast
        bool enable_zegons_dispatch = true; //调度时使用使用zegons

        NetAgentWeightSelectStrategy address_weight_select_strategy = NetAgentWeightSelectStrategy::Mode0; //权重选择策略
        NetAgentWeightSelectStrategy protocol_weight_select_strategy = NetAgentWeightSelectStrategy::Mode0; //协议权重选择策
    };
    
    struct NetAgentNode 
    {
        std::string ip;
        int quic_port = 8000;
        int wss_port = 443;
    };

    struct GeoFencingServerAddress
    {
        uint32 area_code_id; // GeoFencingArea
        uint32 server_address_version;
        
        std::vector<std::string> anycast_ipv4s;
        std::vector<std::string> anycast_ipv6s;
        
        std::string zegons_domain;
        std::vector<std::string> zegons_ipv4s;
        std::vector<std::string> zegons_ipv6s;
        
        std::string netagent_domain;
        std::vector<std::string> netagent_ipv4s;
        std::vector<std::string> netagent_ipv6s;
        
        std::string log_report_domain;
        std::vector<std::string> log_report_ipv4s;
        std::vector<std::string> log_report_ipv6s;

        std::string wss_log_report_domain;
        std::vector<std::string> wss_log_report_ipv4s;
        std::vector<std::string> wss_log_report_ipv6s;
        
        std::string log_detail_domain;
        std::vector<std::string> log_detail_ipv4s;
        std::vector<std::string> log_detail_ipv6s;
        
        std::string stun_server_domain;
        std::vector<std::string> stun_server_ipv4s;
        std::vector<std::string> stun_server_ipv6s;
        
        //the detect server only have domain, have no backup ips
        std::string detect_domain;
        
        //用来做线路解析的域名，SDK需要对该域名做A和AAAA记录的localdns解析，解析结果需要上报
        std::string accesscube_domain;
        
        std::string zim_file_domain;
        std::vector<std::string> zim_file_ipv4s;
        std::vector<std::string> zim_file_ipv6s;

        std::string agent_sni_host; //指定sni 域名
    };

    struct NetAgentGetRootCertRequestInfo
    {
        NetAgentBusinessType type = NetAgentBusinessType::RootCert;
        int timeout = 10*1000;
        
    };

    struct NetAgentGetServerAddressRequestInfo
    {
        NetAgentBusinessType type = NetAgentBusinessType::ServerAddress;
        int timeout = 10*1000;
    };

	/**
	http响应

	@param context http请求执行结果
	*/
	using OnNetAgentHttpResponseDelegate = std::function<void(std::shared_ptr<HttpContext> context)>;

    /**
     统一接入dispatch结果更新
     
     @param info 当前app环境
     @param result dispatch结果
     */
    using OnNetAgentDispatchResultUpdateDelegate = std::function<void(const ZegoAppInfo& info, const std::shared_ptr<ZegoNetAgentAddressInfo>& result)>;
    
    /**
     统一接入 dispatch 的调度请求事件
     
     @param result 调度请求详细信息
     */
    using OnNetAgentDispatchEventDelegate = std::function<void(const std::shared_ptr<NetAgentDispatchResult>& result)>;
    
    /**
     统一接入的连接事件上报
     
     @param data 连接详情
     */
    using OnNetAgentConnectEventDelegate = std::function<void(const std::shared_ptr<NetAgentConnectData>& data)>;
    
    /**
     统一接入的断开连接事件上报
     
     @param data 断开连接详情
     */
    using OnNetAgentDisconnectEventDelegate = std::function<void(const std::shared_ptr<NetAgentDisconnectData>& data)>;
    
    /**
     统一接入的 proxy 模式的事件上报
     
     @param data 断开连接详情
     */
    using OnNetAgentProxyEventDelegate = std::function<void(const std::shared_ptr<NetAgentCollectedData>& data)>;

    /**
     统一接入的网络信息
     
     @param data 网络信息
     */
    using OnNetAgentNetworkInfoChangeDelegate = std::function<void(const std::shared_ptr<NetAgentNetworkInfo>& data)>;

    /**
    统一接入获取柔性配置接口

    @param data 连接详情
    */
    using OnGetInitConfigByAgentDelegate = std::function<void(const std::shared_ptr<NetAgentInitConfigResult>& data)>;

    /**
    统一接入与 后台代理服务链接事件

    @param event 事件详情
	@param uConnectID 链接ID  
    @param ProxyServicesConnectInfo   统一接入 收到
    */
    using OnProxyServicesConnectEventDelegate = std::function<void(const ProxyServicesConnectEvent& event, uint32 linkStreamID, int tempBrokenTimeOut)>;

     /**
    统一接入链接事件

    @param event 事件详情
	@param error_code 错误码

    @attention 注意 只有event = NetAgentConnected 时 protocol 才有意义
    */
    using OnNetAgentConnectStateEventDelegate = std::function<void(NetAgentConnectEvent event, uint32 error_code, TLProtocolType protocol)>;

    /**
    本地网络变化通知代理事件

    @param type 通知的网络类型
    */
    using OnLocalNetChangeEventDelegate = std::function<void(ZegoConnectionNetType type)>;


      /**
      统一接入与 后台代理服务链接事件上报回调

    @param event 事件详情
	@param uConnectID 链接ID  
    @param ProxyServicesConnectInfo   统一接入 收到
    */
    using OnReportProxyServicesConnectEventDelegate = std::function<void(const ProxyServicesConnectEvent& event, uint32 linkStreamID, const ProxyServicesConnectInfo& info)>;

    /**
    IM 获取配置相关接口

    @param data 连接详情
    */
    using OnGetConfigByAgentDelegate = std::function<void(const std::shared_ptr<NetAgentConfigResponse>& data)>;

     /**
    统一接入开启0RTT功能时，回调的用于0RTT验证的相关数据

    @param ZegoNetAgent0RttVerifyInfo  0RTT 服务验证数据
    @attention data 为nullptr 时需要删除domain 对应的缓存的cache 数据
    */
    using OnNetAgent0RttVerifyInfoDelegate = std::function<void(const std::shared_ptr<Quic0RttVerifyInfo>& data)>;

      /**
     统一接入访问HttpDNS得上报信息
     
     @param data HttpDNS相关信息
     */
    using OnNetAgentHttpDNSDelegate = std::function<void(const std::shared_ptr<NetAgentHttpDNSReportData>& data)>;

     /**
     HttpDNS 过期回调通知
     
     @param 过期的域名列表
     */
    using OnNetAgentHttpDNSCacheTimeoutDelegate = std::function<void(const std::vector<std::string>& domainList)>;
    
    /**
     网络时间对齐回调通知
     
     @param network_time_offset 网络时间和本地时间的偏差值，将本地时间加上偏差值就是网络时间，单位毫秒
     @param server_time 统一接入服务器时间 单位ms
     */
    using OnNetAgentNetworkTimeSyncedDelegate = std::function<void(int64 network_time_offset, uint64 server_time)>;

    /**
     收到根证书 和ECC证书版本号的回调通知
     
     @param root_cert_version 根证书版本号
     */
    using OnRecvRootCertVersionDelegate = std::function<void(uint32 cert_version, bool is_wss)>;

    using OnGetRootCertDelegate = std::function<void(uint32 seq, uint32 error, uint32 cert_version, const std::string& cert_data)>;

    /**
     收到服务地址版本号的回调通知
     
     @param geofencing_id 地理范围 ID
     @param server_address_version 服务地址版本号
     */
    using OnRecvServerAddressVersionDelegate = std::function<void(uint32 geofencing_id, uint32 server_address_version)>;

    using OnGetServerAddressDelegate = std::function<void(uint32 seq, uint32 error, const GeoFencingServerAddress& server_address)>;

    /**
     收到访问被拒绝的回调通知
     
     @param error 错误码
     */
    using OnAccessDeniedDelegate = std::function<void(uint32 error)>;

    /*
    * 内部链接时使用协议栈多次失败时 的回调
    */
    using OnIPStackFailedTooMuchDelegate = std::function<void(int count, IPStack)>;

    enum ProxyRetryState 
    {
        RetryNow = 0,
        RetryDelay = 1,
        RetryForbid = 2
    };

    namespace NetAgentError 
    {
        ProxyRetryState IsProxyConnectRetryNow(uint32 error);
        ProxyRetryState IsProxyCloseRetryNow(uint32 error);

        //统一接入业务请求，是否可以立即重试
        bool IsAgentBussinessRetryNow(uint32 error);
    }
    }// CONNECTION
}// ZEGO


#endif /* zego_connection_agent_define_hpp */
