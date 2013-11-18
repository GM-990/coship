#ifndef CSSHELL_NVOD_H
#define CSSHELL_NVOD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h"
#include "NVODInterface.h"

#define CSSHELL_NVOD_MAX_DELIVER_COUNT 8

/**
* nvod��ʼ�����ò���
*/
typedef struct _CSShellNvodParam_S
{
	/**
	* ����nvod�洢��Ŀ���ڴ���ʼ��ַ
	*/
	BYTE*	m_pvMemAddr;

	/**
	* nvod�ڴ��С
	*/
	int		m_nMemSize;
	/**
	*NVOD����Ƶ�����
	*/
	int     m_nDeliverCount;

	/**
	* nvod�����ĸ���Ƶ��
	*/
	Deliver  m_sDeliver[CSSHELL_NVOD_MAX_DELIVER_COUNT];

	/**�洢tshiftEvent���ݵ�����*/
	int 	m_nEventDurationDays;

	/**�Ƿ�ǿ��ָ�����������ݹ�������,ע�⣺��Ϊtrueʱ�ǲ�������DM,falseΪ������DM*/
	BOOL 	m_bSearchDependDM;

	/**"��ʾNIT�����Ƿ����Servcie_List_Descriptor��������*/
	BOOL 	m_bSearchNitList;

	/**������ʱʱ�䣬��λΪms*/
	int 	m_nSearchTimeout;

	/**
		* "true"��ʾǰ�˰�TimeShiftEvent������TableID ����0x4e��0x4f�ı��з��͡� 
		* "false"��ʾǰ�˰�TimeShiftEvent������TableID ����0x50-0x5f��0x60-0x6f�ı��з��͡�
	**/
	BOOL 	m_bScheduleToSimple;

	/**��ʾSchedule EIT�����Ƿ����Servcie_List_Descriptor��������*/
	BOOL 	m_bSearchScheduleEIT;

	/**Ĭ��Ƶ���Ƿ�ΪNVODƵ��
	 *�˲����ڲ���tuneParams����ʱ���á�
	 *����true����ʾ����tuneParams�е�Ƶ�����NVODƵ�㣬��������������в�����NVODƵ�㡣
	 *����false����ʾ�û���֪������tuneParams�е�Ƶ���Ƿ����NVODƵ�㣬�������������������NVODƵ�㡣
	 **/
	BOOL 	m_bIsNvodFreq;

	/**
	 * �Ƿ�����EIT OTHER��,
	 * ����NVODƵ�㲻��EIT OTHER����������ֻ����ǰƵ��NVOD��Ϣ��,Ӧ�ý�����Ϊfalse
	**/
	BOOL 	m_bSearchEitOther;

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

	NVODExt 	m_fnParseData;
}CSShellNvodInitParam_S;

/**
@brief ����Nvod��ʼ������
@param [in] psNvod, ��ʼ�����ò���
@return �ɹ�����TRUE,���򷵻�FALSE
*/
BOOL CSShellNvodSetParam(const CSShellNvodInitParam_S* psNvod);

/**
@brief ��ȡ��ʼ������Nvod���
@param ��
@return ����nvod��������nvodδ��ʼ���ɹ����򷵻�NULL
*/
HCSHANDLE CSShellNvodGetHandle();

/**
@brief ���ڽ���NVOD����ʱ����NVOD���»��Ƶ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��CSShellNvodExit_F �������ʹ�ñ��뱻����
*/
typedef BOOL (*CSShellNvodEnter_F)(void);

/**
@brief �����˳�NVOD����ʱͬʱ�˳�NVOD״̬
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��CSShellNvodEnter_F �������ʹ�ñ��뱻����
*/
typedef BOOL (*CSShellNvodExit_F)(void);

/**
@brief ��ȡ����ĸ���
@return ����ĸ���
@note 
*/
typedef int (*CSShellNvodGetGroupCount_F)(void);

/**
@brief ��ȡָ�����������
@param [in] nGroupIndex, ���������
@param [out] pcGroupName, ָ�������ķ���������ַ���,���뷵��unicode��
@param [in] nLength, �������pcGroupName�ĳ���
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note 
*/
typedef BOOL (*CSShellNvodGetGroupName_F)(int nGroupIndex, char* pcGroupName, int nLength);

/**
@brief ��ȡָ��������������Ĳο��¼��ĸ���
@param [in] nGroupIndex, ���������
@return ָ��������������Ĳο��¼��ĸ���
@note 
*/
typedef int (*CSShellNvodGetRefEventCount_F)(int nGroupIndex);

/**
@brief ��ȡָ��������������Ĳο��¼�����Ϣ
@param [in] nGroupIndex, ���������
@param [in] nRefEventIndex, �ο��¼�������
@param [out] pstRefEvent,  �ο��¼�����Ϣ
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note 
*/
typedef BOOL (*CSShellNvodGetRefEventParam_F)(int nGroupIndex, int nRefEventIndex, ReferenceEvent *pstRefEvent);

/**
@brief ��ȡָ���������ڲο��¼���ʱ���¼��ĸ���
@param [in] nGroupIndex, ���������
@param [in] nRefEventIndex, �ο��¼�������
@return ָ���������ڲο��¼���ʱ���¼��ĸ���
@note 
*/
typedef int (*CSShellNvodGetTimeShiftEventCount_F)(int nGroupIndex, int nRefEventIndex);

/**
@brief ��ȡָ����ʱ���¼�����Ϣ
@param [in] nGroupIndex, ���������
@param [in] nRefEventIndex, �ο��¼�������
@param [in] nTimeshiftEventIndex, ʱ���¼�������
@param [out] pstTimeShiftEvent,  ʱ���¼�����Ϣ
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note 
*/
typedef BOOL (*CSShellNvodGetTimeShiftEventParam_F)(int nGroupIndex, int nRefEventIndex, int nTimeshiftEventIndex, TimeShiftedEvent *pstTimeShiftEvent);


/**
���ṹ�����˻�ȡDM�е���Ϣʱ�õ�����Ϣ��
�����ɻ�ȡCSShellDMFunc_S
*/
typedef struct _CSShellNvodFunc_S
{
    int                                 m_nSize;            ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellNvodFunc_S)
    CSShellNvodEnter_F           m_fnEnter; ///< ����NVOD����ʱ����NVOD����
    CSShellNvodExit_F           m_fnExit; ///< �˳�NVOD����ʱͬʱ�˳�NVOD״̬
    CSShellNvodGetGroupCount_F           m_fnGetGroupCount; ///< ��ȡ����ĸ���
    CSShellNvodGetGroupName_F           m_fnGetGroupName; ///< ��ȡָ�����������
    CSShellNvodGetRefEventCount_F           m_fnGetRefEventCount; ///< ��ȡָ��������������Ĳο��¼��ĸ���
    CSShellNvodGetRefEventParam_F           m_fnGetRefEventParam; ///< ��ȡָ��������������Ĳο��¼�����Ϣ
    CSShellNvodGetTimeShiftEventCount_F           m_fnGetTimeShiftEventCount; ///< ��ȡָ���������ڲο��¼���ʱ���¼��ĸ���
    CSShellNvodGetTimeShiftEventParam_F           m_fnGetTimeShiftEventParam; ///< ��ȡָ����ʱ���¼�����Ϣ
}CSShellNvodFunc_S;

/**
@brief ����NVOD��ز����ӿڸ�MMCP
@param[in] psNvodFunc  NVOD��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellNvodSetFunc(CSShellNvodGetDefaulFunc());
@endcode
*/
BOOL CSShellNvodSetFunc(const CSShellNvodFunc_S* psNvodFunc);

/**
@brief ��ȡNVODĬ��ʵ�ֲ���
@return MMCP�ṩNVOD Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native��jsext)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���NVOD�Ķ�����ʹ��csshell_nvod_private.h�е�CSShellNvodGetFunc�ӿ�
@see CSShellNvodGetFunc
*/
const CSShellNvodFunc_S* CSShellNvodGetDefaulFunc(void);

#ifdef __cplusplus
}
#endif

#endif  /* SHELL_SUBTITLE_H */

