/**@defgroup EW200JSExtern EW200 自定义JS扩展接口

本模块为百汇200中间件自定义JavaScript扩展接口,
若需为百汇200增加新的自定义JavaScript支持,请使用本模块.

@note 整个百汇200依赖UDI,部分未标注申明,请参见UDI文档
@todo 本模块尚未完成,目前仅能增加IOCtrl的自定义处理,可能会下一版本中出现较大变动
@{
*/
#ifndef _JSINTERFACE_H
#define _JSINTERFACE_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "udi2_typedef.h"
class JSInterface;

/**
@brief 将一组具体的JS扩展添加到百汇200中

使得百汇200可以支持该组扩展的JavaScript函数
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWExternAddJS(JSInterface &cJSHandle);

class JSInterface
{
public:
    virtual ~JSInterface(void) {}
    /**
    @brief 以通用IO方式扩展JavaScript的写操作

    对JavaScript函数coship.ioctlWrite进行扩展,使之支持更多的关键字
    @return 成功返回nValueLen,失败或不识别的关键字返回-1
    @note 很可能会有不识别的关键字传入,请谨慎处理
    */
    virtual int ioWrite(char * key, char * value, int nValueLen)
    {
        return -1;
    }

    /**
    @brief 以通用IO方式扩展JavaScript的读操作

    对JavaScript函数coship.ioctlRead进行扩展,使之支持更多的关键字
    @return 成功返回实际读到的字节数,失败或不识别的关键字返回-1
    @note 很可能会有不识别的关键字传入,请谨慎处理
    */
    virtual int ioRead(char * key, char * value, int nValueLen)
    {
        return -1;
    }
};


#ifdef __cplusplus
}
#endif

#endif  //_JSINTERFACE_H

/** @} */


