#ifndef zego_connection_traceroute_define_hpp
#define zego_connection_traceroute_define_hpp

#include "zego_connection_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{
	struct PingConfig
	{
		std::string  url;
		bool bHost = false;
		int nICMPTimeoutMs = 2000; //一次ping的超时时间
		int nPingCount = 4;  //默认ping 4
	};

	struct RouteConfig
	{
		std::string url;  //IP or host
		int port = 33434;  //默认此端口
		bool bHost = false;
		int nMaxFollowJump = 30; //最多跟纵30个跳跃点
		int nMaxOneJumpSendCout = 3; // route 每一跳重复发送ICMP的最大次数。
		int nICMPTimeoutMs = 2000; //发送ICMP协议超时时间默认3s 
	};

	struct PingInfo
	{
		std::string ip;
		int recvData = 0;
		int timeMs = 0;
		int ttl = 0;
		int rtt = 0;
	};

	struct RouteInfo
	{
		int nJump = 0;
		int rtt = 0;
		std::string ip;
		std::string message;
	};

    //回调类
    class IZegoConnectionTracerouteCallback
    {
    public:
        virtual void OnPing(ZCSeq uSeq,const PingInfo& info)= 0;
		virtual void OnPingResult(ZCSeq uSeq,uint32 uCode,const std::string& message) = 0;

		virtual void OnTraceroute(ZCSeq uSeq,const RouteInfo& info) = 0;
        virtual void OnTracerouteResult(ZCSeq uSeq,uint32 uCode,const std::string& message) = 0;
    };
   
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_traceroute_define_hpp */