#ifndef _CSNOGDSA_H_
#define _CSNGODSA_H_
#include <stdio.h>
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern  "C" {	 //在C++中将用C格式编译extern  “C” {}内的函。注意：大写“C”
#endif
 
/*频点结构体*/
typedef struct _NgodSaFreqInfo_S
{
	unsigned int uFrequency;   //频率 单位 hz
	unsigned int uSymnolRate;  //符号率
	unsigned int uQamMode;     //调制解调方式
}CSNgodSaFreqInfo_S;

//频道信息结构体
typedef struct _NgodSaChannelInfo_S
{
   int  nChannelId;          //频道ID
   int  nServiceId ;         
   int  nTsId;
   int nOriginalNetworkId;
   BOOL bIsStartOver;        //是否支持回看
   BOOL bIsNpvr;             //是否支持Npvr
   BOOL bIsTVAnyTime;         //是否支持时移
   BOOL bNpvrOrderFlag;       //是否已订购Npvr
   BOOL bTVAnyTimeOrderFlag; //是否已订购时移
   BOOL bStartOverOrderFlag;  //是否已订购回看
  
}CSNgodSaChannelInfo_S;

typedef enum  _CSNgodSaRet_E
{
	EM_NGODSA_SUCCESS,
	EM_NGODSA_INVALIDPARA,
	EM_NGODSA_FAILURE,
	EM_NGODSA_NETERROR
}CSNgodSaRet_E;

typedef enum _CSNgodSaType_E
{
	EM_NGODSA_TYPE_COMMON,
	EM_NGODSA_TYPE_CA,
	EM_NGODSA_TYPE_CAINIT_OK
}CSNgodSaType_E;


/**
@brief 创建一个NGOD 认证模块句柄
@param[in] phSaHandle 
@param[out] phSaHandle  NGOD 认证模块句柄
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note 在开机认证时要先调用此接口创建认证句柄，根据句柄向认证服务器发送认证请求
*/
CSNgodSaRet_E CSNgodSaCreate(HCSHANDLE *phSaHandle); 

/**
@brief 获取NOGDSA 模块句柄
@param[out] phHandle NGOD 认证模块句柄
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note 
*/
CSNgodSaRet_E CSNgodSaGetHandle(HCSHANDLE *phHandle);
/**
@brief 销毁一个NGOD 认证模块句柄
@param[in] hSaHandle NGOD 认证模块句柄
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note 在开机认证和获取区域码完成之后要调用此接口销毁认证句柄
*/
CSNgodSaRet_E CSNgodSaDestory(HCSHANDLE hSaHandle);

/**
@brief 设置卡号
@param[in] hSaHandle: NGOD 认证模块句柄
@param[in] pcSmcId: 传入卡号字符串的首地址
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note 在发起认证请求之前 要先调用此接口把卡号设置进去
*/
CSNgodSaRet_E CSNgodSaSetSmartCard(HCSHANDLE hSaHandle,char *pcSmcId);

/**
@brief 设置认证服务器的IP地址和端口号
@param[in] hSaHandle: NGOD 认证模块句柄
@param[in] pServerIp: 传入IP地址字符串的首地址
@param[in] nServerPort: 认证服务器的端口号
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note 在发起认证请求之前 要先调用此接口把认证服务器的IP地址和端口号设置进去
*/
CSNgodSaRet_E CSNgodSaSetServerInfo(HCSHANDLE hSaHandle,char *pcServerIp,int nServerPort);

/**
@brief  设置服务器响应超时时间
@param[in] hSaHandle: NGOD 认证模块句柄
@param[in] nTimeout: 用户传入超时时间，单位毫秒
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note   在认证请求之前调用此接口,传入超时时间,发送认证请求时服务器响应超时时间已用户设置为准，
		用户可以不调用此接口，服务器响应超时时间为默认值4000。
*/
CSNgodSaRet_E  CSNgodSaSetTimeout(HCSHANDLE hSaHandle, int nTimeout);

/**
@brief 发起认证请求并从认证服务器获取频点列表
@param[in] hSaHandle: NGOD 认证模块句柄
@param[inout] pnFreqCount: 用户传入的结构体数组的大小,返回实际获取频点的个数
@param[out]psFreqInfo:  返回频点信息列表存入以psFreqInfo为首地址的结构体数组中
@param[out]pnErrCode: 返回服务器错误码
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note 此函数为阻塞函数，直到等到服务器返回或达到超时时间才会返回
*/
CSNgodSaRet_E CSNgodSaNavCheck(HCSHANDLE hSaHandle,CSNgodSaFreqInfo_S *psFreqInfo, int* pnFreqCount,int *pnErrCode);

/**
@brief  从认证服务器获取频道列表
@param[in] hSaHandle: NGOD 认证模块句柄
@param[inout] pnChnlCount: 用户传入的结构体数组的大小,返回实际获取频道的个数
@param[out]psChnlsInfo: 返回频道列表存入以psChnlsInfo为首地址的结构体数组中
@param[out]pnErrCode: 返回服务器错误码
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note 此函数为阻塞函数，直到服务器返回或达到超时时间后才会返回
*/
CSNgodSaRet_E CSNgodSaGetChannels(HCSHANDLE hSaHandle, CSNgodSaChannelInfo_S *psChnlsInfo, int*pnChnlCount,int *pnErrCode);

/**
@brief  获取区域码
@param[in] hSaHandle: NGOD 认证模块句柄
@param[in] psFreqInfo: 用户传入频点信息列表的结构体数组首地址
@param[in] nFreqCount: 用户传入的结构体数组的大小
@param[in] bAutodeployerRunning: 应用部署是否完成
@param[in] bGetAreaCodeType 为TRUE表示锁定所有频点来获取区域码，为FALSE表示只锁定频点列表中最小的两个频点来获取区域码
@param[out]pnAreaCode: 返回区域码
@param[out]psOutFreqInfo: 返回获取区域码时锁频成功的频点
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note 此函数为阻塞函数，直到锁频成功并且获取区域码后才会返回
*/
CSNgodSaRet_E CSNgodSaGetAreaCode(HCSHANDLE hSaHandle, BOOL bAutodeployerRunning, CSNgodSaFreqInfo_S *psFreqInfo,const int nFreqCount,int *pnAreacode,CSNgodSaFreqInfo_S *psOutFreqInfo, BOOL bGetAreaCodeType);

/**
@brief  设置机顶盒与npt时间同步
@param[in] hSaHandle: NGOD 认证模块句柄
@return 成功返回EM_NGODSA_SUCCESS ,失败则返回EM_NGODSA_FAILURE或EM_NGODSA_INVALIDPARA
@note 
*/
CSNgodSaRet_E  CSNgodSaTimeSync(HCSHANDLE hSaHandle ,char *pcNptServer);


/**
@brief 点播时发现区域码为空的情况下获取区域码
@param[in] none
@param[out]pnTempAreaCode: 返回区域码
@return 成功返回0,失败返回-1
@note 此函数中获取区域码时，如果读取配置文件上次锁定获取区域码成功的频点不为空，优先锁
@定改频点，如果为空或者锁定改频点获取区域码失败，那么只锁定频点列表中最小的前两个频点
*/
int CSVodCheckGetAreacode(int *pnTempAreaCode);

/**
@brief  判断portal是否已经启动
@returnTRUE 表示已经启动了，FALSE表示尚未启动
@note 
*/
BOOL CSNgodSa_IsPortalRunning();

/**
@brief  设置是否校验区域码锁频成功的频点是否是频点列表中的第一个频?
@param[in] bCheck TRUE表示校验，如果锁频成功的频点不是第一个频点，那么再从第一个频点开始循环锁一次, 默认为FALSE
@return 无
@note 
*/
void CSNgodSa_SetCheckLockedFreqMode(BOOL bCheck);


#ifdef __cplusplus
}
#endif

#endif



