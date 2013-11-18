#ifndef WEBKIT_PROTOCOL_EXT_H
#define WEBKIT_PROTOCOL_EXT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

typedef enum _SelectStates_E
{
	EM_SELECT_WAIT = 1,
	EM_SELECT_READY,
	EM_SELECT_FAIL,
	EM_SELECT_NO_OBJ,
	EM_SELECT_NO_ERROR,   //  正常返回，正常处理
	EM_SELECT_START_LOAD  // 开始下载，数据可读   
}SelectStates_E;

typedef struct _GTRegProtocol_S
{
	char acProtocolHead[16];  // 协议头 如: "dvb"
	int (*open)(const char * pcUrl, const char * pcMode, int nTimeout); // url 地址,返回索引
	SelectStates_E (*select)(int nIndex, int nTimeout); // 轮询状态
	int (*recv)(int nIndex, void *pvBuffer, unsigned int unSize); // 读取数据
	int (*size)(int nIndex); // 关闭索引
	int (*close)(int nIndex); // 关闭索引
	int (*exit)(void); // 退出浏览器调用，清理注册协议资源
}GTRegProtocol_S;

BOOL CSGTRegisterProtocol(GTRegProtocol_S *pstGTProtocol);
BOOL CSGTUnRegisterProtocol(char *pProtocolHead);

#ifdef __cplusplus
}
#endif

#endif // QFILENETWORKREPLY_P_H
