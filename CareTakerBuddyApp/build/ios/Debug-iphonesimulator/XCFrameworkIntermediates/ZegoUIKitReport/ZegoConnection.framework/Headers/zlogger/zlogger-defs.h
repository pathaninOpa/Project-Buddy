#pragma once

// stl
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace zlogger
{
enum level_enum
{
    debug    = 0,
    info     = 1,
    warn     = 2,
    err      = 3,
    critical = 4,
    off      = 5,
    n_levels
};

class ISink;
class ILogger;
class ILogbuffer;
using ILogger_ptr    = std::shared_ptr<ILogger>;
using ISink_ptr      = std::shared_ptr<ISink>;
using ILogbuffer_ptr = std::shared_ptr<ILogbuffer>;

}  // namespace zlogger

#pragma once
#ifdef _WIN32
#define ZLOGGER_ATTR_CHECK_FMT(a, b)
#include <sal.h>
#define ZLOGGER_ATTR_CHECK_FMTVAR _Printf_format_string_
#else
#define ZLOGGER_ATTR_CHECK_FMT(a, b) __attribute__((format(printf, a, b)))
#define ZLOGGER_ATTR_CHECK_FMTVAR
#endif