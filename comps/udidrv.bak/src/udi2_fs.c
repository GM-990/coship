/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_fs.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_fs.h"
/**
@brief ����豸֪ͨ�ص�����

���RDIģ��CSUDIRDIAddCallback_F�ӿڶ��壬�����ڴ���STORAGE������VOLUME�����͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,���豸״̬�����仯ʱ���øú���
@param[in] pvUserData �û�����,����Ϊ�ص�������pUserData��������
@return �ɹ���ӷ���CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��ӻص�ǰ�������豸�仯��Ϣ,���Ჹ��֪ͨ.��������ʱ��Щ�豸�ʹ���,���ñ�����ʱ����õ�֪ͨ.
@note fnNotify��pUserDataһ��Ψһȷ��һ���ص�������������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIFS_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ɾ���豸֪ͨ�ص�����

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ���STORAGE������VOLUME�����͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,֮ǰע��ĺ���
@param[in] pvUserData �û�����,������ע��ʱ��һ��
@return �ɹ�ɾ������CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note fnNotify��pUserDataһ��Ψһȷ��һ���ص�����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȫ�Ƴ����ƶ��洢�豸

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ���STORAGE������VOLUME�����͵Ŀ��ƶ��豸
@param[in] dwDeviceId Ŀ���豸ID
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ֻ��ɾ�����ƶ��豸����ͼɾ�������豸�����򷵻�CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED��ͨ���豸���豸Id���ж��豸����
@note ɾ���豸�󲻿���ͨ�����豸Id��ȡ�豸��Ϣ
@note �û����øýӿڻᴥ��EM_UDIRDI_PLUGOUT_SAFE���͵�EM_UDIRDI_EVENT_PLUGOUT��Ϣ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ�����豸��Id

�������ƶ��ĺ͹̶��Ĵ洢�豸�ͷ־��豸
@param[out] pdwIds ���ڴ洢���д��ڵ��豸��Id
@param[in] nMaxIdCnt pdwIds����Ĵ�С,������ܷ��ض����豸ID,һ����˵��һ���־��U�̻��ʾΪ2���豸,��ֵ����32��������������
@param[out] pnActIdCnt ʵ�ʵ��豸ID������ֵ��ʾ�ж����豸���뿽����pdwIds�еĸ�����ͬ
@return �ɹ�����CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ
@note �ýӿڽ������м�����ע���豸,��U��,�ƶ�Ӳ��,�����м���з��ʵ�cramfs�豸��,������һ����˵����ʵ�ʵ��豸��.
��UDI������Ҫ������Ŀ����һЩ���ù�����
@note Ϊ���ּ��ݣ����зǿ��ƶ��豸���豸���Դ�0��ʼ����
@note ����pdwIdsΪCSUDI_CSUDI_NULL����������±�ʾӦ�ý���ע�ж��ٸ��洢�豸�������������Ǿ����Id

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �����豸ID��ȡ�豸��Ϣ

@param[in] dwDeviceId Ŀ���豸ID,��ֵ��Դ��CSUDIFSGetAllDeviceId��ص�����
@param[out] psDeviceInfo ָ�򷵻��豸��Ϣ�ڴ��ָ��
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ʹ�豸�������״̬

@param[in] dwDeviceId Ŀ���豸ID
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��֧�ִ洢�豸����,�־��豸����ʧ��
@note �Ѿ��ڴ���״̬�µ��ñ��ӿ��Է���CSUDI_SUCCESS

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}
/**
@brief �����豸

@param[in] dwDeviceId Ŀ���豸ID
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��֧�ִ洢�豸����,�־��豸����ʧ��
@note �Ѿ��ڻ���״̬�µ��ñ��ӿ��Է���CSUDI_SUCCESS

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ʽ���豸

@param[in] dwDeviceId Ŀ���豸ID
@param[in] eType ���ʽ���ɵ��ļ�ϵͳ��ʽ
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ����־��豸����ʽ�������־�
@note ����洢�豸�������豸��ʽ��Ϊһ���־�,������ԭ�־�(�������)PLUGOUT,�·־����Ļص�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ·�����뷽ʽ

@param[out] pnCodePage �����ֽ�˳�� | ���뷽ʽ,������Ϻ궨��
ͨ���ú���ȷ����UDIFS������·���ı��뷽ʽ
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetCodePage(int * pnCodePage)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ���ļ�

@param[in] pcFileName �ļ���
@param[in] pcMode �򿪷�ʽ,���C99��׼
@return �ļ����,ʧ�ܷ���CSUDI_NULL
@note ��������������C99��׼fopen

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ���ļ�

@param[in] hFile �ļ����,CSUDIFS_Open�ķ���ֵ
@param[in] pcBuf ָ���Ŷ������ݵ��ڴ�
@param[in] uCount �������ֽ���
@return ʵ�ʶ������ֽ���,����������uCount
@note uCountΪ0ʱ����0,�ļ�״̬�������κα仯
@note ����ֵ<0��ʾ��������ļ�����β��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief д�ļ�

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] pcBuf ָ����д�����ݵ��ڴ�
@param[in] uCount �������ֽ���
@return ʵ��д����ֽ���,����������uCount
@note uCountΪ0ʱ����0,�ļ�״̬�������κα仯

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �ر��ļ�

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSClose(CSUDI_HANDLE hFile)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief seek�ļ�

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] lOffset �ƶ�ƫ��
@param[in] uOrigin �ƶ���ʼλ��,CSUDIFS_SEEK_SET,CSUDIFS_SEEK_CUR,CSUDIFS_SEEK_END
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief seek�ļ�64λ��

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] i64Offset �ƶ�ƫ��
@param[in] uOrigin �ƶ���ʼλ��,CSUDIFS_SEEK_SET,CSUDIFS_SEEK_CUR,CSUDIFS_SEEK_END
@return �ɹ�����0,�����������-1
@note ��ʹ�ļ�ϵͳ��֧�ֳ���2G���ļ�Ҳ����ʵ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ�ļ���ǰλ��

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@return �ɹ������ļ���ǰ�࿪ʼ����ƫ��,�����������-1
@note ��ǰƫ�ƴ���long�ܱ�ʾ�����Χʱ(32λ����Ϊ2G),����-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
long CSUDIFSTell(CSUDI_HANDLE hFile)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ�ļ���ǰλ��64λ��

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@return �ɹ������ļ���ǰ�࿪ʼ����ƫ��,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
{
	CSUDI_INT64 value = 0;
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return value;
}

/**
@brief flush�ļ�

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFlush(CSUDI_HANDLE hFile)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ɾ���ļ�

@param[in] pcFileName �ļ���
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRemove(const char * pcFileName)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �������ļ�

@param[in] pcOldPath ���ļ���
@param[in] pcNewPath ���ļ���
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ�����Ч
@note �ýӿڿ��Ըı��ļ���Ŀ¼,�����ᴴ����Ŀ¼,����pNewPathָ����Ŀ¼�������򷵻�ʧ��
@note ���pNewPathָ����ļ��Ѵ���,����ǲ�ȷ����,�����ø�����.

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �ض��ļ�

��һ���ļ��ض�Ϊһ������
@param[in] pcPathName �ļ���
@param[in] ulLength ���ļ�����
@return �ɹ�����0,�����������-1
@note ����³���С��ԭ�ļ�����,�����Ĳ��ֶ���;����³��ȴ���ԭ�ļ�����,������������0���
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �ض��ļ���������

��һ���ļ��ض�Ϊһ������
@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] ulLength ���ļ�����
@return �ɹ�����0,�����������-1
@note ����³���С��ԭ�ļ�����,�����Ĳ��ֶ���;����³��ȴ���ԭ�ļ�����,������������0���
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �ض��ļ�64λ��

��һ���ļ��ض�Ϊһ������
@param[in] pcPathName �ļ���
@param[in] u64Length ���ļ�����
@return �ɹ�����0,�����������-1
@note ����³���С��ԭ�ļ�����,�����Ĳ��ֶ���;����³��ȴ���ԭ�ļ�����,������������0���
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �ض��ļ�������64λ��

��һ���ļ��ض�Ϊһ������
@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] u64Length ���ļ�����
@return �ɹ�����0,�����������-1
@note ����³���С��ԭ�ļ�����,�����Ĳ��ֶ���;����³��ȴ���ԭ�ļ�����,������������0���
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ�ļ�����

@param[in] pcFileName �ļ���
@param[out] psFileStat ����ļ�����
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ�����Ч
@note psFileStat->m_lMode ֵ������ȷ��ֻ������CSUDIFS_ISREG��CSUDIFS_ISDIR����Ӧ��Ϊ����ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ�ļ������ļ���������

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[out] psFileStat ����ļ�����
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ�����Ч
@note psFileStat->m_lMode ֵ������ȷ,��ΪCSUDIFS_ISREG��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �����ļ���

@param[in] pcPathName �ļ�����
@param[in] nReserved �����ֶ�,���봫0,�����뷵��ʧ��
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSMkdir(const char * pcPathName, int nReserved)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ɾ���ļ���

@param[in] pcPathName �ļ�����
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRmdir(const char * pcPathName)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ���ļ���

@param[in] pcPathName �ļ�����
@return �ɹ������ļ��о��,ʧ�ܷ���CSUDI_NULL

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �ر��ļ���

@param[in] hDir �ļ��о��,CSUDIFSOpendir�ķ���ֵ
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ�ļ���

һ�ζ�ȡ���ļ����µ�һ���ڵ����Ϣ,���ε��ÿɻ�ȡ���ļ����������ļ���Ϣ
@param[in] hDir �ļ��о��,CSUDIFSOpendir�ķ���ֵ
@return ����һ���ļ������ݽڵ�ָ��,û�и���ڵ�ʱ����CSUDI_NULL
@note
- �ýӿڽ��г������ļ����ļ��У����ݹ�
- ���ļ��а���"."��".."����Ŀ¼

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ�־���Ϣ

��ȡpcPathName���ڷ־�ķ־���Ϣ,����̴�С,���пռ��
@param[in] pcPathName �־�·��,��������Ϣ�ķ־��·��
@param[out] psFsStat �洢�־���Ϣ�ڴ�ָ��
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ����ļ��о���Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ͨ���ļ������ȡ�־���Ϣ

��ȡhFile��Ӧ���ļ����ڷ־�ķ־���Ϣ,����̴�С,���пռ��
@param[in] hFile �ļ����,��ȥ��ȡ���ļ����ڵķ־����Ϣ
@param[out] psFsStat �洢�־���Ϣ�ڴ�ָ��
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

