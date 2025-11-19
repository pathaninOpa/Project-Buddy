#pragma once

#include <stdarg.h>
#include "zlogger-defs.h"

namespace zlogger
{
/**
 * @brief    格式化方法 c-style format
 *
 * @param    fmt  如 zlogger_fmt("example %s %d",string.c_str(),int_var);
 */
std::string zlogger_fmt(ZLOGGER_ATTR_CHECK_FMTVAR const char* fmt, ...) ZLOGGER_ATTR_CHECK_FMT(1, 2);  // 参数检查的说明见本文件末

/**
 * @brief    va_list型重载
 * 
 * @usage    
 *          {
 *              va_list args_fmt;
 *              va_start(args_fmt, fmt);
 *
 *              std::string ret = zlogger_fmt_va(fmt, args_fmt);
 *
 *              va_end(args_fmt);
 *              return ret;
 *          }
 */
std::string zlogger_fmt_va(const char* fmt, va_list args_fmt);

/**
 * @brief    设置全局错误回调
 *
 * @param    error_handler
 */
void zlogger_set_error_handler(std::function<void(const std::string& error_message)> error_handler);

/**
 * @brief    创建 log_buffer
 *
 * @param    buffer_size
 * @return   ILogbuffer_ptr buffer指针
 */
ILogbuffer_ptr debug_buffer_create(size_t buffer_size = 64);

/**
 * @brief    嵌入式平台 设置最大queue size
 *
 * @param    max_queue_size      最大queue size // 内部默认8192 (arm_linux为50条)
 */
void zlogger_set_max_queue(size_t max_queue_size);  // 仅嵌入式平台有效

class ILogger
{
  public:
    // set functions
    virtual void set_level(zlogger::level_enum log_level)  = 0;  // [与sink独立] 都会按level过滤一次
    virtual void set_headinfo(const std::string& headinfo) = 0;  // 日志头部信息，某些时机打印（如：每次轮换文件时）
    virtual void set_encrypt_key(const std::string& encrypt_key) = 0;  // [覆盖所有Sink] 不调用 或 传空 则不加密
    virtual void set_limitlog_cfg(int64_t limit_ms)              = 0;  // 设置限频，不调用时默认为500ms

    // 设置特殊回调
    virtual void set_callback_logfull(std::function<void(void)> on_logfull) = 0;  // 目前仅rotating_file支持

    // get functions
    virtual zlogger::level_enum level() const = 0;

    // write functions
    virtual void write(std::vector<std::string> tag, zlogger::level_enum lvl, const char* module, int line, const std::string& msg) = 0;
    virtual void write(std::vector<std::string> tag, zlogger::level_enum lvl, const char* module, int line, std::string&& msg)      = 0;
    virtual void write_raw(zlogger::level_enum lvl, const std::string& msg)                                                         = 0;
    virtual void write_raw(zlogger::level_enum lvl, std::string&& msg)                                                              = 0;
    virtual void write_limit(const std::string& limit_key, std::vector<std::string> tag, zlogger::level_enum lvl, const char* module,
      int line, const std::string& msg)
      = 0;

    // debug_buffer
    virtual void debug_buffer_dump(ILogbuffer_ptr debug_buffer, size_t count = 0) = 0;

    // flush functions
    virtual void                flush()                                 = 0;
    virtual void                flush_on(zlogger::level_enum log_level) = 0;
    virtual zlogger::level_enum flush_level() const                     = 0;

    virtual void change_file(const std::string& full_name)                 = 0;
    virtual void change_limit(std::size_t max_size, std::size_t max_files) = 0;

    /**
     * @brief    设置线程名回调
     *
     * @param    callback
     */
    virtual void set_threadname_callback(std::function<std::string(void)> callback) = 0;
  public:
    virtual ~ILogger() = default;
};
class ISink
{
  public:  // set
    virtual void set_level(zlogger::level_enum log_level)        = 0;
    virtual void set_encrypt_key(const std::string& encrypt_key) = 0;  // 不调用 或 传空 则不加密

    // 设置特殊回调
    virtual void set_callback_logfull(std::function<void(void)> callback) = 0;  // 目前仅rotating_file支持
    virtual void set_callback_rotate(const std::function<void(const std::string& filename, int max_file_count)>& callback) = 0; // 目前仅rotating_file支持

  public:  // get
    virtual zlogger::level_enum level() const = 0;

  public:
    virtual ~ISink() = default;
};

class ILogbuffer
{
  public:
    virtual void write(std::vector<std::string> tag, zlogger::level_enum lvl, const char* module, int line, const std::string& msg) = 0;
    virtual void clear()                                                                                                            = 0;

  public:
    virtual ~ILogbuffer() = default;
};

}  // namespace zlogger

/**
 * ZLOGGER_ATTR_CHECK_FMT的说明：
 *
 *  printf("%s",100);printf("%d","a string");
 *
 * 以上这种格式化错误的语句，可编译通过，但可能会带来严重问题，利用ZLOGGER_ATTR_CHECK_FMT可检查这种格式错误，会在编译时给出警告
 *
 * (msvc 需开启 /analyze)
 * (gcc clang 需开启 "-Wformat -Wno-format-security",
 * 可考虑使用以下编译参数组 使一些fmt错误编译失败)
 *
  cflags=[]
  if(is_win){
    cflags += [
      "/analyze",
    ]
  } else {
    cflags += [
      "-Wformat",
      "-Wno-format-security", # 这个参数会使zlogger_fmt("log")报错
      "-Wno-format-nonliteral", # 这个参数会使zlogger_fmt(string.c_str())报错
    ]
  }
  if (is_ios || is_mac || is_android || is_ohos) {
    cflags += [
      "-Werror=format-signedness",
      "-Werror=format-contains-nul",
      "-Werror=format-extra-args",
      "-Werror=format-zero-length",
      "-Werror=format-truncation",
      "-Werror=format-overflow",
    ]
  }
 *
 * try it by
 *     zlogger_fmt("wrong log:%d", "headinfo balabala");
 *     zlogger_fmt("wrong log:%s", 100);
 *     zlogger_fmt("wrong log:%s %d", 100);
 *     zlogger_fmt("wrong log:%s %d", 100, 100, 100);
 *
 *    92 |     zlogger_fmt("wrong log:%d", "headinfo balabala");
 *       |                            ~^   ~~~~~~~~~~~~~~~~~~~
 *       |                             |   |
 *       |                             int const char*
 *       |                            %s
 * unitest.cc:93:29: error: format ‘%s’ expects argument of type ‘char*’, but argument 2 has type ‘int’ [-Werror=format=]
 *    93 |     zlogger_fmt("wrong log:%s", 100);
 *       |                            ~^   ~~~
 *       |                             |   |
 *       |                             |   int
 *       |                             char*
 *       |                            %d
 * unitest.cc:94:29: error: format ‘%s’ expects argument of type ‘char*’, but argument 2 has type ‘int’ [-Werror=format=]
 *    94 |     zlogger_fmt("wrong log:%s %d", 100);
 *       |                            ~^      ~~~
 *       |                             |      |
 *       |                             char*  int
 *       |                            %d
 * unitest.cc:94:32: error: format ‘%d’ expects a matching ‘int’ argument [-Werror=format=]
 *    94 |     zlogger_fmt("wrong log:%s %d", 100);
 *       |                               ~^
 *       |                                |
 *       |                                int
 * unitest.cc:95:29: error: format ‘%s’ expects argument of type ‘char*’, but argument 2 has type ‘int’ [-Werror=format=]
 *    95 |     zlogger_fmt("wrong log:%s %d", 100, 100, 100);
 *       |                            ~^      ~~~
 *       |                             |      |
 *       |                             char*  int
 *       |                            %d
 * unitest.cc:95:17: error: too many arguments for format [-Werror=format-extra-args]
 *    95 |     zlogger_fmt("wrong log:%s %d", 100, 100, 100);
 *       |                 ^~~~~~~~~~~~~~~~~
 * cc1plus: some warnings being treated as errors
 */
