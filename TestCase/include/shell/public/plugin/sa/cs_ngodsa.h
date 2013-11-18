#ifndef _CSNOGDSA_H_
#define _CSNGODSA_H_
#include <stdio.h>
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern  "C" {	 //��C++�н���C��ʽ����extern  ��C�� {}�ڵĺ���ע�⣺��д��C��
#endif
 
/*Ƶ��ṹ��*/
typedef struct _NgodSaFreqInfo_S
{
	unsigned int uFrequency;   //Ƶ�� ��λ hz
	unsigned int uSymnolRate;  //������
	unsigned int uQamMode;     //���ƽ����ʽ
}CSNgodSaFreqInfo_S;

//Ƶ����Ϣ�ṹ��
typedef struct _NgodSaChannelInfo_S
{
   int  nChannelId;          //Ƶ��ID
   int  nServiceId ;         
   int  nTsId;
   int nOriginalNetworkId;
   BOOL bIsStartOver;        //�Ƿ�֧�ֻؿ�
   BOOL bIsNpvr;             //�Ƿ�֧��Npvr
   BOOL bIsTVAnyTime;         //�Ƿ�֧��ʱ��
   BOOL bNpvrOrderFlag;       //�Ƿ��Ѷ���Npvr
   BOOL bTVAnyTimeOrderFlag; //�Ƿ��Ѷ���ʱ��
   BOOL bStartOverOrderFlag;  //�Ƿ��Ѷ����ؿ�
  
}CSNgodSaChannelInfo_S;

typedef enum  _CSNgodSaRet_E
{
	EM_NGODSA_SUCCESS,
	EM_NGODSA_INVALIDPARA,
	EM_NGODSA_FAILURE,
	EM_NGODSA_NETERROR
}CSNgodSaRet_E;

typedef enum _CSNgodSaType_E
{
	EM_NGODSA_TYPE_COMMON,
	EM_NGODSA_TYPE_CA,
	EM_NGODSA_TYPE_CAINIT_OK
}CSNgodSaType_E;


/**
@brief ����һ��NGOD ��֤ģ����
@param[in] phSaHandle 
@param[out] phSaHandle  NGOD ��֤ģ����
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note �ڿ�����֤ʱҪ�ȵ��ô˽ӿڴ�����֤��������ݾ������֤������������֤����
*/
CSNgodSaRet_E CSNgodSaCreate(HCSHANDLE *phSaHandle); 

/**
@brief ��ȡNOGDSA ģ����
@param[out] phHandle NGOD ��֤ģ����
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note 
*/
CSNgodSaRet_E CSNgodSaGetHandle(HCSHANDLE *phHandle);
/**
@brief ����һ��NGOD ��֤ģ����
@param[in] hSaHandle NGOD ��֤ģ����
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note �ڿ�����֤�ͻ�ȡ���������֮��Ҫ���ô˽ӿ�������֤���
*/
CSNgodSaRet_E CSNgodSaDestory(HCSHANDLE hSaHandle);

/**
@brief ���ÿ���
@param[in] hSaHandle: NGOD ��֤ģ����
@param[in] pcSmcId: ���뿨���ַ������׵�ַ
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note �ڷ�����֤����֮ǰ Ҫ�ȵ��ô˽ӿڰѿ������ý�ȥ
*/
CSNgodSaRet_E CSNgodSaSetSmartCard(HCSHANDLE hSaHandle,char *pcSmcId);

/**
@brief ������֤��������IP��ַ�Ͷ˿ں�
@param[in] hSaHandle: NGOD ��֤ģ����
@param[in] pServerIp: ����IP��ַ�ַ������׵�ַ
@param[in] nServerPort: ��֤�������Ķ˿ں�
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note �ڷ�����֤����֮ǰ Ҫ�ȵ��ô˽ӿڰ���֤��������IP��ַ�Ͷ˿ں����ý�ȥ
*/
CSNgodSaRet_E CSNgodSaSetServerInfo(HCSHANDLE hSaHandle,char *pcServerIp,int nServerPort);

/**
@brief  ���÷�������Ӧ��ʱʱ��
@param[in] hSaHandle: NGOD ��֤ģ����
@param[in] nTimeout: �û����볬ʱʱ�䣬��λ����
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note   ����֤����֮ǰ���ô˽ӿ�,���볬ʱʱ��,������֤����ʱ��������Ӧ��ʱʱ�����û�����Ϊ׼��
		�û����Բ����ô˽ӿڣ���������Ӧ��ʱʱ��ΪĬ��ֵ4000��
*/
CSNgodSaRet_E  CSNgodSaSetTimeout(HCSHANDLE hSaHandle, int nTimeout);

/**
@brief ������֤���󲢴���֤��������ȡƵ���б�
@param[in] hSaHandle: NGOD ��֤ģ����
@param[inout] pnFreqCount: �û�����Ľṹ������Ĵ�С,����ʵ�ʻ�ȡƵ��ĸ���
@param[out]psFreqInfo:  ����Ƶ����Ϣ�б������psFreqInfoΪ�׵�ַ�Ľṹ��������
@param[out]pnErrCode: ���ط�����������
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note �˺���Ϊ����������ֱ���ȵ����������ػ�ﵽ��ʱʱ��Ż᷵��
*/
CSNgodSaRet_E CSNgodSaNavCheck(HCSHANDLE hSaHandle,CSNgodSaFreqInfo_S *psFreqInfo, int* pnFreqCount,int *pnErrCode);

/**
@brief  ����֤��������ȡƵ���б�
@param[in] hSaHandle: NGOD ��֤ģ����
@param[inout] pnChnlCount: �û�����Ľṹ������Ĵ�С,����ʵ�ʻ�ȡƵ���ĸ���
@param[out]psChnlsInfo: ����Ƶ���б������psChnlsInfoΪ�׵�ַ�Ľṹ��������
@param[out]pnErrCode: ���ط�����������
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note �˺���Ϊ����������ֱ�����������ػ�ﵽ��ʱʱ���Ż᷵��
*/
CSNgodSaRet_E CSNgodSaGetChannels(HCSHANDLE hSaHandle, CSNgodSaChannelInfo_S *psChnlsInfo, int*pnChnlCount,int *pnErrCode);

/**
@brief  ��ȡ������
@param[in] hSaHandle: NGOD ��֤ģ����
@param[in] psFreqInfo: �û�����Ƶ����Ϣ�б�Ľṹ�������׵�ַ
@param[in] nFreqCount: �û�����Ľṹ������Ĵ�С
@param[in] bAutodeployerRunning: Ӧ�ò����Ƿ����
@param[in] bGetAreaCodeType ΪTRUE��ʾ��������Ƶ������ȡ�����룬ΪFALSE��ʾֻ����Ƶ���б�����С������Ƶ������ȡ������
@param[out]pnAreaCode: ����������
@param[out]psOutFreqInfo: ���ػ�ȡ������ʱ��Ƶ�ɹ���Ƶ��
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note �˺���Ϊ����������ֱ����Ƶ�ɹ����һ�ȡ�������Ż᷵��
*/
CSNgodSaRet_E CSNgodSaGetAreaCode(HCSHANDLE hSaHandle, BOOL bAutodeployerRunning, CSNgodSaFreqInfo_S *psFreqInfo,const int nFreqCount,int *pnAreacode,CSNgodSaFreqInfo_S *psOutFreqInfo, BOOL bGetAreaCodeType);

/**
@brief  ���û�������nptʱ��ͬ��
@param[in] hSaHandle: NGOD ��֤ģ����
@return �ɹ�����EM_NGODSA_SUCCESS ,ʧ���򷵻�EM_NGODSA_FAILURE��EM_NGODSA_INVALIDPARA
@note 
*/
CSNgodSaRet_E  CSNgodSaTimeSync(HCSHANDLE hSaHandle ,char *pcNptServer);


/**
@brief �㲥ʱ����������Ϊ�յ�����»�ȡ������
@param[in] none
@param[out]pnTempAreaCode: ����������
@return �ɹ�����0,ʧ�ܷ���-1
@note �˺����л�ȡ������ʱ�������ȡ�����ļ��ϴ�������ȡ������ɹ���Ƶ�㲻Ϊ�գ�������
@����Ƶ�㣬���Ϊ�ջ���������Ƶ���ȡ������ʧ�ܣ���ôֻ����Ƶ���б�����С��ǰ����Ƶ��
*/
int CSVodCheckGetAreacode(int *pnTempAreaCode);

/**
@brief  �ж�portal�Ƿ��Ѿ�����
@returnTRUE ��ʾ�Ѿ������ˣ�FALSE��ʾ��δ����
@note 
*/
BOOL CSNgodSa_IsPortalRunning();

/**
@brief  �����Ƿ�У����������Ƶ�ɹ���Ƶ���Ƿ���Ƶ���б��еĵ�һ��Ƶ?
@param[in] bCheck TRUE��ʾУ�飬�����Ƶ�ɹ���Ƶ�㲻�ǵ�һ��Ƶ�㣬��ô�ٴӵ�һ��Ƶ�㿪ʼѭ����һ��, Ĭ��ΪFALSE
@return ��
@note 
*/
void CSNgodSa_SetCheckLockedFreqMode(BOOL bCheck);


#ifdef __cplusplus
}
#endif

#endif



