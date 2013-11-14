/****************************************************************************/
/*
 * Filename:        cs_ir_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_ir_plus.h"

/**
@brief ң������ʼ�����û���Ҫָ��ң�����������������ȼ���

@param[in] nPriority  ָ��ң�����������������ȼ���
@return 
- CSIR_SUCCESS��ң������ʼ���ɹ�
- CSIR_ERR_ALREADY_INIT��ң����֮ǰ��ʼ�����ˡ�
- CSIR_ERR_BAD_PARAMETER�������������
@note ���ȼ�0-255
*/
CSIR_Error_t CSIRInitialize( int nPriority )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ң������ʼ�����û���Ҫָ��ң�����������������ȼ���ң�����̵߳�ջ��С��

@param[in] nPriority ָ��ң�����������������ȼ���
@param[in] nStackSize ָ��ң���������������ջ��С��
@return
- CSIR_SUCCESS��ң������ʼ���ɹ�
- CSIR_ERR_ALREADY_INIT��ң����֮ǰ��ʼ�����ˡ�
- CSIR_ERR_BAD_PARAMETER�������������
@note ���ȼ�0-255
*/
CSIR_Error_t CSIRInitialize_Ext( int nPriority, int nStackSize )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ����֧�ֵ�ң�����ͺţ���Ҫָ��ң������Э����û��롣
@param[in] ptIREquip ָ��ң�������ݽṹ��ָ�룬�ṹ��Ա����ң����Э�顢�û��롢��������ɨ�����빦�����ӳ���
@param[out] phIRHandle ����ң�����ľ����
@return
- CSIR_SUCCESS�����ң�����ɹ���
- CSIR_ERR_EQUIPMENT_EXIST���豸�Ѿ����ڡ�
- CSIR_ERR_EQUIPMENT_FULL���豸�б�������
- CSIR_ERR_BAD_PARAMETER�������������
@note �û����������MAX_IR_EQUIPMENT��ң������ 
@note ���ң����Э����û�ж�������(��UserId��KeyCode)���ֽ�����Ĭ�ϰ�little-endian��ʽ����������ڴ��е�����Ϊ0x12|0x34��UserId=0x3412
@note ���ִ���ʽ����Ϊ�����Ǵ�0λ��ʼ���α����յģ���16λ�����ݽ���˳��Ϊ:D0 D1 D2...D15
@note Ŀǰ��֪�Ĵ����ֽ����Э����:
- NECЭ����ʵ��Ӧ���о�����UserId��չ��16λ(����ȡ������)���򰴴˹����λ��ǰ��ԭUserId�ֽ���Ϊ��λ��UserId�����ֽ���Ϊ��λ������NECЭ���������һ�µ�
- TopwayIЭ���е�UserId��4�ֽڣ����˹����λ��ǰ��Ҳ���䶨����C0-C31�ķ���˳��һ��
- TopwayIЭ����KeyCode��UserId����ͬ�Ĺ���
@note ��ֵΪң�����������ԣ����������м��������κ����ݣ���UserId�ȡ�
*/
CSIR_Error_t CSIRAddEquipment( const CSIREquipList_t	* ptIREquip, HCSHANDLE* phIRHandle )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


/**
@brief ɾ��֧�ֵ�ң�����豸����Ҫָ������豸ʱ���صľ����
@param[in] hIRHandle ָ������豸ʱ���ص�ң�����ľ����
@return
- CSIR_SUCCESS��ɾ��ң�����豸�ɹ���
- CSIR_ERR_BAD_PARAMETER�������������
- CSIR_FAILURE: ����ʧ��
*/
CSIR_Error_t CSIRDelEquipment(HCSHANDLE hIRHandle )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


/**
@brief �����ڰ�����������ʱ���Ƿ���������ظ�

@param[in] nLogicValue Ҫ���õİ����Ĺ�����
@param[in] bIsRepeat TRUE��ʾ���������ظ���FALSE��ʾ��ֹ���������ظ�
*/
void CSIRSetSingleKeyRepeat( int nLogicValue, BOOL bIsRepeat )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
}

/**
@brief ע��ң���������ص�������

�ú����������ã��û����Բ���Ҫ�����豸�Ϳ��Ի�ȡң�����Ļص���Ϣ�� 
@param[in] DebugCallback ָ��ص������ĺ���ָ�룬�û�ͨ���ص������õ�ң��������Ͱ�����Դ��״̬�͹�����
@return CSIR_SUCCESS��ע��ɹ���CSIR_ERR_BAD_PARAMETER�������������
*/
CSIR_Error_t CSIRRegisterDebugCallback( CSHDICallback DebugCallback )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@briefע��ң���������ص�����������ң������������ɨ����ͨ��ע��Ļص�����֪ͨӦ�ò㡣

@param[in] KeySendCallback ָ��ص������ĺ���ָ�룬�û�ͨ���ص������õ�ң��������Ͱ�����Դ��״̬�͹����룬���CSHDICallback���塣
@return CSIR_SUCCESS��ע��ɹ�;CSIR_ERR_BAD_PARAMETER�������������
@note
*/
CSIR_Error_t CSIRRegisterKeySendCallback( CSHDICallback KeySendCallback )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ����ң�����ķ�Ӧ�ٶȡ�

@param[in] enmIRProtocolType Ҫ���õ�ң����Э�顣 
@param[in] enmIRRepeatType ����repeat������:��press��repeat ����repeat��repeat��
@param[in] nRepeatTimes ���ü�⵽repeat��Ϣ�Ĵ�����Ĭ��ֵ��
- NecЭ�飺press��repeat 2;repeat��repeat 0��
- Philip RC5Э��: press��repeat 4;repeat��repeat 1��
- Philip RC6Э��: press��repeat 2;repeat��repeat 1��
- Topway Э�飺press��repeat 2;repeat��repeat 0��
@return CSIR_SUCCESS:���óɹ���CSIR_ERR_BAD_PARAMETER:��������
*/
CSIR_Error_t CSIRSetRepeatSpeed(CSIRProtocol_t enmIRProtocolType, CSIRRepeatType_t enmIRRepeatType,int nRepeatTimes)
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief �������������ң����Э��

@param[in] enmProtocolIndex  ָ����ң����Э������
@return CSIR_SUCCESS�����óɹ� ��CSIR_FAILURE������ʧ��
@note �ýӿڽ�Ϊ���ֲ����Զ�ʶ��Э���ƽ̨���裬�����ڳ�ʼ��֮ǰ���ã���ʼ��������ֻ��֧�����һ�����õ�Э�飻��ʼ��֮��ýӿ���Ч
@note ���ڿ����Զ�ʶ��Э���ƽ̨���ýӿ���Ч
*/
CSIR_Error_t  CSIRSelectProtocol(CSIRProtocol_t  enmProtocolIndex)
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief get nkeyNum keys during dwTimeOut ms

@param[out] pKeyValue Logic value of key to detect
@param[in] dwTimeOut time out (ms)
@return CSIR_SUCCESS�����óɹ� CSIR_FAILURE������ʧ��
*/
CSIR_Error_t CSIRDetectKey(DWORD *pKeyValue, DWORD dwTimeOut)
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief�رպ���ң�ؽ��չ��ܣ�

���øú����󣬽�������Ӧң��������
@return CSIR_SUCCESS�����óɹ���
*/
CSIR_Error_t CSIRDisable ( void )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief �򿪺���ң�ؽ��չ���

ֻ��CSIRDisable֮����ã����ڻָ�����ң�ؽ��չ��ܡ�
@return CSIR_SUCCESS�����óɹ���
*/
CSIR_Error_t CSIREnable( void )
{
    CSIR_Error_t Retcode = CSIR_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ��ȡң����Դ��İ汾�š�

@param[in] pchVer ���ڴ�Ű汾��Ϣ
@param[in] nSize �汾��Ϣ�ĳ���
@return ���ذ汾����
@note �汾��ʽΪ��"[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
*/
int CSIRGetVersion( PCHAR pchVer, int nSize )
{
    int Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


