/**@defgroup EW200ProtocalExtern EW200 自定义协议扩展

本模块为百汇200中间件自定义协议的扩展接口,
若需为百汇200增加新的自定义协议支持,请使用本模块.
所谓自定义协议是指当浏览器碰到不可识别的协议链接时,
可由应用指定的程序或者函数处理该链接.
如电驴下载协议ed2k,即当浏览器碰到类似于ed2k://|file|test.rmvb|/的链接时时启动电驴进行处理

@note 整个百汇200依赖UDI,部分未标注申明,请参见UDI文档
@{
*/
#ifndef _PROTOCALINTERFACE_H
#define _PROTOCALINTERFACE_H

#include "udi2_typedef.h"


#ifdef __cplusplus
extern "C"
{
#endif

class ProtocalInterface;

/**
@brief 将一个具体的协议扩展添加到百汇200中

使得百汇200可以支持该类型的协议扩展
@param[in] cProtocalHandle 具体的协议扩展实例,负责完成该协议扩展的具体功能
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWExternAddProtocal(ProtocalInterface &cProtocalHandle);

class ProtocalInterface
{
public:
    virtual ~ProtocalInterface(void) {}
    /**@brief 获取协议名称,如http,ed2k等*/
    virtual const char* GetScheme(void) = 0;
    /**
    @brief 处理该协议URL的函数
    @return 成功处理返回TRUE;未处理返回FALSE
    */
    virtual CSUDI_BOOL NotifyProtocal(const char* pUrl, int nUrlLength) = 0;
};

#ifdef __cplusplus
}
#endif

#endif  //_PROTOCALINTERFACE_H

/** @} */


