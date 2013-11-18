#ifndef _CSUSPDMINTERFACE_H_
#define _CSUSPDMINTERFACE_H_

#include "CSUSPDMCommonType.h"

#define USPDM_ANTSATRELATION_HANDLE		6
#define USPDM_SERVICE_HANDLE			5
#define USPDM_C_TRANSPONDER_HANDLE		4
#define USPDM_S_TRANSPONDER_HANDLE		3
#define USPDM_T_TRANSPONDER_HANDLE		2
#define USPDM_SATELLITE_HANDLE			1
#define USPDM_ANTENNA_HANDLE			0

#define USPDM_C_CONTAINER		(0x01)   //���߻�
#define USPDM_T_CONTAINER		(0x02)   //�����
#define USPDM_S_CONTAINER		(0x04)   //���ǻ�

#define CABLE_SERVICE					(0x00) //���߽�Ŀ
#define TERRESTRIA_SERVICE				(0x01) //�����Ŀ
#define SATELLITE_SERVICE				(0x02) //���ǽ�Ŀ
#define IP_SERVICE						(0x03) //IP��Ŀ

#define USPDM_TERRESTREA_DELIVER		(0x02) //����Deliver
#define USPDM_SATELLITE_DELIVER			(0x03) //����Deliver
#define USPDM_CABLE_DELIVER				(0x04) //����Deliver
#define USPDM_VIRTUAL_DELIVER			(0x00) // ����Deliver

#define USPDM_UNKNOWN_HANDLE			-1 

#ifdef  __cplusplus
extern "C" {
#endif

//��ȡ���ݾ����IDֵ
DWORD CSUSPDMGetDataHandleID( HCSHANDLE hData );
//��ȡ���ݾ�����ͣ����庬����μ�����ĺ꣩
int CSUSPDMGetDataHandleType( HCSHANDLE hData );

//������������
BOOL CSUSPDMSetSatelliteName( const char * pstrName, int nDataLen, HCSHANDLE hSatelliteDataHandle );
//��ȡ��������
int CSUSPDMGetSatelliteName( PBYTE pbBuff, int nBuffLen, HCSHANDLE hSatelliteDataHandle );
//�趨���ǵ�DiSEpC1.2����
BOOL CSUSPDMSetDiSEqC1222( DiSEqC1222 * pDiSEqC1222, HCSHANDLE hSatelliteDataHandle );
//��ȡ���ǵ�DiSEpC1.2����
BOOL CSUSPDMGetDiSEqC1222( DiSEqC1222 * pDiSEqC1222, HCSHANDLE hSatelliteDataHandle );
//�������ǲ���
BOOL CSUSPDMSetSatellitePar( SatellitePar * pSatellitePar, HCSHANDLE hSatelliteDataHandle );
//��ȡ���ǲ���
BOOL CSUSPDMGetSatellitePar( SatellitePar * pSatellitePar, HCSHANDLE hSatelliteDataHandle );

BOOL CSUSPDMSetSatelliteNameEx( const char * pstrName, int nDataLen, CSUSPDMCodingType enmCodingType, HCSHANDLE hDataHandle );
int CSUSPDMGetSatelliteNameEx( PBYTE pbBuff, int nBuffLen, CSUSPDMCodingType * penmCodingType, HCSHANDLE hDataHandle );


//��ȡ���ߵ�LNB����
BOOL CSUSPDMGetLNB( LNB * pLNB, HCSHANDLE hAntennaDataHandle );
//�������ߵ�LNB����
BOOL CSUSPDMSetLNB( LNB * pLNB, HCSHANDLE hAntennaDataHandle );
//��ȡ���ߵ�DiSEpC1.0����
BOOL CSUSPDMGetDiSEqC1020( DiSEqC1020 * pDiSEqC1020, HCSHANDLE hAntennaDataHandle );
//�������ߵ�DiSEpC1.0����
BOOL CSUSPDMSetDiSEqC1020( DiSEqC1020 * pDiSEqC1020, HCSHANDLE hAntennaDataHandle );
//��ȡ���ߵ�DiSEpC1.1����
BOOL CSUSPDMGetDiSEqC1121( DiSEqC1121 * pDiSEqC1121, HCSHANDLE hAntennaDataHandle );
//�������ߵ�DiSEpC1.1����
BOOL CSUSPDMSetDiSEqC1121( DiSEqC1121 * pDiSEqC1121, HCSHANDLE hAntennaDataHandle );
//��ȡ���߲���
BOOL CSUSPDMGetAntennaPar( AntennaPar * pAntennaPar, HCSHANDLE hAntennaDataHandle );
//�������߲���
BOOL CSUSPDMSetAntennaPar( AntennaPar * pAntennaPar, HCSHANDLE hAntennaDataHandle );

//��ȡ����ת������Ƶ����Ϣ
BOOL CSUSPDMGetSatelliteDeliver( SatelliteDeliver * pSatelliteDeliver, HCSHANDLE hTransponderSDataHandle );
//��������ת������Ƶ����Ϣ
BOOL CSUSPDMSetSatelliteDeliver( SatelliteDeliver * pSatelliteDeliver, HCSHANDLE hTransponderSDataHandle );
//��������ת��������������ID
BOOL CSUSPDMSetAntennaID( DWORD dwAntennaID, HCSHANDLE hTransponderSDataHandle );
//��������ת��������������ID
BOOL CSUSPDMSetSatelliteID( DWORD dwSatelliteID, HCSHANDLE hTransponderSDataHandle );
//��ȡ����ת��������������ID
DWORD CSUSPDMGetSatelliteID( HCSHANDLE hTransponderSDataHandle );
//��ȡ����ת��������������ID
DWORD CSUSPDMGetAntennaID( HCSHANDLE hTransponderSDataHandle );
//��ȡ����ת�������������Ǿ��
HCSHANDLE CSUSPDMGetSatelliteHandleByTransponder( HCSHANDLE hTransponderSDataHandle, HCSHANDLE hContainer );
//��ȡ����ת�������������߾��
HCSHANDLE CSUSPDMGetAntennaHandleByTrnasponder( HCSHANDLE hTransponderSDataHandle, HCSHANDLE hContainer );

//��ȡ����Ƶ�����( Ƶ�㣬�����ʣ����Ʒ�ʽ )
BOOL CSUSPDMGetCableDeliver( CableDeliver * pCableDeliver, HCSHANDLE hTransponderCDataHandle );
//��������Ƶ�����( Ƶ�㣬�����ʣ����Ʒ�ʽ )
BOOL CSUSPDMSetCableDeliver( CableDeliver * pCableDeliver, HCSHANDLE hTransponderCDataHandle );
//��ȡ����Ƶ���ƫ�Ʋ���
BOOL CSUSPDMGetTransponderCOffset( TransponderOffset * pTransponderOffset, HCSHANDLE hTransponderCDataHandle );
//��������Ƶ���ƫ�Ʋ���
BOOL CSUSPDMSetTransponderCOffset( TransponderOffset * pTransponderOffset, HCSHANDLE hTransponderCDataHandle );

//��ȡ����Ƶ�����
BOOL CSUSPDMGetTerrestrialDeliver( TerrestrialDeliver * pTerrestrialDeliver, HCSHANDLE hTransponderTDataHandle );
//���õ���Ƶ�����
BOOL CSUSPDMSetTerrestrialDeliver( TerrestrialDeliver * pTerrestrialDeliver, HCSHANDLE hTransponderTDataHandle );

//��ȡƵ���IDֵ
BOOL CSUSPDMGetTransponderID( DWORD * pdwTransponderHandleID, HCSHANDLE hTransponderDataHandle );
//��ȡƵ���SourcdID������TSID��OriginalNetworkID��NetworkID��
BOOL CSUSPDMGetSourceID( SourceID * pSourceID, HCSHANDLE hTransponderDataHandle );
//����Ƶ���SourcdID������TSID��OriginalNetworkID��NetworkID��
BOOL CSUSPDMSetSourceID( SourceID * pSourceID, HCSHANDLE hTransponderDataHandle );
//����Ƶ���Deliver����
BOOL CSUSPDMSetDeliver( Deliver * pDeliver, HCSHANDLE hTransponderDataHandle );
//����Ƶ���ƫ�Ʋ���
BOOL CSUSPDMSetOffset( TransponderOffset * pTransponderOffset, HCSHANDLE hTransponderDataHandle );
//��ȡƵ���ƫ�Ʋ���
BOOL CSUSPDMGetOffset( TransponderOffset * pTransponderOffset, HCSHANDLE hTransponderDataHandle );

//��ȡ������ָ��Deliverֵ��Ƶ����
HCSHANDLE CSUSPDMGetTransponderHandle( Deliver * pDeliver, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMGetTransponderHandleExt( DeliverExt * pDeliverExt, HCSHANDLE hDataContainerHandle );
//��ȡ����������ָ��Deliverֵ������ID��AntennaID Ƶ����
HCSHANDLE CSUSPDMGetTransponderHandleEx( Deliver * pDeliver, DWORD dwSatID, DWORD dwAntID, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMGetTransponderHandleForSatExt( DeliverExt * pDeliverExt, DWORD dwSatID, DWORD dwAntID, HCSHANDLE hDataContainerHandle );

//��ȡ��Ŀ��CA����
int CSUSPDMGetFreeCAMode( HCSHANDLE hServiceDataHandle );
//�趨��Ŀ��CA����
BOOL CSUSPDMSetFreeCAMode( int nFreeCAMode, HCSHANDLE hServiceDataHandle );
//��Service���ݾ���л�ȡService_id
BOOL CSUSPDMGetService_id( int * pnServiceID, HCSHANDLE hServiceDataHandle );
//����Service���ݾ����Service_id
BOOL CSUSPDMSetService_id( int nServiceID, HCSHANDLE hServiceDataHandle );
//��Service���ݾ���л�ȡ�û��Զ������ݣ�δʵ�֣�
int CSUSPDMGetReUseData( PBYTE pbData, int nBufLen, HCSHANDLE hServiceDataHandle );
//����Service���ݾ���е��û��Զ������ݣ�δʵ�֣�
BOOL CSUSPDMSetReUseData( PBYTE pbReUseData, int nDataLen, HCSHANDLE hServiceDataHandle );

//����˵��������������������
//ServicePID�ṹ����ֻ�������ֶ���Ч��
//	m_dwPMTPID_13��m_dwPCRPID_13��m_dwVideoPID_13��m_dwAudioPID_13
//�����ֶ�
//	m_dwSubtitle_PID_Index_6��m_dwAudio_PID_Index_6��m_dwTeletext_PID_Index_6ͨ���ض������ӿڷ���
//�����ֶ���Ч
//��Service���ݾ���л�ȡServicePID(����service������PID��Ϣ)
BOOL CSUSPDMGetServicePID( ServicePID * pServicePID, HCSHANDLE hServiceDataHandle );
//�趨Service���ݾ���е�ServicePID
BOOL CSUSPDMSetServicePID( ServicePID * pServicePID, HCSHANDLE hServiceDataHandle );

//��Service���ݾ���л�ȡSERVICEPAR
BOOL CSUSPDMGetSERVICEPAR( SERVICEPAR * pServicePar, HCSHANDLE hServiceDataHandle );
//�趨Service���ݾ���е�SERVICEPAR
BOOL CSUSPDMSetSERVICEPAR( SERVICEPAR * pServicePar, HCSHANDLE hServiceDataHandle );
//�趨Service���ݾ��������Transponder��ID
BOOL CSUSPDMSetTransponderID( WORD wTransponderHandleID, HCSHANDLE hServiceDataHandle );
//�趨Service���ݾ���Ľ�Ŀ����(DM�ڲ���23���ֽڴ洢��Ŀ���ƣ�һ���ֽڴ洢��������)
BOOL CSUSPDMSetServiceName( const char * pbServiceName, CSUSPDMCodingType enmCodingType, HCSHANDLE hServiceDataHandle );
BOOL CSUSPDMSetServiceNameEx( const char * pbServiceName, int nNameLen, CSUSPDMCodingType enmCodingType, HCSHANDLE hServiceDataHandle );
//��Service���ݾ���л�ȡ��Ŀ����(DM�ڲ���23���ֽڴ洢��Ŀ���ƣ�һ���ֽڴ洢��������)
BOOL CSUSPDMGetServiceName( char * pbServiceName, CSUSPDMCodingType * penmCodingType, HCSHANDLE hServiceDataHandle );
BOOL CSUSPDMGetServiceNameEx( char * pbServiceName, int nNameLen, CSUSPDMCodingType * penmCodingType, HCSHANDLE hServiceDataHandle );
//�趨Service���ݾ���ļ���״̬
BOOL CSUSPDMSetServiceLockState( BOOL bLockState, HCSHANDLE hServiceDataHandle );
//��Service���ݾ���л�ȡ��Ŀ����״̬
BOOL CSUSPDMGetServiceLockState( BOOL * pbLockState, HCSHANDLE hServiceDataHandle );
//�趨Service���ݾ���Ľ�Ŀ����
BOOL CSUSPDMSetServiceType( CSUSPDMServiceType enmServiceType, HCSHANDLE hServiceDataHandle );
//��Service���ݾ���л�ȡ��Ŀ����
BOOL CSUSPDMGetServiceType( CSUSPDMServiceType * penmServiceType, HCSHANDLE hServiceDataHandle );
//��Service���ݾ���л�ȡϲ�ò���
BOOL CSUSPDMGetFAV( CSUSPDMServiceFAVLevel enmFAVLevel, BOOL * pbFAV, HCSHANDLE hServiceDataHandle );
//����Service���ݾ���е�ϲ������
BOOL CSUSPDMSetFAV( CSUSPDMServiceFAVLevel enmFAVLevel, BOOL bFAV, HCSHANDLE hServiceDataHandle );
//��ȡ������ĳ��Service������Transponder���
HCSHANDLE CSUSPDMGetTransponderByService( HCSHANDLE hServiceDataHandle, HCSHANDLE hContainer );
//�趨CA����(nFreeCAType: 1~13)
BOOL CSUSPDMSetFreeCAType( int nFreeCAType, BOOL bFreeCAMode, HCSHANDLE hServiceDataHandle );
//��ȡ�Ƿ�����ĳ��CA��������(nFreeCAType: 1~13)
BOOL CSUSPDMGetFreeCAType( int nFreeCAType, BOOL * pbReValue, HCSHANDLE hServiceDataHandle );
//�趨Teletext������0~63��
BOOL CSUSPDMSetTeletextIndex( int nIndex, HCSHANDLE hServiceDataHandle );
//��ȡTeletext����
int CSUSPDMGetTeletextIndex( HCSHANDLE hServiceDataHandle );
//�趨Subtitle������0~63��
BOOL CSUSPDMSetSubtitleIndex( int nIndex, HCSHANDLE hServiceDataHandle );
//��ȡSubtitle����
int CSUSPDMGetSubtitleIndex( HCSHANDLE hServiceDataHandle );
//�趨Audio������0~63��
BOOL CSUSPDMSetAudioIndex( int nIndex, HCSHANDLE hServiceDataHandle );
//��ȡAudio����
int CSUSPDMGetAudioIndex( HCSHANDLE hServiceDataHandle );
//��ȡ��Ŀ��FAV���ԣ�����ֵ ÿһλ����һ��FAV���ԣ���0λ����FAV1����1λ����FAV2...��0:������������飻1:������������飩
WORD CSUSPDMGetServiceFAVDrct( HCSHANDLE hServiceDataHandle );

//��ȡĳһ����Ŀ��ChannelNum
WORD CSUSPDMGetServiceChannelNum( HCSHANDLE hServiceDataHandle );
//�趨ĳһ����Ŀ��ChannelNo
BOOL CSUSPDMSetServiceChannelNum( HCSHANDLE hServiceDataHandle, WORD wChannelNum );

//�趨��Ŀ���������ԣ�ռ��2��bitλ
BOOL CSUSPDMSetServiceChannelType( HCSHANDLE hService, BYTE ucServiceChannelType );
//��ȡ��Ŀ���������ԣ�ռ��2��bitλ
BYTE CSUSPDMGetServiceChannelType( HCSHANDLE hService );

//��ȡ�û��Զ����Ŀ����
int CSUSPDMGetUserData( PBYTE pbData, int nBufLen, int nOffset, HCSHANDLE hServiceDataHandle );
//�趨�û��Զ����Ŀ����
BOOL CSUSPDMSetUserData( const PBYTE pbData, int nBufLen, int nOffset, HCSHANDLE hServiceDataHandle );

//��ȡ���趨�ƶ���־λ
BOOL CSUSPDMSetServiceMoveFlag( HCSHANDLE hServiceDataHandle, BOOL bMoveFlag );
BOOL CSUSPDMGetServiceMoveFlag( HCSHANDLE hServiceDataHandle );

BYTE CSUSPDMGetTPNITVersion( HCSHANDLE hTranponder );
BYTE CSUSPDMGetTPSDTVersion( HCSHANDLE hTranponder );
BYTE CSUSPDMGetTPBATVersion( HCSHANDLE hTranponder );
BYTE CSUSPDMGetTPPATVersion( HCSHANDLE hTranponder );
BOOL CSUSPDMSetTPNITVersion( HCSHANDLE hTranponder, BYTE ucVersionNum );
BOOL CSUSPDMSetTPSDTVersion( HCSHANDLE hTranponder, BYTE ucVersionNum );
BOOL CSUSPDMSetTPBATVersion( HCSHANDLE hTranponder, BYTE ucVersionNum );
BOOL CSUSPDMSetTPPATVersion( HCSHANDLE hTranponder, BYTE ucVersionNum );

//��ȡĳTP�µĽ�Ŀ����
int CSUSPDMGetServiceNumBlgdTP( HCSHANDLE hTranponder, HCSHANDLE hDataContainerHandle );

//�趨UniNUM������֧��RAPS
BOOL CSUSPDMSetServiceUniNum( HCSHANDLE hServiceDataHandle, WORD wUniNum );
//��ȡUniNUM������֧��RAPS
WORD CSUSPDMGetServiceUniNum( HCSHANDLE hServiceDataHandle );

// �趨��Ŀ����Ƶ��ʽ���ԣ�ռ��3��bitλ��
//	0����ͨ��Mpge-2
//	1��H.264
//	����δ����
BOOL CSUSPDMSetServiceVideoType( HCSHANDLE hService, BYTE ucServiceVideoType );
// ��ȡ��Ŀ���������ԣ�ռ��3��bitλ
BYTE CSUSPDMGetServiceVideoType( HCSHANDLE hService );

//��Service���ݾ���л�ȡ������ͱ�־λ
BOOL CSUSPDMGetServiceNameCCTableIndex( HCSHANDLE hServiceDataHandle, BYTE * pucCCTableIndex );

//ֱ������ServiceName�����ٶ�CodingType�����ж�
BOOL CSUSPDMSetServiceNameDirect( const char * pbServiceName, int nNameLen, HCSHANDLE hServiceDataHandle );

//��ȡ��������
DWORD CSUSPDMGetContainerType( HCSHANDLE hDataContainerHandle );

//��ȡ��Ŀ����TP����
DWORD CSUSPDMGetServiceTPType( HCSHANDLE hServiceDataHandle, HCSHANDLE hContainerDataHandle );

//����Deliver��DeliverType��ȡTP���������֧�ֶ�ģ��S+T��
HCSHANDLE CSUSPDMGetTransponderForMultiModel( HCSHANDLE hDataContainerHandle, DeliverExt * pDeliverExt, DWORD dwDeliverType );

BOOL CSUSPDMSetDeliverExt( HCSHANDLE hTransponderDataHandle, const DeliverExt * pDeliverExt );

BOOL CSUSPDMGetSatelliteDeliverExt( HCSHANDLE hTransponderSDataHandle, SatelliteDeliverExt * pSatelliteDeliverExt );

BOOL CSUSPDMSetSatelliteDeliverExt( HCSHANDLE hTransponderSDataHandle, const SatelliteDeliverExt * pSatelliteDeliverExt );

BOOL CSUSPDMSetServiceAudioType( HCSHANDLE hService, BYTE ucAudioType );
BYTE CSUSPDMGetServiceAudioType( HCSHANDLE hService );

BOOL CSUSPDMSetModulationForSat( HCSHANDLE hTransponderS, BYTE ucModulation );

BOOL CSUSPDMGetModulationForSat( HCSHANDLE hTransponderS, BYTE * pucModulation );

HCSHANDLE CSUSPDMGetTransponderForMultiModelEx( HCSHANDLE hDataContainerHandle, Deliver * pDeliver, DWORD dwDeliverType );

BOOL CSUSPDMSetModulationSystemForSat( HCSHANDLE hTransponderS, BYTE ucFlag );
BOOL CSUSPDMGetModulationSystemForSat( HCSHANDLE hTransponderS, BYTE * pucFlag );

BOOL CSUSPDMSetTPSPilot( HCSHANDLE hTransponderSHandle, BYTE ucPilot );
BOOL CSUSPDMGetTPSPilot( HCSHANDLE hTransponderSHandle, BYTE * pucPilot );

BOOL CSUSPDMSetDiSEqC1121Ext( DiSEqC1121Ext * pDiSEqC1121, HCSHANDLE hAntennaDataHandle );
BOOL CSUSPDMGetDiSEqC1121Ext( DiSEqC1121Ext * pDiSEqC1121, HCSHANDLE hAntennaDataHandle );

BOOL CSUSPDMGetTPSPilotStatus( HCSHANDLE hTransponderS, BYTE * pucStatus );

BOOL CSUSPDMSetTPSPilotStatus( HCSHANDLE hTransponderS, BYTE ucStatus );

BOOL CSUSPDMSetTunerIndex(HCSHANDLE hTransponder, int nIndex);

BYTE CSUSPDMGetTunerMask(HCSHANDLE hTransponder);

BOOL CSUSPDMSetBATINFO(HCSHANDLE hBouquetHandle, CSDTVDMBatInfo_S *psBatInfo);

BOOL CSUSPDMGetBATINFO(HCSHANDLE hBouquetHandle, CSDTVDMBatInfo_S *psBatInfo);

BOOL CSUSPDMGetBouquetIndex(HCSHANDLE hBouquetHandle, UINT *puIndex);

BOOL CSUSPDMSetServiceBouquetInfo(HCSHANDLE hServiceHandle, UINT uIndex);

BOOL CSUSPDMCheckServiceOfBouquet(HCSHANDLE hServiceHandle, HCSHANDLE hBouquetHandle);

BOOL CSUSPDMSetServicePFFlag(HCSHANDLE hServiceHandle, BOOL bFlag);

BOOL CSUSPDMGetServicePFFlag(HCSHANDLE hServiceHandle, BOOL *pbFlag);

BOOL CSUSPDMSetServiceScheduleFlag(HCSHANDLE hServiceHandle, BOOL bFlag);

BOOL CSUSPDMGetServiceScheduleFlag(HCSHANDLE hServiceHandle, BOOL *pbFlag);

//���º���δʵ��
BOOL CSUSPDMGetTerrestrialDeliverFromService( TerrestrialDeliver * pTerrestrialDeliver, HCSHANDLE hServiceDataHandle );
BOOL CSUSPDMGetSourceIDFromService( SourceID * pSourceID, HCSHANDLE hServiceDataHandle );

//��Ŀ��ӵ�DM��ʱ�������Ŀ�Ѿ����ڣ��Ƿ�������е�ĳЩ���ݣ�Ĭ�ϸ��£�����ΪFALSE����ʾ������
BOOL CSUSPDMSetIsUpdateSimilarObject(BOOL bUpdate);

#ifdef __cplusplus
}
#endif

#endif 

