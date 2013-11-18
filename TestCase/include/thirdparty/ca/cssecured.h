/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    cssecured.h  
* Author:      heyingbo
* Date:        Nov 2008
* Description:  ST SecuredChipset driver
*              
*            
*****************************************************************************************************************************/

/****************************************************************************************************************************
*                           Revision History                                *
*****************************************************************************************************************************
* - Revision 1.0  2008/11/11
*   Programmer:heyingbo
*   Create.
*****************************************************************************************************************************/
#ifndef _CSSECURED_H_
#define _CSSECURED_H_

#include "udiplus_os.h"
#include "udiplus_typedef.h"
#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

//高级安全类型
typedef enum _SecuredType
{
    SECURED_NACHIPSET_NASECURED,    //非高级安全芯片，不带高级安全功能
    SECURED_CHIPSET_SECURED,        //高级安全芯片，带高级安全功能
    SECURED_CHIPSET_NASECURED   	//高级安全芯片，不带高级安全功能    
}SecuredType_EM;

typedef enum _Algorithm
{
    ALGORITHM_TEDS  
}Algorithm_E;

typedef enum _SCWCaType
{
    EM_SCW_UNKNOWN_CA = 0,
    EM_SCW_CONAX_CA,  
    EM_SCW_VIACCESS_CA , 
    EM_SCW_TONGFANG_CA ,
    EM_SCW_DVN_CA , 
    EM_SCW_CRYPTOWORKS_CA , 
    EM_SCW_MEDIAGUARD_CA , 
    EM_SCW_SUMAVISION_CA ,
    EM_SCW_ST_CA,
    EM_SCW_NAGRAVISON_CA , 
    EM_SCW_IRDETO_CA 
}E_SCW_CaType;

/*高级安全接口的IOCtl类型*/
typedef enum _SecIOCtlType
{
	EM_SEC_SET_BOOTROM = 0,  /*打开安全启动(签名校验).不同的CA类型打开的熔丝可能不同,详见wiki里的CA开发下的高级安全.非指定的熔丝,请调用EM_SEC_SET_BY_APP实现*/
	EM_SEC_GET_BOOTROM,	     /*获取安全启动熔丝状态.不同的CA类型打开的熔丝可能不同,详见wiki里的CA开发下的高级安全.非指定的熔丝,请调用EM_SEC_GET_BY_APP实现*/
	EM_SEC_SET_JTAGKEY,      /*keyed模式下,关闭JTAG(关闭成功后,可以向CA厂商申请密码进行解锁).不同的CA类型打开的熔丝可能不同,详见wiki里的CA开发下的高级安全.非指定的熔丝,请调用EM_SEC_SET_BY_APP实现*/
	EM_SEC_GET_JTAGKEY,      /*keyed模式下,关闭JTAG对应的熔丝状态.不同的CA类型打开的熔丝可能不同,详见wiki里的CA开发下的高级安全.非指定的熔丝,请调用EM_SEC_GET_BY_APP实现*/
	EM_SEC_SET_JTAGLOCK,     /*永久性关闭JTAG(关闭成功后JTAG不可用).不同的CA类型打开的熔丝可能不同,详见wiki里的CA开发下的高级安全.非指定的熔丝,请调用EM_SEC_SET_BY_APP实现*/
	EM_SEC_GET_JTAGLOCK,     /*获取永久性关闭JTAG对应的熔丝状态.不同的CA类型打开的熔丝可能不同,详见wiki里的CA开发下的高级安全.非指定的熔丝,请调用EM_SEC_GET_BY_APP实现*/
	EM_SEC_SET_SCW,          /*熔断密文控制字熔丝(熔丝熔断后只支持密文控制字).不同的CA类型打开的熔丝可能不同,详见wiki里的CA开发下的高级安全.非指定的熔丝,请调用EM_SEC_SET_BY_APP实现*/
	EM_SEC_GET_SCW,	         /*获取密文控制字熔丝状态.不同的CA类型打开的熔丝可能不同,详见wiki里的CA开发下的高级安全.非指定的熔丝,请调用EM_SEC_GET_BY_APP实现*/
	EM_SEC_SET_BY_APP,       /*应用用户熔断指定的熔丝,调用方式参考wiki里的CA开发下的高级安全*/
	EM_SEC_GET_BY_APP,       /*应用用户获取指定熔丝的状态,调用方式参考wiki里的CA开发下的高级安全*/
	EM_SEC_SET_MARKETID,     /*Mstar平台设置MarketID*/
	EM_SEC_GET_MARKETID      /*Mstar平台获取MarketID*/
}E_SecIOCtlType;

#define SEC_MAX_DATA_SIZE	32
#define SEC_MAX_CONFIG_NUM	8

typedef struct _SECSetParam
{
	BOOL 	m_bBulkData;					//类型，是否是大块数据，默认为FALSE
	INT  	m_nItem;						//熔丝位，对应于STSECTOOLFUSE_ITEM_t, 具体数值请咨询相应CA负责人
	DWORD 	m_dwLocation;					//起始地址，m_bBulkData为TRUE时有效，默认为FALSE
	DWORD	m_dwSize;						//数据大小，m_bBulkData为TRUE时为整个数组大小，m_bBulkData为FALSE时，值为1
	BYTE	m_aucData[SEC_MAX_DATA_SIZE];	//数据，m_bBulkData为FALSE时，数组只有一个数据
}SECSetParam_S;

typedef struct _SECConfigParam
{
	INT 			m_nItemCount;           //需要熔断熔丝的个数，最大值为SEC_MAX_CONFIG_NUM					
	SECSetParam_S	m_stSetParam[SEC_MAX_CONFIG_NUM];
}SECConfigParam_S;

#ifndef SUCCESS
#define SUCCESS (0)
#endif

#ifndef FAILURE
#define FAILURE (-1)
#endif

typedef struct _SECDsmInfo
{
	DWORD 	m_dwDescrambleID;  	   //dsmid
	DWORD   m_dwKeyType     : 1;   //0:evenkey    1:oddkey
	DWORD 	m_dwNotSumCheck : 1;   //是否进行和校验, 为0时会进行和校验, 为1时不进行和校验
	DWORD   m_dwReserved    : 30;  //预留
}SECDsmInfo_S;


/**************************************************************************
* Function Name: CSCASCWInit
* 初始化高级安全驱动
* Parameters :      无
* Return Values:    成功SUCCESS
*                   失败FAILURE
**************************************************************************/
INT CSCASCWInit(void);

/**************************************************************************
* Function Name: CSCASCWGetSecuredType
* 获取芯片类型以及芯片高级功能是否打开
* Parameters :      pemSecuredType:读取到的信息
* Return Values:    成功SUCCESS
*                   失败FAILURE
**************************************************************************/
INT CSCASCWGetSecuredType(SecuredType_EM* pemSecuredType);

/**************************************************************************
* Function Name: CSCAGetChipId
* 获取芯片ID
* Parameters :      无
* Return Values:    返回chipid值
 *                  失败FAILURE
**************************************************************************/
DWORD CSCASCWGetChipId(void);

/**************************************************************************
* Function Name: CSCAProcessCW
* 对明文／密文控制字处理
* 首先需要根据参数地址dwPara解析出SEDsmInfo_S具体内容
* 1 对于ST平台的处理:-->如果是对明文控制字处理, 成功返回0, 失败返回FAILURE
                        根据解析出的SEDsmInfo_S结构中的m_dwNotSumCheck判断是否和校验，然后拷贝输出，
					 -->如果是对密文控制字处理，成功返回0, 失败返回FAILURE
*                       密文控制字需要输出处理后的CW(寄存器index)
* 2 对于ALI、MSTAR平台:-->如果是对明文控制字处理, 成功返回0, 失败返回FAILURE
*                         根据解析出的SEDsmInfo_S结构中的m_dwNotSumCheck判断是否和校验，然后拷贝输出，
*                      -->如果是对密文控制字处理，成功返回1, 失败返回FAILURE
                          需要在此接口先解密密文控制字，然后直接设置到解扰器中，dsmid、keytype都在结构体SEDsmInfo_S给出，
                          此时pcOutputKey数据无效
*Parameters : 		pcInputKey :输入的CW
*           		cLength : key长度
*           		pcOutputKey : 处理后的CW
*					bIsClearCW : 是否为明文控制字，1:明文，0:密文
*           		dwPara : SEDsmInfo_S结构体地址
*Return Values :  	成功:0:需要再设置处理后的CW
*                        1:不需要再设置处理后的CW   
*              		失败:FAILURE
**************************************************************************/
INT CSCASCWProcessCW(BYTE *pcInputKey, BYTE cLength, BYTE *pcOutputKey, BOOL bIsClearCW, DWORD dwPara);

/**************************************************************************
* Function Name: CSCASCWLoadCWPK
* 将CWPK加载到芯片内部
*Parameters :  		pcKey :输入的key
*              		cLength: key的长度
*              		dwPara : 其他参数, 预留接口
*Return Values :  	成功SUCCESS
*              		失败FAILURE
**************************************************************************/
INT CSCASCWLoadCWPK(BYTE *pcKey, BYTE cLength, DWORD dwPara);

/**************************************************************************
* Function Name: CSCASCWEncryptData
* 加密数据
*Parameters :  		pucCPCW :加密数据采用的key，长度为16字节,若为NULL,则默认采用各CA高级安全芯片本身SCK作为密钥进行加密
                	bIsNeedCBCVector :是否需要CBC Vector
                	pucVector :CBC Vector地址
                	nVentorLen:CBC Vector长度，ST7111平台支持的长度最大为4
                	pucSrcBuf :待加密数据地址
                	pucDesBuf :加密后数据地址
                	dwLen :加密(后)数据长度
                	emType : 加密数据选用的算法，目前ST7111平台支持算法为TDES
*Return Values :  	成功SUCCESS
*              		失败FAILURE
**************************************************************************/
INT CSCASCWEncryptData(BYTE* pucCPCW, BOOL bIsCBC,BOOL bIsNeedCBCVector,INT nVentorLen,BYTE* pucVector,
						BYTE* pucSrcBuf, INT nLen,Algorithm_E emType, BYTE* pucDesBuf);

/**************************************************************************
* Function Name: CSCASCWDecryptData
* 解密数据
*Parameters :  		pucCPCW :解密数据采用的key，长度为16字节,若为NULL,则默认采用各CA高级安全芯片本身SCK作为密钥进行解密
                	bIsNeedCBCVector :是否需要CBC Vector,设置为TRUE则为CBC，FALSE为ECB
                	pucVector :CBC Vector地址
                	nVentorLen:CBC Vector长度，ST7111平台支持的长度最大为4
                	pucSrcBuf :待解密数据地址
                	pucDesBuf :解密后数据地址
                	dwLen :解密(后)数据长度
                	emType : 解密数据选用的算法，目前ST7111平台支持算法为TDES
*Return Values :  	成功SUCCESS
*              		失败FAILURE
**************************************************************************/
INT CSCASCWDecryptData(BYTE* pucCPCW,BOOL bIsCBC, BOOL bIsNeedCBCVentor,INT nVentorLen,BYTE* pucVentor,
						BYTE* pucSrcBuf, INT nLen,Algorithm_E emType, BYTE* pucDesBuf);

/**************************************************************************
* Function Name: CSCASCWSetCAType
* 配置CA类型
*Parameters :  		E_SCW_CaType : CA枚举类型
               		dwPara :预留参数
*Return Values :  	无
***************************************************************************/
void CSCASCWSetCAType(E_SCW_CaType emType, DWORD dwPara);

/**************************************************************************
* Function Name: CSCASecureIoctl
* 设置和获取高级安全接口BOOTROM,JTAGKEY,JTAGLOCK,SCW;
* Parameters :      nDeviceIndex 使用设备的index, 从0开始, 目前需传0;
* Parameters :      emSecIOCtlType 使用该接口时传入传出参数的类型;
* Parameters :      pParams 根据传入的E_SecIOCtlType 类型设置或者获取的参数指针,
	         emSecIOCtlType为EM_SEC_SET_BOOTROM、EM_SEC_SET_JTAGKEY、
	                EM_SEC_SET_JTAGLOCK、EM_SEC_SET_SCW时, 该参数设置为NULL。
	         emSecIOCtlType为EM_SEC_SET_BY_APP和EM_SEC_GET_BY_APP时, 该参数为
	         		SECConfigParam_S结构体变量的地址。
	         emSecIOCtlType为其他值时, 该参数为unsigned int变量的地址。
* Return Values:   操作成功SUCCESS
*                  操作失败FAILURE
备注:emSecIOCtlType为EM_SEC_SET_BY_APP时可能熔断熔丝不成功，需要应用调用EM_SEC_GET_BY_APP
     判断熔丝状态，若熔丝没有熔断，需再次进行熔断。
**************************************************************************/
INT CSCASecureIoctl(INT nDeviceIndex, E_SecIOCtlType emSecIOCtlType, unsigned int * pParams);

/**************************************************************************
* Function Name: CSCASecureGetSMICVCValue
* Mstar平台SMI芯片获取CVC值(CVC value值长度不小于16BYTE);
* Parameters :      pucData 应用分配用来存放CVC value的内存地址;
* Parameters :      dwLen 应用分配用来存放CVC value的内存大小;

* Return Values:   操作成功SUCCESS
*                  操作失败FAILURE
备注:Mstar平台专有接口
**************************************************************************/
INT CSCASecureGetSMICVCValue(BYTE *pucData, DWORD dwLen);
//以下接口废弃掉，将不再使用
void CSCASCWWriteFuse(void);
void CSCASetCsdForNagra(BOOL bFlag);

#ifdef  __cplusplus
}
#endif

#endif 

