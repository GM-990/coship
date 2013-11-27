/****************************************************************************/
/*
 * Filename:        cs_lan_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_lan_plus.h"

/*-------------------------------------------------------------------------
**                      有线网卡                                         **
-------------------------------------------------------------------------*/

/****************************************************************************
*  Function:	Lan Initialize                 
*  Input:	LAN_CONFIG_INFO *	, int			
*  Output:	None							
*  Return:	0 for success and -1 for FAILURE
*  Note:	                                    
****************************************************************************/ 
int  CSLanInitialize( LAN_CONFIG_INFO* psLanConfig, int nNicCount)
{
    int Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/****************************************************************************  
*   Function:	Set Lan Mac Address to chip 		    
*   Input:	nChipIndex:may be 0 or other ( if there are muti piece of chips)	
*           pbMacAddress:is Mac Address
*           nLen:pbMacAddress buffer lenght Must be 6
*   Output:	NONE;						    
*   Return:	0 for successfully get and -1 for get failed.	    
****************************************************************************/ 
int  CSLanSetMacAddress(int nChipIndex,unsigned char* pucMacAddress,int nLen)
{
    int Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


 /**
 @brief 设置网卡的mac地址
 @param[in] szEthName:网卡设备名称，例如"eth0"等,要求以'\0'作为结束符
 @param[in] pbMacAddress：要设置的mac地址,例如设置为{0x00,0x0e,0x09,0xee,0x21,0xef},注意各个网卡的mac地址不能设置为相同的，也不能设置为多播或者广告地址
 @param[in] nMacLen：要设置的mac地址的长度，定义为6个字节		                                        
 @return 成功返回CSUDI_SUCCESS;失败返回CSUDI_FAILURE  。
 */
int  CSLanSetMacByName(char * szEthName,unsigned char *pbMacAddress,int nMacLen)
{
    int Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


/*-------------------------------------------------------------------------
**                     无线网卡                                          **
-------------------------------------------------------------------------*/

 /******************************************************************************
Function Name:    CSWLanInit
Description:
    Initialize the WLAN.
Input:
    ptInitPara: the initializition parameter of WLAN
Output:
    None
Return Value:
    EM_WLAN_INVALPARA:
    EM_WLAN_FAIL:
    EM_WLAN_SUCCESS
*******************************************************************************/
CSWLanRet_E CSWLanInit(CSWLanInitPara_S* psWLanInitPara)
{
    CSWLanRet_E Retcode = EM_WLAN_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


 /******************************************************************************
Function Name:    CSWLanUnInit
Description:
    UnInitialize the WLAN.
Input:
    None
Output:
    None
Return Value:
    EM_WLAN_FAIL:
    EM_WLAN_SUCCESS
*******************************************************************************/
CSWLanRet_E CSWLanUnInit(void)
{
    CSWLanRet_E Retcode = EM_WLAN_SUCCESS;  
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


