
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
	
	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口	
	
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
@brief 设置TopCA相关操作接口给MMCP
@param[in] TopCA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellTopCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellTopCaSetParam(CSShellTopCaGetDefaulParam());
@endcode
*/
BOOL CSShellTopCaSetParam(const CSShellTopCaInitParam_S* psTopCa);

//void  RegTopCaFunc(TopCaFunc_S Topcafunc);
/**
@brief 获取TopCA默认实现参数
@return MMCP提供TopCA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得TopCA的对象须使用csshell_topca_private.h中的CSShellTopCaGetParam接口
@see CSShellTopCaGetParam
*/
const CSShellTopCaInitParam_S* CSShellTopCaGetDefaulParam(void);

//void TopCaConfig(ShellTopCaCfg_S CaConfig );

/**
@brief 设置TopCA的相关配置信息
@param[in] psConfig TopCA相关配置信息的结构体实例
@note 该接口必须在CSShellTopCaSetParam之后调用
@see CSShellTopCaConfig_S
*/
void CSShellTopCaSetConfig(CSShellTopCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


