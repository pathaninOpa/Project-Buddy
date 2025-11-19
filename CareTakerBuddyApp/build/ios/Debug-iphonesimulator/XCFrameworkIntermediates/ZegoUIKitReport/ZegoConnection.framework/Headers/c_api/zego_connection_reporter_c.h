//
// Created by zego on 2023/7/24.
//

#ifndef ZEGOCONNECTIONDEMO_ZEGO_CONNECTION_REPORTER_C_H
#define ZEGOCONNECTIONDEMO_ZEGO_CONNECTION_REPORTER_C_H

#include "zego_connection_api_defines_c.h"

#ifdef __cplusplus
extern "C" {
#endif

ZEGOCONNECTION_CAPI void zego_connection_reporter_init(const char* product_name, const zego_reporter_options* options);
ZEGOCONNECTION_CAPI void zego_connection_reporter_uninit();
ZEGOCONNECTION_CAPI void zego_connection_reporter_set_token(const char* token);
ZEGOCONNECTION_CAPI void zego_connection_reporter_set_url(const char* url);
ZEGOCONNECTION_CAPI void zego_connection_reporter_set_package_size_byte(unsigned int size);
ZEGOCONNECTION_CAPI void zego_connection_reporter_set_package_item_count(unsigned int count);
ZEGOCONNECTION_CAPI void zego_connection_reporter_set_report_interval(unsigned int interval);
ZEGOCONNECTION_CAPI void zego_connection_reporter_set_retry_interval(unsigned int interval);
ZEGOCONNECTION_CAPI void zego_connection_reporter_set_wait_ntp_timeout(unsigned int timeout);
ZEGOCONNECTION_CAPI void zego_connection_reporter_set_ntp_offset(int offset);
ZEGOCONNECTION_CAPI void zego_connection_reporter_start_report(const char* user_id, const char* process);
ZEGOCONNECTION_CAPI void zego_connection_reporter_report(const char* event_name, const char* content, unsigned int level, bool instant);
ZEGOCONNECTION_CAPI void zego_connection_reporter_instant_report();


#ifdef __cplusplus
}
#endif

#endif //ZEGOCONNECTIONDEMO_ZEGO_CONNECTION_REPORTER_C_H
