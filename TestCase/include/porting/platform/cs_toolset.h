/** @addtogroup CSTOOLSET
@{
*/
#ifndef _CS_TOOLSET_H_
#define _CS_TOOLSET_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "cs_sysvideo.h"
#include "cs_sysaudio.h"
#include "udi_toolset.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ����ϵͳ�ڴ���䣬���ST71XX ��ST5197ϵ�� ��OS21����ϵͳ*/
typedef struct _CSSYSAdvancedParam
{
	BOOL				m_bSupportHDDecoder;		/**< �����Ƿ�֧��MPEG-2 MP@HL*/
	BOOL				m_bDualOSDBuffer;			/**<������,˫���棬Ĭ��Ϊ˫����(ָͬʱ֧�ֵ����棬˫����Ļ���)	*/

	DWORD				m_dwMemoryStartAddr;		/**< ���������ڴ���ʼ��ַ*/
	DWORD				m_dwMemoryTotalSize;		/**< �����ڴ�Ĵ�С*/

	DWORD*				m_pdwMemCacheStartAddr;		/**< Ignore,No used*/
	DWORD*				m_pdwMemCacheEndAddr;		/**< Ignore,No used*/
	DWORD*				m_pdwMemCacheCount;			/**< Ignore,No used*/
	
	DWORD*				m_pdwAVMemPartitionStartAddr;	/**< Ignore,No used*/
	DWORD*				m_pdwAVMemPartitionEndAddr;		/**< Ignore,No used*/
	DWORD*				m_pdwAVMemPartitionCount;		/**< Ignore,No used*/
	BOOL 				m_bSupportHDDecoder_H264;		/**< �����Ƿ�֧�� AVC HP@L4.1*/
} CSSYSAdvancedParam;

/**@brief ����ϵͳ�ڴ���䣬���ST71XX ��ST5197ϵ�� ��OS21����ϵͳ*/
typedef struct _CSSYSAdvancedParamEx
{
	CSSYSAdvancedParam  m_stSysAdvancedParam;	/**<CSSYSAdvancedParam */
	BOOL  				m_bPvrSupport;     		/**<PVR application support*/
	BOOL  				m_bPipSupport;     		/**<PIP application support*/
	BYTE  	       		m_ucPtiNumber;     		/**<multi pti support*/
	DWORD				m_dwReserve;			/**<0---��֧��NDS CA,1---֧��NDS CA*/
 } CSSYSAdvancedParamEx;
 
/**@brief ����ϵͳ�ڴ���䣬���ST71XX ��ST5197ϵ�� ��OS21����ϵͳ*/
typedef struct _CSBootParamEx
{
	BOOL				m_bDualOSDBuffer;		/**<������,˫���棬Ĭ��Ϊ˫����(ָͬʱ֧�ֵ����棬˫����Ļ���)	*/
	DWORD				m_dwColorSize;			/**<8λɫ��16λɫ��Ĭ��Ϊ8λɫ	*/
	DWORD				m_dwMemoryTotalSize;	/**<Ĭ��Ϊ8m�ڴ�*/
}CSBootParamEx;


/********************ϵͳ��ʼ��������***********************/
/**
@brief ��ֲ�����ýӿڣ�����ʱ���ɸ�eCfgType����,�����CSSYSCfg_E����

�ýӿڿɱ����ö�Σ�ͬһ���͵����������һ�εĵ���Ϊ׼
@param[in] eCfgType ��������,�����CSSYSCfg_E����;
@param[in] dwDevice ��Ҫ���õ��豸ID,��cs_device.h,�����eCfgType�޶�Ӧ���豸ʱ,����CSDEVICE_IGNORE;
@param[in] dwValue  ���ò���,���庬����eCfgType����;
@param[in] dwRserved ��������,Ŀǰ��ʹ�ã�����ΪNULL��
@return ���óɹ�����TRUE ������ʧ�ܷ���FALSE
*/
BOOL CSSYSSetCfg(CSSYSCfg_E eCfgType, DWORD dwDevice, DWORD dwValue,DWORD dwRserved);

/**
@brief ��ֲ���ʼ���ӿڣ�ʵ��ϵͳ�ķ������֣�����������ϵͳ��ϵͳ����ʱ��һ��������

@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�����NULL��
@return ���óɹ�����TRUE ������ʧ�ܷ���FALSE
*/
BOOL CSSYSChipBoot(HCSHANDLE hUserData);

/**
@brief ��ֲ��ϵͳ��ʼ���ӿ�

�ýӿ�ֻ�ɱ�����һ�Σ�������CSSYSChipBoot��CSSYSChipBootEX�����
@param[in] pstSysInfo ��Ӧ�����õ�STB��ϵͳ��Ϣ������ṹ�ο�CSSYSInfo�����ݽṹ��
@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�����ΪNULL��
@return �豸�����
@note 
- ��ʼ���������豸��������оƬ�Ĳ�ͬ���Է���clkrv,dma, audio, video, osd, pti etc.����Դ��
	�����ѵײ��豸��֧�ֵ�STB�ķ������ͣ�DVB �� IPTV������Ƶ�����������ʽ���ã�
	������Ƶ�����ʽ���á�ͨ���ú����ӿ�֪ͨ�ײ㱣����������I2c,uart,gpio���豸��
	ʼ��֮�����.�û�����Ҫ���ĸ��豸�ĳ�ʼ��˳�򡣶��ڲ���pstSysInfo���ϲ�Ӧ��
	�����ȷ�Ͼ���������ݣ��ɽ��ò�����NULL����ֲ���ʱ����Ԥ�õ�ȱʡֵ��
	��ֲ��Ҫ��֤��NULL����Ĵ���
*/
HCSHANDLE CSSYSChipInit(CSSYSInfo* pstSysInfo, HCSHANDLE hUserData);

/**
@brief ��ֲ��ϵͳ��ʼ���ӿڣ�CSSYSChipInit����չ

�ýӿ�ֻ�ɱ�����һ�Σ�������CSSYSChipBoot��CSSYSChipBootEX�����
@param[in] pstSysInfo ��Ӧ�����õ�STB��ϵͳ��Ϣ������ṹ�ο�CSSYSInfo�����ݽṹ��
@param[in] hUserData �ò�������ΪNULLҲ����ΪCSSYSAdvancedParamEx
@return �豸�����
@note 
- ��ʼ���������豸��������оƬ�Ĳ�ͬ���Է���clkrv,dma, audio, video, osd, pti etc.����Դ��
	�����ѵײ��豸��֧�ֵ�STB�ķ������ͣ�DVB �� IPTV������Ƶ�����������ʽ���ã�
	������Ƶ�����ʽ���á�ͨ���ú����ӿ�֪ͨ�ײ㱣����������I2c,uart,gpio���豸��
	ʼ��֮�����.�û�����Ҫ���ĸ��豸�ĳ�ʼ��˳�򡣶��ڲ���pstSysInfo���ϲ�Ӧ��
	�����ȷ�Ͼ���������ݣ��ɽ��ò�����NULL����ֲ���ʱ����Ԥ�õ�ȱʡֵ��
	��ֲ��Ҫ��֤��NULL����Ĵ���
*/
HCSHANDLE CSSYSChipInitEx(CSSYSInfo* pstSysInfo, HCSHANDLE hUserData);

/**
@brief ���������Ƿ�֧��PVR
@param[in] bSupport  TRUE��ʾ֧��,FALSE��ʾ��֧��
@return ���óɹ�����TRUE ������ʧ�ܷ���FALSE
*/
BOOL CSSYSSetPvrSupport(BOOL bSupport);

/**
@brief �Զ���CSSYSEventMsg�¼������ȼ�

��CSSYSEventMsg������¼��������ȼ�����������ʱ,ʹ�øýӿڽ����Զ���ע��
�¼����ȼ������������е��Ⱥ�˳���ɸߵ���
���CSSYSEventMsg������¼������ȼ�������Ҫ��ʱ,������ñ��ӿ�
����ƽ̨��س�ʼ������Ժ����һ�Σ�����Ҫע��ĸ����¼����ͼ��������
@param[in] hHandle ���Զ����¼������ȼ�������Ƶ�豸���豸�����
@param[in] pstItems �¼�����(��)��������ʼ��ַ��
@param[in] nCount ����Ԫ�ظ���
*/
BOOL CSSYSInitEventStatusTable(HCSHANDLE hHandle, CSSYSEventItem * pstItems,  int nCount);

/**
@brief ��ô����豸������豸���������һ���Ϸ��豸���

@param[in] hInstance NULL ���� CSSYSChipInit�������ľ��
@return �豸�����
@note �������ΪNULL���򷵻��豸�������ͷ��������û�н�����������򷵻�NULL
@note �������һ���Ϸ������������������һ���Ϸ�������򷵻ظþ�������߷���NULL��
@note ��RM֧�ֵ��������
*/
HCSHANDLE CSSYSGetNextHandle(HCSHANDLE hInstance);

/**
@brief �����ֲ��ƽ̨�ӿڵİ汾��Ϣ

����ES,�Լ�video��audio�ĸ�����Ϣ
@param[out] pcVersionInfo ϵͳ��ǰ����ֲ��ƽ̨�ӿڵİ汾��Ϣ
@param[in] nSize ��ʾ�ַ�������󳤶�
@return �ɹ�:��ǰ�汾�ĳ���;ʧ��:0
@note �����ʽΪ���¼�����ʽ֮һ��
- "PLAT-R-���汾.�Ӱ汾.�Ķ����� ";Release Version 
- "PLAT--���汾.�Ӱ汾.�Ķ�����";  Debug Version
- "PLAT--���汾.�Ӱ汾.�Ķ�����";  Test Version 
*/
int  CSSYSGetPlatformVersion( char* pcVersionInfo, int nSize);

/**
@brief ��õ�ǰоƬ�İ汾������Ϣ

����ES,�Լ�video��audio�ĸ�����Ϣ
@param[out] pcVersionInfo ϵͳ��ǰ��оƬ�İ汾��Ϣ
@param[in] nSize ��ǰоƬ�汾�ŵĳ���
@return �ɹ�:��ǰ�汾�ĳ���;ʧ��:0
@note ����ƽ̨�ṩ�̵ײ�����ģ�鲻���и��£�ƽ̨��Ҫ��¼����ģ��ĸ���״̬���ṩ����ӿ������ϵͳ��ǰ��оƬ�İ汾���޸����
*/
int  CSSYSGetChipVersion( char* pcVersionInfo, int nSize);

/*-------------���涨��ֻ��Ϊ�˼���֮ǰ����ƽ̨,����Ҫʵ��-------------*/
/**
@brief ��ֲ���ʼ���ӿڣ�CSSYSChipBoot����չ�ӿڣ�STϵ�� OS21�汾��Ҫʵ�֣���������ʵ��Ϊ׮��������
@param[in] pBootParam ���ò���
@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�����NULL��
@return ���óɹ�����TRUE ������ʧ�ܷ���FALSE
*/
BOOL CSSYSChipBootEx(CSBootParamEx* pBootParam,HCSHANDLE hUserData);

/**
@brief Enable WatchDog������Ҫʵ��
@note ����Ҫʵ��
*/
void CSWatchDogEnable( void );

/**
@brief 	Disable WatchDog������Ҫʵ��
@note ����Ҫʵ��
*/
void CSWatchDogDisable( void );

/**
@brief 	Set WatchDog Reset Time in Seconds������Ҫʵ��
@note ����Ҫʵ��
*/
void CSWatchDogSetResetTime( DWORD dwResetTimeSec );

/**
@brief ��ô����豸������豸���������һ���Ϸ��豸���������Ҫʵ��

@param[in] hHandle �豸ͨ�������ϵͳ��ʼ���õ���
@param[in] bToBeLimited �Ƿ�����cpu�Ĵ���
@param[in] hUserData ��������������ΪNULL
@return ���óɹ�����TRUE ������ʧ�ܷ���FALSE
@note ����Ҫʵ��
*/
BOOL CSSYSLimitBandWidthOfCPU(HCSHANDLE hHandle,BOOL bToBeLimited,HCSHANDLE hUserData);

/**
@brief 	���ST710X����������mp3�Ĺ�����Ҫ���ã�����Ҫʵ��
@note ����Ҫʵ��
*/
BOOL CSSYSInitSoftMp3(HCSHANDLE hUserData);

/**
@brief оƬ�����Ĺ��Ӻ�������
@param[in] bStandbyStatus		TRUE:�������״̬;FALSE:�˳�����״̬
@param[in] pstStandbyParams	 ��CSSYSStandbyParams_S����
@param[out] pbSkipDefault �Ƿ����Ĭ�ϲ�����TRUE��ʾִֻ��Hook��������Ĳ�����FALSE��ʾͬʱִ��CSSYSChipStandbyԭ�в�����Ĭ��ΪTRUE
@return TRUE ��ʾ���Ӻ���ִ�гɹ���FALSE��ʾ���Ӻ���ִ��ʧ��
*/
typedef BOOL (*CSChipStandbyHookFunc_F)(BOOL bStandbyStatus, const CSSYSStandbyParams_S* pstStandbyParams, BOOL *pbSkipDefault);

/**
@brief ����оƬ�����Ĺ��Ӻ���

@param[in] fnHook ���Ӻ�������CSChipStandbyHookFunc_F����
@return ������óɹ������ص���TRUE���������ʧ���򷵻�FALSE
@note fnHook����ΪNULL����ʾȡ�����Ӻ���
*/
BOOL CSSYSSetChipStandbyHook(CSChipStandbyHookFunc_F fnHook); 


/**
@brief ��ȡ��оƬ��ID
@param[out] pdwChipID ��оƬ��id��
@return ������óɹ������ص���TRUE���������ʧ���򷵻�FALSE
*/
BOOL CSSYSGetChipID (DWORD* pdwChipID);

/**
@brief ��ȡ��оƬ��ID
@param[out] pdwChipID ��оƬ��id��
@param[out] pdwChipIDEx    ���ڸ߼���ȫ��Ҫ����λ����оƬ��id��
@return ������óɹ������ص���TRUE���������ʧ���򷵻�FALSE
*/
BOOL CSSYSGetChipIDEx(DWORD* pdwChipID,DWORD* pdwChipIDEx);

#ifdef  __cplusplus
}
#endif

#endif

/** @} */
