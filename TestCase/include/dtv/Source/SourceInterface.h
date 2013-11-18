#ifndef SourceInterface_H
#define SourceInterface_H

#ifdef PRAGMA
#pragma interface "SourceInterface.h"
#endif

/* {{{USR

   }}}USR */

#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h" 

// {{{USR

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct _SourceSatelliteDeliver
{
	SatelliteDeliver m_SatelliteDeliver;  //���ǻ�Ƶ����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
    LNB m_LNB;                            //��Ƶͷ��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	DiSEqC1020 m_DiSEqC1020;              //diseq1.0��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
    DiSEqC1121 m_DiSEqC1121;              //diseq1.1��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	DiSEqC1222 m_DiSEqC1222;              //diseq1.2��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	SatellitePar m_SatellitePar;          //���ǲ�����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	AntennaPar m_AntennaPar;              //���߲�����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
    TransponderOffset m_TransponderOffset;//Ƶ��ƫ��ֵ��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
} SourceSatelliteDeliver;

typedef union _SourceDeliver
{
	CableDeliver m_CableDeliver;                        //���߻�Ƶ����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	SourceSatelliteDeliver m_SourceSatelliteDeliver;    //�����涨��
	TerrestrialDeliver m_TerrestrialDeliver;            //�����Ƶ����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
}SourceDeliver;

typedef struct _SourceSatelliteDeliverExt
{
	SatelliteDeliverExt m_SatelliteDeliver;  //���ǻ�Ƶ����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
    LNB m_LNB;                            //��Ƶͷ��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	DiSEqC1020 m_DiSEqC1020;              //diseq1.0��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
    DiSEqC1121 m_DiSEqC1121;              //diseq1.1��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	DiSEqC1222 m_DiSEqC1222;              //diseq1.2��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	SatellitePar m_SatellitePar;          //���ǲ�����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	AntennaPar m_AntennaPar;              //���߲�����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
    TransponderOffset m_TransponderOffset;//Ƶ��ƫ��ֵ��Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
} SourceSatelliteDeliverExt;

typedef union _SourceDeliverExt
{
	CableDeliver m_CableDeliver;                        //���߻�Ƶ����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
	SourceSatelliteDeliverExt m_SourceSatelliteDeliver;    //�����涨��
	TerrestrialDeliver m_TerrestrialDeliver;            //�����Ƶ����Ϣ�ṹ�壬���DataManager��CSUSPDMCommonType.hͷ�ļ���˵����
}SourceDeliverExt;

//����Ľṹ��HDI�Ľṹ��CSHDICallbackType����
typedef enum
{
	CSOURCE_CALLBACK_IR,			// callback from cs_ir
	CSSOURCE_CALLBACK_PANEL,		// callback from cs_panel
	CSSOURCE_CALLBACK_QPSK,		    // callback from cs_qpsk
	CSSOURCE_CALLBACK_COFDM,	    // callback from cs_cofdm
	CSSOURCE_CALLBACK_QAM		    // callback from cs_qam
} CSSourceCallbackType;

typedef enum
{
 SOURCE_SIGNAL_CONNECTED,           //Ƶ�������ɹ�
 SOURCE_SIGNAL_DROPPED,             //��Ƶ�������ɹ�תΪ����ʧ��
 SOURCE_SIGNAL_LOST,                //Ƶ������ʧ��
 SOURCE_SIGNAL_SEARCHING,           //��������Ƶ��
 SOURCE_SIGNAL_NOSIGNAL				//���ź�
}SourceCallbackEvents;

//�ص���Ϣ���Ͷ���
typedef struct _SourceCallbackMessage
{
 SourceCallbackEvents m_SourceCallbackEvent;    //��ǰ�涨��
 DWORD dwFrequency;                             //�ص���Ϣ��Ӧ��Ƶ�㣬��λKHZ
 DWORD dwSymbolRate;                            //�ص���Ϣ��Ӧ�ķ����ʣ���λKbps
 DWORD dwBandWidth;                             //�ص���Ϣ��Ӧ�Ĳ��Σ�ʵ�ʶ�ӦUDI�е�CSUDITunerBanuidth_Eö�����ͣ�ֵ���壺0----8M, 1----7M, 2----6M��
 DWORD dwQAMMode;                               //�ص���Ϣ��Ӧ�ĵ��Ʒ�ʽ��ʵ�ʶ�ӦUDI�е�CSUDITunerQAMMode_Eö�����ͣ�ֵ���壺0----Not Define, 1----16QAM, 2----32QAM, 3----64QAM, 4----128QAM, 5----256QAM��
 DWORD dwTunerIndex;                            //�ص���Ϣ��Ӧ��Tuner������
 void *pvUserData;                              //�û���ע��ص�����ʱ�����ָ��
} SourceCallbackMessage;

typedef  void ( *CSUSPSourceCallback )( CSSourceCallbackType nSourceCallbackType, SourceCallbackMessage* pSourceCallbackMessage);

// }}}USR

/* {{{USR
CreateFileSource
    �����ļ���������Ƶ��Source���(δʵ�־�������)��

	 HCSHANDLE CreateFileSource( void );

Parameters
       ��
Return Values
		1������ɹ�����������Ƶ��Source���
		2�����ʧ�ܷ���ΪNULL

   }}}USR */
HCSHANDLE CreateFileSource( void );

/* {{{USR
CreateHDISatelliteSource
    ���������ǵ�������Ƶ��Source�����(���HDI����)

	 HCSHANDLE CreateHDISatelliteSource( HCSHANDLE hDevice );

Parameters
       hDevice
       [in]Tuner��������
Return Values
		1������ɹ�����������Ƶ��Source���
		2�����ʧ�ܷ���ΪNULL

   }}}USR */
HCSHANDLE CreateHDISatelliteSource( HCSHANDLE hDevice );

/* {{{USR
CreateHDICableSource
    ���������ߵ�������Ƶ��Source�����(���HDI����)

	 HCSHANDLE CreateHDICableSource( HCSHANDLE hDevice );

Parameters
       hDevice
       [in]Tuner��������
Return Values
		1������ɹ�����������Ƶ��Source���
		2�����ʧ�ܷ���ΪNULL

   }}}USR */
HCSHANDLE CreateHDICableSource( HCSHANDLE hDevice );

/* {{{USR
CreateHDITerrestriaSource
    ����������������Ƶ��Source�����(���HDI����)

	 HCSHANDLE CreateHDITerrestriaSource( HCSHANDLE hDevice );

Parameters
       hDevice
       [in]Tuner��������
Return Values
		1������ɹ�����������Ƶ��Source���
		2�����ʧ�ܷ���ΪNULL

   }}}USR */
HCSHANDLE CreateHDITerrestriaSource( HCSHANDLE hDevice );

/* {{{USR
CloseSource
    �ͷ�������Ƶ��Source�����Դ��

	 BOOL CloseSource( HCSHANDLE hSource );

Parameters
       hSource 
       [in]������Ƶ��Source���
Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

   }}}USR */
BOOL CloseSource( HCSHANDLE hSource );

/* {{{USR
GetSignalQuality
    ��ȡTuner���ź�������

	 int GetSignalQuality( HCSHANDLE hSource );

Parameters
       hSource 
       [in]������Ƶ��Source���
Return Values
		�����ź�����

   }}}USR */
int GetSignalQuality( HCSHANDLE hSource );

/* {{{USR
GetSignalStrength
    ��ȡTuner���ź�ǿ�ȡ�

	 int GetSignalStrength( HCSHANDLE hSource );

Parameters
       hSource 
       [in]������Ƶ��Source���
Return Values
		�����ź�ǿ��
   }}}USR */
int GetSignalStrength( HCSHANDLE hSource );

/* {{{USR
GetErrorRate
    ��ȡTuner�Ĵ����ʡ�

	 int GetErrorRate( HCSHANDLE hSource );

Parameters
       hSource 
       [in]������Ƶ��Source���
Return Values
		���ش�����
   }}}USR */
int GetErrorRate( HCSHANDLE hSource );

/* {{{USR
ConnectSource
    ����ָ����Ƶ�������Ƶ������

	 BOOL ConnectSource( HCSHANDLE hSource, Deliver * pDeliver );

Parameters
       hSource 
       [in]������Ƶ��Source���
       pDeliver 
       [in]�û����Ƶ����Ϣ�����ݿ�ĵ�ַ
Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

   }}}USR */
BOOL ConnectSource( HCSHANDLE hSource, const SourceDeliver * pSourceDeliver );

/* {{{USR
ConnectSourceFromDM

   ������ָ����Ƶ�������Ƶ������	 ��

     BOOL ConnectSourceFromDM( HCSHANDLE hSource, HCSHANDLE hTransponder, HCSHANDLE hDataContainer);

Parameters
       hSource 
       [in]������Ƶ��Source���
       hTransponder
       [in] Ƶ������Ҳ���������ݹ����еĴ�ŵ�ַ��
       hDataContainer
       [in] ���������ľ��

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

   }}}USR */
BOOL ConnectSourceFromDM( HCSHANDLE hSource, HCSHANDLE hTransponder, HCSHANDLE hDataContainer );

/* {{{USR
CSUSPSourceGetVersionInfo
    ��ȡģ��汾��Ϣ�� �����汾�š�����޶����ڡ��û�����ѡ���Եص��ø÷�����ȡUSP��������ģ��İ汾��Ϣ��

	 int CSUSPSourceGetVersionInfo( char * pcVersionInfo, int nSize );;

Parameters
       pcVersionInfo
       [out]��Ű汾��Ϣ�Ļ������׵�ַ���������,��Ű汾��Ϣ����ʽ���£�
            ��Module Name: EPG\nCurrent Version: 1.2\nRevision Date: 2005-5-11��
       nSize
       [in]    �������ĳ���

Return Values
		1)	�����һ������pcVersionInfoΪ�գ����ذ汾��Ϣ��ʵ�ʳ��ȡ������û�����ڶ�������Ϊ���汾��Ϣ��ʵ�ʳ���+       
1�� 
       2)	�����һ������pcVersionInfo��Ϊ�գ��Һ������óɹ������ذ汾��Ϣ��ʵ�ʳ��ȣ�
       3)	���ʧ�ܣ�����0��

       ���õ�һ�ַ���ֵ��������û��ɲ��Ըú�������Ϊ�ڶ�������nSize�ṩ�ο���
       ����û�����ĵڶ�������С�ڰ汾��Ϣ��ʵ�ʳ��ȣ�β��������ֽڱ��ص���
   }}}USR */
int CSUSPSourceGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
CSUSPSourceGetMemInfo
    ��ȡģ��Ҫ����ڴ�ռ����Сֵ�����ֵ������ģ��Ҫ��һ���̶��ռ�ʱ��*pnMin ��*pnMax��ȣ��е�ģ��������Ҫ*pnMin bytes�����������У��ڷ���*pnMax�ռ�ʱ���ɻ�ø��õ����ܡ�

	 BOOL CSUSPSourceGetMemInfo( int * pnMin, int * pnMax );;

Parameters
       pnMin
       [out]ģ��������С�ڴ� (in bytes)
       pnMax
       [out]ģ����������ڴ�(in bytes)

Return Values
       1)�ɹ�������TRUE;
       2)ʧ�ܣ����� FALSE

Remarks
       1��	��ģ��Ҫ��̶��ڴ�ʱ�����ص�����ڴ����С�ڴ���ȡ���
       2��	��ģ��Ҫ����С�ڴ�Ϊ*pnMin�� ����*pnMin ��*pnMax֮��ɻ�ø��õ����ܻ���๦��ʱ������ڴ����С�ڴ治�ȡ�
       3��	��pnMin =NULL �� pnMax = NULL����������ʧ�ܡ�
       4�� �÷������ڴ�ĺ���ʹ�ã�Ӳ�������þ��вο���ֵ��    

   }}}USR */
BOOL CSUSPSourceGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR
CSUSPSourceAddCallback
     ע��ָ���Ļص�������

     BOOL CSUSPSourceAddCallback(HCSHANDLE hSource, CSUSPSourceCallback callback, void * pData);
  ��
Parameters
       hSource 
       [in]������Ƶ��Source���
       callback
       [in]�ص��������
       pData
       [in]�û�������Զ���ָ��

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

Remarks
       1)�ص����������ע�����Ϊ20��������20��������FALSE
       2)ע��ʱ����tuner����callbackΪ��ֵ��

   }}}USR */
BOOL CSUSPSourceAddCallback( HCSHANDLE hSource, CSUSPSourceCallback callback, void * pData );

/* {{{USR
CSUSPSourceDelCallback
     ɾ��ָ���Ļص�������

     BOOL CSUSPSourceDelCallback( HCSHANDLE hSource, CSUSPSourceCallback callback );

Parameters
       hSource 
       [in]������Ƶ��Source���
       callback
       [in]�ص��������

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

Remarks
      1)����ûص�����������û��ע�ᣬ������FALSE

   }}}USR */
BOOL CSUSPSourceDelCallback( HCSHANDLE hSource, CSUSPSourceCallback callback );

/* {{{USR
CSUSPSourceSetPosition
       �����û����ڵľ�����γ�ȡ�

       BOOL CSUSPSourceSetPosition (DWORD dLongitude, DWORD dLatitude);
Parameters
       dLongitude
       [in]����ֵ
       dLatitude
       [in]γ��ֵ

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

   }}}USR */
BOOL CSUSPSourceSetPosition( double dLongitude, double dLatitude );

/* {{{USR
CSUSPSourceGetPosition

       ��ȡ�û����ڵľ�����γ�ȡ�

Parameters
       pdOutLongitude
       [out]����ֵ������ΪNULL�����򷵻�ʧ��
       pdOutLatitude
       [out]γ��ֵ������ΪNULL�����򷵻�ʧ��

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

   }}}USR */
BOOL CSUSPSourceGetPosition( double * pdOutLongitude, double * pdOutLatitude );


/* {{{USR
CSUSPSourceSetS2Flag
       ����Source������������Ƶ�ӿڡ�

Parameters
       hSatSource
       [in]������Ƶ��Source��������������ǵ�
       hContainerHandle
       [in]DM�������
       bFlag
       [in]��Ҫ���¹��ܣ�S2����option����Ҫ����TRUE

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���FALSE

   }}}USR */
BOOL CSUSPSourceSetS2Flag( HCSHANDLE hSatSource, HCSHANDLE hContainerHandle, BOOL bFlag );

BOOL CSUSPSourceSetSearchingCount( HCSHANDLE hSource, int nCount );

BOOL CSUSPSourceSetTunerDemuxPath(HCSHANDLE hSource, HCSHANDLE hPath);

// {{{USR
#ifdef __cplusplus
}
#endif

// }}}USR

#endif /* SourceInterface_H */
