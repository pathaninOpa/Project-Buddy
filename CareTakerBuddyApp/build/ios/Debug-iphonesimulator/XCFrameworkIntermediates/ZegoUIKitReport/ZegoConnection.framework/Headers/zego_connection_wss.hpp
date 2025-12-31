#ifndef zego_connection_wss_hpp
#define zego_connection_wss_hpp

#include <memory>
#include "zego_connection_wss_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{
    class WssImpl;

    class ZEGOCONNECTION_API ZegoConnectionWssManage 
    {
      public:
        static std::shared_ptr<ZegoConnectionWssManage> GetInstance();
        virtual ~ZegoConnectionWssManage();

      private:
        ZegoConnectionWssManage();

      public:
        /**
		初始化 wss模块
        
        */
        ZCError Init();

        /**
		反初始化 wss模块
        
        */
        void UnInit();

        /**
        设置IP协议栈模式

        @param mode IP协议栈模式
        @notce
        */
        void SetIPStackMode(IPStackMode mode);

        /**
		创建wss 实例

        @param instance_config 创建实例配置
        @param instance_callback 创建回调
        @return 非0 成功，成功的实例id
        
        */
        WssInstanceId CreateWssInstance(const WssInstanceConfig& instance_config);

        /**
		销毁wss 实例

        @param instance_id 实例ID 由CreateWssInstance 

        */
        void DestroyWssInstance(WssInstanceId instance_id);

        /**
		链接服务

        @param instance_id 实例ID 
        @param timeout  超时时间
        @param instance_callback 链接时间回调
        @return 返回false 时instance_id 实例会被删除
        @attention 返回 OnConnect 成功后调用 

        */
        bool ConnectWssServer(WssInstanceId instance_id, int timeout, std::weak_ptr<IWssInstanceCallback> instance_callback);

        /**
		销毁wss 实例

        @param instance_id 实例ID 
        @param send_packet 发送的数据包
        @param response 服务回应数据包
        @attention 返回 OnConnect 成功后调用 
        
        */
        bool SendWssData(WssInstanceId instance_id, const std::string& send_data);

        /**
		断开服务链接

        @param instance_id 实例ID 
        
        */
        void DisConnectWssServer(WssInstanceId instance_id);
        
        /**
		更新服务器时间 wss 进行证书验证时，会根据设置的local_time_offset 计算正确的时间
        
        */
        void UpdateLocalTimeOffset(int64 local_time_offset);

    
    private:
        std::shared_ptr<WssImpl> wss_impl_;
    };
    
}// CONNECTION
}// ZEGO


#endif