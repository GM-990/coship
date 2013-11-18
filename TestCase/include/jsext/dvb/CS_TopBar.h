/*************************************************************************
* Copyright (c) 2006,ShenZhen Coship Electronic Ltd Co.
* All rights reserved.
* 
* File Name：CS_TopBar.h
*
* Summary： from ESP, zhangshm modify for proc-ctrl
*
**************************************************************************/
#ifndef _CS_TopBar_H_
#define _CS_TopBar_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
typedef struct _CSTopBarMsg
{
	DWORD	dwMsgType;							//消息类型
	DWORD	dwMsgSubType;						//消息内容，根据消息类型不同分别为：
	int nTimeOut;								//超时时间（ms），超过这个时间TopBar会自动消隐，如果为-1，则默认为不超时，等待事件触发将其消失，只对ICON和ALARM消息有效
	BOOL    bMsgSubTypeCoexist;					//是否可以与其他TopBar显示的元素共存,主要用于CA中的COMMAND
	char    acShowStr[256];                     // topbar显示的字符串内容
}CSTopBarMsg;

/*	函数名：CSTopBarMsgRegister
 *	功能：注册用户自定义的消息
 *	参数说明：
 *	pstTopBarMsg：用户自定义的消息；
 *	nMsgCount：消息个数；
 */
BOOL CSTopBarMsgRegister( CSTopBarMsg* pstTopBarMsg, int nMsgCount );

/*	函数名：CSTopBarClear
 *	功能：清除topbar模块已存在的topbar消息
 *	参数说明：
 *	    无
 */
void CSTopBarClear(DWORD dwMsgType, DWORD dwMsgSubType);

/*	函数名：CSTopBarMsgProc
 *	功能：TopBar处理程序
 *	参数说明：
 *	dwMsgType：发送消息的消息类型
 *     pTopBarPostMsg 要显示的消息
 *	bShow：显示隐藏标志
 */
void CSTopBarMsgProc(DWORD dwMsgType,CSTopBarMsg *pTopBarPostMsg, BOOL bShow);


/*	函数名：CSTopBarGetRequireDisplayCount
*	功能：获取当前需要显示的Topbar 个数
*/
int CSTopBarGetRequireDisplayCount(void);

/*	函数名：CSTopBarGetRequireDisplayItemInfo
*	功能：获取当前需要显示的某一项Topbar 的内容
*     参数说明: 
			nIndex-索引
			nProperty -属性：0-MsgID ; 1-MsgContent; 2-Timeout; 3-Msg Rect
			pParam - 返回值，根据nProperty的定义返回不同的变量，如整型，字符串，结构体等
			nDataLen - 返回值的Buffer长度
*/
int CSTopBarGetRequireDisplayItemInfo(int nIndex,int nProperty, void *pParam, int nDataLen);

/*	函数名：CSTopBarSetRegisterItemParam
*	功能：设置某一项已注册的Topbar 的内容
*     参数说明: 
			nMsgID-已注册的消息ID
			nProperty -属性：0-MsgID ; 1-MsgContent; 2-Timeout; 3-Msg Rect
			pParam - 设置的具体值，如整型，字符串，结构体等
			nDataLen - 返回值的Buffer长度
			
*/
int CSTopBarSetRegisterItemParam(int dwMsgType,int nProperty, void *pParam, int nDataLen);

int CSGetTopBarInfoIndex(int dwMsgType);

/*	函数名：CSJSMsgProc
 *	功能：向JS发送消息
 *	输入参数：
 *	    dwMessageType：发送的消息的类型，取值范围参见JSMessage.h头文件定义
 *	    caJsonStr：与消息类型相应的消息的附属信息，以json字符串的形式表示，如果不需要此参数，则传0;
 *	               要注意，此字符串的长度最长不能超过1000个字节，否则后面的会被截断;
 *                 另: 如果caJsonStr不为空,本接口内部会拷贝caJsonStr的内容，故此接口返回后，
 *                   caJsonStr指向的地址空间不再被本函数使用;
 *	    nLength：caJsonStr字符串的长度
 *  输出参数：
 *      无
 *  返回值：
 *      0表示成功，其他值表示失败
 *  注意点：
 *      caJsonStr字符串中必须有type属性，其值为dwMessageType
 */
int CSJSMsgProc(DWORD dwMessageType, const char* caJsonStr, int nLength);

/*	函数名：CSJSMsgProcEx
 *	功能：向多page场景下指定页面JS发送消息
 *	输入参数：
 *		hPage:浏览器接口创建的多page句柄
 *	    dwMessageType：发送的消息的类型，取值范围参见JSMessage.h头文件定义
 *      pcJsonStr：与消息类型相应的消息的附属信息，以json字符串的形式表示，如果不需要此参数，则传NULL;
 *	               要注意，此字符串的长度最长不能超过1000个字节，否则后面的会被截断;
 *                 另: 如果caJsonStr不为空,本接口内部会拷贝caJsonStr的内容，故此接口返回后，
 *                   caJsonStr指向的地址空间不再被本函数使用; *	    
 *      nLength：caJsonStr字符串的长度
 *  输出参数：
 *      无
 *  返回值：
 *      0表示成功，其他值表示失败
 *  注意点：
 *      caJsonStr字符串中必须有type属性，其值为dwMessageType
 */
int CSJSMsgProcEx(HCSHANDLE hPage, DWORD dwMessageType, const char* pcJsonStr, int nLength);

#ifdef __cplusplus
}
#endif

#endif
