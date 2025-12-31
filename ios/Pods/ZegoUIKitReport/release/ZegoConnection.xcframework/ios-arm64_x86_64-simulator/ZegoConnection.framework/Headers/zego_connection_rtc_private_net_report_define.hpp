#ifndef zego_connection_rtc_private_report_define_hpp
#define zego_connection_rtc_private_report_define_hpp

#include "zego_connection_define.hpp"

namespace ZEGO
{
namespace CONNECTION
{
    enum ReportEnumOsType
    {
        unknown = 0,
        Windows = 1,
        Android = 2,
        iOS = 3,
        Mac = 4,
        Linux = 5,
        HarmonyOS = 6,
        iOSSimu = 7
    };

    enum ReportStrategy
    {
        WssFirst = 0,
        HttpOnly = 1,
        WssOnly = 2
    };

    struct ReportConfig
    {
        ReportStrategy strategy = ReportStrategy::WssFirst;

        uint32 wss_heartbeat_interval = 30 * 1000;  //ws 的心跳时间间隔
        uint32 wss_heartbeat_timeout = 90 * 1000; //ws 的心跳超时时间
        uint32 wss_idle_timeout = 120 * 1000;  //ws 空闲超时 120s

        uint32 max_connect_timer = 60 * 1000; //ws 持续多长时间链接没成功，切换成http
        uint32 no_report_success_timer = 60 * 1000; //ws 链接成功后1min 都没有事件上报成功过

        void Reset()
        {
            strategy = ReportStrategy::WssFirst;
            wss_heartbeat_interval = 30 * 1000;  //ws 的心跳时间间隔
            wss_heartbeat_timeout = 90 * 1000; //ws 的心跳间隔时间
            wss_idle_timeout = 120 * 1000;  //ws 空闲超时 120s
            max_connect_timer = 60 * 1000; //ws 持续多长时间链接没成功，切换成http
            no_report_success_timer = 60 * 1000; //ws 链接成功后1min 都没有事件上报成功过
        }
    };

    struct HttpCommonParams
    {
        std::string http_user_agent; 
    };

    struct BaseSpeedlogResponse 
    {
        uint32 request_seq = 0;

        bool is_use_wss = true;
        bool is_server_error = false;
        uint32 error = 0;
        std::string message;
    };

    struct EventSpeedlogRequest : public HttpCommonParams
    {
        std::vector <std::string> event_list;
        std::string process;
    };
    
    struct EventSpeedlogResponse : public BaseSpeedlogResponse
    {
        
    };
    
    using OnEventSpeedlogResponse = std::function<void(const std::shared_ptr<EventSpeedlogResponse>& response)>;

    struct StreamSpeedlogRequest : public HttpCommonParams
    {
        std::string stream_data;
    };
    
    struct StreamSpeedlogResponse : public BaseSpeedlogResponse
    {
        std::string response_data; //返回数据 http 上报时返回有效
    };

     using OnStreamSpeedlogResponse = std::function<void(const std::shared_ptr<StreamSpeedlogResponse>& response)>;

     using OnHaveReportSuccessCallback = std::function<void(uint32 seq, bool is_http)>;

     using OnAcquireTokenbucketCallback = std::function<bool(unsigned int data_size, unsigned int& limit)>;
}
}


#endif 