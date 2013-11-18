#ifndef _CS_OCCONTROL_H_
#define _CS_OCCONTROL_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"


typedef enum _CSOCType
{
	EM_DEC,	//coship dvb url 10����
	EM_HEX   //ipanel dvb url 16����
}e_OCType;

typedef enum _eSourceMsg
{
	EM_SOURCE_SIGNAL_CONNECTED,           //Ƶ�������ɹ�
 	EM_SOURCE_SIGNAL_DROPPED,             //��Ƶ�������ɹ�תΪ����ʧ��
 	EM_SOURCE_SIGNAL_LOST      //Ƶ������ʧ��
}eSourceMsg;


/*�����������Ӧ�õ���Ϣ����*/
typedef enum 
{
	EM_BRW_OCCONTROL 
}eMsgType;


typedef enum _selectStatus_E
{
	EM_OC_SELECT_WAIT = 1,
	EM_OC_SELECT_READY,
	EM_OC_SELECT_FAIL,
	EM_OC_SELECT_NO_OBJ,
	EM_OC_SELECT_NO_ERROR,
	EM_OC_SELECT_START_LOAD
}selectStatus_E;


/*************************************************
�ӿں���:OCMsgNotify_F
����˵��:eMsg: EM_BRW_OC nParam is eOCMsgBoxType

����: 
**************************************************/
typedef void* ( *OCMsgNotify_F)(eMsgType eMsg, int nParam, void *pvUserData);


/*************************************************
�ӿں���:fnDVBConnectSourcCallBack
����˵��:��CSOCPortingConnectSource�·���serviceid,tsid,ornetid����
		��ocControl
����: ��
**************************************************/
typedef void ( *DVBConnectSourceCallBack_F)(eSourceMsg eMsg, int nOrnetID, int nTsID, int nServiceID);

/*************************************************
�ӿں���:OCConnectSourceCallBack_F
����˵��:OC��Ƶ����ָ�룬 �ӿ���Ҫͨ��ornetid��tsid��serverid��ȡƵ����Ϣ������Ƶ����ͨ��fnConnectSouceCB֪ͨ��occontrol��fnConnectSouceCB����ΪNULL
����: ��
**************************************************/
typedef BOOL(*OCConnectSourceCallBack_F)(int nOrNetId, int nTsId, int nServiceId, DVBConnectSourceCallBack_F fnConnectSouceCB);

/***************************************************
				C Control API
****************************************************/

/*************************************************
����:CSMidpOCInit
����˵��:midpʹ��OCControlģ�飬Ӧ����Ҫ��ʼ��֮�����
		�ú���ע��
����: 0�ɹ���-1ʧ��
**************************************************/
int CSMidpOCInit(void);

/*************************************************
����:CSDvbOCInit
����˵��:�������ע��dvbЭ��
����: 0�ɹ���-1ʧ��
**************************************************/
int CSDvbOCInit(void);

/*************************************************
����:CSDeliveryOCInit
˵��:�������ע��delivery://Frequency.SymbolRate.Modulation.serviceId.componentTag/xx/xx.txtЭ��
����: 0�ɹ���-1ʧ��
**************************************************/
int CSDeliveryOCInit(void);

/*************************************************
����:CSOCInit(int nOCBufSizem, e_OCType type)
����˵��:nOCBufSizem ����OCģ���ڴ��С,��λΪK
		type ����ǰ���·���dvb urlΪʮ���ƻ���ʮ������
����: 0�ɹ���-1ʧ��
**************************************************/
int CSOCInit(int nOCBufSizem, e_OCType type);

/*************************************************
����:CSOCSetChangeSourceInfo
����˵��: �����ݹ㲥�У�Ӧ����Ƶ�ɹ�ͨ��
		�ýӿڽ�Ƶ����Ϣ����oc controlģ�飬
		���������ݹ㲥�е���Ƶ����Ƶ��,�����֪��networkid��tsid��serviceid����0
����: 
**************************************************/
void CSOCSetChangeSourceInfo(int nFrequency, int nNetworkId, int nTsId, int nServiceId);

/**************************************************
����:  ע��OC Control��Ƶ�ص�������

����˵��: OC Control��Ƶ�ص�������
		  pvUserData �û�����
����: SUCCESS :  TRUE
**************************************************/
BOOL CSRegisterOCConnectSource(OCConnectSourceCallBack_F fnConnectSouceCB);
 
/***************************************************
			Ӧ�ð���OC Controlʵ��PORTING�ӿ�
****************************************************/

/*************************************************
�ӿں���:CSOCPortingConnectSource
����˵��:OC Control�·�OrNetId nTsId nServiceId��ʵ��
		�������ҵ�ǰService����Ƶ�㣬����Ƶ��pConnectSouceCB
		֪ͨOC Control��Ƶ״̬
����: TRUE�ɹ���FALSEʧ��
**************************************************/
extern BOOL CSOCPortingConnectSource(int nOrNetId, int nTsId, int nServiceId, DVBConnectSourceCallBack_F fnConnectSouceCB);


/*************************************************
����:CSOCdestroy
	 �˳�OCӦ�õ�������OCControl���ݣ�������˳�����Ҫ����
����: ��
**************************************************/
void CSOCdestroy(void);

/*************************************************
ע��OCMsgNotify_F�ص�����������OCControl��Ϣ֪ͨӦ��
����: TRUE�ɹ���FALSEʧ��
**************************************************/
BOOL CSOCControlRegisterMsgNotifyCB(OCMsgNotify_F fnMsgNotify);

/*************************************************
����:CSDvbOpen
����˵��: pcUrl ����URL��dvb://��ͷ
		  pcMode �ļ���ʽ(δʹ��)
		  bNeedUpdate TRUE ��Ҫ���ʵʱ���£�FALSE����ظ���
		  dwUpdateInterval���¼��ʱ��
����: > 0 ������<=0����
**************************************************/
int CSDvbOpen(const char * pcUrl, const char * pcMode, BOOL bNeedUpdate, DWORD dwUpdateInterval);

/*************************************************
����:CSDvbOpenEx
����˵��: pcUrl ����URL��dvb://��ͷ
		  pcMode �ļ���ʽ(δʹ��)
		  bNeedUpdate TRUE ��Ҫ���ʵʱ���£�FALSE����ظ���
		  dwUpdateInterval���¼��ʱ��
		  bReload �Ƿ��������أ������ļ��Ƿ����ڻ�����
����: > 0 ������<=0����
**************************************************/
int CSDvbOpenEx(const char * pcUrl, const char * pcMode, BOOL bNeedUpdate, DWORD dwUpdateInterval, BOOL bReload);

/*************************************************
����:CSDvbSelect
����˵��: nIndex CSDvbOpen����ֵ
		  ntimeout δʹ��
		  ���OC�����������
����: selectStatus_E
**************************************************/
selectStatus_E CSDvbSelect(int nIndex, int ntimeout);

/*************************************************
����:CSDvbAvailable
����˵��: nIndex CSDvbOpen����ֵ
CSDvbSelect�ɹ�֮����øýӿڿ��Եõ��ļ���С
����: �ļ���С
**************************************************/
int CSDvbAvailable(int nIndex); 

/*************************************************
����:CSDvbRecv
����˵��: nIndex CSDvbOpen����ֵ
		  pBuffer ���ݴ�ŵ�ַ
		  size��pBuffer��С
CSDvbSelect�ɹ�֮����øýӿڿ��Եõ��ļ�����
����: �������ݴ�С
**************************************************/
int CSDvbRecv(int nIndex, void *pBuffer, unsigned int size);

/*************************************************
����:CSDvbClose
����˵��: nIndex CSDvbOpen����ֵ
	������ɹر�����
����: ��
**************************************************/
int CSDvbClose(int nIndex);

/*************************************************
����:CSDvbPreLoad
����˵��: pszUrl ԤȡURL
Ԥȡ�ӿڣ�Ԥȡ���̲�������Ƶ��ֻ�Ա�Ƶ����Ч
����: ��
**************************************************/
void CSDvbPreLoad(const char * pszUrl);

/*************************************************
����:CSStartPreLoad
����˵��: ��ʼԤȡ��ҳ����Դ
����: ��
**************************************************/
void CSStartPreLoad();

/*************************************************
����:CSCancelPreLoad
����˵��: ȡ��Ԥȡ
����: ��
**************************************************/
void CSCancelPreLoad();

/*************************************************
����:CSOCSetTotalTimeOut
����˵��: ���ó�ʱʱ�䣬Ĭ��ʱ��40s
����: ��
**************************************************/
void CSOCSetTotalTimeOut(DWORD dwTotalTimeout);

/*************************************************
����:CSDvbSeek
����˵��:�򿪵��ļ���ָ����ǰ�Ķ�/дλ��
����: ��
**************************************************/
int CSDvbSeek(int nIndex, int nOffset, int nOrigin);

#ifdef  __cplusplus
}
#endif

#endif 


