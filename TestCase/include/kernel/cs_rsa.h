
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
@brief ʹ��RSA��Կ����

@param[in] pucInput ����
@param[in] unInputLen ���ĳ���
@param[in] psPublicKey RSA��Կ
@param[out] pucOutput ����
@param[out] pucOutputLen ���ĳ���
@return 0��ʾ���ܳɹ�������Ϊʧ��
@note ���ĳ���(bytes) <= ��Կ����(bytes)-11
	  ���ĳ���(bytes) = ��Կ����(bytes)
	  ��Կ����(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSRSAPublicEncrypt(unsigned char *pucOutput, unsigned int *pucOutputLen, unsigned char *pucInput, unsigned int unInputLen,CSRSA_PUBLIC_KEY *psPublicKey);


/**
@brief ʹ��RSA��Կ����

@param[in] pucInput ����
@param[in] unInputLen ���ĳ���
@param[in] psPublicKey RSA��Կ
@param[out] pucOutput ����
@param[out] pucOutputLen ���ĳ���
@return 0��ʾ���ܳɹ�������Ϊʧ��
@note ���ĳ���(bytes) <= ��Կ����(bytes)-11
	  ���ĳ���(bytes) = ��Կ����(bytes)
	  ��Կ����(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSRSAPublicDecrypt(unsigned char *pucOutput,unsigned int *pucOutputLen,unsigned char *pucInput,unsigned int unInputLen,CSRSA_PUBLIC_KEY *psPublicKey);

/**
@brief ʹ��RSA˽Կ����

@param[in] pucInput ����
@param[in] unInputLen ���ĳ���
@param[in] psPublicKey RSA˽Կ
@param[out] pucOutput ����
@param[out] pucOutputLen ���ĳ���
@return 0��ʾ���ܳɹ�������Ϊʧ��
@note ���ĳ���(bytes) <= ��Կ����(bytes)-11
	  ���ĳ���(bytes) = ��Կ����(bytes)
	  ��Կ����(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSRSAPrivateEncrypt(unsigned char *pucOutput,unsigned int *pucOutputLen,unsigned char *pucInput,unsigned int unInputLen,CSRSA_PRIVATE_KEY *psPrivateKey);

/**
@brief ʹ��RSA˽Կ����

@param[in] pucInput ����
@param[in] unInputLen ���ĳ���
@param[in] psPublicKey RSA˽Կ
@param[out] pucOutput ����
@param[out] pucOutputLen ���ĳ���
@return 0��ʾ���ܳɹ�������Ϊʧ��
@note ���ĳ���(bytes) <= ��Կ����(bytes)-11
	  ���ĳ���(bytes) = ��Կ����(bytes)
	  ��Կ����(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSRSAPrivateDecrypt(unsigned char *pucOutput,unsigned int *pucOutputLen,unsigned char *pucInput,unsigned int unInputLen,CSRSA_PRIVATE_KEY *psPrivateKey);

/**
@brief ���ַ�������base64����
!!!!!!!!!!!!!!!!!!!!!!!! �˽ӿڷ�������ʹ��CSBase64Encode!!!!!!!!!!!!!!!!!!!!!!!! 
@param[in] pucSrc ��Ҫ������ַ���
@param[in] nSrcLen �ַ���pucSrc�ĳ���
@param[out] pucDest �������ַ���
@return 0��ʾ�ɹ�������Ϊʧ��
*/
int CSEVPEncodeBlock(unsigned char *pucDest, const unsigned char *pucSrc, int nSrcLen);

/**
@brief ���ַ�������base64����
!!!!!!!!!!!!!!!!!!!!!!!! �˽ӿڷ�������ʹ��CSBase64Decode!!!!!!!!!!!!!!!!!!!!!!!! 
@param[in] pucSrc ��Ҫ������ַ���
@param[in] nSrcLen �ַ���pucSrc�ĳ���
@param[out] pucDest �������ַ���
@return 0��ʾ�ɹ�������Ϊʧ��
*/
int CSEVPDecodeBlock(unsigned char *pucDest, const unsigned char *pucSrc, int nSrcLen);


/**
@brief ���ַ�������base64����

@param[in] nDestLen ָ��pucDestָ��Ŀռ�ĳ���
@param[in] pucSrc ��Ҫ������ַ���
@param[in] nSrcLen �ַ���pucSrc�ĳ���
@param[out] pucDest �������ַ���
@return >0��ʾ�ɹ�,��ֵΪ���������鳤��;����Ϊʧ��
@note ���ĳ��� = (���ĳ���*4+2)/3
      ���ĳ��� <= ���ĳ���*3/4
*/
int CSBase64Encode(unsigned char *pucDest, int nDestLen, const unsigned char *pucSrc, int nSrcLen);

/**
@brief ���ַ�������base64����

@param[in] nDestLen ָ��pucDestָ��Ŀռ�ĳ���
@param[in] pucSrc ��Ҫ������ַ���
@param[in] nSrcLen �ַ���pucSrc�ĳ���
@param[out] pucDest �������ַ���
@return >0��ʾ�ɹ�,��ֵΪ�����������鳤��;����Ϊʧ��
@note ���ĳ��� = (���ĳ���*4+2)/3
      ���ĳ��� <= ���ĳ���*3/4
*/
int CSBase64Decode(unsigned char *pucDest, int nDestLen, const unsigned char *pucSrc, int nSrcLen);


/**
@brief �Ӵ洢RSA˽Կ��PEM�ַ����л�ȡRSA˽Կ

@param[in] pucPEM �洢RSA˽Կ��PEM�ַ���
@param[in] nPEMLen �ַ���pucPEM�ĳ���
@param[out] psPrivateKey �������˽Կ
@return 0��ʾ�����ɹ�������Ϊʧ��
@note ��Կ����(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSPEMToRSAPrivateKey(unsigned char *pucPEM, int nPEMLen, CSRSA_PRIVATE_KEY *psPrivateKey);

/**
@brief �Ӵ洢RSA��Կ��PEM�ַ����л�ȡRSA��Կ

@param[in] pucPEM �洢RSA��Կ��PEM�ַ���
@param[in] nPEMLen �ַ���pucPEM�ĳ���
@param[out] psPublicKey ������Ĺ�Կ
@return 0��ʾ�����ɹ�������Ϊʧ��
@note ��Կ����(bytes) <= RSA_MAX_MODULUS_LEN
*/
int CSPEMToRSAPublicKey(unsigned char *pucPEM, int nPEMLen, CSRSA_PUBLIC_KEY *psPublicKey);


#ifdef __cplusplus
}
#endif

#endif

