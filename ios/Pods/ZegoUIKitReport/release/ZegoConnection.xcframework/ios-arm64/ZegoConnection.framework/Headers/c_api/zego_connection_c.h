//
// Created by zego on 2023/7/25.
//

#ifndef ZEGO_CONNECTION_C_H
#define ZEGO_CONNECTION_C_H

#include "zego_connection_api_defines_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 Android 平台信息
 *
 * @param jvm Java VM
 * @param ctx Android Context
 * @param cls_loader 类加载器
 */
ZEGOCONNECTION_CAPI void zego_connection_init_platform(void *jvm, void *ctx, void *cls_loader);

#ifdef __cplusplus
}
#endif

#endif //ZEGO_CONNECTION_C_H
