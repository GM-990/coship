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

#define USPDM_C_CONTAINER		(0x01)   //有线机
#define USPDM_T_CONTAINER		(0x02)   //地面机
#define USPDM_S_CONTAINER		(0x04)   //卫星机

#define CABLE_SERVICE					(0x00) //有线节目
#define TERRESTRIA_SERVICE				(0x01) //地面节目
#define SATELLITE_SERVICE				(0x02) //卫星节目
#define IP_SERVICE						(0x03) //IP节目

#define USPDM_TERRESTREA_DELIVER		(0x02) //地面Deliver
#define USPDM_SATELLITE_DELIVER			(0x03) //卫星Deliver
#define USPDM_CABLE_DELIVER				(0x04) //有线Deliver
#define USPDM_VIRTUAL_DELIVER			(0x00) // 虚拟Deliver

#define USPDM_UNKNOWN_HANDLE			-1 

#ifdef  __cplusplus
extern "C" {
#endif

//获取数据句柄的ID值
DWORD CSUSPDMGetDataHandleID( HCSHANDLE hData );
//获取数据句柄类型（具体含义请参见上面的宏）
int CSUSPDMGetDataHandleType( HCSHANDLE hData );

//设置卫星名称
BOOL CSUSPDMSetSatelliteName( const char * pstrName, int nDataLen, HCSHANDLE hSatelliteDataHandle );
//获取卫星名称
int CSUSPDMGetSatelliteName( PBYTE pbBuff, int nBuffLen, HCSHANDLE hSatelliteDataHandle );
//设定卫星的DiSEpC1.2属性
BOOL CSUSPDMSetDiSEqC1222( DiSEqC1222 * pDiSEqC1222, HCSHANDLE hSatelliteDataHandle );
//获取卫星的DiSEpC1.2属性
BOOL CSUSPDMGetDiSEqC1222( DiSEqC1222 * pDiSEqC1222, HCSHANDLE hSatelliteDataHandle );
//设置卫星参数
BOOL CSUSPDMSetSatellitePar( SatellitePar * pSatellitePar, HCSHANDLE hSatelliteDataHandle );
//获取卫星参数
BOOL CSUSPDMGetSatellitePar( SatellitePar * pSatellitePar, HCSHANDLE hSatelliteDataHandle );

BOOL CSUSPDMSetSatelliteNameEx( const char * pstrName, int nDataLen, CSUSPDMCodingType enmCodingType, HCSHANDLE hDataHandle );
int CSUSPDMGetSatelliteNameEx( PBYTE pbBuff, int nBuffLen, CSUSPDMCodingType * penmCodingType, HCSHANDLE hDataHandle );


//获取天线的LNB属性
BOOL CSUSPDMGetLNB( LNB * pLNB, HCSHANDLE hAntennaDataHandle );
//设置天线的LNB属性
BOOL CSUSPDMSetLNB( LNB * pLNB, HCSHANDLE hAntennaDataHandle );
//获取天线的DiSEpC1.0属性
BOOL CSUSPDMGetDiSEqC1020( DiSEqC1020 * pDiSEqC1020, HCSHANDLE hAntennaDataHandle );
//设置天线的DiSEpC1.0属性
BOOL CSUSPDMSetDiSEqC1020( DiSEqC1020 * pDiSEqC1020, HCSHANDLE hAntennaDataHandle );
//获取天线的DiSEpC1.1属性
BOOL CSUSPDMGetDiSEqC1121( DiSEqC1121 * pDiSEqC1121, HCSHANDLE hAntennaDataHandle );
//设置天线的DiSEpC1.1属性
BOOL CSUSPDMSetDiSEqC1121( DiSEqC1121 * pDiSEqC1121, HCSHANDLE hAntennaDataHandle );
//获取天线参数
BOOL CSUSPDMGetAntennaPar( AntennaPar * pAntennaPar, HCSHANDLE hAntennaDataHandle );
//设置天线参数
BOOL CSUSPDMSetAntennaPar( AntennaPar * pAntennaPar, HCSHANDLE hAntennaDataHandle );

//获取卫星转发器的频点信息
BOOL CSUSPDMGetSatelliteDeliver( SatelliteDeliver * pSatelliteDeliver, HCSHANDLE hTransponderSDataHandle );
//设置卫星转发器的频点信息
BOOL CSUSPDMSetSatelliteDeliver( SatelliteDeliver * pSatelliteDeliver, HCSHANDLE hTransponderSDataHandle );
//设置卫星转发器所属的天线ID
BOOL CSUSPDMSetAntennaID( DWORD dwAntennaID, HCSHANDLE hTransponderSDataHandle );
//设置卫星转发器所属的卫星ID
BOOL CSUSPDMSetSatelliteID( DWORD dwSatelliteID, HCSHANDLE hTransponderSDataHandle );
//获取卫星转发器所属的卫星ID
DWORD CSUSPDMGetSatelliteID( HCSHANDLE hTransponderSDataHandle );
//获取卫星转发器所属的天线ID
DWORD CSUSPDMGetAntennaID( HCSHANDLE hTransponderSDataHandle );
//获取卫星转发器所属的卫星句柄
HCSHANDLE CSUSPDMGetSatelliteHandleByTransponder( HCSHANDLE hTransponderSDataHandle, HCSHANDLE hContainer );
//获取卫星转发器所属的天线句柄
HCSHANDLE CSUSPDMGetAntennaHandleByTrnasponder( HCSHANDLE hTransponderSDataHandle, HCSHANDLE hContainer );

//获取有线频点参数( 频点，符号率，调制方式 )
BOOL CSUSPDMGetCableDeliver( CableDeliver * pCableDeliver, HCSHANDLE hTransponderCDataHandle );
//设置有线频点参数( 频点，符号率，调制方式 )
BOOL CSUSPDMSetCableDeliver( CableDeliver * pCableDeliver, HCSHANDLE hTransponderCDataHandle );
//获取有线频点的偏移参数
BOOL CSUSPDMGetTransponderCOffset( TransponderOffset * pTransponderOffset, HCSHANDLE hTransponderCDataHandle );
//设置有线频点的偏移参数
BOOL CSUSPDMSetTransponderCOffset( TransponderOffset * pTransponderOffset, HCSHANDLE hTransponderCDataHandle );

//获取地面频点参数
BOOL CSUSPDMGetTerrestrialDeliver( TerrestrialDeliver * pTerrestrialDeliver, HCSHANDLE hTransponderTDataHandle );
//设置地面频点参数
BOOL CSUSPDMSetTerrestrialDeliver( TerrestrialDeliver * pTerrestrialDeliver, HCSHANDLE hTransponderTDataHandle );

//获取频点的ID值
BOOL CSUSPDMGetTransponderID( DWORD * pdwTransponderHandleID, HCSHANDLE hTransponderDataHandle );
//获取频点的SourcdID参数（TSID，OriginalNetworkID，NetworkID）
BOOL CSUSPDMGetSourceID( SourceID * pSourceID, HCSHANDLE hTransponderDataHandle );
//设置频点的SourcdID参数（TSID，OriginalNetworkID，NetworkID）
BOOL CSUSPDMSetSourceID( SourceID * pSourceID, HCSHANDLE hTransponderDataHandle );
//设置频点的Deliver参数
BOOL CSUSPDMSetDeliver( Deliver * pDeliver, HCSHANDLE hTransponderDataHandle );
//设置频点的偏移参数
BOOL CSUSPDMSetOffset( TransponderOffset * pTransponderOffset, HCSHANDLE hTransponderDataHandle );
//获取频点的偏移参数
BOOL CSUSPDMGetOffset( TransponderOffset * pTransponderOffset, HCSHANDLE hTransponderDataHandle );

//获取容器中指定Deliver值得频点句柄
HCSHANDLE CSUSPDMGetTransponderHandle( Deliver * pDeliver, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMGetTransponderHandleExt( DeliverExt * pDeliverExt, HCSHANDLE hDataContainerHandle );
//获取卫星容器中指定Deliver值，卫星ID，AntennaID 频点句柄
HCSHANDLE CSUSPDMGetTransponderHandleEx( Deliver * pDeliver, DWORD dwSatID, DWORD dwAntID, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMGetTransponderHandleForSatExt( DeliverExt * pDeliverExt, DWORD dwSatID, DWORD dwAntID, HCSHANDLE hDataContainerHandle );

//获取节目的CA属性
int CSUSPDMGetFreeCAMode( HCSHANDLE hServiceDataHandle );
//设定节目的CA属性
BOOL CSUSPDMSetFreeCAMode( int nFreeCAMode, HCSHANDLE hServiceDataHandle );
//从Service数据句柄中获取Service_id
BOOL CSUSPDMGetService_id( int * pnServiceID, HCSHANDLE hServiceDataHandle );
//设置Service数据句柄的Service_id
BOOL CSUSPDMSetService_id( int nServiceID, HCSHANDLE hServiceDataHandle );
//从Service数据句柄中获取用户自定义数据（未实现）
int CSUSPDMGetReUseData( PBYTE pbData, int nBufLen, HCSHANDLE hServiceDataHandle );
//设置Service数据句柄中的用户自定义数据（未实现）
BOOL CSUSPDMSetReUseData( PBYTE pbReUseData, int nDataLen, HCSHANDLE hServiceDataHandle );

//下列说明适用于下面两个函数
//ServicePID结构体中只有以下字段有效：
//	m_dwPMTPID_13，m_dwPCRPID_13，m_dwVideoPID_13，m_dwAudioPID_13
//以下字段
//	m_dwSubtitle_PID_Index_6，m_dwAudio_PID_Index_6，m_dwTeletext_PID_Index_6通过特定函数接口访问
//其他字段无效
//从Service数据句柄中获取ServicePID(包含service的所有PID信息)
BOOL CSUSPDMGetServicePID( ServicePID * pServicePID, HCSHANDLE hServiceDataHandle );
//设定Service数据句柄中的ServicePID
BOOL CSUSPDMSetServicePID( ServicePID * pServicePID, HCSHANDLE hServiceDataHandle );

//从Service数据句柄中获取SERVICEPAR
BOOL CSUSPDMGetSERVICEPAR( SERVICEPAR * pServicePar, HCSHANDLE hServiceDataHandle );
//设定Service数据句柄中的SERVICEPAR
BOOL CSUSPDMSetSERVICEPAR( SERVICEPAR * pServicePar, HCSHANDLE hServiceDataHandle );
//设定Service数据句柄所属的Transponder的ID
BOOL CSUSPDMSetTransponderID( WORD wTransponderHandleID, HCSHANDLE hServiceDataHandle );
//设定Service数据句柄的节目名称(DM内部有23个字节存储节目名称，一个字节存储编码类型)
BOOL CSUSPDMSetServiceName( const char * pbServiceName, CSUSPDMCodingType enmCodingType, HCSHANDLE hServiceDataHandle );
BOOL CSUSPDMSetServiceNameEx( const char * pbServiceName, int nNameLen, CSUSPDMCodingType enmCodingType, HCSHANDLE hServiceDataHandle );
//从Service数据句柄中获取节目名称(DM内部有23个字节存储节目名称，一个字节存储编码类型)
BOOL CSUSPDMGetServiceName( char * pbServiceName, CSUSPDMCodingType * penmCodingType, HCSHANDLE hServiceDataHandle );
BOOL CSUSPDMGetServiceNameEx( char * pbServiceName, int nNameLen, CSUSPDMCodingType * penmCodingType, HCSHANDLE hServiceDataHandle );
//设定Service数据句柄的加锁状态
BOOL CSUSPDMSetServiceLockState( BOOL bLockState, HCSHANDLE hServiceDataHandle );
//从Service数据句柄中获取节目加锁状态
BOOL CSUSPDMGetServiceLockState( BOOL * pbLockState, HCSHANDLE hServiceDataHandle );
//设定Service数据句柄的节目类型
BOOL CSUSPDMSetServiceType( CSUSPDMServiceType enmServiceType, HCSHANDLE hServiceDataHandle );
//从Service数据句柄中获取节目类型
BOOL CSUSPDMGetServiceType( CSUSPDMServiceType * penmServiceType, HCSHANDLE hServiceDataHandle );
//从Service数据句柄中获取喜好参数
BOOL CSUSPDMGetFAV( CSUSPDMServiceFAVLevel enmFAVLevel, BOOL * pbFAV, HCSHANDLE hServiceDataHandle );
//设置Service数据句柄中的喜好属性
BOOL CSUSPDMSetFAV( CSUSPDMServiceFAVLevel enmFAVLevel, BOOL bFAV, HCSHANDLE hServiceDataHandle );
//获取容器中某个Service所属的Transponder句柄
HCSHANDLE CSUSPDMGetTransponderByService( HCSHANDLE hServiceDataHandle, HCSHANDLE hContainer );
//设定CA类型(nFreeCAType: 1~13)
BOOL CSUSPDMSetFreeCAType( int nFreeCAType, BOOL bFreeCAMode, HCSHANDLE hServiceDataHandle );
//获取是否属于某种CA类型属性(nFreeCAType: 1~13)
BOOL CSUSPDMGetFreeCAType( int nFreeCAType, BOOL * pbReValue, HCSHANDLE hServiceDataHandle );
//设定Teletext索引（0~63）
BOOL CSUSPDMSetTeletextIndex( int nIndex, HCSHANDLE hServiceDataHandle );
//获取Teletext索引
int CSUSPDMGetTeletextIndex( HCSHANDLE hServiceDataHandle );
//设定Subtitle索引（0~63）
BOOL CSUSPDMSetSubtitleIndex( int nIndex, HCSHANDLE hServiceDataHandle );
//获取Subtitle索引
int CSUSPDMGetSubtitleIndex( HCSHANDLE hServiceDataHandle );
//设定Audio索引（0~63）
BOOL CSUSPDMSetAudioIndex( int nIndex, HCSHANDLE hServiceDataHandle );
//获取Audio索引
int CSUSPDMGetAudioIndex( HCSHANDLE hServiceDataHandle );
//获取节目的FAV属性（返回值 每一位代表一个FAV属性，第0位代表FAV1，第1位代表FAV2...。0:不属于这个分组；1:隶属于这个分组）
WORD CSUSPDMGetServiceFAVDrct( HCSHANDLE hServiceDataHandle );

//获取某一个节目的ChannelNum
WORD CSUSPDMGetServiceChannelNum( HCSHANDLE hServiceDataHandle );
//设定某一个节目的ChannelNo
BOOL CSUSPDMSetServiceChannelNum( HCSHANDLE hServiceDataHandle, WORD wChannelNum );

//设定节目的声道属性：占用2个bit位
BOOL CSUSPDMSetServiceChannelType( HCSHANDLE hService, BYTE ucServiceChannelType );
//获取节目的声道属性：占用2个bit位
BYTE CSUSPDMGetServiceChannelType( HCSHANDLE hService );

//获取用户自定义节目数据
int CSUSPDMGetUserData( PBYTE pbData, int nBufLen, int nOffset, HCSHANDLE hServiceDataHandle );
//设定用户自定义节目数据
BOOL CSUSPDMSetUserData( const PBYTE pbData, int nBufLen, int nOffset, HCSHANDLE hServiceDataHandle );

//获取和设定移动标志位
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

//获取某TP下的节目总数
int CSUSPDMGetServiceNumBlgdTP( HCSHANDLE hTranponder, HCSHANDLE hDataContainerHandle );

//设定UniNUM，用于支持RAPS
BOOL CSUSPDMSetServiceUniNum( HCSHANDLE hServiceDataHandle, WORD wUniNum );
//获取UniNUM，用于支持RAPS
WORD CSUSPDMGetServiceUniNum( HCSHANDLE hServiceDataHandle );

// 设定节目的视频格式属性：占用3个bit位。
//	0：普通的Mpge-2
//	1：H.264
//	其它未定义
BOOL CSUSPDMSetServiceVideoType( HCSHANDLE hService, BYTE ucServiceVideoType );
// 获取节目的声道属性：占用3个bit位
BYTE CSUSPDMGetServiceVideoType( HCSHANDLE hService );

//从Service数据句柄中获取码表类型标志位
BOOL CSUSPDMGetServiceNameCCTableIndex( HCSHANDLE hServiceDataHandle, BYTE * pucCCTableIndex );

//直接设置ServiceName，不再对CodingType进行判断
BOOL CSUSPDMSetServiceNameDirect( const char * pbServiceName, int nNameLen, HCSHANDLE hServiceDataHandle );

//获取容器类型
DWORD CSUSPDMGetContainerType( HCSHANDLE hDataContainerHandle );

//获取节目所属TP类型
DWORD CSUSPDMGetServiceTPType( HCSHANDLE hServiceDataHandle, HCSHANDLE hContainerDataHandle );

//根据Deliver及DeliverType获取TP句柄，用于支持多模（S+T）
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

//以下函数未实现
BOOL CSUSPDMGetTerrestrialDeliverFromService( TerrestrialDeliver * pTerrestrialDeliver, HCSHANDLE hServiceDataHandle );
BOOL CSUSPDMGetSourceIDFromService( SourceID * pSourceID, HCSHANDLE hServiceDataHandle );

//节目添加到DM中时，如果节目已经存在，是否更新其中的某些数据，默认更新，设置为FALSE，表示不更新
BOOL CSUSPDMSetIsUpdateSimilarObject(BOOL bUpdate);

#ifdef __cplusplus
}
#endif

#endif 

