/**@defgroup COSHIPLOAD COSHIPLOAD �ṩcoship�淶loader�ӿ�
@brief COSHIPBOOTģ���Ӧͷ�ļ�<csbl_coshipload.h>��������coship�淶loader���еĹ��ܼ����ýӿڡ�

@version 1.0.0 2011/3/11  
@version 1.0.1 2011/4/21   ��Ӧbl_ver:2.2.0.0
@{
*/
#ifndef _CSBL_COSHIPLOAD_H_
#define _CSBL_COSHIPLOAD_H_

#ifdef  __cplusplus
extern "C" {
#endif

/**
@brief �ӽ����ȡqam��������

@param[in] Param1 ��Ч�������ɴ�NULL��
@param[in] Param2 ��Ч�������ɴ�NULL��
@return �ɹ�����SUCCESS
*/
int CSBLGetUpgParaFromGuiQam(PVOID Param1, PVOID Param2);
/**
@brief �ӽ����ȡcofdm��������

@param[in] Param1 ��Ч�������ɴ�NULL��
@param[in] Param2 ��Ч�������ɴ�NULL��
@return �ɹ�����SUCCESS
*/
int CSBLGetUpgParaFromGuiCofdm(PVOID Param1, PVOID Param2);
/**
@brief �ӽ����ȡqpsk��������

@param[in] Param1 ��Ч�������ɴ�NULL��
@param[in] Param2 ��Ч�������ɴ�NULL��
@return �ɹ�����SUCCESS
*/
int CSBLGetUpgParaFromGuiQpsk(PVOID Param1, PVOID Param2);

/**
@brief tuner��������

@param[in] Param1 ��Ч�������ɴ�NULL��
@param[in] Param2 ��Ч�������ɴ�NULL��
@return �ɹ�����SUCCESS
@note qam��cofdm��qpsk��ʹ�øú�������
*/
int CSBLUpgradeTuner(PVOID Param1, PVOID Param2);

/**
@brief usb��������

@param[in] Param1 ��Ч�������ɴ�NULL��
@param[in] Param2 ��Ч�������ɴ�NULL��
@return �ɹ�����SUCCESS
@note usb�����ӽ����ȡ�����ĺ�����δʵ�֣�������ʱ���Դ�NULL
*/
int CSBLUpgradeUsb(PVOID Param1, PVOID Param2);

/**
@brief �ɰ�ip��������

@param[in] Param1 ��Ч�������ɴ�NULL��
@param[in] Param2 ��Ч�������ɴ�NULL��
@return �ɹ�����SUCCESS
@note �ӽ����ȡ�����ĺ�����δʵ�֣�������ʱ���Դ�NULL
*/
int CSBLUpgradeIpOld(PVOID Param1, PVOID Param2);

/**
@brief ip��������

@param[in] Param1 ��Ч�������ɴ�NULL��
@param[in] Param2 ��Ч�������ɴ�NULL��
@return �ɹ�����SUCCESS
@note �ӽ����ȡ�����ĺ�����δʵ�֣�������ʱ���Դ�NULL
*/
int CSBLUpgradeIp(PVOID Param1, PVOID Param2);


/**
@brief Coship�淶loader��ں���

@return ����ɹ��������᷵��;���ʧ�ܷ���FAILURE
*/
int CSBLEntryCoshipLoad(void);



/**
@brief ����̫�͹淶loader��ں���

@return ����ɹ��������᷵��;���ʧ�ܷ���FAILURE
*/
int CSBLEntryUnitendLoad(void);


/*
���ܣ���ɶ��������ݵ�ǩ�����ǩ�ֹ���
���������
Param1 ������OTA���غ�ǩ��������Ϣ�ĵ�ַ,Ĭ������ΪCSBLDataPos_S*
Param2 �������ǩ�ֺ�������Ϣ�ĵ�ַ,Ĭ������ΪCSBLDataPos_S*

����ֵ��
-2����������
-1��У��ʧ�ܣ��ļ�����
0 ������У��ɹ���

*/


int CSBLDownLoadVerify(PVOID Param1, PVOID Param2);


#ifdef  __cplusplus
}
#endif

#endif



