/**@defgroup PublicShellCA PublicShellCA ����ShellCA��������ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_CA_H
#define CSSHELL_CA_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@brief CA����.Ŀǰ�Ѷ���mmcp�����õ���ca������һЩ��ʱca������ͬ��2δ�����塣dtvm��shellҲδ�Թ�ʱ��ca��Ĭ��֧��*/
typedef enum
{
    EM_CSSHELL_CA_TYPE_FTA = 0,
    EM_CSSHELL_CA_TYPE_NDS,
    EM_CSSHELL_CA_TYPE_TF3,
    EM_CSSHELL_CA_TYPE_DVT,
    EM_CSSHELL_CA_TYPE_SUANTONG,
    EM_CSSHELL_CA_TYPE_IRDETO3,
    EM_CSSHELL_CA_TYPE_NAGRA,
    EM_CSSHELL_CA_TYPE_TOP,
    EM_CSSHELL_CA_TYPE_IRDETO2,
    EM_CSSHELL_CA_TYPE_FINESUM,
    EM_CSSEHLL_CA_TYPE_UDRM,
    EM_CSSHELL_CA_TYPE_IRDETOIUC,
    EM_CSSHELL_CA_TYPE_CONAX,
    EM_CSSHELL_CA_TYPE_DVN,
    EM_CSSHELL_CA_TYPE_COMVENIENT,
    EM_CSSHELL_CA_TYPE_SUANTONGCTI,
    EM_CSSHELL_CA_TYPE_TR
}CSShellCaType_E;


/**
@brief ��ȡ���ܿ��Žӿڶ���
���ӿ�ͬ����ȡ���ܿ��ţ����޿������޷�ʶ���򷵻�ʧ�ܡ�
@param[out] pcCardId �洢��ȡ�������ܿ�����
@param[in/out] pnLen �������ʱ��ʾpcCardIdָ���buffer���ȣ��������������� pcCardId�п��ŵ�ʵ�ʳ���
@return �ɹ�����0��ʧ��-1
@note ͬ���ӿڣ���ca�ײ�(��������ca)�޷��ṩͬ���ӿ���ʵ�����л��棬�Ա�֤�ýӿڵ�ͬ��
*/
typedef int (*CSShellCaGetSmartCardID_F)(char* pcCardId, int *pnLen);

/**
@brief �����Ƿ�����dtvm ca native�رջ������Ƶ
@param[in] bCanControl TRUE,�����������Ƶ;��������
@return ��
@note �ýӿ���Ϊ�˼���������Ŀ֮ǰ��dtvm ca native�п�������Ƶ�����Զ���ӣ�Ĭ��Ϊ FALSE
@note ������������Ŀʹ�ã���ΪTRUE;������Ŀ���鲻Ҫ���øýӿ�
@note Ŀǰ�����ý���nds ca��Ч
*/
void CSShellCaSetControlAV(BOOL bCanControl);


#ifdef __cplusplus
}
#endif
/** @} */
#endif   



