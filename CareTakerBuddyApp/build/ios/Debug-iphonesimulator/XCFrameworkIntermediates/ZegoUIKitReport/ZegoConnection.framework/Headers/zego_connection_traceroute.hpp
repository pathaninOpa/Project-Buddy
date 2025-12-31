#ifndef zego_connection_traceroute_hpp
#define zego_connection_traceroute_hpp

#include <memory>
#include "zego_connection_traceroute_define.hpp"

/* 
    非线程安全，单线程调用
*/

namespace ZEGO
{

namespace CONNECTION
{
	class CTracerouteImpl;

    class ZEGOCONNECTION_API ZegoConnectionTraceroute
    {        
	public:
		static std::shared_ptr<ZegoConnectionTraceroute> GetInstance();
		virtual ~ZegoConnectionTraceroute();

	private:
		ZegoConnectionTraceroute();
  
    public:
        /**
        初始化模块
        
        @param 
		@return 返回错误码
        @notce 
        */
		ZCError Init();

        /**
        反初始化模块
        
        @param 
        @notce 
        */
        void UnInit();

        /**
        设置 connection traceroute 回调类
        
        @param callback 实现对通信的各种状态的处理
        @notce 
        */
        void SetCallback(IZegoConnectionTracerouteCallback* callback);


        /**
        开始ping

        @param data

		@return 返回Seq  非0 成功
        */
		ZCSeq StartPing(const PingConfig& data);

        /**
        停止ping
        
        @param  seq start时的seq
        @notce 
        */
		void StopPing(ZCSeq seq);
        
        /**
       开启traceroute
        
        @param data 数据
		@return seq 非0 成功
        @notce 
        */
		ZCSeq StartTraceroute(const RouteConfig& data);

		/**
		停止traceroute

		@param data 数据
		@return seq 非0 成功
		@notce
		*/
		void StopTraceroute(ZCSeq seq);
        
    private:
		std::shared_ptr<CTracerouteImpl> traceroute_impl_;
                
    };
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_zegoconnectiontraceroute_hpp */