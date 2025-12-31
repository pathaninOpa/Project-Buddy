#ifndef zego_connection_monitor_define_hpp
#define zego_connection_monitor_define_hpp

#include "zego_connection_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{
    /**
    网络状态变化响应
    
    @param result 网络状态变化信息
    @return  
    @note
    */
	using OnNetStateChangedDelegate = std::function<void(ZegoConnectionNetType nt)>;

    /**
     获取当前网络类型
     
     @param net_type 网络类型
     @return
     @note 
     */
    using OnGetCurrentNetTypeDelegate = std::function<void(ZegoConnectionNetType net_type)>;

    /**
    IP协议栈连通状态变化响应

    @param ipv4 IPv4协议栈的最新连通状态
    @param ipv6 IPv6协议栈的最新连通状态
    @note
    */
    using OnIPStackStateChangedDelegate = std::function<void(ReachableState ipv4, ReachableState ipv6)>;

    /**
    IP协议栈检测结果通知

    @param time_consuming 检测耗时
    @note
    */
    using OnCheckIPStackDelegate = std::function<void(uint32 time_consuming)>;

}// CONNECTION
}// ZEGO


#endif /* zego_connection_monitor_define_hpp */
