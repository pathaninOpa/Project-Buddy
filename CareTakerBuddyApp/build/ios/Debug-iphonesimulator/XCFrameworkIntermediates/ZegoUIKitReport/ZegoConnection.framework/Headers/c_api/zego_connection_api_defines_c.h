#ifndef ZEGO_CONNECTION_API_DEFINES_H_
#define ZEGO_CONNECTION_API_DEFINES_H_

#include <stdarg.h>
#include <stdint.h>

#ifndef ZEGOCONNECTION_CAPI
#if defined(_WIN32)
#ifdef ZEGOCONNECTION_EXPORT
#define ZEGOCONNECTION_CAPI __declspec(dllexport)
#elif defined(ZEGOCONNECTION_STATIC)
#define ZEGOCONNECTION_CAPI // * nothing
#else
#define ZEGOCONNECTION_CAPI __declspec(dllimport)
#endif
#else
#define ZEGOCONNECTION_CAPI __attribute__((visibility("default")))
#endif // _WIN32
#endif // ZEGOCONNECTION_CAPI

struct zego_app_info {
    uint64_t app_id;
    int business_type;
    int mode;
    char *user_id;
    char *device_id;
};

enum zego_custom_log_level
{
    debug    = 0,
    info     = 1,
    warn     = 2,
    err      = 3
};

enum zego_net_type {
    ZEGO_NT_NONE = 0,
    ZEGO_NT_LINE = 1,
    ZEGO_NT_WIFI = 2,
    ZEGO_NT_2G = 3,
    ZEGO_NT_3G = 4,
    ZEGO_NT_4G = 5,
    ZEGO_NT_5G = 6,
    ZEGO_NT_UNKNOWN = 32
};

enum zego_dns_strategy {
    LOCALDNS_FIRST, //!< 本地 DNS 优先
    ZEGONS_FIRST,   //!< ZegoNS 优先
};

enum zego_name_server_protocol {
    TCP = 0,
    UPD = 1,
};

struct zego_name_server_address {
    enum zego_name_server_protocol protocol;
    char *ip_address;
    int address_type; //!< 地址类型，例如：AF_INET 等
    uint32_t port;    //!< 端口号
};

enum zego_name_server_source {
    HARDCODE_IP = 0,
    LOCAL_DNS_RESOLVE,
    SERVER_RESPOND,
};

struct zego_net_agent_option
{
    uint64_t app_id;
    unsigned char* appsignature;
    uint32_t signature_size;
    const char* device_id;
    const char* user_id;
};

struct zego_net_agent_address_group {
    const char *group_name;
    const char *quic_nodes; //!< quic 节点。节点格式为 IP地址:端口，多个节点用空格分割
    const char *mtcp_nodes; //!< mtcp 节点。节点格式为 IP地址:端口，多个节点用空格分割
};

struct zego_net_agent_address_info {
    uint64_t tx_id;

    char *rand;        //!< 随机数，非 '\0' 结尾
    uint32_t rand_len; //!< 随机数缓冲区长度

    const char *secret;
    const char *http_stick;
    const char *tcp_stick;
    const char *location_id;

    struct zego_net_agent_address_group *groups; //!< 统一接入地址列表
    uint32_t groups_len;                         //!< 统一接入地址个数
};

enum zego_net_agent_http_method {
    POST = 0,
    GET = 1,
};

enum zego_net_agent_content_type {
    NONE = 0,
    JSON = 1,
    OCTET_STREAM = 2,
};

struct zego_net_agent_http_request {
    const char
        *agent_service; //!< 统一接入服务类型，例如："zpush"，"switch"，"metaworld" 和 "zegoland" 等
    const char *api;

    uint32_t interface_id; //!<统一接入服务http请求的对应的服务id，注意使用了该项就不用传 agent_service + api
    const char *user_id;
    enum zego_net_agent_http_method method;

    const char *headers; //!< 请求头信息，请求头使用等号分隔，多个请求头通过换行符分隔

    enum zego_net_agent_content_type content_type;
    const char *body;  //!< 请求包体内容
    uint32_t body_len; //!< 请求包体长度

    int is_need_header_info; //!< 是否返回请求头信息
    uint32_t retry_times;    //!< 重试次数，最大允许重试次数为 5
};

struct zego_net_agent_http_context {
    uint32_t seq;        //!< 请求序号
    int64_t status_code; //!< HTTP 状态码

    uint32_t error_code;       //!< 错误码
    const char *error_message; //!< 错误信息

    const char *header; //!< 回包头信息

    const char *body;  //!< 回包包体内容
    uint32_t body_len; //!< 回包包体长度
};

struct zego_ntp_sync_data {
    uint32_t code;
    double offset;
    double max_deviation;
    int8_t finished;
};

struct zego_ntp_sync_config {
    uint32_t max_send_num;
    uint32_t min_send_num;
    uint32_t min_recv_num;
    uint32_t interval;
    uint32_t package_timeout;
    uint32_t sync_timeout;
    uint32_t max_succeed_num;
    uint32_t reference_accuracy;
};

struct zego_reporter_options {
    /* 固定参数 业务相关 */
    uint64_t    app_id;
    unsigned char* appsignature;  /* 若不使用sign方式鉴权 调用SetToken接口设置或更新token */
    uint32_t signature_size;
    const char* device_id;
    const char* db_path;              /* db路径 reporter 内部将在此路径下生成若干文件或文件夹用于本地存储 */
    const char* url;
};

struct zego_custom_log_upload_request
{
    /*  基本参数 */
    uint64_t    app_id;
    unsigned char* appsignature; /*  sign方式使用此参数鉴权 */
    uint32_t signature_size;
    const char* token;        /*  token方式使用此参数鉴权 */
    const char* product;      /*  product */
    const char* id_name;       /*  user idname */
    const char* device_id;     /*  device id */
    const char* url;          /*  请求url */
    const char* querykey;     /*  querykey */

    /*  日志文件信息 */
    const char* file_full_path; /*  file utf8 full path */

    /*  可选参数 */
    uint32_t max_speed = 0; /*  限速, 单位 bytes/second, 0 表示不限制 */
};

struct zego_log_upload_res {
    uint32_t    error_code;
    uint32_t    detail_error_code;
    const char* message;
};

/*
cloud setting
*/
struct zego_cloug_setting_options
{
    uint64_t app_id;
    unsigned char* appsignature;
    uint32_t signature_size;
    const char* product;
    const char* device_id;
    const char* cache_path;
};
struct zego_cloud_setting_cache_info
{
    char* cloud_filename;
    uint32_t cloud_filename_size;
    char* json;
    uint32_t json_size;
};

struct zego_cloud_setting_cache_config
{
    uint32_t info_count;
    struct zego_cloud_setting_cache_info* infos;
};

struct zego_cloud_setting_config
{
    uint64_t code = 0;
    const char* message;
    const char* cloud_filename;
    const char* json;
    const char* versions;
    uint32_t g_version;
};

struct zego_cloud_setting_config_result
{
    uint64_t code;
    const char* message;  
    uint32_t config_count;
    struct zego_cloud_setting_config* configs;
};

#endif