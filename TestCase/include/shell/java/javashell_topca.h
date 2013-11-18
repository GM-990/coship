
#ifndef SHELL_TOPCA_H
#define SHELL_TOPCA_H

#ifdef __cplusplus
extern "C" {
#endif
#include "udi_typedef.h"
#include "cstopcainterface.h"

typedef struct _TopCaFunc
{
	INT (*fnCSCAGetMailNum)(MailInfo_S *pstMailInfo);
	INT (*fnCSCAGetMailTitle)(INT nIndex, MailTitle_S *pstMailTitle);
	INT (*fnCSCAGetMailContent)(INT nIndex, MailContent_S *pstMailContent);
	INT (*fnCSCAGetMailIntegerInfoByEmailId)(WORD wEmailId, MailInteger_S *pstMailIntegerInfo);
	INT (*fnCSCADelMail)(INT nIndex);
	INT (*fnCSCADelMailAll)();
	INT (*fnCSCASetMailFlagByIndex)(INT nIndex, BOOL bFlag);
	INT (*fnCSCASetMailFlagByEmailId)(WORD wEmailId, BOOL bFlag);
	INT (*fnCSCAGetEntitledForEnterApp)(INT nAppType, BOOL *pbEntitled);
	INT (*fnCSCAGetPortalInfo)(AppPortal_S *pstPortalInfo);
	INT (*fnCSCAGetTextInfo)(TextInfo_S *pstTextInfo);
	INT (*fnCSCAGetChannelGroupList)(GroupList_S *pstChannelGroupList);
	INT (*fnCSCAGetChannelGroupContent)(DWORD dwGroupId, CHGroupContent_S *pstChannelGroupContent);
	INT (*fnCSCAGetGroupPasswordList)(DWORD dwGroupId, CHGroupPwdList_S *pstPasswordList);
	INT (*fnCSCASetGroupPasswordList)(DWORD dwGroupId, CHGroupPwdList_S *pstPasswordList);
	INT (*fnCSCAReSetGroupPasswordList)(DWORD dwGroupId);
	INT (*fnCSCAChannelGroupSwitch)(DWORD dwGroupId, CHGroupPwd_S *pstPassword);
	INT (*fnCSCAChannelGroupSetDefault)(DWORD dwGroupId);
	void (*fnCSCAStopScrolling)(void);
	void (*fnCSCASetStandbyMode)(EM_SystemPowerMode emStandbyMode);
	void (*fnCSCASetLanguageType)(EM_LanguageType emLanguageType);
	void (*fnCSCAReboot)(void);
	INT (*fnCSCAGetChannelMappingInfo)(CSChannelList_S *pstChannelListInfo);
	INT (*fnCSCAUiDialogRespose)(UiDialogResponse *pstUiDialogResponse);
	void (*fnCSCASearchCb)(void);
	INT (*fnCSCAGetCurrentPpvInfo)(UiDialogConfig *pstUiDialogResponse);
}TopCaFunc_S;

typedef struct
{
	EM_Provider m_emMiddleWareProvider;//详细定义见cstopcainterface.h中的EM_Provider声明
	EM_Provider m_emStbProvider;//详细定义见cstopcainterface.h中的EM_Provider声明
	EM_ChipType m_emChipType;//详细定义见cstopcainterface.h中的EM_ChipType声明
	BYTE          m_aucMiddleWareVersion[MAX_MIDDLEWARE_VER_LEN];
	BYTE          m_aucIpAddr[MAX_IPADDR_LEN];
	BYTE          m_aucMacAddr[MAX_MACADDR_LEN];
	Version_S          m_stCurrentProductVer;
	TopcasSystemInfo_S m_stRamInfo;
	TopcasSystemInfo_S m_stFlashInfo;
	

	DWORD m_FLSADDR_CA;

	DWORD m_dwAlarm;
	DWORD m_dwInfo;
	DWORD m_dwCommand;
	DWORD m_dwStatus;
	DWORD m_dwMMI;
}ShellTopCaCfg_S;


void  RegTopCaFunc(TopCaFunc_S Topcafunc);

//void TopCaConfigInit(ShellTopCaCfg_S caAddrConfig );
/*
配置top ca，此时并没有对ca进行初始化，仅仅是参数配置
*/
void TopCaConfig(ShellTopCaCfg_S CaConfig );

/*
注册ca信息的回调函数并初始化ca
由于top ca在初始化完成后即可能发消息出来，而此时jvm有可能还没有初始化
导致系统异常(死机等)，故把ca的初始化放在jvm初始化完成后进行
*/
void TopCaInit( void );



#endif


