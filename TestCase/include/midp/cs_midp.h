  /******************************************************************************
 FileName:  cs_midp.h
 Copyright (c) 2008�� coship��
 
 Version:       Ver 1.0
 Author:        ybq (yinbenqing@coship.com)
 
 File Description:
         Support MIDP Interface
 
 Modify History:
 Version     Date        Author          Description
 --------    ----------  --------        ------------
 1.0         2008-1203  ybq             Create
 
 *******************************************************************************/
 
#ifndef CSMIDP_H
#define CSMIDP_H
 
#ifdef __cplusplus
 extern "C" {
#endif
 
 
 /******************************************************************************
  *                                 Header File Include                         *
  ******************************************************************************/
#include "udiplus_typedef.h"
 
 /******************************************************************************
  *                                 Macro/Define/Structus                       *
  ******************************************************************************/
 /**
 * ����MIDP�˳��ļ�ֵ
 */
#define CSMIDPEXIT_KEYCODE 0xF1F2

 /**
 * ����Ϊmidp �淶�ж���ļ�
 */
typedef enum {
    MIDP_KEY_0        = '0',
    MIDP_KEY_1        = '1',
    MIDP_KEY_2        = '2',
    MIDP_KEY_3        = '3',
    MIDP_KEY_4        = '4',
    MIDP_KEY_5        = '5',
    MIDP_KEY_6        = '6',
    MIDP_KEY_7        = '7',
    MIDP_KEY_8        = '8',
    MIDP_KEY_9        = '9',
    MIDP_KEY_ASTERISK = '*',
    MIDP_KEY_POUND    = '#',

    MIDP_KEY_UP       = -1,
    MIDP_KEY_DOWN     = -2,
    MIDP_KEY_LEFT     = -3,
    MIDP_KEY_RIGHT    = -4,
    MIDP_KEY_SELECT   = -5, //��Ӧ�ֻ���ȷ�ϼ�

    MIDP_KEY_SOFT1    = -6, //��Ӧ�ֻ��������
    MIDP_KEY_SOFT2    = -7, //��Ӧ�ֻ��������
    MIDP_KEY_CLEAR    = -8, //��Ӧ�ֻ��Ϸ��ؼ�

    MIDP_KEY_POWER    = -12,  //��Ӧ�ֻ��Ϲһ������˼��ᵼ��midp ǿ���˳�������

    MIDP_KEY_INVALID  = 0 //��Ч��������ȡ��ע��ĳ������
    
} CSMidpKeyType_E;

 /*
 ����Ϊmidp ֧�ֵ�ѡ������
 �ر�ע�⣬�Ժ���չ����ʱ�ں�����ӣ����ɸı�ԭ������
 -----------------------------------------------------------------------------------------------
EM_MIDP_KEY_MODE: 
    wParam ���������ֵ��
    EM_MIDP_KEYMODE_UDI-��ʾ ���ⲿ����midp�ļ�ֵ (Ҳ���Ǳ��ؼ�ֵ)
                                        ֱ��͸����java��Ӧ�ã�������sunԼ���׳ɵļ�ֵ����
                                        ������Ŀ�Ż�����ʹ�ã���:topway��һ������£��벻Ҫ���ô˺���
    EM_MIDP_KEYMODE_DEFAULT-��ʾ��Ĭ�ϴ���
    lParam ��������
------------------------------------------------------------------------------------------------
EM_MIDP_FONT_SIZE:
    ���������С��
    wParam��CSMidpFontSize_E  �ж����ֵ
    lParam���������Ӧ��ʵ�ʴ�С������0ʱ�ָ�Ĭ�ϣ�
 -----------------------------------------------------------------------------------------------
EM_MIDP_STORAGE_HOME:
    ����MIDP���������ݴ洢����ĸ�Ŀ¼��
    wParam�����ʾ·�����ַ���
    ��:CSMidpSetOption(EM_MIDP_STORAGE_HOME, (void *)"/workdir/mnt/hd/HDD9/midpHome", NULL);
 -----------------------------------------------------------------------------------------------
 EM_MIDP_HEAP_SIZE:
    ����MIDP���ڴ�ش�С��
    wParam���ڴ�ش�С����λ�ֽ�
    ��:CSMidpSetOption(EM_MIDP_HEAP_SIZE, (void *)0x200000, NULL);
 -----------------------------------------------------------------------------------------------
 */
typedef enum {
    EM_MIDP_KEY_MODE = 1,    
    EM_MIDP_FONT_SIZE = 2,
    EM_MIDP_STORAGE_HOME = 3,
    EM_MIDP_HEAP_SIZE = 4
} CSMidpOption_E;

typedef enum {
    EM_MIDP_KEYMODE_DEFAULT = 0, 
    EM_MIDP_KEYMODE_UDI    
} CSMidpKeyMode_E;

 /**
 * ����Ϊmidp �����С
 */
typedef enum {
    EM_MIDP_FONT_SIZE_SMALL,    
    EM_MIDP_FONT_SIZE_MEDIUM,
    EM_MIDP_FONT_SIZE_LARGE    
} CSMidpFontSize_E;

 typedef struct _MidpInitParam
 {
     int nClientId;                  //GrOpen���ص�ClientID
     int nParentWinId;          //midp ���ڵĸ�����ID, С�ڵ���0ʱ����Ϊ�Ǹ�Window
     int nX;
     int nY; 
     int width;
     int height;                     //midp���ڵ�λ�úʹ�С,λ��������丸����
 
     int nEvtFromFlag;     // midpϵͳ�����¼��ķ�ʽ
                                      //0 -midp auto get event form microwin
                                     // 1 caller post event to midp by CSMIDPHandEvent
 }CSMIDPInitParams_S;

 typedef enum 
{
	EM_MIDP_DVB_WAIT = 1,
	EM_MIDP_DVB_READY,
	EM_MIDP_DVB_FAILE
}CSMidpDvbState_E;

/**MIDP��Ϸ״̬ö��*/
typedef enum {
	EM_MIDP_ENTER,        //����MIDP��Ϸ
	EM_MIDP_EXIT 		   //�˳�MIDP��Ϸ
} CSMIDPState_E;

typedef struct _MidpDvbProtocolReg
{
	int (*DvbInit)(void); 
	int (*DvbOpen)(const char * pszUrl, const char * pcMode, BOOL bNeedUpdate, DWORD dwUpdateInterval); //pszUrl: "dvb://207.2a.1069"; mode: "rb" ��bNeedUpdate-��ʾ�Ƿ���£�dwUpdateInterval-��ʾ�����µļ��ʱ��
	int (*DvbRecv)(int nIndex, void *pBuffer, unsigned int size);
	int (*DvbClose)(int nIndex); //return CSMidpDvbState_E
	int (*DvbSelect)(int nIndex, int nTimeout); //nTimeout:ms; return value:CSMidpDvbState_E
	int (*DvbAvailable)(int nIndex);
	int (*DvbDestroy)(void);
}CSMidpDvbProtocolReg_S;

typedef int (*CSPlatformRequest_F)(char *pcUrl);


/**
@brief ��ȡMIDP״̬�ص���������
@param[in] eType���CSMIDPState_E˵��
@param[in] pvUserData �û�����
@param[in] pMidpData ����MIDP��Ϸ�����ƺ�jar��URL����ʽ���£�"{midpname:value1,midpjar:value2}"
*/
typedef int (*CSMIDPStateCallback_F) (CSMIDPState_E eType, char* pMidpData, void* pvUserData);

/**
@brief ע���ȡMIDP��Ϸ״̬�Ļص���
@param[in] fnCallback ָ��ص������ĺ���ָ��
@param[in] pvUserData �û����ݣ�����ΪNULL
@note  ��������ע�����ص�
*/
BOOL CSMIDPAddStateCallback (CSMIDPStateCallback_F fnCallback ,void* pvUserData);

/**
@brief ɾ����ȡMIDP��Ϸ״̬�Ļص���
@param[in] fnCallback ָ��ص������ĺ���ָ��
*/
BOOL CSMIDPDelStateCallback (CSMIDPStateCallback_F fnCallback, void* pvUserData);

 /******************************************************************************
  * FUNCTION:    CSMIDPInit
  * 
  * OVERVIEW:
  *        ����pMIDPInitParams��ʼ��midpϵͳ��
  *         ����midp���ڣ�
  *         �����¼���Դ��
  *         ��ʼ��midp   ϵͳ�õ��Ĳ������磬gc��font�ȡ�
  *   
  * Input:
  *     pMIDPInitParams:��ʼ�����������CSMIDPInitParams_S ����
  * 
  * Output:
  *     ��
  *
  * Return Value:
  *     midp���ڵ�windowID
  * 
  *Other:
  *     ������midpǰ����
  * *****************************************************************************/
  
  int CSMIDPInit(CSMIDPInitParams_S *pMIDPInitParams);
  
  
  /*=========================================================================
  * FUNCTION:      CSMIDPStart
  * OVERVIEW:      The main method. There are three modes that MIDP can
  *                be run:
  *
  *                   - TCK       driven by loading descriptors from
  *                                a url
  *                   - debug     classpath and MIDlet class on the
  *                                command line
  *                   - resident  install, remove, list, run apps
  * Input:
  *   parameters:  argc:  The total number of arguments
  *                argv:  An array of 'C' strings containing the arguments
  *   Return Value:
  *          int:   The status of the last VM invocation.
  *=======================================================================*/
 
 int CSMIDPStart(int argc, char* argv[]);
 
 /*=========================================================================
  * FUNCTION:      CSMIDPHandEvent
  * OVERVIEW:      ��������˵����ⷢ��Ϣ��midp����ͨ���˽ӿڷ�����Ϣ��midp
                            �����˳�MIDP�ĵ���,CSMIDPEXIT_KEYCODE�����ֵͬ���˳�MIDP
  * 
  *  
  * Input
  *   parameters:  
  *                   dwKeyType: �������ͣ�GR_EVENT_TYPE_KEY_UP = 9  GR_EVENT_TYPE_KEY_DOWN = 8
  *                   dwKeyCode  ��Ӧ��ֵ,����microwin��GTools�ļ�ֵ��CSMIDPEXIT_KEYCODEʹMIDP�˳�
  *                  
  *   returns:     
  *              �ɹ�Ϊ0��ʧ�ܷ���-1����������-2
  *=======================================================================*/
 
 int CSMIDPHandEvent(DWORD dwKeyType, DWORD dwKeyCode);
 
  /*=========================================================================
  * FUNCTION:      CSMIDPGetStatus
  * OVERVIEW:     �õ�KVM+MIDP������״̬
  * 
  *  
  * Input
  *   parameters:  
  *                  ��
  *                  
  *   returns:     
  *            1: ����״̬
  *            0: δ����״̬
  *=======================================================================*/
 
 int CSMIDPGetStatus();

/*=========================================================================
 * FUNCTION:      CSMIDPExit
 * OVERVIEW:      �˳�KVM+MIDPӦ�ã�һ������²�ʹ�ã�Ӧ�ñ����˳�����������˳�
 *                      ����ǿ���˳�KVMʱʹ��
 * 
 *  
 * Input
 *   parameters:  
 *          ��
 *                  
 *   returns:     
 *              �ɹ�Ϊ0����������ֵ
 *=======================================================================*/

int CSMIDPExit();


  /********************************************************************************
 Function Name:    CSMIDPGetVersion
 
 Description:
         Function Description.  ��ȡ��ǰMIDP �İ汾��
 
 Input:
         int   nSize       ����汾�Ŵ洢����                                                     
 
 Output:
        char*  pcVersion   MIDP �İ汾�� 
 
 Return Value:
     0 : ���ذ汾�ų���;
     -1 : ����Ĭ�ϳ���;
 
 Other:
 *********************************************************************************/
 int CSMIDPGetVersion( char * pcVersion, int nSize );

  /********************************************************************************
 Function Name:    CSMidpRegDvbProtocol
 
 Description:
         Function Description. 
         �ѹ�ʱ��Ϊ��������Ŀ����
         ע��dvb Э��������OC���ݽ��սӿ�
         ע��ʹ�øýӿ�Ĭ����dvb://��ΪЭ��ͷ
 
 Input:
         CSMidpDvbProtocolReg_S *pDvbProtocol      dvbЭ��ע�ắ���б�                                 
 
 Output:
        �� 
 
 Return Value:
     �ɹ�Ϊ0����������ֵ
 
 Other:
 *********************************************************************************/
int CSMidpRegDvbProtocol(CSMidpDvbProtocolReg_S *pDvbProtocol);

/********************************************************************************
 Function Name:    CSMidpRegUserProtocol
 
 Description:
         Function Description. ע���û�����ĸ���Э�����������ݽ��սӿ�
 
 Input:
  	   char *pUserProtocolHead  			    ע���Э��ͷ,Ŀǰֻ֧��dvb://��pushfile://����
         CSMidpDvbProtocolReg_S *pDvbProtocol      dvbЭ��ע�ắ���б�                                 
 
 Output:
        �� 
 
 Return Value:
     �ɹ�Ϊ0����������ֵ
 
 Other:
 *********************************************************************************/
int CSMidpRegUserProtocol(const char *pUserProtocolHead, CSMidpDvbProtocolReg_S *fnUserCallBack);

 /********************************************************************************
 Function Name:    CSMidpRegPlatformRequest
 
 Description:
         Function Description. ע��platformRequest�ص���������midpӦ����
         ����MIDlet.resumeRequest ʱ��midp��url͸������������û�
         ��������δ���
 
 Input:
         fnCSPlatformRequest fun      ����platformRequest url�Ļص�����    
         BOOL bExitMidp         midp�׳�url֮���Ƿ������˳�,Ĭ��Ϊ�˳�
         void *pUserData        �û�˽�в����������Ժ�ʹ��
         
 Output:
        �� 
 
 Return Value:
     �ɹ�Ϊ0����������ֵ
 
 Other:
 *********************************************************************************/
int CSMidpRegPlatformRequest(CSPlatformRequest_F fnCallback, BOOL bExit, void *pUserData);

 /********************************************************************************
 Function Name:    CSMidpRegKeyMap
 
 Description:
         Function Description. ע��midp����ļ�ֵӳ�䣬�û�ע��󽫸ı�midp����ص�Ĭ�ϼ�ֵӳ�䣻
				�û�ע���midp���ȴ��û�ע���ӳ����ƥ���ֵ��
				�����Ҫȡ��֮ǰע��ļ�ֵ��nMidpKey ע��ΪMIDP_KEY_INVALID ���ɣ�
 
 Input:
         int nCoshipUdiKey      ��Ӧcoship��udi����ļ�ֵ    
         CSMidpKeyType_E nMidpKey           ��Ӧmidp�淶�еļ�ֵ
         
 Output:
        �� 
 
 Return Value:
     �ɹ�Ϊ0����������ֵ
 
 Other:
 *********************************************************************************/
int CSMidpRegKeyMap(int nCoshipUdiKey, CSMidpKeyType_E nMidpKey);

/********************************************************************************
 Function Name:    CSMidpSetOption
 
 Description:
         Function Description. ������������
         
 Input:
         ��
         
 Output:
        �� 
 
 Return Value:
     �ɹ�Ϊ0����������ֵ
 
 Other:
 *********************************************************************************/
int CSMidpSetOption(CSMidpOption_E eOption, void *wParam, void *lParam);


/********************************************************************************
 Function Name:    CSMidpPause
 
 Description:
         Function Description. ������ͣmidp�߳�,ͬʱ�ᱸ�ݻ�����Դ棬
         resumeʱ�ָ�
         
 Input:
         nX			���ݾ�����������x����
         nY			���ݾ�����������y����
         nW		���ݾ�������Ŀ��
         nH		       ���ݾ�������ĸ߶�
         ��������ȫ0�Ļ��ᱸ��ȫ����ȱ�����˷��ڴ�
 Output:
        �� 
 
 Return Value:
     �ɹ�Ϊ0����������ֵ
 
 Other:
 *********************************************************************************/
int  CSMidpPause(int nX, int nY, int nW, int nH);

/********************************************************************************
 Function Name:    CSMidpResume
 
 Description:
         Function Description. ���ڻָ�midp�̣߳�ͬʱ��ָ�������Դ棬
         �����CSMidpPause���ʹ��
         
 Input:
         ��
         
 Output:
        �� 
 
 Return Value:
     �ɹ�Ϊ0����������ֵ
 
 Other:
 *********************************************************************************/
int  CSMidpResume(void);

/********************************************************************************
 Function Name:    CSMidpSetOption
 
 Description:
         Function Description. ����midp������Ƶ�ӿڳ�ʼ��,ע�Ქ����Ҫ�Ľӿ�,�����
         	mediaplayer����Ϲ�ϵ�����������ʾMIDP��֧����Ƶ���š�
         
 Input:
         ��
         
 Output:
        �� 
 
 Return Value:
     �ɹ�Ϊ0����������ֵ
 
 Other:
 *********************************************************************************/
int CSMidpVideoInit(void);
  
#ifdef __cplusplus
 }
#endif
#endif
 

