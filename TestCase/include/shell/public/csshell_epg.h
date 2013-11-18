
#ifndef CSSHELL_EPG_H
#define CSSHELL_EPG_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "EPGReceiverInterface.h"
#include "SectionInterface2C.h"
#include "MemoryPoolInterface.h"

#define CSSHELL_EPG_MAX_TAG_COUNT 8

/**@brief ������ʾEPG�ķ��෽ʽ*/
typedef enum _CSShellEPGLevelType
{
	EPGTYPE_NONE,            //�������๦��
	EPGTYPE_LEVEL1_PRESENT,  //�Ե�ǰ��Ϣ���з���
	EPGTYPE_LEVEL1_SCHEDULE  //��������Ϣ���з���
}CSShellEPGLevelType_E;

typedef int (*CSShellEpgExtProc_F)(EPGCallbackType emType, void *pParam);

typedef void (*CSShellEpgPriInit_F)(HCSHANDLE hEpgHandle);

/**@brief EPG��ʼ������ */
typedef struct _CSShellEpgInitParam
{
	int                    m_nSize;                    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellEpgInitParam_S)
	void*                  m_pvUserData;                            ///< �û����ݣ��˲��������ű��ṹ�еĺ���ָ��һ�𴫵ݸ�����ʵ��

	//EPG�������ĵ�ַ�ʹ�С
	PBYTE        m_pbMemAddr;
	int          m_nMemSize;

	//��Ҫ/һ��EPG������/�������
	int          m_nPFRecvPolicy;
	int          m_nPFSavePolicy;
	int          m_nScheduleRecvPolicy;
	int          m_nScheduleSavePolicy;

	//Ҫ����Ļص���Ϣ����
	int 		 m_nRegCallbackType;

	//������ʱ����
	int          m_nTimeOut;

	//��Ŀ������Ϣ��Դ
	int          m_nDescriptionFrom;	
	//pf event�Ľ�Ŀ���������ͱ������
	int          m_nPFDescriptionPolicy;
	//schedule event��Ŀ���������ͱ������
	int          m_nScheduleDescriptionPolicy;

	//�����Ƿ���ʾ
	BOOL         m_bPastShown;

	//������ʾ��ʽ
	CSShellEPGLevelType_E          m_eTypeMode;

	//������ʾ���軺����
	PBYTE        m_pbTypeMemAddr;
	int          m_nTypeMemSize;

	//����ʱ�䷶Χ:�ӵ�ǰʱ����XXmin.
	int          m_nTypeDuraMin;
	//ʱ��
	float 		m_fTimezone;

    //EPG��չ����֧��,�ûص�����EPG�ļ�ӻص���ֱ�ӻص���dtvm native��ʵ��
	CSShellEpgExtProc_F m_fnExpProc;

	//P/F PEG�豣�����������Ϣ
	int				m_nSimpleTagCount;                         ///< �豣�������������
	BYTE			m_aucSimpleTags[CSSHELL_EPG_MAX_TAG_COUNT]; ///< �豣���������Tags

	//Schedule PEG�豣�����������Ϣ
	int				m_nScheduleTagCount;                         ///< �豣�������������
	BYTE			m_aucScheduleTags[CSSHELL_EPG_MAX_TAG_COUNT]; ///< �豣���������Tags
	CSShellEpgPriInit_F m_fnPrivateInit;		///<�ûص���EPG��ʼ��ʱ�����ã�ʹӦ�ÿ�������һЩ˽�е�����

	//�Ƿ�ȡ��ԭʼ��Ŀ������ͱ�־λ
    BOOL        m_bGetOriginalLanguage;

}CSShellEpgInitParam_S;

/*********************���úͻ�ȡpf epg *******************************/
/**
@brief ����EPG��ʼ������
@param psEpg, ��ʼ������
@return �ɹ�����FALSE��ʧ�ܷ���TRUE
*/
BOOL CSShellEpgSetPFParam(const CSShellEpgInitParam_S* psEpg);

/**
@brief ��ȡ��ʼ������epg���
@param ��
@return ����epg��������epgδ��ʼ�����򷵻�NULL
*/
HCSHANDLE CSShellEpgGetPFHandle();
/*********************end  pf epg *************************************/


/*********************���úͻ�ȡSchedule epg *******************************/
/**
@brief ����EPG��ʼ������
@param psEpg, ��ʼ������
@return �ɹ�����FALSE��ʧ�ܷ���TRUE
*/
BOOL CSShellEpgSetScheduleParam(const CSShellEpgInitParam_S* psEpg);

/**
@brief ��ȡ��ʼ������epg���
@param ��
@return ����epg��������epgδ��ʼ�����򷵻�NULL
*/
HCSHANDLE CSShellEpgGetScheduleHandle();
/*********************end  Schedule epg *************************************/

/**
@brief ��ȡschedule sort epg ���
@param ��
@return schedule sort epg ���
*/
HCSHANDLE CSShellEpgGetScheduelSortHandle();

/*********************end  pf epg *************************************/

/**
@brief ��ȡp sort epg ���
@param ��
@return p sort epg ���
*/
HCSHANDLE CSShellEpgGetPSortHandle();


/**
@brief ��ȡf sort epg ���
@param ��
@return f sort epg ���
*/
HCSHANDLE CSShellEpgGetFSortHandle();

#ifdef __cplusplus
}
#endif

#endif  /* SHELL_EITLOADER_H */




