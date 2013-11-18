#ifndef CSSHELL_BOOT_H
#define CSSHELL_BOOT_H

#include "mmcp_typedef.h"
#include "SearchUI.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
���ṹ������Boot�������ṹ����Ϣ
*/
typedef struct _CSShellCommonBoot_S
{
    int                    m_nSize;                ///<���ṹ��Ĵ�С�����븳ֵΪsizeof(CommonBoot_S)
    void*                  m_pvUserData;           ///<�û����ݣ��˲��������ű��ṹ�еĺ���ָ��һ�����ݸ�����ʵ��

    /*
        @brief ��ȡBoot��������̵�״̬
        @return TURE��ʾBoot�������������������״̬��FALSE��ʾBoot��������������ڷ�����״̬
        */
    int (* m_fnBootGetBootState)();
}CSShellCommonBoot_S;

/**
@brief ����CSShellCommonBoot_S����
@param psParam Boot Shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE��ʧ�ܷ���FALSE
*/
BOOL CSShellCommonBootSetParam(CSShellCommonBoot_S *psParam);

/**
@brief ��ȡCSShellCommonBoot_S����
@param ��
@�ɹ�����CSShellCommonBoot_S��ʧ�ܷ���NULL
*/
const CSShellCommonBoot_S* CSShellCommonBootGetParam();

#ifdef __cplusplus
}
#endif
/** @} */
#endif

