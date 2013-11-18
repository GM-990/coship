#ifndef QNETWORKPROTOCOL_P_H
#define QNETWORKPROTOCOL_P_H

typedef enum _ntProtocolState
{
	EM_NT_SELECT_WAIT = 1,
	EM_NT_SELECT_READY,
	EM_NT_SELECT_FAIL,
	EM_NT_SELECT_NO_OBJ,
	EM_NT_SELECT_NO_ERROR,   //  正常返回，正常处理
	EM_NT_SELECT_START_LOAD  // 开始下载，数据可读   
}ntProtocolState;

typedef bool(*ntProtocolisExistScheme_F)(const char *scheme);

typedef int (*ntProtocolOpen_F)(const char *scheme, const char *url);
typedef ntProtocolState (*ntProtocolSelect_F)(const char *scheme, int index, int timeout);
typedef int (*ntProtocolRecv_F)(const char *scheme, int index, void *buffer, unsigned int size);
typedef int (*ntProtocolClose_F)(const char *scheme, int index);
typedef int (*ntProtocolsize_F)(const char *scheme, int index);

typedef struct _NTProtocolRegister 
{
	ntProtocolisExistScheme_F m_isExistScheme;
	ntProtocolOpen_F m_open;
	ntProtocolSelect_F m_select;
	ntProtocolRecv_F m_recv;
	ntProtocolClose_F m_close;
	ntProtocolsize_F m_size;
}ntProtocolRegister;

bool registerCommonProtocolFuns(ntProtocolRegister *funs);

#endif // QFILENETWORKREPLY_P_H
