
#ifndef __CS_WINDOWS_H
#define __CS_WINDOWS_H

#ifdef  __cplusplus
extern "C" {
#endif
#ifdef WIN32
	#define GUIDLL_EXPORTS
	#ifdef GUIDLL_EXPORTS
	#define GUIDLL_API __declspec(dllexport)
	#else
	#define GUIDLL_API __declspec(dllimport)
	#endif
#else
	#define GUIDLL_API 
#endif
#define UNALIGNED

#ifndef _MSC_VER
#define _MSC_VER 0x0     //versions of vc the compiler don't use
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x500		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0501	//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0400 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0500	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif


#ifndef _WIN32_WCE
#define _WIN32_WCE   0x0400
#endif

#ifndef ISOLATION_AWARE_ENABLED
#define ISOLATION_AWARE_ENABLED 0
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs_typedef.h"
#include "cs_assert.h"

#include "CS_WinDef.h"
#include "CS_GDIDDK.h"
#include "CS_WINGDI.h"
#include "CS_WINUser.h"
#include "CS_CommCtrl.h"
#include "CS_WINError.h"
#include "CS_HotVKey.h"
#include "CS_DefaultGUIObject.h"
#include "CS_GUIInfo.h"

#ifdef  __cplusplus
}
#endif

#endif
