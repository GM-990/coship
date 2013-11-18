#ifndef _CS_IPTOOL_H_
#define _CS_IPTOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "udi2_socket.h"

#define MAX_TTL		256
#define HOST_NAME_LEN 64
typedef enum
{
    EM_DNS_PRIORITY = 0,//�������Ȳ��ԣ�������ж��DNS�������������ȷ�����һ�ν���������������̨������
    EM_DNS_UNPRIORITY,//���������Ȳ��ԣ�ÿ�δӵ�һ��DNS����
    EM_DNS_UNDEFINED
} CSDNSStrategyType_E;

typedef struct 
{
    char acHost[HOST_NAME_LEN];		 ///< ����ping��ϵ����������ַ��
    int nNumberOfRepetitions;		 ///< �ڱ�����֮ǰ��ping����ظ��Ĵ�����
	int nTimeout;					 ///< �ú����ʾ��ping��ϳ�ʱʱ�䣻
	int nDataBlockSize;			     ///< ÿ��ping����͵����ݴ�С�����ֽ�Ϊ��λ;
	int nTTL;						 ///< TTLֵ��������ʱ��;
	int nDSCP;					     ///< ���԰�������DiffServ����㣬Ĭ��ֵΪ0��
}CSPINGAttribute_S;

typedef struct 
{
	char acHost[HOST_NAME_LEN];			 ///< Ӧ��ping��ϵ�������ַ��
	int nTotalCount;				     ///< �������ping�����е��ܴ�����
	int nSuccessCount;			         ///< �������ping�����гɹ��Ĵ�����
	int nFailureCount;				     ///< �������ping������ʧ�ܵĴ�����
	int nAverageResponseTime;	         ///< �Ժ���Ϊ��λ�����һ��ping�������гɹ���Ӧ��ƽ��ʱ�䣻
	int nMinimumResponseTime;	         ///< �Ժ���Ϊ��λ�����һ��ping�������гɹ���Ӧ�е����ʱ�䣻
	int nMaximumResponseTime;	         ///< �Ժ���Ϊ��λ�����һ��ping�������гɹ���Ӧ�е��ʱ�䣻
}CSPINGResult_S;


typedef struct 
{
	char acHost[HOST_NAME_LEN];		  ///< ����·����ϵ����������ַ��
	int nTimeout;					  ///< �ú����ʾ��·����ϳ�ʱʱ�䣻
	int nDataBlockSize;			      ///< ÿ��·����Ϸ��͵����ݴ�С;
	int nMaxHopCount;			      ///< ���͵Ĳ������ݰ����������(���TTL��)��Ĭ��Ϊ30����
	int nDSCP;					      ///< ���԰�������DiffServ����㣬Ĭ��ֵΪ0��
}CSTRACERTAttribute_S;


typedef struct 
{
	int nTTL;            ///< ����ʱ��
	char acHopHost[HOST_NAME_LEN];  ///< ·�ɹ����о���������IP��ַ
}CSTRACERTHopHost_S;


typedef struct 
{
	char acHost[HOST_NAME_LEN];					    ///< ����·����ϵ�������ַ��
	int nResponseTime;			            ///< �Ժ����ʾ�����һ��·���������Ե���Ӧʱ�䣬����޷���������·�ɣ���Ĭ��Ϊ0��
	int nNumberOfRouteHops;		            ///< ���ڷ���·�ɵ�����������޷�����·�ɣ���Ĭ��Ϊ0��
	CSTRACERTHopHost_S gHopHost[MAX_TTL];	///< ·��·����
}CSTRACERTResult_S;


/**@brief ping�ص���Ϣ���Ͷ���*/
typedef enum 
{
    EM_PING_SUCCESS = 0,     ///< ping�ɹ�
    EM_PING_TIMEOUT,          ///< ping��ʱ
    EM_PING_FAILURE            ///< ping δ֪ԭ��ʧ��
}CSPingState_E; 


/**
@brief Ping�¼��ص�����ԭ�Ͷ���

@param[in] ePingState �����ص���ping��״̬����,��ϸ��CSPingState_E��˵��
@param[in] pBackStr �ص���Ϣ���ݣ�ping�Ľ��
@param[in] pUserData �û��Զ�������
*/
typedef BOOL (*CSOnPingStateChange_F)(CSPingState_E ePingState, char *pcBackStr,void *pUserData);


 /**
@brief ע��pingģ��ص�

@param[in] dwUserData  �����û�������Ϣ��
@param[in] fnOnPingStateChange  �ص����������
@return ����ɹ�����CSUDI_TRUE, ʧ��CSUDI_FALSE
@note
- �ص�������ע���������֧��20��
*/
BOOL CSPingAddCallback(DWORD dwUserData, CSOnPingStateChange_F fnOnPingStateChange);


 /**
@brief ɾ��ָ����ping �ص�������ע��

@param[in] dwUserData �û�������Ϣ��
@param[in] fnOnPingStateChange �ص����������
@return�ɹ�����CSUDI_TRUE ����ûص�����������û��ע�ᣬ������CSUDI_FALSE
*/
BOOL CSPingRemoveCallback(DWORD dwUserData, CSOnPingStateChange_F fnOnPingStateChange);


/**
@brief ���Ե�������һ̨����������·����·������������

@param[in] psTracertAttribute,�������tracert����Ҫ�Ĳ���
@param[out] psTracertResult ,�������tracert�Ľ��
@return      �ɹ�����0��ʧ�ܷ���-1                      
*/
int CSTracert(CSTRACERTAttribute_S* psTracertAttribute, CSTRACERTResult_S * psTracertResult);


/**
@brief ������һ̨�����Ƿ񵽴

@param[in] psPingAttribute,�������ping����Ҫ�Ĳ���
@param[out] psPingResult �������ping�Ľ����
@return      �ɹ�����0��ʧ�ܷ���-1                      
*/
int CSPingStat(CSPINGAttribute_S *psPingAttribute,CSPINGResult_S *psPingResult);


/**
@brief ������һ̨�����Ƿ񵽴

@param[in] pcPingStr ��ʾping �����У��磺"ping -n 8  192.168.110.90"
@param[in] pcBackStr �����ַ����ĳ��ȣ���С��512�ֽ�
@param[in] uBackStrLen ��ʾping�������󣬵ȴ�Ӧ���ʱ��,��λms
@param[out] uSleepTime �������ping�Ľ�����ռ����û����룬��С��512�ֽڡ�����磺"Reply from 192.168.1.1,  ��.. time<5ms  "
@return      �ɹ�����0��ʧ�ܷ���-1                      
*/
int CSPing(char * pcPingStr,char * pcBackStr,unsigned int uBackStrLen,unsigned int  uSleepTime);
/**
@brief ������һ̨�����Ƿ񵽴

@param[in] psPingAttribute,�������ping����Ҫ�Ĳ���
@param[out] phPingHandle,��������ping���
@return      �ɹ�����0��ʧ�ܷ���-1                      
@note  �첽ʵ��ping������ping�Ľ����:�ɹ����߳�ʱ��ͨ���ص��׸�Ӧ��
*/
int CSPingAsyn(CSPINGAttribute_S *psPingAttribute, HCSHANDLE *phPingHandle);


/**
@brief  ȡ��ping������

@param[in] hPingHandle,ping���,ȡCSPingAsyn��������
@param[out] �����ǰ��ping����������ping�����Ѿ������������0���������1
@note �ýӿڵ��ú�,CSPingAsyn �е��õ�ping���������ж�
@return      �ɹ�����0��ʧ�ܷ���-1              
*/
int CSCancelPing(HCSHANDLE hPingHandle,int * pnState);


/**
@brief ��鱾��IP��ַ�Ƿ������������г�ͻ

@param[in]  pcEthName �����豸�����磺"eth0"  
@param[in]  uInAddr �����IP��ַ�������ֽ����ַ
@param[in]  uTimeOut �ȴ�arpӦ���ʱ�䣬��λms  
@return      �ɹ�����0��ʧ�ܷ���-1                      
*/
int  CSArpSniffer(const char * pcEthName, const unsigned int uInAddr, const unsigned int uTimeOut);


/**
@brief ����CSDNSGetHostByName������Ĭ�ϳ�ʱʱ�䣬��λms

@param[in]  dwTimeout  CSDNSGetHostByName������Ĭ�ϳ�ʱʱ�䣬��λms. �ɸ��ݻ�����ʵ��������ú���ʱʱ��
@return ���óɹ��򷵻�0,���dwTimeout = 0, �����ڲ�Ĭ�ϳ�ʱ��������-1
*@note �ڲ� Ĭ�ϳ�ʱΪ2000ms  
*/
int CSDNSSetDefaultTimeout(unsigned int dwTimeout) ;


/**
@brief ������������gethostbyname��ͬ���ǿ������ó�ʱ����Ӧÿ������������

@param[in] pcName  Ҫ����������, ��"www.baidu.com",Ҳ������IP��ַ
@param[in] dwTimeout  ������ʱʱ��, ��λms  
@return  �����ɹ��򷵻ؽ��CSUDIHostent_S*�����򷵻�CSUDI_NULL
*@note   dwTimeout Ϊ�ȴ�һ��DNS������Ӧ���ʱ�䣬���������n��DNS�������������ȴ���ʱ��Ϊn*dwTimeout(ms)
*/
CSUDIHostent_S * CSDNSGetHostByNameWithTimeout(const char* pcName,unsigned int dwTimeout);


/**
@brief ����������ʹ��CSDNSSetDefaultTimeout ���õ�Ĭ�ϳ�ʱ

@param[in] pcName  Ҫ����������, ��"www.baidu.com",Ҳ������IP��ַ
@return  �����ɹ��򷵻ؽ��CSUDIHostent_S*�����򷵻�CSUDI_NULL
*@note   ���˳�ʱʱ��ʹ��Ĭ�ϳ�ʱ���⣬����ͬCSDNSGetHostByNameWithTimeout
*/
CSUDIHostent_S *  CSDNSGetHostByName(const char* pcName); 


/***********************************************************
* ����: CSDNSGetHostByNameR
* ����: ʹ��CSDNSSetDefaultTimeout ���õ�Ĭ�ϳ�ʱ
* ����:   
*     name :  Ҫ����������, ��"www.baidu.com",Ҳ������IP��ַ
*     
*     buf:   ָ��һ��buf�ռ䣬����������ݴ����buf��
*     buflen: buf ��С 
* 
*  ע��:
*     ������n��DNS�������������ȴ���ʱ��Ϊn*�����õ�Ĭ�ϳ�ʱʱ��
*     ����ʱʱ���⣬������ͬCSDNSGetHostByNameWithTimeoutR
*          
* ���: 
*     result_buf: ָ��һ�������ṹ,��Ϊ�������
*     result:������Ľ������ȡ�������ṹ��ָ�룬��Ϊ�������
*     h_errnop:������롣
* ˵��:
*     result_buf��result������Ϊ���������result_bufָ��buf��
*     ��result��ָ��ָ��buf��������ʽ����ͬ��Ϊ�û��ṩ�����������ʽ
* ����ֵ: 
*       �����ɹ�0
*       ���򷵻ش������
*     
************************************************************/
int CSDNSGetHostByNameR(const char * name,
                           CSUDIHostent_S * result_buf,
                           char * buf, unsigned int buflen,
                           CSUDIHostent_S ** result,
                           int * h_errnop);



/***********************************************************
* ����: CSDNSGetHostByNameWithTimeoutR
* ����: ����������֧�ֶ���������,��Ӧ��׼����gethostbyname_r
* ����:   
*     name :  Ҫ����������, ��"www.baidu.com",Ҳ������IP��ַ
*     buf:   ָ��һ��buf�ռ�
*     buflen: buf ��С 
*     dwTimeout : ������ʱʱ��, ��λms  
*     
*  ע��:
*     dwTimeout Ϊ�ȴ�һ��DNS������Ӧ���ʱ�䣬���
*     ������n��DNS�������������ȴ���ʱ��Ϊn*dwTimeout(ms)
*          
* ���: 
*     result_buf: ָ��һ�������ṹ������������ݴ���buf�У�ָ��buf
*     result:������Ľ������ָ��ָ��buf
*     h_errnop:�������
* ˵��:
*     result_buf��result������Ϊ���������result_bufָ��buf��
*     ��result��ָ��ָ��buf��������ʽ����ͬ��Ϊ�û��ṩ�����������ʽ
* ����ֵ: 
*       �����ɹ�0
*       ���򷵻ش������
*     
************************************************************/
int CSDNSGetHostByNameWithTimeoutR(const char * name,
                           CSUDIHostent_S * result_buf,
                           char * buf, unsigned int buflen,
                           CSUDIHostent_S ** result,
                           int * h_errnop,unsigned int dwTimeout);




/***********************************************************
* ����: CSDNSSetCache
* ����: �����Ƿ����������������,Ĭ��Ϊ��������������ƣ�����֮��
* ֻҪ���������������´ξͲ����ٽ��������¿����Ż��ٽ���
* ����: bFlagΪTRUE��ʾ ����CACHE���ƣ�����ΪFALSE��ʾ��ʹ�û��� ������Ϊ      
* FALSE֮��ÿ�ζ��������������Ӱ�������ҳ���ٶ�         
* ���: ��
* ˵��:
* ����ֵ: 
*      ���óɹ�����0
*       ���򷵻ش������
*     
************************************************************/
int CSDNSSetCache(CSUDI_BOOL bFlag);

/***********************************************************
* ����: CSDNSSetStrategyType
* ����: ����DNS�ķ��ʲ��ԣ�����ж��DNS�������ò�ͬ�ķ���DNS����������
* ����: eDNSStrategyType �������DNS�������Ĳ�������
* EM_DNS_PRIORITY��ʾ���ȷ�����һ�ν�������������DNS������;
* EM_DNS_UNPRIORITY��ʾ����֮ǰ�������������Σ�ÿ�ζ��ӵ�һ��DNS����,
* Ĭ�ϲ���EM_DNS_PRIORITY��ʽ   
* ���: ��
* ˵��:
* ����ֵ: 
*      ���óɹ�����0
*       ���򷵻�-1
*     
************************************************************/
int CSDNSSetStrategyType(CSDNSStrategyType_E eDNSStrategyType);


#ifdef __cplusplus
}
#endif

#endif

