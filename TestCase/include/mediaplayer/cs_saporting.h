/******************************************************************************
FileName:  cs_saporitng.h

File Description:
        citv server porting interface
        用该层的代码屏蔽SA模块的依赖性，以方便SA在整个MMCP环境之外使用        
        如果在MMCP中使用SA，则不必关注此模块

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2009-02-26  lihui           Create
*******************************************************************************/
#ifndef CS_SA_PORTING_H
#define CS_SA_PORTING_H

#include "mmcp_typedef.h"
#include "mmcp_tuner.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 定义SA获取IPQam域信息接口

认证模块通过调用终端实现的该接口得到IPQam的域信息
目前取参数指定Section的第4、5字节(即data[3],data[4])
@param[in] pDeliverList 存放信息所在频点列表数组
@param[in] nDeliverCount pDeliverList数组中的频点个数
@param[in] wPid IPQam域信息所在Pid
@param[in] ucTableId IPQam域信息所在TableId
@param[out] pIpQamDataBuffer 用于存放IPQam域信息的内存指针
@param[in] nBufferLength pIpQamDataBuffer的长度
@param[in] pCookie 配置层自定义数据,等于SA_Interface中的pCookie
@return 成功获取返回TRUE,失败返回FALSE
*/
typedef BOOL (*FOnCSSAGetIPQamDomainInfo)(const CSUDITunerSRCDeliver_U* pDeliverList,
    int nDeliverCount,
    WORD wPid,
    BYTE ucTableId,
    BYTE* pIpQamDataBuffer,
    int nBufferLength,
    void* pCookie
    );

/**
@brief 定义SA停止获取IPQam域信息接口
由于获取IPQam的域信息耗费的时间可能比较长，此接口允许用户在
获取IPQam的域信息过程中停止获取。
@return 成功获取返回TRUE,失败返回FALSE
*/
typedef BOOL (*FOnCSSAStopGetIPQamDomainInfo)();
/**@brief SA依赖层接口结构,SA模块依赖于以下功能实现*/
typedef struct _SA_Interface
{
    void*                   pCookie;                ///依赖层自定义数据,调用一下函数时以参数形式返给依赖层
    FOnCSSAGetIPQamDomainInfo   fnGetIPQamDomainInfo;   ///获取IPQam域接口,认证时需要通过该接口获取IPQam域信息
	FOnCSSAStopGetIPQamDomainInfo fnStopGetIPQamDomainInfo;//停止获取IPQam域接口
}CS_SA_Interface;

/**
@brief 注册SA依赖层接口

初始化前调用该函数注册SA模块的依赖接口
@param[in] pSaInterface 依赖曾接口结构
@return 成功返回TRUE,失败返回FALSE
@note 接口将对结构进行浅拷贝,函数返回后SA_Interface结构本身不再需要,但pCookie指向的内容不会也无法复制
*/
BOOL CSSARegInterface(CS_SA_Interface* pSaInterface);


#ifdef __cplusplus
}
#endif

#endif


