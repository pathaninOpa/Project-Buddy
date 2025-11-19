#ifndef zego_connection_rtc_private_report_hpp
#define zego_connection_rtc_private_report_hpp

#include <memory>
#include "zego_connection_rtc_private_net_report_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{
    class CRtcNetReportImpl;
    class ZEGOCONNECTION_API ZegoRtcPrivateNetReport 
    {
      public:
        static std::shared_ptr<ZegoRtcPrivateNetReport> GetInstance();
        virtual ~ZegoRtcPrivateNetReport();

      private:
        ZegoRtcPrivateNetReport();

      public:
        /**
		初始化 rtc 上报模块
        @param appid appid
        @param appsign_hex_string hex 编码的字符串
        
        */
        ZCError Init(uint32 appid, const std::string &appsign_hex_string, ReportEnumOsType os_type);
        
        /**
		反始化 rtc 上报模块
        
        */
        void UnInit();
        
        /**
        * 设置配置上报
        @param config 参数

        */
        void SetReportConfig(const ReportConfig& config);

        /**
		设置http上报的url
        @param  url上报配置

        */
        void UpdateHttpReportUrl(const std::string& url);

        /**
		设置wss 上报得url
        @param url wss 上报得url
        @param default_ipv4_list 保底IPV4地址
        @param default_ipv6_list 保底IPV6地址

        */
        void UpdateWssReportUrl(const std::string &url, const std::vector<std::string>& default_ipv4_list, const std::vector<std::string>& default_ipv6_list);

         /**
		设置wss 上报时的证书
        @param  wss_cert_data  证书指针

        */
        void UpdateWssReportCert(std::shared_ptr<CZegoCACertData> wss_cert_data);
        
        /**
		更新本地与服务端时间的偏移差
        @param local_time_offset

        */
        void UpdateLocalTimeOffset(int64 local_time_offset);

        /**
		事件上报

        @param request 请求
        @param response 请求回应
        @return 成功返回对应seq 失败返回0
        */
        uint32 ReportEventSpeedlog(const EventSpeedlogRequest &request, const OnEventSpeedlogResponse& response);
        
        /**
		质量计费信息上报
        
        @param request 请求
        @param response 请求回应
        @return 成功返回对应seq 失败返回0
        */
        uint32 ReportStreamSpeedlog(const StreamSpeedlogRequest &request, const OnStreamSpeedlogResponse& response);
        
        /**
        设置有上报成功得回调通知

        */

        void SetHaveReportSuccessCallback(const OnHaveReportSuccessCallback& callback);
       
        /**
        设置获取限速的回调通知

        */

        void SetAcquireTokenbucketCallback(const OnAcquireTokenbucketCallback &callback);

      private:
          std::shared_ptr<CRtcNetReportImpl> rtc_net_report_impl_;
    };
    
}// CONNECTION
}// ZEGO


#endif