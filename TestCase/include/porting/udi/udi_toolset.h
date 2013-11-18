/**@defgroup CSTOOLSET toolset ��ֲ��ϵͳ��ʼ�������úͿ���
@brief ��ֲ��ϵͳ��ʼ�������úͿ��ƽӿ�
- ϵͳ��ʼ��������ϵͳ��������Ҫ��ģ���ʼ�������������
- ���ù��ܣ�ϵͳ��ʼ��ʱ�����ṩ���û������ýӿڣ�������������ѡ��
- �������ܣ��ṩreboot����

@version 0.3 2009/8/5
- ��ȷģ�鹦��
- ��ȷ����Ҫʵ�ֵĽӿ�
- ����ϵͳ���̺͵���ʾ������
@version 0.2 2008/10/29
- ��Ӳ�����ϸ����
@version 0.1 2008/10/22
- ��ʼ�汾
@{
*/
#ifndef _UDI_TOOLSET_H_
#define _UDI_TOOLSET_H_

#include "udi_typedef.h"
#include "udi_public.h"
#include "udi_video.h"
#include "udi_audio.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief system status information*/
typedef enum _CSSYSStatusInfo	
{	
	CSSYS_STATUS_BOOT,				/**< boot the Kernel*/
	CSSYS_STATUS_RUN,				/**< STB system runing on normally mode*/
	CSSYS_STATUS_HALT,				/**< STB system halt ��Ŀǰ��ʹ�ô�״̬*/
	CSSYS_STATUS_REBOOT,			/**< reboot the Kernel */
	CSSYS_STATUS_STANDBY			/**< Standby the STB system*/
}CSSYSStatusInfo;

/**@brief system information*/
typedef struct _CSSYSInfo
{
	CSSYSStatusInfo		SysStatusInfo;	/**< system status information: boot/run/halt/reboot*/
	CSSYSServiceType	SysServiceInfo;	/**< play status: DVB/IPTV*/

	CSSYSAUDInputInfo	InputAUDInfo;	/**< Audio input stream information*/
	CSSYSVIDInputInfo	InputVIDInfo; 	/**< Video input stream information*/
	CSSYSAUDOutputInfo 	OutputAUDInfo;	/**< Audio ouput data information*/
	CSSYSTVOutputInfo 	OuputTVInfo;	/**< Video output data information*/
} CSSYSInfo;

typedef struct _CSSYSStandbyParams_S
{
	BOOL	m_bReal; /**< �Ƿ�Ϊ����� */
	DWORD 	m_dwStandbyTime; /**< ��ʱ����ʱ�䣬�����������Ч */
	DWORD 	m_dwReserved1;  /**< ��չ������Ŀǰ����Ϊ0 */
	DWORD 	m_dwReserved2; /**< ��չ������Ŀǰ����Ϊ0 */
}CSSYSStandbyParams_S;

/**
@brief ϵͳ������ӦΪӲ����Ӳ����

@param[in] hReserved �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�����ΪNULL��
@return ������óɹ������ص���TRUE���������ʧ���򷵻�FALSE
@note �Ƽ�ʹ��watckdog��ʽ����
*/
BOOL CSSYSChipReboot(HCSHANDLE hReserved);

/**
@brief ʵ��ϵͳ�Ĺػ�,Ŀǰ��ʵ��stub������

@param[in] hReserved �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�����ΪNULL��
@return ������óɹ������ص���TRUE���������ʧ���򷵻�FALSE
@note Ŀǰ����ʵ�֣�ʵ��ϵͳ�Զ��ػ����ܣ���ҪӲ��֧��
*/
BOOL CSSYSChipHalt(HCSHANDLE hReserved);	

/**
@brief ʹϵͳ�������״̬�����˳�����״̬��

@param[in] bStandbyStatus TRUE:�������״̬;FALSE:�˳�����״̬��ΪFALSE��pstStandbyParams������Ч������ΪNULL
@param[in] hStandbyParams ΪCSSYSStandbyParams_S�ṹ��ָ�룬ΪNULL���ٴ������������CSSYSStandbyParams_S����
@return ������óɹ������ص���TRUE���������ʧ���򷵻�FALSE
@note ��Ҫ��Ϊ�ٴ���ʱ���ر�AV DAC��HDMI�ȸ�����ӿ�
*/
BOOL CSSYSChipStandby(BOOL bStandbyStatus, const CSSYSStandbyParams_S* pstStandbyParams);	

/**
@brief ���ϵͳ��ǰ�ĸ���״̬��Ϣ

����ES,�Լ�video��audio�ĸ�����Ϣ
@param[in] hHandle �豸ͨ�������ϵͳ��ʼ���õ�
@param[out] pstSysInfo ϵͳ��ǰ����Ϣ
@param[in] hReserved �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�����ΪNULL��
@return ������óɹ������ص���TRUE���������ʧ���򷵻�FALSE
*/
BOOL CSSYSGetInfo(HCSHANDLE hHandle, CSSYSInfo* pstSysInfo, HCSHANDLE hReserved);

/*-------------���½ӿڲ�����ƽ̨�����ṩ����-------------*/

/**
@brief UDI��ʼ���ӿڣ��ӿڲ�����ƽ̨�����ṩ���֣�����Ҫʵ��

ͳһ��ʼ��UDI�ж����ģ�飬���ú�UDI����
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note CITV3.0��ʼ��ʱ����ñ��������г�ʼ������
@note �ýӿڵ�ʵ�ָ�����Ŀ��ͬ����ͬ��һ��������ɼ����߸�����Ŀ�����������ƽ̨����ʵ��
@note �ӿڲ�����ƽ̨�����ṩ���֣�����Ҫʵ��
*/
BOOL CSUDIInit(void);

/**
@brief ��ȡϵͳ������Ϣ���ӿڲ�����ƽ̨�����ṩ���֣�����Ҫʵ��

��MAC��ַ��IP��ַ��VOD��������ַ����Ϣ�����ƻ�������ϵͳ
@param[in] key Ψһ��ʶ������Ϣ���ַ����ؼ���
@return �ɹ����ض�Ӧ���ַ���ֵ��ʧ�ܷ���NULL
@todo ������Ҫ��Щ��Ϣ��ȷ��
@note �ýӿڵ�ʵ�ָ�����Ŀ��ͬ����ͬ���ɼ����߸�����Ŀ�������ʵ��
@note �ӿڲ�����ƽ̨�����ṩ���֣�����Ҫʵ��
*/
const char* CSUDIGetEnv(const char* key);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif

