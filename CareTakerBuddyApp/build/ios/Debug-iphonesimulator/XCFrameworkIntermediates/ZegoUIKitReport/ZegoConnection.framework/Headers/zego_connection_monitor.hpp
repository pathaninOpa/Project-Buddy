//
//  网络状态监控接口
//
//  zego_connection_monitor.h
//  
//
//  Created by arvin on 2019/07/18.
//

#ifndef zego_connection_monitor_hpp
#define zego_connection_monitor_hpp

#include <memory>
#include "zego_connection_monitor_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{     
	class NetMonitorImpl;

    class ZEGOCONNECTION_API ZegoConnectionMonitor
    {
    public:
        static std::shared_ptr<ZegoConnectionMonitor> GetInstance();
        virtual ~ZegoConnectionMonitor();
        
    private:
		    ZegoConnectionMonitor();
        
    public:        
        /**
         注册网络变化监听
         
         @param delegate 状态变化回调
         @note 注意 delegate 的回调不一定在 Register 的线程
         @return 注册的序列号
         */
        ZCSeq RegisterMonitorDelegate(const OnNetStateChangedDelegate& delegate);
        
        /**
         取消监听网络状态变化
         
         @param seq 注册的序列号
         */
        void UnRegisterMonitorDelegate(ZCSeq seq);
        
        /**
         获取当前网络类型
         
         @note 在初始化阶段，在 iOS 平台下，返回值有可能不精确，当网络是移动网络类型时，需要异步才能获取到具体的类型，此时会返回 ZegoConnectionNetType::ZEGO_NT_UNKNOWN 类型。如果需要准确的类型，请调用 [GetCurrentNetType(const OnGetCurrentNetTypeDelegate&)] 方法
         @return 返回当前网络类型
         */
        ZegoConnectionNetType GetCurrentNetType();

        /**
         获取当前网络类型
         
         @note 注意 delegate 的回调不一定在调用的线程
         */
        void GetCurrentNetType(const OnGetCurrentNetTypeDelegate& delegate);

        /**
         获取上一次的网络类型
         
         @return 返回上一次的网络类型
         */
        ZegoConnectionNetType GetLastNetType();

        /**
         手动检查网络类型是否变化
         
         */
        void CheckCurrentNetType();
        
        /**
         注册全链路IP协议栈状态变化监听
         
         @param delegate 状态变化回调
         @note 注意 delegate 的回调不一定在 Register 的线程
         @return 注册的序列号
         */
        ZCSeq RegisterIPStackDelegate(const OnIPStackStateChangedDelegate& delegate);
        
        /**
         取消监听全链路IP协议栈状态变化
         
         @param seq 注册的序列号
         */
        void UnRegisterIPStackDelegate(ZCSeq seq);
        
        /**
         注册客户端IP协议栈状态变化监听
         
         @param delegate 状态变化回调
         @note 注意 delegate 的回调不一定在 Register 的线程
         @return 注册的序列号
         */
        ZCSeq RegisterClientIPStackDelegate(const OnIPStackStateChangedDelegate& delegate);
        
        /**
         取消监听客户端IP协议栈状态变化
         
         @param seq 注册的序列号
         */
        void UnRegisterClientIPStackDelegate(ZCSeq seq);
        
        /**
         设置客户端IP协议栈检测结果回调
         
         @param delegate 发生客户端IP协议栈检测完成时触发
         */
        void SetCheckIPStackDelegate(const OnCheckIPStackDelegate& delegate);
        
        /**
         获取当前网络的IP节点连通状态
            
         @param ipv4 IPv4节点的最新连通状态
         @param ipv6 IPv6节点的最新连通状态
         */
        void GetIPStackState(ReachableState& ipv4, ReachableState& ipv6);
        
        /**
         获取当前网络的全链路IP协议栈连通状态
            
         @param ipv4 IPv4协议栈的最新连通状态
         @param ipv6 IPv6协议栈的最新连通状态
         */
        void GetServerIPStackState(IPStack stack, ReachableState& state);
        
        /**
         检查当前网络的IP协议栈连通状态
         
         @param ipv4 IPv4协议栈的最新连通状态
         @param ipv6 IPv6协议栈的最新连通状态
         */
        void CheckIPStackState(ReachableState& ipv4, ReachableState& ipv6, const std::string& reason);
        
        /**
         检查当前网络的客户端IP协议栈是否发生变更，异步触发
    
         */
        void AsyncCheckClientIPStackState();
        
        /**
         更新当前网络的IP协议栈连通状态
         
         @param stack IP协议栈类型
         @param state IP协议栈的连通状态
         @param reason 更新协议栈的原因
         */
        void UpdateIPStackState(IPStack stack, ReachableState state, const std::string& reason);

        /**
         获取 wifi 信号值

         @return 返回 dbm 值
         @note 不支持ios/linux平台
         */
        int GetWifiSignalDbm();

        /**
         获取 wifi 信号等级

         @return 返回信号等级，有效值 [0, 4]，0 - none or unknown, 1 - poor, 2 - moderate , 3 - greate, 4 - good
         @note 不支持ios/linux平台
         */
        int GetWifiSignalLevel();

        /**
         获取移动信号值

         @return 返回 dbm 值
         @note 只支持android
         */
        int GetCellSignalDbm();

        /**
         获取移动信号等级

         @return 返回信号等级，有效值 [0, 4]，0 - none or unknown, 1 - poor, 2 - moderate , 3 - greate, 4 - good
         @note 只支持android
         */
        int GetCellSignalLevel();

	private:
		    std::shared_ptr<NetMonitorImpl> impl_;
    };
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_monitor_hpp */
