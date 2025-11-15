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
        std::shared_ptr<ISink> callback(std::function<void(std::string&&)> callback);

    }  // namespace sink

    namespace async_logger
    {
        std::shared_ptr<ILogger> callback(std::function<void(std::string&&)> callback);

    }  // namespace async_logger

}  // namespace factory

}  // namespace zlogger
