#ifndef _CS_CAINTERFACE_PVR_EXT_H_
#define _CS_CAINTERFACE_PVR_EXT_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

#define CS_CA_MAX_PID_NUM     (10)
#define CS_CA_CW_SIZE         (8)

typedef struct _CSCaCtrlWord_S
{
	WORD    m_wPid;
	BOOL    m_bEkey;
	BYTE    m_aucKey[CS_CA_CW_SIZE];
}CSCaCtrlWord_S;

//���ڽ�CW�ش���Ӧ�ã�Ԥ�� 
typedef void (*CSOnCaCtrlWordCome_F)(HCSHANDLE hService, CSCaCtrlWord_S* psPvrKey, void* pUserData); 
 

typedef enum _CSCaOutputType_E
{
	EM_CA_OUTPUT_PLAY   = 0x01, /*����*/
	EM_CA_OUTPUT_RECORD = 0x02, /*¼��*/
	EM_CA_OUTPUT_CAT    = 0x04  /*�������ʱ�����⴦��CAT*/
}CSCaOutputType_E;

typedef struct _CSCaFtaService
{
	WORD    m_wDemuxId;          //DMX ID��
	WORD    m_wOrginalNetworkId; //ԭʼ����ID��
	WORD    m_wTsId;             //��ID��
	WORD    m_wServiceId;        //��ĿID��
	WORD    m_wPidNum;           //Pid ����
	DWORD   m_dwDesNo;
	WORD    m_awPid[CS_CA_MAX_PID_NUM]; //Pid
	CSCaOutputType_E m_eOutputType;     //�������,NDS CAҪ�õ�
	CSOnCaCtrlWordCome_F m_fnOnCaCtrlWordCome;
	void*   m_pUserData;         //Ԥ��
}CSCaFtaService_S;

typedef struct _FuncbParam
{
	DWORD    m_hService;
	DWORD    m_dwUserData;
}FuncbParam_S;

/**************************************************************************
*Function Name: CSCASetSerivcePmtPid
*  ���ý�Ŀ��PmtPid
*
*Parameters 
*   wDemuxId:demux id
*	wOriNetId[in]:��Ŀ��ԭʼ����id
*	wTsID[in]:��Ŀ��ts id
*	wServiceID[in]:��Ŀ��service id
*	wPmtPid[in]:pmt���pmt pid
*
*Note:�˺�����Ҫ��CSCAStartService֮ǰ����
*     
**************************************************************************/
void CSCASetSerivcePmtPid(WORD wDemuxId, WORD wOriNetId, WORD wTsID, WORD wServiceID, WORD wPmtPid);

/**************************************************************************************************
 *FunctionName : CSCAStartService
 *Descriptor   : ��������/¼��
 *Parame[IN]   : pstService - ��Ŀ�����Ϣ
 *����ֵ       : NULL  - ��������/¼�Ʋ��ɹ�
 *               ��NULL- ��������/¼�Ƴɹ�
 **************************************************************************************************/
HCSHANDLE CSCAStartService(CSCaFtaService_S* pstService);

/**************************************************************************************************
 *FunctionName : CSCAStopService
 *Descriptor   : ֹͣ����/¼��
 *Parame[IN]   : hService - ����/¼��ʱ�����Handle
 *����ֵ       : SUCCESS - ֹͣ����/¼�Ƴɹ�
 *               FAILURE - ֹͣ����/¼�Ʋ��ɹ�
 **************************************************************************************************/
INT CSCAStopService(HCSHANDLE hService);

/**************************************************************************************************
 *FunctionName : CSCANewPmtSectionNotify
 *Descriptor   : ֪ͨCAģ���µ�PMT
 *Parame[IN]   : pcSectionData - PMT����
 *               nLen - PMT����
 *               hService - ����/¼��ʱ�����Handle
 *����ֵ       : SUCCESS - ����PMT���ϱ�׼
 *               FAILURE - ����PMT�����ϱ�׼
 **************************************************************************************************/
INT CSCANewPmtSectionNotify(HCSHANDLE hService, BYTE* pcSectionData, INT nLen);


/**************************************************************************************************
 *Function:   Ϊ��֧�ֶ�·���ţ��ص�������չһ����������������һ·��Ŀ����Ϣ
 *Descriptor: dwParam3�������ṹFuncbParam_S��Ӧ�ĵ�ַ��Ӧ�����յ���Ϣ��ȡdwParm3ʱҪת����
 *            FuncbParam_S�ṹ�壬m_hService��Ա������������һ·��Ŀ/�ĸ�ģ�����Ϣ������Ӧ��
 *            �����Ӧ����Ϣ,m_dwUserData��Ա��Ӧ��CSCARegisterMsgCallbackע��ص�ʱ�û���������
 *			  �Զ�������dwUserData
 **************************************************************************************************/
typedef void (*F_CBMsg)(DWORD dwType, DWORD dwParm1, DWORD dwParm2, DWORD dwParm3);

/**************************************************************************************************
 *FunctionName: CSCARegisterMsgCallback
 *Descriptor  : ����Ҫ�����Ĳ���ģ����ע��ص�������֧�ֶ��ģ��ע�ᣬ�ú�������CS_CA_Init֮ǰ���ã�
 *              ÿ��ģ��ע��ʱ��CAģ�鶼��Ǽǣ����ͬʱ֧��6��ģ��ע��
 **************************************************************************************************/
INT CSCARegisterMsgCallback(DWORD dwUserData,F_CBMsg fcbMsg);

/**************************************************************************************************
 *FunctionName: CSCARegisterMsgCallbackCancel
 *Descriptor  : ����ע���Ѿ�ע��ص�����
 * 
 **************************************************************************************************/
INT CSCARegisterMsgCallbackCancel(DWORD dwUserData,F_CBMsg fcbMsg);


#ifdef  __cplusplus
}
#endif

#endif

