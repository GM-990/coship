#ifndef _CLIFE_INTERFACE_H_
#define _CLIFE_INTERFACE_H_

#include "CS_Assert.h"
#include "cs_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

//MHEG5运行状态
typedef enum _MHEG_RUNNING_STATUS
{
	EM_MHEG_RUNNING_STATUS_STOP,
	EM_MHEG_RUNNING_STATUS_START,
	EM_MHEG_RUNNING_STATUS_SHOW,
	EM_MHEG_RUNNING_STATUS_HIDE
}EMHEG_RUNNING_STATUS;

//app->MHEG5 按键映射表
typedef enum _MHEG_VKEY
{
	EM_MHEG_VK_UNDEFINED = 0,	//not use
	EM_MHEG_VK_RED,   				//VK_RED
	EM_MHEG_VK_GREEN,     			//VK_GREEN
	EM_MHEG_VK_YELLOW,  			//VK_YELLOW
	EM_MHEG_VK_BLUE,  				//VK_BLUE
	EM_MHEG_VK_EXIT,       			//VK_EXIT
	EM_MHEG_VK_TEXT,       			//VK_TEXT
	EM_MHEG_VK_LEFT,    			//VK_LEFT
	EM_MHEG_VK_RIGHT,  			//VK_RIGHT
	EM_MHEG_VK_UP,  				//VK_UP
	EM_MHEG_VK_DOWN,   			//VK_DOWN
	EM_MHEG_VK_OK,   				//VK_OK
	EM_MHEG_VK_0,   				//VK_0
	EM_MHEG_VK_1,					//VK_1
	EM_MHEG_VK_2,					//VK_2
	EM_MHEG_VK_3,					//VK_3
	EM_MHEG_VK_4,					//VK_4
	EM_MHEG_VK_5,					//VK_5
	EM_MHEG_VK_6,					//VK_6
	EM_MHEG_VK_7,					//VK_7
	EM_MHEG_VK_8,					//VK_8
	EM_MHEG_VK_9,					//VK_9
	EM_MHEG_VK_EPG				//VK_EPG
}MHEG_VKEY; 

/*==========================================================
函数原型:CSPLF_MHEGInit();
功能 : 初始化MHEG5中间件
返回值：TRUE表示成功，FALSE表示失败
=========================================================== */
BOOL CSPLF_MHEGInit(void);

/*==========================================================
函数原型:CSPLF_MHEGStart();
功能 : 启动MHEG5中间件
返回值：TRUE表示成功，FALSE表示失败
=========================================================== */
BOOL CSPLF_MHEGStart(void);

/*==========================================================
函数原型:CSPLF_MHEGStop();
功能 : 停止MHEG5中间件
返回值：TRUE表示成功，FALSE表示失败
=========================================================== */
BOOL CSPLF_MHEGStop(void);

/*==========================================================
函数原型:CSPLF_MHEGEventSender();
功能 : 发送按键消息给MHEG5中间件
返回值：0表示成功MHEG5应用处理了此键，-1表示失败
注意:要是返回失败应用应该处理相应返回失败的按键
=========================================================== */
int CSPLF_MHEGEventSender(int nMHEGKey);

/*==========================================================
函数原型:CSPLF_MHEGShow();
功能 : 显示MHEG5中间件OSD
返回值：TRUE表示成功，FALSE表示失败
=========================================================== */
BOOL CSPLF_MHEGShow(void);

/*==========================================================
函数原型:CSPLF_MHEGHide();
功能 : 隐藏MHEG5中间件
返回值：TRUE表示成功，FALSE表示失败
=========================================================== */
BOOL CSPLF_MHEGHide(void);

EMHEG_RUNNING_STATUS CSPLF_GetMHEGStatus(void);
void CSPLF_MHEGVersion(void);
#ifdef __cplusplus
}
#endif

#endif

