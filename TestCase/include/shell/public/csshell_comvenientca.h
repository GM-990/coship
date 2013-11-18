
#ifndef CSSHELL_ComvenientCA_H
#define CSSHELL_ComvenientCA_H


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
#include "thirdparty/ca/comvenient/CS_COMVENIENT_Interface.h"
#include "CSUSPDMCommonType.h"

/**
@brief ComvenientCA信息及相关操作结构体
@note 目前mmcp提供了Comvenient ca的完整默认实现。一般情况下，项目使用默认实现即可满足需求
@see CSShellComvenientCaSetParam CSShellComvenientCaGetDefaulParam
*/
typedef struct
{
    int                   m_nSize;    ///< 本结构体的大小，必须赋值为sizeof(CSShellComvenientCaInitParam_S)
    CSShellCaType_E       m_eCaType;       ///< CA类型，本结构体中固定为EM_CSSHELL_CA_TYPE_Comvenient
    CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口,native直接调用此方法获取智能卡号
    INT (*m_fnGetCaBoxId)(BYTE* pcBoxId,INT *pnLength);  //CSGetCaBoxId  //此处由于CA头文件变动，编译错误，跟头文件保持一致
    void (*m_fnGetEpgTime)(CaDateTime_S* pstDataTime);  //CSCAGetEpgTime
    INT  (*m_fnChangeCardPin)(BYTE ucOldPinLength,BYTE* pcOldPin,BYTE ucNewPinLength,BYTE* pcNewPin,CaPinIndex_E ePinIndex);  //CS_CA_ChangePin
    INT  (*m_fnCheckCardPin)(BYTE  ucPinLength, BYTE* pcPin,CaPinInfo_S sPinInfo);   //CS_CA_CheckCardPin
    INT  (*m_fnGetCardNumber)(BYTE* pCSmcNo, WORD wLen);  //CS_CA_GetCardNumber
    INT  (*m_fnGetPurseInfo)(void);   //CS_CA_GetPurseInfo
    WORD  (*m_fnGetCaSystemId)(void);   //CA_INF_GetCaSystemId
    void  (*m_fnGetBCLibVersion)(BYTE* pacVersion, BYTE* pacDate, BYTE* pacTime) ;    //CS_CA_GetBCLibVersion
    INT  (*m_fnGetParingState)(void);   //CA_CA_GetParingState
    INT (*m_fnAddMail)(CaMail_S* pstTxt);  //CS_CA_TXT_Add
    INT (*m_fnDelMail)(DWORD dwIndex);  //CS_CA_TXT_Del
    INT (*m_fnDelMailAll)(void);  //CS_CA_TXT_DelAll
    INT (*m_fnGetEmailNum)(void); //CS_CA_TXT_GetNum
    INT (*m_fngetMailInfoByIndex)(DWORD dwIndex, CaMail_S** ppstTxt);  //CS_CA_TXT_GetInfo
    BOOL (*m_fnhaveNew)(void);   //CS_CA_TXT_HaveNew
    INT (*m_fngetNewMailNum)(void);  //CS_CA_TXT_HaveNewMailNum
    INT (*m_fnsaveMailtoFlash)(void);   //CS_CA_TXT_Save
    INT (*m_fnsetReadFlag)(DWORD dwIndex, BOOL bRead);  //CS_CA_TXT_SetReadFlag
    BOOL (*m_fnGetIntersectionDeliverListByDM)(Deliver* pDeliverFromCA, int nDeliverCACount, Deliver *pDeliverRet, int* nDeliverRetCount);  //频点检测 
}CSShellComvenientCaInitParam_S;

/**@brief ComvenientCA相关配置信息结构体*/
typedef struct
{
	DWORD  m_dwCaMailAddress;///<邮件的首地址	
	DWORD  m_dwCaFlashAddress;	
	DWORD  m_dwCaBackupFlashAddress;	
}CSShellComvenientCaConfig_S;


/**
@brief 设置ComvenientCA相关操作接口给MMCP
@param[in] ComvenientCA信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@note 必须在调用CSShellComvenientCaSetConfig之前调用该接口。
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellComvenientCaSetParam(CSShellComvenientCaGetDefaulParam());
@code end
*/
BOOL CSShellComvenientCaSetParam(const CSShellComvenientCaInitParam_S* psComvenientCa);

	
/**
@brief 获取Comvenient CA默认实现参数
@return MMCP提供Comvenient CA Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native)不得直接使用本接口，若MMCP需要获得同方3CA的对象须使用csshell_Comvenientca_private.h中的CSShellComvenientCaGetParam接口
@see CSShellComvenientCaGetParam
*/
const CSShellComvenientCaInitParam_S* CSShellComvenientCaGetDefaulParam(void);

/**
@brief 设置Comvenient CA的相关配置信息
@param[in] psConfig Comvenient CA相关配置信息的结构体实例
@note 该接口必须在CSShellComvenientCaSetParam之后调用
@see CSShellComvenientCaConfig_S
*/
void CSShellComvenientCaSetConfig(CSShellComvenientCaConfig_S* psConfig);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


