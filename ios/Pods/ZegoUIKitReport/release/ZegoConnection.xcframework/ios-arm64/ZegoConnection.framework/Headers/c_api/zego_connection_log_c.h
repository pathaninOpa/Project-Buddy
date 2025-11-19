#ifndef ZEGO_CONNECTION_LOG_H_
#define ZEGO_CONNECTION_LOG_H_

#include "zego_connection_api_defines_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 设置日志输出文件
 * 
 * @param log_file 日志文件路径
 */
ZEGOCONNECTION_CAPI void zego_connection_set_log_file(const char *log_file);

ZEGOCONNECTION_CAPI bool zego_connection_set_custom_log_file(const char* log_path, const char* file_name, const char* head_info = "");

ZEGOCONNECTION_CAPI void zego_connection_write_custom_log(const char *content, enum zego_custom_log_level level, const char* module = "", int line = 0);

typedef void(*zego_custom_log_upload_response)(zego_log_upload_res);

ZEGOCONNECTION_CAPI void zego_connection_upload_custom_log(struct zego_custom_log_upload_request req, zego_custom_log_upload_response rsp); /*上报自定义日志路径下的文件*/

#ifdef __cplusplus
}
#endif

#endif