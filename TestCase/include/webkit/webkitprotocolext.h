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
	EM_SELECT_NO_ERROR,   //  �������أ���������
	EM_SELECT_START_LOAD  // ��ʼ���أ����ݿɶ�   
}SelectStates_E;

typedef struct _GTRegProtocol_S
{
	char acProtocolHead[16];  // Э��ͷ ��: "dvb"
	int (*open)(const char * pcUrl, const char * pcMode, int nTimeout); // url ��ַ,��������
	SelectStates_E (*select)(int nIndex, int nTimeout); // ��ѯ״̬
	int (*recv)(int nIndex, void *pvBuffer, unsigned int unSize); // ��ȡ����
	int (*size)(int nIndex); // �ر�����
	int (*close)(int nIndex); // �ر�����
	int (*exit)(void); // �˳���������ã�����ע��Э����Դ
}GTRegProtocol_S;

BOOL CSGTRegisterProtocol(GTRegProtocol_S *pstGTProtocol);
BOOL CSGTUnRegisterProtocol(char *pProtocolHead);

#ifdef __cplusplus
}
#endif

#endif // QFILENETWORKREPLY_P_H
