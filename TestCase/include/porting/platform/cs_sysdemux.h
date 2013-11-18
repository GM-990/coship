#ifndef _CS_SYSDEMUX_H_
#define _CS_SYSDEMUX_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "udi_demux.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
@brief Section��PES���Ļ���Buffer��С�Ĺ��Ӻ���

@param[in] dwDemuxID Demux ID
@param[in] nRequestDataType ��ʾ���������������:PES��SECTION��TS��,��USPSectionDemuxCallbackType
@param[in] pRequestInfo �û������������ݵ�Filter���� 
@return ��0��ʾ��Ĭ��ֵ�������������ֵ���û���Buffer��С
@note ���û���ʵ������ʵ��
*/
typedef int (*USPSectionDemuxBufSizeHook_F)(DWORD dwDemuxID, USPSectionDemuxCallbackType eRequestDataType, const RequestInfo *pRequestInfo);

/**
@brief ��ʼ��Demux�豸

@param[in] dwDemuxID Demux��ʶ;��ϵͳ��ֻ��һ��Demuxʱ,�ò�������ʡ�ԣ��ò���Ҳ������һ��Demux�豸�����豸��
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL InitUSPSectionDemux( DWORD dwDemuxID );

/**
@brief ����ָ����Demux��Tuner�豸

Ŀǰ�ú�������
@param[in] dwDemuxID ָ����Demux�豸���
@param[in] dwTsInID �豸ָ����Tuner�豸���
@param[in] hReserved ��������
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/ 
BOOL USPTsSourceConnect( DWORD dwDemuxID, DWORD dwTsInID, HCSHANDLE hReserved );

/**
@brief ����Section��PES���Ļ���Buffer��С�Ĺ��Ӻ���

@param[in] fnBufSizeHook
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note ��Ŀ�п���ͨ�����Ӻ�������ֵ���趨ƽ̨�ײ����ڴ��Section��PES���Ļ���Buffer��С��û�����������Ĭ��ֵ��ʵ��
*/
BOOL USPSectionDemuxSetBufSizeHook(USPSectionDemuxBufSizeHook_F fnBufSizeHook);


#ifdef  __cplusplus
}
#endif

#endif

