#pragma once

#include "zsetting_fwd.h"
#include <ZegoJson.hpp>

namespace zego
{
namespace zegosetting
{
#define CLOUD_RTC "RTC"
#define CLOUD_ZIM "ZIM"
#define CLOUD_ROOMKIT "ROOM_KIT"
#define CLOUD_DOC_SHARE "DOC_SHARE"
#define CLOUD_WHITE_BOARD "WHITE_BOARD"
#define CLOUD_GOENJOY "GOENJOY"
    /*
    * 云控新增环境变量配置
    * SDK 可指定N组环境变量，服务器返回N+1结果，默认配置，和环境变量的特有配置。
    */
    struct env_condition_value
    {
        std::string key;
        std::string value;
        bool operator==(const env_condition_value &other) const {
            return key == other.key && value == other.value;
        }

        bool operator!=(const env_condition_value &other) const { return !(*this == other); }
    };

 
    struct env_condition
    {
        std::vector<env_condition_value> env_condition_value_list; //环境变量的条件参数
        std::string condition_config;  //该环境变量下服务的配置，set_env_list 请求时该参数为空

        bool operator==(const env_condition &other) const 
        {
            ZegoJson src_json(condition_config);
            if (!src_json.IsObject())
            {
                assert(false);
                return false;
            }

            ZegoJson other_src_json(other.condition_config);
            if (!other_src_json.IsObject())
            {
                assert(false);
                return false;
            }
            
            return env_condition_value_list == other.env_condition_value_list &&
                   ((nlohmann::json)src_json == (nlohmann::json)other_src_json);
        }

        bool operator!=(const env_condition &other) const { return !(*this == other); }
    };

    struct cloud_env
    {
        std::string env_name; //环境变量名名称，例如gpu 信息， 后续可自定定义扩展
        std::vector<env_condition> env_condition_list; // 一个env_condition 为一个环境变量
        bool operator==(const cloud_env &other) const {
            return env_name == other.env_name && env_condition_list == other.env_condition_list;
        }

        bool operator!=(const cloud_env &other) const { return !(*this == other); }
    };
    
    struct cloud_local_config_data {
        std::string default_json_config; //默认的json 配置，与之前返回的json 一致
        std::vector<cloud_env> cloud_env_config;
    };

    class ISetting
    {
      public:
        static ISetting_ptr create();
        virtual ~ISetting() = default;

        // 初始化接口
        virtual int32_t init(const init_options_t& options)   = 0;
        virtual void    uninit()                              = 0;
        virtual void    stop_polling()                        = 0;
        virtual void    start_polling()                       = 0;
        virtual void    set_token(const std::string& token)   = 0;
        virtual void    set_userid(const std::string& userid) = 0;
        

        // 环境变量接口 (如果同一个key设置了多次，后向前覆盖)
        virtual void set_request_kv(const std::string& key, int32_t value)            = 0;
        virtual void set_request_kv(const std::string& key, uint32_t value)           = 0;
        virtual void set_request_kv(const std::string& key, const std::string& value) = 0;
        virtual void set_request_kv(const std::string& key, float value)              = 0;

        //设置云控环境变量
        virtual void set_env_list(const std::vector<cloud_env> &cloud_env_list) = 0;

        // 以上四个接口 内部转为string存储(后台协议统一使用string类型请求)
        virtual std::map<std::string, std::string> get_request_kv() = 0;

        virtual std::map<std::string, std::string> local_cache()        = 0;
        virtual std::map<std::string, std::string> local_versions()     = 0;
        virtual std::map<std::string, uint32_t> local_number_versions() = 0;

        //载入本地缓存，需要先通过set_env_list 设置环境后，才能获取到 对应的环境变量配置信息
        virtual std::map<std::string, cloud_local_config_data> local_cache_v2() = 0;

        virtual void fetch_bypass(const std::vector<std::string>& cloud_filenames, fetch_callback_t fetch_callback, bool need_polling, std::string reason = "timer loop") = 0;
    };

    struct init_options_t {
        // 基本配置
        uint32_t    appid;       // appid
        std::string appsign;     // appsign
        std::string product;     // 产品名称
        std::string deviceid;    // 设备ID
        std::string cache_path;  // 缓存路径

        polling_notify_t  polling_notify{nullptr};   // 轮询回调

        // 是否使用云控测试环境 (客户的mode为test时 云控也使用test 与柔性配置的两个环境配置对齐)
        bool use_test_env = false;
    };

    struct fetch_result_t {
        struct item_t {
            int64_t     code           = 0;
            std::string message        = "";
            std::string cloud_filename = "";
            std::string json           = "";
            std::string versions       = "";
            uint32_t    g_versions     = 0;
            std::string hash_data_hex_string;
            std::vector<cloud_env> response_cloud_env_list;
        };
        int64_t                             code    = 0;
        std::string                         message = "";
        std::vector<fetch_result_t::item_t> items{};
        std::shared_ptr<struct ::ZEGO::CONNECTION::HttpContext> detail{nullptr};  // 数据上报使用
        std::string url;
    };

}  // namespace zegosetting
}  // namespace zego

// 1. 存储
//      按product、appid、文件分开存储   cache_path/product/appid/dyamic/file_hash - v
//                                     cache_path/product/appid/static/file_hash - v
// 2. 缓存
//      1. 内部维护一个 file_list 每次轮询会拉取所有的file , 与对应的缓存对比，若有差异，通过polling_notify_t回调给上层
//      2. 内部的file_list会维护 "上次拉取成功时间"，用户主动拉取时，若间隔时间小于 polling_freq 则时间返回缓存
//      3. 需要后台支持：每次轮询事件，会拉取所有文件，此时希望后台能够支持批量拉取，否则10个文件要轮询50分钟，无法接受
