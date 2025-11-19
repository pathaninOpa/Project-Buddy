#pragma once

// stl
#include <stdint.h>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// inner

namespace zlogger
{
class ISink;
class ILogger;

using ILogger_ptr = std::shared_ptr<ILogger>;
using ISink_ptr   = std::shared_ptr<ISink>;
namespace factory
{
    namespace sink
    {
        std::shared_ptr<ISink> basic_file(const std::string& filename, bool truncate = false);

    }  // namespace sink

    namespace async_logger
    {
        std::shared_ptr<ILogger> basic_file(const std::string& filename, bool truncate = false);

    }  // namespace async_logger

}  // namespace factory

}  // namespace zlogger
