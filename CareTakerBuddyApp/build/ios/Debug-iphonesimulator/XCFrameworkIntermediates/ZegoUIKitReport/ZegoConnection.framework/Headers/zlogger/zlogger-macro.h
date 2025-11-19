#pragma once
#include "zlogger-interface.h"

#ifndef __ZLOGGER_SIMPLE_FUNC__
#if defined(__ANDROID__) || defined(ANDROID) || defined(_OS_ANDROID_)
#define __ZLOGGER_SIMPLE_FUNC__ __FUNCTION__
#else
#define __ZLOGGER_SIMPLE_FUNC__ __func__
#endif
#endif

// #define ztags(...) ({__VA_ARGS__})

/**
 * @brief    tag日志宏  传tag时 建议统一使用ztags宏
 *
 *   由于需要支持多tag，为避免多tag参数与多fmt参数冲突的问题，需使用({})将tags包住：
 *
 *    可利用ztags宏统一以下情况的各种写法
 *
 *    // 无tag写法：
 *        zlog_info(logger, ztags(), "info %d", 100);
 *    // 单tag写法：
 *        zlog_info(logger, ztags("tag1"), "info %d", 100);
 *    // 多tag写法：
 *        zlog_info(logger, ztags("tag1", "tag2"), "info %d", 100);
 *
 *
 * 错误写法：
 *   zlog_info(logger, {"tag1", "tag2"}, "info %d",100);
 *   zlog_info(logger, "tag1", "tag2", "info %d",100);
 *
 * 错误说明见宏定义下方注释
 *
 */
#define zlog_debug(logger, tag, fmt, ...)    zlog(logger, tag, zlogger::level_enum::debug, fmt, ##__VA_ARGS__)
#define zlog_info(logger, tag, fmt, ...)     zlog(logger, tag, zlogger::level_enum::info, fmt, ##__VA_ARGS__)
#define zlog_warn(logger, tag, fmt, ...)     zlog(logger, tag, zlogger::level_enum::warn, fmt, ##__VA_ARGS__)
#define zlog_error(logger, tag, fmt, ...)    zlog(logger, tag, zlogger::level_enum::err, fmt, ##__VA_ARGS__)
#define zlog_critical(logger, tag, fmt, ...) zlog(logger, tag, zlogger::level_enum::critical, fmt, ##__VA_ARGS__)

/**
 * 错误示例：
 *   zlog_info(logger, {"tag1", "tag2"}, "info %d",100);
 *   以上写法会编译报错，因为宏展开时，将 {"tag1", "tag2"} 视为两个参数，导致传参错误
 *   展开为:zlogger_write_to(logger, std::vector<std::string> {"tag1", zlogger::level_enum::info, __MODULE__, 87,
 zlogger_fmt("tag2"}, "info %d",100));
 *
 *   zlog_info(logger, "tag1", "tag2", "info %d",100);
 *   以上写法也会编译报错，因为无法默认将"tag1"转换为std::vector<string>
 *   展开为zlogger_write_to(logger, std::vector<std::string> "tag1", zlogger::level_enum::info, __MODULE__, 88, zlogger_fmt("tag2",
 "info %d",100));
 *
 *
 */

/**
 * @brief    you need define __MODULE__ when use there macros
 *
 */
#define zlog(logger, tag, lvl, fmt, ...) zlogger_write_to(logger, tag, lvl, __MODULE__, __LINE__, zlogger::zlogger_fmt(fmt, ##__VA_ARGS__))

void zlogger_write_to(std::shared_ptr<zlogger::ILogger> logger, std::vector<std::string> tag, zlogger::level_enum lvl, const char* module,
  int line, const std::string&& log);

void zlogger_write_to(std::shared_ptr<zlogger::ILogbuffer> logbuffer, std::vector<std::string> tag, zlogger::level_enum lvl,
  const char* module, int line, const std::string&& log);

/**
 * @brief    按key分类限频日志宏   用法同普通日志宏 多了限频key参数
 *
 *
 *    // 无tag写法：
 *        zlog_lmt_info(logger, "limit_key",ztags(), "info %d", 100);
 *    // 单tag写法：
 *        zlog_lmt_info(logger, "limit_key",ztags("tag1"), "info %d", 100);
 *    // 多tag写法：
 *        zlog_lmt_info(logger, "limit_key",ztags("tag1", "tag2"), "info %d", 100);
 *
 *
 *  可通过 logger->set_limitlog_cfg(500);来设置限频，默认为500ms
 *
 */
#define zlog_lmt_debug(logger, limit_key, tag, fmt, ...) zlog_lmt(logger, limit_key, tag, zlogger::level_enum::debug, fmt, ##__VA_ARGS__)
#define zlog_lmt_info(logger, limit_key, tag, fmt, ...)  zlog_lmt(logger, limit_key, tag, zlogger::level_enum::info, fmt, ##__VA_ARGS__)
#define zlog_lmt_warn(logger, limit_key, tag, fmt, ...)  zlog_lmt(logger, limit_key, tag, zlogger::level_enum::warn, fmt, ##__VA_ARGS__)
#define zlog_lmt_error(logger, limit_key, tag, fmt, ...) zlog_lmt(logger, limit_key, tag, zlogger::level_enum::err, fmt, ##__VA_ARGS__)
#define zlog_lmt_critical(logger, limit_key, tag, fmt, ...) \
    zlog_lmt(logger, limit_key, tag, zlogger::level_enum::critical, fmt, ##__VA_ARGS__)
#define zlog_lmt(logger, limit_key, tag, lvl, fmt, ...) \
    zlogger_write_to_lmt(logger, limit_key, tag, lvl, __MODULE__, __LINE__, zlogger::zlogger_fmt(fmt, ##__VA_ARGS__))

void zlogger_write_to_lmt(std::shared_ptr<zlogger::ILogger> logger, const std::string& limit_key, std::vector<std::string> tag,
  zlogger::level_enum lvl, const char* module, int line, const std::string&& log);

std::vector<std::string> ztags();
std::vector<std::string> ztags(const char* tag0);
std::vector<std::string> ztags(const char* tag0, const char* tag1);
std::vector<std::string> ztags(const char* tag0, const char* tag1, const char* tag2);
std::vector<std::string> ztags(const char* tag0, const char* tag1, const char* tag2, const char* tag3);
std::vector<std::string> ztags(const char* tag0, const char* tag1, const char* tag2, const char* tag3, const char* tag4);
std::vector<std::string> ztags(const char* tag0, const char* tag1, const char* tag2, const char* tag3, const char* tag4, const char* tag5);
std::vector<std::string> ztags(const char* tag0, const std::string& tag1);
std::vector<std::string> ztags(const char* tag0, uint32_t tag_seq);
std::vector<std::string> ztags(const char* tag0, uint32_t tag1, uint32_t tag2);
