/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_socket.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_socket.h"
 /**
 @brief 创建套接字。
 
 @param[in] nDomain 指定要创建的套接字协议族
 @param[in] nType 指定要创建的套接字类型              
 @param[in] nProtocol 指定哪种协议，通常设置为0
 @return 成功返回创建的套接字描述符；失败返回-1。
 @note 详细描述参见Linux下的socket接口说明。
 */
int CSUDISOCKSocket(int nDomain,int nType,int nProtocol)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief 将指定的主机地址与套接字进行绑定。
 
 @param[in] nFd 需要绑定的套接字描述符
 @param[in] psAddr 待绑定的主机地址结构体的指针
 @param[in] uAddrLen 地址结构体CSUDISockAddr的长度
 @return 成功返回0; 失败返回-1。
 @note 详细描述参见Linux下的bind接口说明。
 */
int CSUDISOCKBind(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
} 

 /**
 @brief 对连接进行监听。
 
 @param[in] nFd 套接字描述符
 @param[in] nNum 接收队列的最大长度
 @return 成功返回0; 失败返回-1。
 @note 详细描述参见Linux下的listen接口说明。
 */
int CSUDISOCKListen(int nFd,int nNum)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}



 /**
 @brief 向指定地址发起连接。
 
 @param[in] nFd 套接字描述符
 @param[in] psAddr 指向待连接地址结构体的指针
 @param[in] uAddrLen 结构体CSUDISockAddr_S 的长度
 @return 成功返回0 ; 失败返回-1。
 @note 详细描述参见Linux下的connect接口说明。
 */
int CSUDISOCKConnect(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
} 

  /**
 @brief 在指定套接字上接收一个连接。
 
 @param[in] nFd 处于监听状态的套接字描述符
 @param[out] psAddr 指向保存连接方地址的结构体指针
 @param[out] puAddrLen 结构体CSUDISockAddr_S 的长度
 @return 成功返回新套接字描述符；失败返回-1  。
 @note 详细描述参见Linux下的accept接口说明。
 */
int CSUDISOCKAccept(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen)
{   
  CSUDI_Error_Code Retcode = CSUDI_SUCCESS;   
  UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
  return Retcode;
}


 /**
 @brief 发送数据。
 
 @param[in] nFd 发送端套接字描述符
 @param[in] pvBuf 待发送数据缓存区
 @param[in] uNum 缓存区中数据长度
 @param[in] nFlags 发送选项，一般设置为0
 @return 成功返回实际发送的数据长度; 失败返回-1  。
 @note 详细描述参见Linux下的send接口说明。
 */
int CSUDISOCKSend(int nFd,const void * pvBuf,unsigned int uNum,int nFlags)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief 接收数据。
 
 @param[in] nFd 接收端套接字描述符
 @param[out] pvBuf 用于接收数据的缓存区
 @param[in] uNum 缓存区长度
 @param[in] nFlags 操作选项，一般置为0
 @return 成功返回实际接收的字节数; 失败返回-1。
 @note 详细描述参见Linux下的recv接口说明。
 */
int CSUDISOCKRecv(int nFd,void * pvBuf,unsigned int uNum,int nFlags)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief 发送数据到指定目的地址。
 
 @param[in] nFd 套接字描述符
 @param[in] pvBuf 待发送数据的缓存区
 @param[in] uNum 缓存区中数据的长度
 @param[in] nFlags 操作选项，一般为0
 @param[in] psAddr 指向CSUDISockAddr_S结构体的指针
 @param[in] uAddrLen CSUDISockAddr_S 的长度				                                        
 @return 成功返回实际发送出去的数据字节数;失败返回-1  。
 @note 详细描述参见Linux下的sendto接口说明。
 */
int CSUDISOCKSendTo(int nFd,const void * pvBuf,unsigned int uNum,int nFlags,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}



 /**
 @brief 从指定源地址接收数据。
 
 @param[in] nFd 套接字描述符
 @param[out] pvBuf 用于接收数据的缓存区
 @param[in] uNum 缓存区长度
 @param[in] nFlags 操作选项，一般置为0
 @param[out] psAddr 指向CSUDISockAddr_S 结构体的指针
 @param[in] puAddrLen CSUDISockAddr_S 的长度，此参数亦作为输出参数
 @return 成功返回读取的字节数;失败返回-1 。
 @note 详细描述参见Linux下的recvfrom接口说明。
 */
int CSUDISOCKRecvFrom(int nFd,void * pvBuf,unsigned int uNum, int nFlags,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}



 /**
 @brief 确定一个或多个套接字的状态，判断套接字上是否有数据可读，能否向套接字写入数据，是否有意外情况。
 
 @param[in] nFds 需要管理的最大套接字数加一
 @param[in] psReadnFds 用来检查可读性的一组套接字描述符
 @param[in] psWritenFds 用来检查可写性的一组套接字描述符
 @param[in] psExceptnFds 用来检查意外状态的套接字文件描述符，错误不属于意外状态
 @param[in] psTimeout 等待时间
 @return 成功返回准备就绪的套接字数；超时返回0；失败返回错误码。
 @note 详细描述参见Linux下的select接口说明。
 */
int CSUDISOCKSelect(int nFds,CSUDIFdSet_S * psReadnFds,CSUDIFdSet_S * psWritenFds,CSUDIFdSet_S * psExceptnFds,CSUDITimeval_S * psTimeout)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief 根据套接字描述符来操作套接字的属性, 参考BSD Socket 标准定义。
 
 @param[in] nFd 被参数nCmd 操作的描述符
 @param[in] nCmd 操作命令，可供使用的操作命令参见标准Linux定义。
 @param[in] nArg1 参数1
 @param[in] nArg2 参数2
 @return 成功对于不同的操作命令有不同的返回值；失败返回-1。
 */
int CSUDISOCKFcntl(int nFd,int nCmd,int nArg1,int nArg2)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief 设备驱动程序中对设备的I/O通道进行管理，参考BSD Socket 标准定义。
 
 @param[in] nFd 套接字描述符
 @param[in] ulRequest 用户对程序的控制命令，可供使用的操作命令参见标准Linux定义。
 @param[in] pArg1 参数1
 @param[in] pArg2 参数2
 @return 成功对于不同的操作命令有不同的返回值；失败返回-1。
 @note 详细描述参见Linux下的ioctl接口说明。
 */
int CSUDISOCKIoctl(int nFd,unsigned int ulRequest,void * pArg1,void * pArg2)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief 获取套接字选项。
 
 @param[in] nFd 套接字描述符
 @param[in] nLevel  选项定义的层次，可取值有 CSUDI_SOL_SOCKET，CSUDI_SOL_TCP，CSUDI_SOL_UDP， CSUDI_SOL_IP
 @param[in] nOptName 要获取的选项名
 @param[out] pvOptVal 指向保存选项值的缓存区
 @param[in] puOptLen 保存选项值缓存区的长度，此参数亦作为输出参数
 @return 成功返回0；失败返回错误码。
 @note 详细描述参见Linux下的getsockopt接口说明。
 */
int CSUDISOCKGetSockOpt(int nFd,int nLevel,int nOptName,void * pvOptVal,unsigned int * puOptLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

 /**
 @brief 设置套接字选项。
 
 @param[in] nFd 套接字描述符
 @param[in] nLevel  选项定义的层次，可取值有 CSUDI_SOL_SOCKET，CSUDI_SOL_TCP，CSUDI_SOL_UDP， CSUDI_SOL_IP
 @param[in] nOptName 需要设置的选项名
 @param[in] pvOptVal 指向保存选项值的缓存区
 @param[in] uOptLen  保存选项值缓存区的长度
 @return 成功返回0；失败返回错误码。
 @note 详细描述参见Linux下的setsockopt接口说明。
 */
 int CSUDISOCKSetSockOpt(int nFd,int nLevel,int nOptName,const void * pvOptVal, unsigned int uOptLen)
 {	 
	 CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	 UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	 return Retcode;
 }


 /**
 @brief 根据协议名称获取协议的其它信息。
 
 @param[in] pcProtoName 协议名称
 @return 成功返回协议CSUDIProtoent_S结构体信息；失败返回CSUDI_NULL。
 @note 详细描述参见Linux下的getprotobyname接口说明。
 */
CSUDIProtoent_S * CSUDISOCKGetProtoByName(const char * pcProtoName)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

 
 /**
 @brief 根据服务名和协议名获取服务信息。
 
 @param[in] pcServName 服务名称
 @param[in] pcProtoName 服务使用的协议
 @return 成功返回服务CSUDIServent_S 结构体信息；失败返回CSUDI_NULL。
 @note 详细描述参见Linux下的getservbyname接口说明。
 */
CSUDIServent_S * CSUDISOCKGetServByName(const char * pcServName,const char * pcProtoName)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief 获取与套接字相关的远端地址。

 @param[in] nFd 标识已连接的套接字描述符
 @param[out] psAddr 结构体CSUDISockAddr_S 的指针，保存连接的另一方的信息
 @param[in] puAddrLen 指向sizeof(CSUDISockAddr_S)的指针，此指针亦作为输出参数
 @return 成功返回0 ,失败返回错误码。
 @note 详细描述参见Linux下的getpeername接口说明。
 */
int CSUDISOCKGetPeerName(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief 获取与套接字相关的本地地址。
 
 @param[in] nFd 套接字描述符
 @param[out] psAddr 指向接收套接字地址
 @param[in] puAddrLen 作为输入参数时，为空间的最大长度。作为输出参数时，为缓存区的实际大小，以字节为单位。
 @return 成功返回0,失败返回错误码。
 @note 详细描述参见Linux下的getsockname接口说明。
 */
int CSUDISOCKGetSockName(int nFd,CSUDISockAddr_S * psAddr, unsigned int * puAddrLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief 获取本地主机的主机名。

 @param[out] pcHostName 接收本地主机名的缓存区
 @param[in] uAddrLen 缓存区的长度
 @return 成功返回0；失败返回错误码。
 @note 详细描述参见Linux下的gethostname接口说明。
 */
 int CSUDISOCKGetHostName(char * pcHostName,unsigned int uAddrLen)
 {	  
	 CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	 UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	 return Retcode;
 }


 /**
 @brief 根据主机名获取主机的其它信息。
 
 @param[in] pcHostName 主机名
 @return 成功返回获取的主机CSUDIHostent_S结构体信息；失败返回CSUDI_NULL
 @note 详细描述参见Linux下的gethostbyname接口说明。
 @note 该接口已废弃，中间件不再使用
 */
 CSUDIHostent_S * CSUDISOCKGetHostByName(const char * pcHostName)
 {	  
	 CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	 UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	 return Retcode;
 }


/**
 @brief 根据主机名获取主机的其它信息。
 
 @param[in] pcHostName 主机名
 @param[in] psResultBuf 指向一个主机结构
 @param[in] pcBuf 指向一个缓存，此指针亦作为输出
 @param[in] uBufLen 缓存区大小，此参数亦作为输出
 @param[out] psResult 函数成功调用后指向获取的主机结构CSUDIHostent_S
 @param[out] pnErrCode 错误码
 @return 成功返回0；失败返回错误号。
 @note 详细描述参见Linux下的gethostbyname_r接口说明。
 @note 该接口已废弃，中间件不再使用
 */
int CSUDISOCKGetHostByName_r(const char * pcHostName,CSUDIHostent_S * psResultBuf,char * pcBuf,unsigned int uBufLen,CSUDIHostent_S ** psResult,int * pnErrCode)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief 根据给定的地址获取主机的其它信息。
 
 @param[in] pvAddr 主机地址(IP地址网络字节序)
 @param[in] uAddrLen 地址的长度
 @param[in] nType 地址类型
 @return 成功返回获取的主机CSUDIHostent_S 结构体信息；失败返回CSUDI_NULL
 @note 详细描述参见Linux下的gethostbyaddr接口说明。
 */
CSUDIHostent_S * CSUDISOCKGetHostByAddr(const void * pvAddr,unsigned int uAddrLen,int nType)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


  /**
 @brief 把点分IP地址转换为网络字节序的网络地址。
 @param[in] pcIp 需要转化的标准点分IP地址字符串,例如"192.168.0.1"
 @note 当IP是255.255.255.255时，函数CSUDIInet_addr()会认为这是个无效的IP地址
 @return 成功返回无符号整数；失败返回-1。
 @note 详细描述参见Linux下的inet_addr接口说明。
 */
CSUDIInAddr_t CSUDISOCKInet_addr(const char * pcIp)
{    
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;    
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode); 	
	return Retcode;
}


 /**
 @brief 把点分IP地址转换为网络字节序的网络地址。
 
 @param[in] pcIp 需要转化的点分IP地址
 @param[out] psIpNet IP地址结构体
 @return 成功返回非0；失败返回0。
 @note 详细描述参见Linux下的inet_aton接口说明。
 */
CSUDIInAddr_t CSUDISOCKInet_aton(const char * pcIp, CSUDIInAddr_S * psIpNet)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}

 
 /**
 @brief 把网络字节序地址转换为点分IP地址。
 
 @param[in] sIpNet 网络字节序地址
 @return 转换后的点分IP 地址字符串；失败返回CSUDI_NULL。
 @note 详细描述参见Linux下的inet_ntoa接口说明。
 */
char * CSUDISOCKInet_ntoa(CSUDIInAddr_S sIpNet)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 
 /**
 @brief 把字符串转换成网络地址。
 
 @param[in] nAf 地址族
 @param[in] pSrc 字符型地址
 @param[out] pDst 地址转换为in_addr的结构体，并复制在pDst中
 @return 成功返回1；失败返回0
 @note 详细描述参见Linux下的inet_pton接口说明。
 */
int CSUDISOCKInet_pton(int nAf,const char *pSrc, void *pDst)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


  
 /**
 @brief 把网络二进制结构地址转换成字符串地址。
 
 @param[in] nAf 地址族
 @param[in] pSrc 网络地址
 @param[out] pDst 字符型地址
 @param[in] nDsrLen pDst 缓存区
 @return 成功返回转换后的字符串地址；失败返回CSUDI_NULL
 @note 详细描述参见Linux下的inet_ntop接口说明。
 */
const char * CSUDISOCKInet_ntop(int nAf,const void *pSrc,char *pDst,int nDsrLen)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}



 /**
 @brief 把short型的网络字节序转化为主机字节序。
 
 @param[in] ushNet 网络字节序的16bit数据
 @return 主机字节序值。
 @note 详细描述参见Linux下的ntohs接口说明。
 */
unsigned short CSUDISOCKNtohs(unsigned short ushNet)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief 把short型的主机字节序转换为网络字节序。
 
 @param[in] ushHost 主机字节序的16bit数据
 @return 网络字节序值
 @note 详细描述参见Linux下的htons接口说明。
 */
unsigned short CSUDISOCKHtons(unsigned short ushHost)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief 把32位的网络字节序转化为主机字节序。
 
 @param[in] uNet 网络字节序的32bit数据
 @return 主机字节序值
 @note 详细描述参见Linux下的ntohl接口说明。
 */
unsigned int CSUDISOCKNtohl(unsigned int uNet)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

 /**
 @brief 把32位的主机字节序转换为网络字节序。
 
 @param[in] uHost 主机字节序的32bit数据
 @return 网络字节序值
 @note 详细描述参见Linux下的htonl接口说明。
 */
unsigned int CSUDISOCKHtonl(unsigned int uHost)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief  在指定套接字上禁止数据的接收、发送或同时禁止数据接收和发送。
 
 @param[in] nFd 需要关闭的套接字描述符
 @param[in] nHow 关闭方式 
 @return 成功返回0；失败返回-1。
 @note nHow=0:禁止数据接收
 @note nHow=1:禁止数据发送
 @note nHow=2:同时禁止数据发送和接收
 */
int CSUDISOCKShutDown(int nFd,int nHow)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief 关闭指定的套接字。
 
 @param[in] nFd 套接字描述符
 @return 成功返回0；失败返回-1。
 @note 详细描述参见Linux下的close接口说明。
 */
int CSUDISOCKClose(int nFd)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**@brief 获取错误号。

 @return 返回错误号,详见udi2_sockerror.h头文件定义。
 */
int CSUDISOCKGetLastError(void)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


