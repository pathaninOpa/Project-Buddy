//
//  zego_connection_dns.h
//  ZegoConnection
//
//  Created by Strong on 2019/10/10.
//

#ifndef zego_connection_dns_hpp
#define zego_connection_dns_hpp

#include "zego_connection_dns_define.hpp"
#include "zego_connection_define.hpp"
#include <memory>
#include <zegotask.h>

namespace ZEGO
{

namespace CONNECTION
{
    class DNSImpl;
    
    class ZEGOCONNECTION_API ZegoConnectionDNS
    {
    public:
        static std::shared_ptr<ZegoConnectionDNS> GetInstance();
        virtual ~ZegoConnectionDNS();
        
    private:
        ZegoConnectionDNS();
        
    public:
        /**
         初始化
         
         */
        ZCError Init();
        
        /**
         反初始化
         
         */
        void UnInit();
        
        /**
         设置 app 信息
         */
        void SetAppInfo(const ZegoAppInfo& info);
        
        /**
        是否启用dns 网络请求

        @param enable 是否启用
        @discussion 非线程安全，只在Init 和UnInit时调用。关闭时会立即终止正在解析的dns。建议外部在Unit时调用，避免因为dns导致退出卡顿
        */
        void EnableDNSRequest(bool bEnable);

          /**
        是否启用了dns 网络请求

        @return 是否启用
        @discussion 非线程安全，只在Init 和UnInit时调用。关闭时会立即终止正在解析的dns。建议外部在Unit时调用，避免因为dns导致退出卡顿
        */
        bool IsEnableDNSRequest();

        /**
         是否启用ZegoNS
         
         @param enable 是否启用
         @discussion 非线程安全，在Init后调用一次后不再调用
         */
        void EnableZegoNS(bool enable);
        
        /**
         设置DNS优先级
         
         @param strategy 优先级
         @discussion 非线程安全，在Init后调用一次后不再调用
         */
        void SetDNSStrategy(DNSStrategy strategy);
        
        /**
         设置IP协议栈模式
         
         @param strategy 优先级
         @discussion 非线程安全，在Init后调用一次后不再调用
         */
        void SetIPStackMode(IPStackMode mode);

        /**
         设置ZegoNS 地址
         
         @param addresses 服务器地址
         @param source 服务器地址来源
         */
        void SetZegoNSServerAddress(const std::vector<NameServerAddressInfo>& addresses, NameServerSource source);
        
        /**
         添加 ZegoNS 地址
         
         @param addresses 服务器地址
         @param source 服务器地址来源
         */
        void AppendZegoNSServerAddress(const std::vector<NameServerAddressInfo>& addresses, NameServerSource source);
        
        /**
         DNS 解析
         
         @param domain 解析的域名
         @param timeout 解析超时时间
         @param no_use_cache 不使用内部缓存，重新解析
         @return DNS解析结果
         
         @discussion isHttps参数仅对柔性配置、统一调度域名有效
         @discussion SetReportLocalDNSResolveEventDelegate  和 SetReportZegoNSResolveEventDelegate已删除，如需要上报，则直接取DNSData 中 localEvent zegoNSEvent 和中数据上报即可
         */
        DNSData DNSResolve(const std::string& domain, int32 timeout = 2000, bool no_use_cache = false);

        /*
         是否允许域名内部自动解析更新，默认允许(内部默认5min 更新一次)
        * 
        * @param domain 域名
        * @allow_auto_update true 允许，false 不允许
        */

        void SetDomainAutoUpdate(const std::string& domain, bool allow_auto_update);
        
        /**
         Local DNS解析
         
         @param domain 解析的域名
         @param timeout 解析超时时间
         @return DNS解析结果
         */
        DNSData LocalDNSResolve(const std::string& domain, int32 timeout = 2000);
        
        /**
         预解析DNS
         
         @param domain 解析的域名
         @param type 请求的记录类型
         */
        void LocalDNSPreResolve(const std::string& domain);

        /**
        Local DNS 异步解析接口

        @param domain 解析的域名
        @param timeout 解析超时时间
        @param no_use_cache 是否使用缓存
        @param delegate  解析回调
        @return 调用函数成功，true，否则调用失败
        */
        bool LocalDNSAsyncResolve(const std::string &domain, bool no_use_cache, int timeout, const OnLocalDNSAsyncResolveResultDelegate &delegate);
        
        /**
         设置备用信息
         
         @param domain 解析的域名
         @param backupDomain 备用域名
         @param backupIp     备用IPv4
         */
        void SetBackupInfo(const std::string& domain, const std::string& backupDomain, const std::string& backupIp);
        
        /**
         设置备用信息
         
         @param domain 解析的域名
         @param backupDomain 备用域名
         @param backupIpv4     备用IPv4列表
         @param backupIpv6     备用IPv6列表
         */
        void SetBackupInfo(const std::string& domain, const std::string& backupDomain, const std::vector<std::string>& backupIpv4, const std::vector<std::string>& backupIpv6);
        
        /**
         使用DNS请求的结果
         
         @param domain 域名
         @param ip 对应的IP
         @param isHttps 请求的协议类型
         @param type 更新的IP结果类型
         */
        void DNSFeedback(const std::string& domain, const std::string& ip, bool isHttps, DNSUpdateType type);
        
        /**
         设置ZegoNS服务入口结果回调
         
         @param delegate 结果回调
         */
        void SetZegoNSAddressUpdateDelegate(const OnZegoNSAddressUpdateDelegate& delegate);
        
        /*
        通过zegons 获取网络探测协议下载配置的url

        @param timeout 超时时间 单位ms
        @param delegate 结果回调
        @return 成功true fasle 失败
        */
        bool GetNetworkTraceConfig(int timeout,const OnZegoNSGetDectecConfigDelegate& delegate);

        /*
        通过zegons 获取NTP服务器的地址

        @param timeout 超时时间 单位ms
        @param delegate 结果回调
        @return 成功true fasle 失败
        */
        bool GetNtpServerConfig(int timeout, const OnZegoNSGetNtpServerDelegate& delegate);

        /*
        获取链接统一接入的ip地址.通过zegons

        @param timeout 设置的超时时间
        @param delegate 结果回调
        @return 成功true fasle 失败
        */
        bool GetNetAgentAddressInfo(int timeout,  const OnZegoNSGetNetAgentAddressInfoDelegate& delegate);

         /*
        通过tcp 获取证书

        @param timeout 设置的超时时间
        @return 成功true fasle 失败
        */
        bool GetRootCertByZegoNS(int timeout, const OnZegoNSGetRootCertDelegate &delegate);
        
        /*
        获取客户端出口IP.通过zegons

        @param timeout 设置的超时时间, 单位ms
        @param ip_stack 对应IP协议栈
        @param delegate 结果回调
        @return 成功true fasle 失败
        */
        bool GetOuterIP(uint32 timeout, IPStack ip_stack, const OnZegoNSGetOuterIPDelegate& delegate);
        
        /*
        转换IPv6 , 输入一个IPV4 地址，若是当前事Nat64 网络，则会返回对应得IPV6地址 
        SyntheSizeIPv4 中会使用 线程池去转换

        @param ipv4 输入的ip 地址
        @param ipv6 输出的ip 地址
        @timeout 超时时间 1000
        @from_desc 内部打印日志使用透传串
        */

        void SyntheSizeIPv4(const std::string &ipv4, std::string &ipv6, int timeout = 1000, std::string from_desc = "");

        /*
        设置 Local DNS 事件上报回调

        @param delegate Local DNS 事件上报回调
        */
        void SetReportLocalDNSResolveEventDelegate(const OnLocalDNSResolveEventDelegate &delegate);
        
        /*
        设置 Zego NS 事件上报回调

        @param delegate Zego NS 事件上报回调
        */
        void SetReportZegoNSResolveEventDelegate(const OnZegoNSResolveEventDelegate &delegate);

    private:
        std::shared_ptr<DNSImpl> dns_impl_ = nullptr;
    };
}

}
#endif /* zego_connection_dns_hpp */
