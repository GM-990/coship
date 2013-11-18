/**@defgroup  提供第三方加密处理
@brief 对应头文件<Csthird_secure.h>，定义了所有第三方注册的回调和设置接口
-对所有的第三方加密相关的实现都有效
*/

#ifndef _CS_THIRD_SECURE_H
#define _CS_THIRD_SECURE_H
#endif
#include "udiplus_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSTHIRD_CERT_LEN		2560			// 证书字符串的长度
#define CSTHIRD_CERT_NULL		1<<31			// 没有找到证书的错误码
#define CSTHIRD_PRIVATEKEY_LEN	2560			// 私钥长度
#define CSTHIRD_CERT_NAME_LEN	20				// 第三方加密的名称长度

/**@brief 终端信息证书和密钥类型*/
typedef enum _CSThird_INFO_E
{
	EM_THIRD_CERT_LOCAL = 0x2000,					// 加载本地客户端证书
	EM_THIRD_CERT_PRIVATEKEY,						// 加载客户端的私钥
	EM_THIRD_CERT_ROOT_CA = 0x1000,					// 加载客户端的根证书
	EM_THIRD_CERT_SECOND_CA = 0x1001,				// 加载客户端的二级CA证书
	EM_THIRD_CERT_THIRD_CA,							// 加载客户端的三级CA证书
	EM_HIRD_CERT_SERVCER = 0x3000,					// 服务器端证书
	EM_THIRD_PRIVATE_LOCAL							// 加载客户端的私钥
}CSThird_Info_E;

/**@brief 终端密钥所支持的类型*/
typedef enum _CSTHIRD_KEY_INCRYPT_E
{
	E_THIRD_PKEY_RSA =1 ,						// RSA私钥
	E_THIRD_PKEY_DSA,							// DSA私钥
	E_THIRD_PKEY_EC								// EC私钥
}CSthird_Key_Incrypt_E;

/**@brief 读取证书或密钥的类型:文件还是内存形势*/
typedef enum _CSTHIRD_CERT_KEY_E
{
	E_CERT_FILE_TYPE =1,							// 文件类型证书
	E_CERT_MEM_TYPE									// 内存类型证书
}CSThird_Cert_Key_E;


/**@brief 文件证书或文件密钥的类型为以下两种*/
typedef enum _CSTHIRD_CE_FILE_E
{
	E_CERT_FILETYPE_ASN1 =1,						// asn1的文件类型
	E_CERT_FILETYPE_PEM								// Pem的文件类型
}CSThird_File_E;

/**
@brief 获取证书的回调
@param[in] unsigned short usCertType 获取证书的类型，具体见 CSThird_Info_E
@param[in|out] unsigned char *pcCert 存放证书的buffer
@param[in|out] int *pnLen 获得证书的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_Cert)(unsigned short usCertType, unsigned char *pcCert, int *pnLen);

/**
@brief 获取私钥的回调
@param[in] unsigned short usCertType 获取私钥的类型，具体见 CSThird_Info_E
@param[in|out] unsigned char *pcKey 存放私钥的buffer
@param[in|out] int *pnLen 获得私钥的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_PrivateKey)(unsigned short usCertType, unsigned char *pcKey, int *pnLen);

/**
@brief 获取公钥的回调
@param[in] unsigned short usCertType 获取公钥的类型，具体见 CSThird_Info_E
@param[in|out] unsigned char *pcKey 存放私钥的buffer
@param[in|out] int *pnLen 获得公钥的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_PublicKey)(unsigned short usCertType, unsigned char *pcKey, int *pnLen);

/**
@brief 使用RSA公钥验签的回调
@param[in] unsigned char ucDigisteAlgID 签名算法，具体由第三方定义其意义
@param[in] const unsigned char *szPublicKey 输入公钥
@param[in] int nKeyLen 公钥的长度
@param[in] const unsigned char *pcInData 验签的数据
@param[in] int nInLen 验签前数据的长度
@param[in|out] const unsigned char *pcSignData 验签后的数据
@param[in] nSignLen 验签后数据的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_RsaVerify)(unsigned char ucDigisteAlgID, const unsigned char *szPublicKey, int nKeyLen, 
	const unsigned char *pcInData, int nInLen, const unsigned char *pcSignData, int nSignLen);

/**
@brief 使用RSA公钥签名的回调
@param[in] unsigned char ucDigisteAlgID 签名算法，具体由第三方定义其意义
@param[in] const unsigned char *pcPrivateKey 输入私钥
@param[in] int nKeyLen 私钥的长度
@param[in] const unsigned char *pcInData 要签名的数据
@param[in] int nInLen 签名前数据的长度
@param[in|out] const unsigned char *pcOutData 签名后的数据
@param[in] nSignLen 签名后数据的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_RsaSign)(unsigned char ucDigisteAlgID, const unsigned char *pcPrivateKey, int nKeyLen, 
	const unsigned char*pcInData, int nInLen, const unsigned char *pcOutData, int *nOutLen);

/**
@brief 获得随机数的回调
@param[in|out] unsigned char *pcRandom 获得随机数的buffer
@param[in] int nLen 随机数的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_GetRandom)(unsigned char *pcRandom, int nLen);

/**
@brief 使用RSA算法解密
@param[in] const unsigned char *pzPrivateKey 输入的私钥
@param[in] int nKeyLen 私钥的长度
@param[in] unsigned char *pcInData 要解密的数据
@param[in] int nInLen 要解密数据的长度
@param[in|out] unsigned char *pcOutData 解密后的数据
@param[in] int *pnOutLen 解密后数据的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_RsaDecrypt)(const unsigned char *pzPrivateKey, int nKeyLen, unsigned char *pcIndata, 
	int nInLen, unsigned char *pcOutData, int *pnOutLen);

/**
@brief 使用RSA算法加密
@param[in] const unsigned char *pzPublicKey 输入的公钥
@param[in] int nKeyLen 公钥的长度
@param[in] unsigned char *pcInData 要加密的数据
@param[in] int nInLen 要加密数据的长度
@param[in|out] unsigned char *pcOutData 加密后的数据
@param[in] int *pnOutLen 加密后数据的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_RsaEncrypt)(const unsigned char *pzPublicKey, int nKeyLen, unsigned char *pcIndata, 
	int nInLen, unsigned char *pcOutData, int *pnOutLen);

/**
@brief 产生RSA密钥对
@param[in] unsigned char ucType 产生密钥对的类型，具体为生成密钥的长度
@param[in|out] unsigned char *pzPublicKey 获得公钥的buffer
@param[in|out] int* nKeyLen 公钥的长度
@param[in|out] unsigned char *pcPrivateKey 存放私钥的buffer
@param[in|out] int *pnPrivateKeyLen 私钥的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_GenRsaKey)(unsigned char ucType, char *pcPublicKey, int* pnPublicKeyLen, 
	unsigned char *pcPrivateKey, int *pnPrivateKeyLen);

/**
@brief 使用私钥对数据加密
@param[in] const char *pcInData 要加密的数据
@param[in] int nInLen 要加密的数据长度
@param[in|out] unsigned char *pcOutData 存放加密后的数据
@param[in|out] int *pnOutLen 加密后数据的长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_SecureDigest)(const char *pcInData, int nInLen, unsigned char *pcOutData, int *pnOutLen);

/**
@brief 使用私钥对数据签名
@param[in] unsigned short usCertType 签名算法
@param[in] const unsigned char *pcInData 要签名的数据
@param[in] int nInLen 要签名数据的长度
@param[in|out] unsigned char *pcSignDat 签名后的数据
@param[in|out] int *nSignLen 签名后的数据长度
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int(*CSThird_SecureSign)(unsigned short usCertType, const unsigned char *pcInData, 
	int nInLen, unsigned char *pcSignData, int *nSignLen);

/**
@brief 从第三方文件中读出证书
@param[in|out] void *x509 存放x509证书的buffer
@param[in] char *pCertPath 证书文件的路径
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int (*CSThird_Cert_File)(void *x509, char *pCertPath);

/**
@brief 从第三方文件中读出私钥
@param[in|out] void *pKey 存放私钥的buffer
@param[in] char *pCertPath 私钥文件的路径
@return 成功返回0；失败返回具体的错误号
@note 返回的错误号目前由第三方定义
*/
typedef int (*CSThird_PriKey_File)(void *pKey, char *pPriKeyPath);

/**@brief 加密套件注册回调的结构体*/
typedef struct _Third_SECURE_EVP
{
	char					acSecureName[CSTHIRD_CERT_NAME_LEN];
	BOOL 					bCheckExtentions;		
	CSThird_Cert			pCert;									 
	CSThird_Cert_File		pFileCert;
	CSThird_PrivateKey		pPrivateKey;
	CSThird_PriKey_File		pFilePriKey;
	CSThird_PublicKey		pPublicKey;
	CSThird_RsaVerify		pRsaVerify;
	CSThird_RsaSign			pRsaSign;
	CSThird_GetRandom		pRsaRandom;
	CSThird_RsaDecrypt		pRsaDecrypt;
	CSThird_RsaEncrypt		pRsaEncrypt;
	CSThird_GenRsaKey		pGenRsaKey;
	CSThird_SecureDigest	pSecureDigest;
	CSThird_SecureSign		pSecureSign;
}CSThird_Secure_EVP;


/**
@brief 注册第三方加密的信息

@param[in] CSThird_Secure_EVP *sSSLEvp   RtspClient_S结构
@return 如果成功，返回1，失败返回0
@note 无
*/	
int CSRegisterThirdSecureEvp(CSThird_Secure_EVP *sSSLEvp);

/**
@brief 删除第三方加密的信息

@param[in] pName 第三防产商的名称
@return 如果成功，返回1，失败返回-1
@note 无
*/	
int CSDeleteThirdSecureEvp(char *pName);

/**
@brief设置证书参数

@param[in] CSThird_Info_E eInfoOpt 证书类型
@param[in] CSThird_Cert_Key_E eCertOpt 证书文件类型，包括文件密钥，内存私钥
@param[in] CSThird_File_E eFileOpt 文件证书的类型，包括ASN1,PEM
@param[in] char * pFilePath 如果是文件证书，设置文件的地址(只有当eFileOpt文件密钥是才有效)
@param[in] char* pProName 第三方厂商的名称
@return 如果成功，返回1，失败返回0
@note 无
*/
int CSThird_Secure_SetCertOpt(CSThird_Info_E eInfoOpt, CSThird_Cert_Key_E eCertOpt, 
	CSThird_File_E eFileOpt, char *pFilePath, char* pProName);
/*
@brief设置密钥参数

@param[in] CSThird_Cert_Key_E eKeyOpt 私钥文件类型，包括文件密钥，内存私钥
@param[in] CSThird_File_E eFileOpt 文件密钥的类型，包括ASN1,PEM
@param[in] CSthird_Key_Incrypt_E  eInopt 密钥的形式，RSA, DSA, EC
@param[in] char * pFilePath 如果是文件密钥，设置文件密钥的地址(只有当eFileOpt文件密钥是才有效)
@param[in] char* pProName第三方厂商的名称
@return 如果成功，返回1，失败返回0
@note 无
*/
int CSThird_Secure_SetKeyOpt(CSThird_Cert_Key_E eKeyOpt, CSThird_File_E eFileOpt, 
							 CSthird_Key_Incrypt_E eInopt,
							 char *pFilePath, char* pProName);

#ifdef  __cplusplus
}
#endif
