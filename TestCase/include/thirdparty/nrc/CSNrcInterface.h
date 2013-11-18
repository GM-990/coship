#ifndef _CSNRCINTERFACE_H
#define _CSNRCINTERFACE_H

#include "mmcp_typedef.h"
#include "udi2_tuner.h"

#ifdef __cplusplus
extern "C" 
{
#endif

#define CSNRC_INFOLEN 32

/**@brief STB״̬ö��*/
typedef enum{
	EM_NRC_STATUS_DVB,
	EM_NRC_STATUS_BROWSE,
	EM_NRC_STATUS_VOD,
	EM_NRC_STATUS_INVALID
 }CSNrcStatus_E;

typedef struct{
	char m_acVersion[CSNRC_INFOLEN];		///<�汾��Ϣ������IPT�汾�š�NRC�汾�š�IPTЭ��汾��
	char m_acRf[CSNRC_INFOLEN];			///<������Ϣ������RFƵ�㡢RF��ַ
	char m_acLinkstatus[CSNRC_INFOLEN];	///<ͨ��״̬������/δ���ӡ�����ʱ��
	char m_acRssi[CSNRC_INFOLEN];		///<խ����ƽ������RSSI������RSSI
	char m_acSignalQuality[CSNRC_INFOLEN];	///<�ź�Ʒ�ʣ�RF�����������RF������
	char m_acTsInfo[CSNRC_INFOLEN];		///<������Ϣ��TS��Ƶ���Ƶ�ʡ������ʡ����Ʒ�ʽ
	char m_acIpgIp[CSNRC_INFOLEN];		///<�ֶ˵�ַ��IPG IP
	char m_acErrorInfo[CSNRC_INFOLEN];		///<��Ͻ����
}CSNrcStatusInfo_S;

typedef struct{
	char m_acTsInfo[CSNRC_INFOLEN];			///<����Ƶ�㣺TS��Ƶ���Ƶ�ʡ������ʺ͵��Ʒ�ʽ
	char m_acTunerStatus[CSNRC_INFOLEN];		///<��Ƶ״̬��TS��Ƶ�����Ƶ״̬
	char m_acSignalLevel[CSNRC_INFOLEN];		///<�ź�ǿ�ȣ�TUNER���ź�ǿ��
	char m_acIPQAMIp[CSNRC_INFOLEN];		///<ת�����ã�IPQAM��IP��ַ����
	char m_acRfLink[CSNRC_INFOLEN];			///<RF  ���ԣ�RF���Ե��ܰ���������������ʱʱ�䡢��������
	char m_acTsLink[CSNRC_INFOLEN];			///<TS  ���ԣ�TS���Ե��ܰ���������������ʱʱ�䣬��������
	char m_acUartInfo[CSNRC_INFOLEN];			///<�����շ������ڷ����ֽ����������ֽ���
	char m_acErrorInfo[CSNRC_INFOLEN];		///<��Ͻ��
}CSNrcTestResult_S;

/**
@brief ֪ͨNRC�⣬ STBҵ���л�

@param[in] eMode STB���л�������״̬,ȡֵ�μ�CSNrcStatus_E
@return �ɹ�TRUE��ʧ�ܷ���FALSE
*/
BOOL CSNrcSwitch(CSNrcStatus_E eMode);

/**
@brief ����NRC�㲥Ƶ��

@param[in] sDeliver VOD�㲥Ƶ��
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
*/
BOOL CSNrcSetVodDeliver(CSUDITunerCableDeliver_S sDeliver);

/**
@brief ��ȡ��ǰNRC״̬�����ô˽ӿ�ǰ��Ҫ����CSNrcSwitch�л���EM_NRC_STATUS_BROWSE״̬

@param[out] psStatusInfo NRC״̬��Ϣ
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note �˽ӿ����ڻ�ȡ��̬�ĵ�ǰNRC״̬
*/
BOOL CSNrcGetStatusInfo(CSNrcStatusInfo_S* psStatusInfo);

/**
@brief NRC״̬����

@param[out] psTestInfo NRC���Խ����Ϣ
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note �˽ӿ���Ҫ����ε����Ի�ȡ��̬����Ϣ�����ô˽ӿ�ǰ��Ҫ����CSNrcSwitch�л���EM_NRC_STATUS_BROWSE״̬
*/
BOOL CSNrcLinkTest(CSNrcTestResult_S* psTestInfo);

#ifdef __cplusplus
}
#endif

#endif

