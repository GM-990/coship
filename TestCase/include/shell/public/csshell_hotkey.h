#ifndef _CSSHELL_HOTKEY_H_
#define _CSSHELL_HOTKEY_H_

#include "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
�����ȼ�Ӧ�ú�����ע���ircode�е��ȼ�����ص�����
*/
 void CSHotkKey_ProcessHotkeyApp(DWORD dwKeyState, int nKeyCode, void * pvUserData);


/**
������ע���ȼ�Ӧ��
������keyCode - Number���ͣ�Ϊ�ȼ���ֵ������������淶��
	orgID - Number���ͣ�Ϊ�ȼ�Ӧ�õ���֯��ʶ��
      appID - Number���ͣ�Ϊ�ȼ�Ӧ�õ�Ӧ�ñ�ʶ��
      params - String���ͣ������ȼ�Ӧ�õĲ�����
      ע�� orgID ��appID��Ϊ0 ��ʾ��Ӧ�á�
���أ�boolean���ͣ�true - ע��ɹ���false - ע��ʧ�ܡ�
˵����
	�ýӿ�ֻ����Ĭ��Ӧ��(��Ӧ��)���á�
	�м���ڴ����ȼ�ʱ�������ǰӦ���������Ӧ�ã��ȼ����͵����������������ȼ���Ϣ����ҳ�棬���ҳ�洦�������˰�����Ϣ(�¼�����������false)�����м���������ȼ�Ӧ�ã����ҳ��û�д�������Ϣ����Ҫ���ظ���Ϣ(�¼�����������true)�����м�������ȼ�Ӧ�á������ǰӦ����JavaӦ�ã��м����javaӦ�÷���һ���˳������˳�java��Ȼ���м������ת���ȼ�ע���ҳ�档
	�����ͬһ��keyCode���ע�᲻ͬ���ȼ�Ӧ�ã��Ḳ����һ��ע����ȼ�Ӧ�á�
*/
BOOL  CSHotkKey_RegisterHotKeyApp(int nKeyCode, int nOrgId, int nAppId, char* pcParams);

/**
������ɾ���ȼ�Ӧ�á�
������keyCode - Number���ͣ�Ϊ�ȼ���ֵ������������淶
���أ���
	˵������ҳ�治����Ҫ���ȼ��Ĵ����ҳ����Ҫ���øýӿڣ�����ȼ���ע�ᡣ�ýӿ�ֻ����Ĭ��Ӧ��(��Ӧ��)���á�
*/
void  CSHotkKey_UnRegisterHotKeyApp(int nKeyCode);

/**
�����������ȼ���Ӧ��Ӧ�á�
������keyCode - Number���ͣ�Ϊ�ȼ���ֵ������������淶��
���أ��ޡ�
˵���������Ӧ�������ȼ�Ӧ�ã��м�������д���
*/
BOOL CSHotkKey_StartHotKeyApp(int nKeyCode);

#ifdef __cplusplus
}
#endif


#endif  

