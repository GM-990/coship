#include "setup.h"


extern void *  CSBSendMessageToBrowser(char * url,int nUrlLen);
extern void* MsgBoxCallBack(CSGTRet_E enType);


#ifdef __cplusplus
{
	extern "C" int startJavaApp(char* classpath, char* mainclass, char* entrymain, char* args);
	extern "C" void ioctlWrite(char * key, char * value, int nValueLen);
	extern "C" void ioctlRead(char * key, char *szReadBuf, int nBufLen);
}
#endif	

/*int startJavaApp(char* classpath, char* mainclass, char* entrymain, char* args)
{
	return 1;
}
void ioctlWrite(char * key, char * value,int nValueLen)
{
	return;
}
void ioctlRead(char * key, char *szReadBuf, int nBufLen)
{
	return;
}*/

/*�������ʼ��*/
void SetupGUITool(void)
{
	//�����ڲ�����г�ʼ��������Ҫ
}



