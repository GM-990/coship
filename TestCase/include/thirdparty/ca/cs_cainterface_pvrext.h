#ifndef _CS_CAINTERFACE_PVR_EXT_H_
#define _CS_CAINTERFACE_PVR_EXT_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

#define CS_CA_MAX_PID_NUM     (10)
#define CS_CA_CW_SIZE         (8)

typedef struct _CSCaCtrlWord_S
{
	WORD    m_wPid;
	BOOL    m_bEkey;
	BYTE    m_aucKey[CS_CA_CW_SIZE];
}CSCaCtrlWord_S;

//用于将CW回传给应用，预留 
typedef void (*CSOnCaCtrlWordCome_F)(HCSHANDLE hService, CSCaCtrlWord_S* psPvrKey, void* pUserData); 
 

typedef enum _CSCaOutputType_E
{
	EM_CA_OUTPUT_PLAY   = 0x01, /*播放*/
	EM_CA_OUTPUT_RECORD = 0x02, /*录制*/
	EM_CA_OUTPUT_CAT    = 0x04  /*进入界面时，特殊处理CAT*/
}CSCaOutputType_E;

typedef struct _CSCaFtaService
{
	WORD    m_wDemuxId;          //DMX ID号
	WORD    m_wOrginalNetworkId; //原始网络ID号
	WORD    m_wTsId;             //流ID号
	WORD    m_wServiceId;        //节目ID号
	WORD    m_wPidNum;           //Pid 个数
	DWORD   m_dwDesNo;
	WORD    m_awPid[CS_CA_MAX_PID_NUM]; //Pid
	CSCaOutputType_E m_eOutputType;     //输出类型,NDS CA要用到
	CSOnCaCtrlWordCome_F m_fnOnCaCtrlWordCome;
	void*   m_pUserData;         //预留
}CSCaFtaService_S;

typedef struct _FuncbParam
{
	DWORD    m_hService;
	DWORD    m_dwUserData;
}FuncbParam_S;

/**************************************************************************
*Function Name: CSCASetSerivcePmtPid
*  设置节目的PmtPid
*
*Parameters 
*   wDemuxId:demux id
*	wOriNetId[in]:节目的原始网络id
*	wTsID[in]:节目的ts id
*	wServiceID[in]:节目的service id
*	wPmtPid[in]:pmt表的pmt pid
*
*Note:此函数需要在CSCAStartService之前调用
*     
**************************************************************************/
void CSCASetSerivcePmtPid(WORD wDemuxId, WORD wOriNetId, WORD wTsID, WORD wServiceID, WORD wPmtPid);

/**************************************************************************************************
 *FunctionName : CSCAStartService
 *Descriptor   : 启动播放/录制
 *Parame[IN]   : pstService - 节目相关信息
 *返回值       : NULL  - 启动播放/录制不成功
 *               非NULL- 启动播放/录制成功
 **************************************************************************************************/
HCSHANDLE CSCAStartService(CSCaFtaService_S* pstService);

/**************************************************************************************************
 *FunctionName : CSCAStopService
 *Descriptor   : 停止播放/录制
 *Parame[IN]   : hService - 启动/录制时分配的Handle
 *返回值       : SUCCESS - 停止播放/录制成功
 *               FAILURE - 停止播放/录制不成功
 **************************************************************************************************/
INT CSCAStopService(HCSHANDLE hService);

/**************************************************************************************************
 *FunctionName : CSCANewPmtSectionNotify
 *Descriptor   : 通知CA模块新的PMT
 *Parame[IN]   : pcSectionData - PMT数据
 *               nLen - PMT长度
 *               hService - 启动/录制时分配的Handle
 *返回值       : SUCCESS - 传送PMT符合标准
 *               FAILURE - 传送PMT不符合标准
 **************************************************************************************************/
INT CSCANewPmtSectionNotify(HCSHANDLE hService, BYTE* pcSectionData, INT nLen);


/**************************************************************************************************
 *Function:   为了支持多路解扰，回调函数扩展一个参数用于区分哪一路节目的消息
 *Descriptor: dwParam3是声明结构FuncbParam_S对应的地址，应用上收到消息后，取dwParm3时要转换成
 *            FuncbParam_S结构体，m_hService成员是用于区分哪一路节目/哪个模块的消息，便于应用
 *            处理对应的消息,m_dwUserData成员对应于CSCARegisterMsgCallback注册回调时用户传过来的
 *			  自定义数据dwUserData
 **************************************************************************************************/
typedef void (*F_CBMsg)(DWORD dwType, DWORD dwParm1, DWORD dwParm2, DWORD dwParm3);

/**************************************************************************************************
 *FunctionName: CSCARegisterMsgCallback
 *Descriptor  : 用于要启动的播放模块来注册回调函数，支持多个模块注册，该函数放在CS_CA_Init之前调用，
 *              每个模块注册时，CA模块都会登记，最多同时支持6个模块注册
 **************************************************************************************************/
INT CSCARegisterMsgCallback(DWORD dwUserData,F_CBMsg fcbMsg);

/**************************************************************************************************
 *FunctionName: CSCARegisterMsgCallbackCancel
 *Descriptor  : 用于注销已经注册回调函数
 * 
 **************************************************************************************************/
INT CSCARegisterMsgCallbackCancel(DWORD dwUserData,F_CBMsg fcbMsg);


#ifdef  __cplusplus
}
#endif

#endif

