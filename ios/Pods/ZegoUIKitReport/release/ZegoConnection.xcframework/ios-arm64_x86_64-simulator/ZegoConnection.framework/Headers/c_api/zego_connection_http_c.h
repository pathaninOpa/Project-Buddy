//
// Created by zego on 2023/7/25.
//

#ifndef ZEGO_CONNECTION_HTTP_C_H
#define ZEGO_CONNECTION_HTTP_C_H


#include "zego_connection_api_defines_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 http 模块
 */
ZEGOCONNECTION_CAPI void zego_connection_net_http_init();

/**
 * @brief 反初始化 http 模块
 */
ZEGOCONNECTION_CAPI void zego_connection_net_http_uninit();

#ifdef __cplusplus
}
#endif

#endif //ZEGO_CONNECTION_HTTP_C_H
