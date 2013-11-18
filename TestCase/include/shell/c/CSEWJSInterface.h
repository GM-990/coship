/**@defgroup EW200JSExtern EW200 �Զ���JS��չ�ӿ�

��ģ��Ϊ�ٻ�200�м���Զ���JavaScript��չ�ӿ�,
����Ϊ�ٻ�200�����µ��Զ���JavaScript֧��,��ʹ�ñ�ģ��.

@note �����ٻ�200����UDI,����δ��ע����,��μ�UDI�ĵ�
@todo ��ģ����δ���,Ŀǰ��������IOCtrl���Զ��崦��,���ܻ���һ�汾�г��ֽϴ�䶯
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
@brief ��һ������JS��չ��ӵ��ٻ�200��

ʹ�ðٻ�200����֧�ָ�����չ��JavaScript����
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWExternAddJS(JSInterface &cJSHandle);

class JSInterface
{
public:
    virtual ~JSInterface(void) {}
    /**
    @brief ��ͨ��IO��ʽ��չJavaScript��д����

    ��JavaScript����coship.ioctlWrite������չ,ʹ֧֮�ָ���Ĺؼ���
    @return �ɹ�����nValueLen,ʧ�ܻ�ʶ��Ĺؼ��ַ���-1
    @note �ܿ��ܻ��в�ʶ��Ĺؼ��ִ���,���������
    */
    virtual int ioWrite(char * key, char * value, int nValueLen)
    {
        return -1;
    }

    /**
    @brief ��ͨ��IO��ʽ��չJavaScript�Ķ�����

    ��JavaScript����coship.ioctlRead������չ,ʹ֧֮�ָ���Ĺؼ���
    @return �ɹ�����ʵ�ʶ������ֽ���,ʧ�ܻ�ʶ��Ĺؼ��ַ���-1
    @note �ܿ��ܻ��в�ʶ��Ĺؼ��ִ���,���������
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


