
#ifndef __CS_RSA_H__
#define __CS_RSA_H__

#ifdef __cplusplus
extern "C" {
#endif


//#define RSA_MIN_MODULUS_BITS 508
#define RSA_MAX_MODULUS_BITS 1024
#define RSA_MAX_MODULUS_LEN ((RSA_MAX_MODULUS_BITS + 7) / 8)
#define RSA_MAX_PRIME_BITS ((RSA_MAX_MODULUS_BITS + 1) / 2)
#define RSA_MAX_PRIME_LEN ((RSA_MAX_PRIME_BITS + 7) / 8)


/* RSA public and private key.
 */
typedef struct {
  unsigned int bits;                           /* length in bits of modulus */
  unsigned char modulus[RSA_MAX_MODULUS_LEN];                    /* modulus */
  unsigned char exponent[RSA_MAX_MODULUS_LEN];           /* public exponent */
} CSRSA_PUBLIC_KEY;

typedef struct {
  unsigned int bits;                           /* length in bits of modulus */
  unsigned char modulus[RSA_MAX_MODULUS_LEN];                    /* modulus */
  unsigned char publicExponent[RSA_MAX_MODULUS_LEN];     /* public exponent */
  unsigned char exponent[RSA_MAX_MODULUS_LEN];          /* private exponent */
  unsigned char prime[2][RSA_MAX_PRIME_LEN];               /* prime factors */
  unsigned char primeExponent[2][RSA_MAX_PRIME_LEN];   /* exponents for CRT */
  unsigned char coefficient[RSA_MAX_PRIME_LEN];          /* CRT coefficient */
} CSRSA_PRIVATE_KEY;


/**
@brief 使用RSA公钥加密

@param[in] pucInput 明文
@param[in] unInputLen 明文长度
@param[in] psPublicKey RSA公钥
@param[out] pucOutput 密文
@param[out] pucOutputLen 密文长度
@return 0表示加密成功，其他为失败
@note 明文长度(bytes) <= 密钥长度(bytes)-11
	  密文长度(bytes) = 密钥长度(bytes)
	  密钥长度(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSRSAPublicEncrypt(unsigned char *pucOutput, unsigned int *pucOutputLen, unsigned char *pucInput, unsigned int unInputLen,CSRSA_PUBLIC_KEY *psPublicKey);


/**
@brief 使用RSA公钥解密

@param[in] pucInput 密文
@param[in] unInputLen 密文长度
@param[in] psPublicKey RSA公钥
@param[out] pucOutput 明文
@param[out] pucOutputLen 明文长度
@return 0表示解密成功，其他为失败
@note 明文长度(bytes) <= 密钥长度(bytes)-11
	  密文长度(bytes) = 密钥长度(bytes)
	  密钥长度(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSRSAPublicDecrypt(unsigned char *pucOutput,unsigned int *pucOutputLen,unsigned char *pucInput,unsigned int unInputLen,CSRSA_PUBLIC_KEY *psPublicKey);

/**
@brief 使用RSA私钥加密

@param[in] pucInput 明文
@param[in] unInputLen 明文长度
@param[in] psPublicKey RSA私钥
@param[out] pucOutput 密文
@param[out] pucOutputLen 密文长度
@return 0表示加密成功，其他为失败
@note 明文长度(bytes) <= 密钥长度(bytes)-11
	  密文长度(bytes) = 密钥长度(bytes)
	  密钥长度(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSRSAPrivateEncrypt(unsigned char *pucOutput,unsigned int *pucOutputLen,unsigned char *pucInput,unsigned int unInputLen,CSRSA_PRIVATE_KEY *psPrivateKey);

/**
@brief 使用RSA私钥解密

@param[in] pucInput 密文
@param[in] unInputLen 密文长度
@param[in] psPublicKey RSA私钥
@param[out] pucOutput 明文
@param[out] pucOutputLen 明文长度
@return 0表示解密成功，其他为失败
@note 明文长度(bytes) <= 密钥长度(bytes)-11
	  密文长度(bytes) = 密钥长度(bytes)
	  密钥长度(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSRSAPrivateDecrypt(unsigned char *pucOutput,unsigned int *pucOutputLen,unsigned char *pucInput,unsigned int unInputLen,CSRSA_PRIVATE_KEY *psPrivateKey);

/**
@brief 对字符串进行base64编码
!!!!!!!!!!!!!!!!!!!!!!!! 此接口废弃，请使用CSBase64Encode!!!!!!!!!!!!!!!!!!!!!!!! 
@param[in] pucSrc 需要编码的字符串
@param[in] nSrcLen 字符串pucSrc的长度
@param[out] pucDest 编码后的字符串
@return 0表示成功，其他为失败
*/
int CSEVPEncodeBlock(unsigned char *pucDest, const unsigned char *pucSrc, int nSrcLen);

/**
@brief 对字符串进行base64解码
!!!!!!!!!!!!!!!!!!!!!!!! 此接口废弃，请使用CSBase64Decode!!!!!!!!!!!!!!!!!!!!!!!! 
@param[in] pucSrc 需要解码的字符串
@param[in] nSrcLen 字符串pucSrc的长度
@param[out] pucDest 解码后的字符串
@return 0表示成功，其他为失败
*/
int CSEVPDecodeBlock(unsigned char *pucDest, const unsigned char *pucSrc, int nSrcLen);


/**
@brief 对字符串进行base64编码

@param[in] nDestLen 指针pucDest指向的空间的长度
@param[in] pucSrc 需要编码的字符串
@param[in] nSrcLen 字符串pucSrc的长度
@param[out] pucDest 编码后的字符串
@return >0表示成功,该值为编码后的数组长度;其他为失败
@note 密文长度 = (明文长度*4+2)/3
      明文长度 <= 密文长度*3/4
*/
int CSBase64Encode(unsigned char *pucDest, int nDestLen, const unsigned char *pucSrc, int nSrcLen);

/**
@brief 对字符串进行base64解码

@param[in] nDestLen 指针pucDest指向的空间的长度
@param[in] pucSrc 需要解码的字符串
@param[in] nSrcLen 字符串pucSrc的长度
@param[out] pucDest 解码后的字符串
@return >0表示成功,该值为解码码后的数组长度;其他为失败
@note 密文长度 = (明文长度*4+2)/3
      明文长度 <= 密文长度*3/4
*/
int CSBase64Decode(unsigned char *pucDest, int nDestLen, const unsigned char *pucSrc, int nSrcLen);


/**
@brief 从存储RSA私钥的PEM字符串中获取RSA私钥

@param[in] pucPEM 存储RSA私钥的PEM字符串
@param[in] nPEMLen 字符串pucPEM的长度
@param[out] psPrivateKey 解析后的私钥
@return 0表示解析成功，其他为失败
@note 密钥长度(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSPEMToRSAPrivateKey(unsigned char *pucPEM, int nPEMLen, CSRSA_PRIVATE_KEY *psPrivateKey);

/**
@brief 从存储RSA公钥的PEM字符串中获取RSA公钥

@param[in] pucPEM 存储RSA公钥的PEM字符串
@param[in] nPEMLen 字符串pucPEM的长度
@param[out] psPublicKey 解析后的公钥
@return 0表示解析成功，其他为失败
@note 密钥长度(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSPEMToRSAPublicKey(unsigned char *pucPEM, int nPEMLen, CSRSA_PUBLIC_KEY *psPublicKey);


#ifdef __cplusplus
}
#endif

#endif

