//
// Created by longjuncai on 2023/10/16.
//

#ifndef ZEGO_CONNECTION_CLOUD_SETTING_C_H
#define ZEGO_CONNECTION_CLOUD_SETTING_C_H

#include "zego_connection_api_defines_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化云控
 *
 * @param option 初始化配置参数
 */
ZEGOCONNECTION_CAPI void zego_connection_cloud_setting_init(struct zego_cloug_setting_options* option);

/**
 * @brief 反初始化云控
 *
 */
ZEGOCONNECTION_CAPI void zego_connection_cloud_setting_uninit();

/**
 * @brief 设置用户ID
 *
 */
ZEGOCONNECTION_CAPI void zego_connection_cloud_setting_set_user_id(const char* userid);

/**
 * @brief 设置 token
 * note: 需要使用 token 鉴权时传入此参数
 *
 */
ZEGOCONNECTION_CAPI void zego_connection_cloud_setting_set_token(const char* token);

/**
 * @brief 设置用户参数 key - value
 *
 */
ZEGOCONNECTION_CAPI void zego_connection_cloud_setting_set_custom_param(const char* key, const char* value);

/**
 * @brief 请求获取云控配置
 *
 */
ZEGOCONNECTION_CAPI void zego_connection_cloud_setting_fetch_cloud_setting_config();

/*
* @brief 请求云控配置结果回调定义
*/
typedef void(*zego_cloud_setting_config_result_callback)(struct zego_cloud_setting_config_result* changed_infos, struct zego_cloud_setting_config_result* all_infos);

/**
 * @brief 设置请求云控配置结果回调
 *
 */
ZEGOCONNECTION_CAPI void zego_connection_cloud_setting_register_fetch_cloud_setting_result_callback(zego_cloud_setting_config_result_callback callback);

/**
 * @brief 获取本地缓存的云控配置
 *
 */
ZEGOCONNECTION_CAPI struct zego_cloud_setting_cache_config* zego_connection_cloud_setting_get_cloud_setting_cache_config();

#ifdef __cplusplus
}
#endif

#endif //ZEGO_CONNECTION_C_H