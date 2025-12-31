/** 
@file 
@brief		加解密. 实现下列算法: Hash算法: MD5,已实现. 对称算法: DES,未实现. 非对称算法: RSA,未实现
*/

#pragma once

#include "zegotypes.h"

#define MD5_DIGEST_LENGTH	16
#define MD5_LBLOCK			16

namespace zego {
    

// MD5数据结构
typedef struct MD5state_st
	{
	uint32 A,B,C,D;
	uint32 Nl,Nh;
	uint32 data[MD5_LBLOCK];
	int32 num;
	} MD5_CTX;


_ZEGO_API void MD5_Init(MD5_CTX *c);
_ZEGO_API void MD5_Update(MD5_CTX *c, const uint8 *data, uint32 len);
_ZEGO_API void MD5_Final(uint8 *md, MD5_CTX *c);

/**@ingroup ov_Crypt
@{
*/
/// MD5 Hash函数
/** @param outBuffer out, Hash后的Buffer, 该Buffer的长度固定为MD5_DIGEST_LENGTH(16uint8)
	@param inBuffer in, 原始buffer.
	@param length in, 原始buffer的长度, 接受长度为0的buffer
*/
_ZEGO_API void Md5HashBuffer( uint8 *outBuffer, const void *inBuffer, uint32 length);


}
