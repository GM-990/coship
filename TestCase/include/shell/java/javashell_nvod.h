#ifndef SHELL_NVOD_H
#define SHELL_NVOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

#define NVODMGR_MAX_DELIVER_COUNT 8

/**
* nvod��ʼ�����ò���
*/
typedef struct _CSNvodParam_S
{
	/**
	*NVOD����Ƶ�����
	*/
	int         m_nDeliverCount;

	/**
	* nvod�����ĸ���Ƶ��
	*/
	Deliver    m_stDeliver[NVODMGR_MAX_DELIVER_COUNT];

	/**�洢tshiftEvent���ݵ�����*/
	int 		m_nEventDurationDays;

	/**�Ƿ�ǿ��ָ�����������ݹ�������*/
	BOOL 	m_bSearchDependDM;

	/**"��ʾNIT�����Ƿ����Servcie_List_Descriptor��������*/
	BOOL 	m_bSearchNitList;

	/**������ʱʱ�䣬��λΪms*/
	int 		m_nSearchTimeOutMs;

	/**
		* "true"��ʾǰ�˰�TimeShiftEvent������TableID ����0x4e��0x4f�ı��з��͡� 
		* "false"��ʾǰ�˰�TimeShiftEvent������TableID ����0x50-0x5f��0x60-0x6f�ı��з��͡�
	**/
	BOOL 	m_bIsScheduleToSimple;

	/**��ʾNIT�����Ƿ����Servcie_List_Descriptor��������*/
	BOOL 	m_bIsSearchScheduleEIT;

	/**Ĭ��Ƶ���Ƿ�ΪNVODƵ��
	 *�˲����ڲ���tuneParams����ʱ���á�
	 *����true����ʾ����tuneParams�е�Ƶ�����NVODƵ�㣬��������������в�����NVODƵ�㡣
	 *����false����ʾ�û���֪������tuneParams�е�Ƶ���Ƿ����NVODƵ�㣬�������������������NVODƵ�㡣
	 **/
	BOOL 	m_bIsNVODFreq;

	/**
	 * �Ƿ�����EIT OTHER��,
	 * ����NVODƵ�㲻��EIT OTHER����������ֻ����ǰƵ��NVOD��Ϣ��,Ӧ�ý�����Ϊfalse
	**/
	BOOL 	m_bIsFindOther;

	/**ÿ��NVOD��������Ҫ��Ҫɾ���ϵ�����
	 * ����ж��NVODƵ�㣬��ÿ��Ƶ�㶼ֻ���Լ�Ƶ���NVOD��Ϣ����Ӧ�ý����������Ϊfalse
	 * ����ᵼ���Ѿ�������������Ƶ������ݱ�ɾ����
	  *Ҳ����˵ֻ��ÿ��NVODƵ�㶼������NVODƵ����Ϣ��ȫʱ����Ϊtrue
	**/
	BOOL 	m_bDelOldDataInSearch;

	/**
	* Ҫ��Ҫ��ʾû��ʱ���¼��Ĳο��¼�
	*/
	BOOL 	m_bDisplayEmptyRefEvt;

	/**
	* ʱ��ƫ��(��λΪСʱ)
	*/
	int 		m_nTimeZoneHour;

	/**
	* ����nvod�洢��Ŀ���ڴ���ʼ��ַ
	*/
	BYTE*	m_pMem;

	/**
	* nvod�ڴ��С
	*/
	DWORD	m_nMemLen;

}CSNvodParam_S;

/**
* ��ȡĬ��nvod���ò���
*/
void CSSHELLNvodGetDefaultParam(CSNvodParam_S* pOutParam);

/**
* ��ʼ��nvodģ�飬
* ���س�ʼ���ɹ����NVODInterface.h�ж����nvod���
* ��Ŀ�п���ʹ�øþ������NVODExtendInterface.h�Ľӿ�ע�Ტ������չ���ݡ�
*/
HCSHANDLE CSSHELLNvodInit(CSNvodParam_S* sParam);


#ifdef __cplusplus
}
#endif

#endif  /* SHELL_SUBTITLE_H */

