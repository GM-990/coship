#ifndef __CS_GUIOS_H
#define __CS_GUIOS_H

//modified by xia chao 2006.9.13, 去掉部分平台相关的宏定义

#ifdef  __cplusplus
extern "C" {
#endif
// Standard constants

#ifndef ENDIAN_BIG    //if the cpu is big endian
	#define ENDIAN_BIG	1
#endif

#ifndef ENDIAN_LITTLE    //if the cpu is little endian
	#define ENDIAN_LITTLE	0
#endif

#ifndef ALIGIN4           //if #pragma pack(1) has not effect,the operate sizeof can't aligned by byte
	#define ALIGIN4	1
#endif

#ifndef ALIGIN1           //if #pragma pack(1) has  effect,the operate sizeof can aligned by byte
	#define ALIGIN1	0
#endif

#ifndef PC
	#define PC		0
#endif

#ifndef LSI
	#define LSI		1
#endif

#ifndef ST
	#define ST		2
#endif

#ifndef LINUX
	#define LINUX	3
#endif

#ifndef NEC
	#define NEC	    4
#endif

#ifndef CNX
	#define CNX	    5
#endif

#ifndef CT
	#define CT	    6
#endif


#if ( GUI_VIDEOHARD == NEC ||  GUI_VIDEOHARD == CT)
	#define SWITCHSHOWBUF               //if you want gui use it's own buffer,recomment it;
#endif

//#define NEED_NOTIFY_BUFFER_CHANGE   //if you want go notify the screen buffer cahnge,define it


#ifndef GUI_VIDEOHARD

	#define GUI_VIDEOHARD	PC				//set the videohard to your plantform
	#define ATUOTEST     TRUE

	#define GUI_GB2312_UNICODE              //define for font 
	#define GUI_BIG5_UNICODE
	#define GUI_ISO6937_UNICODE

	#define GUI_SCREENCOLORFORMAT_8			//define for display mode	
	#define GUI_SCREENCOLORFORMAT_16
	#define GUI_SCREENCOLORFORMAT_24
	#define GUI_SCREENCOLORFORMAT_32

#endif

#ifndef PRAGMA_PACK_ORDER
#if (GUI_VIDEOHARD == ST || GUI_VIDEOHARD == CNX)
	#define PRAGMA_PACK_ORDER      ALIGIN4
#else
	#define PRAGMA_PACK_ORDER      ALIGIN1
#endif
#endif

int GetCurrentByteOrder(void);

#ifdef  __cplusplus
}
#endif

#endif
