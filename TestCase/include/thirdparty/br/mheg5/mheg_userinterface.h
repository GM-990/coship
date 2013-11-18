#ifndef _CLIFE_INTERFACE_H_
#define _CLIFE_INTERFACE_H_

#include "CS_Assert.h"
#include "cs_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

//MHEG5����״̬
typedef enum _MHEG_RUNNING_STATUS
{
	EM_MHEG_RUNNING_STATUS_STOP,
	EM_MHEG_RUNNING_STATUS_START,
	EM_MHEG_RUNNING_STATUS_SHOW,
	EM_MHEG_RUNNING_STATUS_HIDE
}EMHEG_RUNNING_STATUS;

//app->MHEG5 ����ӳ���
typedef enum _MHEG_VKEY
{
	EM_MHEG_VK_UNDEFINED = 0,	//not use
	EM_MHEG_VK_RED,   				//VK_RED
	EM_MHEG_VK_GREEN,     			//VK_GREEN
	EM_MHEG_VK_YELLOW,  			//VK_YELLOW
	EM_MHEG_VK_BLUE,  				//VK_BLUE
	EM_MHEG_VK_EXIT,       			//VK_EXIT
	EM_MHEG_VK_TEXT,       			//VK_TEXT
	EM_MHEG_VK_LEFT,    			//VK_LEFT
	EM_MHEG_VK_RIGHT,  			//VK_RIGHT
	EM_MHEG_VK_UP,  				//VK_UP
	EM_MHEG_VK_DOWN,   			//VK_DOWN
	EM_MHEG_VK_OK,   				//VK_OK
	EM_MHEG_VK_0,   				//VK_0
	EM_MHEG_VK_1,					//VK_1
	EM_MHEG_VK_2,					//VK_2
	EM_MHEG_VK_3,					//VK_3
	EM_MHEG_VK_4,					//VK_4
	EM_MHEG_VK_5,					//VK_5
	EM_MHEG_VK_6,					//VK_6
	EM_MHEG_VK_7,					//VK_7
	EM_MHEG_VK_8,					//VK_8
	EM_MHEG_VK_9,					//VK_9
	EM_MHEG_VK_EPG				//VK_EPG
}MHEG_VKEY; 

/*==========================================================
����ԭ��:CSPLF_MHEGInit();
���� : ��ʼ��MHEG5�м��
����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ��
=========================================================== */
BOOL CSPLF_MHEGInit(void);

/*==========================================================
����ԭ��:CSPLF_MHEGStart();
���� : ����MHEG5�м��
����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ��
=========================================================== */
BOOL CSPLF_MHEGStart(void);

/*==========================================================
����ԭ��:CSPLF_MHEGStop();
���� : ֹͣMHEG5�м��
����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ��
=========================================================== */
BOOL CSPLF_MHEGStop(void);

/*==========================================================
����ԭ��:CSPLF_MHEGEventSender();
���� : ���Ͱ�����Ϣ��MHEG5�м��
����ֵ��0��ʾ�ɹ�MHEG5Ӧ�ô����˴˼���-1��ʾʧ��
ע��:Ҫ�Ƿ���ʧ��Ӧ��Ӧ�ô�����Ӧ����ʧ�ܵİ���
=========================================================== */
int CSPLF_MHEGEventSender(int nMHEGKey);

/*==========================================================
����ԭ��:CSPLF_MHEGShow();
���� : ��ʾMHEG5�м��OSD
����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ��
=========================================================== */
BOOL CSPLF_MHEGShow(void);

/*==========================================================
����ԭ��:CSPLF_MHEGHide();
���� : ����MHEG5�м��
����ֵ��TRUE��ʾ�ɹ���FALSE��ʾʧ��
=========================================================== */
BOOL CSPLF_MHEGHide(void);

EMHEG_RUNNING_STATUS CSPLF_GetMHEGStatus(void);
void CSPLF_MHEGVersion(void);
#ifdef __cplusplus
}
#endif

#endif

