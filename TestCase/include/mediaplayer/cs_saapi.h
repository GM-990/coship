/******************************************************************************
FileName:  cs_sa_api.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD��

Version:       Ver 1.0
Author:        Tong Fulei (tongfulei@coship.com)

File Description:
        citv server auth api.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2008-02-28  Tong Fulei      Create
2.0		    2008-06-28  Tong Fulei	    Add V3.0
*******************************************************************************/

#ifndef CS_SAAPI_H
#define CS_SAAPI_H
#ifdef __cplusplus
extern "C"
{
#endif
    /******************************************************************************
     *                                 ͷ�ļ�����                                 *
     ******************************************************************************/
#include "mmcp_typedef.h"
#include "cs_utils.h"
#include "udi2_tuner.h"
    /******************************************************************************
     *                                 �ꡢ���塢�ṹ��                           *
     ******************************************************************************/
//�����ؽ�Ŀ��Ϣ
#define CSSA_MAX_Program_NUMBER 		(1000)
#define MAX_ERR_MSG_INFO_LEN   (128)
    typedef struct CSSAProgramInfo_S
    {
        char szChannelId[16];
        char szProgramName[64];
        char szProgramId[16];
        int nStartTime;
        int nEndTime;
    }CSSAProgramInfo_S;

    typedef struct CSSAProgramList_S
    {
        int nTotalCount;//psCorrelativeInfo��ָ��Ŀռ�������CSSAProgramInfo_S������
        int nAvailCount;//��Ч��Ŀ�ĸ���
        CSSAProgramInfo_S * psCorrelativeInfo;
    }CSSAProgramList_S;
//�汾�ţ�Ŀǰ֧����Ѷ2.3��3.0
    typedef enum CSITVServerVer_E
    {
        EM_ITV_SECOND_POINT_THREE,
        EM_ITV_THREE_POINT_ZERO,
        EM_ITV_THREE_POINT_ONE,
        EM_ITV_VER_COUNT
    }CSITVServerVer_E;
//��������ǰ�˵Ľ���ͨ�����뷽ʽ
    typedef enum CSITVMODE_E
    {
        EM_ITV_MODE_IPTV,//��iptv
        EM_ITV_MODE_IPQAM,//˫��
        EM_ITV_MODE_DVBIP,//˫ģ
        EM_ITV_MODE_MAX
    }CSITVMODE_E;
    typedef enum CSITVDomainID_E
    {
        EM_ITV_UNDEFINED = 0,
        EM_ITV_NTPDOMAIN,
        EM_ITV_EPGDOMAIN,
        EM_ITV_UPGRADEDOMAIN,
        EM_ITV_MANAGEDOMAIN,
        EM_ITV_INFODOMAIN,//...5
        EM_ITV_GAMEDOMAIN,//��Ϸ
        EM_ITV_PLAYBACKDOMAIN,
        EM_ITV_MOVIEDOMAIN,//�㲥
        EM_ITV_PORTALDOMAIN,
        EM_ITV_COLLECTIONDOMAIN,//...10
        EM_ITV_SEARCHDOMAIN,
        EM_ITV_PREFERDOMAIN,
        EM_ITV_VVDOMAIN,
        EM_ITV_SZDOMAIN,
        EM_ITV_ITVTVDOMAIN,//...15
        EM_ITV_MP3DOMAIN,
        EM_ITV_SHORTCUTDOMAIN,
        EM_ITV_TIMESHIFTDOMAIN,
        EM_ITV_PAYOUTDOMAIN,//֧��
        EM_ITV_KLOKDOMAIN,//����OK  20
        EM_ITV_STOCKDOMAIN//��Ʊ

    }CSITVDomainID_E;
//return value
    typedef enum CSITVRetVal_E
    {
        EM_ITV_SUCCESS = 0,
        EM_ITV_FAILURE,
        EM_ITV_NOAREACODE,//û�л�ȡ��������
        EM_ITV_SYNCFAIL,//NTPͬ��ʧ��
        EM_ITV_NETFAULT,//�������
        EM_ITV_INVALIDSINA,//��Чǩ��
        EM_ITV_TOKEN_OVERDUE,//ǩ������
        EM_ITV_INVALIDUSER,//��Ч�û�
        EM_ITV_NO_ORDER,
        EM_TTV_DATA_ERROR
    }CSITVRetVal_E;
//��֤״̬
    typedef enum CSITV_SA_STATE_E
    {
        EM_ITV_SASTATE_INIT = 0,//û����֤
        EM_ITV_SASTATE_FAIL ,//��֤ʧ��
        EM_ITV_SASTATE_SUCCESS ,//��֤�ɹ�
        EM_ITV_SASTATE_NOTAREACODE, //��֤�ɹ�,��û������������
        EM_ITV_SASTATE_TIMESYNC

    }CSITV_SA_STATE_E;
//�ն˽����豸������
    typedef enum CSITV_SA_CPETYPE_E
    {
        EM_ITV_CPETYPE_STB,
        EM_ITV_CPETYPE_MOBILE,
        EM_ITV_CPETYPE_PC
    }CSITV_SA_CPETYPE_E;
//�ն�֧�ֵ���Ƶ����
    typedef enum CSITV_SA_VIDEO_TYPE_E
    {
        EM_ITV_VIDEO_MPEG2_SD,
        EM_ITV_VIDEO_MPEG2_HD,
        EM_ITV_VIDEO_H264_SD,
        EM_ITV_VIDEO_H264_HD
    }CSITV_SA_VIDEO_TYPE_E;
//���豸��ʾģ��֧�ֵķֱ���
    typedef enum CSITV_SA_RESOLUTION_E
    {
        EM_ITV_RESOLUTION_720_576,
        EM_ITV_RESOLUTION_1280_720
    }CSITV_SA_RESOLUTION_E;
    typedef int (* FOnCSSAGetMac)(char *pMacAdress,int nLen);
    typedef int (* FOnCSSAGetIP)(char *pIPAddress,int nLen);
    typedef struct  CSITVLoginInfo_S
    {
        char m_caSTBSMID[64]; //CA����
        char m_caSTBID[32]; //���������к�
        char m_caSTBPSWORD[32]; //����������
        char m_caSTBProvider[32];//�����й�Ӧ��
        char m_caUserId[64];//��һ�ε�½�ɹ��󱣴浽flash��
        char m_caITVToken[36];//��Ҫ�ڻ����г��ڱ��棬ÿ�ε�½�����
        //��һ�ε�½���ֶα���Ϊnull
        char m_caServerIP[20];//������IP
        char m_caLoginKey[64];//��������
        char m_caAlias[64];//����
        CSITVServerVer_E m_eSTBVersion;//�������汾
        int  m_nServerPort;//�������˿�,Ĭ��80
        CSITVMODE_E m_eMode;//���뷽ʽ
        char m_caSTBModel[32];//�������ͺ�
        CSITV_SA_CPETYPE_E  m_eCPEType;//Ĭ��EM_ITV_CPETYPE_STB
		char m_caVideoType[32];//Ĭ��EM_ITV_VIDEO_MPEG2_SD
		CSITV_SA_RESOLUTION_E m_eResolution;//Ĭ��EM_ITV_RESOLUTION_720_576
        int m_nBandWith;//m_eModeΪEM_ITV_MODE_IPTVʱ��Ч
        char  m_caBrowserVersion[64];//������汾
        FOnCSSAGetMac m_fGetMac;
        FOnCSSAGetIP m_fGetIP;
		//����Ϊ����ʡ����֤�����ֶΣ�������Ŀ���Բ���
		char m_caMiddleWareVer[32];  //�м���汾��
		char m_caSTBSoftWareVer[16];  //����������汾��
		char m_caTVCode[16];  //���Ӻ�
    } CSITVLoginInfo_S;
   typedef struct CSSAErrorMsg_S
   {
   		int nErrorCode;
		char pcMsg[MAX_ERR_MSG_INFO_LEN];
   }CSSAErrorMsg_S;

	typedef struct CSSAPlaybackChannelInfo_S
  {
	 char m_caName[64];
	 char m_caResourceName[64];
	 char m_caChannelID[64];
	 char m_caMulticastIP[20];
	 int m_nFlag;
	 int m_nChannelType;
	 int m_nTSID;
	 int m_nServiceID;
	 int m_nMulticastPort;
 }CSSAPlaybackChannelInfo_S;
   //�ص���Ϣ����
	typedef enum CSSAMsg_E
	{
	    EM_ITV_GETAREACODE_SUCCESS = 0,//��Ӧ��һ�������Ǵ洢��������׵�ַ���ڶ��������ǿռ�Ĵ�С
		EM_ITV_GETAREACODE_FAIL
	}CSSAMsg_E;
    /******************************************************************************
     *                                 ��������                                   *
     ******************************************************************************/

    /******************************************************************************
    Function Name:    CSSA_Initialize

    Description:
      �����ĳ�ʼ����֤ģ�顣��ʼ��ǰ����ע�������ӿ�(SA_Interface)

    Input:
      ��
    Output:

    Return Value:
      success : 	EM_ITV_SUCCESS
      fail :		EM_ITV_FAILURE

    Other:
    *******************************************************************************/
    int CSSA_Initialize();

    /******************************************************************************
    Function Name:    CSSA_Init

    Description:
      MMCPר�ó�ʼ����֤ģ�顣�������MMCPΪ������SA_Interface��ע�Ṥ��
      ʹ�øýӿ�ʱ�ɲ�����SA_Interface��ع�����

    Input:
      ��
    Output:

    Return Value:
      success : 	EM_ITV_SUCCESS
      fail :		EM_ITV_FAILURE

    Other:
    *******************************************************************************/
    int CSSA_Init();

    /******************************************************************************
    ��������:	CSSA_Login

    ��������:
    		��½��֤������,����֮ǰ��֤�Ѿ���������֤��Ҫ����Ϣ��

    �������:
    		��

    �������:
    		��

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:ͬ����ѶSA��֤,����EM_ITV_FAILUREʱ������CSSA_GetErrorMsg�ӿ�ȥ��ó�����Ϣ
    ******************************************************************************/
    int CSSA_Login();

	/******************************************************************************
    ��������:	CSSA_Logout

    ��������:
    		ע����ǰ��¼

    �������:
    		��

    �������:
    		��

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:ͬ����ѶSA��֤,����EM_ITV_FAILUREʱ������CSSA_GetErrorMsg�ӿ�ȥ��ó�����Ϣ
    ******************************************************************************/
    int CSSA_Logout();
    /******************************************************************************
    ��������:	CSSA_SetUserInfo

    ��������:
    		�����û���Ϣ

    �������:
    		psInfo:�û���Ϣ

    �������:
    		��

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:ͬ����ѶSA��֤
    ******************************************************************************/
    int CSSA_SetUserInfo(CSITVLoginInfo_S* psInfo);

    /******************************************************************************
    ��������:	CSSA_GetAuthResult

    ��������:
    		��ȡ��֤���

    �������:
    		��

    �������:
    		��

    �� �� ֵ:
    		�ο�CSITV_SA_STATE_E.

    ����˵��:ͬ����ѶSA��֤
    ******************************************************************************/
    CSITV_SA_STATE_E CSSA_GetAuthResult();
    
    /******************************************************************************
    ��������:	CSSA_GetIndexByTsIdAndServiceId

    ��������:
    		��֤ͨ�������ͨ���˺����жϣ�nTsId,nServiceId����ǰ��Ŀ�Ƿ�֧��
    		�طţ����֧���򷵻سɹ���ͬʱ��ȡ����Ӧ��������

    �������:
    		nTsId��
    		nServiceId��

    �������:
    		pnIndex

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:ͬ����ѶSA��֤
    ******************************************************************************/
    int CSSA_GetIndexByTsIdAndServiceId(int nTsId, int nServiceId, int *pnIndex);
        
    /******************************************************************************
    ��������:	CSSA_GetAuthenCode

    ��������:
    		��ȡ��֤��.

    �������:
    		szAuthenCode:�洢��֤��,�洢���ɵ��������롣�洢�ռ����32���ֽڡ�

    �������:
    		��

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_GetAuthenCode(char *szAuthenCode);

	/******************************************************************************
    ��������:	CSSA_SetAreaCode

    ��������:
    		��ȡ������.

    �������:
    		szAreaCode:	�洢������,�洢���ɵ��������롣�洢�ռ�С��64���ֽ�
    		nLen:		nLenΪ���õ�������ĳ���,Ӧ��С�ڵ���64

    �������:
    		��

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_SetAreaCode(char * szAreaCode,int nLen);
    /*****************************************************************************
    ��������:	CSSA_GetAreaCode

    ��������:
    		��ȡ������.

    �������:
    		szAreaCode:�洢������,�洢���ɵ��������롣�洢�ռ����32���ֽڡ�
			bBlock     �Ƿ�ǿ��ɨ��������bbBlockΪTRUE�ҵײ�û�������������Чֵʱǿ��ɨ��������,
					   ����ֻ���صײ㱣��ĵ�ǰֵ���ջ�����Чֵ��
    �������:
    		��

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
	    ����û�����bForceΪtrue,ǿ��ͬ����ȡ�����룬�˷�������
	    ����û�����bForceΪFALSE,��ȡ���е�������
	******************************************************************************/
    int CSSA_GetAreaCode(char * szAreaCode, BOOL bBlock);

/******************************************************************************
��������:	CSSA_GetAreaCodeByChannel

��������:
		�������������ͨ������ȡ��Ӧ�������룬���û�кϷ��������룬�����'\0'.

�������:
		pszAreaCode:�洢������,�洢���ɵ��������롣�洢�ռ����32���ֽڡ�
		pszChannel: ����ͨ������ѡֵ:"IPTV","IPQAM"
�������:
		��

�� �� ֵ:
		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

����˵��:
	�˺�����Ҫ����˫ģ�����п�������ͨ��Э�̵�����£����Ը��ݲ�ͬ��ͨ��ѡ���ȡ��Ӧ��������
    ����û���ȡIPQAM������,���ǵ�һ�λ�ȡ����֮ǰû�л�ȡ�ɹ�������ǿ��ͬ����ȡ�����룬�˷�������
    ����û���ȡIP������,���Ƿ���ǰ���·���������
******************************************************************************/
int CSSA_GetAreaCodeByChannel(char *pszAreaCode,const char* pszChannel);

/******************************************************************************
    ��������:	int CSSA_GetErrorMsg(CSSAErrorMsg_S * psErrorMsg)

    ��������:
    		��ȡ��֤�����еĴ�����Ϣ

    �������:
    		��

    �������:
    		psErrorMsg ָ��Ӧ�÷���Ļ�����,�����������ʹ�����Ϣ��.

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:nErrorCode�ο�
    
	    0000������ɹ� ��¼�ɹ�
		0602���û���������Ϊ��
		0611����¼��������Ϊ��
		0612���û�ǩ������Ϊ��
		0613�������������Ϊ��
		0614���û���������Ϊ��
		0616���û��ı�ʶid����Ϊ��
		0617���û���ʶid��������
		0619��Ӳ���ṩ���̲���Ϊ��
		0622���û��Ľӿڰ汾����Ϊ��
		0623���ӿڰ汾����
		0633������ʧ��
		0650���ն��û���¼ʧ��
		0661��ָ���ն��û�������
		0662��ָ���û������ݿ��в�����
		0665���û����°汾ֵ������
		0668���ն��û���û�м�����ȼ���
		0669�������������
		0671�����ǩ��ʧ��
		0676���汾��Ϣ����Ϊ��
		0678��Stb�����ṩ�߲���Ϊ��
		0690��3.1��֤�ؼ������ô���
		0691���û���Ψһ
		0692����������sic,stb,alies,mac�ֶ���ȡ����һ����Ϊ��ݱ�ʶ
    ******************************************************************************/
	int 	CSSA_GetErrorMsg(CSSAErrorMsg_S * psErrorMsg);
	/******************************************************************************
    ��������:	CSSA_GetSmcId

    ��������:
    		��ȡsmart card id.

    �������:
    		szSmcId:�洢smart card id,�洢���ɵ��������롣�洢�ռ����32���ֽڡ�

    �������:
    		��

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_GetSmcId(char *szSmcId);
    
    /******************************************************************************
    ��������:	CSSA_SetSmcId

    ��������:
    		����smart card id,��Ϊ��֤�Ĺؼ���ʹ��.

    �������:
    		szSmcId:�洢smart card id,�洢���ɵ��������롣

    �������:
    		��

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_SetSmcId(char *szSmcId);
    
    /******************************************************************************
    ��������:	CSSA_GetCurChannelProgramList

    ��������:
    		��ȡ��tsid��serviceidָ��Ƶ���Ľ�Ŀ�б�
    �������:
    		nTsId:
    		nServiceId :
    �������:
    		pProList:�洢��ָ��Ƶ���Ľ�Ŀ�б�
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_GetCurChannelProgramList(int nTsId, int nServiceId, CSSAProgramList_S * pProList);
	/******************************************************************************
    ��������:	CSSA_GetChannelProgListBat

    ��������:
    		��ȡָ���ļ���Ƶ���ĻطŽ�Ŀ�б�
    �������:
    		psLctDVBInfo 	ָ��һ��CSLocatorDVBInfo_S�����飬�洢��ָ����Ƶ����TsId,ServiceID
			pnChannelCount	ָ��psLctDVBInforָ������Ĵ�С��Ҳ����ָ����Ƶ������,�������Ƶ�������ظ�ʱ�������Ὣ��ָ���ֵ��Ϊʵ��Ƶ����
								
    �������:
    		pProList  		�洢ָ������Ƶ���ĻطŽ�Ŀ�б�
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:����EM_ITV_FAILUREʱ������CSSA_GetErrorMsg�ӿ�ȥ��ó�����Ϣ
    ******************************************************************************/
	int CSSA_GetChannelProgListBat(CSLocatorDVBInfo_S  * psLctDVBInfo,CSSAProgramList_S * pProList,int * pnChannelCount);
    /******************************************************************************
    ��������:	CSSA_GetCorrelativeProgramList

    ��������:
    		��ȡ��ָ����Ŀ�����Ľ�Ŀ�б�
    �������:
    		pcChannelId:Ƶ����id
    		pcProgramName :��Ŀ����
    		pcStartTime:��Ŀ��ʼʱ��
    		pcProgramId:��Ŀ��id,2.3�汾����ΪNULL
    �������:
    		pCorrProList:�洢��ָ����Ŀ�����Ľ�Ŀ�б�
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:����EM_ITV_FAILUREʱ������CSSA_GetErrorMsg�ӿ�ȥ��ó�����Ϣ
    ******************************************************************************/
    int CSSA_GetCorrelativeProgramList(char * pcChannelId, char * pcProgramName, char * pcStartTime,char *pcProgramId , CSSAProgramList_S * pCorrProList);
    
    /******************************************************************************
    ��������:	CSSA_GetCorrRtspUrlByIndex

    ��������:
    		��ȡ�����б���ָ��������url
    �������:
    		Index:������Ŀ��������
    		nLen :szUrl�ĳ���
    �������:
    		szUrl:�洢ָ����Ŀ��url
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:����EM_ITV_FAILUREʱ������CSSA_GetErrorMsg�ӿ�ȥ��ó�����Ϣ
    ******************************************************************************/
    int CSSA_GetCorrRtspUrlByIndex(int nIndex, char * szUrl,int nLen);
    
    /******************************************************************************
    ��������:	CSSA_GetChannelProgRtspUrlByIndex

    ��������:
    		��ȡƵ���б���ָ��������url
    �������:
    		Index:Ƶ����Ŀ��������
    		nLen :szUrl�ĳ���
    �������:
    		szUrl:�洢ָ����Ŀ��url
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_GetChannelProgRtspUrlByIndex(int nIndex, char * szUrl,int nLen);
    
    /******************************************************************************
    ��������:	CSSA_GetPBRtspUrl

    ��������:
    		��ȡָ��Ƶ��ָ��ʱ���url
    �������:
    		nTsId:   tsID
    		nServiceId:  ��ĿserviceID
    		dwStartTime :ָ���Ŀ�ʼʱ��,��λs
    		nUrlLen:pDstUrl�Ĵ洢�ռ䳤��
    �������:
    		pDstUrl:ָ��洢�ý�Ŀ��url��buffer


    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_GetPBRtspUrl(int nTsId, int nServiceId, DWORD dwStartTime, char *pDstUrl, int nUrlLen);
	/******************************************************************************
	��������:	CSSA_GetMultTimeShiftUrl

	��������:
			�����鲥��ַ���鲥�˿ڡ�ʱ�ƿ�ʼʱ���ȡ�鲥ʱ�Ƶ�url.

	�������:
			pcMultIP:�鲥IP,��ʽ��224.0.1.10
			wMultPort:�鲥�˿�
			dwStartTime:ʱ�ƿ�ʼʱ��,���1970�������
			nUrlLen:ʱ��url�洢�ռ�ĳ���

	�������:
			pDstUrl:ʱ��url�洢�ռ䣬�ɵ���������
	�� �� ֵ:
			EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

	����˵��:
******************************************************************************/
	int CSSA_GetMultTimeShiftUrl(char* pcMultIP, WORD wMultPort,DWORD dwStartTime, char *pDstUrl, int nUrlLen);
	/******************************************************************************
	��������:	CSSA_ISSupportMultTimeShift

	��������:
			�����鲥IP��ַ���鲥�˿ڡ�

	�������:
			pcMultIP:�鲥IP,��ʽ��224.0.1.10
			wMultPort:�鲥�˿�
	�������:

	�� �� ֵ:
			TRUE ֧��ʱ�ƣ�FALSE ��֧��ʱ��

	����˵��:
	******************************************************************************/
	BOOL CSSA_ISSupportMultTimeShift(char* pcMultIP, WORD wMultPort);
    /******************************************************************************
    ��������:	CSSA_GetVersion

    ��������:
    		��ȡ��ǰʹ�õİ汾��
    �������:

    �������:
    �� �� ֵ:
    		����ο�CSITVServerVer_E

    ����˵��:
    ******************************************************************************/
    CSITVServerVer_E CSSA_GetVersion();
    
    /******************************************************************************
    ��������:	CSSA_GetServerUrl

    ��������:
    	��ȡ�����͵ķ�������ַ,Ϊ������ǰ��Ӧ�ñ���,����ʹ��CSSA_GetParameterInfo
    �������:
    	eDomainType:����������
    	nServerLen:szServer�ĳ���
    �������:
    	szServer:�洢�������ĵ�ַ

    �� �� ֵ:
    		����ο�CSITVServerVer_E

    ����˵��:�Ƽ�����CSSA_GetParameterInfo����÷�������ַ
    ******************************************************************************/
    int CSSA_GetServerUrl(CSITVDomainID_E eDomainType, char * szServer, int nServerLen);
    /******************************************************************************
    ��������:	CSSA_GetParameterInfo

    ��������:
    	��ȡǰ�˷��͵����ò�����Ϣ
    �������:
    	pcKey:���ò���������,��ο� CiTVƽ̨��֤�ӿڹ淶.doc ���°汾������	
    	nLen: �û�����ռ�Ĵ�С,�����ò���ʵ�ʳ��ȴ���nLen-1ʱ,����EM_ITV_FAILURE.
    �������:
    	pcValue:�洢���ò�����ֵ,�ڴ���Ӧ������

    �� �� ֵ:
    	EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    	���õ�KEY:iEPG_URL,NTP_SERVER���<<CiTVƽ̨��֤�ӿڹ淶.doc>>
    ******************************************************************************/
    int CSSA_GetParameterInfo(char * pcKey,char * pcValue,int nLen);
    /******************************************************************************
    ��������:	CSSA_InsertStrToUrl

    ��������:
    	�Ե㲥url����,������֤��Ϣ������3.0�汾��
    �������:
    	pUrl:�㲥�������´���url.
    �������:
    	pDstUrl:������url
    	nDstUrlLen:pDstUrl�ĳ���
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_InsertStrToUrl(const char *pUrl, char *pDstUrl, int nDstUrlLen);
    
    /******************************************************************************
    ��������:	CSSA_GetUserId

    ��������:
    		��ȡ�û��ı�ʶ
    �������:
    		pUserId:�洢�û���ʶ
    		nLen:pUserId�ĳ���
    �������:
    		pUserId:�洢�û���ʶ
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_GetUserId(char * pUserId,int nLen);
    
    /******************************************************************************
    ��������:	CSSA_SetMode

    ��������:
    		������֤ģʽ,Ĭ��EM_ITV_MODE_IPQAM_PES
    �������:
    		eMode:��֤ģʽ����Ҫ����IPTV��IPQAM
    �������:
    		��
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_SetMode(CSITVMODE_E eMode);
    
    /******************************************************************************
    ��������:	CSSA_SetSAServer

    ��������:
    		����������֤�����������ڳ�ʼ��ʱ���������ô���
    		��֤ǰ��ǰ���÷�������ַ��
    �������:
    		pSAServer:��������ַ
    �������:
    		��
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    ******************************************************************************/
    int CSSA_SetSAServer(char *pSAServer);

    int CSSA_GetFreListForAreacode(char *pFrequenceList,int nLen);
   /******************************************************************************
    ��������:	CSSA_GetFreList

    ��������:
    		��ȡ��֤ǰ���·���Ƶ���б�
    		
    �������:
    		psFreqList:Ƶ���б�ָ��
    		pnNum: �����CSUDITunerCableDeliver_S�ṹ��Ĵ�С
    �������:
    		psFreqList:����˷������·���Ƶ���б�ָ��
    		pnNum: ʵ�ʷ��ص�CSUDITunerCableDeliver_S�ṹ��Ĵ�С
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    		������ȷ��ȡ��ǰ������֤ͨ��
    ******************************************************************************/
	int CSSA_GetFreList(CSUDITunerCableDeliver_S* psFreqList,int* pnNum);

    int CSSA_GetSignature(char *pSignature,int nLen);

    int CSSA_GetMac(char *pMac,int nLen);    

	int CSSA_Cleanup();
	/***���溯����Ϊdtvmxʵ�ֵ�,ע���Ժ󲹳�**/

	int  CSSA_GetChannelId(int networkId,int nTsId, int nServiceId,char *szChannelId ,int nLen );

	BOOL CSSA_ISSupportPlayBack(int nNetworkId,int nTsId,int nServiceId);

	int   CSSA_GetOrderFlag(int nNetworkId,int nTsId,int nServiceId,int * pnFlag);
	 /******************************************************************************
    ��������:	CSSA_GetPlaybackURLByChannelID

    ��������:
    		��ȡָ��Ƶ��ָ��ʱ���url
    �������:
    		pChannelID:Ƶ��ID
    		dwStartTime :ָ���Ŀ�ʼʱ��,��λs
    		nUrlLen:pDstUrl�Ĵ洢�ռ䳤��
    �������:
    		pDstUrl:ָ��洢�ý�Ŀ��url��buffer

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    		�˺�����ɵĹ��ܺ�CSSA_GetPBRtspUrl��ͬ����������CSSA_GetPlaybackURLByChannelIDҪ����pChannelID,
    		��������Ҫ���е�tsID  serviceID
    ******************************************************************************/
	int   CSSA_GetPlaybackURLByChannelID(char *pChannelID, DWORD dwStartTime, char *pDstUrl, int nUrlLen);
	 /******************************************************************************
    ��������:	CSSA_StartScanAreaCode

    ��������:
    		����ɨ��������
    �������:
    
    �������:
    		

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    	CSSA_StartScanAreaCode��CSSA_StopScanAreaCodeһ��Ҫ�ɶԵ���
    ******************************************************************************/
	int   CSSA_StartScanAreaCode();
	 /******************************************************************************
    ��������:	CSSA_StopScanAreaCode

    ��������:
    		����ɨ��������
    �������:
    
    �������:
    		

    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

    ����˵��:
    	CSSA_StartScanAreaCode��CSSA_StopScanAreaCodeһ��Ҫ�ɶԵ���
    ******************************************************************************/
	int   CSSA_StopScanAreaCode();
	/******************************************************************************
	Function Name:    FOnCSSACallback
	 
	Description:
		�û��ص�,SA����Ϣ���ظ��û�
	 
	Input:
		dwUserData: �û��ڴ���SAʱ������û�����
		nMsg : ��Ϣ����
		dwPara1 : 	����1 
		dwPara2 :	����2 
	 
	Output:
	 
	Return Value:
	 
	Other:
	*******************************************************************************/
	typedef void (*FOnCSSACallback)(DWORD dwUserData, CSSAMsg_E enMsg, DWORD dwPara1, DWORD dwPara2);	
	 /******************************************************************************
	��������:	CSSA_AddCallback

	��������:
			
	�������:
		dwUserData:�û�����
		callback:�ص�����
	�������:
			

	�� �� ֵ:
			EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

	����˵��:
	******************************************************************************/
	int CSSA_AddCallback(DWORD dwUserData, FOnCSSACallback callback);
	 /******************************************************************************
	��������:	CSSA_RemoveCallback

	��������:
		dwUserData:�û�����
		callback:�ص�����
	�������:

	�������:
			

	�� �� ֵ:
			EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��

	����˵��:
	******************************************************************************/
	int CSSA_RemoveCallback(DWORD dwUserData, FOnCSSACallback callback);
	/******************************************************************************
    ��������:	CSSA_GetChannelList

    ��������:
    		
    �������:
    			
    �������:			
    			psChannelInfo     Ƶ���б������׵�ַ
    			pnChannelNum	  Ƶ����Ŀ,������Ϊʵ��Ƶ����
    �� �� ֵ:
    		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��
    ����˵��:
    ******************************************************************************/
    int CSSA_GetChannelList(CSSAPlaybackChannelInfo_S * psChannelInfo,int * pnChannelNum);

/******************************************************************************
��������:	CSSA_GetStreamChannel

��������:
		��ȡǰ���·���ͨ��ѡ�����
�������:
			
�������:			
			pcKey     ͨ�����Թؼ��֣���"HD","SD"
			pcValue	  pcKey��Ӧ��ǰ���·���ͨ������
			nLen      pcValue����ռ�ֽڴ�С    
�� �� ֵ:
		EM_ITV_SUCCESS �ɹ���EM_ITV_FAILURE ʧ��
����˵��:
******************************************************************************/
int CSSA_GetStreamChannel(char * pcKey,char * pcValue,int nLen);

#ifdef __cplusplus
}
#endif

#endif /* CS_SAAPI_H */

/* �ļ����� */

