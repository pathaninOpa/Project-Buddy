#ifndef zego_connection_network_trace_define_hpp
#define zego_connection_network_trace_define_hpp

#include "zego_connection_define.hpp"
#include "zego_connection_traceroute_define.hpp"
#include "zego_connection_net_detect_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{
	struct IPNode
	{
		std::string ip;
		std::string tag;
	};

	struct TraceConfig
	{
		int nTraceroute = 0;  //
		uint32 uHttpMaxTimeout = 6000;  //http 探测超时时间
		uint32 uMaxTcpTimeout = 6000;  //进行tcp 探测时 最大的超时时间时间 
		uint32 uMaxUdpTimeout = 6000;  //进行udp 探测时 最大的超时时间
        //tcp/traceroute url info
		std::string url;  //http url trace 时候的url 信息
		//std::vector<IPNode> ipList;   //ip 列表信息
        uint32 tcp;  //trace 时的 tcp 的端口信息列表
        //udp url info
        std::string udp_url;//only for udp detect, must be zegons server url
        uint32 udp; //trace 时 udp 的端口列表信息
		
	};

	struct TraceData
	{
		uint32 uCode = 0;
		std::string ip;
		uint32 uPort = 0;

		uint64 startTime = 0;
		uint64 finishTime = 0;

		uint32 uConnectTime = 0;  //tcp 有效

		ZegoNetDetectDataResult data;  //当有发送探测数据时有效
	};

	struct HttpTraceData : public TraceData
	{
		std::string url;  //探测传入的url
	};

	struct TcpTraceData
	{
		std::vector<TraceData> vcTcp; 
	};

	struct UdpTraceData
	{
		std::vector<TraceData> vcUdp; 
	};

	struct TracerouteData
	{
		uint32 uTracerouteCode = 0;
		uint32 uRouteTime = 0;
		std::vector<RouteInfo> route;
	};

	struct NetworkTraceData
	{
		uint32 uCode = 0;
		HttpTraceData*       pHttp = nullptr;
		TcpTraceData*         pTcp = nullptr;
		UdpTraceData*        pUdp = nullptr;
		TracerouteData*      pTraceroute = nullptr;
	};

	//delegate   代理类
	using OnNetworkTraceDelegate = std::function<void(const NetworkTraceData& data)>;
}// CONNECTION
}// ZEGO


#endif /* zego_connection_network_trace_define_hpp */
