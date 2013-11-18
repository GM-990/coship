#ifndef _KERNEL_LOG_COLLECT_H
#define _KERNEL_LOG_COLLECT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
	������	CSLogCollectInit
	������	���������־�ռ����ߵĳ�ʼ��������������java�Ĺ��̳�ʼ��
	���룺
			��
	�����
			��
	���أ�
			��
*/
void CSLogCollectInit(void);

/*
	������	CSLogCollectInit
	������	���������־�ռ����ߵĳ�ʼ������������û��java�Ĺ��̳�ʼ���������jni�ӿڵ�����
	���룺
			��
	�����
			��
	���أ�
			��
*/
void CSLogCollectInitSimple(void);

/*
	������	CSLogCollecDispatchMsg
	������	ͨ�����硢usb���ַ�ʽ���зַ���־��Ϣ
	���룺
			pcMsg - ��־��Ϣ
	�����
			��
	���أ�
			��
*/
void CSLogCollectDispatchMsg(const char * pcString);

/*
	������	CSLogCollectSetDebug
	������	���ô�ӡ����
	���룺
			pcDebugStr - ��ӡ������Ϣ
			nLength - pcDebugStr�ĳ���
	�����
			��
	���أ�
			��
	��ע��
			����ģ��Ĵ�ӡ������Ϣ���ַ�����ʽ���룬���ʽ���£�

			Cģ��Ĵ�ӡ��ʽ��
			DbgL.ģ����=��ӡ������

			Cģ���ӡ����
			0��DEBUG_LEVEL
			1��INFO_LEVEL
			2��NOTICE_LEVEL
			3��WARN_LEVEL
			4��ERROR_LEVEL
			5��FATAL_LEVEL
			6��CLI_LEVEL
			7��DISABLE_LEVEL

			�����е�Cģ������Ϊͬһ��ӡ����ĸ�ʽ���£�
			DbgL.default=��ӡ������

			ע�⣺���޸Ĵ�ӡ����ʱ������ӡ������Ϣ�����ޡ�DbgL.default��������Ĭ�ϴ�ӡ�������Ƚ����е�Cģ������Ϊ��DISABLE_LEVEL����Ȼ������������עģ��Ĵ�ӡ����

			Javaģ��Ĵ�ӡ��ʽ��
			-Dģ����.debug=��ӡ�������

			Javaģ���ӡ����
			0��DEBUG_LEVEL
			1��INFO_LEVEL
			2��WARN_LEVEL
			3��ERROR_LEVEL


			�����ӡ��ʽ��ӡ������Ϣʾ����
			DbgL.default=5
			DbgL.Java=0
			DbgL.CVM=0
			DbgL.JSPrint=0
			-Dappmanager.debug=3
			ע�⣺��ͬģ��֮���Իس����з�(\n��\r\n))���зָ��ĳһģ�鱻�����˶�Σ������һ�δ���
*/
void CSLogCollectSetDebug(const char * pcDebugStr, int nLength);

#ifdef __cplusplus
}
#endif

#endif

