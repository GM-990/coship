/**@defgroup CSSYSVBI CSSYSVBI
@brief VBI��ʽ��teletext��close caption�Լ�subtitle��ؽӿڣ�Ŀǰֻ֧��teletext

@{
*/
#ifndef _CS_SYSVBI_H_
#define _CS_SYSVBI_H_

#include "cs_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ����teletext���ݵ�PID

������VBI teletext֮ǰ��������Դ��PID
@param[in] hHandle �豸ͨ�������ϵͳ��ʼ���õ�
@param[in] wPID teletext���ݵ�PID
@return ���óɹ�����TRUE�� ����ʧ�ܷ���FALSE
*/
BOOL CSSYSVBISetTeletextPid(HCSHANDLE hHandle, WORD wPID);

/**@brief ����VBI teletext

@param[in] hHandle �豸ͨ�������ϵͳ��ʼ���õ�
@return �����ɹ�����TRUE�� ����ʧ�ܷ���FALSE
*/
BOOL CSSYSVBIStartTeletext(HCSHANDLE hHandle);

/**@brief ֹͣVBI teletext

@param[in] hHandle �豸ͨ�������ϵͳ��ʼ���õ�
@return �����ɹ�����TRUE�� ����ʧ�ܷ���FALSE
*/
BOOL CSSYSVBIStopTeletext(HCSHANDLE hHandle);

/**@brief �򿪺͹ر�VBI CLOSE CAPTION

@param[in] hHandle �豸ͨ�������ϵͳ��ʼ���õ�
@param[in] bEnable ΪTRUE�򿪣�FALSE�ر�
@return �����ɹ�����TRUE�� ����ʧ�ܷ���FALSE
*/
BOOL CSSYSVBIEnableCloseCaption(HCSHANDLE hHandle,BOOL bEnable);

/**@brief �򿪺͹ر�VBI Macrovision

@param[in] hHandle �豸ͨ�������ϵͳ��ʼ���õ�
@param[in] bEnable ΪTRUE�򿪣�FALSE�ر�
@return �����ɹ�����TRUE�� ����ʧ�ܷ���FALSE
*/
BOOL CSSYSVBISetMacrovision(HCSHANDLE hHandle, BOOL bEnable);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* !_CS_SYSVBI_H_ */


