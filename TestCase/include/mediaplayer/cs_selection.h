/******************************************************************************
FileName:  SelectionInterface.h
Copyright (c) 2007�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        pengyunjuan (pengyunjuan@coship.com)

File Description:
The interface of selection module.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2007-08-1  pengyunjuan       Create

*******************************************************************************/

#ifndef SELECTIONINTERFACE_H
#define SELECTIONINTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

    /******************************************************************************
    *                                 Header File Include                         *
    ******************************************************************************/
#include "mmcp_debug.h"
#include "mmcp_typedef.h"
#include "mmcp_tuner.h"
#include "cs_player_message.h"
    /******************************************************************************
    *                                 Macro/Define/Structus                       *
    ******************************************************************************/

#define   SEL_SERVICE_LOCK		0x01//��ǰ��ĿΪ���˽�Ŀ���Ľ�Ŀ����Ŀ��Ч
#define   SEL_CAT               0x02//���ز�����CAT��Ƶ����Ч
#define   SEL_NIT				0x04//���ز�����NIT��Ƶ����Ч
#define   SEL_BAT				0x08//���ز�����BAT��Ƶ����Ч
#define   SEL_SDT				0x10//���ز�����SDT��Ƶ����Ч

#define   SEL_STOP_MONITOR_ALL 0 //ֹͣ����ʱֹͣ���б�ļ���
#define   SEL_STOP_WITHOUT_CAT_MONITOR 0x1		//ֹͣ���ŵ�ʱ��ֹͣcat�����

    typedef enum
    {
        EM_SEL_IPTV,             //IPTV MODE
        EM_SEL_IPQAM, 	       //IPQAM MODE
        EM_SEL_IPUDP_ES, 	       //IPQAM DUP����ES����
        EM_SEL_UNKNOWN    //δ֪����
    }CSSELTransMode_E;

/* STB״̬ö�� */
typedef enum{
	EM_SEL_STATUS_DVB,      //����DVB
	EM_SEL_STATUS_BROWSE,   //���������
	EM_SEL_STATUS_VOD,      //����VOD
	EM_SEL_STATUS_INVALID   
 }CSSELNrcStatus_E;

    /*Ϊ֧����Ϣ��͸�����ܣ��ص���Ϣ���¹滮.
        �޸�ԭ��:���е���Ϣ������ͷ�ļ��У�˽�е���Ϣ(�����ض��������ϵ�������Ϣ)͸��
         
        IPPUMP:0x0~0xfff
        MEDIAPLAYER:0x1000~0x1fff
        SELECTION:0x2000~0x2fff
         
        */
    /******************************************************************************
    *                                 Global Function Declare                     *
    ******************************************************************************/
    /******************************************************************************
    Function Name:    FOnCSSELCallback
     
    Description:
    	�û��ص�,selection ����Ϣ���ظ��û�
     
    Input:
    	dwUserData: �û��ڴ���selectionʱ������û�����
    	nMsg : ��Ϣ����
    	dwPara1 : 	����1 
    	dwPara2 :	����2 
     
    Output:
     
    Return Value:
     
    Other:
    *******************************************************************************/
    typedef void (*FOnCSSELCallback)(DWORD dwUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);

/******************************************************************************
Function Name:    FOnCSEOCSwitch
 
Description:
	EOC�ص�, ֪ͨNRC�⣬ STBҵ���л�
 
Input:
	eMode: STB���л�������״̬,ȡֵ�μ�CSSELNrcStatus_E
	   
Output:
 
Return Value:
    �ɹ�����TRUE��ʧ�ܷ���FALSE
Other:
*******************************************************************************/
typedef BOOL (*FOnCSEOCSwitch)(CSSELNrcStatus_E eMode);

    /******************************************************************************
    Function Name:    CSSELSetTransMode
     
    Description:
    	�û����û���������Ƶ����ģʽ:EM_SEL_IPTV��EM_SEL_IPQAM��EM_SEL_IPUDP_ES
     
    Input:
    	hSelection: selection���
    	eTranMode : CSSELTransMode_E����ö�ٱ�������CSSELTransMode_E����
     
    Output:
     
    Return Value:
       ���óɹ�����TRUE,���򷵻�FALSE
    Other:
      Ĭ�Ϸ�ʽΪEM_SEL_IPQAM,
      CSSELCreate֮������ŵ��ã�  �ڽ���㲥��ʱ��֮ǰҪ��֤�Ѿ����ú�
    *******************************************************************************/
    BOOL CSSELSetTransMode (HCSHANDLE hSelection, CSSELTransMode_E eTranMode);


    /******************************************************************************
    Function Name:    CSSELGetTransMode
     
    Description:
    	�û���ȡ����������Ƶ����ģʽ:EM_SEL_IPTV��EM_SEL_IPQAM���EM_SEL_IPUDP_ES��EM_SEL_UNKNOWN
     
    Input:
    	hSelection: selection���
    	eTranMode : CSSELTransMode_E����ö�ٱ�������CSSELTransMode_E����
     
    Output:
     
    Return Value:
         ���ػ�ȡ���Ĵ���ģʽ��
    Other:
    ���Locator�д���streamChannel�ֶΣ���˺����õ��Ľ���Դ��ֶ�
    ������ģʽΪ׼��
    ����:
    Ӧ�ÿ�ʼ������IPTV:CSSELSetTransMode(hSelection,EM_SEL_IPTV);
    ��������ʱLocatorΪrtsp://10.10.99.10/ddd.ts&streamChannel=IPQAM
    ֮����ô˽ӿڵõ��Ľ��ΪEM_SEL_IPQAM
    *******************************************************************************/
    CSSELTransMode_E CSSELGetTransMode (HCSHANDLE hSelection);


    /******************************************************************************
        Function Name:    CSSELCreate
         
        Description:
        	����selectionģ��
         
        Input:
        	fnCallback : ע��Ļص�����
        	dwUserData :  �û����ݣ��ص�������dwPara2�ķ���ֵ
         
        Output:
         
        Return Value:
        	���ش�����selectionģ����
         
        Other:
        *******************************************************************************/
    HCSHANDLE CSSELCreate (FOnCSSELCallback	fnCallback,
                           DWORD	dwUserData);

    /******************************************************************************
    Function Name:    CSSELDestroy
     
    Description:
    	����selectionģ�飬�ͷ������Դ
     
    Input:
    	hSelection : selection���
     
    Output:
     
    Return Value:
    	�ɹ���TRUE
    	ʧ�ܣ�FALSE
     
    Other:
    *******************************************************************************/
    BOOL CSSELDestroy(HCSHANDLE hSelection);

    /******************************************************************************
    Function Name:    CSSELSelectByLocator
     
    Description:
    	����service��Locator����ҵ��ѡ��
     
    Input:
    	hSelection : selection���
    	pszLocator : Locator�ַ���
    	dwOption :	����ѡ�����ΪNULL��Ҳ����Ϊͷ�ļ���define��ѡ�� 
     
    Output:
     
    Return Value:
    	�ɹ���TRUE
    	ʧ�ܣ�FALSE
     
    Other:
    �Ϸ�Locator��ʽ�����¼���:
	1��MP3 ���±߲���http://a/xxx.mp3l��
	2��VOD��rtsp://10.10.99.10/ddd.ts��
	3��TV Channel��֧������URL��ʽ��
	1��dvb://OriginalNetworkID.TS_ID.ServiceID.ComponentID������ο������ֵ����м�������淶����3���ָ�¼I��1.2.1С�ڡ�
	
	2)component://Frequency.SymbolRate.Modulation.pcrpid.vpid.vtype.apid.atype
	��ע��dvb��Ҫ�ظ�ʽ������Ϊ16����,component�о�Ϊ10����
			Frequency:��λHz
			SymbolRate:��λkbps
			Modulation:16,32,64,128,256
	4���ļ���file:///�ļ�·��
	5��IFrame:  iframe://addr.nlen  �����ָ�ʽֻ��UDI2.0��Ч��
	addrΪIFrame�������ڴ��ַ��nLenΪ���ݳ��ȣ����߾�Ϊʮ����
    
    *******************************************************************************/
    BOOL CSSELSelectByLocator (HCSHANDLE hSelection,
                               const char *pszLocator,
                               DWORD	dwOption);

    /******************************************************************************
    Function Name:    CSSELSelectByChannel
     
    Description:
    	����service�ľ������ҵ��ѡ�񣬽����DVBҵ��
     
    Input:
    	hSelection : selection���
    	dwChannel : ��Ҫ���ŵ�ҵ������ServiceHandle
    	dwOption :	����ѡ�����ΪNULL��Ҳ����Ϊͷ�ļ���define��ѡ�� 
     
    Output:
     
    Return Value:
    	�ɹ���TRUE
    	ʧ�ܣ�FALSE
     
    Other:
    *******************************************************************************/
    BOOL CSSELSelectByChannel (HCSHANDLE hSelection,
                               DWORD dwChannel,
                               DWORD	dwOption);

    /******************************************************************************
    Function Name:    CSSELStop
     
    Description:
    	ֹͣCSSELSelectByLocator/CSSELSelectByLocator������ҵ��
     
    Input:
    	hSelection : selection��� 
     
    Output:
     
    Return Value:
    	�ɹ���TRUE
    	ʧ�ܣ�FALSE
     
    Other:
    *******************************************************************************/
    BOOL CSSELStop (HCSHANDLE hSelection, DWORD dwStopFlag);


    /******************************************************************************
    Function Name:    CSSELGetMeidaPlayer
     
    Description:
    	�����selection������MediaPlayer�ľ��
     
    Input:
    	hSelection : selection��� 
     
    Output:
    	phPlayerHandle��MediaPlayer���
     
    Return Value:
    	�ɹ���TRUE
    	ʧ�ܣ�FALSE
     
    Other:
    *******************************************************************************/
    BOOL CSSELGetMeidaPlayer (HCSHANDLE hSelection,
                              HCSHANDLE *phPlayerHandle);

    /******************************************************************************
    Function Name:    CSSELUnlock
     
    Description:
    	������Ŀ��������
     
    Input:
    	hSelection : selection��� 
     
    Output:
    	�ɹ���TRUE
    	ʧ�ܣ�FALSE
     
    Return Value:
     
    Other:
    *******************************************************************************/
    BOOL CSSELUnlock (HCSHANDLE hSelection);
    /******************************************************************************
    Function Name:    CSSELGetDevicePath
     
    Description:
    	����selection�ľ����ȡ����ʹ�õ�demux�豸(UDI1.0����demux�豸���,UDI2.0����demux�豸����)
     
    Input:
    	hSelection:selection���
    Output:
    	
    Return Value:
    	�ɹ�������decoder���
    	ʧ�ܣ�����0
    Other:
    *******************************************************************************/
    int CSSELGetDevicePath(HCSHANDLE hSelection);
    /******************************************************************************
    Function Name:    CSSELGetTunnerPath
     
    Description:
    	����selection�ľ����ȡ����ʹ�õ�tunner���,
     
    Input:
    	hSelection:selection���
    Output:
    	
    Return Value:
    	�ɹ�������tunner���
    	ʧ�ܣ�����0
    Other:
    *******************************************************************************/
    int CSSELGetTunnerPath(HCSHANDLE hSelection);
    /******************************************************************************
    Function Name:    CSSELGetUsedSelections
     
    Description:
    	��ȡ����ʹ�õ�selection,
     
    Input:
     
    Output:
     
     
    Return Value:
    	����һ��selection ���飬һ��ȡ����ĵ�һ��Ԫ��
    Other:
    *******************************************************************************/
    HCSHANDLE* CSSELGetUsedSelections(void);
    /******************************************************************************
    Function Name:    CSSELSelectByFrequency
     
    Description:
    	����Frequency��SymbolRate��Modulation��ServiceId����
     
    Input:
    	hSelection : selection��� 
    	pszLocator:  Ƶ�㡢�����ʡ����Ʒ�ʽ����Ŀ�Ź��ɵ�locator
    	��ʽ����������
    	1) dvb://Frequency.SymbolRate.Modulation.ServiceID
    	2) deliver://Frequency.SymbolRate.Modulation.ServiceID
    	3) dvb://Frequency:SymbolRate:Modulation:ServiceID
    	4) deliver://Frequency:SymbolRate:Modulation:ServiceID
		Frequency:Ƶ��.��λHz
		SymbolRate:������
		Modulation:����ģʽ.
		ServiceId:��ĿID
		��: deliver://490000000.6875.64.10

		��ע����������locator�г������־�Ϊ10����
    Output:
    	�ɹ���TRUE
    	ʧ�ܣ�FALSE
     
    Return Value:
     
    Other:
    *******************************************************************************/
    BOOL CSSELSelectByFrequency(HCSHANDLE hSelection, char *pszLocator);
    /******************************************************************************
    Function Name:    CSSELGetServiceHandle
     
    Description:
    	��ȡ��ǰ��Ŀ��service�����ֻ��Ӧ��DVB��Ŀ��
    Input:
    	hSelection : selection��� 
    Output:
     
    Return Value:
    	�ɹ�����0
    	ʧ�ܣ�0
     
    Other:
    *******************************************************************************/
    HCSHANDLE CSSELGetServiceHandle(HCSHANDLE hSelection);

    BOOL CSSELEnableCatMonitor(HCSHANDLE hSelection);

    BOOL CSSELDisableCatMonitor(HCSHANDLE hSelection);
    /******************************************************************************
    Function Name:    CSSELAddCallback
     
    Description:
    	��selectionע��һ��callback
    Input:
    	hSelection : selection��� 
    	callback:Ҫע���callback
    Output:
     
    Return Value:
    	�ɹ�����0
    	ʧ�ܣ�0
     
    Other:��ע����callback
    *******************************************************************************/
    BOOL CSSELAddCallback(HCSHANDLE hSelection, DWORD dwUserData, FOnCSSELCallback callback);
    /******************************************************************************
    Function Name:    CSSELRemoveCallback
     
    Description:
    	�Ƴ�һ��callback
    Input:
    	hSelection : selection��� 
    	callback:Ҫ�Ƴ���callback
    Output:
     
    Return Value:
    	�ɹ�����0
    	ʧ�ܣ�0
     
    Other:
    *******************************************************************************/

    BOOL CSSELRemoveCallback(HCSHANDLE hSelection, DWORD dwUserData, FOnCSSELCallback callback);

    /******************************************************************************
    Function Name:    CSSELGetCurrentLocator
     
    Description:
    	��ȡ��ǰ��Ŀ��url
    Input:
    	hSelection : selection��� 
    	nLen:pcLocator��ָ��Ĵ洢�ռ�Ĵ�С
    Output:
    	pcLocator:���ڴ�ŵ�ǰ��Ŀ��url.
    Return Value:
    	�ɹ�����0
    	ʧ�ܣ�0
     
    Other:
    *******************************************************************************/
    BOOL CSSELGetCurrentLocator(HCSHANDLE hSelection, char *pcLocator, int nLen);
    /**********************************************************************************************
    Function Name:    CSSELSetPMTMonitorPolicy
     
    Description:
    	����pmt��������
    Input:
    	hSelection : selection��� 
	    dwPeriod:�������ڣ���λms
	    dwCount:�������������ڵ㲥��Ŀpmt��Ϣһ�㶼����ʵʱ���£��������ڲ����Ѹ���Ŀ��ǰ�ڽ��м���
			��dwCount = 0xffffffffʱ��ʾһֱ������û�д�������
    Output:
    
    Return Value:
    	�ɹ�����0
    	ʧ�ܣ�0
     
    Other:
    Ŀ��:		����人�㲥��������
    ����ԭ��:	��Ϊipqam�����ݽ����˻��棬����Ȳ�һ��mpg2�Ľ�Ŀ�����Ų���h264�Ľ�Ŀ���պ�
    ���������Ŀ�ɱ����䵽ͬһƵ�㡢ͬһͨ���ϣ��ͻ�����ն���Ƶ�ɹ��󣬽���pmt����������
    ��Ƶ��ʽ��mpg2�Ľ�Ŀ��������ʵ��������h264��,pmt�İ汾����û�з����ı䣬�����ն˽���ʧ�ܡ�
    �������: �����汾�ŵ�����Ԥ��Ч��������ÿ��һ��ʱ����������pmt�Ĳ��ԡ�
    **********************************************************************************************/
    BOOL CSSELSetPMTMonitorPolicy(HCSHANDLE hSelection, DWORD dwPeriod, DWORD dwCount);
    /**********************************************************************************************
    Function Name:    CSSELSetPATMonitor
     
    Description:
    	�����Ƿ�Ҫ��pat���м�����
    Input:
    	hSelection : selection��� 
	bMonitor: TRUE����PAT,  FALSE������ PAT
    Output:
    
    Return Value:
    	�ɹ�����0
    	ʧ�ܣ�0
     
    Other:
    Ŀ��:		�����Ŀǰ��pat pmtһ�����ı�ʱ��ǰ��Ŀ����������ʾ��
    ��ע:		Ϊ�˲�Ӱ��������Ŀ�����ǰ�pat�������ɿ����õ�ѡ�Ĭ�ϻ���
    				�������ģ�����м�������������ô˽ӿڽ�������.
    **********************************************************************************************/
    BOOL CSSELSetPATMonitor(HCSHANDLE hSelection,BOOL bMonitor  );

	/**********************************************************************************************
    Function Name:    CSSELSetSiTableMonitor
     
    Description:
    	�����Ƿ�Ҫ��SI����صı���м�����
    Input:
    	hSelection : selection��� 
		dwOption :	��Ҫ�����ı�������ΪNULL������Ϊͷ�ļ���define��ѡ��,
					�������Ϊ: SEL_BAT|SEL_SDT����ʾ����BAT��SDT
    Output:
    
    Return Value:
    	�ɹ�����0
    	ʧ�ܣ�0
     
    Other:
    Ŀ��:		Ϊ����irdeto CA������һ��alarm��Ϣ����Ҫ����SDT�������
    ��ע:		Ϊ�˲�Ӱ������CA����Ŀ��������Ҫ�����ͼ���SDT����ע��CA�Ĺ���ʱ���������ǰѼ���
    			SI����ر����ɿ����õ�ѡ�Ĭ���ǲ������ģ�����м���ָ�������������ô˽ӿڽ������á�
    			Ŀǰ��ʵ�ּ���SDT���������CAע��
    **********************************************************************************************/
    BOOL CSSELSetSiTableMonitor(HCSHANDLE hSelection,DWORD	dwOption);



/******************************************************************************
Function Name:    CSSELRegisterEOCCallback

Description:
	 ��selectionģ��ע��EOC��غ���.

Input:
	hSelection: selection��� 
	fnEOCSwitchCallback: ע���EOC�ص�, ֪ͨNRC�⣬ STBҵ���л�

Output:
Return Value:
	 �ɹ�����TRUE��ʧ�ܷ���FALSE

Other:
	��Ҫ�����Ҫʹ��EOCģ�����Ŀ����Ҫ����ע����Ӧ�ӿڣ�Ĭ���ǲ�ע���
*******************************************************************************/
BOOL CSSELRegisterEOCCallback(HCSHANDLE hSelection,FOnCSEOCSwitch fnEOCSwitchCallback);

/**********************************************************************************************
Function Name:    CSSELSetServiceLockCtr
 
Description:
	���ò������Ƿ�Ҫ�Խ�Ŀ����������Ƶ����
Input:
	hSelection : selection��� 
	bServiceLockCtr: Ĭ��ֵΪTRUE,TRUE��ʾ�������Խ�Ŀ����������Ƶ���ƣ�FALSE��ʾ�����п���
Output:

Return Value:
	�ɹ���TRUE
	ʧ�ܣ�FALSE
 
Other:
Ŀ��:		�㶫��Ŀ���󣬲��������Խ�Ŀ����������Ƶ���ƣ������Ʒ���Ӧ����ʵ��
**********************************************************************************************/
BOOL CSSELSetServiceLockCtr(HCSHANDLE hSelection, BOOL bServiceLockCtr);	

/**********************************************************************************************
Function Name:    CSSELSetNgodSaFreq
 
Description:
	����NGOD˫����֤����Ƶ���������
Input:
	CSUDITunerCableDeliver_S : ˫����֤����Ƶ���б�
       nFreqCount: ˫�򻥶�Ƶ�����
Output:

Return Value:
	�ɹ���TRUE
	ʧ�ܣ�FALSE
 
**********************************************************************************************/

BOOL CSSELSetNgodSaFreq (CSUDITunerCableDeliver_S *pstTunerCableDeliver , int nFreqCount);

/**********************************************************************************************
    Function Name:    CSSELSetIsCHKServiceIdchanged
     
    Description:
    	����PAT��仯ʱ���Ƿ�ȥУ�����еĽ�ĿID�����˱仯
    Input:
    	hSelection : selection��� 
		bServiceIDChangeCHK �Ƿ�У��serviceID��PAT����ʱ�����˱仯��Ĭ��ֵΪFALSE
    Output:
    
    Return Value:
    	�ɹ���TRUE
    	ʧ�ܣ�FALSE
     
   Other:
    Ŀ��:		�����Ŀǰ��pat����ʱ��ʱ��ı�serviceID����ҪУ�����£�����ô˽ӿڽ�������.
**********************************************************************************************/
BOOL CSSELSetIsCHKServiceIdchanged(HCSHANDLE hSelection, BOOL bServiceIDChangeCHK );

/**********************************************************************************************
    Function Name:    CSSELSetStackSize
     
    Description:
    	����mediaplayer�̶߳�ջ��С��������CSSELCreate��������֮ǰ���øýӿڲ���Ч����Ҫ���ڽ�����䲥�����첽����ʱmediaplayer�̹߳�С����
    Input:
    	nStackSize ���õ��̶߳�ջ��С��һ��Ҫ���� 32 * 1024
    Output:
    
    Return Value:
    	none
**********************************************************************************************/
void CSSELSetStackSize(int nStackSize);
#ifdef __cplusplus
}
#endif

#endif
