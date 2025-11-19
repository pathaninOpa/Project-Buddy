#pragma once

#include <stdint.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ZEGO
{
namespace CONNECTION
{
    struct HttpContext;
}
}  // namespace ZEGO

namespace zego
{
namespace zegosetting
{
    struct init_options_t;
    struct fetch_result_t;
    class ISetting;
    using ISetting_ptr     = std::shared_ptr<ISetting>;
    using fetch_callback_t = std::function<void(const fetch_result_t& results)>;
    using polling_notify_t = std::function<void(const fetch_result_t& changed_results, const fetch_result_t& all_results)>;
    // changed_results 变更的配置列表
    // all_results     所有的配置列表(包含前者)

}  // namespace zegosetting
}  // namespace zego
