/**@defgroup CHECKDECRYPT CHECKDECRYPT �ṩУ����ܽӿ�
@brief CHECKDECRYPTģ���Ӧͷ�ļ�<csbl_checkdecrypt.h>�������˸���У����ܵĽӿ�

@version 1.0.0 2011/3/30 ����У�����ģ��,����conax�淶��У�����
@{
*/
#ifndef _CSBL_CHECK_DECRYPT_H_
#define _CSBL_CHECK_DECRYPT_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "csbl_gconfig.h"

#define AES128LEN 16		//AES128 ��Կ���Ȼ���������

typedef struct
{
    BYTE m_aucKeyData[AES128LEN];	//AES 128 ��Կ������ʵ��
}CSBLAESData_S;

typedef struct		
{
	BOOL m_bValid;		//��Կ�Ƿ���Ч��־����ЧΪ0������Ϊ��Ч
	CSBLAESData_S m_sKey;	//AES ������Կ
	CSBLAESData_S m_sIV;	//AES ��������	
}CSBLAESKeyInfo_S;

typedef struct 
{
	CSBLDataPos_S *m_pSrcPos;	//APP ����ǰ��ַ
	CSBLDataPos_S *m_pDstPos;	//APP ���ܺ��ַ
}CSBLDataAndBootPos_S;


/**
@brief ��ȡConax�淶У��������������Ϣ�ĺ���
@param[in] Param1 �����У��������ݡ���CSBLDataPos_S���塣
@param[out] Param2 ��ȡ�Ľ������ݵ���Ϣ����CSBLCheckDecryptInfo_S���塣

@return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
@note ��EM_CSBL_CONFIG_CHECKDECRYPT_BOOT��EM_CSBL_CONFIG_CHECKDECRYPT_LOAD��˵��
*/
int CSBLGetCheckDecryptInfoConax(PVOID Param1, PVOID Param2);

/**
@brief Conax�淶����ʱʹ�õ�У����ܺ���
@param[in] Param1 �����У��������ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�
@param[out] Param2 �����У����ܺ����ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�

@return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
@note ��EM_CSBL_CONFIG_CHECKDECRYPT_BOOT��˵��
*/
int CSBLCheckDecryptConaxBoot(PVOID Param1, PVOID Param2);

/**
@brief Conax�淶����ʱʹ�õ�У����ܺ���
@param[in] Param1 �����У��������ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�
@param[out] Param2 �����У����ܺ����ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�

@return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
@note ��EM_CSBL_CONFIG_CHECKDECRYPT_LOAD��˵��
@note load��boot�������������ڶ���һ���������е�����汾�������ص�loader/app�е�����汾�űȽϵĹ��̣�����ͬ���ж�ΪУ��ʧ��
*/
int CSBLCheckDecryptConaxLoad(PVOID Param1, PVOID Param2);

/**
@brief Unitend�淶����appʱ��ʹ�õ�У�麯��
@param[in] Param1��Param2��δʹ��

@return����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
*/
int CSBLCheckUnitendBoot(PVOID Param1, PVOID Param2);

//��ʱδʹ��
int CSBLGetCheckDecryptInfoUnitend(PVOID Param1, PVOID Param2);
/**
@brief ��ȡVerimatrix�淶У��������������Ϣ�ĺ���
@param[in] Param1 �����У��������ݡ���CSBLDataPos_S���塣
@param[out] Param2 ��ȡ�Ľ������ݵ���Ϣ����CSBLCheckDecryptInfo_S���塣

@return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
@note ��EM_CSBL_CONFIG_CHECKDECRYPT_BOOT��EM_CSBL_CONFIG_CHECKDECRYPT_LOAD��˵��
*/
int CSBLGetCheckDecryptInfoVerimatrix(PVOID Param1, PVOID Param2);

/**
@brief Verimatrix�淶����ʱʹ�õ�У����ܺ���
@param[in] Param1 �����У��������ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�
@param[out] Param2 �����У����ܺ����ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�

@return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
@note ��EM_CSBL_CONFIG_CHECKDECRYPT_BOOT��˵��
*/
int CSBLCheckDecryptVerimatrixBoot(PVOID Param1, PVOID Param2);

/**
@brief Verimatrix�淶����ʱʹ�õ�У����ܺ���
@param[in] Param1 �����У��������ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�
@param[out] Param2 �����У����ܺ����ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�

@return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
@note ��EM_CSBL_CONFIG_CHECKDECRYPT_LOAD��˵��
@note load��boot�������������ڶ���һ���������е�����汾�������ص�loader/app�е�����汾�űȽϵĹ��̣�����ͬ���ж�ΪУ��ʧ��
*/
int CSBLCheckDecryptVerimatrixLoad(PVOID Param1, PVOID Param2);

/**
@brief Conax�淶����ʱʹ�õ�У����ܺ���
@param[in/out] Param1 ʵ��ʹ������ΪCSBLDataAndBootPos_S,  m_pSrcPos Ϊ����Ĵ�У��������ݵ�ַ��
			   m_pSrDst ΪУ����ܺ�����ݵ�ַ�� ��CSBLDataPos_S���塣
@param[in] Param2 ΪAES ������Կ����CSBLAESKeyInfo_S ���塣

@return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
@note ��EM_CSBL_CONFIG_CHECKDECRYPT_BOOT��˵��
*/
int  CSBLCheckDecrypUseOOBConaxBoot(PVOID Param1, PVOID Param2);

/**
@brief Conax�淶����ʱʹ�õ�У����ܺ���
@param[in] Param1 �����У��������ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�
@param[out] Param2 �����У����ܺ����ݡ���CSBLDataPos_S���壬����Ϊ�ڴ������ݡ�

@return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
@note ��EM_CSBL_CONFIG_CHECKDECRYPT_LOAD��˵��
*/
int CSBLCheckDecryptUseOOBConaxLoad(PVOID Param1, PVOID Param2);


/**
 @bried Conax �淶booter ����ʱʹ�õ�У�麯�����汾V2
 @Param1[in]		: CSBLDataAndBootPos_S ���ͣ���CSBLDataAndBootPos_S ����
 @Param2[out]	: CSBLAESKeyInfo_S ���ͣ���CSBLAESKeyInfo_S ����

 @return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
 @note ��EM_CSBL_CONFIG_CHECKDECRYPT_BOOT��˵��
 */
int  CSBLCheckDecrypConaxBoot_V2(PVOID Param1, PVOID Param2);


/**
 @bried Conax �淶����ʱʹ�õ�У�麯�����汾V2
 @Param1[in]		: CSBLDataPos_S *���ͣ���ʾУ�������ݣ�����Ϊ�ڴ��е�����
 @Param2[out]	: CSBLDataPos_S *���ͣ���ʾУ�������ݣ�����Ϊ�ڴ��е�����

 @return ����ɹ���������SUCCESS;���ʧ�ܷ���FAILURE;
 @note ��EM_CSBL_CONFIG_CHECKDECRYPT_LOAD��˵��
 */
int CSBLCheckDecryptConaxLoad_V2(PVOID Param1, PVOID Param2);



#ifdef  __cplusplus
}
#endif

#endif



