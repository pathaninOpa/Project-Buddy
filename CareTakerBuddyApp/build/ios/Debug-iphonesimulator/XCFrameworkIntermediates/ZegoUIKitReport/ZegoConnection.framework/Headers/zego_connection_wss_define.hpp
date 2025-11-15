#ifndef zego_connection_wss_define_hpp
#define zego_connection_wss_define_hpp

#include "zego_connection_define.hpp"

namespace ZEGO
{
namespace CONNECTION
{
    typedef uint32 WssInstanceId;

#define  WssInstanceInvaildId 0

    struct WssInstanceConfig
    {
        std::string url;
        std::shared_ptr<CZegoCACertData> cert = nullptr; 
        std::vector<std::string> default_ipv4_list;
        std::vector<std::string> default_ipv6_list;
    };
    
    //callback 回调
    class IWssInstanceCallback 
    {
      public:
        virtual ~IWssInstanceCallback(){};
        virtual void OnConnect(WssInstanceId instance_id, uint32 code, const std::string& message, bool local_net_connected) = 0;
        virtual void OnClosed(WssInstanceId instance_id, uint32 code, const std::string& message) = 0;
        virtual void OnRecv(WssInstanceId instance_id, const std::string&  recv_data) = 0;
    };

    }
}


#endif 