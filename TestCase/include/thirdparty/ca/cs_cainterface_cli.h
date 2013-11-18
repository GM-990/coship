#ifndef _CS_CAINTERFACE_CLI_H_
#define _CS_CAINTERFACE_CLI_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"

typedef void (*pfCLIprint)(const CHAR * format,...);
typedef INT (*pfLibprint)(DWORD ,INT, pfCLIprint);

#define CA_CLI          0xF0 //  CLI 消息类型，不要和CA_ALARM等消息相等

typedef enum
{
	CA_CLI_SRV=0,	//multi service
	CA_CLI_CAT,		//cat,
	CA_CLI_PMT,		//pmt
	CA_CLI_NIT,		//nit,
	CA_CLI_BAT,		//bat
	
	CA_CLI_DMX,		//filter
	CA_CLI_MSG,		//the lastest msg	
	CA_CLI_CARD,	//card status
	CA_CLI_CW,		//original cw
	CA_CLI_ECM,     //ecm
	
	CA_CLI_EMM,     //emm
	CA_CLI_MENU,	//irdeto 
	
	CA_CLI_TM   ,   //获取某个消息的当前时间，常用于视频开启消息之后，用于测量解扰时间
	CA_CLI_MAX
}T_CaCli;

/**************************************************************************
*Function Name: CS_CACLIVIEW_SRV
*   查看CA内部的多路信息
*
*Parameters
*		   dwHandle:节目句柄/低于0x10000，就认为是Index
*          nParm:   用作参数送入，不需要的情况可以输入0
*          Fprint:  打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_SRV(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_CAT
*   查看CA内部的CAT表
*
*Parameters
*		   dwHandle:节目句柄/低于0x10000，就认为是Index
*          nParm:   用作参数送入，不需要的情况可以输入0
*          Fprint:  打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_CAT(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_PMT
*   查看CA内部的PMT表
*
*Parameters
*		   dwHandle:节目句柄/低于0x10000，就认为是Index
*          nParm:   用作参数送入，不需要的情况可以输入0
*          Fprint:  打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_PMT(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_NIT
*   查看CA内部的NIT表
*
*Parameters
*		   dwHandle:节目句柄/低于0x10000，就认为是Index
*          nParm:   用作参数送入，不需要的情况可以输入0
*          Fprint:  打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_NIT(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_BAT
*   查看CA内部的BAT表
*
*Parameters
*		   dwHandle:节目句柄/低于0x10000，就认为是Index
*          nParm:   用作参数送入，不需要的情况可以输入0
*          Fprint:  打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_BAT(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_DMX
*   查看CA内部，全部filter参数
*
*Parameters
*		   nSrv:  用作参数送入，不需要的情况可以输入0
*          nParm: 用作参数送入，不需要的情况可以输入0
*          Fprint:打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_DMX(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_DMX
*   查看CA抛出的最后消息，用于故障查看
*
*Parameters
*		   dwHandle:节目句柄/低于0x10000，就认为是Index
*          nParm:   用作参数送入，不需要的情况可以输入0
*          Fprint:  打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_MSG(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_CARD
*   查看CA内部卡的信息
*
*Parameters
*		   dwHandle:节目句柄/低于0x10000，就认为是Index
*          nParm:   用作参数送入，不需要的情况可以输入0
*          Fprint:  打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_CARD(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_CW
*   查看CA库设定解扰字，非高级安全，字节打出解扰字。高级安全，打出CW1
*
*Parameters
*		   dwHandle:节目句柄/低于0x10000，就认为是Index
*          nParm:   用作参数送入，不需要的情况可以输入0
*          Fprint:  打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_CW(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_ECM
*   查看CA库ECM信息
*
*Parameters
*		   dwHandle:  请输入PID/如果输入的是句柄，就把当前所有ECM都打印出来
*          nParm:     1:符合PID的就打印出来  0:关闭打印
*          Fprint:打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_ECM(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_EMM
*   查看CA库EMM信息
*
*Parameters
*		   dwHandle:  请输入PID/如果输入的是句柄，就把当前所有EMM都打印出来
*          nParm:     1:符合PID的就打印出来  0:关闭打印
*          Fprint:打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_EMM(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

/**************************************************************************
*Function Name: CS_CACLIVIEW_MENU
*   针对CA库特有的MENU
*
*Parameters
*		   dwHandle:节目句柄/低于0x10000，就认为是Index
*          nParm:   用作参数送入，不需要的情况可以输入0
*          Fprint:  打印函数
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CACLIVIEW_MENU(DWORD dwHandle,INT nParm, pfCLIprint Fprint);

#ifdef  __cplusplus
}
#endif

#endif

