
#ifndef CSSHELL_TOPCA_H
#define CSSHELL_TOPCA_H

#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,pls check your code!
#endif
#define CSSHELL_CA_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "csshell_ca.h"
#include "cstopcainterface.h"

typedef struct 
{
	int 			m_nSize;
	CSShellCaType_E m_eCaType;
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�	
	
	INT (*m_fnGetMailNum)(MailInfo_S *pstMailInfo);
	INT (*m_fnGetMailTitle)(INT nIndex, MailTitle_S *pstMailTitle);
	INT (*m_fnGetMailContent)(INT nIndex, MailContent_S *pstMailContent);
	INT (*m_fnGetMailIntegerInfoByEmailId)(WORD wEmailId, MailInteger_S *pstMailIntegerInfo);
	INT (*m_fnDelMail)(INT nIndex);
	INT (*m_fnDelMailAll)();
	INT (*m_fnSetMailFlagByIndex)(INT nIndex, BOOL bFlag);
	INT (*m_fnSetMailFlagByEmailId)(WORD wEmailId, BOOL bFlag);
	INT (*m_fnGetEntitledForEnterApp)(INT nAppType, BOOL *pbEntitled);
	INT (*m_fnGetPortalInfo)(AppPortal_S *pstPortalInfo);
	INT (*m_fnGetTextInfo)(TextInfo_S *pstTextInfo);
	INT (*m_fnGetChannelGroupList)(GroupList_S *pstChannelGroupList);
	INT (*m_fnGetChannelGroupContent)(DWORD dwGroupId, CHGroupContent_S *pstChannelGroupContent);
	INT (*m_fnGetGroupPasswordList)(DWORD dwGroupId, CHGroupPwdList_S *pstPasswordList);
	INT (*m_fnSetGroupPasswordList)(DWORD dwGroupId, CHGroupPwdList_S *pstPasswordList);
	INT (*m_fnReSetGroupPasswordList)(DWORD dwGroupId);
	INT (*m_fnChannelGroupSwitch)(DWORD dwGroupId, CHGroupPwd_S *pstPassword);
	INT (*m_fnChannelGroupSetDefault)(DWORD dwGroupId);
	void (*m_fnStopScrolling)(void);
	void (*m_fnSetStandbyMode)(EM_SystemPowerMode emStandbyMode);
	void (*m_fnSetLanguageType)(EM_LanguageType emLanguageType);
	void (*m_fnReboot)(void);
	INT (*m_fnGetChannelMappingInfo)(CSChannelList_S *pstChannelListInfo);
	INT (*m_fnUiDialogRespose)(UiDialogResponse *pstUiDialogResponse);
	void (*m_fnSearchCb)(void);
	INT (*m_fnGetCurrentPpvInfo)(UiDialogConfig *pstUiDialogResponse);
}CSShellTopCaInitParam_S;

EM_Provider   m_emMiddleWareProvider;
EM_Provider   m_emStbProvider;
EM_ChipType   m_emChipType;
BYTE		  m_aucMiddleWareVersion[MAX_MIDDLEWARE_VER_LEN];
BYTE		  m_aucIpAddr[MAX_IPADDR_LEN];
BYTE		  m_aucMacAddr[MAX_MACADDR_LEN];
Version_S		   m_stCurrentProductVer;
TopcasSystemInfo_S m_stRamInfo;
TopcasSystemInfo_S m_stFlashInfo;



typedef struct
{
	TopcasConfig_S m_sTopCaConfig;
	
	DWORD m_dwFlashAddr;
}CSShellTopCaConfig_S;
/**
@brief ����TopCA��ز����ӿڸ�MMCP
@param[in] TopCA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellTopCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellTopCaSetParam(CSShellTopCaGetDefaulParam());
@endcode
*/
BOOL CSShellTopCaSetParam(const CSShellTopCaInitParam_S* psTopCa);

//void  RegTopCaFunc(TopCaFunc_S Topcafunc);
/**
@brief ��ȡTopCAĬ��ʵ�ֲ���
@return MMCP�ṩTopCA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���TopCA�Ķ�����ʹ��csshell_topca_private.h�е�CSShellTopCaGetParam�ӿ�
@see CSShellTopCaGetParam
*/
const CSShellTopCaInitParam_S* CSShellTopCaGetDefaulParam(void);

//void TopCaConfig(ShellTopCaCfg_S CaConfig );

/**
@brief ����TopCA�����������Ϣ
@param[in] psConfig TopCA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellTopCaSetParam֮�����
@see CSShellTopCaConfig_S
*/
void CSShellTopCaSetConfig(CSShellTopCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


