#ifndef _CSSHELL_HOTKEY_H_
#define _CSSHELL_HOTKEY_H_

#include "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
处理热键应用函数，注册给ircode中的热键处理回调函数
*/
 void CSHotkKey_ProcessHotkeyApp(DWORD dwKeyState, int nKeyCode, void * pvUserData);


/**
描述：注册热键应用
参数：keyCode - Number类型，为热键键值，具体见川网规范。
	orgID - Number类型，为热键应用的组织标识。
      appID - Number类型，为热键应用的应用标识。
      params - String类型，启动热键应用的参数。
      注： orgID 、appID都为0 表示主应用。
返回：boolean类型，true - 注册成功；false - 注册失败。
说明：
	该接口只能由默认应用(主应用)调用。
	中间件在处理热键时，如果当前应用是浏览器应用，热键发送到浏览器，浏览器将热键消息发往页面，如果页面处理并拦截了按键消息(事件处理函数返回false)，则中间件不启动热键应用；如果页面没有处理按键消息或不需要拦截该消息(事件处理函数返回true)，则中间件启动热键应用。如果当前应用是Java应用，中间件向java应用发送一个退出键，退出java，然后中间件再跳转到热键注册的页面。
	如果对同一个keyCode多次注册不同的热键应用，会覆盖上一次注册的热键应用。
*/
BOOL  CSHotkKey_RegisterHotKeyApp(int nKeyCode, int nOrgId, int nAppId, char* pcParams);

/**
描述：删除热键应用。
参数：keyCode - Number类型，为热键键值，具体见川网规范
返回：无
	说明：当页面不在需要改热键的处理后，页面需要调用该接口，清除热键的注册。该接口只能由默认应用(主应用)调用。
*/
void  CSHotkKey_UnRegisterHotKeyApp(int nKeyCode);

/**
描述：启动热键对应的应用。
参数：keyCode - Number类型，为热键键值，具体见川网规范。
返回：无。
说明：如果对应按键无热键应用，中间件不进行处理。
*/
BOOL CSHotkKey_StartHotKeyApp(int nKeyCode);

#ifdef __cplusplus
}
#endif


#endif  

