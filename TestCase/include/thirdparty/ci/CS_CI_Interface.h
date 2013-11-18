/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_CI_Interface.h  
* Author:      HuYuanfeng
* Date:        Oct 2006 
* Description: CI interface
*            
*****************************************************************************************************************************/

/****************************************************************************************************************************
* 			                Revision History                                *
*****************************************************************************************************************************
Revision: 1.0  
date October 11,2006
author:Hu Yuanfeng 
reason:Create.

Revision: 1.1  
date: November 21,2007
author:Hu Yuanfeng 
reason:change some fuction definitons.

Revision: 1.2  
date: March,19,2008
author:Hu Yuanfeng 
reason:delete some old functions ,and add new function for Twin Tuner&Twin CI



*****************************************************************************************************************************/


#ifndef _CS_CI_INTERFACE_H_
#define _CS_CI_INTERFACE_H_


#ifdef _cplusplus
extern "C"{
#endif

#define MAX_ANSW_LEN  		100
#define MAX_TITLIE_LEN  		200
#define MAX_SUBTITILE_LEN 	 200
#define MAX_BOTTOM_LEN  	200
#define MAX_ITEM_NO		25
#define MAX_MESSAGE_LEN  	256

#define CS_CI_MESSAGE_BASE  (0)


typedef enum _CiTraceFlow
{
	CI_FLOW_NONE = 0x0,
	CI_FLOW_INIT = 0x01,
	CI_FLOW_PLAY = 0x02,
	CI_FOLW_UI   = 0x04,
	CI_FOLW_MSG  = 0x08,
	CI_FLOW_CIS  = 0x10,
	CI_FLOW_BUFFER  = 0x20,
	CI_FLOW_WRITE  = 0x40,
	CI_FLOW_READ  = 0x80
}T_CiTraceFlow;


typedef enum _CI_MSG
{
	CI_ALARM = CS_CI_MESSAGE_BASE,
	CI_COMMAND,
	CI_STATUS,
	CI_INFO,
	CI_MMI
}E_CI_MSG;

typedef enum _CI_Alarm
{
	CI_ALARM_CAM_OUT,             /*Please Insert CAM Card*/
	CI_ALARM_CAM_UNKNOW,          /*Unknow CAM Card*/
	CI_ALARM_CAM_IN,              /*CAM Card In*/	
	CI_ALARM_CAM_INIT,           //CAM 正在初始化
	CI_ALARM_CAM_OK,              /*CAM Card Ok*/

	//CAM_SCRAMBLED - 频道加扰不能播放 ,参数: 0 - PMT有效，但图像不能播放(scrambled); 1 - PMT无效且图像不能播放(bad)
	CI_ALARM_SCRAMBLED,//节目加扰, 解扰不成功

	CI_ALARM_NEED_TECHNICAL   	//发消息到UI,告知出现技术问题，如解扰能力有限，
									//可要求用户选择较少的基本流	
}E_CI_Alarm;

typedef enum _CAM_STATUS
{
	CAM_STATUS_EMPTY,//空，发生在拔出或者没有插卡的时候                  
	CAM_STATUS_INITIALIZE, //开始初始化，发生在插入CAM的时候     
	CAM_STATUS_WORK_DELAY, //准备工作中，此时CAM的信息已经获得
	CAM_STATUS_WORKING,   //正常工作中 
	CAM_STATUS_LOCKED,     //模块被锁定
	CAM_STATUS_UNLOCK      //模块未锁定
}E_CAM_STATUS;

typedef enum _CI_MMI
{
	CI_MMI_ENQ, //查询monitor
	CI_MMI_MENU, //列出菜单
	CI_MMI_LIST, //列表
	CI_MMI_CLOSE//关闭对话
}E_CI_MMI;
	
typedef enum _CI_INFO
{
	CI_INFO_CAMINFO,
	CI_INFO_RESERVED
}E_CI_INFO;

typedef enum _CaPmtListOperationModel 
{
	PMT_SECTION_MORE,
	PMT_SECTION_FIRST,
	PMT_SECTION_LAST,
	PMT_SECTION_ONLY,
	PMT_SECTION_ADD,
	PMT_SECTION_UPDATE,
	PMT_SECTION_RESERVED
} E_CaPmtListOperationModel;



typedef struct _CAM_INFO
{
	BYTE  ucAppType; //取值: 1:CA   0: EPG  
	INT   nAppManufacture;//CA system ID
	INT   nManufactureCode;//各个厂商自己定义的内容
	BYTE ucNameLen;
	BYTE  acSystemName[30];//ＣＡ厂商的名字
}T_CAM_INFO;



/*========================================================================================*/
/*本结构体用在两个地方:
	1、作为消息类型为CI_MMI_ENQ的回调函数的第四个参数，此时以下几个值
		起作用:
		  ucBlind [out]:    为1时，表示输入的内容不显示，可用*代替,当为其他值时
		  			    内容 需 显示
		  			    
		  ucAnswLen[out]: 为0xff 时,表明可由上层定义输入内容的长度,当为不为0xff
		  				时,表示用户需输入的内容的长度
		  				
		  acAnswText[out] 为在界面上显示的文本信息
		  
	2、作为函数CS_CI_MMI_AnswEnq 获取用户应答的参数,此时以下几个值起作用:
	
		ucAnswId[in]:根据用户的选择取值,当用户同意应答时取值为1,当用户取消
					应答时,取值为0
		
		ucAnswLen[in\out]: 当取值为0xff 时,上层自定义长度,不能超过MAX_ANSW_LEN.
						  当取值为某一具体值时,就表示用户需输入的长度,此时
						  用户不能自定义长度
		
		acAnswText[in]:用户输入的内容

==========================================================================================*/

typedef struct _Answ_Info
{
	BYTE ucAnswId;   
	BYTE ucBlind;
	BYTE ucAnswLen;
	CHAR acAnswText[MAX_ANSW_LEN];
}T_Answ_Info;




/*=======================================================================================
用来处理list 和menu 的结构体，作为消息类型为CI_MMI_MENU和CI_MMI_LIST 的回调函数
的第四个参数,对于上层以下几个值起作用:

	ucItem_NB[out]:选项的个数,为数组aacMessage的第一个下标 的最大值

	acTitle[out]:标题文字,当他的长度小于MAX_TITLIE_LEN 或者长度为0 的时候,该数组
			    以'\0'结束,  当他的长度大于等于MAX_TITLIE_LEN 的时候,只显示前
			    MAX_TITLIE_LEN 个

	acSubtitle[out]:子标题文字,当他的长度小于MAX_SUBTITILE_LEN 或者长度为0 的时候,
				   该数组 以'\0'结束,  当他的长度大于等于MAX_SUBTITILE_LEN 的时候,只
				   显示前MAX_TITLIE_LEN 个

	acBottom[out]:底线文字,当他的长度小于MAX_BOTTOM_LEN 或者长度为0 的时候,
				 该数组 以'\0'结束,  当他的长度大于等于MAX_BOTTOM_LEN 的时候,只显
				 示前MAX_BOTTOM_LEN 个
	aacMessage[out]:选项信息,最多显示ucItem_NB 个,每个的长度小于MAX_MESSAGE_LEN 或者
				    长度为0 的时候,该数组 以'\0'结束,  当他的长度大于等于MAX_MESSAGE_LEN 
				    的时候,只显示前MAX_MESSAGE_LEN 个

=======================================================================================*/

typedef struct _MMI_Event
{
	BYTE  ucItem_NB;
	WORD wTag_Header;
	BYTE ucTag_Value;

	BYTE acTitle[MAX_TITLIE_LEN];
	BYTE acSubtitle[MAX_SUBTITILE_LEN];
	BYTE acBottom[MAX_BOTTOM_LEN];
	BYTE aacMessage[MAX_ITEM_NO][MAX_MESSAGE_LEN];
}T_MMI_Event;


typedef struct _CI_InitPara
{
	//ci task priority
	INT m_nTaskPrio_Phy;
	INT m_nTaskPrio_Link_SS;
	INT m_nTaskPrio_Rm;

	//task stack size
	INT m_nStackSize_Phy;
	INT m_nStackSize_Link_SS;
	INT m_nStackSize_Rm;
	
	//ci capabilites
	DWORD m_dwSlotNumber;
	DWORD m_dwTunerNumber;
	DWORD m_dwTsChannel;	
	
}T_CI_InitPara;

/*=======================================================================================*/

									/*以下为API 函数*/

/*=======================================================================================*/

typedef void (*T_CI_Msg)(HCSHANDLE hCiSlot, DWORD dwParm1, DWORD dwParm2, DWORD dwParm3);


/**************************************************************************
*Function Name: CS_CI_RegisterMsgCallback
*    CI消息接收注册回调函数
*
*Parameters	
*     hcbMsg [in]:
*         T_CI_Msg类型回调函数.
*           hCiSlot;    //SlotID
*           dwParm1;    //Msg type
*           dwParm2;   //item
*           dwParm3;   //只在消息类型为CI_MMI和CI_INFO的时候有用，其他情况时为0 
*
*Return Values
*       0 : SUCCESS 注册回调函数成功
*       -1: FAILURE 参数错误或注册回调函数失败
**************************************************************************/
INT CS_CI_RegisterMsgCallback(T_CI_Msg hcbMsg);	

/**************************************************************************
*Function Name: CS_CI_Init
*
* 描述:增加初始化参数
*
*  功能: 初始化CI模块
*
*Parameters	
*    stInitPara[in]:CI 模块初始化参数
*		           
*Return Values
*		0 : SUCCESS 初始化CI模块成功
*		-1: FAILURE 初始化CI模块失败
**************************************************************************/
INT CS_CI_Init(T_CI_InitPara stInitPara);


/**************************************************************************
*Function Name: CS_CI_StartService
*
*  功能说明：调用该函数，传入PMT表，通知CI模块开始解扰某套节目
*
*Parameters
*   eModel[in]:具体取值为枚举类型T_CaPmtListOperationModel的选项
*   pacPmtSectionBuf[in]:指向整个完整的PMT SECTION（包括TABLEID字段和CRC）
*   dwBufSize[in]：填充PMT SECTION的缓冲区的大小（>=3+SECTION_LENGTH）
*		           
*Return Values
*		0 : SUCCESS pmt 发送成功，可以开始解扰
*		-1: FAILURE  pmt发送失败
**************************************************************************/
INT CS_CI_StartService(E_CaPmtListOperationModel eModel, BYTE* pacPmtSectionBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name: CS_CI_Stop
*
*描述:为了统一命名风格，更改接口名称，功能同CS_StopCI
*
*  功能说明：要求CI 模块停止解扰
*Parameters
*   
*   
*Return Values
*		0 : SUCCESS  停止解扰
*		-1: FAILURE  停止解扰失败
**************************************************************************/
INT CS_CI_Stop(void);
/**************************************************************************
*Function Name: CS_CI_StopService
*
*  功能说明：要求CI 模块停止解扰某一个节目
*
*Parameters
*   nVideoPID[in]:
*   nAudioPID[in]:
*   nPcrPID[in]: 存放PcrPID
*   
*Return Values
*		0 : SUCCESS  停止解扰某一个节目
*		-1: FAILURE  停止解扰失败
**************************************************************************/
INT CS_CI_StopService( INT nVideoPID, INT nAudioPID, INT nPcrPID);

/******************************************************************************
*Function Name :	CS_CI_GetCamStatus
*    获得CAM卡锁定状态
*Parameters :		
*　　　hCiSlot[in]:	待查询的对应的CAM卡的SlotID
*      pcStatus[out]: 获得的CAM卡锁定状态，
*			取值为E_CAM_STATUS中的:
*			CAM_STATUS_LOCKED,     //模块被锁定
*	        CAM_STATUS_UNLOCK      //模块未锁定
*Return　Values:
*　　　0 : SUCCESS 请求成功
*	　 -1: FAILURE 请求失败
******************************************************************************/
INT CS_CI_GetCamLockStatus(HCSHANDLE hCiSlot, BYTE* pcStatus);

/**************************************************************************
*Function Name: CS_CI_GetCamInfo
*  获取CAM卡信息
*
*Parameters 
*   hCiSlot[in]:
*       对应的CAM卡的SlotID
*
*Return Values: 
*		0 : SUCCESS 请求成功
*	　 -1: FAILURE 请求失败
**************************************************************************/
 INT CS_CI_GetCamInfo(HCSHANDLE hCiSlot);

/******************************************************************************
*Function Name :	CS_CI_MMI_EnterMenu
*    实现人机交互，请求进入CAM应用菜单，所有对模块应用菜单访问前必须先申请进入菜单
*Parameters :		
*　　　hCiSlot[in]:	待查询的对应的CAM卡的SlotID
*Return　Values:
*　　　0 : SUCCESS 请求成功
*	　     -1: FAILURE 请求失败
******************************************************************************/
INT CS_CI_MMI_EnterMenu(HCSHANDLE hCiSlot);

/**************************************************************************
*Function Name: CS_CI_MMI_CloseMmi
*  请求关闭CAM MMI会话，该函数如果被调用，模块应用将退出全部菜单
*
*Parameters 
*   hCiSlot[in]:
*       待查询的对应的CAM卡的SlotID
*
*Return Values: 
*　　　0 : SUCCESS 请求成功
*	　	     -1: FAILURE 请求失败
**************************************************************************/
INT CS_CI_MMI_CloseMmi(HCSHANDLE hCiSlot);

/**************************************************************************
*Function Name: CS_CI_MMI_AnswMenu
*  选择CAM应用的菜单项，进入相应的子菜单或者对应的查询信息
*  列表
*
*Parameters 
*   hCiSlot[in]:
*       待查询的对应的CAM卡的SlotID
*   ucChoice_Ref[in]:
*       用户选择的菜单选项的编号，当ucChoice_Ref = 0x00的时候，表示
*       用户没有做出选择，而是退出菜单或列表
*Return Values: 
*　　　0 : SUCCESS 请求成功
*	　     -1: FAILURE 请求失败
**************************************************************************/
INT CS_CI_MMI_AnswMenu(HCSHANDLE hCiSlot, DWORD dwChoice_Ref);
	
/**************************************************************************
*Function Name: CS_CI_MMI_AnswEnq
*  回应ＣＩ模块的查询命令，发送回应信息
*
*Parameters 
*   hCiSlot[in]:
*       待查询的对应的CAM卡的SlotID
*   tAnswerInfo[in]:
*       表示通知模块是否对此应答做出反应：0取消应答  1 应答
*
*Return Values: 
*　　　0 : SUCCESS 发送信息成功
*	　	    -1: FAILURE 发送信息失败
**************************************************************************/
INT CS_CI_MMI_AnswEnq(HCSHANDLE hCiSlot, T_Answ_Info* tAnswerInfo);

/**************************************************************************
*Function Name: CS_CI_SetDebugMode
*  CI 打印函数
*
*Parameters 
*   dwMode[in]:
*       见T_CiTraceFlow
*
*Return Values: 
*　　　
**************************************************************************/
void CS_CI_SetDebugMode(DWORD dwMode);

/**************************************************************************************

		为解决德国现场测试的黑客CAM  重复初始化,特增加以下接口 

***************************************************************************************/



/**************************************************************************
*Function Name: CS_CI_NotifyPid
* 将播放的节目的音视频PID 通知CI  模块
*
*Parameters 
*   pwPID[in]:
*       所有音视频PID 的数组的地址
*   ucNumOfPID[in]:
*    PID个数
*hTuner[in]:
*Tuner的句柄
*
*Return Values: 
*　　　0 : SUCCESS 成功
*	　	    -1: FAILURE 参数错误
**************************************************************************/
INT CS_CI_NotifyPid(WORD  * pwPID, DWORD  dwNumOfPID,HCSHANDLE hTuner);


/*=========================================================================

			以下为新增结构体以及新增接口,供多路解扰时使用

===========================================================================*/
#define CI_MAX_PID_NUM		 (10)


typedef enum _CSCIOutputType_E
{
 	EM_CI_OUTPUT_PLAY   = 0x01, /*播放*/
 	EM_CI_OUTPUT_RECORD = 0x02  /*录制*/
}CSCIOutputType_E;

 typedef struct _CIChannelInfo_S
{
	WORD	m_wTunerID;			 /*tuner号*/
	WORD	m_wCPUChnl;			 /*cpu ts输入口*/
}CIChannelInfo_S;

typedef struct _CISrvInfo_S
{
	WORD	m_wOrginalNetworkID; /*初始网络ID*/
	WORD    m_wTsID;             /*流ID号*/
	WORD    m_wServiceID;        /*节目ID号*/
	WORD    m_wPidNum;      /*Pid 数目*/
 	WORD    m_awPid[CI_MAX_PID_NUM];/*Pid*/
	DWORD   m_dwUserData;        /*预留*/
	CSCIOutputType_E m_eOutputType; /*输出类型*/
	CIChannelInfo_S m_stCIChannelInfo;
	
}CISrvInfo_S;

/**************************************************************************
*Function Name: CS_CI_StopService
*
*  功能说明：启动播放/录制
*
*Parameters
*   pstCISrvInfo[in]: 节目相关信息
*   
*Return Values
 *   NULL  - 启动播放/录制不成功
 *   非NULL- 启动播放/录制成功
**************************************************************************/
HCSHANDLE CSCIStartService(CISrvInfo_S* pstCISrvInfo);

/**************************************************************************
*Function Name: CS_CI_StopService
*
*  停止播放/录制
*
*Parameters
*   hCIService[in]:启动/录制时分配的Handle
*   
*Return Values
*		 SUCCESS - 停止播放/录制不成功
*        FAILURE - 停止播放/录制成功
**************************************************************************/
INT CSCIStopService( HCSHANDLE hCIService );

/**************************************************************************
*Function Name: CSCINotifyNewPmt
*
*  功能说明：调用该函数，传入PMT表，通知CI模块开始解扰某套节目
*
*Parameters
*   hCIService[in]:启动/录制时分配的Handle
*   eModel[in]:具体取值为枚举类型T_CaPmtListOperationModel的选项
*   pacPmtSectionBuf[in]:指向整个完整的PMT SECTION（包括TABLEID字段和CRC）
*   dwBufSize[in]：填充PMT SECTION的缓冲区的大小（>=3+SECTION_LENGTH）
*		           
*Return Values
*		0 : SUCCESS pmt 发送成功，可以开始解扰
*		-1: FAILURE  pmt发送失败
**************************************************************************/
INT CSCINotifyNewPmt(HCSHANDLE hCIService, E_CaPmtListOperationModel eModel, BYTE* pacPmtSectionBuf, DWORD dwBufSize);
	
#ifdef  __cplusplus
}
#endif

#endif 

