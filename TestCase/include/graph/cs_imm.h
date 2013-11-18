/**
�ļ����ƣ�CS_IMM.H
��Ȩ����(c)  2009�� ͬ�޵��ӡ�
 
@version  Ver 1.0    
@author   ��ʽ�� (longshirong@coship.com)
@file
		���뷨�ӿڣ���ʹ�÷�������:

		1������CSIMMCreate�ӿڴ������뷨�����������
		�ýӿڴ���Ĳ����������ļ���·�����ڿ����Ȳ������ã�
		�ù��ܻ�δ���ƣ�������ʱ��Ҫָ��Ĭ�����뷨���Ա���δע�����뷨ʱ��
		����ʹ��Ĭ�ϵ�ϵͳ���뷨��
		ͬʱ��Ҳ����ָ�����뷨�������ڴ���֮���Ƿ����������Ч��
		2������CSIMMStart�������뷨���ýӿ���Ҫ�������뷨�����������
		������CSIMMCreate��ķ���ֵ��
		3����ʱ�����뷨�Ѿ�����������ͨ��CSIMMSendEvent�����¼�CSIMM_EVENT_IMM_START�����뷨����
		4���ر����뷨�����Ե���CSIMMDestroy�ӿڣ�Ҳ�����ȷ���CSIMM_EVENT_IMM_CLOSE	�¼��ر����뷨
 
History:
�汾��		����		����		�޸�����
---------	     --------        -------   	-------------
1.0			2009-12-22	��ʽ��		����
1.1                   2010-11-17   ��ʽ��               �޸�ע��ӿڣ�����һ������������ָ�����뷨���������뷨����
*/

#ifndef __CS_IMM_H_
#define __CS_IMM_H_

#include "mwapi.h"
#include "mwengine.h"
#include "cs_event.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 ͷ�ļ�����                                 *
 ******************************************************************************/


/******************************************************************************
 *                                 �ꡢ���塢�ṹ��                           *
 ******************************************************************************/
 #define CSMIN(a,b)              ((a) < (b) ? (a) : (b))
#define CSMAX(a,b)              ((a) > (b) ? (a) : (b))

/**ϵͳ���뷨��ʶ��*/
#define CSIMM_IME_PINYIN 		0x00000001
#define CSIMM_IME_ENGLISH 	0x00000002
#define CSIMM_IME_SYMBOL		0x00000004
#define CSIMM_IME_DIGITAL		0x00000008
#define CSIMM_IME_ZHUYIN        0x00000010

//ϵͳ���뷨��ʶ��������
#define CSIMM_SYS_IME			0x0000000F

//��������¼�����
#define CSIMM_EVENT_IMM_BEGAIN  	1			///<���뷨ʹ���¼�
#define CSIMM_EVENT_IMM_CLOSE 		(CSIMM_EVENT_IMM_BEGAIN + 0)			///<�ر����뷨�¼�
#define CSIMM_EVENT_IMM_REINIT 		(CSIMM_EVENT_IMM_BEGAIN + 1)			///<���³�ʼ�����뷨���ڣ�����л�������뷨ʹ�õ���֮ǰ���뷨�Ĵ��壬����Ҫ�Ը���Ϣ������
#define CSIMM_EVENT_IMM_MAX		(CSIMM_EVENT_IMM_BEGAIN + 2)

///<���뷨�ڲ�������¼����ͣ�ֵ��������Ϊ�������뷨�����¼����͵�ֵ
#define CSIMM_IME_EVENT_OPEN		CSUDI_VK_INPUT				///<���뷨���¼����л�
#define CSIMM_IME_EVENT_CLOSE		CSUDI_VK_EXIT				///<���뷨�ر��¼�

//�ڴ�����¼�

//���뷨����
#define CS_T9INPUT_KEY_LEFT           			CSUDI_VK_LEFT
#define CS_T9INPUT_KEY_UP             			CSUDI_VK_UP
#define CS_T9INPUT_KEY_RIGHT          		CSUDI_VK_RIGHT
#define CS_T9INPUT_KEY_DOWN           		CSUDI_VK_DOWN
#define CS_T9INPUT_KEY_PRIOR          		CSUDI_VK_PAGE_UP
#define CS_T9INPUT_KEY_NEXT           			CSUDI_VK_PAGE_DOWN
#define CS_T9INPUT_KEY_T9INPUT			CSUDI_VK_INPUT
#define CS_T9INPUT_KEY_BACK             		CSUDI_VK_TOGGLE

#define CSIMM				"IMM"

#define CSIMM_ENABLE			1			///<���뷨ʹ��
#define CSIMM_UNABLE			0			///<���뷨ʹ����

#define CSIMM_MAX_MSG_NUM   3
#define CSIMM_THREAD_PRIORITY   2
#define CSIMM_DEFAULT_TIMEOUT 10

#define CSIMM_KEY_TYPE			GR_EVENT_TYPE_KEY_DOWN

#define	EVT_INPUT_IME    				(0x3005)

typedef struct  
{
    DWORD m_dwEvent;		 ///< ��Ϣ����
    DWORD m_dwType;		 ///< ��Ϣ����,��ҪΪ������keydown ����keyup����keypress
    DWORD m_dwPara1;		 ///<��Ϣ�ĸ��Ӳ���1
    DWORD m_dwPara2; 		///<��Ϣ�ĸ��Ӳ���2
}CSIMM_EVENT_S;

typedef INT (*IMMPostResultFunc)(const struct _MMCP_EvtEventEntry *event, void  *client_data,void  *object,void  *method );

enum
{
	E_IMM_RETURN_OK = 0,
	E_IMM_RETURN_FALSE = -1,
	E_IMM_RETURN_INITIALED = -2,	///<������ʵ���Ѿ���ʼ��
	E_IMM_RETURN_UNINITIAL = -3,	///<���뷨������δ��ʼ��
	E_IMM_RETURN_UNABLED = -4,		///<����ֹ��
	E_IMM_RETURN_MAX
}; 

typedef struct _IME_WININFO_
{
	//�����ߴ��ڴ���Ĳ���
	HCSHANDLE m_hUserClient;			///<�����ߴ���Client
	HCSHANDLE m_hUserWindow;			///<�����ߴ��ھ��
	HCSHANDLE m_hUserGC;				///<�����ߴ���GC

	//���뷨���ڴ���֮��Ĳ���
	HCSHANDLE m_hImeClient;			///<���뷨����client
	HCSHANDLE m_hImeWindow;			///<���뷨���ھ��
	HCSHANDLE m_hImeGC;				///<���뷨����GC

	//���ô��ڵ���ʼλ�á���ȵ���Ϣ
	GR_RECT m_sWinRect;					///<���뷨������ʾλ�ü���С
	GR_SIZE m_usBorderSize;				///<���뷨���ڱ߿��С
	GR_COLOR m_unBackgroundColor;		///<���뷨���ڱ�����ɫ
	GR_COLOR m_unBorderColor;			///<���뷨���ڱ߿���ɫ

	void * m_pvImeWinData;					///<���뷨���ڵĸ�������
} CSIME_WININFO_S;

typedef struct _IMM_INFO_
{
	DWORD m_dwCurImeID;				///<��ǰ���뷨��ʶ��
	DWORD m_dwDefImeID;				///<Ĭ�����뷨��ʶ��
	DWORD m_dwImeMask;				///<��ǰ�������뷨������
	int m_nCount;							///<���뷨����
	int m_nProcRet;							///<���뷨����������ֵ
	BOOL m_bUse;							///<���뷨�������Ƿ����
	BOOL m_bShowFlag;					///<���뷨�����Ƿ���ʾ
	BOOL m_bSetPostResult;				///< �Ƿ����������뷨����ֵ�������
	BOOL m_bPreserve;					
}CSIMM_INFO_S;

/**
@brief  ��������:
		���뷨�Ĵ���ص�����
 
@param[in]
		��
 
@param[out] 
		��
 
@return
 		��
@exception
@note
*/
typedef int (*IMEProcFunc)(CSIMM_EVENT_S* psEvent, CSIME_WININFO_S* psWinInfo);

/**
@brief  ��������:
		�ûص������ж��¼��Ƿ���Ҫ���͸����뷨
 
@param[in]
		CSIMM_EVENT_S* psEvent���͵���Ϣ�ṹ��
 
@param[out] 
		��
 
@return
 		����¼�Ϊ���뷨������¼����򷵻�TRUE�����򷵻�FALSE
@exception
@note
*/
typedef BOOL (*IMEIsValidEventFunc)(CSIMM_EVENT_S* psEvent);

/******************************************************************************
 *                                 ��������                                   *
 ******************************************************************************/

/**
@brief  ��������:
		�����뷨,������Ϣ����
 
 @param[in]
 		DWORD dwDefImeID����ʱ����Ĭ�����뷨����ʱ��������Ϊϵͳ�����뷨�����������û��Զ�������뷨����ʹ�������ļ�����ʱ���ò������Բ�������
 		
@param[out] 
		��
 
@return
 		�����ʼ���ɹ��򷵻�E_IMM_RETURN_OK�����򣬸���������ز�ͬ��ֵ
@exception
@note
*/
int CSIMMInit (DWORD dwDefImeID);


/**
@brief  ��������:
		�������뷨
 
@param[in]
		��
 
@param[out] 
		��
 
@return
 		�ɹ����ٺ󷵻�E_IMM_RETURN_OK�����򷵻�����ֵ
@exception
@note
		����ʧ�ܺ��������뷨�������֮���ٵ��ô˺���
*/
int CSIMMUnInit(void);

/**
@brief  ��������:
		�����¼�����Ϣ����
 
 @param[in]
 		CSIMM_EVENT_S* psEvent���͵���Ϣ�ṹ��
 		
@param[out] 
		��
 
@return
 		�ɹ����ͷ���E_IMM_RETURN_OK�����򷵻�����ֵ
@exception
@note
*/
int CSIMMSendEvent(CSIMM_EVENT_S *psEvent);

/**
@brief  ��������:
		��ȡ��ǰ���뷨��������һЩ��Ϣ�����統ǰ���뷨��Ĭ�����뷨���������Ƿ���Ч����ǰ�����Ƿ���ʾ�ȵ�
 
@param[in]
		��
 
@param[out] 
		CSIMM_INFO_S *psImmInfo���뷨��������Ϣ�ṹ
 
@return
 		�ɹ���ȡ����E_IMM_RETURN_OK�����򷵻�����ֵ
@exception
@note
*/
int CSIMMGetImmInfo(CSIMM_INFO_S * psImmInfo);

/**
@brief  ��������:
		������뷨���ڼ���״̬����������뷨���л���������������Ĭ�����뷨��
 
@param[in]
		void* pvAddData���뷨���ڴ���������������л������д����ֵ����ʹ��ǰһ��
		���뷨�Ĵ���ϵͳ���������Ϊ��һ�δ����뷨���ڣ�����Ҫ���ø�ֵ
 
@param[out] 
		��
 
@return
 		�ɹ��������л�����E_IMM_RETURN_OK�����򷵻�����ֵ
@exception
@note
*/
int CSIMMOpenOrSwitchIme(void* pvAddData);

/**
@brief  ��������:
		�رյ�ǰ����״̬���뷨��
 
@param[in]
		��
 
@param[out] 
		��
 
@return
 		�ɹ��رշ���E_IMM_RETURN_OK�����򷵻�����ֵ
@exception
@note
*/
 int CSIMMCloseIme(void);

/**
@brief  ��������:
		���÷������뷨�������Ļص�������ֻ���������˸ò���֮�����뷨�Ĵ������Żᱻ�����߽��յ���
 
@param[in]
		IMMPostResultFunc fnPostResultProc���뷨�Ĵ��������ػص�����
 
@param[out] 
		��
 
@return
 		�ɹ����÷���E_IMM_RETURN_OK�����򷵻�����ֵ
@exception
@note
*/
 int CSIMMSetResultProc(IMMPostResultFunc fnPostResultProc);

/**
@brief  ��������:
		�������뷨�ܹ������¼��Ļص�����
 
@param[in]
		IMEIsValidEventFunc fnIsValidEventProc ע��������ж����뷨�Ƿ��ܹ������¼��Ľӿ�
 
@param[out] 
		��
 
@return
 		�ɹ�ע�᷵��E_IMM_RETURN_OK�����򷵻�����ֵ
		E_IMM_RETURN_FALSE = -1,
		E_IMM_RETURN_INITIALED = -2,	///<������ʵ���Ѿ���ʼ��
		E_IMM_RETURN_UNINITIAL = -3,	///<���뷨������δ��ʼ��
		E_IMM_RETURN_UNABLED = -4,		///<����ֹ��
@exception
@note
*/
 int CSIMMSetIsValidEventProc(IMEIsValidEventFunc fnIsValidEventProc);

 /**
 @brief  ��������:
        ע�����뷨�ӿ�

 @param[in]
        IMEProcFunc fnImeProc�����뷨�ڵ�Ĵ�����
 @param[in]
        IMEIsValidEventFunc fnImeIsValid�����ж��¼��Ƿ�Ϊ���뷨�ܹ�������¼���
        �ûص�����ΪNULL����ΪNULLʱ�����е��¼������͸����뷨����
 @param[in]
       int nImeCount ���뷨�ĸ���

 @param[out] 
 ��

 @return
        �ɹ�ע�ᣬ����E_IMM_RETURN_OK�����򷵻�����ֵ
 @exception
 @note
        2010.11.17�Ըýӿڽ������䣬����һ��nImeCount������
        ���ڸ�֪����������ǰע������뷨�����У����ܰ���������뷨
 */
 DWORD CSIMMRegisterIme(IMEProcFunc fnImeProc, IMEIsValidEventFunc fnImeIsValid, int nImeCount);

/**
@brief  ��������:
		ж��ָ�������뷨
 
@param[in]
		DWORD dwImeIDж�ص����뷨��ʶ��
 
@param[out] 
		��
 
@return
 		�ɹ�ж�أ�����E_IMM_RETURN_OK�����򷵻�����ֵ
@exception
@note
		ֻ��ж�ص���CSIMMRegisterIme()����ע������뷨����������Ľڵ�Ҫ�е������Լ�ȥ�ͷ�
*/
int CSIMMUnRegisterIme(DWORD dwImeID);

/**
@brief  ��������:
		�õ����ַ����ַ�����
 
@param[in]
		const char* pchwStr ���ַ��ַ���
 
@param[out] 
		��
 
@return
 		�ɹ������ַ����������򷵻�0
@exception
@note
		�����ַ������������ַ����ֽڳ���
*/
int CSIMMwcslen(const char* pchwStr);

/**
@brief  ��������:
		�жϵ�ǰ���뷨�Ƿ��Ѵ�
 
@param[in]
		��
 
@param[out] 
		��
 
@return
 		������ڴ�״̬����E_IMM_RETURN_OK�����򷵻�E_IMM_RETURN_FALSE��
@exception
@note
*/
 int CSIMMIsImeActive(void);

#ifdef __cplusplus
}
#endif

#endif  /* __IMM_H_ */

/* �ļ����� */

