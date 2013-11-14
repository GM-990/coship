/****************************************************************************/
/*
 * Filename:        cs_toolset_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------

 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_toolset_plus.h"


/**
@brief ��ֲ�����ýӿڣ�����ʱ���ɸ�eCfgType����,�����CSSYSCfg_E����

�ýӿڿɱ����ö�Σ�ͬһ���͵����������һ�εĵ���Ϊ׼
@param[in] eCfgType ��������,�����CSSYSCfg_E����;
@param[in] nDeviceIndex ��Ҫ���õ��豸index,�����eCfgType�޶�Ӧ���豸ʱ,����-1;
@param[in] unValue1	���ò���1,���庬����eCfgType����;
@param[in] unValue2 ���ò���2,���庬����eCfgType����;
@return ���óɹ�����CSUDI_SUCCESS ������ʧ�ܷ��ش���ֵ��
*/
CSUDI_Error_Code CSUDISYSSetCfg(CSUDISYSCfg_E eCfgType, int nDeviceIndex, CSUDI_UINT32 unValue1, CSUDI_UINT32 unValue2)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief ��ֲ��ϵͳ��ʼ���ӿڣ�ʵ��ϵͳ�ķ������֣�����������ϵͳ��ϵͳ����ʱ��һ��������

@return ���óɹ�����CSUDI_SUCCESS ������ʧ�ܷ��ش���ֵ��
*/
CSUDI_Error_Code CSUDISYSInit(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief ��ֲ��ϵͳ��ʼ���ӿ�

�ýӿ�ֻ�ɱ�����һ�Σ�������CSUDISYSInit�����
@return ���óɹ�����CSUDI_SUCCESS ������ʧ�ܷ��ش���ֵ��
@note 
- ��ʼ����������оƬ����豸��������AV decoder��Demux�ȵȣ�����ʼ����Ӧ����ֲ��ģ�飬
	��Player,Inject,Audio,Video,Screen,Demux,Section�ȣ���ʼ��֮�����.�û�����Ҫ���ĸ��豸�ĳ�ʼ��˳��
	��ʼ�������ò���ʹ��CSUDISYSSetCfg���ã����û�����ý���Ĭ��ֵ��ʼ����
*/
CSUDI_Error_Code CSUDIPlatformInit(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief ������������Ѳ���

@param[in] pstStandbyParams ��CSUDISYSStandbyParams_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDISYSSetWakeupParams(const CSUDISYSWakeupParams_S* pstWakeupParams)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief оƬ�����Ĺ��Ӻ�������
@param[in] bStandbyStatus 	TRUE:�������״̬;FALSE:�˳�����״̬
@param[in] nStandbyTime  ����ʱ��
@param[out] pbSkipDefault �Ƿ����Ĭ�ϲ�����TRUE��ʾִֻ��Hook��������Ĳ�����FALSE��ʾͬʱִ��CSUDISYSChipStandbyԭ�в�����Ĭ��ΪTRUE
@return CSUDI_SUCCESS ��ʾ���Ӻ���ִ�гɹ�������Ϊ���صĴ������ֵ��
*/
//typedef CSUDI_Error_Code (*CSUDIChipStandbyHookFunc_F)(CSUDI_BOOL bStandby,int nStandbyTime, CSUDI_BOOL *pbSkipDefault);

/**
@brief ����оƬ�����Ĺ��Ӻ���

@param[in] fnHook ���Ӻ�������CSUDIChipStandbyHookFunc_F����
@return ������óɹ������ص���CSUDI_SUCCESS���������ʧ���򷵻ش������ֵ��
@note fnHook����ΪNULL����ʾȡ�����Ӻ���
*/
CSUDI_Error_Code CSUDISYSSetChipStandbyHook(CSUDIChipStandbyHookFunc_F fnHook)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
} 

/**
@brief ����������Ϣ

���������������ö����������Ϣ��������CSUDIToolsetExecʱʹ����ͬ��Ŀ�������ɲ�ͬ�Ĺ�����
��loader��app����ʹ����ͬLinux�ں˵����������������ݱ��ӿ����õ������������ز�ͬ��rootfs��
@param[in] eType ������Ϣ
@param[in] pExecParam ��������������������CSUDIToolsetExec���о���������֪��ʼ��ģ�����ã������߱��뱣֤��ָ���ڴ�һֱ��Ч��
@return ���óɹ�����CSUDI_SUCCESS ������ʧ�ܷ��ش������
@see CSUDIToolsetExec
*/
CSUDI_Error_Code CSUDIToolsetSetExecInfo(CSUDIToolsetExecType_E eType,void* pExecParam)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief ����Ŀ����������кź�Key����UDI�ӿ�CSUDIGetSerialKey��Ӧ

@param[in] psSerialKey ָ���Ż�ȡ�������кź�Key�Ľṹ��ָ�룬���кź�Key������0��β���ַ�������μ�CSUDISerialKey_S��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ������coship�ڲ���CSUDIGetSerialKey�ӿ���������ʵ�֣�������������֪����λ�ȡ���кź�Key��������Ҫ��һ��set�ӿڣ���Ӧ���ڳ�ʼ��ʱ����
*/
//CSUDI_Error_Code CSUDISetSerialKey(const CSUDISerialKey_S * psSerialKey);


