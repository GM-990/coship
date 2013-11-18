/**
   *@version 1.0.2 2009/10/12 设置配置参数
 **/
 
/*Include head files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cs_testkit.h"
#include "cs_panel_test.h"
#include "udi2_error.h"

//平台支持的面板数，从配置文件读出
int g_PanelCnt;

//平台支持的文本灯显示字符个数
int g_TextCnt;

//平台支持的组合灯子灯数量
int g_SuitCnt;

//平台前面板是否支持该显示灯的布尔数组
CSUDI_BOOL g_abSuppLight[EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART + 1];

//控制灯显示属性数组，用来存放该控制灯所具有的属性
CSUDI_UINT32  g_CtrlFunMask[EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART + 1];

//显示灯枚举数组，存放该显示灯的枚举值
CSUDIPanelLightName_E g_aeLight[EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART];

//显示灯名称数组，存放该显示灯的名称
char g_sLight[EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART][10];


void CSTC_PANEL_InitCfg()
{
	char cBuf[5];
	memset(cBuf, '\0', sizeof(cBuf));
	//平台支持的面板数
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_PANEL_COUNT", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_PanelCnt = CSTKGetIntFromStr(cBuf,10);
	}

	//平台支持的文本灯显示字符个数
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_TEXT_COUNT", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_TextCnt = CSTKGetIntFromStr(cBuf,10);
	}
	
	//平台支持的组合灯子灯数量
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_SUIT_COUNT", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_SuitCnt = CSTKGetIntFromStr(cBuf,10);
	}
	
	//平台前面板是否支持显示灯
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_MAIL", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_MAIL - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_SIGNAL", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_SIGNAL - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_AUDIO", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_AUDIO - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CLOCK", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_CLOCK- EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_PLAY", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_PLAY - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_PAUSE", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_PAUSE - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_STOP", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_STOP - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_HDTV", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_HDTV - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_BACKWARD", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_BACKWARD - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_FORWARD", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_FORWARD - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_BALL", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_BALL - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_DUBY", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_DUBY - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_PREVIOUS", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_PREVIOUS - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_NEXT", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_NEXT - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_IR", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_IR - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_USB", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_USB - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_N", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_N - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_VOD", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_VOD - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_POWER", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_POWER - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_TV", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_TV - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_NETWORK", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_NETWORK - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_RADIO", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_RADIO - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CLOCK", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_CLOCK - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_MODEM", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_MODEM - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_REC", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_REC - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_UP", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_UP - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_DOWN", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_DOWN - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_LEFT", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_LEFT - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_RIGHT", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_RIGHT - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_TEXT0", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_TEXT0 - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_TEXT1", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_TEXT1 - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_TEXT2", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_TEXT2 - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_TEXT3", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_TEXT3 - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_TEXT4", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_TEXT4 - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_SUIT0", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_SUIT0 - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_SUIT1", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_SUIT1 - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_SUIT2", cBuf, sizeof(cBuf));
	if(cBuf[0] ==  '1')
	{
		g_abSuppLight[EM_UDIPANEL_SUIT2 - EM_UDIPANEL_CTRLSTART] = CSUDI_TRUE;
	}
	
	//各控制灯所具有的显示属性
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_MAIL_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_MAIL - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_SIGNAL_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_SIGNAL - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_AUDIO_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_AUDIO - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_CLOCK_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_CLOCK- EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_PLAY_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_PLAY - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_PAUSE_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_PAUSE - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}

	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_STOP_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_STOP - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_HDTV_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_HDTV - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_BACKWARD_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_BACKWARD - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_FORWARD_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_FORWARD - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_BALL_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_BALL - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_DUBY_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_DUBY - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_PREVIOUS_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_PREVIOUS - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_NEXT_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_NEXT - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_IR_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_IR - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_USB_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_USB - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_N_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_N - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_VOD_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_VOD - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_POWER_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_POWER - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_TV_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_TV - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_NETWORK_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_NETWORK - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_RADIO_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_RADIO - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}

	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_CLOCK_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_CLOCK - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_MODEM_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_MODEM - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_REC_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_REC - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_UP_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_UP - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_DOWN_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_DOWN - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_LEFT_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_LEFT - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	memset(cBuf, '\0', sizeof(cBuf));
	CSTKGetConfigInfo("PANEL", "CS_UDI2PANEL_SUPPORT_CTRL_RIGHT_CAPABILITY", cBuf, sizeof(cBuf));
	if(cBuf[0] != '\0')
	{
		g_CtrlFunMask[EM_UDIPANEL_RIGHT - EM_UDIPANEL_CTRLSTART] = CSTKGetIntFromStr(cBuf,10);
	}
	
	//显示灯的枚举数组
	g_aeLight[0] = EM_UDIPANEL_CTRLSTART;
	g_aeLight[EM_UDIPANEL_MAIL - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_MAIL;
	g_aeLight[EM_UDIPANEL_SIGNAL - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_SIGNAL;
	g_aeLight[EM_UDIPANEL_AUDIO - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_AUDIO;
	g_aeLight[EM_UDIPANEL_CLOCK- EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_CLOCK;  //add by xuhaitao for panel_timepoint 2011.11.22
	g_aeLight[EM_UDIPANEL_PLAY - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_PLAY;
	g_aeLight[EM_UDIPANEL_PAUSE - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_PAUSE;
	g_aeLight[EM_UDIPANEL_STOP - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_STOP;
	g_aeLight[EM_UDIPANEL_HDTV - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_HDTV;
	g_aeLight[EM_UDIPANEL_BACKWARD - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_BACKWARD;
	g_aeLight[EM_UDIPANEL_FORWARD - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_FORWARD;
	g_aeLight[EM_UDIPANEL_BALL - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_BALL;
	g_aeLight[EM_UDIPANEL_DUBY - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_DUBY;
	g_aeLight[EM_UDIPANEL_PREVIOUS - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_PREVIOUS;
	g_aeLight[EM_UDIPANEL_NEXT - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_NEXT;
	g_aeLight[EM_UDIPANEL_IR - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_IR;
	g_aeLight[EM_UDIPANEL_USB - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_USB;
	g_aeLight[EM_UDIPANEL_N - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_N;
	g_aeLight[EM_UDIPANEL_VOD - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_VOD;
	g_aeLight[EM_UDIPANEL_POWER - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_POWER;
	g_aeLight[EM_UDIPANEL_TV - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_TV;
	g_aeLight[EM_UDIPANEL_NETWORK - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_NETWORK;
	g_aeLight[EM_UDIPANEL_RADIO - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_RADIO;
	g_aeLight[EM_UDIPANEL_CLOCK - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_CLOCK;
	g_aeLight[EM_UDIPANEL_MODEM - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_MODEM;
	g_aeLight[EM_UDIPANEL_REC - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_REC;
	g_aeLight[EM_UDIPANEL_UP - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_UP;
	g_aeLight[EM_UDIPANEL_DOWN - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_DOWN;
	g_aeLight[EM_UDIPANEL_LEFT - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_LEFT;
	g_aeLight[EM_UDIPANEL_RIGHT - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_RIGHT;
	g_aeLight[EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_CTRLALL;		
	
	g_aeLight[EM_UDIPANEL_TEXTSTART - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_TEXTSTART;		
	g_aeLight[EM_UDIPANEL_TEXT0 - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_TEXT0;
	g_aeLight[EM_UDIPANEL_TEXT1 - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_TEXT1;
	g_aeLight[EM_UDIPANEL_TEXT2 - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_TEXT2;
	g_aeLight[EM_UDIPANEL_TEXT3 - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_TEXT3;
	g_aeLight[EM_UDIPANEL_TEXT4 - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_TEXT4;	
	g_aeLight[EM_UDIPANEL_TEXTALL - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_TEXTALL;		

	g_aeLight[EM_UDIPANEL_SUITSTART - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_SUITSTART;		
	g_aeLight[EM_UDIPANEL_SUIT0 - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_SUIT0;		
	g_aeLight[EM_UDIPANEL_SUIT1 - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_SUIT1;		
	g_aeLight[EM_UDIPANEL_SUIT2 - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_SUIT2;		
	g_aeLight[EM_UDIPANEL_SUITALL - EM_UDIPANEL_CTRLSTART] = EM_UDIPANEL_SUITALL;		

	strcpy(g_sLight[0] , "CTRLSTART");
	strcpy(g_sLight[EM_UDIPANEL_MAIL - EM_UDIPANEL_CTRLSTART] , "MAIL");
	strcpy(g_sLight[EM_UDIPANEL_SIGNAL - EM_UDIPANEL_CTRLSTART] , "SIGNAL");
	strcpy(g_sLight[EM_UDIPANEL_AUDIO - EM_UDIPANEL_CTRLSTART] , "AUDIO");
	strcpy(g_sLight[EM_UDIPANEL_CLOCK- EM_UDIPANEL_CTRLSTART] , "CLOCK");  //add by xuhaitao for panel_timepoint 2011.11.22
	strcpy(g_sLight[EM_UDIPANEL_PLAY - EM_UDIPANEL_CTRLSTART] , "PLAY");
	strcpy(g_sLight[EM_UDIPANEL_PAUSE - EM_UDIPANEL_CTRLSTART] , "PAUSE");
	strcpy(g_sLight[EM_UDIPANEL_STOP - EM_UDIPANEL_CTRLSTART] , "STOP");
	strcpy(g_sLight[EM_UDIPANEL_HDTV - EM_UDIPANEL_CTRLSTART] , "HDTV");
	strcpy(g_sLight[EM_UDIPANEL_BACKWARD - EM_UDIPANEL_CTRLSTART] , "BACKWARD");
	strcpy(g_sLight[EM_UDIPANEL_FORWARD - EM_UDIPANEL_CTRLSTART] , "FORWARD");
	strcpy(g_sLight[EM_UDIPANEL_BALL - EM_UDIPANEL_CTRLSTART] , "BALL");
	strcpy(g_sLight[EM_UDIPANEL_DUBY - EM_UDIPANEL_CTRLSTART] , "DUBY");
	strcpy(g_sLight[EM_UDIPANEL_PREVIOUS - EM_UDIPANEL_CTRLSTART] , "PREVIOUS");
	strcpy(g_sLight[EM_UDIPANEL_NEXT - EM_UDIPANEL_CTRLSTART] , "NEXT");
	strcpy(g_sLight[EM_UDIPANEL_IR - EM_UDIPANEL_CTRLSTART] , "IR");
	strcpy(g_sLight[EM_UDIPANEL_USB - EM_UDIPANEL_CTRLSTART] , "USB");
	strcpy(g_sLight[EM_UDIPANEL_N - EM_UDIPANEL_CTRLSTART] , "N");
	strcpy(g_sLight[EM_UDIPANEL_VOD - EM_UDIPANEL_CTRLSTART] , "VOD");
	strcpy(g_sLight[EM_UDIPANEL_POWER - EM_UDIPANEL_CTRLSTART] , "POWER");
	strcpy(g_sLight[EM_UDIPANEL_TV - EM_UDIPANEL_CTRLSTART] , "TV");
	strcpy(g_sLight[EM_UDIPANEL_NETWORK - EM_UDIPANEL_CTRLSTART] , "NETWORK");
	strcpy(g_sLight[EM_UDIPANEL_RADIO - EM_UDIPANEL_CTRLSTART] , "RADIO");
	strcpy(g_sLight[EM_UDIPANEL_CLOCK - EM_UDIPANEL_CTRLSTART] , "CLOCK");
	strcpy(g_sLight[EM_UDIPANEL_MODEM - EM_UDIPANEL_CTRLSTART] , "MODEM");
	strcpy(g_sLight[EM_UDIPANEL_REC - EM_UDIPANEL_CTRLSTART] , "REC");
	strcpy(g_sLight[EM_UDIPANEL_UP - EM_UDIPANEL_CTRLSTART] , "UP");
	strcpy(g_sLight[EM_UDIPANEL_DOWN - EM_UDIPANEL_CTRLSTART] , "DOWN");
	strcpy(g_sLight[EM_UDIPANEL_LEFT - EM_UDIPANEL_CTRLSTART] , "LEFT");
	strcpy(g_sLight[EM_UDIPANEL_RIGHT - EM_UDIPANEL_CTRLSTART] , "RIGHT");
	strcpy(g_sLight[EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART] , "CTRLALL");		
	
	strcpy(g_sLight[EM_UDIPANEL_TEXTSTART - EM_UDIPANEL_CTRLSTART] , "TEXTSTART");		
	strcpy(g_sLight[EM_UDIPANEL_TEXT0 - EM_UDIPANEL_CTRLSTART] , "TEXT0");
	strcpy(g_sLight[EM_UDIPANEL_TEXT1 - EM_UDIPANEL_CTRLSTART] , "TEXT1");
	strcpy(g_sLight[EM_UDIPANEL_TEXT2 - EM_UDIPANEL_CTRLSTART] , "TEXT2");
	strcpy(g_sLight[EM_UDIPANEL_TEXT3 - EM_UDIPANEL_CTRLSTART] , "TEXT3");
	strcpy(g_sLight[EM_UDIPANEL_TEXT4 - EM_UDIPANEL_CTRLSTART] , "TEXT4");	
	strcpy(g_sLight[EM_UDIPANEL_TEXTALL - EM_UDIPANEL_CTRLSTART] , "TEXTALL");		

	strcpy(g_sLight[EM_UDIPANEL_SUITSTART - EM_UDIPANEL_CTRLSTART] , "SUITSTART");		
	strcpy(g_sLight[EM_UDIPANEL_SUIT0 - EM_UDIPANEL_CTRLSTART] , "SUIT0");		
	strcpy(g_sLight[EM_UDIPANEL_SUIT1 - EM_UDIPANEL_CTRLSTART] , "SUIT1");		
	strcpy(g_sLight[EM_UDIPANEL_SUIT2 - EM_UDIPANEL_CTRLSTART] , "SUIT2");		
	strcpy(g_sLight[EM_UDIPANEL_SUITALL - EM_UDIPANEL_CTRLSTART] , "SUITALL");	
}

