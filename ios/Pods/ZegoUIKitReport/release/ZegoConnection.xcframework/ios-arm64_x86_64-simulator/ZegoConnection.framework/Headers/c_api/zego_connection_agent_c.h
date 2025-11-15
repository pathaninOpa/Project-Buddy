#ifndef ZEGO_CONNECTION_AGENT_H_
#define ZEGO_CONNECTION_AGENT_H_

#include "zego_connection_api_defines_c.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*zego_connection_on_net_type_change)(enum zego_net_type type);

/**
 * @brief 注册网络状态变化回调
 * 
 * @param callback 网络状态变化回调
 */
ZEGOCONNECTION_CAPI void
zego_connection_register_on_net_type_change_callback(zego_connection_on_net_type_change callback);

/**
 * @brief 设置本地缓存的 ZegoNS 域名解析结果到 ZegoConnection
 * 
 * @param ns_addresses ZegoNS 域名解析结果列表
 * @param len ZegoNS 域名解析结果个数
 * @param source ZegoNS 域名类型
 */
ZEGOCONNECTION_CAPI void
zego_connection_set_zegons_address_info(struct zego_name_server_address *ns_addresses,
                                        uint32_t len, enum zego_name_server_source source);

typedef void (*zego_connection_save_zegons_address_info)(
    struct zego_app_info *app_info, enum zego_dns_strategy strategy,
    struct zego_name_server_address *ns_addresses, uint32_t len);

/**
 * @brief 注册缓存 ZegoNS 域名解析结果回调函数
 * 
 * @param callback 缓存 ZegoNS 域名解析结果回调函数
 */
ZEGOCONNECTION_CAPI void zego_connection_register_save_zegons_address_info_callback(
    zego_connection_save_zegons_address_info callback);

/**
 * @brief 设置本地缓存的统一接入地址信息到 ZegoConnection
 * 
 * @param address_info 本地缓存的统一接入地址信息
 */
ZEGOCONNECTION_CAPI void
zego_connection_set_net_agent_address_info(struct zego_net_agent_address_info *address_info);

typedef void (*zego_connection_save_net_agent_address_info)(
    struct zego_app_info *app_info, struct zego_net_agent_address_info *address_info);

/**
 * @brief 注册缓存统一接入地址信息回调函数
 * 
 * @param callback 缓存统一接入地址信息回调函数
 */
ZEGOCONNECTION_CAPI void zego_connection_register_save_net_agent_address_info_callback(
    zego_connection_save_net_agent_address_info callback);

/**
 * @brief 设置统一接入保底 IP 地址，多个 IP 地址之间使用空格分隔
 * 
 * @param backup_ips 保底 IP 地址
 */
ZEGOCONNECTION_CAPI void zego_connection_net_agent_set_backup_ips(const char *backup_ips);

/**
 * @brief 设置统一接入默认域名
 * 
 * @param domain 统一接入默认域名
 */
ZEGOCONNECTION_CAPI void zego_connection_net_agent_set_default_domain(const char *domain);

/**
 * @brief 设置用户 ID
 * 
 * @param user_id 用户 ID
 */
ZEGOCONNECTION_CAPI void zego_connection_net_agent_set_user_id(const char *user_id);

/**
 * @brief 初始化统一接入模块
 * 
 * @param option 初始化设置
 */
ZEGOCONNECTION_CAPI void zego_connection_net_agent_init(struct zego_net_agent_option* option);

/**
 * @brief 初始化回调
 * 
 * @param code 错误码，0 表示成功，其它表示失败
 * 
 */
typedef void (*zego_connection_net_agent_on_inited)(uint32_t code);

/**
 * @brief 注册初始化回调
 * 
 * @param callback 初始化回调
 */
ZEGOCONNECTION_CAPI void zego_connection_net_agent_register_on_inited_callback(
    zego_connection_net_agent_on_inited callback);                                                        

/**
 * @brief 连接状态回调
 * 
 * @param conn_id 连接 ID
 * @param code 错误码，0 表示成功，其它表示失败
 * 
 */
typedef void (*zego_connection_net_agent_on_connected)(uint32_t conn_id, uint32_t code);

/**
 * @brief 注册统一接入连接状态回调
 * 
 * @param callback 连接状态回调
 */
ZEGOCONNECTION_CAPI void zego_connection_net_agent_register_on_connected_callback(
    zego_connection_net_agent_on_connected callback);

/**
 * @brief 建立统一接入连接
 * 
 * @param agent_service 统一接入服务名称，例如：“zpush”，“switch”，“metaworld” 和 “zegoland” 等
 * @param timeout_ms 超时时间，单位：毫秒
 * @return 建立连接成功，返回对应的连接 ID；否则返回 0 
 */
ZEGOCONNECTION_CAPI uint32_t zego_connection_net_agent_connect(const char *agent_service,
                                                               uint32_t timeout_ms);

/**
 * @brief 通过统一接入连接发送数据
 * 
 * @param conn_id 统一接入连接 ID
 * @param agent_service 统一接入服务名称，例如：“zpush”，“switch”，“metaworld” 和 “zegoland” 等
 * @param buf 要发送的数据
 * @param len 数据长度
 * @return 发送成功，返回 0；否则返回非 0 
 */
ZEGOCONNECTION_CAPI int32_t zego_connection_net_agent_send(uint32_t conn_id,
                                                           const char *agent_service, uint8_t *buf,
                                                           uint32_t len);

typedef void (*zego_connection_net_agent_on_receive)(uint32_t conn_id, uint8_t *buf, uint32_t len);

/**
 * @brief 注册统一接入接收数据回调接口
 * 
 * @param callback 接收数据回调接口
 */
ZEGOCONNECTION_CAPI void zego_connection_net_agent_register_on_receive_callback(
    zego_connection_net_agent_on_receive callback);

typedef void (*zego_connection_net_agent_on_closed)(uint32_t conn_id, uint32_t code);

/**
 * @brief 注册统一接入连接关闭回调接口
 * 
 * @param callback 统一接入连接关闭回调接口
 */
ZEGOCONNECTION_CAPI void
zego_connection_net_agent_register_on_closed_callback(zego_connection_net_agent_on_closed callback);

/**
 * @brief 关闭统一接入连接
 * 
 * @param conn_id 统一接入连接 ID
 */
ZEGOCONNECTION_CAPI void zego_connection_net_agent_close(uint32_t conn_id);

typedef void (*zego_connection_net_agent_on_http_response)(struct zego_net_agent_http_context *ctx);
ZEGOCONNECTION_CAPI uint32_t
zego_connection_net_agent_start_http_request(struct zego_net_agent_http_request *req,
                                             zego_connection_net_agent_on_http_response rsp);

/**
 * @brief 返初始化统一接入模块
 */
ZEGOCONNECTION_CAPI void zego_connection_net_agent_uninit();

#ifdef __cplusplus
}
#endif

#endif
