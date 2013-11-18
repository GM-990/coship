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
@brief VOD������

VODʵ��ͨ��VODListener��ʵ������VOD��ص���Ϣ
*/
class VODListener
{
public:
    virtual ~VODListener(void) {}
    /**@param[in] pVod ���͸���Ϣ��VODʵ��*/
    virtual int Notify(CSEWVODMsgType_E eMsg, CSEWParam_S *psParam, VODInterface* pVOD) = 0;
};

#ifdef __cplusplus
}
#endif
#endif  //_VODLISTENER_H

