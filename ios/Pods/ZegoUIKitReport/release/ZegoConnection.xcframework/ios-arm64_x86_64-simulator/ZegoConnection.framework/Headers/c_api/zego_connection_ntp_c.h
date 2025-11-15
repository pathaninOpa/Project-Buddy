#ifndef ZEGO_CONNECTION_NTP_H_
#define ZEGO_CONNECTION_NTP_H_

#include "zego_connection_api_defines_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief NTP 服务地址回调
 * 
 * @param code 错误码
 * @param servers NTP 服务地址，多个地址使用空格分隔
 * 
 */
typedef void (*zego_connection_ntp_on_get_servers)(uint32_t code, const char *servers);

/**
 * @brief 注册 NTP 服务地址回调
 * 
 * @param callback NTP 服务地址回调
 */
ZEGOCONNECTION_CAPI void zego_connection_ntp_register_get_servers_callback(
    zego_connection_ntp_on_get_servers callback);

/**
 * @brief 获取 NTP 服务地址
 */
ZEGOCONNECTION_CAPI void zego_connection_ntp_get_servers(uint32_t timeout);

/**
 * @brief 初始化 NTP 模块
 */
ZEGOCONNECTION_CAPI void zego_connection_ntp_init();

/**
 * @brief NTP 时间同步回调
 * 
 * @param data NTP 时间同步数据
 * 
 */
typedef void (*zego_connection_ntp_on_sync_data)(struct zego_ntp_sync_data *data);

/**
 * @brief 注册 NTP 时间同步回调
 * 
 * @param callback NTP 时间同步回调
 */
ZEGOCONNECTION_CAPI void zego_connection_ntp_register_on_sync_data_callback(
    zego_connection_ntp_on_sync_data callback);

/**
 * @brief 开始同步 NTP 时间
 * 
 * @param servers NTP 服务器域名列表，多个域名使用空格分隔
 * @param config NTP 时间同步配置
 */
ZEGOCONNECTION_CAPI void zego_connection_ntp_start_sync(const char *servers,
                                                               struct zego_ntp_sync_config *config);

/**
 * @brief 停止同步 NTP 时间
 */
ZEGOCONNECTION_CAPI void zego_connection_ntp_stop_sync();

/**
 * @brief 返初始化 NTP 模块
 */
ZEGOCONNECTION_CAPI void zego_connection_ntp_uninit();

#ifdef __cplusplus
}
#endif

#endif
