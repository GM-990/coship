
#ifndef CSSHELL_EPG_PRIVATE_H
#define CSSHELL_EPG_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_epg.h"
#include "mmcp_typedef.h"
#include "CSEPGSortInterface.h"

/*********************���úͻ�ȡpf epg *******************************/
/**
@brief ��ȡ���õ�EPG����
@param ��
@return EPG��ʼ������
@note �ú���Ӧ����dtvm native����.
*/
const CSShellEpgInitParam_S* CSShellEpgGetPFParam();

/**
@brief ��ʼ��epg
@param[in] callback epg�ص����ûص���dtvm native��ʵ��
@return �ɹ��򷵻�֮ǰ��ʼ������epg�����ʧ���򷵻�NULL
@note ����Ѿ���ʼ����epg����ֱ�ӷ����ϴγ�ʼ������epg�����
*/
HCSHANDLE CSShellEpgPFInit(EPGCallBack callback );

/*********************end ���úͻ�ȡpf epg *******************************/


/*********************���úͻ�ȡschedule epg *******************************/
/**
@brief ��ȡ���õ�EPG����
@param ��
@return EPG��ʼ������
@note �ú���Ӧ����dtvm native����.
*/
const CSShellEpgInitParam_S* CSShellEpgGetScheduleParam();

/**
@brief ��ʼ��epg
@param[in] callback epg�ص����ûص���dtvm native��ʵ��
@return �ɹ��򷵻�֮ǰ��ʼ������epg�����ʧ���򷵻�NULL
@note ����Ѿ���ʼ����epg����ֱ�ӷ����ϴγ�ʼ������epg�����
*/
HCSHANDLE CSShellEpgScheduleInit(EPGCallBack callback ,EPGLevelGCallBack sortCallback);

/*********************end ���úͻ�ȡschedule epg *******************************/


/**
@brief ����schedule epg (pf��ռ�ÿռ�С����������)
@param ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellEpgScheduleDestroy();



#ifdef __cplusplus
}
#endif

#endif  /* SHELL_EITLOADER_H */




