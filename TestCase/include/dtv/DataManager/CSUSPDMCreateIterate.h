#ifndef CreateIterateInterface_H
#define CreateIterateInterface_H

#include "CSUSPDMCommonType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/**
@brief DM�����ݱ仯дflashʱ�Ļص�֪ͨ
@param[in] nDataType �����仯���������ͣ�ȡֵ��ΧΪ
				USPDM_ANTENNA_HANDLE	= 0		
				USPDM_SATELLITE_HANDLE  = 1		
				USPDM_T_TRANSPONDER_HANDLE =2		
				USPDM_S_TRANSPONDER_HANDLE =3			
				USPDM_C_TRANSPONDER_HANDLE =4		
				USPDM_SERVICE_HANDLE =5			
				USPDM_ANTSATRELATION_HANDLE =6		
			���������CSUSPDMParseData.h�й����⼸�������Ķ���
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
*/
typedef void (*CSOnUSPDMFlashDataChange_F) (int nDataType, void* pvUserData);

/**
@brief ��Ӽ���DM�����ݱ仯дflash�Ļص�����
@param[in] fnCallback ע��Ļص�����
@param[in] pvUserdata �û�����
@note ����������ע��8���ص�����,���ע��ʧ�ܷ���FALSE
*/
BOOL CSUSPDMAddFlashDataChangeCallback(CSOnUSPDMFlashDataChange_F fnCallback, void* pvUserdata);

/**
@brief ɾ������DM�����ݱ仯дflash�Ļص�����
@param[in] fnCallback ע��Ļص�����
*/
BOOL CSUSPDMRemoveFlashDataChangeCallback(CSOnUSPDMFlashDataChange_F fnCallback);

/*
 *	Iterator ��ز���
 */

// ���� ���ڱ���RAM�����й�ϵ�� �������
HCSHANDLE CSUSPDMCreateIterateAllAntSatRelation( HCSHANDLE hDataContainerHandle );
// ���� ���ڱ���RAM���������ǵ� �������
HCSHANDLE CSUSPDMCreateIterateAllSatellite( HCSHANDLE hDataContainerHandle );
// ���� ���ڱ���RAM���������ߵ� �������
HCSHANDLE CSUSPDMCreateIterateAllAntenna( HCSHANDLE hDataContainerHandle );
// ���� ���ڱ���RAM������TP�� �������
HCSHANDLE CSUSPDMCreateIterateAllTransponder( HCSHANDLE hDataContainerHandle );
// ���� ���ڱ���RAM��������ָ������,�ƶ����ߵ�����TP�� �������
HCSHANDLE CSUSPDMCreateIterateAllTransponderS( DWORD dwSatelliteID, BOOL bSatelliteIDMask, DWORD dwAntennaID, BOOL bAntennaIDMask, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateAllTPC( HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateAllTPT( HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateAllTPS( HCSHANDLE hDataContainerHandle );

//���������Ĳ���˵����
//	nSortKey��0 ServiceID��1 ServiceName��2 Service CA Mode��3 ServiceChannelNum��4 service lock state,5��tp_id+ser_id, 6: ��Ŀ������ tp_id
//  nSortType��1 ����2 ����
//

//���ս�Ŀ����Ƶ���TSID+ServiceID��������
//	�������ڴ��������·�����Ľ�����������ǵ�ID�趨Ϊ0x00030000����ô���ǰ���TSID+ServiceID�������ַ�ʽ����������������������������������Ч�����磺
//		CSUSPDMCreateIterateSatelliteService( 0x00030000, 0xff, TRUE, FALSE, hContainer );
//

//�����µĽ����˵��
//	�������ڴ��������½�Ŀ�ķ�������������������ѭ���¹���
//		1. dwSatelliteID�ĵ�16λ������ʵ������ID����Ϊ���еĽ���ID�Ŷ���13λ��
//		2. dwSatelliteID�ĸ�16λ����ͬ������ʽ������Ĺ涨����������
//		���dwSatelliteID�ĵ�16λΪ0xffff����ô��ʾ����������������µĽ�Ŀ��
//		���dwSatelliteID�ĸ�16λΪ0x0000����ô��ʾ���ս�Ŀ��Flash�е�����˳������
//		���dwSatelliteID�ĸ�16λΪ0x0001����ô��ʾ���ս�Ŀ������TPFre�ؼ��ֽ�����������
//		���dwSatelliteID���ܹ�32λΪ0xffffffff����ô��ʾ�õ����������µİ���������TPFre�ؼ��ֽ�����������Ϊ��ǰ����ݣ���
//		���dwSatelliteID�ĸ�16λΪ����ֵδ����,�벻Ҫʹ��.
//
//	���ӣ��Ժ��� CSUSPDMCreateIterateSatelliteService Ϊ��
//			��˵��� CSUSPDMCreateIterateSatelliteService��0x00000001, 0xff, FALSE, FALSE, hContainer );
//				��ʾ�õ�����IDΪ0x0001�������µ����н�Ŀ���Խ�Ŀ������˳��Ϊ��
//			��˵��� CSUSPDMCreateIterateSatelliteService��0x00010001, 0xff, FALSE, FALSE, hContainer );
//				��ʾ�õ�����IDΪ0x0001�������µ����н�Ŀ���Խ�Ŀ����TP��FreΪ�򣨴�С���󣩣�
//			��˵��� CSUSPDMCreateIterateSatelliteService��0x0001ffff, 0xff, FALSE, FALSE, hContainer );
//				��ʾ�õ����������µ����н�Ŀ���Խ�Ŀ����TP��FreΪ�򣨴�С���󣩣�
//			��˵��� CSUSPDMCreateIterateSatelliteService��0xffffffff, 0xff, FALSE, FALSE, hContainer );
//				��ʾ�õ����������µ����н�Ŀ���Խ�Ŀ����TP��FreΪ�򣨴�С���󣩣�
//	 ���Ϻ����Ĳ�ͬ�������µ��ٶȲ��첻��
//
//	ע���������õĴ���������ĺ����������������������У����ܻ����¼ӽӿ�û�а�����������
//			CSUSPDMCreateIterateServiceBelongedSatellite
//			CSUSPDMCreateIterateFreeModeSerBlgedSat
//			CSUSPDMCreateIterateSatelliteService
//			CSUSPDMCreateIterateSatelliteFavFreeService
//			CSUSPDMCreateIterateSatelliteFavFreeLockService
//		�����е���Ҫ����dwSatelliteID���Ƿ�����Ľ���������á�
//	
//	��:
//		Ϊʲôû�в���ͨ�õ�SortKey,SortType��Ϊ�����½�Ŀ��TPFre�ؼ��ֵ�����Ľӿڷ�ʽ,��Ҫ��
//		Ϊ�������Ч�ʿ���.Ϊ��ʹ5000�׽�Ŀ��ĳ��(����)�����µĽ�Ŀ����TPFre�����ʱ��Ч����ms
//		���㣬���Ǿ����������ֽӿ���ʽ������������ַ�ʽ������Ŀǰ�Ĺ����¼���Iterate����������
//		��������֪Ϥ��
//



// ���� ���ڱ�������RAM�е�����Service���ݵ� �������
HCSHANDLE CSUSPDMCreateIterateAllService( HCSHANDLE hDataContainerHandle, BYTE bServiceType );
// ���� ���ڱ�������RAM�е�����ָ��ϲ������Service���ݵ� �������
HCSHANDLE CSUSPDMCreateIterateFAVService( HCSHANDLE hDataContainerHandle, int nFAVType, BYTE bServiceType );
// ���� ���ڱ�������RAM�е�ָ������״̬��Service���ݵ� �������
HCSHANDLE CSUSPDMCreateIterateLockService( HCSHANDLE hDataContainerHandle, BOOL bLockState, BYTE bServiceType );
// ���� �û�ָ������ʽ�����н�Ŀ�� �������
HCSHANDLE CSUSPDMCreateIterateServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey , int nSortType , BYTE bServiceType );
// ���� �û�ָ������ʽ��ָ��FAV���Ե����н�Ŀ�� �������
HCSHANDLE CSUSPDMCreateIterateFAVServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, int nFAVIndex, BYTE bServiceType );
// ���� �û�ָ������ʽ��ָ������״̬�����н�Ŀ�� �������
HCSHANDLE CSUSPDMCreateIterateLockServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, BOOL bLockState, BYTE bServiceType );
// ���� ���ڱ������м��ܣ���ѣ��Ľ�Ŀ�� �������
HCSHANDLE CSUSPDMCreateIteateFreeService( BOOL bFreeMode, BYTE bServiceType, HCSHANDLE hContainer );
// ���� �������н�Ŀ������������������ǣ���һ�ؼ��֣���TP���ڶ��ؼ��֣�����
HCSHANDLE CSUSPDMCreateIterateSatTPServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, BYTE bServiceType );
// ���� �������û�ָ�������ǵ����н�Ŀ�� �������
HCSHANDLE CSUSPDMCreateIterateServiceBelongedSatellite( DWORD dwSatelliteID, BYTE bServiceType, HCSHANDLE hDataContainerHandle );
// ���� �������û�ָ�������ǵ����н�Ŀ�����Ұ����û�ָ����ʽ����
HCSHANDLE CSUSPDMCreateIterateSerBlgedSatSorted( DWORD dwSatelliteID, BYTE bServiceType, int nSortKey, int nSortType, HCSHANDLE hDataContainerHandle );
// ���� �������û�ָ�������ǣ�ָ���������͵����н�Ŀ
HCSHANDLE CSUSPDMCreateIterateFreeModeSerBlgedSat( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, HCSHANDLE hDataContainerHandle );
// ���� �������û�ָ�������ǣ�ָ���������͵����н�Ŀ�����Ұ����û�ָ����ʽ����
HCSHANDLE CSUSPDMCreateIterateFreeModeSerBlgedSatSorted( DWORD dwSatelliteID, BYTE bServiceType, int nSortKey, int nSortType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, HCSHANDLE hDataContainerHandle );
// �����ݹ���������˳��Ϊ˳���ȡĳ�������µ����н�Ŀ
HCSHANDLE CSUSPDMCreateIterateSatelliteService( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, HCSHANDLE hDataContainerHandle );
// ���� ���ܣ��Ǽ��ܣ���Ŀ������ʽ�Ľ����
HCSHANDLE CSUSPDMCreateIterateFreeServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, BOOL bFreeType, BYTE bServiceType );
// ���� ĳ�ּ������ͽ�Ŀ�Ľ����
HCSHANDLE CSUSPDMCreateIterateCATypeService( HCSHANDLE hDataContainerHandle, WORD wCAType, BYTE bServiceType );

// ���� �������û�ָ�������ǣ�ָ����������, ָ��FAV �����н�Ŀ
HCSHANDLE CSUSPDMCreateIterateSatelliteFavFreeService( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, WORD wFAVLevel, BOOL bFAVLevelMask, HCSHANDLE hDataContainerHandle );
// ���� �������û�ָ�������ǣ�ָ����������, ָ��FAV �����д�������� ��Ŀ
HCSHANDLE CSUSPDMCreateIterateSatFavFreeServiceSorted( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, WORD wFAVLevel, BOOL bFAVLevelMask, int nSortKey, int nSortType, HCSHANDLE hDataContainerHandle );
// ���� �������û�ָ�������ǣ�ָ����������, ָ��FAV ָ��LockState �����н�Ŀ
HCSHANDLE CSUSPDMCreateIterateSatelliteFavFreeLockService( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, WORD wFAVLevel, BOOL bFAVLevelMask, BOOL bLockState, BOOL bLockStateMask, HCSHANDLE hDataContainerHandle );


// ���� ��Ŀ�����а���ָ���ַ��������н�Ŀ�� �������
HCSHANDLE CSUSPDMCreateIterateSimilarNameService( HCSHANDLE hDataContainerHandle, const BYTE * pServiceName, int nServiceNameLen, BOOL bCaseSensitive, BYTE bServiceType );
// �����趨 IterateSimilarNameService ��ָ���ַ���
BOOL CSUSPDMResetSimilarName( HCSHANDLE hSimilarNameIterate, const BYTE * pServiceName, int nNameLen );

// ���� ��Ŀ�����а���ָ���ַ��������н�Ŀ�� �������������ʽ��
HCSHANDLE CSUSPDMCreateIterateSimilarNameServiceSorted( HCSHANDLE hDataContainerHandle, const BYTE * pServiceName, int nServiceNameLen, BOOL bCaseSensitive, BYTE bServiceType, int nSortKey, int nSortType );
// �����趨 IterateSimilarNameServiceSorted ��ָ���ַ���������ʽ��
BOOL CSUSPDMResetSimilarNameSorted( HCSHANDLE hSimilarNameIterateSorted, const BYTE * pServiceName, int nNameLen );

//nFAV: 0(�õ�����FAV���ԣ� 1~16��FAV�ȼ��� 17��ĳ��FAV�ȼ�
HCSHANDLE CSUSPDMCreateIterateFAVServicePlus( HCSHANDLE hDataContainerHandle, int nFAVType, BYTE bServiceType );
//�õ�ĳ��FAVLevel�������Ǽ���/��ѵĽ�Ŀ�Ľ������bFAVLevelMask�趨ΪFALSE����wFAVLevel�������ã�bFreeCAModeMask�趨ΪFALSE����bFreeCAMode�������ã�
HCSHANDLE CSUSPDMCreateIterateFAVFreeService( HCSHANDLE hDataContainerHandle, WORD wFAVLevel, BOOL bFAVLevelMask, BOOL bFreeCAMode, BOOL bFreeCAModeMask, BYTE bServiceType );

// �����û��Զ������ݵĽ����
HCSHANDLE CSUSPDMCreateIterateUserDataService( const BYTE * pbMatch, const BYTE * pbMask, const BYTE * pbNegate, int nFilterDepth, BYTE bServiceType, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateUserDataServiceSorted( const BYTE * pbMatch, const BYTE * pbMask, const BYTE * pbNegate, int nFilterDepth, BYTE bServiceType, int nSortKey, int nSortType, USPDMCompareUserServiceData pUserCompareFunc, HCSHANDLE hDataContainerHandle );

//��������UniNum��ֵ��Ϊ0�Ľ�Ŀ�Ľ����������֧��RAPS��
HCSHANDLE CSUSPDMCreateIterateUniNumService( HCSHANDLE hDataContainerHandle, BYTE bServiceType );

//���½ӿڴ����Ľ��������˵�����Skip��־�Ľ�Ŀ
HCSHANDLE CSUSPDMCreateIterateAllServiceForSkip( HCSHANDLE hDataContainerHandle, BYTE bServiceType );
HCSHANDLE CSUSPDMCreateIterateFAVServiceForSkip( HCSHANDLE hDataContainerHandle, CSUSPDMServiceFAVLevel enmFAVType, BYTE bServiceType );
HCSHANDLE CSUSPDMCreateIterateFAVServicePlusForSkip( HCSHANDLE hDataContainerHandle, int nFAVType, BYTE bServiceType );
HCSHANDLE CSUSPDMCreateIterateServiceSortedForSkip( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, BYTE bServiceType );
HCSHANDLE CSUSPDMCreateIterateFreeServiceForSkip( BOOL bFreeMode, BYTE bServiceType, HCSHANDLE hContainer );
HCSHANDLE CSUSPDMCreateIterateFreeServiceSortedForSkip( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, BOOL bFreeType, BYTE bServiceType );
HCSHANDLE CSUSPDMCreateIterateSerBlgedSatSortedForSkip( DWORD dwSatelliteID, BYTE bServiceType, int nSortKey, int nSortType, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateFreeModeSerBlgedSatForSkip( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateFreeModeSerBlgedSatSortedForSkip( DWORD dwSatelliteID, BYTE bServiceType, int nSortKey, int nSortType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateServiceBelongedSatelliteForSkip( DWORD dwSatelliteID, BYTE bServiceType, HCSHANDLE hDataContainerHandle );

// �ͷ� �������
BOOL CSUSPDMFreeIteratorHandle( HCSHANDLE hIteratorHandle, HCSHANDLE hContainer );


// ��ȡָ����������ĵ�һ�����ݾ��
HCSHANDLE CSUSPDMGetHeadDataHandle( HCSHANDLE hIterateHandle );
// ��ȡָ��������������һ�����ݾ��
HCSHANDLE CSUSPDMGetTailDataHandle( HCSHANDLE hIterateHandle );
// ��ȡָ����������ĵ�ǰ���ݾ������һ�����ݾ��
HCSHANDLE CSUSPDMGetPrevDataHandle( HCSHANDLE hIterateHandle );
// ��ȡָ����������ĵ�ǰ���ݾ������һ�����ݾ��
HCSHANDLE CSUSPDMGetNextDataHandle( HCSHANDLE hIterateHandle );
// ��ȡ�������ָ�����������ݾ��
HCSHANDLE CSUSPDMFindIndexDataHandle( int nIndex, HCSHANDLE hIterateHandle );
// ��ȡ������������ݵ��ܸ���
int CSUSPDMGetDataHandleCount( HCSHANDLE hIterateHandle );
// ��ȡָ�����ݾ������������е�����λ��
int CSUSPDMGetCurDataHandleIndex( HCSHANDLE hIterateHandle );
// �趨���������ǰ������λ��
BOOL CSUSPDMSetCurDataHandleIndex( HCSHANDLE hIterateHandle, int nIndex );


/*
 *	��ӵ������ݵ���ز���
 */


// �������������ڴ����Satellite���ݾ����������������������
HCSHANDLE CSUSPDMCreateSatelliteDataHandle( HCSHANDLE hDataContainerHandle );
// �������������ڴ�����������ݾ����������������������
HCSHANDLE CSUSPDMCreateAntennaDataHandle( HCSHANDLE hDataContainerHandle );
// �������������ڴ���д���Transponder���ݾ��(����container�Ĳ�ͬ������ͬ�����ݾ����
HCSHANDLE CSUSPDMCreateTransponderDataHandle( HCSHANDLE hDataContainerHandle );
// �������������ڴ���д���TransponderS���ݾ����������������������
HCSHANDLE CSUSPDMCreateTransponderSDataHandle( HCSHANDLE hDataContainerHandle );
// �������������ڴ����TransponderC���ݾ����������������������
HCSHANDLE CSUSPDMCreateTransponderCDataHandle( HCSHANDLE hDataContainerHandle );
// �������������ڴ����TransponderT���ݾ�������޵�������������
HCSHANDLE CSUSPDMCreateTransponderTDataHandle( HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateTransponderVDataHandle(HCSHANDLE hDataContainerHandle);
// �������������ڴ����Service���ݾ��
HCSHANDLE CSUSPDMCreateServiceDataHandle( HCSHANDLE hDataContainerHandle );
// �ͷ��������������ݾ��
BOOL CSUSPDMFreeDataHandle( HCSHANDLE hDataHandle, HCSHANDLE hDataContainerHandle );

//��ȡĳ��ChannelNo��ServiceHandle
HCSHANDLE CSUSPDMGetServiceHandleByChannelNum( WORD wChannelNo, HCSHANDLE hDataContainerHandle );




// �����ݾ����ӵ�ָ����������ͷλ��
BOOL CSUSPDMAddHeadDataHandle( HCSHANDLE hDataHandle, HCSHANDLE hDataContainerHandle );
// �����ݾ����ӵ�������βλ��
BOOL CSUSPDMAddTailDataHandle( HCSHANDLE hDataHandle, HCSHANDLE hDataContainerHandle );
// �����ݾ����ӵ�������ָ��λ��
BOOL CSUSPDMInsertDataHandle( HCSHANDLE hPosDataHandle, HCSHANDLE hInsertDataHandle, HCSHANDLE hDataContainerHandle );
// �����ݾ���ƶ���������ָ��λ��
BOOL CSUSPDMMoveDataHandle( HCSHANDLE hPosDataHandle, HCSHANDLE hMovedDataHandle, HCSHANDLE hDataContainerHandle );
// ������Ŀ��λ��
BOOL CSUSPDMSwapServiceDataHandle( HCSHANDLE hServiceA, HCSHANDLE hServiceB, HCSHANDLE hDataContainerHandle );

// �����û�ָ���ؼ���˳������Ŀ���� (sort key::service id, 1:service name, 2:ser ca mode, 3: channel num, 8:ser user data)(sort type: 1,��������������)
DWORD CSUSPDMInsertDataHandleSorted( HCSHANDLE hDataContainerHandle, HCSHANDLE hServiceHandle, int nSortKey, int nSortType );
// �Խ�Ŀ�������������򣬰����û�ָ���ıȽϺ���������ʽ
BOOL CSUSPDMSortAllService( HCSHANDLE hContainer, USPDMCompareService pCompareFun, CSUSPDMSortType enmSortType );

// �����ݾ���������Ƴ���û���ͷţ�
BOOL CSUSPDMRemoveDataHandle( HCSHANDLE hDataHandle, HCSHANDLE hDataContainerHandle );
// ����Service��Delete��־ɾ�����ͷ�����Delete��־ΪTRUE�Ľ�Ŀ��û��ͬ��Flash��������ͬ��Flash���������غ���
BOOL CSUSPDMDeleteAllServiceByDelFlag( HCSHANDLE hDataContainerHandle );
// �ָ�Ĭ�ϣ���pAddr����ȡ����
BOOL CSUSPDMRestoreDefaultData( HCSHANDLE hDataContainerHandle, int nType, const BYTE * pAddr, int nLen );
// �ָ�Ĭ�ϣ���pAddr����ȡ����, ���Service����
BOOL CSUSPDMRestoreDefaultDataEx( HCSHANDLE hDataContainerHandle, const BYTE * pAddr, int nLen, BOOL bServiceLimitFlag, DWORD dwHMinFre, DWORD dwHMaxFre, DWORD dwVMinFre, DWORD dwVMaxFre, BOOL bDefaultLNBMask, BYTE ucDefaultLNBType, BOOL bDefaultLNBPowerMask, BYTE ucDefaultLNBPower );

// �����ƶ�����
//haHandleArray[]: Ҫ�ƶ���Service�������
//nHandleCount: ����Ԫ�ظ���
//nPosHandle: λ��Ԫ�ؾ��
BOOL CSUSPDMMoveBatchDataHandle( HCSHANDLE hDataContainerHandle, HCSHANDLE haHandleArray[], int nHandleCount, HCSHANDLE nPosHandle );
BOOL CSUSPDMMoveBatchDataHandleExt( HCSHANDLE hDataContainerHandle, HCSHANDLE nPosHandle );
//�����ƶ����ݡ�
//	����ѵ����������趨Ϊ1�����ƶ�֮�󲻸ı��κν�Ŀ���ƶ���־λ��
//	�趨Ϊ0���ƶ�֮���Զ���ս�Ŀ���ƶ���־λ�����嶯����CSUSPDMMoveBatchDataHandleExt��ͬ��
BOOL CSUSPDMMoveBatchDataHandleAd( HCSHANDLE hDataContainerHandle, HCSHANDLE nPosHandle, DWORD dwMoveType );


// �������������ڴ��е�����ͬ�����洢��
BOOL CSUSPDMSyncToFlash( HCSHANDLE hDataContainerHandle );
// ���� hStorageHandle ָʾ�����ݴ��ڴ�ͬ����Flash
BOOL CSUSPDMRefresh( HCSHANDLE hStorageHandle );

//ɾ��ĳ�������µ����н�Ŀ��û��ͬ��Flash��
BOOL CSUSPDMDeleteServiceBlgedSat( HCSHANDLE hDataContainerHandle, HCSHANDLE hSatellite );
//ɾ��ĳ��ת�����µ����н�Ŀ��û��ͬ��Flash��
BOOL CSUSPDMDeleteServiceBlgedTP( HCSHANDLE hDataContainerHandle, HCSHANDLE hTransponder );
//ɾ��ĳ�������µ�����TP��������ɾ��TP�µ����н�Ŀ����û��ͬ��Flash��
BOOL CSUSPDMDeleteTPBlgedSat( HCSHANDLE hDataContainerHandle, HCSHANDLE hSatellite );


//ɾ�����н�Ŀ��û��ͬ��Flash��
BOOL CSUSPDMDeleteAllService( HCSHANDLE hDataContainerHandle );
// ���Container�е����н�Ŀ��Ƶ��
BOOL CSUSPDMCleanContainer( HCSHANDLE hDataContainerHandle );
// ��Flash�е������滻�ڴ��е����ݣ���Ŀ��Ƶ�㣬���ǣ����ߣ�
BOOL CSUSPDMReLoadAllData( HCSHANDLE hDataContainerHandle );


// ������ϵ���ݾ��
HCSHANDLE CSUSPDMCreateAntSatRelationDataHandle( HCSHANDLE hDataContainerHandle );
// ���ҹ�ϵ���ݾ��
HCSHANDLE CSUSPDMFindAntSatRelation( DWORD dwSatelliteID, DWORD dwAntennaID, HCSHANDLE hContainer );
// ɾ���û�ָ����ĳһ��ϵ
BOOL CSUSPDMDeleteAntSatRelation( DWORD dwSatelliteID, DWORD dwAntennaID, HCSHANDLE hContainer );
// ֱ�����һ����ϵ����������
DWORD CSUSPDMAddAntSatRelation( DWORD dwSatelliteID, DWORD dwAntennaID, HCSHANDLE hContainer );
// ��ȡ��ϵ���������ֶ�
int CSUSPDMGetAntSatRelationData( DWORD * pdwSatelliteID, DWORD * pdwAntennaID, HCSHANDLE hRSDataHandle );
// �趨��ϵ���������ֶ�
int CSUSPDMSetAntSatRelationData( DWORD dwSatelliteID, DWORD dwAntennaID, HCSHANDLE hRSDataHandle );

// ���� ���ڱ������е����Ŀ�� �������
HCSHANDLE CSUSPDMCreateIterateAllTerService( BOOL bFreeMode, BOOL bFreeModeMask, BYTE bServiceType, HCSHANDLE hDataContainerHandle );

// ���� ���ڱ����������ǵĽ�Ŀ�� �������
HCSHANDLE CSUSPDMCreateIterateAllSatService( BOOL bFreeMode, BOOL bFreeModeMask, BYTE bServiceType, HCSHANDLE hDataContainerHandle );

// ���� ���ڱ����������ߵĽ�Ŀ�� �������
HCSHANDLE CSUSPDMCreateIterateAllCabService( BOOL bFreeMode, BOOL bFreeModeMask, BYTE bServiceType, HCSHANDLE hDataContainerHandle );

/*
Function: �趨��Ŀ����Ƶ�����͵Ĺ�������

Para:
	hDataContainerHandle[in]�����������Container���
	hIterateDataHandle[in]:�����������������
	dwServiceTPType[in]:�����������Ŀ����Ƶ������

Return:
	SUCCESS: ����TRUE
	FAILURD: ����FALSE

Description:
	1) 
	2) 
*/
BOOL CSUSPDMSetServiceTPTypeFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, DWORD dwServiceTPType );

/*
Function: �趨��Ŀ����״̬��������

Para:
	bLockState[in]:����״̬��TUREΪ�����ģ�FALSEΪ��������

Return:
	SUCCESS: ����TRUE
	FAILURD: ����FALSE
 */
BOOL CSUSPDMSetServiceLockStateFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, BOOL bLockState );

/*
Function: �趨��Ŀ���͹�������

Para:
	ucServiceType[in]:�����������Ŀ����

 */
BOOL CSUSPDMSetServiceTypeFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, BYTE ucServiceType );

/*
Function: �趨UniNum = 0�Ĺ�������

Para:
	wUniNum[in]:���������һ��Ϊ0

Description:
	1) ����UniNum��ֵ��Ϊ0�Ľ�Ŀ������֧��RAPS��
	2) 
*/
BOOL CSUSPDMSetUniNumFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, WORD wUniNum );

/* 
Function: �趨skip��������

Para:
	bSkipMask[in]:���������TUREΪ��Ҫ���ˣ�FALSEΪ����Ҫ����(�����ô˺���ʱĬ��Ϊ����Ҫ����)

Description:
	1) ����˵�����skip��ǵĽ�Ŀ
	2) 
*/
BOOL CSUSPDMSetSkipFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, BOOL bSkipMask );

/*
Function: ����ĳ��FAVlevel�Ĺ�������

Para:
	wFAVIndex[in]:���������FAVLevle

*/
BOOL CSUSPDMSetFAVIndexFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, WORD wFAVIndex );

/* 
Function: �趨�������͹�������

Para:
	wCAType[in]:�����������������

*/
BOOL CSUSPDMSetCATypeFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, WORD wCAType );

/* 
Function: ��������еĽ�Ŀ��ָ����ʽ����

Para:
	nSortKey[in]:�������������ؼ���
	nSortType[in]:�������������ʽ���������
*/
BOOL CSUSPDMIterateSortService( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, int nSortKey, int nSortType );
BOOL CSUSPDMIterateSortServiceByUserFunc( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, USPDMCompareServiceEx	pUserCompareFunc, void* pUserData );

BOOL CSUSPDMRestoreDefaultDataForSymbol( HCSHANDLE hDataContainerHandle, const BYTE * pAddr, int nLen, BOOL bServiceLimitFlag, DWORD dwHMinFre, DWORD dwHMaxFre, DWORD dwVMinFre, DWORD dwVMaxFre, BOOL bDefaultLNBMask, BOOL ucDefaultLNBType, BOOL bDefaultLNBPowerMask, BYTE ucDefaultLNBPower, DWORD dwSymbol );


//��������ĳƵ�������н�Ŀ�Ľ���������еڶ��������������ǵ�ǰƵ��ľ����Ҳ�����ǵ�ǰƵ����ĳ��Ŀ�ľ��
HCSHANDLE CSUSPDMCreateIterateAllServiceBlgedTP( HCSHANDLE hContainerHandle, HCSHANDLE hDataHandle );

BOOL CSUSPDMSetServiceSatIDFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, DWORD dwSatID, BOOL bSatIDMask );


//��ȡ�Ѿ����ڵ�Iterate�ĸ���
int CSUSPDMGetExistAllIterateCount( HCSHANDLE hDataContainerHandle );

//����bouquet���
HCSHANDLE CSUSPDMCreateBouquetHandle(HCSHANDLE hContainerHandle);

HCSHANDLE CSUSPDMCreateIterateAllBouquet(HCSHANDLE hContainerHandle);

BOOL CSUSPDMSetBouquetFilter(HCSHANDLE hContainerHandle, HCSHANDLE hIterateHandle, HCSHANDLE hBouquetHandle, BOOL bMask);
BOOL CSUSPDMSetBouquetIDFilter(HCSHANDLE hContainerHandle, HCSHANDLE hIterateHandle, UINT nBouquetId, BOOL bMask);

BOOL CSUSPDMGetNextPrevServiceNumberArray(HCSHANDLE hContainerHandle, HCSHANDLE hSerIterate, int listType, int subType, int channelNumber, int numbers[], int count, int nextFlag);

BOOL CSUSPDMIsIterateChanged(HCSHANDLE hConatinerHandle, HCSHANDLE hSerIterate);
BOOL CSUSPDMSetIterateChangeState(HCSHANDLE hConatinerHandle, HCSHANDLE hSerIterate, BOOL changed);

/*
 *	���º���δʵ��
 */

// the function is the same as CSUSPDMCleanContainer. The third para is not used.
int CSUSPDMCleanFileSysContainer( HCSHANDLE hContainer, HCSHANDLE hFileStorageBlock, void* pVoid );
// the function is the same as CSUSPDMSyncToFlash. The third para is not used.
int CSUSPDMSyncFileSysContainerToFlash( HCSHANDLE hContainer, HCSHANDLE hFileStorageBlock, void* pVoid );
// sorted the service with enmSortType & the serivce_id
int CSUSPDMSortedByServiceID( CSUSPDMSortType enmSortType, HCSHANDLE hContainer );


HCSHANDLE CSUSPDMCreateIterateFlashAllSatellite( HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateFlashAllTransponder( HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateFlashAllService( HCSHANDLE hDataContainerHandle );

BOOL CSUSPDMRestructureIterate( HCSHANDLE hIterate );
BOOL CSUSPDMNeedRestructureIterate( HCSHANDLE hIterate );

/*
Function: ���ý�Ŀ�������������û��Զ���ıȽϺ������бȽ�����

Para:
	nSortType[in]:�������������ʽ���������
	pUserCompareFunc[in]:�������������ȽϺ���(������ǽ�Ŀ���)

PS:902256 added 

*/
BOOL CSUSPDMIterateUserSortService( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, int nSortType, USPDMCompareService	pUserCompareFunc);

/*
Function: ����ĳ��UserData�Ĺ�������

Para:
	pbMatch[in]:���������pbMatch
	pbMask[in]:���������pbMask
	pbNegate[in]:���������pbNegate
	nFilterDepth[in]:���������nFilterDepth

PS:902256 added 
*/
BOOL CSUSPDMSetUserDataFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle,const BYTE* pbMatch,const BYTE* pbMask,const BYTE* pbNegate, int nFilterDepth );

BOOL CSUSPDMSetUserFuncFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, USPDMFilterService fnFilterService, void* pUserData );

#ifdef __cplusplus
}
#endif


#endif

