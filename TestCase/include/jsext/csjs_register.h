#ifndef __CS_JSREGISTER_H__
#define __CS_JSREGISTER_H__

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
Function Name:    CSJSInit
 
Description:
    ��ʼ��jsextģ�飬�˷�������ע��һЩ������js��չ����
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�
 
Input:
    ��

Output:
    ��
Return Value:
    ��

Other:
*******************************************************************************/
void CSJSInit(void);

/******************************************************************************
Function Name:    CSJSRegisterVOD
 
Description:
    ע��˫����ѶVODǰ��ҳ���õ���JS��չ����
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�
 
Input:
    ��

Output:
    ��
Return Value:
    ��

Other:
*******************************************************************************/
void CSJSRegisterVOD(void);

/******************************************************************************
Function Name:    CSJSRegister2DElement
 
Description:
    ע��2D��ص�JS����
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�JS ����2d�ӿڣ�����Ч 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegister2DElement(void);

/******************************************************************************
Function Name:    CSJSRegister2DElement
 
Description:
    ע��2D��ص�JS����
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ�.
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegister2D(void);

/******************************************************************************
Function Name:    CSJSRegisterDVB
 
Description:
    ע�����JS�м������Ŀ���õ��ĵ���DVBҵ������ص�JS��չ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�

    �˷��������ڳ�ʼ����dtv��mediaplayer��fs֮����ã���Ϊjsext dvb��������
    ����Щģ��
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/

void CSJSRegisterDVB(void);

/******************************************************************************
Function Name:    CSJSRegisterCITV
 
Description:
    ע�����JS�м������Ŀ���õ���˫��DVBҵ������ص�JS
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�ͬ����ѶVOD�½�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterCITV(void);

/******************************************************************************
Function Name:    CSJSRegisterTeletext
 
Description:
    ע��subtitle/teletext/closecaption������ص�JS��չ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�Teletext�½ڡ�Subtitle�½�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterTeletext(void);


/******************************************************************************
Function Name:    CSJSRegisterPVR
 
Description:
    ע��PVR������ص�JS��չ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�PVR�����½�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterPVR(void);

/******************************************************************************
Function Name:    CSJSRegisterEW200VOD
 
Description:
    ע��EW200VOD������ص�JS��չ
    �˽ӿڸ�����������EW200�м����ʱ��ʹ�ã���ʱ��������
    ֻ�������ǵ�VOD���������м���еĲ���ģ��(MP)�����Զ���
    JS�����е�MediaPlayer��ʵ�־Ͳ�����CSJSRegisterVOD�е�ֱ�ӵ����м��
    �Ĳ���ģ��(MP)��ʵ�֣�������Ҫ���ø��������ṩ��EW200��
    UDI�ӿ���ʵ��

    ���Դ˽ӿ���CSJSRegisterVOD�ǲ���ͬʱʹ�õģ�������������Ԥ֪
    ���쳣
    
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��

Other:
*******************************************************************************/
void CSJSRegisterEW200VOD(void);

/******************************************************************************
Function Name:    CSJSRegisterTTS
 
Description:
    ע��TTS��������Ҫ��JS��չ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterTTS(void);

/******************************************************************************
Function Name:    CSJSRegister3D
 
Description:
    ע��3D��Ч��������Ҫ��JS��չ ����opengl��opengles��ʵ�ֵ�3D JS�ӿڷ�װ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegister3D(void);

/******************************************************************************
Function Name:    CSJSRegister3DMesa
 
Description:
    ע��3DMesa��Ч��������Ҫ��JS��չ ����Mesa��ʵ�ֵ�3DӦ�� ��������ǽ�������ǵ�
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegister3DMesa(void);

/******************************************************************************
Function Name:    CSJSRegisterUPNP
 
Description:
    ע��UPNP��������Ҫ��JS��չ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterUPNP(void);

/******************************************************************************
Function Name:    CSJSRegisterCAppManager
 
Description:
    ע���CӦ�ù���������Ҫ��JS��չ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�Ӧ�ù����½�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterCAppManager(void);

/******************************************************************************
Function Name:    CSJSRegisterJavaAppManager
 
Description:
    ע���JavaӦ�ù���������Ҫ��JS��չ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�Ӧ�ù����½�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterJavaAppManager(void);


/******************************************************************************
Function Name:    CSJSRegisterTJ
 
Description:
  ע������������������Ҫ��JS��չ
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/

void CSJSRegisterTJ(void);

/******************************************************************************
Function Name:    CSJSRegister2DFlatWall
 
Description:
    ע��2D ƽ����Ч��ص�JS����
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�JS 2D ƽ����Ч�½�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegister2DFlatWall(void);

/******************************************************************************
Function Name:    CSJSRegisterSatellite
 
Description:
    ע��Satellite������ص�JS��չ
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterSatellite(void);

/******************************************************************************
Function Name:    CSJSRegisterIPTV1
 
Description:
    ע���������,Device����
Input:
    ��

Output:
    ��
Return Value:
    ��
notes:
var device = new De
Other:
*******************************************************************************/
void CSJSRegisterIPTV1(void);


/******************************************************************************
Function Name:    CSJSRegisterAD
 
Description:
    ע����ģ������Ҫ��jsext����
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�
 
Input:
    ��

Output:
    ��
Return Value:
    ��

Other:
*******************************************************************************/
void CSJSRegisterAD(void);

/******************************************************************************
Function Name: CSJSRegisterSTMosaic
 
Description:
    ע����ͨ�����˹���
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterSTMosaic(void);

/******************************************************************************
Function Name: CSJSRegisterHMMosaic
 
Description:
    ע����������˹���
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterHMMosaic(void);

/******************************************************************************
Function Name: CSJSRegisterSaition
 
Description:
    ע��˦�Ź���
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/


void CSJSRegisterSaition(void);
/**
@brief JS ����
*/
typedef enum _CSJSPrototype_E {
	EM_JS_METHOD,   //����Ϊ����
	EM_JS_PROPERTY  //����Ϊ����
}CSJSPrototype_E;

/**
@brief JS����ֵ ����
*/
typedef enum _CSJSResultBack_E {
     EM_JS_NUMBER,   //JS���󷵻�ֵ������Ϊnumber ,��Ϊnumber��ʱ�򣬴���
     EM_JS_STRING,  //����Ϊstring
     EM_JS_BOOLEAN,//����Ϊboolean
     EM_JS_ARRAY //����ΪArray
}CSJSResultBack_E;

/**
@brief JS���������Ϣ����
*/
typedef struct _CSJSObjectCallInfo_S {
	CSJSPrototype_E m_eType;    //���õ�����
	char m_acJSObjectStr[32];   //���������
	char m_acPrototypeName[64]; //�˶����Ӧ�����Ի��߷���������
	char m_acArgs[512]; //���Ի��߷�������Ӧ������ֵ����ʽΪ"{arg1:value1;arg2:value2...}", ����Ӧ��JSҳ����д����ʱJS����ķ��������������
}CSJSObjectCallInfo_S;


/**
@brief JSע���Զ���ص�����
@param[in] pstJSObjectCallInfo:�˶����һЩ������Ϣ
@param[out] pOutResult:������õķ���ֵ
@param[in] nResultSize:����ֵ�ĳ���
@param[out]pResultType:������÷���ֵ������,�����CSJSResultBack_E�Ķ���
*/
typedef void (*CSJSCallUserDefineObj_F)(CSJSObjectCallInfo_S *pstJSObjectCallInfo, CSJSResultBack_E *pResultType, void *pOutResult, int nResultSize);
 
/******************************************************************************
Function Name: CSJSRegisterJSObjectC
 
Description:
    ע���Զ����JS��չ����
    ע�⣬Ŀǰֻ֧��ע�����ö���ʹ��C�ķ�����ʵ��jsext��
    ������˽ӿ�֮�󣬻���Ҫ����CSJSAddJSObjectMethod��CSJSAddJSObjectProperty����Ӵ�JS����ķ���������
    �����JS�������κ����Ժͷ�����û��������

Input:
    pcJSObjectStr : JS��������ƣ���Ӧ��JSҳ����д����ʱJS������ַ�����
    fnJSCallback:�ص�����ָ�룬���CSJSCallUserDefineObj_F����
Output:
    ��
Return Value:
    �ɹ�����TRUE
    ʧ�ܷ���FALSE
 
Other:
*******************************************************************************/
BOOL CSJSRegisterJSObjectC(const char *pcJSObjectStr, CSJSCallUserDefineObj_F fnJSCallback);


/******************************************************************************
Function Name: CSJSRegisterJSObject
 
Description:
    ע���Զ����JS��չ����ע�⣬Ŀǰֻ֧��ע�����ö���

Input:
    pcJSObjectStr : JS��������ƣ���Ӧ��JSҳ����д����ʱJS������ַ�����
    pcJavaClassPath : JS�����Ӧ��Java���������·��
 
Output:
    ��
Return Value:
    �ɹ�����TRUE
    ʧ�ܷ���FALSE
 
Other:
*******************************************************************************/
BOOL CSJSRegisterJSObject(const char *pcJSObjectStr, const char *pcJavaClassPath);

/******************************************************************************
Function Name: CSJSAddJSObjectMethod
 
Description:
    ����Զ���JS����ķ���

Input:
    pcJSObjectStr : JS��������ƣ���Ӧ��JSҳ����д����ʱJS������ַ�����
    pcMethodName : JS����ķ�����
 
Output:
    ��
Return Value:
    �ɹ�����TRUE
    ʧ�ܷ���FALSE
 
Other:
*******************************************************************************/
BOOL CSJSAddJSObjectMethod(const char *pcJSObjectStr, const char *pcMethodName);

/******************************************************************************
Function Name: CSJSAddJSObjectProperty
 
Description:
    ����Զ���JS���������

Input:
    pcJSObjectStr : JS��������ƣ���Ӧ��JSҳ����д����ʱJS������ַ�����
    pcMethodName : JS�����������
 
Output:
    ��
Return Value:
    �ɹ�����TRUE
    ʧ�ܷ���FALSE
 
Other:
*******************************************************************************/
BOOL CSJSAddJSObjectProperty(const char *pcJSObjectStr, const char *pcPropertyName);

/******************************************************************************
Function Name: CSJSRemoveJSObject
 
Description:
    ɾ��֮ǰ��ӵ��Զ���JS����

Input:
    pcJSObjectStr : JS��������ƣ���Ӧ��JSҳ����д����ʱJS������ַ�����
 
Output:
    ��
Return Value:
    �ɹ�����TRUE
    ʧ�ܷ���FALSE
 
Other:
*******************************************************************************/
BOOL CSJSRemoveJSObject(const char *pcJSObjectStr);

/******************************************************************************
Function Name:    CSJSRegisterHWVOD
 
Description:
    ע�����JS�м������Ŀ���õ���˫��DVBҵ������ص�JS��չ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�ͬ����ѶVOD�½�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterHWVOD(void);

/**
@brief JSҵ��ö��
*/
typedef enum _CSJSServic_E{
    EM_JS_EPG,        //EPG ,�ص���Ӧ��pcServiceDataȡֵΪNULL
    EM_JS_MOSIC,     //�����ˣ��ص��е�pcServiceData��ʽ:{mosaicIndex:value1, url:"value2"}";����mosaicIndex��Ӧ��ֵΪint���ͣ���ʾ�����˵�����;url��Ӧ��ֵΪ"dvb://OriginalNetworkID.TS_ID.ServiceID",��ʾ��ǰ�����˶�Ӧ����Ҫ�ء�
    EM_JS_AD,		 //��棬�ص��е�pcServiceData�ĸ�ʽΪ"{typeIndex:value1, isShow:value2}"��typeIndex��ֵΪint���ͣ���ʾ���λ,���������ο�<JavaScript��չAPI�ӿڹ淶>�ĵ��еĹ���½�;isShow��Ӧ��ֵΪint���ͣ���ֵΪ0��ʱ���ʾ����ʾ��Ϊ������ʾ��ʾ�ù��λ��
    EM_JS_APP           //Ӧ�ã��ص��е�pcServiceData�ĸ�ʽΪ"{appId:"value1"}��appId��ӦΪ�ַ���,��ʾӦ�õ����ơ�
} CSJSServic_E;

/**
@brief ��ȡJSҵ��״̬�ص���������
@param[in] eService ���CSJSServic_E˵��
@param[in] pcServiceData  ���CSJSServic_Eע��˵����ע�������ʽ��Ҫ��ascii
@param[in] pUserData �û�����
*/
typedef void (*CSJSServiceStateCallback_F) (CSJSServic_E eService, const char* pcServiceData, void* pUserData);

/**
@brief ע���ȡJSҵ����Ϣ�Ļص�������
@param[in] fnServiceCallback ָ��ص������ĺ���ָ��
@param[in] pUserData �û����ݣ�����ΪNULL
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note  ����������ע��5���ص�
*/
BOOL CSJSAddServiceStateCallback(CSJSServiceStateCallback_F fnServiceCallback, void* pUserData);


/**
@brief ɾ����ȡJSҵ����Ϣ�Ļص�������
@param[in] fnServiceCallback ָ��ص������ĺ���ָ��
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
*/
BOOL CSJSRemoveServiceStateCallback(CSJSServiceStateCallback_F fnServiceCallback);

/******************************************************************************
Function Name:    CSJSRegisterLocalService
 
Description:
    ע��localservice��ص�JS��չ����
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterLocalService(void);

/******************************************************************************
Function Name:    CSJSRegisterNVOD
 
Description:
    ע��NVOD��ص�JS��չ����
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�NVOD�½�	
	    
	�˷��������ڳ�ʼ����dtv֮�����, ��Ϊjs nvod��������Щ��ʼ����ʵ��
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterNVOD(void);

/******************************************************************************
Function Name:    CSJSRegisterMotoVod

Description:
ע�����JS�м������Ŀ���õ���MOTO VODҵ������ص�JS��չ

Input:
��

Output:
��
Return Value:
��

Other:
*******************************************************************************/
void CSJSRegisterMotoVod(void);
/******************************************************************************
Function Name:    CSJSRegisteGCable

Description:
ע�����JS�м������Ŀ���õ��Ĺ㶫��Ŀҵ������ص�JS��չ

Input:
��

Output:
��
Return Value:
��

Other:
*******************************************************************************/
void CSJSRegisteGCable(void);


/******************************************************************************
Function Name:    CSJSRegisterSeachangeVod
 
Description:
    ע�����JS�м������Ŀ���õ�����ص�JS��չ
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterSeachangeVod(void);

/******************************************************************************
Function Name:    CSJSRegisterHLJAD
 
Description:
    ע����������
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterHLJAD(void);

/******************************************************************************
Function Name:    CSJSRegisterNrc
 
Description:
    ע�����JS�м������Ŀ���õ���˫��DVBҵ������ص�JS��չ
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�ͬ����ѶVOD�½�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterNrc(void);

/******************************************************************************
Function Name:    CSJSRegisterVideoPhone

Description:
ע�����JS�м������Ŀ���õ�����Ƶ�绰�����JS����

Input:
��

Output:
��
Return Value:
��

Other:
*******************************************************************************/
void CSJSRegisterVideoPhone(void);

/******************************************************************************
Function Name:    CSJSRegisterUnionBank
 
Description:
    ע��������JS��չ����.
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterUnionBank(void);

/******************************************************************************
Function Name:    CSJSRegisterUSBKey
 
Description:
	ע��usbkey֧������
     
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterUSBKey(void);

/******************************************************************************
Function Name:    CSJSRegisterTSMEncryption
 
Description:
    ע�����֧��TSM������js����
   JS�ӿڶ����<���֧����Ŀ-�ն˰�ȫģ���м����ֲ�ֲ�.pdf>�ĵ��е�TSM�ṩ���м�ӵ��õĺ����ӿ��½�
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterTSMEncryption(void);

/******************************************************************************
Function Name:    CSJSRegisteDVBWithoutCA
 
Description:
    ע�����JS�м������Ŀ���õ��ĵ���DVBҵ������ص�JS��չ,��ȥca��ע��
    ��Ӧ��JS�ӿڼ�<JavaScript��չAPI�ӿڹ淶>�ĵ��е�

    �˷��������ڳ�ʼ����dtv��mediaplayer֮����ã���Ϊjsext dvb��������
    ����Щģ��
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisteDVBWithoutCA(void);

/******************************************************************************
Function Name:    CSJSRegisteDVNCA
 
Description:
   ע�����ca
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisteDVNCA(void);

/******************************************************************************
Function Name:    CSJSRegisterPayCard
 
Description:
   ע������JS����Ŀǰ�����ڽ�����Ŀ��
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisterPayCard(void);

/******************************************************************************
Function Name:    CSJSRegisterDLNA

Description:
    ע��DLNA������ص�JS����

Input:
    ��

Output:
    ��
Return Value:
    ��

Other:
*******************************************************************************/
void CSJSRegisterDLNA(void);

/******************************************************************************
Function Name:    CSJSRegisteRSAUtils
 
Description:
    RSA���ܽ�����ؽӿ�ʵ��
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisteRSAUtils(void);

/******************************************************************************
Function Name:    CSJSRegisteSkyworthAD
 
Description:
    ��ά�����ؽӿ�ʵ��
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisteSkyworthAD(void);

/******************************************************************************
Function Name:    CSJSRegisteNGB
 
Description:
    ע��NGB�淶�Ľӿ�
    ��Ӧ��JS�ӿڼ�<��һ���㲥�������м�������淶-����2��NGB-HӦ�ñ�̽ӿڣ��������۸壩_v0.7.2012080900.doc>�ĵ��е�

    �˷��������ڳ�ʼ����dtv��mediaplayer��fs֮����ã���Ϊjsext dvb��������
    ����Щģ��
 
Input:
    ��
 
Output:
    ��
Return Value:
    ��
 
Other:
*******************************************************************************/
void CSJSRegisteNGB(void);


/******************************************************************************
Function Name:    CSJSRegisterMutiPageCtrl

Description:
ע���page��JS����

Input:
��

Output:
��
Return Value:
��

Other:
*******************************************************************************/
BOOL CSJSRegisterMutiPageCtrl(void);
/******************************************************************************
Description:
	ע������ʶ�����JS����

Input:
��

Output:
��
Return Value:
��

Other:
*******************************************************************************/
void CSJSRegisterSR(void);

#ifdef __cplusplus
}
#endif

#endif
