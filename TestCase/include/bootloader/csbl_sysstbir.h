/******************************************************************

@defgroup   bl�Զ���ȡң������ֵ��ʶ�����ң������
@Ӧ�ó���:


��bl����ʱ�򣬿���֪����Ҫ֧�ֶ�����ң������
���ǣ� ��bl������ɺ��������Ҫ����µ�ң������Ϊ�˲���Ҫ�޸�bl,
����ֱ������1���µ�ir_key.bin��¼��ң�����������򣬼��ɡ�

@version    1.0.0   bootloader     2011/9/14
***********************************************************************/

#ifndef _CSBL_SYSSTB_IR_H_
#define _CSBL_SYSSTB_IR_H_

#include "udiplus_debug.h"
#include "cs_ir_plus.h"

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************************
@brief ��flash�����ȡң������ֵ���û�ID�ȵȲ���
@dwAdrr: ir_key.bin�ļ���flash�ĵ�ַ
@g_CSIREquipFlashList[out]:  �����ң�����ṹ��
@pdwCount[out]: ir_key.bin������ڵ�ң��������

@return �ɹ�����TRUE, ʧ�ܷ���FALSE
************************************************************/
BOOL  CSBLGetSTBIRInfo(DWORD dwAdrr, CSIREquipList_t * g_CSIREquipFlashList, DWORD * pdwCount);


#ifdef	__cplusplus
}
#endif


#endif


