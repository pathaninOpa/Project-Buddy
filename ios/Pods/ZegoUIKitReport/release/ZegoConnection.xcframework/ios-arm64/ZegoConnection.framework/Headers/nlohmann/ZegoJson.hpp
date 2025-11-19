#ifndef NLOHMANN_ZEGO_JSON_HPP
#define NLOHMANN_ZEGO_JSON_HPP

#include "nlohmann/json.hpp"

#include <assert.h>
#include <string>

#ifdef WIN32
#undef GetObject
#endif

/*
Nlohmann json helper
It is forbidden to directly call [parse], [dump] in <nlohmann/json.hpp>
*/
class ZegoJson {
  public:
    ZegoJson() {}
    ZegoJson(const std::string &content);
    ZegoJson(const char *content);
    ZegoJson(const nlohmann::json &json);
    ~ZegoJson();

    // Json to string
    std::string Dump();

    operator nlohmann::json();

    // Write
    nlohmann::json &operator[](const char *key);

    bool IsObject() const;
    bool Contains(const char *key) const;
    void Remove(const char *key);

    // Read, it will return json_ when key is nullptr
    bool GetBool(const char *key = nullptr) const;
    int GetInt32(const char *key = nullptr) const;
    unsigned int GetUint32(const char *key = nullptr) const;
    long long GetInt64(const char *key = nullptr) const;
    unsigned long long GetUint64(const char *key = nullptr) const;
    float GetFloat(const char *key = nullptr) const;
    double GetDouble(const char *key = nullptr) const;
    std::string GetString(const char *key = nullptr) const;

    ZegoJson GetObject(const char *key) const;
    ZegoJson GetArrayItem(unsigned int index) const;

    ZegoJson GetArray(const char *key) const;
    void AppendToArray(const ZegoJson &zego_json);

    bool IsArray() const;
    // Get array size, json_ is array
    unsigned int GetSize() const;

  private:
    nlohmann::json json_;
    nlohmann::json null_json_;
};

#endif // NLOHMANN_ZEGO_JSON_HPP
