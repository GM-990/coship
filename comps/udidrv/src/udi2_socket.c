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
 @brief �����׽��֡�
 
 @param[in] nDomain ָ��Ҫ�������׽���Э����
 @param[in] nType ָ��Ҫ�������׽�������              
 @param[in] nProtocol ָ������Э�飬ͨ������Ϊ0
 @return �ɹ����ش������׽�����������ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�socket�ӿ�˵����
 */
int CSUDISOCKSocket(int nDomain,int nType,int nProtocol)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief ��ָ����������ַ���׽��ֽ��а󶨡�
 
 @param[in] nFd ��Ҫ�󶨵��׽���������
 @param[in] psAddr ���󶨵�������ַ�ṹ���ָ��
 @param[in] uAddrLen ��ַ�ṹ��CSUDISockAddr�ĳ���
 @return �ɹ�����0; ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�bind�ӿ�˵����
 */
int CSUDISOCKBind(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
} 

 /**
 @brief �����ӽ��м�����
 
 @param[in] nFd �׽���������
 @param[in] nNum ���ն��е���󳤶�
 @return �ɹ�����0; ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�listen�ӿ�˵����
 */
int CSUDISOCKListen(int nFd,int nNum)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}



 /**
 @brief ��ָ����ַ�������ӡ�
 
 @param[in] nFd �׽���������
 @param[in] psAddr ָ������ӵ�ַ�ṹ���ָ��
 @param[in] uAddrLen �ṹ��CSUDISockAddr_S �ĳ���
 @return �ɹ�����0 ; ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�connect�ӿ�˵����
 */
int CSUDISOCKConnect(int nFd,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
} 

  /**
 @brief ��ָ���׽����Ͻ���һ�����ӡ�
 
 @param[in] nFd ���ڼ���״̬���׽���������
 @param[out] psAddr ָ�򱣴����ӷ���ַ�Ľṹ��ָ��
 @param[out] puAddrLen �ṹ��CSUDISockAddr_S �ĳ���
 @return �ɹ��������׽�����������ʧ�ܷ���-1  ��
 @note ��ϸ�����μ�Linux�µ�accept�ӿ�˵����
 */
int CSUDISOCKAccept(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen)
{   
  CSUDI_Error_Code Retcode = CSUDI_SUCCESS;   
  UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
  return Retcode;
}


 /**
 @brief �������ݡ�
 
 @param[in] nFd ���Ͷ��׽���������
 @param[in] pvBuf ���������ݻ�����
 @param[in] uNum �����������ݳ���
 @param[in] nFlags ����ѡ�һ������Ϊ0
 @return �ɹ�����ʵ�ʷ��͵����ݳ���; ʧ�ܷ���-1  ��
 @note ��ϸ�����μ�Linux�µ�send�ӿ�˵����
 */
int CSUDISOCKSend(int nFd,const void * pvBuf,unsigned int uNum,int nFlags)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief �������ݡ�
 
 @param[in] nFd ���ն��׽���������
 @param[out] pvBuf ���ڽ������ݵĻ�����
 @param[in] uNum ����������
 @param[in] nFlags ����ѡ�һ����Ϊ0
 @return �ɹ�����ʵ�ʽ��յ��ֽ���; ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�recv�ӿ�˵����
 */
int CSUDISOCKRecv(int nFd,void * pvBuf,unsigned int uNum,int nFlags)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief �������ݵ�ָ��Ŀ�ĵ�ַ��
 
 @param[in] nFd �׽���������
 @param[in] pvBuf ���������ݵĻ�����
 @param[in] uNum �����������ݵĳ���
 @param[in] nFlags ����ѡ�һ��Ϊ0
 @param[in] psAddr ָ��CSUDISockAddr_S�ṹ���ָ��
 @param[in] uAddrLen CSUDISockAddr_S �ĳ���				                                        
 @return �ɹ�����ʵ�ʷ��ͳ�ȥ�������ֽ���;ʧ�ܷ���-1  ��
 @note ��ϸ�����μ�Linux�µ�sendto�ӿ�˵����
 */
int CSUDISOCKSendTo(int nFd,const void * pvBuf,unsigned int uNum,int nFlags,const CSUDISockAddr_S * psAddr,unsigned int uAddrLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}



 /**
 @brief ��ָ��Դ��ַ�������ݡ�
 
 @param[in] nFd �׽���������
 @param[out] pvBuf ���ڽ������ݵĻ�����
 @param[in] uNum ����������
 @param[in] nFlags ����ѡ�һ����Ϊ0
 @param[out] psAddr ָ��CSUDISockAddr_S �ṹ���ָ��
 @param[in] puAddrLen CSUDISockAddr_S �ĳ��ȣ��˲�������Ϊ�������
 @return �ɹ����ض�ȡ���ֽ���;ʧ�ܷ���-1 ��
 @note ��ϸ�����μ�Linux�µ�recvfrom�ӿ�˵����
 */
int CSUDISOCKRecvFrom(int nFd,void * pvBuf,unsigned int uNum, int nFlags,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}



 /**
 @brief ȷ��һ�������׽��ֵ�״̬���ж��׽������Ƿ������ݿɶ����ܷ����׽���д�����ݣ��Ƿ������������
 
 @param[in] nFds ��Ҫ���������׽�������һ
 @param[in] psReadnFds �������ɶ��Ե�һ���׽���������
 @param[in] psWritenFds ��������д�Ե�һ���׽���������
 @param[in] psExceptnFds �����������״̬���׽����ļ���������������������״̬
 @param[in] psTimeout �ȴ�ʱ��
 @return �ɹ�����׼���������׽���������ʱ����0��ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�select�ӿ�˵����
 */
int CSUDISOCKSelect(int nFds,CSUDIFdSet_S * psReadnFds,CSUDIFdSet_S * psWritenFds,CSUDIFdSet_S * psExceptnFds,CSUDITimeval_S * psTimeout)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief �����׽����������������׽��ֵ�����, �ο�BSD Socket ��׼���塣
 
 @param[in] nFd ������nCmd ������������
 @param[in] nCmd ��������ɹ�ʹ�õĲ�������μ���׼Linux���塣
 @param[in] nArg1 ����1
 @param[in] nArg2 ����2
 @return �ɹ����ڲ�ͬ�Ĳ��������в�ͬ�ķ���ֵ��ʧ�ܷ���-1��
 */
int CSUDISOCKFcntl(int nFd,int nCmd,int nArg1,int nArg2)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief �豸���������ж��豸��I/Oͨ�����й����ο�BSD Socket ��׼���塣
 
 @param[in] nFd �׽���������
 @param[in] ulRequest �û��Գ���Ŀ�������ɹ�ʹ�õĲ�������μ���׼Linux���塣
 @param[in] pArg1 ����1
 @param[in] pArg2 ����2
 @return �ɹ����ڲ�ͬ�Ĳ��������в�ͬ�ķ���ֵ��ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�ioctl�ӿ�˵����
 */
int CSUDISOCKIoctl(int nFd,unsigned int ulRequest,void * pArg1,void * pArg2)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief ��ȡ�׽���ѡ�
 
 @param[in] nFd �׽���������
 @param[in] nLevel  ѡ���Ĳ�Σ���ȡֵ�� CSUDI_SOL_SOCKET��CSUDI_SOL_TCP��CSUDI_SOL_UDP�� CSUDI_SOL_IP
 @param[in] nOptName Ҫ��ȡ��ѡ����
 @param[out] pvOptVal ָ�򱣴�ѡ��ֵ�Ļ�����
 @param[in] puOptLen ����ѡ��ֵ�������ĳ��ȣ��˲�������Ϊ�������
 @return �ɹ�����0��ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�getsockopt�ӿ�˵����
 */
int CSUDISOCKGetSockOpt(int nFd,int nLevel,int nOptName,void * pvOptVal,unsigned int * puOptLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

 /**
 @brief �����׽���ѡ�
 
 @param[in] nFd �׽���������
 @param[in] nLevel  ѡ���Ĳ�Σ���ȡֵ�� CSUDI_SOL_SOCKET��CSUDI_SOL_TCP��CSUDI_SOL_UDP�� CSUDI_SOL_IP
 @param[in] nOptName ��Ҫ���õ�ѡ����
 @param[in] pvOptVal ָ�򱣴�ѡ��ֵ�Ļ�����
 @param[in] uOptLen  ����ѡ��ֵ�������ĳ���
 @return �ɹ�����0��ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�setsockopt�ӿ�˵����
 */
 int CSUDISOCKSetSockOpt(int nFd,int nLevel,int nOptName,const void * pvOptVal, unsigned int uOptLen)
 {	 
	 CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	 UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	 return Retcode;
 }


 /**
 @brief ����Э�����ƻ�ȡЭ���������Ϣ��
 
 @param[in] pcProtoName Э������
 @return �ɹ�����Э��CSUDIProtoent_S�ṹ����Ϣ��ʧ�ܷ���CSUDI_NULL��
 @note ��ϸ�����μ�Linux�µ�getprotobyname�ӿ�˵����
 */
CSUDIProtoent_S * CSUDISOCKGetProtoByName(const char * pcProtoName)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

 
 /**
 @brief ���ݷ�������Э������ȡ������Ϣ��
 
 @param[in] pcServName ��������
 @param[in] pcProtoName ����ʹ�õ�Э��
 @return �ɹ����ط���CSUDIServent_S �ṹ����Ϣ��ʧ�ܷ���CSUDI_NULL��
 @note ��ϸ�����μ�Linux�µ�getservbyname�ӿ�˵����
 */
CSUDIServent_S * CSUDISOCKGetServByName(const char * pcServName,const char * pcProtoName)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief ��ȡ���׽�����ص�Զ�˵�ַ��

 @param[in] nFd ��ʶ�����ӵ��׽���������
 @param[out] psAddr �ṹ��CSUDISockAddr_S ��ָ�룬�������ӵ���һ������Ϣ
 @param[in] puAddrLen ָ��sizeof(CSUDISockAddr_S)��ָ�룬��ָ������Ϊ�������
 @return �ɹ�����0 ,ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�getpeername�ӿ�˵����
 */
int CSUDISOCKGetPeerName(int nFd,CSUDISockAddr_S * psAddr,unsigned int * puAddrLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief ��ȡ���׽�����صı��ص�ַ��
 
 @param[in] nFd �׽���������
 @param[out] psAddr ָ������׽��ֵ�ַ
 @param[in] puAddrLen ��Ϊ�������ʱ��Ϊ�ռ����󳤶ȡ���Ϊ�������ʱ��Ϊ��������ʵ�ʴ�С�����ֽ�Ϊ��λ��
 @return �ɹ�����0,ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�getsockname�ӿ�˵����
 */
int CSUDISOCKGetSockName(int nFd,CSUDISockAddr_S * psAddr, unsigned int * puAddrLen)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
 @brief ��ȡ������������������

 @param[out] pcHostName ���ձ����������Ļ�����
 @param[in] uAddrLen �������ĳ���
 @return �ɹ�����0��ʧ�ܷ��ش����롣
 @note ��ϸ�����μ�Linux�µ�gethostname�ӿ�˵����
 */
 int CSUDISOCKGetHostName(char * pcHostName,unsigned int uAddrLen)
 {	  
	 CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	 UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	 return Retcode;
 }


 /**
 @brief ������������ȡ������������Ϣ��
 
 @param[in] pcHostName ������
 @return �ɹ����ػ�ȡ������CSUDIHostent_S�ṹ����Ϣ��ʧ�ܷ���CSUDI_NULL
 @note ��ϸ�����μ�Linux�µ�gethostbyname�ӿ�˵����
 @note �ýӿ��ѷ������м������ʹ��
 */
 CSUDIHostent_S * CSUDISOCKGetHostByName(const char * pcHostName)
 {	  
	 CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	 UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	 return Retcode;
 }


/**
 @brief ������������ȡ������������Ϣ��
 
 @param[in] pcHostName ������
 @param[in] psResultBuf ָ��һ�������ṹ
 @param[in] pcBuf ָ��һ�����棬��ָ������Ϊ���
 @param[in] uBufLen ��������С���˲�������Ϊ���
 @param[out] psResult �����ɹ����ú�ָ���ȡ�������ṹCSUDIHostent_S
 @param[out] pnErrCode ������
 @return �ɹ�����0��ʧ�ܷ��ش���š�
 @note ��ϸ�����μ�Linux�µ�gethostbyname_r�ӿ�˵����
 @note �ýӿ��ѷ������м������ʹ��
 */
int CSUDISOCKGetHostByName_r(const char * pcHostName,CSUDIHostent_S * psResultBuf,char * pcBuf,unsigned int uBufLen,CSUDIHostent_S ** psResult,int * pnErrCode)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief ���ݸ����ĵ�ַ��ȡ������������Ϣ��
 
 @param[in] pvAddr ������ַ(IP��ַ�����ֽ���)
 @param[in] uAddrLen ��ַ�ĳ���
 @param[in] nType ��ַ����
 @return �ɹ����ػ�ȡ������CSUDIHostent_S �ṹ����Ϣ��ʧ�ܷ���CSUDI_NULL
 @note ��ϸ�����μ�Linux�µ�gethostbyaddr�ӿ�˵����
 */
CSUDIHostent_S * CSUDISOCKGetHostByAddr(const void * pvAddr,unsigned int uAddrLen,int nType)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


  /**
 @brief �ѵ��IP��ַת��Ϊ�����ֽ���������ַ��
 @param[in] pcIp ��Ҫת���ı�׼���IP��ַ�ַ���,����"192.168.0.1"
 @note ��IP��255.255.255.255ʱ������CSUDIInet_addr()����Ϊ���Ǹ���Ч��IP��ַ
 @return �ɹ������޷���������ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�inet_addr�ӿ�˵����
 */
CSUDIInAddr_t CSUDISOCKInet_addr(const char * pcIp)
{    
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;    
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode); 	
	return Retcode;
}


 /**
 @brief �ѵ��IP��ַת��Ϊ�����ֽ���������ַ��
 
 @param[in] pcIp ��Ҫת���ĵ��IP��ַ
 @param[out] psIpNet IP��ַ�ṹ��
 @return �ɹ����ط�0��ʧ�ܷ���0��
 @note ��ϸ�����μ�Linux�µ�inet_aton�ӿ�˵����
 */
CSUDIInAddr_t CSUDISOCKInet_aton(const char * pcIp, CSUDIInAddr_S * psIpNet)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}

 
 /**
 @brief �������ֽ����ַת��Ϊ���IP��ַ��
 
 @param[in] sIpNet �����ֽ����ַ
 @return ת����ĵ��IP ��ַ�ַ�����ʧ�ܷ���CSUDI_NULL��
 @note ��ϸ�����μ�Linux�µ�inet_ntoa�ӿ�˵����
 */
char * CSUDISOCKInet_ntoa(CSUDIInAddr_S sIpNet)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 
 /**
 @brief ���ַ���ת���������ַ��
 
 @param[in] nAf ��ַ��
 @param[in] pSrc �ַ��͵�ַ
 @param[out] pDst ��ַת��Ϊin_addr�Ľṹ�壬��������pDst��
 @return �ɹ�����1��ʧ�ܷ���0
 @note ��ϸ�����μ�Linux�µ�inet_pton�ӿ�˵����
 */
int CSUDISOCKInet_pton(int nAf,const char *pSrc, void *pDst)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


  
 /**
 @brief ����������ƽṹ��ַת�����ַ�����ַ��
 
 @param[in] nAf ��ַ��
 @param[in] pSrc �����ַ
 @param[out] pDst �ַ��͵�ַ
 @param[in] nDsrLen pDst ������
 @return �ɹ�����ת������ַ�����ַ��ʧ�ܷ���CSUDI_NULL
 @note ��ϸ�����μ�Linux�µ�inet_ntop�ӿ�˵����
 */
const char * CSUDISOCKInet_ntop(int nAf,const void *pSrc,char *pDst,int nDsrLen)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}



 /**
 @brief ��short�͵������ֽ���ת��Ϊ�����ֽ���
 
 @param[in] ushNet �����ֽ����16bit����
 @return �����ֽ���ֵ��
 @note ��ϸ�����μ�Linux�µ�ntohs�ӿ�˵����
 */
unsigned short CSUDISOCKNtohs(unsigned short ushNet)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief ��short�͵������ֽ���ת��Ϊ�����ֽ���
 
 @param[in] ushHost �����ֽ����16bit����
 @return �����ֽ���ֵ
 @note ��ϸ�����μ�Linux�µ�htons�ӿ�˵����
 */
unsigned short CSUDISOCKHtons(unsigned short ushHost)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief ��32λ�������ֽ���ת��Ϊ�����ֽ���
 
 @param[in] uNet �����ֽ����32bit����
 @return �����ֽ���ֵ
 @note ��ϸ�����μ�Linux�µ�ntohl�ӿ�˵����
 */
unsigned int CSUDISOCKNtohl(unsigned int uNet)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

 /**
 @brief ��32λ�������ֽ���ת��Ϊ�����ֽ���
 
 @param[in] uHost �����ֽ����32bit����
 @return �����ֽ���ֵ
 @note ��ϸ�����μ�Linux�µ�htonl�ӿ�˵����
 */
unsigned int CSUDISOCKHtonl(unsigned int uHost)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief  ��ָ���׽����Ͻ�ֹ���ݵĽ��ա����ͻ�ͬʱ��ֹ���ݽ��պͷ��͡�
 
 @param[in] nFd ��Ҫ�رյ��׽���������
 @param[in] nHow �رշ�ʽ 
 @return �ɹ�����0��ʧ�ܷ���-1��
 @note nHow=0:��ֹ���ݽ���
 @note nHow=1:��ֹ���ݷ���
 @note nHow=2:ͬʱ��ֹ���ݷ��ͺͽ���
 */
int CSUDISOCKShutDown(int nFd,int nHow)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**
 @brief �ر�ָ�����׽��֡�
 
 @param[in] nFd �׽���������
 @return �ɹ�����0��ʧ�ܷ���-1��
 @note ��ϸ�����μ�Linux�µ�close�ӿ�˵����
 */
int CSUDISOCKClose(int nFd)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


 /**@brief ��ȡ����š�

 @return ���ش����,���udi2_sockerror.hͷ�ļ����塣
 */
int CSUDISOCKGetLastError(void)
{	  
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	  
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	   
	return Retcode;
}


