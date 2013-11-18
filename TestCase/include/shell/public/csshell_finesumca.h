
#ifndef CSSHELL_finesumCA_H
#define CSSHELL_finesumCA_H


#ifdef CSSHELL_CA_INCLUDED
@error Error! more then one CA Shell file included in the same file,pls check your code!
#endif
#define CSSHELL_CA_INCLUDED

#ifdef __cplusplus
extern "C"
{
#endif

#include "csshell_ca.h"
#include "mmcp_typedef.h"
#include "thirdparty/ca/finesum/csfinesumcainterface.h"
/**
@brief finesumCA信息及相关操作结构体
@note 目前mmcp提供了finesum ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellFinesumCaSetParam CSShellFinesumCaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellFinesumCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_FINESUM
	
    CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号
    INT (*m_fnGetMailNum)(INT* pnEmailNum, INT* pnEmptyNum);///< CSCAGetMailNum
    INT (*m_nfnGetMailHead)(INT nIndex, CaEmailHead_S *pstMailHead);///< CSCAGetMailHead
    INT (*m_nfnGetMailContentByIndex)(INT nIndex, CaEmailContent_S *pstMailContent);///<CSCAGetMailContentByIndex
    INT (*m_fnGetContentByEmailId)(WORD wEmailId, CaEmailContent_S *pstMailContent);///<CSCAGetMailContentByEmailId
    INT (*m_fnDelMail)(INT nIndex);///< CSCADelMail
    INT (*m_fnDelMailAll)(void);///< CSCADelMailAll
    INT (*m_fnGetCaInfo)(CaInfo_S *pstCaInfo);///< CSCAGetCaInfo
    CA_ReturnValue_E (*m_fnPairstatus)(void) ;///< CSCAGetPairstatus 
    void (*m_fnNotifyShowOSDOver)(void); ///<CSCANotifyShowOSDOver
    CA_ReturnValue_E (*m_fnGetCardID)(BYTE* paucCardID);///<CSCAGetCardID
}CSShellFinesumCaInitParam_S;

/**@brief FinesumCA相关配置信息结构体*/
typedef struct
{
	DWORD  m_dwMailAddress;///<邮件的首地址
	DWORD  m_dwMailAddressBak;///<邮件备份的首地址
}CSShellFinesumCaConfig_S;

/**
@brief 设置FinesumCA相关操作接口给MMCP
@param[in] FinesumCA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellFinesumCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellFinesumCaSetParam(CSShellFinesumCaGetDefaulParam());
@code end
*/
BOOL CSShellFinesumCaSetParam(const CSShellFinesumCaInitParam_S* psFinesumCa);

	
/**
@brief 获取同方3CA默认实现参数
@return MMCP提供同方3CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得同方3CA的对象须使用csshell_finesumca_private.h中的CSShellFinesumCaGetParam接口
@see CSShellFinesumCaGetParam
*/
const CSShellFinesumCaInitParam_S* CSShellFinesumCaGetDefaulParam(void);

/**
@brief 设置同方3CA的相关配置信息
@param[in] psConfig 同方3CA相关配置信息的结构体实例
@note 该接口必须在CSShellFinesumCaSetParam之后调用
@see CSShellFinesumCaConfig_S
*/
void CSShellFinesumCaSetConfig(CSShellFinesumCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


