//
//  ZegoCrypto.hpp
//  zegoavkit
//
//  Created by Randy Qiu on 3/27/16.
//  Copyright © 2016 Zego. All rights reserved.
//

#ifndef ZegoCrypto_hpp
#define ZegoCrypto_hpp

#include <zegostd.h>
#include <zegotypes.h>

namespace ZEGO {
namespace BASE {
#define BLOCK_SIZE (16)

class CZegoCrypto {
  public:
    using BYTE = unsigned char;

    zego::strutf8 AESDecWithUnPadding(zego::strutf8 &encyptedText, const zego::strutf8 &key,
                                      const zego::strutf8 &iv = "");
    zego::strutf8 AESEncWithPadding(const zego::strutf8 &plainText, const zego::strutf8 &key,
                                    const zego::strutf8 &iv = "");

    std::string AESDecWithUnPadding(const std::string &encyptedText, const std::string &key,
                                    const std::string &iv = "");
    std::string AESEncWithPadding(const std::string &plainText, const std::string &key,
                                  const std::string &iv = "");

    zego::strutf8 AESDec(zego::strutf8 &encyptedText, const zego::strutf8 &key,
                         const zego::strutf8 &iv = "");
    zego::strutf8 AESEnc(const zego::strutf8 &plainText, const zego::strutf8 &key,
                         const zego::strutf8 &iv = "");

    zego::strutf8 AESDec(zego::strutf8 &encyptedText, const zego::buffer &key,
                         const zego::buffer &iv = "");
    zego::strutf8 AESEnc(const zego::strutf8 &plainText, const zego::buffer &key,
                         const zego::buffer &iv = "");

    zego::strutf8 AESEncECB(const zego::strutf8 &plainText, const zego::strutf8 &key);
    zego::strutf8 AESDecECB(zego::strutf8 &encyptedText, const zego::strutf8 &key);

  private:
    void InitKV(const zego::strutf8 &key, const zego::strutf8 &iv);
    zego::strutf8 PKCS7Padding(const zego::strutf8 &plainText);

    //新加padding attention 之前的aes 存在问题
    void PKCS7Unpadding(zego::strutf8 &plainText);
    zego::strutf8 PKCS7PaddingData(const zego::strutf8 &plainText);

  private:
    BYTE m_Key[32]; // max
    BYTE m_IV[BLOCK_SIZE];
};

std::string AESDec(const std::string &encryptedText, uint8 key[], uint32 keyLength,
                   uint8 iv[BLOCK_SIZE], uint32 paddingLength);
std::string AESEnc(const std::string &plainText, uint8 key[], uint32 keyLength,
                   uint8 iv[BLOCK_SIZE], uint32 &paddingLength);
} // namespace BASE
} // namespace ZEGO

#endif /* ZegoCrypto_hpp */
