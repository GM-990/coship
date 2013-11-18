/**
Copyright (c) 2005,ShenZhen Coship Electronic Ltd Co.
All rights reserved.

File Name: cs_mulscreen.h
Summary: 广东省网三屏互动头文件
Current Version: 1.0
Author: Zheng Fen

History:
1.0.1 2012/04/11 1初稿
*/

#ifndef _MUL_SCREEN_H_
#define _MUL_SCREEN_H_


#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**
@brief 三屏互动模块使用的传输设备
*/
typedef enum  
{
    EM_MULTISCREEN_TANSDEVTYPE_BLUETOOTH ,  /*设备类型:蓝牙*/
    EM_MULTISCREEN_TANSDEVTYPE_ETHERNET , 	/*设备类型:无线或者有线以太网传输设备*/
    EM_MULTISCREEN_TANSDEVTYPE_COM			/*设备类型:com口*/
} CSMultiScreenDevType_E;


/**
@brief 三屏互动模块指令类型
*/
typedef enum  
{
	EM_MULTISCREEN_INSTRUCT_MATCH , 					/*指令类型:匹配指令*/
	EM_MULTISCREEN_INSTRUCT_KEYPRESS , 					/*指令类型:按键指令*/
	EM_MULTISCREEN_INSTRUCT_INPUT,						/*指令类型:输入法指令*/
	EM_MULTISCREEN_INSTRUCT_SENSOR,					/*指令类型:模拟传感器指令*/
	EM_MULTISCREEN_INSTRUCT_MOUSE,						/*指令类型:鼠标指令*/
	EM_MULTISCREEN_INSTRUCT_MTOSTBSWITCH,				/*指令类型:手机向机顶盒切屏指令*/
	EM_MULTISCREEN_INSTRUCT_STBTOMSWITCH,				/*指令类型:机顶盒向手机切屏指令*/

	EM_MULTISCREEN_INSTRUCT_ANSWERMATCH ,  			/*指令类型:匹配指令的应答指令*/
	EM_MULTISCREEN_INSTRUCT_ANSWERKEYPRESS ,  			/*指令类型:按键指令的应答指令*/
	EM_MULTISCREEN_INSTRUCT_ANSWERINPUT ,  			/*指令类型:输入法指令的应答指令*/

	EM_MULTISCREEN_INSTRUCT_ANSWERSENSOR,
	EM_MULTISCREEN_INSTRUCT_ANSWERMOUSE ,  			/*指令类型:鼠标指令的应答指令*/
	EM_MULTISCREEN_INSTRUCT_ANSWERMTOSTBSWITCH ,  	/*指令类型:切屏(手机向机顶盒)指令的应答指令*/
	EM_MULTISCREEN_INSTRUCT_ANSWERSTBTOMSWITCH ,  	/*指令类型:切屏(机顶盒向手机)指令的应答指令*/
	  	
	EM_MULTISCREEN_INSTRUCT_OTHER						/*其他指令*/

} CSMultiScreenInstructType_E;


typedef struct  
{
	CSMultiScreenInstructType_E 	eInstructType;	/*指令类别*/
	unsigned int  		m_unSyn;    					/*同步标识*/
	unsigned char   	m_acMobileId[32];			/*移动终端标识符*/
	unsigned int 		m_unLen;					/*附加数据长度*/
	char*  			m_pcAdditionalData;    		/*附加数据*/
} CSMultiScreenPacket_S;


/**
@brief 调用CSMultiScreenAddCallback和CSMultiScreenRemoveCallback的时候, 需要传入此类型的函数指针
@param[in] sPacketInfo:表示当前收到的消息值
@param[in] pvUserData:用来保存用户数据
*/
typedef void  (*CSOnMultiScreenMsg_F)(CSMultiScreenPacket_S *sPacketInfo, void * pvUserData);

/**
@brief 	创建三屏互动实例
@param[in]  eDeviceType: 三屏互动模块使用的传输设备,具体定义见CSMultiScreenDevType_E
@param[in] nPort:  三屏互动监听端口
@return   返回创建的三屏互动实例句柄,为NULL表示未创建成功
*/
HCSHANDLE CSMultiScreenCreate(CSMultiScreenDevType_E eDevType, int nPort);

/**
@brief 	启动三屏互动功能
@param[in]  hMultiScreen: CSMultiScreenCreate所创建出来的三屏互动实例句柄
@return   成功CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenStart(HCSHANDLE hMultiScreen);

/**
@brief 	停止三屏互动功能
@param[in]  hMultiScreen: CSMultiScreenCreate所创建出来的三屏互动实例句柄
@return   成功CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenStop(HCSHANDLE hMultiScreen);

/**
@brief 	销毁三屏互动实例
@param[in]  hMultiScreen: CSMultiScreenCreate所创建出来的三屏互动实例句柄
@return   成功CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenDestroy(HCSHANDLE hMultiScreen);

/**
@brief 	三屏互动发送数据
@param[in]  hMultiScreen: CSMultiScreenCreate所创建出来的三屏互动实例句柄
@param[in] psPacketInfo:要发送的报文
@return   成功CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenSend(HCSHANDLE hMultiScreen, CSMultiScreenPacket_S *psPacketInfo);

/**
@brief 添加三屏互动通知回调函数

@param[in] hMultiScreen CSMultiScreenCreate所创建出来的三屏互动实例句柄
@param[in] pCallback 回调函数,当三屏互动通信模块状态发生变化时调用该函数
@param[in] pvUserData 用户数据,将作为回调函数的pvUserData参数返回
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
@note 添加回调前产生的状态变化不会补充通知
*/
CSUDI_Error_Code CSMultiScreenAddCallback(HCSHANDLE hMultiScreen, CSOnMultiScreenMsg_F pCallback, void* pvUserData);

/**
@brief 删除三屏互动回调函数

@param[in] hMultiScreen CSMultiScreenCreate所创建出来的三屏互动实例句柄
@param[in] pCallback 回调函数,之前注册的函数
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSMultiScreenRemoveCallback(HCSHANDLE hMultiScreen, CSOnMultiScreenMsg_F pCallback);


#ifdef __cplusplus
}
#endif


#endif

