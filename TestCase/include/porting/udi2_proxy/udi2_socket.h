#ifndef _UDI2_SOCKET_H_PROXY
#define _UDI2_SOCKET_H_PROXY

#include "udi2_typedef.h"
#include "../udi2/udi2_socket.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDISOCKSocket(format, ...) CSUDISOCKSocket_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)


 
 int CSUDISOCKSocket_PROXY(int nDomain,int nType,int nProtocol, const char *pcFuncName, const int nLine);

#define CSUDISOCKBind(format, ...) CSUDISOCKBind_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKBind_PROXY(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKListen(format, ...) CSUDISOCKListen_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKListen_PROXY(int nFd,int nNum, const char *pcFuncName, const int nLine);

#define CSUDISOCKConnect(format, ...) CSUDISOCKConnect_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKConnect_PROXY(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKAccept(format, ...) CSUDISOCKAccept_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKAccept_PROXY(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKSend(format, ...) CSUDISOCKSend_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKSend_PROXY(int nFd,const void * pvBuf,unsigned int uNum,int nFlags, const char *pcFuncName, const int nLine);

#define CSUDISOCKRecv(format, ...) CSUDISOCKRecv_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKRecv_PROXY(int nFd,void * pvBuf,unsigned int uNum,int nFlags, const char *pcFuncName, const int nLine);

#define CSUDISOCKSendTo(format, ...) CSUDISOCKSendTo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKSendTo_PROXY(int nFd,const void * pvBuf,unsigned int uNum,int nFlags,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKRecvFrom(format, ...) CSUDISOCKRecvFrom_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKRecvFrom_PROXY(int nFd,void * pvBuf,unsigned int uNum, int nFlags,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKSelect(format, ...) CSUDISOCKSelect_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKSelect_PROXY(int nFds,CSUDIFdSet_S * psReadnFds,CSUDIFdSet_S * psWritenFds,CSUDIFdSet_S * psExceptnFds,CSUDITimeval_S * psTimeout, const char *pcFuncName, const int nLine);

#define CSUDISOCKFcntl(format, ...) CSUDISOCKFcntl_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKFcntl_PROXY(int nFd,int nCmd,int nArg1,int nArg2, const char *pcFuncName, const int nLine);

#define CSUDISOCKIoctl(format, ...) CSUDISOCKIoctl_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKIoctl_PROXY(int nFd,unsigned int ulRequest,void * pArg1,void * pArg2, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetSockOpt(format, ...) CSUDISOCKGetSockOpt_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKGetSockOpt_PROXY(int nFd,int nLevel,int nOptName,void * pvOptVal,unsigned int * puOptLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKSetSockOpt(format, ...) CSUDISOCKSetSockOpt_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKSetSockOpt_PROXY(int nFd,int nLevel,int nOptName,const void * pvOptVal, unsigned int uOptLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetProtoByName(format, ...) CSUDISOCKGetProtoByName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDIProtoent_S * CSUDISOCKGetProtoByName_PROXY(const char * pcProtoName, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetServByName(format, ...) CSUDISOCKGetServByName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDIServent_S * CSUDISOCKGetServByName_PROXY(const char * pcServName,const char * pcProtoName, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetPeerName(format, ...) CSUDISOCKGetPeerName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKGetPeerName_PROXY(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetSockName(format, ...) CSUDISOCKGetSockName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKGetSockName_PROXY(int nFd,CSUDISockAddr_S * psAddr, unsigned int * puAddrLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetHostName(format, ...) CSUDISOCKGetHostName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKGetHostName_PROXY(char * pcHostName,unsigned int uAddrLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetHostByName(format, ...) CSUDISOCKGetHostByName_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDIHostent_S * CSUDISOCKGetHostByName_PROXY(const char * pcHostName, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetHostByName_r(format, ...) CSUDISOCKGetHostByName_r_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKGetHostByName_r_PROXY(const char * pcHostName,CSUDIHostent_S * psResultBuf,char * pcBuf,unsigned int uBufLen,CSUDIHostent_S ** psResult,int * pnErrCode, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetHostByAddr(format, ...) CSUDISOCKGetHostByAddr_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDIHostent_S * CSUDISOCKGetHostByAddr_PROXY(const void * pvAddr,unsigned int uAddrLen,int nType, const char *pcFuncName, const int nLine);

#define CSUDISOCKInet_addr(format, ...) CSUDISOCKInet_addr_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDIInAddr_t CSUDISOCKInet_addr_PROXY(const char * pcIp, const char *pcFuncName, const int nLine);

#define CSUDISOCKInet_aton(format, ...) CSUDISOCKInet_aton_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDIInAddr_t CSUDISOCKInet_aton_PROXY(const char * pcIp, CSUDIInAddr_S * psIpNet, const char *pcFuncName, const int nLine);

#define CSUDISOCKInet_ntoa(format, ...) CSUDISOCKInet_ntoa_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
char * CSUDISOCKInet_ntoa_PROXY(CSUDIInAddr_S sIpNet, const char *pcFuncName, const int nLine);

#define CSUDISOCKInet_pton(format, ...) CSUDISOCKInet_pton_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKInet_pton_PROXY(int nAf,const char *pSrc, void *pDst, const char *pcFuncName, const int nLine);

#define CSUDISOCKInet_ntop(format, ...) CSUDISOCKInet_ntop_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
const char * CSUDISOCKInet_ntop_PROXY(int nAf,const void *pSrc,char *pDst,int nDsrLen, const char *pcFuncName, const int nLine);

#define CSUDISOCKNtohs(format, ...) CSUDISOCKNtohs_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
unsigned short CSUDISOCKNtohs_PROXY(unsigned short ushNet, const char *pcFuncName, const int nLine);

#define CSUDISOCKHtons(format, ...) CSUDISOCKHtons_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
unsigned short CSUDISOCKHtons_PROXY(unsigned short ushHost, const char *pcFuncName, const int nLine);

#define CSUDISOCKNtohl(format, ...) CSUDISOCKNtohl_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
unsigned int CSUDISOCKNtohl_PROXY(unsigned int uNet, const char *pcFuncName, const int nLine);

#define CSUDISOCKHtonl(format, ...) CSUDISOCKHtonl_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
unsigned int CSUDISOCKHtonl_PROXY(unsigned int uHost, const char *pcFuncName, const int nLine);

#define CSUDISOCKShutDown(format, ...) CSUDISOCKShutDown_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKShutDown_PROXY(int nFd,int nHow, const char *pcFuncName, const int nLine);

#define CSUDISOCKClose(format, ...) CSUDISOCKClose_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDISOCKClose_PROXY(int nFd, const char *pcFuncName, const int nLine);

#define CSUDISOCKGetLastError() CSUDISOCKGetLastError_PROXY(__FUNCTION__, __LINE__)
int CSUDISOCKGetLastError_PROXY(const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
