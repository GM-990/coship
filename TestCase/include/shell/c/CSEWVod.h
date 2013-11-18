/**@defgroup EW200VOD EW200VOD

@brief ��ģ��Ϊ�ٻ�200�м����VOD����Ӧ�ò�ӿ�,�����ɰٻ�200��Ʒʱʹ��
@note �����ʹ�õķ����汾��֧��VOD����,���½ӿڽ�����ʧ��
@note �����ٻ�200����UDI,����δ��ע����,��μ�UDI�ĵ�
@{
*/
#ifndef _CS_EW_VOD_H_
#define _CS_EW_VOD_H_

#include "udi2_typedef.h"
#include "CSEWPublic.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DEFAULT_PLAY_ID         (0)       ///< PlayIdĬ��ֵ,�޷�ȷ��ʹ���ĸ�PlayId����ֻ֧��һ·���ŵ�VODϵͳ��,����ʹ�ø�ֵ
#define VOD_MSG_START		(0x0)

/**@brief ����ٻ�200 VOD��Ӧ�ò㽻������Ϣ����

��ͬ��Ϣ,���ܻḽ����ͬ�Ĳ���,��ϸ��������Ŀ��˵��
�����ر�˵��,����ָ���Ͳ���ָ�����������Ϣ���غ�����Ч
*/
typedef enum
{
    EM_EW_MSG_VOD_PLAYSTATE_PAUSESUCC = VOD_MSG_START + 0,     ///< ��ͣ�ɹ�,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_PAUSEFAILED,   ///< ��ͣʧ��,p0=PlayId(int),p1=�������(int)
    EM_EW_MSG_VOD_PLAYSTATE_RESUMESUCC,    ///< �ָ����ųɹ�,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_RESUMEFAILED,  ///< �ָ�����ʧ��,p0=PlayId(int),p1=�������(int)
    EM_EW_MSG_VOD_PLAYSTATE_SETRATESUCC,   ///< ���ò����ٶȳɹ�,p0=PlayId(int),p1=ʵ������(int)���������õ�ֵ����ȫһ��
    EM_EW_MSG_VOD_PLAYSTATE_SETRATEFAILED = VOD_MSG_START + 5, ///< ���ò����ٶ�ʧ��,p0=PlayId(int),p1=�������(int)
    EM_EW_MSG_VOD_PLAYSTATE_SEEKSUCC,      ///< ����ʱ��ɹ�,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_SEEKFAILED,    ///< ����ʱ��ʧ��,p0=PlayId(int),p1=�������(int)
    EM_EW_MSG_VOD_PLAYSTATE_SERVERERROR = VOD_MSG_START + 10 ,  ///< ����������,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_TOSTART,       ///< ��Ŀ���￪ʼ��,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_TOEND,         ///< ��Ŀ�����β��,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_REDIRECT,		///< ��Ŀ�ض������ӵ���һ��������,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_OPTIONS,		///<  ����rtsp
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_GETPARAMETER =  VOD_MSG_START + 15,  ///< �ӿͻ��˵õ��Ĳ���
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_SETPARAMETER,        ///<  ��ȡ����
    EM_EW_MSG_VOD_PLAYSTATE_DESCRIBE_SUCC,			///<   ��ý������������ȷ
    EM_EW_MSG_VOD_PLAYSTATE_DESCRIBE_FAIL,			///< ��ý��������������
    EM_EW_MSG_VOD_PLAYSTATE_SETUP_SUCC,				///< ��������������ӳɹ�
    EM_EW_MSG_VOD_PLAYSTATE_SETUP_FAIL = VOD_MSG_START + 20,			///< ���������������ʧ��
    EM_EW_MSG_VOD_PLAYSTATE_AUTH_SUCC,   				///< ��Ȩ�ɹ�
    EM_EW_MSG_VOD_PLAYSTATE_AUTH_FAIL, 				///< ��Ȩʧ��
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_NODATA, 			///<�ӷ��������ղ�������
    EM_EW_MSG_VOD_PLAYSTATE_LIVE_TO_TIMESHIFT, 		///<IPTV ֱ����ʱ����Ϣ
    EM_EW_MSG_VOD_PLAYSTATE_TIMESHIFT_TO_LIVE = VOD_MSG_START + 25,	///IPTV ֱ����ʱ����Ϣ
    EM_EW_MSG_VOD_PLAYSTATE_SAVE_BOOK_MARK,
    EM_EW_MSG_VOD_PLAYSTATE_GET_START_TIME,                 ///< ��ȡ��Ŀ��ʼʱ��
    EM_EW_MSG_VOD_PLAYSTATE_PLAYSUCC,		///< ���ųɹ�,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_SHAKEHAND_TIMEOUT= VOD_MSG_START + 30 ,  ///< ���Ź�����ÿ��50s���������һ�������ղ���ʱ�򣬷��������Ϣ
    EM_EW_MSG_VOD_PLAYSTATE_INVALID ,	///<
    ///<����Ϊ�����ӵ���Ϣ
    EM_EW_MSG_VOD_CONNECT_SUCCESS= VOD_MSG_START + 90	,	///< ��Ƶ�ɹ�
    EM_EW_MSG_VOD_CONNECT_FAILED,		///< ��Ƶʧ��
    EM_EW_MSG_VOD_NO_PROGRAME ,   ///< �޽�Ŀ
    EM_EW_MSG_VOD_LOAD_SUCCESS,   ///< ����PAT��PMT�ɹ�
    EM_EW_MSG_VOD_NOT_SUPPORT,    ///< ��Ŀ���Ͳ�֧��
    EM_EW_MSG_VOD_FRONTTS_STOP,  ///< ǰ��ͣ����������
    EM_EW_MSG_VOD_FRONTTS_RUN,   ///< ǰ�˻ָ�����
    EM_EW_MSG_VOD_PLAYSTATE_READY = VOD_MSG_START + 0x1000 , ///< ����Ԥ��,EPG�е��VOD�������Ӻ�����Ҫ����Ӧ�ÿ��ƵĲ��Ž�����������Ϣ.p0=PlayURL(const char*),ʱ�Ʋ��Ų����������Ϣ
    EM_EW_MSG_VOD_PLAYSTATE_PLAYFAILED,    ///< ����ʧ��,p0=PlayId(int)   
    EM_EW_MSG_VOD_CARD_VALID = VOD_MSG_START + 0x1100 ,   ///< ����Ч������Ŀ�����ȷ��ȡ������Ч��
    EM_EW_MSG_VOD_CARD_INVALID,   ///< ����Ч�����γ����������
    EM_EW_MSG_VOD_CARD_PREPARING  ///<���Ѿ����룬���ǻ�û�л�ȡ�����ţ��������첽��ȡ���ţ�������Ҫʱ��ϳ���CA����irdeto
}
CSEWVODMsgType_E;



/**
@brief �������������
@see CSEWVODOpenHomePage
    */
typedef enum
{
    EM_EW_UNDEFINED,                    ///< δ����
    EM_EW_VOD_EPG,                       ///< VOD IEPG������
    EM_EW_VOD_RTV_URL,                   ///< ���ʻطŷ���URL
    EM_EW_VOD_FAVORITE_URL,              ///< �ղط������URL
    EM_EW_VOD_PORTAL_URL,                ///< ��ҳ����URL
    EM_EW_VOD_UPGRADE_URL,               ///< IP��������URL
    EM_EW_VOD_SHORTCUT_URL               ///<  ��ݼ�URL
} CSEWVODServerType_E;

/**
@brief �ٻ�200 VOD Event��Ϣ���ݽṹ

�洢һ��Event
*/
typedef struct
{
    char aEventName[64];                  ///< ��Ŀ����,������������.���û�н�Ŀ����Ϣ�򷵻�.���뷽ʽ��ǰ�˾���������ǰ��ȷ�ϡ�
    int nStartTime;                       ///< ��Ŀ��ʼʱ��,ʱ����CSEWVODGetSoftTimeΪ��׼��λ,�㲥��ĿΪ0.
    int nEndTime;                         ///< ��Ŀ����ʱ��,ʱ����CSEWVODGetSoftTimeΪ��׼��λ,�㲥��ĿΪʱ��.
} CSEWVODEventInfo_S;

/**
@brief �ٻ�200 VOD EPG��Ϣ���ݽṹ

�洢���Event
@note nEventTotalCntΪʵ�ʵĽ�Ŀ������nEventAvailCntΪ��Ч�Ľ�Ŀ������С�ڵ���nEventTotalCnt��
���ڵ���nEventArrayLen��ֵ����ʵ�ʸ���С��Ӧ�����õ�nEventArrayLenֵʱ��nEventTotalCnt����nEventAvailCnt��
������ʵ�ʸ�����nEventArrayLenֵ���䣻��ʵ�ʸ�������Ӧ�����õ�nEventArrayLenֵʱ��nEventTotalCntΪʵ�ʸ�����
nEventAvailCnt����nEventArrayLen��ΪӦ�����õ�ֵ��
@code
Ӧ��ʹ����������:
CSEWVODEpgInfo_S *s_pEPGInfo;
//CSEWVODEventInfo_SΪ�䳤���飬����EVENTCOUNTΪӦ����Ҫ�Ľ�Ŀ������������ĳ���

if (s_pEPGInfo == NULL)
     s_pEPGInfo = (CSEWVODEpgInfo_S *)CSMalloc(sizeof(CSEWVODEpgInfo_S)+sizeof(CSEWVODEventInfo_S)*(EVENTCOUNT-1));

if(s_pEPGInfo != NULL)
{
	s_pEPGInfo->nEventArrayLen = EVENTCOUNT;
	s_pEPGInfo->nEventTotalCnt = 0;
	s_pEPGInfo->nEventAvailCnt = 0;
}
@endcode
@see CSEWVODGetEpgEvent
*/
typedef struct
{
    int nEventTotalCnt;  ///< EPG��Event���ܸ���
    int nEventAvailCnt;  ///< EPG����Ч��Event����
    int nEventArrayLen;  ///< EPG��asEvents����ĳ��ȣ���Ӧ��ָ��
    CSEWVODEventInfo_S asEvents[1];     ///< CSEWVODEventInfo_S������,�䳤���顣Ӧ�ø�����Ҫ����asEvents����Ĵ�С������nEventArrayLen��ֵ
} CSEWVODEpgInfo_S;


/**
@brief �ٻ�200 VOD Correlate��Ŀ��Ϣ���ݽṹ

�洢һ������Event��URL����Event��Ϣ
@see CSEWVODGetCorrelateEventByIndex
*/
typedef struct
{
    char aCorrelateUrl[1024];     ///< �洢��Ŀ��URL
    CSEWVODEventInfo_S sEventInfo;	   ///< ��Ŀ�Ļ�����Ϣ���������Ŀ���ƣ���ʼ�ͽ���ʱ��
} CSEWVODCorrelateEvtInfo_S;


/**
@brief ע��VOD��Ϣ�ص�����

ע���ٻ�200 VOD��ص���Ϣ��ͨ���ýӿ�֪ͨ��Ӧ��.
@param[in] pUserData �û�����,���ڻص���ԭ������
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_TRUE
@note �ص�������dwMsg������CSEWVODMsgType_E��ȡֵ
*/
CSUDI_BOOL CSEWVODRegisterCallback(CSEWCallback_F fnCallback, void* pUserData);




/**
@brief VOD���ٵ�½�ӿ�

����ɵ�¼VOD�����м�Ȩ�ȱ�Ҫ������һ����˵��ɺ�ɽ���iEPG���ɻ��VOD��Ϣ�ȡ�
�����ĵ�¼���̻�������ȡ�����룬�ڳɹ���ȡ������֮ǰ���㲥������ʧ�ܡ�
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_TRUE
*/
CSUDI_BOOL CSEWVODFastLogin(void);
/**
@brief VOD��½-��ȡ������ӿ�

�ɹ���Ȩ��ʹ�øýӿڻ�ȡ�����룬֮���ܽ��е㲥������
��ɵĵ�¼���̰�����Ȩ�ͻ�ȡ�������������衣
@note δ���������ʱ�ýӿڻ�ȥǰ�˻�ȡ�����룬�ѻ�ȡ�������ýӿڽ��᷵�ػ���������룬����Ҫ���»�ȡ����Logout��
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWVODGetAreaCode(void);

/**
    @brief VOD�˳���¼�ӿ�

    ������Ҫ�˳�VOD�����øýӿں󣬿�����ձ����VOD����֤��Ϣ��
    @return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
    @note ������CSEWVODFastLogin()��¼�ɹ�����Ҫ�����������������µ�¼ʱ���ɵ��ø�CSEWVODLogOut()�ӿڣ���յ�¼��Ϣ
    */
CSUDI_BOOL CSEWVODLogOut(void);



/**
@brief ��URL����һ��VOD��Ŀ

Ŀǰ֧�ֵ�URL���Ͱ���:
 - ����EPGҳ��������ʽ��URL,������ʽ��URLһ���ɰٻ�200���д��������ȡ���Զ���ʼ����
 - ֱ����ĿURL,����ֱ���ط����Ŀ�Ĳ���.����ʽΪ:"dvb://networkid.tsid.serviceid/starttime",�������ݾ�����16����,����Ҫ0x���,��dvb://a.8.f/49D02FEB
 ����starttime��ʾ��Ŀ�Ŀ�ʼʱ��,��ʱ���UDI��CSEWVODGetSoftTime�ӿڻ�ȡ,����ӵ�ǰʱ�俪ʼ���ſ�ʡ��starttime����.
@param[in] nPlayId ����Id,��ֵ���û������趨,Ψһ��ʶһ·VOD�Ĳ���ҵ��,����Ŀǰ��֧�ֵ�·���ŵ�VODϵͳ,��ȡDEFAULT_PLAY_ID����.
@param[in] pUrl Ŀ��URL
@param[in] nLength URL����
@param[in] nTimeout ��ʱʱ��,��λms,���鳬ʱʱ�����2000ms,������ʱ�����޷����ӷ������᷵��ʧ��
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@note nPlayIdʵ�������ڱ�ʶ֧�ֶ�·VOD��������µĲ�ͬ·����,�ٻ㲻������ȡֵ,
��nPlayId��ͬ�ĺ�һ��Play�����Ḳ��ǰһ��;��nPlayId��ͬ������Play���������໥Ӱ��.
����µ�nPlayId�����ڲ��ŵ�nPlayId��ͬ,VODϵͳ���޷������µĲ���ͨ��,�������᷵��ʧ��.
*/
CSUDI_BOOL CSEWVODPlayStream(int nPlayId, const char* pUrl, int nLength, int nTimeout);

/**
@brief ֹͣ����һ��VOD��Ŀ

ֹͣ��ǰ��VOD����,���Ͽ�����������
@param[in] nPlayId ����Id,�����벥��ʱ��һ��
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@note ���������ͷŸ�nPlayId��Ӧ����Դ,���ɹ����ñ�������,��ʹ�ڽ�֧��һ·���ŵ�VODϵͳ��,Ҳ�������µ�nPlayId������������
*/
CSUDI_BOOL CSEWVODStopPlay(int nPlayId);

/**
@brief ����һ��VOD��Ŀ�Ĳ����ٶ�

һ�����ڿ��˿���Ȳ���
@param[in] nPlayId ����Id,�����벥��ʱ��һ��
@param[in] nRate �����ٶ�
@note ��������1000ʱΪ1����ǰ��,2000Ϊ������ǰ��,Ϊ500��ʾ����,С��0��ʾ����
@note һ�������֧��2,4,8,16��������,�����������ʲ�֧��,���Զ�ѡ����ӽ�������
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWVODSetPlayRate(int nPlayId, int nRate);

/**
@brief ��ͣ����һ��VOD��Ŀ

@param[in] nPlayId ����Id,�����벥��ʱ��һ��
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWVODPause(int nPlayId);

/**
@brief �ָ�һ��VOD��Ŀ��������

���ڴӿ��˿����ͣ��״̬�ָ�����������״̬
@param[in] nPlayId ����Id,�����벥��ʱ��һ��
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWVODResume(int nPlayId);


/**
@brief ����һ��VOD��Ŀ���ŵ�ʱ��

ֱ����ת��ָ��ʱ�䲥��,��Ҫ����VOD��Ŀ��ֱ�Ӷ�λ����
@param[in] nPlayId ����Id,�����벥��ʱ��һ��
@param[in] nTime ����ʱ��,��λ��
@note ʱ�ƽ�Ŀ��ʱ����CSEWVODGetSoftTimeΪ��׼��λ,�㲥��Ŀʱ���0��ʼ����
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@see CSEWVODTell
*/
CSUDI_BOOL CSEWVODSeek(int nPlayId, int nTime);

/**
@brief ��ȡһ��VOD��Ŀ�ĵ�ǰ����ʱ��

��CSEWVODSetTime��ʱ�����Ӧ,��Ҫ���ڻ��Ƶ�ǰ���Ž���,ʱ����
@param[in] nPlayId ����Id,�����벥��ʱ��һ��
@param[out] pnTime ����ʱ��,��λ��
@note ʱ�ƽ�Ŀ��ʱ����CSEWVODGetSoftTimeΪ��׼��λ,�㲥��Ŀʱ���0��ʼ����
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@see CSEWVODSeek
*/
CSUDI_BOOL CSEWVODTell(int nPlayId, int* pnTime);

/**
@brief ��ȡһ��VOD��Ŀ��EPG��Ϣ

��������VOD��Ŀ�Ŀ�ʼʱ��,����ʱ��,��Ŀ���ݵ�EPG��Ϣ
@param[in] nPlayId ����Id,�����벥��ʱ��һ��
@param[out] psEpg �洢��ĿEvent��Ϣ���ڴ�
@note �㲥��Ŀһ��ֻ��1��Event,����ָʾ��Ŀ�ܳ���
@return �ɹ�����CSUDI_TRUE,ʧ�ܻ����и����Event����CSUDI_FALSE
@note ÿһ�λ�ȡEPG��Ϣ,�����ܻᴥ����������Ľ���,������EPG��Ϣ.
*/
CSUDI_BOOL CSEWVODGetEpgEvent(int nPlayId, CSEWVODEpgInfo_S *psEpg);

/**
@brief��URL ��ȡһ��VOD��Ŀ��EPG��Ϣ

��������VOD��Ŀ�Ŀ�ʼʱ��,����ʱ��,��Ŀ���ݵ�EPG��Ϣ
@param[in] pUrl Ŀ��URL,�������CSEWVODPlayStream�ӿڵ�URLһ��
@param[out] psEpg �洢��ĿEvent��Ϣ���ڴ�
@note �㲥��Ŀһ��ֻ��1��Event,����ָʾ��Ŀ�ܳ���
@return �ɹ�����CSUDI_TRUE,ʧ�ܻ����и����Event����CSUDI_FALSE
@note ÿһ�λ�ȡEPG��Ϣ,�����ܻᴥ����������Ľ���,������EPG��Ϣ.
*/
CSUDI_BOOL CSEWVODGetEpgEventByURL(const char* pUrl, CSEWVODEpgInfo_S *ptEvent);

/**
@brief��URL��������ȡָ���Ĺ�����Ŀ��Ϣ

��������������Ŀ�Ŀ�ʼʱ��,����ʱ��,��Ŀ��URl����Ϣ
@param[in] pUrl Ŀ��URL,�������CSEWVODPlayStream�ӿڵ�URLһ��
@param[in] nIndex Ŀ������,Ϊ��Ҫ��õĹ�����Ŀ�����
@param[out] psCorrelateEvent �洢ָ��URL��ָ�������Ĺ�����Ŀ
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@note ÿһ�λ�ȡ������Ŀ��Ϣ,�����ܻᴥ����������Ľ���,�����¹�����Ϣ.
@note ���汾�иù�����Ŀ�������⣬���޸�
*/
CSUDI_BOOL CSEWVODGetCorrelateEventByIndex(const char* pUrl, int nIndex, CSEWVODCorrelateEvtInfo_S *psCorrelateEvent);


/**
@brief�������б�����ͻ����Value��Ϣ

��������������Ϣ������������Ӧ��ֵ
@param[in] pcParaKey ���õĹؼ���pcParaKey ȡֵ��ע��
@param[out] pcParaValue ��Ӧ�������͵�ֵ
@param[in] nParaValueLen ���ص�pcParaVale�ĳ���
@return �ɹ�����TRUE,ʧ�ܻ���û�ж�Ӧ���������ͣ�������������Ϣ�����ڷ���FALSE
@note  pcParaKey�Ŀ�ȡ��ֵ����:
@ note  "iEPG_URL"      	  	   ��ʾ  iEPG����url��http://domain:port domain��������IPV4��ַ��Ҳ������������ʽ
@ note  "NTP_SERVER"   	   	   ��ʾ ʱ��ͬ����������ַ
@ note   "RTV_URL"             	   ��ʾ  ���ʻط�ҵ�����url
@ note    "FAVORITE_URL"   	   ��ʾ �ղط������url
@ note    "SEARCH_URL" 		   ��ʾ �����������url
@ note    "PORTAL_URL"  		   ��ʾ��ҳ����url
@ note   "UPGRADE_URL"	    	   ��ʾIP��ʽ����������URL
@ note   "SHORTCUT_URL"         ��ʾ��ݼ�URL����http:// domain:port/shortcut?domain��������IPV4��ַ��Ҳ������������ʽ��
@ note   "TIMESHIFT_PROVIDER"  ��ʾ��չ�ֶΣ���ʶʱ��ҵ���ܿ��ṩ�������û��ǣ�浽������ʱ��ҵ��⣬���������á���Ҫ����ʵ����Ŀ���������Ƿ����øĲ�����
@ note  "TTV_SKIPTIME" 		��ʾʱ��ҵ�����������ʱ��Ρ���ʱ��ҵ���ȡ������Ŀ����ң�����ϰ�����ҳ������ҳ��ʱ��ÿ��һ��ʱ������TTV_SKIPTIME����ֵΪ��λ������������λΪ���ӡ�
@ note  "TTV_MAXSILENTTIME" ��ʾ  ʱ��ҵ�����Ĭ�ڡ����ʱ��ҵ���������ų������Ĭ�����м�û���κβ�������STB��ʾ�û��Ƿ�����طţ������ʾ�������˲��������Զ��˳�ֱ���طŽ���ֱ��״̬����λΪСʱ
@ note  "BTV_PERIODTIME"    ��ʾ �ط�ҵ������ط���Ч�ڡ��ӵ�ǰʱ���������ֻ�ܻطŶ���ʱ��ǰ�Ľ�Ŀ��������Ч��ǰ�����ݲ����ٻطţ�����λΪСʱ
@�Ƽ�ʹ��
*/
CSUDI_BOOL CSEWVODGetParameterInfobyKey(char * pcParaKey, char *pcParaValue, int nParaValueLen);
/**
@brief��ȡ��֤�����еĴ�����

@param[out] nErrorCode ������Ϣ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note  ������Ϣ���Ӧ�Ĵ�����Ϣ�б�:
- 0000������ɹ� ��¼�ɹ�
- 0602���û���������Ϊ��
- 0611����¼��������Ϊ��
- 0612���û�ǩ������Ϊ��
- 0613�������������Ϊ��
- 0614���û���������Ϊ��
- 0616���û��ı�ʶid����Ϊ��
- 0617���û���ʶid��������
- 0619��Ӳ���ṩ���̲���Ϊ��
- 0622���û��Ľӿڰ汾����Ϊ��
- 0623���ӿڰ汾����
- 0633������ʧ��
- 0650���ն��û���¼ʧ��
- 0661��ָ���ն��û�������
- 0662��ָ���û������ݿ��в�����
- 0665���û����°汾ֵ������
- 0668���ն��û���û�м�����ȼ���
- 0669�������������
- 0671�����ǩ��ʧ��
- 0676���汾��Ϣ����Ϊ��
- 0678��Stb�����ṩ�߲���Ϊ��
- 0690��3.1��֤�ؼ������ô���
- 0691���û���Ψһ
- 0692����������sic,stb,alies,mac�ֶ���ȡ����һ����Ϊ��ݱ�ʶ
*/
CSUDI_BOOL CSEWVODGetErrorInfo(int *nErrorCode);

/**
@brief ����ʶ��һ��URL�Ƿ�Ϊһ��֧�ֵ�VOD��Ŀ

��Ҫ�����ж�һ����Ŀ�Ƿ�֧��ʱ��,��������¿����ڵ㲥��ĿVOD�ĺϷ�����֤(���Ƽ�ʹ��).
@param[in] pUrl Ŀ��URL,�������CSEWVODPlayStream�ӿڵ�URLһ��
@param[in] nLength URL����
@return ֧�ַ���CSUDI_TRUE;��֧�ֻ��߳�����CSUDI_FALSE
@note �ýӿ�ֻ���м򵥼��,����TRUE������100%��֤�ܲ���
@see CSEWVODPlayStream
*/
CSUDI_BOOL CSEWVODIdentifyStream(const char* pUrl, int nLength);


/**
@brief VOD��½�ӿ�
���ڽ��г�ʼ���ɹ���,��ʽʹ���κ�VOD����ǰ��׼������.
һ����˵���ڸù�������������������֤��������.
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@deprecated �Ƽ�ʹ��CSEWVODFastLogin ��CSEWVODGetAreaCode��ʵ�ִ˹���
*/
CSUDI_BOOL CSEWVODLogin(void);

/**
@brief ��VOD�����ҳ

�ðٻ�200�������VOD���ҳ����ҳ
@param[in] eServerType ����������
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@deprecated �Ƽ�ʹ��CSEWVODGetParameterInfobyKey(),���ʹ�ã������ȵ���CSEWExternAddVOD
*/
CSUDI_BOOL CSEWVODOpenHomePage(CSEWVODServerType_E eServerType);

/**
@brief�������б�����ͻ����Value��Ϣ

��������������Ϣ������������Ӧ��ֵ
@param[in] eParaKey ������Ϣ�����ͣ��μ�CSEWVODPARAType_E
@param[in] nParaValueLen ���ص�pcParaVale�ĳ���
@param[out] pcParaValue ��Ӧ�������͵�ֵ
@return �ɹ�����CSUDI_TRUE,ʧ�ܻ���û�ж�Ӧ���������ͣ�������������Ϣ�����ڷ���CSUDI_FALSE
@note ��CSEWVODServerType_E��������EM_EW_VOD_EPG�����᷵��pcParaValueֵ������ֱ�Ӵ�ҳ�棬��CSEWVODOpenHomePage()�Ĺ�����ͬ
@deprecated �ýӿڲ��Ƽ�ʹ�ã��Ƽ�ʹ��CSEWVODGetParameterInfobyKey
*/
CSUDI_BOOL CSEWVODGetParameterInfo(CSEWVODServerType_E eParaKey, char *pcParaValue, int nParaValueLen);


/**
@brief ��ȡ��ǰϵͳʱ��

@return  �ɹ�������1970��1��1���賿�������ڵ�������,ʧ�ܷ���0
@note ��δ����ʱ��֮ǰ,��Ϊ����ʱ��Ϊ1970��1��1���賿
@note ��ʱ����VOD����������ͬ����������VOD��ص�ʱ���ʹ�ø�ʱ�䡣
*/
int  CSEWVODGetSoftTime(void);


/**
@brief ����ʶ��һ��URL�Ƿ�Ϊһ���Ѿ�������ʱ�ƽ�Ŀ

��Ҫ�����ж�һ����Ŀ�Ƿ��Ѿ�������ʽΪ"dvb://networkid.tsid.serviceid"
@param[in] pcUrl Ŀ��URL,�������CSEWVODPlayStream�ӿڵ�URLһ��
@param[in] nLength URL����
@return �Ѿ���������CSUDI_TRUE;δ�������߳�����CSUDI_FALSE
@see CSEWVODPlayStream
*/
CSUDI_BOOL CSEWVODIsOrderd(const char* pcUrl, int nLength);
#ifdef __cplusplus
}
#endif

/** @} */

#endif//_CS_EW_VOD_H_

