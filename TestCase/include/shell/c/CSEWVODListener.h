#ifndef _VODLISTENER_H
#define _VODLISTENER_H

#include "CSEWPublic.h"
#include "CSEWVod.h"

#ifdef __cplusplus
extern "C"
{
#endif
class VODInterface;

/**
@brief VOD监听者

VOD实例通过VODListener的实例发送VOD相关的信息
*/
class VODListener
{
public:
    virtual ~VODListener(void) {}
    /**@param[in] pVod 发送该消息的VOD实例*/
    virtual int Notify(CSEWVODMsgType_E eMsg, CSEWParam_S *psParam, VODInterface* pVOD) = 0;
};

#ifdef __cplusplus
}
#endif
#endif  //_VODLISTENER_H

