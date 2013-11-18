/**@defgroup ����̫�͹淶���ṩ��Ӧ�ö�ȡ����̫��config���е���Ϣ
@brief ģ���Ӧͷ�ļ�<csbl_UnitendLib.h>��

@version 1.0.0 2011/12/15 
@{
*/
#ifndef _CSBL_UNITENDLIB_H_
#define _CSBL_UNITENDLIB_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define BLUNITENDDESCRIPT_MAX_LEN (14)

typedef struct
{
	WORD  wConfigLen;
	BYTE	aucUConfigDes[BLUNITENDDESCRIPT_MAX_LEN];
	WORD 	wOperateId;
	WORD 	wRegionId;
	DWORD 	dwLoaderVer;
	DWORD dwModuleId;
	DWORD   dwCrc;
}T_BLUnitendConfig;


/**
@brief ��ʼ������̫��config��

@return �ɹ�����TRUE;���ʧ�ܷ���FALSE

@note ���ñ��ӿ�ǰ����Ҫ��ͨ��CSBLSetConfig(�����csbl_gconfig.h)��������̫��config��ĵ�ַ
*/
BOOL CSBLInitUnitendConfig(void);


/**
@brief ��ȡ����̫��config������Ϣ

@Param[1]:�����ȡ������Ϣ
@Param[2]:�����buf�ĳ���

@return �ɹ�����TRUE;���ʧ�ܷ���FALSE

@note ���ñ��ӿ�ǰ����Ҫ�ȵ���CSBLInitUnitendConfig��ʼ��config��
*/
BOOL CSBLGetUnitendConfig(T_BLUnitendConfig* pUnitendCfg,DWORD dwLen );

#ifdef  __cplusplus
}
#endif

#endif



