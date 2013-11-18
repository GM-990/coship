/***************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    cs_cacommand_Interface.h  
* Author:      Wansuanlin
* Date:        Match 2008
* Description: CA command interface
*            
****************************************************************************************************/
#include "mmcp_typedef.h"

#ifndef __CS_CAINTERFACE_H__
#define __CS_CAINTERFACE_H__

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef FAILURE
	#define FAILURE      (-1)
#endif

#ifndef SUCCESS
	#define SUCCESS      (0)
#endif


#ifndef TRUE
	#define TRUE         (1)
#endif

#ifndef FALSE
	#define FALSE        (0)
#endif

typedef enum _CaTraceFlow
{
	CA_FLOW_NONE = 0x00,
	CA_FLOW_INIT = 0x01,
	CA_FLOW_PLAY = 0x02,
	CA_FLOW_UI   = 0x04,
	CA_FLOW_MSG  = 0x08,
	CA_FLOW_ECM  = 0x10,
	CA_FLOW_EMM  = 0x20,
	CA_FLOW_SMC  = 0x40,
	CA_FLOW_KEY  = 0x80,	
    CA_FLOW_TEST= 0x100
}T_CaTraceFlow;

typedef struct _CaFtaService
{
	WORD    m_wDemuxID;          //DMX ID号
	WORD    m_wOrginalNetworkID; //原始网络ID号
	WORD    m_wTsID;             //流ID号
	WORD    m_wServiceID;        //节目ID号
	WORD    m_wVideoPid;         //视频PID
	WORD    m_wAudioPid;         //音频PID
	DWORD   m_dwUserData;        //预留
}T_CaFtaService;

typedef void (*T_CBMsg)(DWORD dwType, DWORD dwParm1, DWORD dwParm2);
typedef INT (*T_CBGetSTBId)(BYTE * pucSTBId,BYTE *pucSTBIdLen);

//高级安全类型
typedef enum _CaSecuredType
{
    EM_CA_SECURED_NACHIPSET_NASECURED, //非高级安全芯片，不带高级安全功能
    EM_CA_SECURED_CHIPSET_SECURED,     //高级安全芯片，带高级安全功能
    EM_CA_SECURED_CHIPSET_NASECURED,   //高级安全芯片，不带高级安全功能
    EM_CA_SECURED_MAX
}E_CaSecuredType;

typedef enum _SecurityPolicy
{
	EM_NON_ADVANCE_SECURITY = 0,
	EM_CONAX_VERIFIER_MODE,  
	EM_CONAX_CHIPSET_PAIRING, 
	EM_NAGRA_ST_POLICY ,
	EM_NAGRA_PAIRING, 
    EM_CONAX_CHIPSET_PAIRING_WITH_LP, 
	EM_CA_SECURITY_POLICY_MAX

}E_SecurityPolicy;

typedef enum _PlatType
{
	EM_ST5107 = 0,
	EM_ST7100,  
 	EM_CONEXANT,
	EM_BCM7309,
	EM_BCM740x,
	EM_ST5197,
	EM_MAX
}E_PlatType;

//Verimatrix初始化配置参数
#define MAX_VMX_STRING_lEN 256

typedef enum _SupportEncAlg
{
    emEncAlg_null = 0,
    emEncAlg_RC4 = 1, 
    emEncAlg_AESECBT = 2,
    emEncAlg_proprietary1 = 3,
    emEncAlg_AESECBL = 4,
    emEncAlg_DVBCSA = 5,
    emEncAlg_proprietary2 = 6,
    emEncAlg_AESCBC1 = 7
}SupportEncAlg_E;

#define  MAX_ALGPRITHN_COUNT 10

typedef struct _VMInitParm_S
{
	CHAR			m_acCompany[MAX_VMX_STRING_lEN];		//client register company name
	CHAR			m_acServer[MAX_VMX_STRING_lEN];			//vcas server ip address
	CHAR			m_acVKSServer[MAX_VMX_STRING_lEN];		//VKS Server ip address and port ep, 10.9.8.1:12579
	DWORD			m_dwPort;								//vcas server ip port
	BOOL			m_bEnableIPv6;							//enable ipv6
	DWORD			m_dwTimeout;							//setting specifies the timeout (in seconds) for connect operations.
	CHAR			m_acCertStorePath[MAX_VMX_STRING_lEN];
	CHAR			m_acRootCert[MAX_VMX_STRING_lEN];		//setting specifies the name and location of the “rootcert.pem” file
	DWORD			m_dwErrorLevel; 						//range 1-5
	BOOL			m_bEnableLogging;						//setting causes logging to the error log to be disabled. FALSE is disable
	BOOL			m_bEnableHDDecrypt;						//enable hardware decrypt or not

	/*cxj add begin*/
	INT				m_nAlgthSupport;						//支持的解扰算法个数
	SupportEncAlg_E m_aeAlgSpt[MAX_ALGPRITHN_COUNT];		//支持的解扰算法
	/*cxj add end*/
}VMInitParm_S;

//CA配置结构
typedef struct _CaConfig
{
	//viaccess ca专用
	BOOL     			m_bCamlock;                 //TRUE:开启camlock 功能,FALSE:关闭
	BOOL     			m_bIsDrvTest;               //TRUE:驱动测试 ,FALSE:认证测试
	BOOL     			m_bIsPVR;                   //TRUE:PVR功能,FALSE:仅对一路节目解扰
	//conax专用
	BOOL     			m_bIsIPPV;                  // TRUE:开启回传功能,FALSE:关闭
	E_PlatType        	m_emPlatType;      			//平台类型，用于配置chipset pairing mech 参数
	//conax & nagra
	E_SecurityPolicy  	m_emPolicy;	     			//高级安全采用的策略

	//通用
	E_CaSecuredType   	m_emSecured;       			//高级安全类型
	DWORD    			m_dwCaDataStartAddr;        //  CA flash address, 1 block
	DWORD   			m_dwCaPrivateDataStartAddr; // CA backup flash address,1 block
	DWORD    			m_dwCaOTPAddr;              //用于要OTP的起始地址
	BOOL     			m_bIsReverseCard;           //TRUE:表示反向卡板，默认FALSE:正向卡板
	DWORD    			m_dwPrivate;                //预留
		//Irdeto 专用//re-move in the end of the structure
	BOOL	 			m_bIsFTABlock;    			//是否开启FTABlock功能,TRUE:开启,FLASH:关闭
	DWORD	 			m_dwOwnerID;				//Irdeto为当前项目分配的专有OwnerID
	BOOL	 			m_bIsEOF;    				//是否开启EOF功能,TRUE:开启,FLASH:关闭
	BOOL	 			m_bIsSOL;					//是否开启SOL功能,TRUE:开启,FLASH:关闭

	/*获取机顶盒内部序列号，机卡配对的时候会用到,pucSTBIdLen为输入输出参数，
	输入的时候为STBId的buffer大小，输出的时候为STBId的实际长度*/
	T_CBGetSTBId 		m_stGetSTBId;

	DWORD 				dwSTBSoftVer;				//软件版本号
	DWORD 				dwSTBHardVer;				//硬件版本号
	DWORD 				dwManufacturerCode;			//厂商代码
	VMInitParm_S     	m_sConfigParm;				//cxj add for Verimatrix init param
}CaConfig_S;

/**************************************************************************
*Function Name: CSCAConfig
*   配置初始化CA模块所需参数
*
*Parameters	
*	pstCaConfig[in]:
*		---m_bCamlock   相当于是否启用运营商与卡配对功能，适用于viaccess ca
*		---m_bIsDrvTest 驱动测试与认证测试处理方式不同，适用于viaccess/mediaguard/nagra ca
*		---m_bIsPVR     是否需要支持多路同时解扰，适用于viaccess ca，这个要根据viaccess开展项目
*                          要求viaccess提供对应的是否要支持多路解扰的内核库
*		---m_bIsIPPV    是否开启回传功能，FALSE表示不用，适用于conax ca
*		---m_emPlatType 设置芯片型号，用于配置chipset pairing mech参数，非高级安全芯片或不用启用
*			   chipset pairing mech功能可以不配置这个参数
*		---m_emPolicy   高级安全策略，适用于conax/nagra ca
*		---m_emSecured  芯片类型，可以不传
*		---m_dwCaDataStartAddr   存储CA数据的Flash起始地址，对于NDS CA是传E2PROM起始地址
*		---m_dwCaPrivateDataStartAddr  存储邮件等数据的Flash起始地址，对于NDS CA是传E2PROM起始地址
*		---m_dwCaOTPAddr   OTP起始地址，对于Nagra ca有一段数据要OTP，OTP后只能读，不能写
*		---m_bIsReverseCard 卡板是正向还是反向，默认情况为正向卡板，这个项目组要特别注意
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
*
*Descriptiont:
*	对于之前已出货的项目就按照之前的流程做，对于新开的项目，希望是在初始化CA之前调用此函数，按照参数的
*   描述传入数据给CA模块
*
**************************************************************************/
INT CSCAConfig(CaConfig_S *pstCaConfig);

/**************************************************************************
*Function Name: CS_CA_Init
*   初始化CA模块
*
*Parameters	
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_Init(void);

/**************************************************************************
*Function Name: CS_CA_RegisterMsgCallback
*    CA消息接受注册回调函数
*
*Parameters	
*	hcbMsg [in]:
*		T_CBMsg类型回调函数.
*		参数dwType 为CA消息类型(CA_COMMAND或CA_ALARM)
*		参数dwParm1为CA消息名称(如CA_COMMAND_PPV_REQUEST)
*		参数dwParm2为消息所带数据结构体的地址
*
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
*
* 备注: 此函数需要CS_CA_Init初始化之前调用
**************************************************************************/
INT CS_CA_RegisterMsgCallback(T_CBMsg hcbMsg);
/**************************************************************************
*Function Name: CS_CA_GetCoreVersion
*   获取CA内核库版本信息
*
*Parameters
*   pacVerBuf[out]:指向存储CA内核版本信息的空间
*   dwBufSize[in]:给定一个最大长度值.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetCoreVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name: CS_CA_GetVersion
*   获取CA版本信息
*
*Parameters
*   pacVerBuf[out]:指向存储CA版本信息的空间
*   dwBufSize[in]:给定一个最大长度值.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name:  CS_CA_StartSrvPlay
*  启动一个CA服务通道
*
*Parameters
*	pstService [in]:
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_StartSrvPlay(T_CaFtaService* pstService);

/**************************************************************************
*Function Name: CS_CA_StopSrvPlay
*  停止一个CA服务通道
*
*Parameters 
*	pstService [in]:
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_StopSrvPlay(T_CaFtaService* pstService);

/**************************************************************************
*Function Name: CS_CA_NewBatSectionNotify
*  中间件提供一个接口供应注入某个指定的BouquetId(比如:IRDETO CA要3622这个BouquetId)
*  
*  通知CA模块BAT数据情况:
*  (1)当BAT版本发生变化;
*  (2)切换频点;
*
*Parameters 
*	pcSectionData[in]:
*		BAT数据缓存区地址 
*	nLen [in]:
*		BAT数据长度
*	cDemux[in]:
*		通道号
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewBatSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewNitSectionNotify
*  通知CA模块NIT数据情况:
*  (1)当NIT版本发生变化;
*  (2)切换频点;
*
*Parameters 
*	pcSectionData[in]:
*		NIT数据缓存区地址 
*	nLen [in]:
*		NIT数据长度
*	cDemux[in]:
*		通道号
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewNitSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewPmtSectionNotify
*  通知CA新PMT数据.
*
*Parameters 
*	pcSectionData[in]:
*		PMT数据缓存区地址 
*	nLen [in]:
*		PMT数据长度
*	cDemux[in]:
*		通道号
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewPmtSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewCatSectionNotify
*  通知CA新CAT数据
*
*Parameters 
*	pcSectionData[in]:
*		CAT数据缓存区地址 
*     nLen [in]:
*		CAT数据长度
*	cDemux[in]:
*		通道号
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewCatSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewSdtSectionNotify
*  通知CA新SDT数据.
*
*Parameters 
*	pcSectionData[in]:
*		SDT数据缓存区地址 
*	nLen [in]:
*		SDT数据长度
*	cDemux[in]:
*		通道号
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewSdtSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
* Function Name: CSCANotifyFrequencyChange
* 	通知CA频点发生改变
* Parameters	
*             无
* Return Values
*			  无
**************************************************************************/
void CSCANotifyFrequencyChange(void);

/**************************************************************************
*Function Name: CS_CA_SetDebugMode
*  设置打印模式,用于应用调试
*
*Parameters 
*	dwMode[in]:
*		T_CaTraceFlow类型中的一种或多种
*       例如：如果dwMode = CA_FLOW_SMC，则打印与智能卡相关的信息
**************************************************************************/
void CS_CA_SetDebugMode(T_CaTraceFlow dwMode);

#ifdef  __cplusplus
}
#endif

#endif //__CS_CAINTERFACE_H__

