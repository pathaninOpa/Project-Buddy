#ifndef LIBQUIC_H_
#define LIBQUIC_H_

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdint.h>
    
/** Client Usage
 
 libquic_init(); // global call once
 quic_set_log_level(kLogLevelInfo);
 
 QuicClientConfig config;
 memset(&config, 0, sizeof(config));
 config.bEncrypt = 0;
 config.msg_queue_size = 1024;
 config.ring_buffer_size = 1024 * 1024 * 4; //4MB
 
 quic_ctx* ctx = quic_ctx_create(&config);

 quic_set_event_callback(ctx_, connect_event_cb, stream_event_cb, stream_recv_cb, quic_crypto_config_cb, this);
 
 quic_connect(ctx, "127.0.0.1", 8089, 0);
 
 unsigned int stread_id = 0;
 quic_stream_create(ctx, &stream_id);
 
 quic_stream_send(ctx, stream_id, "hello", 6);
 quic_stream_close(ctx, stream_id);
 
 quic_close(ctx);
 
 // never call this method in callback
 quic_ctx_destory(ctx);
 
 libquic_uninit(); // global call once
 **/

struct quic_context;
typedef struct quic_context quic_ctx;
typedef void (*quic_log_callback)(int severity, const char *msg);

#define MINI_STREAM_ID 3

enum QuicTransportVersion
{
    kQuicVersionZego =  1 << 0,
    kQuicVersion39 = 1 << 1,    // Integers and floating numbers are written in big
                                // endian. Dot not ack acks. Send a connection level
                                // WINDOW_UPDATE every 20 sent packets which do not
                                // contain retransmittable frames.

    kQuicVersion43 = 1 << 2,  // PRIORITY frames are sent by client and accepted by server.
    kQuicVersion44 = 1 << 3,  // Use IETF header format.
    kQuicVersion46 = 1 << 4,  // Use CRYPTO frames for QuicCryptoStreams.
};

enum QuicConnectionEvent
{
    kQuicConnectCreated,
    kQuicConnected,
    kQuicConnectClosed,
    kQuicConnectCreateFailed
};
    
enum QuicStreamEvent
{
    kQuicStreanCreated,
    kQuicStreamClosed
};

enum QuicLogLevel
{
    kLogLevelVerbose = -1,
    kLogLevelInfo = 0,
    kLogLevelWarning = 1,
    kLogLevelError = 2,
    kLogLevelFatal = 3
};
    
enum QuicLogMod
{
    kLogModDefault = 0,
    kLogModProcessId = 1,
    kLogModThreadId = 1 << 1,
    kLogModTimestamp = 1 << 2,
};

enum QuicRtoPolicy
{
    RTO_POLICY_AGGRESIVE = 0,
    RTO_POLICY_NORMAL,
    RTO_POLICY_AUTO,     //客户端设置, 默认选择RTO_POLICY_AGGRESIVE, 协商后以服务器下发配置为准
    RTO_POLICY_DEFAULT = RTO_POLICY_AGGRESIVE
};
typedef struct crypto_string_piece {
    const char* data;
    int length;
}CryptoStringPiece;

typedef struct quic_client_config_t
{
    unsigned char bEncrypt; // 0: disable, 1: enable, [deprecated] use transport_version
    unsigned char enable_0rtt; // 0: disable, 1: enable
    unsigned int msg_queue_size; // 0: use sys default value (1024)
    unsigned int ring_buffer_size; // 0: use sys defult value (4MB)
    unsigned int handshake_timeout_secs_; //  0: use sys defult value (10 secs)
    unsigned int idle_network_timeout_secs_; // 0: use sys defult value (600 secs)
    unsigned int enable_verify; // 0: disable, 1: enable RSA 2: enable ECC
    unsigned int enable_no_pad_hello; // 0: disable, 1: enable; default:1 for zego, standard quic need set to 0
    int rto_policy_; //default: RTO_POLICY_AGGRESIVE
    int transport_version; //@see QuicTransportVersion
    CryptoStringPiece client_id; // unique ID for  local IP change
    const char* cert_data; //local root cert data
    unsigned int hello_min_padding; //client hello padding min size
    unsigned int hello_max_padding; //client hello padding max size
    unsigned int server_hello_min_padding;  //server hello padding min size
    unsigned int server_hello_max_padding;  //server hello padding max size
}QuicClientConfig;
    
typedef struct quic_crypto_config_t
{
    CryptoStringPiece server_config;
    CryptoStringPiece chlo_hash;
    CryptoStringPiece signature;
    CryptoStringPiece source_address_token;
    CryptoStringPiece cert;
    uint64_t expire_time;
}QuicCryptoConfig;

typedef struct quic_socks5_config_t
{
    CryptoStringPiece socks5_ip;
    CryptoStringPiece socks5_usr;
    CryptoStringPiece socks5_psd;
    unsigned short socks5_port;
    unsigned short disable_socks5_udp_ip;
}QuicSocks5Config;

typedef struct quic_cloud_agent_config_t
{
    CryptoStringPiece socks5_ip;
    CryptoStringPiece cloud_agent_data;
    unsigned short socks5_port;
}QuicCloudAgentConfig;
    
typedef void (*quic_crypto_config_cb)(quic_ctx* ctx, QuicCryptoConfig* config, const char* server_id, void* user_data);
typedef void (*quic_connect_event_cb)(quic_ctx* ctx, uint64_t conn_id, int event, int error, void* user_data);
typedef void (*quic_stream_event_cb)(quic_ctx* ctx, uint64_t conn_id, unsigned int stream_id, int event, int error, void* user_data);

typedef void (*quic_can_send_0rtt_cb)(quic_ctx* ctx, uint64_t conn_id, void* user_data);
typedef void (*quic_fail_send_0rtt_cb)(quic_ctx* ctx, uint64_t conn_id, const char* rej_reason, void* user_data);


/**
 @return 0 recv blocked, > 0 consumed bytes
 **/
typedef int (*quic_stream_recv_cb)(quic_ctx* ctx, uint64_t conn_id, unsigned int stream_id, const char* data, int len, void* user_data);
typedef void (*quic_read_timeout_cb)(quic_ctx* ctx, void* user_data);
typedef int (*quic_get_read_timeout_ms)(quic_ctx* ctx, void* user_data);

typedef void (*quic_can_write_new_data_cb)(quic_ctx* ctx, uint64_t conn_id, unsigned int stream_id, void* user_data);

typedef void (*quic_write_data_blocked_cb)(quic_ctx* ctx, uint64_t conn_id, unsigned int stream_id, void* user_data);

typedef struct quic_event_callbacks_t
{
    quic_connect_event_cb on_conn_event;
    quic_stream_event_cb on_stream_event;
    quic_stream_recv_cb on_stream_data;
    quic_crypto_config_cb on_crypto_config;
    quic_read_timeout_cb on_read_timeout;
    quic_get_read_timeout_ms get_read_timeout_ms;

    quic_can_send_0rtt_cb on_can_write_0rtt_data;
    quic_fail_send_0rtt_cb on_fail_send_0rtt_data;
    
    quic_can_write_new_data_cb on_quic_can_write_new_data;
    quic_write_data_blocked_cb on_quic_write_data_blocked;
}QuicEventCallbacks;

const char* libquic_version();
const char* libquic_buildtime();
/**
 @return 0 not support, 1 support
 */
int libquic_support_encrpto();
    
int libquic_init();
void libquic_uninit();
    
void quic_set_log_level(int level);
void quic_set_log_mod(int mod);
void quic_set_log_callback(quic_log_callback cb);

quic_ctx* quic_ctx_create(QuicClientConfig* config);
void quic_ctx_destory(quic_ctx* ctx);

const char* quic_error_string(int error);
/**
@return 0 successful, -1 failed
**/
int quic_set_event_callback(quic_ctx* ctx, quic_connect_event_cb conn_cb,  quic_stream_event_cb stream_cb, quic_stream_recv_cb recv_cb, quic_crypto_config_cb config_cb, void* user_data);

int quic_set_event_callbacks(quic_ctx* ctx, QuicEventCallbacks* callbacks, void* user_data);

/**
@return 0 successful, -1 failed
**/
int quic_connect_cloud_agent(quic_ctx* ctx, const char* host, unsigned short port, QuicCryptoConfig* crypto_config, unsigned char socks5_method, QuicCloudAgentConfig* socks5_config, const char* server_ip = 0);

/**
@return 0 successful, -1 failed
**/
int quic_connect_socks5(quic_ctx* ctx, const char* host, unsigned short port, QuicCryptoConfig* crypto_config, unsigned char socks5_method, QuicSocks5Config* socks5_config, const char* server_ip = 0);

/**
@return 0 successful, -1 failed
**/
int quic_connect(quic_ctx* ctx, const char* host, unsigned short port, QuicCryptoConfig* crypto_config, const char* server_ip = 0);

/**
 @return 0 successful, -1 failed
 **/
int quic_close(quic_ctx* ctx);
    
/**
@return
 -1: stream create fail
 > 0 stream id 
@error
 0: success
 1: connection not exsit
 2: encryption no established,
 3: over max stream,
 4: connection goaway
**/
int quic_stream_create(quic_ctx* ctx, unsigned int* error);

/**
@return 0 successful, < 0 failed, -2 buffer overflow, please retry
**/
int quic_stream_send(quic_ctx* ctx, unsigned int stream_id, const char* data, int len);

/**
 @return 0 successful, -1 failed
 **/
int quic_stream_recv_unblocked(quic_ctx* ctx, unsigned int stream_id);
    
/**
@return 0 successful, -1 failed
**/
int quic_stream_close(quic_ctx* ctx, unsigned int stream_id);


/**
get smooth rtt
@return >=0 rtt, -1 failed(not have any rtt sample)
**/
int quic_get_rtt_ms(quic_ctx* ctx);

/**
 * if you hope to override server config,  must be call this after handshake successful (kQuicConnected)
@return 0 successful, -1 failed
**/
int quic_set_timeouts(quic_ctx* ctx, unsigned int ping_timeout_secs, unsigned int idle_network_timeout_secs);

#ifdef __cplusplus
} //  extern "C" 
#endif

#endif // LIBQUIC_H_
