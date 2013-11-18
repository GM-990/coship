#ifndef _CS_JSMOTOVOD_H_
#define _CS_JSMOTOVOD_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif


/**@brief moto vod�㲥ϵͳ�ж�Ӧ�ĸ�����Ϣ������ϵͳʹ��*/
typedef enum
{
	EM_MOVOVOD_SERVICETYPE, 		///< ServiceType 		String(16) 	R 	�������ͣ��㲥��ʱ��
	EM_MOVOVOD_SERVICEID,    		///< ServiceID 		UnsignedInt R 	����ID
	EM_MOTOVOD_SERVICELENGTH,  		///< ServiceLength 		UnsignedInt R 	������ʱ��(��)
	EM_MOTOVOD_RESTLENGTH, 			///< RestLength 		UnsignedInt R 	ʣ��ʱ��(��)
	EM_MOTOVOD_FREQUENCY, 			///< Frequency 		UnsignedInt R 	��������Ƶ��
	EM_MOTOVOD_SERVICEGROUP,		///< ServiceGroup 		String(8) 	   R 	�����
	EM_MOTOVOD_GSRMIPADDRESS, 		///< GSRMIPAddress 	String(16) 	R 	Global SRM ��IP ��ַ
	EM_MOTOVOD_BMIIPADDRESS,		///< BMIIPAddress 		String(16) 	R 	BMI ��IP ��ַ
	EM_MOTOVOD_DIAGNOSTICSTATE,		///< DiagnosticState 	String(32) 	RW 	VOD ģ��㲥����״̬
	EM_MOTOVOD_SRMIPADDRESS, 		///< SRMIPAddress 		String(16) 	RW 	SRM ��IP ��ַ
	EM_MOTOVOD_CONTENTID, 			///< ContentID 		String(16) 	RW 	�㲥����ID
	EM_MOTOVOD_ACKTIME, 			///< AckTime 			UnsignedInt R 	�㲥������Ӧʱ��
	EM_MOTOVOD_RETURNCODE, 			///< ReturnCode 		UnsignedInt R 	�㲥����״̬��	
	EM_MOTOVOD_PLAYCURRTIME,		///<CurrentTime             UnsignedInt R 	��ǰ�Ĳ���ʱ��(��)
	EM_MOTOVOD_PROGRAMENAME,		///<Programename         String(32)		��ǰ���Ž�Ŀ������
	EM_MOTOVOD_TIMESHIFT_CHANNELNAME	///channelname       String(32)		���ʱ�Ƶ�Ƶ������
}CSMotoVodInfo_E;

/**@brief moto vod �㲥ϵͳ�еĲ�������Ϣ������ϵͳʹ��*/
typedef enum
{
	EM_MOTOVOD_STOP,			//ֹͣ��ȡ�����˳�����ʧ
	EM_MOTOVOD_PLAY,			//���ţ�ȷ�ϣ��������ʾ
	EM_MOTOVOD_PAUSE, 			//��ͣ
	EM_MOTOVOD_RESUAME,			//����
	EM_MOTOVOD_NEXT,			//��һ��
	EM_MOTOVOD_PREVIOUS,		//ǰһ��
	EM_MOTOVOD_SEEK,			//��λ
	EM_MOTOVOD_FAST,			//���
	EM_MOTOVOD_BACK			//����
}CSMotoAction_E;

/**@brief moto vod �㲥�����еĴ�����Ϣ������ϵͳʹ��*/
typedef enum
{
	EM_MOTOVOD_SSP_OPEN_FAILURE,			//ssp����ʧ��
	EM_MOTOVOD_LSCP_CONNECT_FAILURE,		//LSCP����ʧ��
	EM_MOTOVOD_LSCP_OPERATOR_FAILURE,		//LSCP����ʧ��
	EM_MOTOVOD_SSP_REALEAS_FAILURE			//ssp�ͷ�����ʧ��
}CSMotoNetErr_E;


/**
@brief ���moto vod�㲥�Ķ�������
@param [in] eTag��ǰ��vod����
@note �ûص����ڴ����û�����
*/
typedef void(*CSJSMotoOperAction)(CSMotoAction_E eTag);

/**
@brief���motovod�㲥�����еķ����Ĵ���
param[in]eTag��ǰ���������
param[in]pAssertID��ǰ���ŵ���Դ��
@note�ûص����ڴ���㲥�����кͲ��Ź����з����Ĵ���
*/
typedef void(*CSJSMotoNetError)(char* pcErrorStr, char *pcAssertID);

typedef struct _MOTO_ACTION_RIGISTER
{
	CSJSMotoOperAction m_pMotoOperAction;
	CSJSMotoNetError m_pMotoNetAction;
}CSMotoActionRigister;


/**
@brief ��ȡmoto vod�㲥�����ж�Ӧ����Ϣ

@return TRUE:��ȡ�ɹ�; FALSE:��ȡʧ��
@note �ýӿ�һ����ϵͳ��ʼ��ʱ����Ҫ���ã�ǰ��������CSMISInit�Ѿ����ù�
*/
BOOL CSJSMotoVodGetInfo(CSMotoVodInfo_E eTag, char *pcValue, int nLen);

/**
@brief ����moto vod�㲥��Ϣ

@return TRUE:���óɹ�; FALSE:����ʧ��
@note �ýӿ�����ģ��㲥����(����ʡ�����)��CSMotoVodInfo_E�о���RW���Ե��ֶβſ��Ե��ñ��ӿ�
*/
BOOL CSJSMotoVodSetInfo(CSMotoVodInfo_E eTag, char *pcValue);

/**
@brief ע��VOD��������
@param [in] motoActionvod�����Ļص�
@return TRUE:���óɹ�; FALSE:����ʧ��
*/
BOOL CSJSRigisterMotoAction(CSMotoActionRigister motoAction);

#ifdef __cplusplus
}
#endif

#endif
