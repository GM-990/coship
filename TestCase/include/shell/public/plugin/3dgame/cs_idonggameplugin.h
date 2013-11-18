#ifndef _CSIDONGGAMEPLUGIN_H
#define _CSIDONGGAMEPLUGIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 由工程调用CSGTRegisterIoctlFun注册，用于MMCP和第三方游戏切换
pcKey:游戏名描述
nKeyLen:可为NULL;
pcValue:可为NULL;
nValueLen:可为NULL;
cs_idonggameplugin.h
详情可见cs_gools.h
*/
int CSIDONGGAMEIoWrite(const char * pcKey, int nKeyLen, char * pcValue, int nValueLen);

/**
@brief 由工程调用CSGTRegisterIoctlFun注册，用于MMCP和第三方游戏切换,可以不实现
pcKey:游戏名描述
nKeyLen:可为NULL;
pcValue:可为NULL;
nValueLen:可为NULL;
详情可见cs_gools.h
*/
int CSIDONGGAMEIoRead(const char * pcKey, int nKeyLen, char * pcReadBuf, int nBufLen);

#ifdef __cplusplus
}
#endif
#endif  

