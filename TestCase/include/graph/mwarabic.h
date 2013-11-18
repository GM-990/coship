
#ifndef __MWARABIC_H
#define __MWARABIC_H

#include "mwcommon.h"

#ifdef  __cplusplus
extern "C" {
#endif


/**
@brief �ж��ַ��Ƿ��ǰ�������

@param[Uint16] pusInText : Ҫ�жϵ�Unicode�ַ���

@return �Ƿ���TRUE ���򷵻�FALSE
@note   ��
*/
BOOL GrFont_IsArabicText(Uint16 * pusInText);

/**
@brief ������İ�������ת������ʾʱ�ı����ַ�

@param[Uint16] pusInText   : Ҫת���İ��������ַ���
@param[Uint16] pusOutText  : ת����İ������ı����ַ����洢�׵�ַ
@param[Uint16] pnOutTextLen: ��ʼ��ֵΪpusOutText�ܴ洢���ַ�����,���ʵ��ת����ı����ַ�����

@return �ɹ�����0�����򷵻�-1
@note   �����������ַ�������Unicode�ַ���
*/
int GrFont_ChangeArabicText(Uint16 * pusInText, Uint16 * pusOutText, int *pnOutTextLen);


#ifdef  __cplusplus
}
#endif

#endif
