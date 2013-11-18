/******************************************************************************
FileName:  CS_Verimatrix_Interface.h
Copyright (c) 2011， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        caoxiaojing 

File Description:       

Modify History:
Version           Date                Author           Description
--------    ----------       --------        ------------
1.0            2011-01-05        caoxiaojing         Create

*******************************************************************************/
#ifndef CS_VERIMATRIX_INTERFACE_H
#define CS_VERIMATRIX_INTERFACE_H
/******************************************************************************
 *                                 Header File Include                        *
 ******************************************************************************/
#include "udiplus_typedef.h"
#include "cs_cainterface.h"


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 Macro/Define/Structs                       *
 ******************************************************************************/
typedef struct VM_Context_S
{
	INT		m_nmagic;
	BOOL	m_bOpened;
	BOOL	m_bKeyFound;
	BYTE*	m_pucbuf;	
	// configuration
	INT		m_nnodecrypt;
	void*	m_pContext;
	INT         m_ntoss_until_pat;
	// statistics
	INT        m_nPacketCount;
} VM_Context_S;


/**************************************************************************
* Function Name: CSCAVMDecryptStreamData
*   软解接口
*
*Parameters	
*	  pcbuf:
*         [in] 存放加扰TS数据的buffer
*		  [out]存放解扰后清流数据的buffer
*     nSize:buffer的大小
*       
*Return Values
*		-1 	  失败
*		0	  成功
*       1     输入参数无效
*       2     HANDLE 无效
*       3     环境未打开，即DRM信息有误
**************************************************************************/
INT CSCAVMDecryptStreamData(unsigned char * pcbuf, int nSize);


#ifdef __cplusplus
}
#endif

#endif  

