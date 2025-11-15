#ifndef zego_connection_network_trace_hpp
#define zego_connection_network_trace_hpp

#include <memory>
#include "zego_connection_network_trace_define.hpp"

/* 
    非线程安全，单线程调用
*/

namespace ZEGO
{

namespace CONNECTION
{
	class CNetworkTraceImpl;

    class ZEGOCONNECTION_API ZegoConnectionNetworkTrace
    {        
	public:
		static std::shared_ptr<ZegoConnectionNetworkTrace> GetInstance();
		virtual ~ZegoConnectionNetworkTrace();

	private:
		ZegoConnectionNetworkTrace();

    public:

		/**
		初始化NetworkTraceImpl 模块

		*/
		ZCError Init();

		/**
		反初始化NetworkTraceImpl

		*/
		void UnInit();

		/**
		开启网络探测

		*/
		void StartNetworkTrace(const TraceConfig& config,const OnNetworkTraceDelegate& delegate);

		/**
		停止网络探测

		*/
		void StopNetworkTrace();


        
    private:
		std::shared_ptr<CNetworkTraceImpl> neworktrace_impl_ = nullptr;
                
    };
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_zegoconnectiontraceroute_hpp */