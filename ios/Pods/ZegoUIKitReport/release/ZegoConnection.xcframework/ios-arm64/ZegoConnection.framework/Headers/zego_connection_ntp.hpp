#ifndef zego_connection_ntp_hpp
#define zego_connection_ntp_hpp

#include <memory>
#include <vector>
#include <string>
#include "zego_connection_ntp_define.hpp"

/* 
    非线程安全，单线程调用
*/

namespace ZEGO
{

namespace CONNECTION
{
    class CNTPImpl;

    class ZEGOCONNECTION_API ZegoConnectionNTP
    {        
    public:
        static std::shared_ptr<ZegoConnectionNTP> GetInstance();
        virtual ~ZegoConnectionNTP();

    private:
        ZegoConnectionNTP();

    public:

        /**
        初始化NTPImpl 模块

        */
        ZCError Init();

        /**
        反初始化NTPImpl

        */
        void UnInit();

        /**
        启动NTP同步时间，回调不在调用方的调用线程

        */
        void StartNTPSync(const std::vector<std::string>& servers, const StartNTPSyncConfig& config, const OnStartNTPSyncDelegate& delegate);

        /**
        停止继续NTP同步时间

        */
        void StopNTPSync();
        
    private:
        std::shared_ptr<CNTPImpl> ntp_impl_ = nullptr;
                
    };
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_ntp_hpp */