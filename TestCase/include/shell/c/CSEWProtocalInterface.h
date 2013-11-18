/**@defgroup EW200ProtocalExtern EW200 �Զ���Э����չ

��ģ��Ϊ�ٻ�200�м���Զ���Э�����չ�ӿ�,
����Ϊ�ٻ�200�����µ��Զ���Э��֧��,��ʹ�ñ�ģ��.
��ν�Զ���Э����ָ���������������ʶ���Э������ʱ,
����Ӧ��ָ���ĳ�����ߺ������������.
���¿����Э��ed2k,�������������������ed2k://|file|test.rmvb|/������ʱʱ������¿���д���

@note �����ٻ�200����UDI,����δ��ע����,��μ�UDI�ĵ�
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
@brief ��һ�������Э����չ��ӵ��ٻ�200��

ʹ�ðٻ�200����֧�ָ����͵�Э����չ
@param[in] cProtocalHandle �����Э����չʵ��,������ɸ�Э����չ�ľ��幦��
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWExternAddProtocal(ProtocalInterface &cProtocalHandle);

class ProtocalInterface
{
public:
    virtual ~ProtocalInterface(void) {}
    /**@brief ��ȡЭ������,��http,ed2k��*/
    virtual const char* GetScheme(void) = 0;
    /**
    @brief �����Э��URL�ĺ���
    @return �ɹ�������TRUE;δ������FALSE
    */
    virtual CSUDI_BOOL NotifyProtocal(const char* pUrl, int nUrlLength) = 0;
};

#ifdef __cplusplus
}
#endif

#endif  //_PROTOCALINTERFACE_H

/** @} */


