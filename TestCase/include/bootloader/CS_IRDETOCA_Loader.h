/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_IRDETO_Loader.h  
* Author:      wansuanlin
* Date:        Match 2006 
* Description: this file provide loader function
*            
*****************************************************************************************************************************/
#include "udiplus_typedef.h"

/****************************************************************************************************************************
* 			                                           Revision History                                                     *
*****************************************************************************************************************************
*   Revision  : V1.0
*   Programmer: Wan suanlin
*   Create    : 2006-03-17
*
*   V2.0         bl              2010-11-11
*
*****************************************************************************************************************************/
#ifndef _CS_IRDETO_LOADER_H_
#define _CS_IRDETO_LOADER_H_

#ifdef  __cplusplus
extern "C" {
#endif


#define CA_MAX_SIZE_REV                     (32) //���ӵ�
typedef struct _CpcbInfo
{
    BYTE      m_cLoadNum;  /*��ǰ���غ�*/
    DWORD     m_dwBin2irdVer;
    DWORD     m_dwBin2irdSerialNum;
    BYTE      m_acSignTimeDate[5];
    BYTE      m_acDownloadTimeDate[5];
    WORD      m_wDownloadDuration;
    DWORD     m_dwImageSize;
}T_CpcbInfo;

typedef struct _BbcbInfo
{
	WORD      m_wCRC;
	BYTE      m_cManufactureCode;  /*���̴���*/                  
	BYTE      m_cHW_Version;       /*Ӳ���汾��*/
	BYTE      m_cVerMajor;
	BYTE      m_cVerMinor;              
	BYTE      m_acSerialNumber[4];  
	BYTE      m_acRandomNumber[4];   
	BYTE      m_acRev[9];
}T_BbcbInfo; 

typedef struct _KeyStatusEx
{
    BYTE      m_cKeyStatus;
    BYTE      m_cLastImage;
    WORD      m_wSysID;
    WORD      m_wKeyVer;
    WORD      m_wVariant; /*�г���*/
    WORD      m_wSigVer;
    DWORD     m_dwRandomNum;
}T_KeyStatusEx;

typedef struct _LoaderIrdInfo
{
    WORD      m_wManufacturerCode;  /*���̴���*/
    WORD      m_wHardwareCode;      /*Ӳ���汾��*/
    WORD      m_wVariant;           /*�г��ţ���Irdeto����*/
    WORD      m_wLoadNum;           /*��ǰ���غ�*/
}T_LoaderIrdInfo;

typedef struct
{
   WORD u16SysID;               /* System ID                                 */
   WORD s16SysIDValid;          /* 0=u16SysID valid, -1=u16SysID invalid     */
   WORD u16KeyVer;              /* Key version                               */
   WORD s16KeyVerValid;         /* 0=u16KeyVer valid, -1=u16KeyVer invalid   */
   WORD u16SigVer;              /* Signature version                         */
   WORD s16SigVerValid;         /* 0=u16SigVer valid, -1=u16SigVer invalid   */
   WORD u16Variant;             /* Operator variant                          */
   WORD s16VariantValid;        /* 0=u16Variant valid, -1=u16Variant invalid */
   BYTE   u8BlockFailMask;      /* Bit mask of failed Irdeto NVRAM partition */
                                /* blocks (block0 = bit0)                    */
} IHL_STATUS, *IHL_PSTATUS;

typedef enum _CaFtaType
{
	E_CA_FTA_SAT   = 0x43,         //����,
	E_CA_FTA_CABLE = 0x44,      // ����
	E_CA_FTA_TER   = 0x5A,        // ����
	E_CA_FTA_T2    = 0x7F,	   //DVB-T2
	E_CA_FTA_MAX
}T_CaFtaType;


//IRDStatus
#define   CA_MAX_SIZE_IRD_SERIALNUMBER      (32)
#define	CA_MAX_SIZE_SC_STATUS               (32)
#define	CA_MAX_SIZE_IRD_VER                 (64)
#define	CA_MAX_SIZE_FIRMWARE                (64)
#define	CA_MAX_SIZE_CAM_VER                 (64)
#define	CA_MAX_SIZE_SC_VER                  (32)
#define	CA_MAX_SIZE_LOASER                  (64)
#define	CA_MAX_SIZE_SW_VER                  (64)

typedef struct _CaIrdStatus
{
	BYTE	m_szSerialNumber[CA_MAX_SIZE_IRD_SERIALNUMBER];
	BYTE	m_szRandomNumber[CA_MAX_SIZE_IRD_SERIALNUMBER]; //Random NO.
	BYTE	m_szScStatus[CA_MAX_SIZE_SC_STATUS];
	BYTE	m_szIrdVer[CA_MAX_SIZE_IRD_VER];
	BYTE	m_szFirmware[CA_MAX_SIZE_FIRMWARE];
	BYTE	m_szCamVer[CA_MAX_SIZE_CAM_VER];
	BYTE	m_szScVer[CA_MAX_SIZE_SC_VER];			//no used
	BYTE	m_szLoader[CA_MAX_SIZE_LOASER];
	BYTE    m_szRev[CA_MAX_SIZE_REV];             //���ӵ�
}T_CaIrdStatus;

//LoaderStatus
#define	CA_MAX_SIZE_MANUFACTURE_ID          (8)
#define	CA_MAX_SIZE_HW_VER                  (8)
#define	CA_MAX_SIZE_LD_SEQNO                (8)
#define	CA_MAX_SIZE_KEY_SEQNO               (8)
#define	CA_MAX_SIZE_VARIANT                 (8)
#define	CA_MAX_SIZE_KEY_SYSID               (8)
#define	CA_MAX_SIZE_KEYSTATUS               (8)
#define	CA_MAX_SIZE_KEY_VER                 (8)
#define	CA_MAX_SIZE_BIN2IRD_VER             (16)
#define	CA_MAX_SIZE_BIN2IRD_SER             (16)
#define	CA_MAX_SIZE_SIGN_DATE               (16)
#define	CA_MAX_SIZE_DL_DATE                 (16)
#define	CA_MAX_SIZE_SIGN_TIME               (16)
#define	CA_MAX_SIZE_DL_TIME                 (16)
#define	CA_MAX_SIZE_DL_DUR                  (16)
#define	CA_MAX_SIZE_DL_IMAGE                (16)

typedef struct _CaLoaderStatus
{
	BYTE	m_szSerialNumber[CA_MAX_SIZE_IRD_SERIALNUMBER];
	BYTE	m_szRandomNumber[CA_MAX_SIZE_IRD_SERIALNUMBER]; //Random NO.
	BYTE	m_szLoader[CA_MAX_SIZE_LOASER];
	BYTE	m_szManuID[CA_MAX_SIZE_MANUFACTURE_ID];
	BYTE	m_szHWVer[CA_MAX_SIZE_HW_VER];
	BYTE	m_szLDSeqNo[CA_MAX_SIZE_LD_SEQNO];
	BYTE	m_szKEYSeqNo[CA_MAX_SIZE_KEY_SEQNO];
	BYTE	m_szVARIANT[CA_MAX_SIZE_VARIANT];
	BYTE	m_szKEYSysId[CA_MAX_SIZE_KEY_SYSID];
	BYTE	m_szKEYStatus[CA_MAX_SIZE_KEYSTATUS];
	BYTE	m_szKEYVer[CA_MAX_SIZE_KEY_VER];
	BYTE	m_szBIN2IRDVer[CA_MAX_SIZE_BIN2IRD_VER];
	BYTE	m_szBIN2IRDSer[CA_MAX_SIZE_BIN2IRD_SER];
	BYTE	m_szSignDate[CA_MAX_SIZE_SIGN_DATE];
	BYTE	m_szDLDate[CA_MAX_SIZE_DL_DATE];
	BYTE	m_szSignTime[CA_MAX_SIZE_SIGN_TIME];
	BYTE	m_szDLTime[CA_MAX_SIZE_DL_TIME];
	BYTE	m_szDLDur[CA_MAX_SIZE_DL_DUR];
	BYTE	m_szImageSize[CA_MAX_SIZE_DL_IMAGE];
	BYTE       m_szRev[CA_MAX_SIZE_REV];             //���ӵ�
}T_CaLoaderStatus;


typedef struct _QpskSwitch
{
	BYTE	m_cSwitch0_12V;     //SWITCH_0V | SWITCH_12V
	BYTE	m_c22K;             //SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBPower;        //SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBType;         //SINGLE | UNIVERSAL
	DWORD	m_dwLNBLowFreq;     //low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]
	DWORD	m_dwLNBMidFreq;     //middle LNB frequency in Universal Mod  [unit:KHz]
	DWORD	m_dwLNBHighFreq;    //high LNB frequency in Universal Mod   [unit:KHz]
	BYTE	m_cDiSEqC10;        //DiSEqC1.0����	( PORT_A | PORT_B | PORT_C | PORT_D )
	BYTE	m_cDiSEqC11;        //DiSEqC1.0 ( 0-16 and 0 for not used )
	BYTE	m_cToneBurst;       //Tone Burst���� ( TONEBURST_NONE / TONEBURST_A / TONEBURST_B )
	BYTE	m_cDiSEqCRepeat;    //�ظ����� ( NoRepeat / OnceRepeat / TwiceRepeat )
	DWORD	m_dwFreqencyOffset; //channel frequency offset in KHz
	BYTE	m_cIQ;              //0-Normal; 1-Invers
	BYTE 	m_cPilot;
	BYTE	m_cOption;        //m_dwOption:2;
	BYTE	m_cModulation; //m_dwModulation:2;1:qpsk,2:8psk
	BYTE  	m_cStandard;    //m_dwstandard:1; 0:s, 1:s2
	BYTE       m_cRev[3];
}T_QpskSwitch;

typedef struct _SatelliteSystem
{
	DWORD      m_dwFrequency;///< Ƶ�ʣ���λKHz
	WORD        m_wPrbitalPosition;
	BYTE          m_cWestEastFlag;
	BYTE          m_cPolarization;///�����źż�������0:ˮƽ����, 1:��ֱ����, 2:��������,3:��������
	BYTE          m_cModulation;///	0: QPSK��8PSK�Զ�ʶ��ʽ,1 : QPSK���Ʒ�ʽ,2 : 8PSK���Ʒ�ʽ
	DWORD      m_dwSymbolRate;///< �����ʣ���λKbps
	BYTE       m_cFecInner;///�����е�FEC������,	0: FEC��ʽ�Զ�ѡ�� ,1: 1/2��ʽ, 2: 2/3��ʽ, 3: 3/4��ʽ, 4: 5/6��ʽ,5:7/8��ʽ,6: 8/9��ʽ��
	T_QpskSwitch  m_stQpskSwitch;
}T_SatelliteSystem;

typedef struct _TerrestrialSystem
{
	DWORD   m_dwCentreFrequency;
	BYTE       m_cBandWidth;
	BYTE       m_cConstellation;
	BYTE       m_cHierarchyInfo;
	BYTE       m_cCodeRateHpStream;
	BYTE       m_cCodeRateLpStream;
	BYTE       m_cGuardInterval;
	BYTE       m_cTransmissionMode;
	BYTE       m_cOtherFrequencyFlag;	
}T_TerrestrialSystem;

typedef struct _T2System
{
	DWORD   m_dwCentreFrequency;
	BYTE       m_cPlpID;
	BYTE       m_cT2SystemID;
	BYTE       m_cSISO_MISO;
	BYTE       m_cBandWidth;
	BYTE       m_cGuardInterval;
	BYTE       m_cTransmissionMode;
	BYTE       m_cOtherFrequencyFlag;
	BYTE       m_cTfsFlag;
	DWORD      m_cCellID;	
}T_T2System;

//ֻ��Ҫm_dwFrequency,     m_cModulation,   m_dwSymbolRate   ����
typedef struct _CableSystem
{
	DWORD      m_dwFrequency;          /* Ƶ��(unit:KHz),Ƶ�ʷ�Χ(48-866)*/
	BYTE          m_cFecOuter;
	BYTE          m_cModulation;       /**< 0 Not Define  QAM���� ( 0:Not Define   1:16QAM	    2:32QAM	   3:64QAM	  4:128QAM	 5:256QAM)*/
	DWORD      m_dwSymbolRate;       /* ������(unit:KHz)       */
	BYTE       m_cFecInner;
}T_CableSystem;


typedef union _DeliverySystem 
{
   	T_CableSystem              m_stCableStytle;        //����
	T_SatelliteSystem           m_stSatelliteStytle;      //����
	T_TerrestrialSystem     m_stTerrestrialStytle;   //����
	T_T2System                    m_stT2Stytle;                //DVB-T2
}T_DeliverySystem;


typedef struct _OtaCfg     // OTA��������
{
	WORD    m_wOtaPid;         //PID
	BYTE    m_cOtaTableId;    // table ID
	INT     m_nOtaTime;      //tunnerId
	
	T_CaFtaType         m_eType;         //tuner����: ����,����,����
	T_DeliverySystem    m_stDlySys;  //Ƶ��,������, ������ʽ......
}T_OtaCfg;




//�ⷵ��ֵ�������,  int ���� 
////----start-----
#define ERROR_CS_IRDETOCA_BL_SUCCESS        0          //�ɹ�
#define ERROR_CS_IRDETOCA_BL_FAILURE        10         //����ʧ�� 



//ģ��EEPROM���� 
#define ERROR_CS_IRDETOCA_BL_NVM        	    20		//ģ��eeprom  ��ʼ������
#define ERROR_CS_IRDETOCA_BL_NVM_BIG       	21		//ģ��eeprom  ���ݿ����0x2000
#define ERROR_CS_IRDETOCA_BL_NVM_UER_BIG    22     //ģ��eeprom  ���ݿ����  eeprom ռ�õ�flash��
#define ERROR_CS_IRDETOCA_BL_NVM_ADDR_ZERO   23     //dwVEEPROMSize =0����. ����  dwVEEPROMUserSize=0����
#define ERROR_CS_IRDETOCA_BL_NVM_INIT_REPEAT  24   //ģ��EEPROM �ظ���ʼ��.
#define ERROR_CS_IRDETOCA_BL_NVM_NO_INIT     25   //ģ��EEPROM û�г�ʼ��

#define ERROR_CS_IRDETOCA_BL_KEY                  30    //Irdeto KEY   ���� ,  û������ KEY������key�Ǵ����
//CPCB����
#define ERROR_CS_IRDETOCA_BL_CPCB_PART            40  //��ȡ CPCB[1] ��appʵ��ռ�õ������ռ��
#define ERROR_CS_IRDETOCA_BL_CPCB_SIZE          41  //cpcb��С����
#define ERROR_CS_IRDETOCA_BL_CPCB_CRC          42  //cpcbУ��ʧ��

//����δ���ô���
#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetFlashEntireSize  50  //BLSetFlashEntireSize����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetBooterSize       51  //BLSetBooterSize����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetLoaderSize       52  //BLSetLoaderSize����δ���� 

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetBBCBSize       53  //BLSetBBCBSize ����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetCPCBSize       54  //BLSetCPCBSize ����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetVEEPROMSize       55  //BLSetVEEPROMSize ����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetVEEPROMUserSize       56 //BLSetVEEPROMUserSize ����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetAppSize       57 //BLSetAppSize ����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetFirstPartitionSize      58 //BLSetFirstPartitionSize ����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetPartitionSize     59 //BLSetPartitionSize ����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetIrdetoKeySize    60 //BLSetIrdetoKeySize ����δ����

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetLoaderptSize    61 //BLSetLoaderptSize ����δ����
///


#define ERROR_CS_IRDETOCA_BL_FLASH_READ         70   //��flashʧ��.
#define ERROR_CS_IRDETOCA_BL_FLASH_WRITE        71   //дflashʧ��.
#define ERROR_CS_IRDETOCA_BL_FLASH_ERASE        72   //����flash ʧ��
///---end----













/**********************************************************

1. App ���������ø������ݵĵ�ַ,��С�Ȼ�����Ϣ,������Ŀflash map��д 
2. App �����ʼ����
3. App дOTA����,OTA��־

**********************************************************/


//(1)--app���õ�ַ��ʼ--------------------------------------------------------------------------

/**********************************************************
���ܣ�����flash�Ļ���ַ�ʹ�С
���룺flash�Ļ���ַ�ʹ�С
˵�����˽ӿ���Ӧ�ó���������, 
**********************************************************/
void BLSetFlashEntireSize(DWORD dwFlashBaseAddr, DWORD dwFlashSize);

/**********************************************************
���ܣ�����Booter����ʼ��ַ�ʹ�С
���룺Booter�Ĵ�С����ʼ��ַ
˵�����˽ӿ���Ӧ�ó���������
		  ����洢��flash����
**********************************************************/
void BLSetBooterSize(DWORD dwBooterAddr, DWORD dwBooterSize);

/**********************************************************
���ܣ�����Loader�ĵ�ַ�ʹ�С
���룺Loader�ĵ�ַ�ʹ�С
*˵�����˽ӿ���Ӧ�ó���������
		����洢��flash����
**********************************************************/
void BLSetLoaderSize(DWORD dwLoaderAddr, DWORD dwLoaderSize);

/**********************************************************
*���ܣ�����BBCB�ĵ�ַ�ʹ�С
*���룺BBCB�ĵ�ַ�ʹ�С
*˵�����˽ӿ���Ӧ�ó���������
	����洢��flash����
**********************************************************/
void BLSetBBCBSize(DWORD dwBBCBAddr, DWORD dwBBCBSize);

/**********************************************************
*���ܣ�����CPCB�ĵ�ַ�ʹ�С
*���룺CPCB�ĵ�ַ�ʹ�С
*˵�����˽ӿ���Ӧ�ó���������
	����洢��flash����
**********************************************************/
void BLSetCPCBSize(DWORD dwCPCBAddr, DWORD dwCPCBSize);

/**********************************************************
*���ܣ�����ģ��EEPROM�ĵ�ַ�ʹ�С,ģ��eeprom�Ŀ��С,���
			��Ŀû��ģ��eeprom,�򲻵��ñ������� 

*���룺ģ��EEPROM�ĵ�ַ�ʹ�С
*˵�����˽ӿ���Ӧ�ó���������

dwVEEPROMAddr:ģ��eeprom��flash�е���ʼ��ַ
dwVEEPROMSize:ģ��eeprom��flash�еĳ���
dwVEEPROMUserSize:ģ��eeprom����ĳ���
**********************************************************/
void BLSetVEEPROMSize(DWORD dwVEEPROMAddr, DWORD dwVEEPROMSize,DWORD dwVEEPROMUserSize);

/**********************************************************
*���ܣ�����App�ĵ�ַ�ʹ�С
*���룺App�ĵ�ַ�ʹ�С
*˵�����˽ӿ���Ӧ�ó���������
	����洢��flash����
**********************************************************/
void BLSetAppSize(DWORD dwAppAddr, DWORD dwAppSize);

/**********************************************************
*���ܣ�����flash��1�������Ĵ�С
*���룺�����Ĵ�С
*˵�����˽ӿ���Ӧ�ó���������
**********************************************************/
void BLSetFirstPartitionSize(DWORD dwPartitionSize);

/**********************************************************
*���ܣ�����flash�����Ĵ�С
*���룺�����Ĵ�С
*˵�����˽ӿ���Ӧ�ó���������
**********************************************************/
void BLSetPartitionSize(DWORD dwPartitionSize);

/**********************************************************
*���ܣ�����Irdeto KEY�Ĵ洢��EEPROM.    ȱʡ״̬���Ǵ洢��flash����
*���룺 
*˵�����˽ӿ���Ӧ�ó���������
**********************************************************/
void BLSetIrdetoKeyOnEEPROM(void);

/**********************************************************
*���ܣ�����Irdeto KEY�ĵ�ַ�ʹ�С,ȱʡ״̬���Ǵ洢��flash����
*���룺Irdeto Key�ĵ�ַ�ʹ�С
*˵�����˽ӿ���Ӧ�ó���������
**********************************************************/
void BLSetIrdetoKeySize(DWORD dwIrdetoKeyAddr, DWORD dwIrdetoKeySize);

/**********************************************************
*���ܣ�����loaderpt�ĵ�ַ�ʹ�С��ȱʡ״̬���Ǵ洢��flash����
*���룺loaderpt�ĵ�ַ�ʹ�С
*˵�����˽ӿ���Ӧ�ó���������,
**********************************************************/
void BLSetLoaderptSize(DWORD dwLoaderptAddr, DWORD dwLoaderptSize);

/**********************************************************
*���ܣ�����loaderpt�����ģ��eeprom��,ȱʡ״̬���Ǵ洢��flash����
*���룺 
*˵�����˽ӿ���Ӧ�ó���������
**********************************************************/
void BLSetLoaderptOnEEPROM(void);

/**********************************************************
*���ܣ�����syssetting�ĵ�ַ�ʹ�Сȱʡ״̬���Ǵ洢��flash����
*���룺syssetting�ĵ�ַ�ʹ�С,
*˵�����˽ӿ���Ӧ�ó���������,���û��syssetting����Ҫ�������
**********************************************************/
void BLSetSysSettingSize(DWORD dwSysSettingAddr, DWORD dwSysSettingSize);

/**********************************************************
*���ܣ�����syssetting�����ģ��eeprom�У�ȱʡ״̬���Ǵ洢��flash����
*���룺 
*˵�����˽ӿ���Ӧ�ó���������
**********************************************************/
void BLSetSysSettingOnEEPROM(void);

/**********************************************************
*���ܣ�����USB������Ҫʹ�õ���ResetTypeȱʡ״̬���Ǵ洢��flash����
*���룺dwAddr: ��ο���Ŀ��flash map�е�reset type����,��ַ
*˵���������Ŀû��USB����Ҫ��,����Ҫ�������
**********************************************************/
void BLSetUSBSize(DWORD dwAddr);
/**********************************************************
*���ܣ�����USB�������������ģ��eeprom�У�ȱʡ״̬���Ǵ洢��flash����
*���룺 
*˵��:�����Ŀû��USB����Ҫ��,����Ҫ�������
**********************************************************/
void BLSetUSBOnEEPROM(void);
//-----���õ�ַ����-----------------------------------------------------------------------


























//(2)---app��ʼ����------------------------------------------------------

/**********************************************************
���ܣ�app��ʼ��OTA ��
���룺
�����
���أ�0��ʾ�ɹ�,  ����ʧ��
˵����app����
**********************************************************/
INT CS_CA_Loader_Init(void);
//-----app��ʼ�������--------------------------------------------------

















//(3) app��ȡ���ݻ���Ϣ,     
/**********************************************************
���ܣ�app��ȡCPCB������
���룺
�����
���أ�0��ʾ�ɹ�,  ����ʧ��
˵����app����
**********************************************************/
INT CS_CA_Loader_GetCPCB(T_CpcbInfo*  pstBuf );
/**********************************************************
���ܣ�app��ȡBBCB������
���룺
�����
���أ�0��ʾ�ɹ�,  ����ʧ��
˵����app����
**********************************************************/

INT CS_CA_Loader_GetBBCB(T_BbcbInfo*  pstBuf);
/**********************************************************
���ܣ�app��ȡIrdeto key��״̬
���룺
�����
���أ�0��ʾ�ɹ�,  ����ʧ��
˵����app����
**********************************************************/
INT CS_CA_Loader_GetKeyStatus(T_KeyStatusEx*  pstKeyStatus);
/**********************************************************
���ܣ�app��ȡIrdeto������״̬,����(���̴���,Ӳ���汾��,�г���,��ǰ���غ�)
���룺
�����
���أ�                         1��ʾ�ɹ�,       0��ʾʧ��
˵����app����
**********************************************************/
BOOL CS_CA_Loader_ReadIrdCfg(T_LoaderIrdInfo*  pIrdCfg);

/**********************************************************
���ܣ���ȡAPP.bin�ļ���flash�е�ʵ�ʳ���
���룺dwAddr:   Ӧ�ó�����flash�еĵ�ַ 
�����pdwRealSize:ʵ�ʴ�С
���أ�SUCCESS(0):�ɹ�,FAILURE(1)
˵�����˽ӿ���Ӧ�ó���������
**********************************************************/
DWORD CSBL_GetAppRealSize(DWORD dwAddr, PDWORD pdwRealSize);
/**********************************************************
���ܣ���ȡIrdeto BL �������汾�š�
���룺pcBuf[] ���鳤�ȱ������50���ֽ�
�����BL�������汾�š��ַ���ģʽ��
˵�����˽ӿ���Ӧ�ó���������
**********************************************************/
void CS_CA_Loader_Version(char * pcBuf);











///(4) дOTAƵ��,������,PID, tableID�Ȳ�����Ϣ
/**********************************************************
���ܣ�appдOTA��������,������loaderpt���е�OTA��Ч��־, ����������loader;
���룺
�����
���أ�0��ʾ�ɹ�,  ����ʧ��
˵����app����
**********************************************************/
INT CS_CA_Loader_SaveOtaInfo(T_OtaCfg*  pstOtaInfo);


/**********************************************************
���ܣ�appдUSB ������־
���룺
�����
���أ�0��ʾ�ɹ�,  ����ʧ��
˵����app����
**********************************************************/
INT CSBLSetIrdetoUsbUpgrade(void);


#ifdef  __cplusplus
}
#endif

#endif




