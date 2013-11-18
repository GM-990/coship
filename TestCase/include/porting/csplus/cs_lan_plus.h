
#ifndef _CS_LAN_PLUS_H_
#define _CS_LAN_PLUS_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************/
/* cUse16Bit                                                      */
/*                                                                */
/* [DM9000A]                                                      */
/* 0 - default;                                                   */
/* 1 - case SWITCH (ks8895x, etc.) with    EEPORM: 11001B;        */
/* 2 - case SWITCH (ks8895x, etc.) without EEPORM: 01010B;        */
/* 3 - case EOC 10M-half                                          */
/* [SMSC9115] or [KSZ8842]                                        */
/* bit [7] ... [3] [2] [1] [0]                                    */
/*                          0 ...... auto_negotiating             */
/*                          1 ...... force                        */
/*                      0 .......... duplex                       */
/*                      1 .......... half                         */
/*                  0 .............. 100Mbps                      */
/*                  1 .............. 10Mbps                       */
/******************************************************************/
typedef struct
{
    unsigned int  nBaseAddr;        /*Ether Chip Base address                  */
    unsigned char cShiftAddr;       /* Shift Address                           */
    unsigned char cInterruptNumber; /*Inerrupt Number                          */
    unsigned char cInterruptLevel;  /* Inerrupt Level                          */
    unsigned char cUse16Bit;        /*access chip port is use 16 bit or 32 bit */
    unsigned int  nStackSize;
    unsigned int  nTaskPri;
}LAN_CONFIG_INFO;


typedef enum/*WPA��֤���ص���Ϣ�Լ��ص��������ظ�Ӧ�õ���Ϣ����*/
{
    EM_WLAN_WPA_DISCONNECTED = 0,
    EM_WLAN_WPA_INACTIVE,
    EM_WLAN_WPA_SCANNING,
    EM_WLAN_WPA_ASSOCIATING,
    EM_WLAN_WPA_ASSOCIATED,
    EM_WLAN_WPA_4WAY_HANDSHAKE,
    EM_WLAN_WPA_GROUP_HANDSHAKE,
    EM_WLAN_WPA_COMPLETED,
    EM_WLAN_WPA_SET_ERR,
    EM_WLAN_WPA_KEY_ERR,
    EM_WLAN_WPA_NO_FOUND_AP,
    EM_WLAN_WPA_FINISH,
    EM_WLAN_WEP_STATE_UP, //12
    EM_WLAN_WEP_STATE_ERROR,
    EM_WLAN_NONE_ENC_UP, //14
    EM_WLAN_CONN_TIMEOUT,
    EM_WLAN_DOWN,
    EM_WLAN_UP,
    EM_WLAN_APLOST,
    EM_WLAN_APCONNECT
}CSWLanMsgInfo_E;

typedef enum /*����ֵ*/
{
    EM_WLAN_SUCCESS =0,
    EM_WLAN_FAIL,
    EM_WLAN_INVALPARA,
    EM_WLAN_NETIFUP_FAIL ,
    EM_WLAN_NETIFDOWN_FAIL,
    EM_WLAN_CONNECTAP_ERROR,
    EM_WLAN_SETKEY_ERROR,
    EM_WLAN_CLEANKEY_ERROR,
    EM_WLAN_GETINFO_ERROR,
    EM_WLAN_GETAPNAME_ERROR	/*û�л�ȡ�� AP����*/
}CSWLanRet_E;



/* callback fun */
typedef void (*CSWLanCallback_F)(CSWLanMsgInfo_E eMsgType, void *pPara);

typedef struct _CSWLanInitPara_S/*����������ʼ������*/
{
    unsigned int dwStackSize;           /*��WPA��֤�������ͨ�ŵ��̵߳Ķ�ջ�Ĵ�С */
    unsigned int dwPriority;            /*��WPA��֤�������ͨ�ŵ��̵߳����ȼ�*/
    CSWLanCallback_F fnWpaCallback;   /* �ص�����������ΪNULL������ʹ��UDI2.0�汾��˵������ص��ǲ���Ҫ�� */
} CSWLanInitPara_S;


/*-------------------------------------------------------------------------
**                      ��������                                         **
-------------------------------------------------------------------------*/

/****************************************************************************
*  Function:	Lan Initialize                 
*  Input:	LAN_CONFIG_INFO *	, int			
*  Output:	None							
*  Return:	0 for success and -1 for FAILURE
*  Note:	                                    
****************************************************************************/ 
int  CSLanInitialize( LAN_CONFIG_INFO* psLanConfig, int nNicCount);


/****************************************************************************  
*   Function:	Set Lan Mac Address to chip 		    
*   Input:	nChipIndex:may be 0 or other ( if there are muti piece of chips)	
*           pbMacAddress:is Mac Address
*           nLen:pbMacAddress buffer lenght Must be 6
*   Output:	NONE;						    
*   Return:	0 for successfully get and -1 for get failed.	    
****************************************************************************/ 
int  CSLanSetMacAddress(int nChipIndex,unsigned char* pucMacAddress,int nLen);


 /**
 @brief ����������mac��ַ
 @param[in] szEthName:�����豸���ƣ�����"eth0"��,Ҫ����'\0'��Ϊ������
 @param[in] pbMacAddress��Ҫ���õ�mac��ַ,��������Ϊ{0x00,0x0e,0x09,0xee,0x21,0xef},ע�����������mac��ַ��������Ϊ��ͬ�ģ�Ҳ��������Ϊ�ಥ���߹���ַ
 @param[in] nMacLen��Ҫ���õ�mac��ַ�ĳ��ȣ�����Ϊ6���ֽ�		                                        
 @return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ���CSUDI_FAILURE  ��
 */
int  CSLanSetMacByName(char * szEthName,unsigned char *pbMacAddress,int nMacLen);


/*-------------------------------------------------------------------------
**                     ��������                                          **
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
CSWLanRet_E CSWLanInit(CSWLanInitPara_S* psWLanInitPara);


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
CSWLanRet_E CSWLanUnInit(void);

 
#ifdef __cplusplus
}
#endif
#endif /* __CSLANPLUS_H__ */


