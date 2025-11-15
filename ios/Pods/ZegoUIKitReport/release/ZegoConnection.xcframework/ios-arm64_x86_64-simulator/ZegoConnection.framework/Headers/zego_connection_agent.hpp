//
//  代理层统一接口
//
//  zego_connection_agent.h
//  
//
//  Created by arvin on 2019/07/18.
//

#ifndef zego_connection_agent_hpp
#define zego_connection_agent_hpp

#include <memory>
#include <zegotask.h>
#include "zego_connection_agent_define.hpp"
#include "zego_connection_define.hpp"

namespace ZEGO
{
namespace CONNECTION
{    
	class NetAgentImpl;

    class ZEGOCONNECTION_API ZegoConnectionAgent
    {
    public:
        static std::shared_ptr<ZegoConnectionAgent> GetInstance();
        virtual ~ZegoConnectionAgent();
        
    private:
        ZegoConnectionAgent();

    public:
        /**
        初始化模块
        
        @param
        */
        ZCError Init();

        /**
        反初始化模块
        
        @param
        */
        void UnInit();
  
        /**
         设置 app 信息
         */
        void SetAppInfo(const ZegoAppInfo& info);

        /**
        设置 user 信息

        @attention 由于userid 是可以，每次登录都发生变更的 不应该放在appinfo中
        */
        void SetUserInfo(const std::string& userid);
        
        /**
         设置IP协议栈模式
         
         @param mode 协议栈模式
         */
        void SetIPStackMode(IPStackMode mode);

        /**
        设置统一接入默认域名

        @param domain 默认统一接入调度域名
        @param isolation 是否是隔离域名，如果是 该字段为true
        @attention 若是domain 为zego.im 的后缀，为正常域名，若是不是则 isolation 为true 隔离域名。
        @attention isolation 为true 时 WSS访问时，指定的SNI 中将不会暴漏zego.im 域名
        */
        void SetNetAgentDefaultDomain(const std::string& domain, bool isolation = false);
        
        /**
         设置统一接入默认域名列表
         domain -> agent_sni_host
         指定 wss 时位sni host
         quic_domain_port 域名解析时使用端口号
         quic_ws_port 域名解析使用wss 端口号
         */
        void SetNetAgentDefaultDomain(const std::vector<std::pair<std::string, std::string>>& domain_sni_list, int quic_domain_port = 8000, int ws_domain_port = 443);

        /**
        设置统一备用ip

        @param ipv4s ipv4协议地址列表
        @param ipv6s ipv6协议地址列表
        @attention SetNetAgentBackupInfo 接口废弃，使用SetNetAgentDefaultIps 传递
        */
        void SetNetAgentDefaultIps(const std::vector<NetAgentNode>& ipv4s, const std::vector<NetAgentNode>& ipv6s);

        /**
        设置统一接入调度的anycastip

         @param ipv4s v4 ip 列表
         @param ipv6s v6 ip 列表
         @attention 当本地域名解析超时时，会通过此方式来进行调度 获取统一接入链接列表
        */
        void SetNetAgentAnyCastIpInfo(const std::vector<std::string>& ipv4s, const std::vector<std::string>& ipv6s);

        /**
        设置统一接入配置

        @param config 配置详情参见 NetAgentConfig
        @attention 如需设置配置 必须在SetDispatchResult 之前调用该接口，设置配置只需要变更自己关注项即可
        */
        void SetNetAgentConfig(const NetAgentConfig& config);

        /**
         设置用于统一接入 用于校验服务端的根证书 WSS 使用
    
         @param cert_data 根证书内容
         @attention  SetNetAgentConfig 中 开启 enableClientVerifyServerCert = true  则必须设置次证书
       */
        void SetNetAgentRootCertificate(std::shared_ptr<CZegoCACertData> cert_data);
        
        /**
         设置0 rtt 相关数据 
        @param verifyInfo 缓存的验证数据
        @discussion 如果SetNetAgentConfig 配置有开启0rtt 功能。需要保存SetNetAgent0RttVerifyInfoEventDelegate 中回调的数据，通过此接口塞回SDK。必须在SetDispatchResult 接口前调用
        */
        void SetNetAgent0RttVerifyInfo(Quic0RttVerifyInfo *verifyInfo);

        /**
        设置调度配置相关信息
        
        @param info 调度配置相关信息
        @param connect_right_now 当有设置地址时，是否立即链接
        @discussion 必须调用此API来触发拉取或更新Dispatch，如有启用0RTT 功能。则必须在调用 接口后，才能调用SetDispatchResult接口
        */
        void SetDispatchResult(ZegoNetAgentAddressInfo* pResult, bool connect_right_now = true);

        /**
        设置区域信息
         
        @param region_info 区域信息
        */
        void SetRegionInfo(const NetAgentRegionInfo& region_info);
        
		/**
		发起http请求，短连接代理

		@param req http请求内容
		@param rsp http返回函数对象
		@return 失败，返回0，否则，返回请求序列ID
		*/
		ZCSeq StartHttpRequest(const NetAgentHttpRequest& req, const OnNetAgentHttpResponseDelegate& rsp, HttpRequestConfig config = HttpRequestConfig());

        /**
		发起http请求，短连接代理 信令精简版本

		@param req http请求内容
		@param rsp http返回函数对象
		@return 失败，返回0，否则，返回请求序列ID
		*/
		ZCSeq StartBetterHttpRequest(const NetAgentBetterHttpRequest& req, const OnNetAgentHttpResponseDelegate& rsp, HttpRequestConfig config = HttpRequestConfig());


        /**
        通过统一接入获取柔性配置请求

        @param type 关键项配置，还是旁路性配置
        @param timeout 超时时间
        @param sign=MD5(appSign+appID+uSignTime)
        @param uSignTime  签名时的时间戳
        @param delegate回调
        */
        ZCSeq GetInitConfigByAgent(const NetAgentInitConfigRequestInfo& requestInfo, const OnGetInitConfigByAgentDelegate& delegate);

        /**
        通过统一接入获取柔性配置请求，目前该请求只有zim使用

        @param request  参见NetAgentConfigRequest
        @param delegate 回调
        @return seq 可在回调结果中匹配
        */
        ZCSeq GetConfigByAgent(const NetAgentConfigRequest& request, const OnGetConfigByAgentDelegate& delegate);
        
        /**
         通过统一接入获取根证书
         */
        ZCSeq GetRootCert(const NetAgentGetRootCertRequestInfo& request, const OnGetRootCertDelegate& delegate);
        
        /**
         通过统一接入获取 Server Address 信息
         */
        ZCSeq GetServerAddress(const NetAgentGetServerAddressRequestInfo& request, const OnGetServerAddressDelegate& delegate);

         /**
        同步通过统一接入获取http DNS。 目前适用于CDN拉流时根据对应厂商解析对应的dns. 

        @param requestInfo 参见NetAgentHttpDNSRequestInfo
        @param sync 是否同步等待结果，false 只是预解析。NetAgentHttpDNSResponse 将不会返回结果
        @param timeout 单位ms 默认等待300ms 300ms未返回结果则外部需要调用LocalDNS去获取解析结果
        @attention 接口 会先从内部缓存中获取，缓存中没有时会同步请求网络。 缓存超时时间由服务端返回。 
        */
        NetAgentHttpDNSResponse RequestHttpDNS(const NetAgentHttpDNSRequestInfo& requestInfo, bool sync = true, int timeout = 300);

         /**
         设置httpdns 缓存过期的回调通知，外部可调用RequestHttpDNS 重新刷新内部请求。注意这可能会极大增大我们成本！！！！！

        @param delegate
        */
        void SetNetAgentHttpDNSCacheTimeoutDelegate(const OnNetAgentHttpDNSCacheTimeoutDelegate& delegate);

        /**
        长连接代理开始连接
        
        @param service 服务类型，参见后端服务类型说明
		@param id_name 用户名
		@param ip  目标ip地址
        @param port 目标port
        @param callback 回调类
        @return 代理ID
        */
		ZCSeq ConnectProxy(const NetAgentProxyConnectRequest& req, uint32 timeout = 5000);
        
        /**
        长连接代理断开连接
        
        @param proxyID 代理ID
        @param disable_reuse_quic_stream proxy 长链接 
        */
        void DisconnectProxy(const ZCSeq proxyID, bool disable_reuse_quic_stream = false);

        /**
        发送长连接数据

        @param proxyID 代理ID
        @param data 发送数据
        @param ack   服务发送该包ack的回调
        @return 返回发送包的txid 如果设置了ack ack中回调该txid 和proxyID 返回0 发送失败
        */
        uint64 SendProxyData(const ZCSeq proxyID, const std::string& data, const NetAgentProxyDataExtra& extra);

         /**
        发送长连接数据

        @param proxyID 代理ID
        @param data 发送数据
        @param ack   服务发送该包ack的回调
        @return 返回发送包的txid 如果设置了ack ack中回调该txid 和proxyID 返回0 发送失败
        */
        uint64 SendProxy(const ZCSeq proxyID, const std::string& data, const NetAgentProxyDataExtra& extra);

        /**
		设置用于缓存服务0rtt 验证数据的回调

		@param delegate
		@attention 只有在SetNetAgentConfig 接口中开启0rtt 功能才有此回调
		*/
        void SetNetAgent0RttVerifyInfoEventDelegate(const OnNetAgent0RttVerifyInfoDelegate& delegate);

		/**
		设置Proxy连接事件的回调(通过ConnectProxy 发起连接后，统一接入服务和后台业务服务之间的事件回调)。

		@param delegate
		@attention 该事件只有在统一接入连接正常时才会回调
		*/
        void SetNetAgentProxyServicesConnectEventDelegate(const OnProxyServicesConnectEventDelegate& delegate);

        /**
		设置统一接入链接状态的回调。该事件表示链接统一接入服务的状态。

		@param delegate
		@attention 该事件只有在统一接入连接正常时才会回调
		*/
        void SetNetAgentConnectStateEventDelegate(const OnNetAgentConnectStateEventDelegate &delegate);
        
        /**
		本地网络发生变更时回调此值

		@param delegate
		@attention 该事件只有在统一接入连接正常时才会回调
		*/
        void SetNetAgentLocalNetChangeEventDelegate(const OnLocalNetChangeEventDelegate& delegate);

        /**
         设置调度结果更新的回调
         
         @param delegate 回调处理函数
         */
        void SetDispatchResultUpdateDelegate(const OnNetAgentDispatchResultUpdateDelegate& delegate);
        
        /**
         设置调度请求事件的回调
         
         @param delegate 回调处理函数
         */
        void SetReportDispatchEventDelegate(const OnNetAgentDispatchEventDelegate& delegate);
        
        /**
         设置统一接入的连接事件的回调
         
         @param delegate 回调处理函数
         */
        void SetReportNetAgentConnectEventDelegate(const OnNetAgentConnectEventDelegate& delegate);
        
        /**
         设置统一接入的断开连接事件的上报回调
         
         @param delegate 回调处理函数
         */
        void SetReportNetAgentDisconnectEventDelegate(const OnNetAgentDisconnectEventDelegate& delegate);
        
         /**
         Uninit 之前 同步获取需要上报的数据, 目前就RTC 使用

         */
        void GetSyncConnectReportDataBeforeUninit(std::shared_ptr<NetAgentConnectData> & connect_data, std::shared_ptr<NetAgentDisconnectData>& disconnect_data);

        /**
         设置统一接入的网络信息变化的上报回调
         
         @param delegate 
         */
        void SetReportNetAgentNetworkInfoChangeDelegate(const OnNetAgentNetworkInfoChangeDelegate& delegate);

        /**
         设置 agent httpdns  数据上报的回调
         
         @param delegate 
         */
        void SetReportNetAgentHttpDNSDelegate(const OnNetAgentHttpDNSDelegate& delegate);
        
        /**
         设置网络时间对齐回调
         
         @param delegate 
         */
        void SetNetworkTimeSyncedDelegate(const OnNetAgentNetworkTimeSyncedDelegate& delegate);
        
        /**
         设置收到 CACert 根证书版本号回调，用于判断本地缓存的根证书是否需要更新
         
         @param delegate
         */
        void SetRecvRootCertVersionDelegate(const OnRecvRootCertVersionDelegate& delegate);
        
        /**
         设置收到 Server Address 版本号回调，用于判断本地缓存的 Server Address 是否需要更新
         
         @param delegate
         */
        void SetRecvServerAddressVersionDelegate(const OnRecvServerAddressVersionDelegate& delegate);
        
        /**
         设置访问被拒绝回调
         
         @param delegate
         */
        void SetAccessDeniedDelegate(const OnAccessDeniedDelegate& delegate);


          /**
         某一个协议栈 多是使用失败时的通知回调
         
         @param delegate
         */
        void SetIPStackFailedTooMuchDelegate(const OnIPStackFailedTooMuchDelegate &delegate);

   
    private:		
		std::shared_ptr<NetAgentImpl> net_agent_impl_;
    };
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_agent_hpp */
