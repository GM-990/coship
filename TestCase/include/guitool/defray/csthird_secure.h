/**@defgroup  �ṩ���������ܴ���
@brief ��Ӧͷ�ļ�<Csthird_secure.h>�����������е�����ע��Ļص������ýӿ�
-�����еĵ�����������ص�ʵ�ֶ���Ч
*/

#ifndef _CS_THIRD_SECURE_H
#define _CS_THIRD_SECURE_H
#endif
#include "udiplus_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSTHIRD_CERT_LEN		2560			// ֤���ַ����ĳ���
#define CSTHIRD_CERT_NULL		1<<31			// û���ҵ�֤��Ĵ�����
#define CSTHIRD_PRIVATEKEY_LEN	2560			// ˽Կ����
#define CSTHIRD_CERT_NAME_LEN	20				// ���������ܵ����Ƴ���

/**@brief �ն���Ϣ֤�����Կ����*/
typedef enum _CSThird_INFO_E
{
	EM_THIRD_CERT_LOCAL = 0x2000,					// ���ر��ؿͻ���֤��
	EM_THIRD_CERT_PRIVATEKEY,						// ���ؿͻ��˵�˽Կ
	EM_THIRD_CERT_ROOT_CA = 0x1000,					// ���ؿͻ��˵ĸ�֤��
	EM_THIRD_CERT_SECOND_CA = 0x1001,				// ���ؿͻ��˵Ķ���CA֤��
	EM_THIRD_CERT_THIRD_CA,							// ���ؿͻ��˵�����CA֤��
	EM_HIRD_CERT_SERVCER = 0x3000,					// ��������֤��
	EM_THIRD_PRIVATE_LOCAL							// ���ؿͻ��˵�˽Կ
}CSThird_Info_E;

/**@brief �ն���Կ��֧�ֵ�����*/
typedef enum _CSTHIRD_KEY_INCRYPT_E
{
	E_THIRD_PKEY_RSA =1 ,						// RSA˽Կ
	E_THIRD_PKEY_DSA,							// DSA˽Կ
	E_THIRD_PKEY_EC								// EC˽Կ
}CSthird_Key_Incrypt_E;

/**@brief ��ȡ֤�����Կ������:�ļ������ڴ�����*/
typedef enum _CSTHIRD_CERT_KEY_E
{
	E_CERT_FILE_TYPE =1,							// �ļ�����֤��
	E_CERT_MEM_TYPE									// �ڴ�����֤��
}CSThird_Cert_Key_E;


/**@brief �ļ�֤����ļ���Կ������Ϊ��������*/
typedef enum _CSTHIRD_CE_FILE_E
{
	E_CERT_FILETYPE_ASN1 =1,						// asn1���ļ�����
	E_CERT_FILETYPE_PEM								// Pem���ļ�����
}CSThird_File_E;

/**
@brief ��ȡ֤��Ļص�
@param[in] unsigned short usCertType ��ȡ֤������ͣ������ CSThird_Info_E
@param[in|out] unsigned char *pcCert ���֤���buffer
@param[in|out] int *pnLen ���֤��ĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_Cert)(unsigned short usCertType, unsigned char *pcCert, int *pnLen);

/**
@brief ��ȡ˽Կ�Ļص�
@param[in] unsigned short usCertType ��ȡ˽Կ�����ͣ������ CSThird_Info_E
@param[in|out] unsigned char *pcKey ���˽Կ��buffer
@param[in|out] int *pnLen ���˽Կ�ĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_PrivateKey)(unsigned short usCertType, unsigned char *pcKey, int *pnLen);

/**
@brief ��ȡ��Կ�Ļص�
@param[in] unsigned short usCertType ��ȡ��Կ�����ͣ������ CSThird_Info_E
@param[in|out] unsigned char *pcKey ���˽Կ��buffer
@param[in|out] int *pnLen ��ù�Կ�ĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_PublicKey)(unsigned short usCertType, unsigned char *pcKey, int *pnLen);

/**
@brief ʹ��RSA��Կ��ǩ�Ļص�
@param[in] unsigned char ucDigisteAlgID ǩ���㷨�������ɵ���������������
@param[in] const unsigned char *szPublicKey ���빫Կ
@param[in] int nKeyLen ��Կ�ĳ���
@param[in] const unsigned char *pcInData ��ǩ������
@param[in] int nInLen ��ǩǰ���ݵĳ���
@param[in|out] const unsigned char *pcSignData ��ǩ�������
@param[in] nSignLen ��ǩ�����ݵĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_RsaVerify)(unsigned char ucDigisteAlgID, const unsigned char *szPublicKey, int nKeyLen, 
	const unsigned char *pcInData, int nInLen, const unsigned char *pcSignData, int nSignLen);

/**
@brief ʹ��RSA��Կǩ���Ļص�
@param[in] unsigned char ucDigisteAlgID ǩ���㷨�������ɵ���������������
@param[in] const unsigned char *pcPrivateKey ����˽Կ
@param[in] int nKeyLen ˽Կ�ĳ���
@param[in] const unsigned char *pcInData Ҫǩ��������
@param[in] int nInLen ǩ��ǰ���ݵĳ���
@param[in|out] const unsigned char *pcOutData ǩ���������
@param[in] nSignLen ǩ�������ݵĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_RsaSign)(unsigned char ucDigisteAlgID, const unsigned char *pcPrivateKey, int nKeyLen, 
	const unsigned char*pcInData, int nInLen, const unsigned char *pcOutData, int *nOutLen);

/**
@brief ���������Ļص�
@param[in|out] unsigned char *pcRandom ����������buffer
@param[in] int nLen ������ĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_GetRandom)(unsigned char *pcRandom, int nLen);

/**
@brief ʹ��RSA�㷨����
@param[in] const unsigned char *pzPrivateKey �����˽Կ
@param[in] int nKeyLen ˽Կ�ĳ���
@param[in] unsigned char *pcInData Ҫ���ܵ�����
@param[in] int nInLen Ҫ�������ݵĳ���
@param[in|out] unsigned char *pcOutData ���ܺ������
@param[in] int *pnOutLen ���ܺ����ݵĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_RsaDecrypt)(const unsigned char *pzPrivateKey, int nKeyLen, unsigned char *pcIndata, 
	int nInLen, unsigned char *pcOutData, int *pnOutLen);

/**
@brief ʹ��RSA�㷨����
@param[in] const unsigned char *pzPublicKey ����Ĺ�Կ
@param[in] int nKeyLen ��Կ�ĳ���
@param[in] unsigned char *pcInData Ҫ���ܵ�����
@param[in] int nInLen Ҫ�������ݵĳ���
@param[in|out] unsigned char *pcOutData ���ܺ������
@param[in] int *pnOutLen ���ܺ����ݵĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_RsaEncrypt)(const unsigned char *pzPublicKey, int nKeyLen, unsigned char *pcIndata, 
	int nInLen, unsigned char *pcOutData, int *pnOutLen);

/**
@brief ����RSA��Կ��
@param[in] unsigned char ucType ������Կ�Ե����ͣ�����Ϊ������Կ�ĳ���
@param[in|out] unsigned char *pzPublicKey ��ù�Կ��buffer
@param[in|out] int* nKeyLen ��Կ�ĳ���
@param[in|out] unsigned char *pcPrivateKey ���˽Կ��buffer
@param[in|out] int *pnPrivateKeyLen ˽Կ�ĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_GenRsaKey)(unsigned char ucType, char *pcPublicKey, int* pnPublicKeyLen, 
	unsigned char *pcPrivateKey, int *pnPrivateKeyLen);

/**
@brief ʹ��˽Կ�����ݼ���
@param[in] const char *pcInData Ҫ���ܵ�����
@param[in] int nInLen Ҫ���ܵ����ݳ���
@param[in|out] unsigned char *pcOutData ��ż��ܺ������
@param[in|out] int *pnOutLen ���ܺ����ݵĳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_SecureDigest)(const char *pcInData, int nInLen, unsigned char *pcOutData, int *pnOutLen);

/**
@brief ʹ��˽Կ������ǩ��
@param[in] unsigned short usCertType ǩ���㷨
@param[in] const unsigned char *pcInData Ҫǩ��������
@param[in] int nInLen Ҫǩ�����ݵĳ���
@param[in|out] unsigned char *pcSignDat ǩ���������
@param[in|out] int *nSignLen ǩ��������ݳ���
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int(*CSThird_SecureSign)(unsigned short usCertType, const unsigned char *pcInData, 
	int nInLen, unsigned char *pcSignData, int *nSignLen);

/**
@brief �ӵ������ļ��ж���֤��
@param[in|out] void *x509 ���x509֤���buffer
@param[in] char *pCertPath ֤���ļ���·��
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int (*CSThird_Cert_File)(void *x509, char *pCertPath);

/**
@brief �ӵ������ļ��ж���˽Կ
@param[in|out] void *pKey ���˽Կ��buffer
@param[in] char *pCertPath ˽Կ�ļ���·��
@return �ɹ�����0��ʧ�ܷ��ؾ���Ĵ����
@note ���صĴ����Ŀǰ�ɵ���������
*/
typedef int (*CSThird_PriKey_File)(void *pKey, char *pPriKeyPath);

/**@brief �����׼�ע��ص��Ľṹ��*/
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
@brief ע����������ܵ���Ϣ

@param[in] CSThird_Secure_EVP *sSSLEvp   RtspClient_S�ṹ
@return ����ɹ�������1��ʧ�ܷ���0
@note ��
*/	
int CSRegisterThirdSecureEvp(CSThird_Secure_EVP *sSSLEvp);

/**
@brief ɾ�����������ܵ���Ϣ

@param[in] pName ���������̵�����
@return ����ɹ�������1��ʧ�ܷ���-1
@note ��
*/	
int CSDeleteThirdSecureEvp(char *pName);

/**
@brief����֤�����

@param[in] CSThird_Info_E eInfoOpt ֤������
@param[in] CSThird_Cert_Key_E eCertOpt ֤���ļ����ͣ������ļ���Կ���ڴ�˽Կ
@param[in] CSThird_File_E eFileOpt �ļ�֤������ͣ�����ASN1,PEM
@param[in] char * pFilePath ������ļ�֤�飬�����ļ��ĵ�ַ(ֻ�е�eFileOpt�ļ���Կ�ǲ���Ч)
@param[in] char* pProName ���������̵�����
@return ����ɹ�������1��ʧ�ܷ���0
@note ��
*/
int CSThird_Secure_SetCertOpt(CSThird_Info_E eInfoOpt, CSThird_Cert_Key_E eCertOpt, 
	CSThird_File_E eFileOpt, char *pFilePath, char* pProName);
/*
@brief������Կ����

@param[in] CSThird_Cert_Key_E eKeyOpt ˽Կ�ļ����ͣ������ļ���Կ���ڴ�˽Կ
@param[in] CSThird_File_E eFileOpt �ļ���Կ�����ͣ�����ASN1,PEM
@param[in] CSthird_Key_Incrypt_E  eInopt ��Կ����ʽ��RSA, DSA, EC
@param[in] char * pFilePath ������ļ���Կ�������ļ���Կ�ĵ�ַ(ֻ�е�eFileOpt�ļ���Կ�ǲ���Ч)
@param[in] char* pProName���������̵�����
@return ����ɹ�������1��ʧ�ܷ���0
@note ��
*/
int CSThird_Secure_SetKeyOpt(CSThird_Cert_Key_E eKeyOpt, CSThird_File_E eFileOpt, 
							 CSthird_Key_Incrypt_E eInopt,
							 char *pFilePath, char* pProName);

#ifdef  __cplusplus
}
#endif
