
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
	EM_Provider m_emMiddleWareProvider;//��ϸ�����cstopcainterface.h�е�EM_Provider����
	EM_Provider m_emStbProvider;//��ϸ�����cstopcainterface.h�е�EM_Provider����
	EM_ChipType m_emChipType;//��ϸ�����cstopcainterface.h�е�EM_ChipType����
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
����top ca����ʱ��û�ж�ca���г�ʼ���������ǲ�������
*/
void TopCaConfig(ShellTopCaCfg_S CaConfig );

/*
ע��ca��Ϣ�Ļص���������ʼ��ca
����top ca�ڳ�ʼ����ɺ󼴿��ܷ���Ϣ����������ʱjvm�п��ܻ�û�г�ʼ��
����ϵͳ�쳣(������)���ʰ�ca�ĳ�ʼ������jvm��ʼ����ɺ����
*/
void TopCaInit( void );



#endif


