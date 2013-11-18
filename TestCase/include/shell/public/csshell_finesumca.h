
#ifndef CSSHELL_finesumCA_H
#define CSSHELL_finesumCA_H


#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,pls check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "thirdparty/ca/finesum/csfinesumcainterface.h"
/**
@brief finesumCA��Ϣ����ز����ṹ��
@note Ŀǰmmcp�ṩ��finesum ca������Ĭ��ʵ�֡�һ������£���Ŀʹ��Ĭ��ʵ�ּ�����������
@see CSShellFinesumCaSetParam CSShellFinesumCaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellFinesumCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA���ͣ����ṹ���й̶�ΪEM_CSSHELL_CA_TYPE_FINESUM
	
    CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�,nativeֱ�ӵ��ô˷�����ȡ���ܿ���
    INT (*m_fnGetMailNum)(INT* pnEmailNum, INT* pnEmptyNum);///< CSCAGetMailNum
    INT (*m_nfnGetMailHead)(INT nIndex, CaEmailHead_S *pstMailHead);///< CSCAGetMailHead
    INT (*m_nfnGetMailContentByIndex)(INT nIndex, CaEmailContent_S *pstMailContent);///<CSCAGetMailContentByIndex
    INT (*m_fnGetContentByEmailId)(WORD wEmailId, CaEmailContent_S *pstMailContent);///<CSCAGetMailContentByEmailId
    INT (*m_fnDelMail)(INT nIndex);///< CSCADelMail
    INT (*m_fnDelMailAll)(void);///< CSCADelMailAll
    INT (*m_fnGetCaInfo)(CaInfo_S *pstCaInfo);///< CSCAGetCaInfo
    CA_ReturnValue_E (*m_fnPairstatus)(void) ;///< CSCAGetPairstatus 
    void (*m_fnNotifyShowOSDOver)(void); ///<CSCANotifyShowOSDOver
    CA_ReturnValue_E (*m_fnGetCardID)(BYTE* paucCardID);///<CSCAGetCardID
}CSShellFinesumCaInitParam_S;

/**@brief FinesumCA���������Ϣ�ṹ��*/
typedef struct
{
	DWORD  m_dwMailAddress;///<�ʼ����׵�ַ
	DWORD  m_dwMailAddressBak;///<�ʼ����ݵ��׵�ַ
}CSShellFinesumCaConfig_S;

/**
@brief ����FinesumCA��ز����ӿڸ�MMCP
@param[in] FinesumCA��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ڵ���CSShellFinesumCaSetConfig֮ǰ���øýӿڡ�
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellFinesumCaSetParam(CSShellFinesumCaGetDefaulParam());
@code end
*/
BOOL CSShellFinesumCaSetParam(const CSShellFinesumCaInitParam_S* psFinesumCa);

	
/**
@brief ��ȡͬ��3CAĬ��ʵ�ֲ���
@return MMCP�ṩͬ��3CA Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���ͬ��3CA�Ķ�����ʹ��csshell_finesumca_private.h�е�CSShellFinesumCaGetParam�ӿ�
@see CSShellFinesumCaGetParam
*/
const CSShellFinesumCaInitParam_S* CSShellFinesumCaGetDefaulParam(void);

/**
@brief ����ͬ��3CA�����������Ϣ
@param[in] psConfig ͬ��3CA���������Ϣ�Ľṹ��ʵ��
@note �ýӿڱ�����CSShellFinesumCaSetParam֮�����
@see CSShellFinesumCaConfig_S
*/
void CSShellFinesumCaSetConfig(CSShellFinesumCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


