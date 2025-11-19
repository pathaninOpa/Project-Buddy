#ifndef ZegoMiscUtility_h
#define ZegoMiscUtility_h
#include "zego_connection_define.hpp"
#include "zego_connection_dns_define.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <zegonet.h>
#include <zegosocket.h>
#include <zegostd.h>
#include <zegostream.h>

namespace ZEGO {
namespace BASE {

uint32 ZegoGetNextSeq();
int ChangeAgentMode(int mode);

std::string GetBaseUrl(const std::string &url);

zego::strutf8 GetBaseUrl(const zego::strutf8 &url);

std::string GetUrlPort(const std::string &url);

std::string GetUrlParams(const std::string &url);
std::string GetURIAndParams(const std::string &url);

std::string ZegoGetRequestID();

std::string ZegoGetModeString(int mode);

uint64 ZegoGetNetAgentTxID();

void ConverSocks5ProxyConfig(const CONNECTION::Socks5ProxyConfig &in_config,
                             Sock5ProxyInfo &out_config);

#if defined(WIN32)
bool StringUtf8ToANSI(const std::string &utf8, std::string &ansi);

std::wstring Utf8ToUnicode(const std::string &utf8);

std::string UnicodeToUtf8(const std::wstring &unicode);

std::string UnicodeToLocal(const std::wstring &unicode);

std::wstring LocalToUnicode(const std::string &local);

bool IsStrUtf8(const char *str);

bool IsStrGBK(const char *str);
#endif

std::string CalcRequestLitesignBin(uint64_t timestamp, uint32 appid);
std::string CalcRequestSignatureBin(uint64_t timestamp, uint32 appid, const std::string &Signature);
void CalcRequestSignatureBin(time_t t, uint32 appID, zego::buffer &bufSignature,
                             zego::strutf8 &outSign);
void CalcRequestSignatureHex(time_t t, uint32 appID, zego::buffer &bufSignature,
                             zego::strutf8 &outSign);

std::string deflate_compress(const std::string &input);
std::string deflate_uncompress(const std::string &input);
std::string MakeNonceStr(size_t len);
uint64_t GetTimeFromHttpHeader(const std::string &header);

std::string AesEncrypt(const std::string &input);
std::string AesDecrypt(const std::string &input);

std::string StringListDetail(const std::vector<std::string> &data);
std::string IPListDetail(const std::vector<CONNECTION::IP2Type> &data);
std::string IPListDetailPrivacy(const std::vector<CONNECTION::IP2Type> &data);
std::string NetAgentResultDetail(const CONNECTION::ZegoNetAgentAddressInfo &netagent_address);
std::string
NameServerAddressInfoDetail(const std::vector<CONNECTION::NameServerAddressInfo> &address_list);
std::string MakePrivateString(const std::string &original_str);
std::string GetThreadName();
std::vector<std::string> SplitString(const std::string &text, const std::string &pattern);

template <typename T> std::string NumberListDetail(const std::vector<T> &data) {
    std::ostringstream oss;

    for (int i = 0; i < data.size(); i++) {
        oss << data[i];
        oss << ",";
    }
    oss << std::endl;
    return oss.str();
}

#if defined(_WIN32)
bool ConvertUtf8ToANSI(const std::string &utf8, std::string &ansi);
bool ConvertUtf8(const zego::strutf8 &src, zego::strutf8 &dst);
#endif

uint32 CalcTimeConsume(uint64 begin_time, uint64 begin_tickcount, uint64 end_time,
                       uint64 end_tickcount);

std::string GetModulePath();

uint32 MakeUint32(uint16 high, uint16 low);

uint32 Set32Low16(uint16 value, uint32 src);
uint32 Set32High16(uint16 value, uint32 src);

uint16 Get32Low16Bit(uint32 src);
uint16 Get32High16Bit(uint32 src);

uint64 MakeUint64(uint32 high, uint32 low);
uint64 Set64Low32(uint32 value, uint64 src);
uint64 Set64High32(uint32 value, uint64 src);

uint32 Get64Low32Bit(uint64 src);
uint32 Get64High32Bit(uint64 src);

} // namespace BASE
} // namespace ZEGO

#endif
