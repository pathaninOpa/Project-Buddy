#ifndef zego_connection_net_detect_define_hpp
#define zego_connection_net_detect_define_hpp

#include "zego_connection_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{	
    //探测时发送的数据
	struct ZegoNetDetectDataConfigNode
	{
		std::string sendPaddingData; //发送给探测服务器的数据大小
		//uint32 uRspDataSize = 0; // 制定探测服务器给 rsp data size 用户mtu 下行测试  udp 有效
	};

    struct ZegoNetDetectDataResultNode
    {
		uint32 uCode = 0;
        uint64 uSendTime = 0; //探测数据开始发送的时间
        uint64 uRecvTime = 0;  //探测数据 接受时间
		//std::string recvBuf;  //探测服务器返回的数据
		void Reset()
		{
			//recvBuf.clear();
			uSendTime = 0;
			uRecvTime = 0;
		}
	};

    struct ZegoNetDetectDataResult
    {
        std::vector<ZegoNetDetectDataResultNode> dataDetail;
	};

	struct ZegoNetDetectRequest
    {
        std::string target;  // ip or host
        bool isHost = false;
        uint32 port = 0;
        uint32 timeout = 3000;
        int nType = 0; //0 tcp 1 udp  
        int reason = 0;      // triggle reason, custom define
		bool bReport = true;
		std::vector<ZegoNetDetectDataConfigNode> config;  //探测数据配置
		Socks5ProxyConfig socks5Config;
    };

	struct ZegoHttpDetectRequest
	{
		std::string detectUrl;  // url
		bool isHost = false;
		std::string ip; //填此ip 则会直接连ip 否则会域名解析url
		uint32 port = 0;
		uint32 timeout = 3000;
		int reason = 0;      // triggle reason, custom define
		std::string tag; //ip tag
		bool bSendDetectdata = false;

		bool bReport = true; //探测数据上报
	};


    struct ZegoNetDetectResult
    {
		ZCSeq uSeq = 0;
        ZCError error = kZCSuccess;
        std::string ip;
        uint32 port = 0;
        uint64 consumeTime = 0; //链接时间 tcp/http 有效
		std::string tag; //ip tag
		
		//与ZegoDetectReportInfo 数据耦合
		std::string host = "";  //域名
		uint64 startTime = 0;
		uint64 finishTime = 0;

		uint64 dnsStartTime = 0;
		uint64 dnsFinishTime = 0;

		ZegoNetDetectDataResult data ; //探测时发送数据
    };

    struct ZegoDetectReportInfo
    {
        uint32 type = 0; // 0 tcp detect, 1 http detect
        int reason = 0; // triggle reason
        ZCError error;
        std::string host = "";
        std::string ip = "";
        uint32 port = 0;
        
        uint64 startTime = 0;
        uint64 finishTime = 0;
        
        uint64 dnsStartTime = 0;
        uint64 dnsFinishTime = 0;
    };
    
    /**
	TCP检测响应

	@param result 网络检测结果
	@return
	@notce
	*/
	using OnTCPDetectDelegate = std::function<void(const std::shared_ptr<ZegoNetDetectResult>& result)>;
   
    using OnHTTPDetectDelegate = std::function<void(const std::shared_ptr<ZegoNetDetectResult>& result)>;
	using OnUDPDetectDelegate = std::function<void(const std::shared_ptr<ZegoNetDetectResult>& result)>;

    using OnReportDetectDelegate = std::function<void(const std::shared_ptr<ZegoDetectReportInfo>& result)>;
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_monitor_define_hpp */
