//
//  网络状态监控接口
//
//  zego_connection_monitor.h
//  
//
//  Created by arvin on 2019/07/18.
//

#ifndef zego_connection_net_detect_hpp
#define zego_connection_net_detect_hpp

#include <memory>
#include <vector>
#include "zego_connection_net_detect_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{     
	class NetDetectorImpl;

    class ZEGOCONNECTION_API ZegoConnectionNetDetect
    {      
	public:
		static std::shared_ptr<ZegoConnectionNetDetect> GetInstance();
        virtual ~ZegoConnectionNetDetect();
        
    private:
        ZegoConnectionNetDetect();
        
    public:
		/**
		初始化模块

		*/
		ZCError Init();

		/**
		反始化模块

        */
		void UnInit();


		/**
		设置 app 信息 目前如果需要与探测服务器 收发数据时需要此信息
		*/
		void SetAppInfo(const ZegoAppInfo& info);

        /**
        停止全部网络检测
        
        @param 
        @notce 
        */
        void StopAllDetect();

		/**
		检测TCP网络连通性

		@param request 请求信息
		@param delegate 检测结果回调
		@return 非0 成功，成功的seq
		*/
		ZCSeq StartTCPDetect(const ZegoNetDetectRequest& request, const OnTCPDetectDelegate& delegate);
        
		/**
		停止TCP 探测

		@param uSeq  Start时的seq
		@return 
		*/
		void StopTCPDetect(ZCSeq uSeq);


		/**
		检测udp网络连通性

		@param request 请求信息
		@param delegate 检测结果回调
		@return 任务是否启动成功
		*/
		ZCSeq StartUDPDetect(const ZegoNetDetectRequest& request, const OnUDPDetectDelegate& delegate);

		/**
		停止UDP 探测

		@param uSeq  Start时的seq
		@return
		*/
		void StopUDPDetect(ZCSeq uSeq);

        /**
         检测HTTP服务可用性
         
         @param detectUrl http请求的url
         @param delegate 检测结果回调
         @return 任务是否启动成功
         */
		ZCSeq StartHTTPDetect(const ZegoHttpDetectRequest& request, const OnHTTPDetectDelegate& delegate);


		/**
		检测UDP网络连通性

		@param uSeq  Start时的seq
		@return
		*/
		void StopHTTPDetect(ZCSeq uSeq);

        /**
         设置数据上报代理

         @param delegate 数据上报回调
         */
        void SetReportDetectDelegate(const OnReportDetectDelegate& delegate);
        
	private:
		std::shared_ptr<NetDetectorImpl> detector_impl_;
    };
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_net_detect_hpp */
