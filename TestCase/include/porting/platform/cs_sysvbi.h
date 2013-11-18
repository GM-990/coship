/**@defgroup CSSYSVBI CSSYSVBI
@brief VBI形式的teletext、close caption以及subtitle相关接口，目前只支持teletext

@{
*/
#ifndef _CS_SYSVBI_H_
#define _CS_SYSVBI_H_

#include "cs_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 设置teletext数据的PID

在启动VBI teletext之前设置数据源的PID
@param[in] hHandle 设备通道句柄，系统初始化得到
@param[in] wPID teletext数据的PID
@return 设置成功返回TRUE， 设置失败返回FALSE
*/
BOOL CSSYSVBISetTeletextPid(HCSHANDLE hHandle, WORD wPID);

/**@brief 启动VBI teletext

@param[in] hHandle 设备通道句柄，系统初始化得到
@return 启动成功返回TRUE， 启动失败返回FALSE
*/
BOOL CSSYSVBIStartTeletext(HCSHANDLE hHandle);

/**@brief 停止VBI teletext

@param[in] hHandle 设备通道句柄，系统初始化得到
@return 操作成功返回TRUE， 操作失败返回FALSE
*/
BOOL CSSYSVBIStopTeletext(HCSHANDLE hHandle);

/**@brief 打开和关闭VBI CLOSE CAPTION

@param[in] hHandle 设备通道句柄，系统初始化得到
@param[in] bEnable 为TRUE打开，FALSE关闭
@return 操作成功返回TRUE， 操作失败返回FALSE
*/
BOOL CSSYSVBIEnableCloseCaption(HCSHANDLE hHandle,BOOL bEnable);

/**@brief 打开和关闭VBI Macrovision

@param[in] hHandle 设备通道句柄，系统初始化得到
@param[in] bEnable 为TRUE打开，FALSE关闭
@return 操作成功返回TRUE， 操作失败返回FALSE
*/
BOOL CSSYSVBISetMacrovision(HCSHANDLE hHandle, BOOL bEnable);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* !_CS_SYSVBI_H_ */


