#ifndef CreateIterateInterface_H
#define CreateIterateInterface_H

#include "CSUSPDMCommonType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/**
@brief DM中数据变化写flash时的回调通知
@param[in] nDataType 发生变化的数据类型，取值范围为
				USPDM_ANTENNA_HANDLE	= 0		
				USPDM_SATELLITE_HANDLE  = 1		
				USPDM_T_TRANSPONDER_HANDLE =2		
				USPDM_S_TRANSPONDER_HANDLE =3			
				USPDM_C_TRANSPONDER_HANDLE =4		
				USPDM_SERVICE_HANDLE =5			
				USPDM_ANTSATRELATION_HANDLE =6		
			具体详情见CSUSPDMParseData.h中关于这几个常量的定义
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
*/
typedef void (*CSOnUSPDMFlashDataChange_F) (int nDataType, void* pvUserData);

/**
@brief 添加监听DM中数据变化写flash的回调函数
@param[in] fnCallback 注册的回调函数
@param[in] pvUserdata 用户数据
@note 最多可以允许注册8个回调函数,如果注册失败返回FALSE
*/
BOOL CSUSPDMAddFlashDataChangeCallback(CSOnUSPDMFlashDataChange_F fnCallback, void* pvUserdata);

/**
@brief 删除监听DM中数据变化写flash的回调函数
@param[in] fnCallback 注册的回调函数
*/
BOOL CSUSPDMRemoveFlashDataChangeCallback(CSOnUSPDMFlashDataChange_F fnCallback);

/*
 *	Iterator 相关操作
 */

// 创建 用于遍历RAM中所有关系的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateAllAntSatRelation( HCSHANDLE hDataContainerHandle );
// 创建 用于遍历RAM中所有卫星的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateAllSatellite( HCSHANDLE hDataContainerHandle );
// 创建 用于遍历RAM中所有天线的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateAllAntenna( HCSHANDLE hDataContainerHandle );
// 创建 用于遍历RAM中所有TP的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateAllTransponder( HCSHANDLE hDataContainerHandle );
// 创建 用于遍历RAM中隶属于指定卫星,制定天线的所有TP的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateAllTransponderS( DWORD dwSatelliteID, BOOL bSatelliteIDMask, DWORD dwAntennaID, BOOL bAntennaIDMask, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateAllTPC( HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateAllTPT( HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateAllTPS( HCSHANDLE hDataContainerHandle );

//排序结果集的参数说明：
//	nSortKey：0 ServiceID，1 ServiceName，2 Service CA Mode，3 ServiceChannelNum，4 service lock state,5：tp_id+ser_id, 6: 节目所属的 tp_id
//  nSortType：1 升序；2 降序
//

//按照节目所属频点的TSID+ServiceID进行排序
//	所有用于创建卫星下非排序的结果集，把卫星的ID设定为0x00030000，那么就是按照TSID+ServiceID排序。这种方式对卫星容器，地面容器，有线容器都有效。例如：
//		CSUSPDMCreateIterateSatelliteService( 0x00030000, 0xff, TRUE, FALSE, hContainer );
//

//卫星下的结果集说明
//	所有用于创建卫星下节目的非排序结果集函数，都遵循如下规则：
//		1. dwSatelliteID的低16位代表真实的卫星ID，因为所有的结点的ID号都是13位；
//		2. dwSatelliteID的高16位代表不同的排序方式。具体的规定如下所述：
//		如果dwSatelliteID的低16位为0xffff，那么表示结果集是所有卫星下的节目；
//		如果dwSatelliteID的高16位为0x0000，那么表示按照节目在Flash中的物理顺序排序；
//		如果dwSatelliteID的高16位为0x0001，那么表示按照节目所属的TPFre关键字进行升序排序；
//		如果dwSatelliteID的总共32位为0xffffffff，那么表示得到所有卫星下的按照所属的TPFre关键字进行升序排序（为了前向兼容）；
//		如果dwSatelliteID的高16位为其他值未定义,请不要使用.
//
//	例子：以函数 CSUSPDMCreateIterateSatelliteService 为例
//			如此调用 CSUSPDMCreateIterateSatelliteService（0x00000001, 0xff, FALSE, FALSE, hContainer );
//				表示得到卫星ID为0x0001的卫星下的所有节目，以节目的物理顺序为序；
//			如此调用 CSUSPDMCreateIterateSatelliteService（0x00010001, 0xff, FALSE, FALSE, hContainer );
//				表示得到卫星ID为0x0001的卫星下的所有节目，以节目所属TP的Fre为序（从小到大）；
//			如此调用 CSUSPDMCreateIterateSatelliteService（0x0001ffff, 0xff, FALSE, FALSE, hContainer );
//				表示得到所有卫星下的所有节目，以节目所属TP的Fre为序（从小到大）；
//			如此调用 CSUSPDMCreateIterateSatelliteService（0xffffffff, 0xff, FALSE, FALSE, hContainer );
//				表示得到所有卫星下的所有节目，以节目所属TP的Fre为序（从小到大）；
//	 以上函数的不同参数导致的速度差异不大。
//
//	注：其他适用的创建结果集的函数包括但不限于如下所列（可能会有新加接口没有包含进来）：
//			CSUSPDMCreateIterateServiceBelongedSatellite
//			CSUSPDMCreateIterateFreeModeSerBlgedSat
//			CSUSPDMCreateIterateSatelliteService
//			CSUSPDMCreateIterateSatelliteFavFreeService
//			CSUSPDMCreateIterateSatelliteFavFreeLockService
//		即所有的需要传入dwSatelliteID但是非排序的结果集都适用。
//	
//	另:
//		为什么没有采用通用的SortKey,SortType作为卫星下节目的TPFre关键字的排序的接口方式,主要是
//		为了排序的效率考虑.为了使5000套节目的某个(所有)卫星下的节目按照TPFre排序的时间效率以ms
//		计算，我们决定采用这种接口形式。另外采用这种方式可以在目前的构架下减少Iterate创建函数的
//		数量。请知悉。
//



// 创建 用于遍历存于RAM中的所有Service数据的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateAllService( HCSHANDLE hDataContainerHandle, BYTE bServiceType );
// 创建 用于遍历存于RAM中的所有指定喜好类型Service数据的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateFAVService( HCSHANDLE hDataContainerHandle, int nFAVType, BYTE bServiceType );
// 创建 用于遍历存于RAM中的指定加锁状态的Service数据的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateLockService( HCSHANDLE hDataContainerHandle, BOOL bLockState, BYTE bServiceType );
// 创建 用户指定排序方式的所有节目的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey , int nSortType , BYTE bServiceType );
// 创建 用户指定排序方式，指定FAV属性的所有节目的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateFAVServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, int nFAVIndex, BYTE bServiceType );
// 创建 用户指定排序方式，指定加锁状态的所有节目的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateLockServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, BOOL bLockState, BYTE bServiceType );
// 创建 用于遍历所有加密（免费）的节目的 搜索句柄
HCSHANDLE CSUSPDMCreateIteateFreeService( BOOL bFreeMode, BYTE bServiceType, HCSHANDLE hContainer );
// 创建 搜索所有节目的搜索句柄，按照卫星（第一关键字），TP（第二关键字）排序
HCSHANDLE CSUSPDMCreateIterateSatTPServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, BYTE bServiceType );
// 创建 隶属于用户指定的卫星的所有节目的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateServiceBelongedSatellite( DWORD dwSatelliteID, BYTE bServiceType, HCSHANDLE hDataContainerHandle );
// 创建 隶属于用户指定的卫星的所有节目，并且按照用户指定方式排序
HCSHANDLE CSUSPDMCreateIterateSerBlgedSatSorted( DWORD dwSatelliteID, BYTE bServiceType, int nSortKey, int nSortType, HCSHANDLE hDataContainerHandle );
// 创建 隶属于用户指定的卫星，指定加密类型的所有节目
HCSHANDLE CSUSPDMCreateIterateFreeModeSerBlgedSat( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, HCSHANDLE hDataContainerHandle );
// 创建 隶属于用户指定的卫星，指定加密类型的所有节目，并且按照用户指定方式排序
HCSHANDLE CSUSPDMCreateIterateFreeModeSerBlgedSatSorted( DWORD dwSatelliteID, BYTE bServiceType, int nSortKey, int nSortType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, HCSHANDLE hDataContainerHandle );
// 以数据管理中物理顺序为顺序获取某科卫星下的所有节目
HCSHANDLE CSUSPDMCreateIterateSatelliteService( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, HCSHANDLE hDataContainerHandle );
// 创建 加密（非加密）节目的排序方式的结果集
HCSHANDLE CSUSPDMCreateIterateFreeServiceSorted( HCSHANDLE hDataContainerHandle, int nSortKey, int nSortType, BOOL bFreeType, BYTE bServiceType );
// 创建 某种加密类型节目的结果集
HCSHANDLE CSUSPDMCreateIterateCATypeService( HCSHANDLE hDataContainerHandle, WORD wCAType, BYTE bServiceType );

// 创建 隶属于用户指定的卫星，指定加密类型, 指定FAV 的所有节目
HCSHANDLE CSUSPDMCreateIterateSatelliteFavFreeService( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, WORD wFAVLevel, BOOL bFAVLevelMask, HCSHANDLE hDataContainerHandle );
// 创建 隶属于用户指定的卫星，指定加密类型, 指定FAV 的所有带有排序的 节目
HCSHANDLE CSUSPDMCreateIterateSatFavFreeServiceSorted( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, WORD wFAVLevel, BOOL bFAVLevelMask, int nSortKey, int nSortType, HCSHANDLE hDataContainerHandle );
// 创建 隶属于用户指定的卫星，指定加密类型, 指定FAV 指定LockState 的所有节目
HCSHANDLE CSUSPDMCreateIterateSatelliteFavFreeLockService( DWORD dwSatelliteID, BYTE bServiceType, BOOL bFreeCAMode, BOOL bFreeCAModeMask, WORD wFAVLevel, BOOL bFAVLevelMask, BOOL bLockState, BOOL bLockStateMask, HCSHANDLE hDataContainerHandle );


// 创建 节目名称中包含指定字符串的所有节目的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateSimilarNameService( HCSHANDLE hDataContainerHandle, const BYTE * pServiceName, int nServiceNameLen, BOOL bCaseSensitive, BYTE bServiceType );
// 重新设定 IterateSimilarNameService 的指定字符串
BOOL CSUSPDMResetSimilarName( HCSHANDLE hSimilarNameIterate, const BYTE * pServiceName, int nNameLen );

// 创建 节目名称中包含指定字符串的所有节目的 搜索句柄（排序方式）
HCSHANDLE CSUSPDMCreateIterateSimilarNameServiceSorted( HCSHANDLE hDataContainerHandle, const BYTE * pServiceName, int nServiceNameLen, BOOL bCaseSensitive, BYTE bServiceType, int nSortKey, int nSortType );
// 重新设定 IterateSimilarNameServiceSorted 的指定字符串（排序方式）
BOOL CSUSPDMResetSimilarNameSorted( HCSHANDLE hSimilarNameIterateSorted, const BYTE * pServiceName, int nNameLen );

//nFAV: 0(得到所有FAV属性； 1~16：FAV等级； 17：某个FAV等级
HCSHANDLE CSUSPDMCreateIterateFAVServicePlus( HCSHANDLE hDataContainerHandle, int nFAVType, BYTE bServiceType );
//得到某种FAVLevel，并且是加密/免费的节目的结果集（bFAVLevelMask设定为FALSE，则wFAVLevel不起作用；bFreeCAModeMask设定为FALSE，则bFreeCAMode不起作用）
HCSHANDLE CSUSPDMCreateIterateFAVFreeService( HCSHANDLE hDataContainerHandle, WORD wFAVLevel, BOOL bFAVLevelMask, BOOL bFreeCAMode, BOOL bFreeCAModeMask, BYTE bServiceType );

// 创建用户自定义数据的结果集
HCSHANDLE CSUSPDMCreateIterateUserDataService( const BYTE * pbMatch, const BYTE * pbMask, const BYTE * pbNegate, int nFilterDepth, BYTE bServiceType, HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateIterateUserDataServiceSorted( const BYTE * pbMatch, const BYTE * pbMask, const BYTE * pbNegate, int nFilterDepth, BYTE bServiceType, int nSortKey, int nSortType, USPDMCompareUserServiceData pUserCompareFunc, HCSHANDLE hDataContainerHandle );

//创建所有UniNum的值不为0的节目的结果集（用于支持RAPS）
HCSHANDLE CSUSPDMCreateIterateUniNumService( HCSHANDLE hDataContainerHandle, BYTE bServiceType );

//以下接口创建的结果集会过滤掉所有Skip标志的节目
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

// 释放 搜索句柄
BOOL CSUSPDMFreeIteratorHandle( HCSHANDLE hIteratorHandle, HCSHANDLE hContainer );


// 获取指定搜索句柄的第一个数据句柄
HCSHANDLE CSUSPDMGetHeadDataHandle( HCSHANDLE hIterateHandle );
// 获取指定搜索句柄的最后一个数据句柄
HCSHANDLE CSUSPDMGetTailDataHandle( HCSHANDLE hIterateHandle );
// 获取指定搜索句柄的当前数据句柄的上一个数据句柄
HCSHANDLE CSUSPDMGetPrevDataHandle( HCSHANDLE hIterateHandle );
// 获取指定搜索句柄的当前数据句柄的下一个数据句柄
HCSHANDLE CSUSPDMGetNextDataHandle( HCSHANDLE hIterateHandle );
// 获取搜索句柄指定索引的数据句柄
HCSHANDLE CSUSPDMFindIndexDataHandle( int nIndex, HCSHANDLE hIterateHandle );
// 获取搜索句柄中数据的总个数
int CSUSPDMGetDataHandleCount( HCSHANDLE hIterateHandle );
// 获取指定数据句柄在搜索句柄中的索引位置
int CSUSPDMGetCurDataHandleIndex( HCSHANDLE hIterateHandle );
// 设定搜索句柄当前的搜索位置
BOOL CSUSPDMSetCurDataHandleIndex( HCSHANDLE hIterateHandle, int nIndex );


/*
 *	添加单个数据的相关操作
 */


// 在数据容器的内存池中Satellite数据句炳（仅限卫星数据容器）
HCSHANDLE CSUSPDMCreateSatelliteDataHandle( HCSHANDLE hDataContainerHandle );
// 在数据容器的内存池中天线数据句炳（仅限卫星数据容器）
HCSHANDLE CSUSPDMCreateAntennaDataHandle( HCSHANDLE hDataContainerHandle );
// 在数据容器的内存池中创建Transponder数据句炳(根据container的不同创建不同的数据句柄）
HCSHANDLE CSUSPDMCreateTransponderDataHandle( HCSHANDLE hDataContainerHandle );
// 在数据容器的内存池中创建TransponderS数据句炳（仅限卫星数据容器）
HCSHANDLE CSUSPDMCreateTransponderSDataHandle( HCSHANDLE hDataContainerHandle );
// 在数据容器的内存池中TransponderC数据句炳（仅限有线数据容器）
HCSHANDLE CSUSPDMCreateTransponderCDataHandle( HCSHANDLE hDataContainerHandle );
// 在数据容器的内存池中TransponderT数据句炳（仅限地面数据容器）
HCSHANDLE CSUSPDMCreateTransponderTDataHandle( HCSHANDLE hDataContainerHandle );
HCSHANDLE CSUSPDMCreateTransponderVDataHandle(HCSHANDLE hDataContainerHandle);
// 在数据容器的内存池中Service数据句炳
HCSHANDLE CSUSPDMCreateServiceDataHandle( HCSHANDLE hDataContainerHandle );
// 释放数据容器的数据句炳
BOOL CSUSPDMFreeDataHandle( HCSHANDLE hDataHandle, HCSHANDLE hDataContainerHandle );

//获取某个ChannelNo的ServiceHandle
HCSHANDLE CSUSPDMGetServiceHandleByChannelNum( WORD wChannelNo, HCSHANDLE hDataContainerHandle );




// 把数据句柄添加到指定的容器的头位置
BOOL CSUSPDMAddHeadDataHandle( HCSHANDLE hDataHandle, HCSHANDLE hDataContainerHandle );
// 把数据句柄添加到容器的尾位置
BOOL CSUSPDMAddTailDataHandle( HCSHANDLE hDataHandle, HCSHANDLE hDataContainerHandle );
// 把数据句柄添加到容器的指定位置
BOOL CSUSPDMInsertDataHandle( HCSHANDLE hPosDataHandle, HCSHANDLE hInsertDataHandle, HCSHANDLE hDataContainerHandle );
// 把数据句柄移动到容器的指定位置
BOOL CSUSPDMMoveDataHandle( HCSHANDLE hPosDataHandle, HCSHANDLE hMovedDataHandle, HCSHANDLE hDataContainerHandle );
// 交换节目的位置
BOOL CSUSPDMSwapServiceDataHandle( HCSHANDLE hServiceA, HCSHANDLE hServiceB, HCSHANDLE hDataContainerHandle );

// 按照用户指定关键字顺序插入节目数据 (sort key::service id, 1:service name, 2:ser ca mode, 3: channel num, 8:ser user data)(sort type: 1,升序；其他，降序)
DWORD CSUSPDMInsertDataHandleSorted( HCSHANDLE hDataContainerHandle, HCSHANDLE hServiceHandle, int nSortKey, int nSortType );
// 对节目的物理链表排序，按照用户指定的比较函数和排序方式
BOOL CSUSPDMSortAllService( HCSHANDLE hContainer, USPDMCompareService pCompareFun, CSUSPDMSortType enmSortType );

// 把数据句柄从容器移出（没有释放）
BOOL CSUSPDMRemoveDataHandle( HCSHANDLE hDataHandle, HCSHANDLE hDataContainerHandle );
// 根据Service的Delete标志删除并释放所有Delete标志为TRUE的节目（没有同步Flash），如需同步Flash，请调用相关函数
BOOL CSUSPDMDeleteAllServiceByDelFlag( HCSHANDLE hDataContainerHandle );
// 恢复默认，从pAddr除读取数据
BOOL CSUSPDMRestoreDefaultData( HCSHANDLE hDataContainerHandle, int nType, const BYTE * pAddr, int nLen );
// 恢复默认，从pAddr除读取数据, 添加Service限制
BOOL CSUSPDMRestoreDefaultDataEx( HCSHANDLE hDataContainerHandle, const BYTE * pAddr, int nLen, BOOL bServiceLimitFlag, DWORD dwHMinFre, DWORD dwHMaxFre, DWORD dwVMinFre, DWORD dwVMaxFre, BOOL bDefaultLNBMask, BYTE ucDefaultLNBType, BOOL bDefaultLNBPowerMask, BYTE ucDefaultLNBPower );

// 批量移动数据
//haHandleArray[]: 要移动的Service句柄数组
//nHandleCount: 数组元素个数
//nPosHandle: 位置元素句柄
BOOL CSUSPDMMoveBatchDataHandle( HCSHANDLE hDataContainerHandle, HCSHANDLE haHandleArray[], int nHandleCount, HCSHANDLE nPosHandle );
BOOL CSUSPDMMoveBatchDataHandleExt( HCSHANDLE hDataContainerHandle, HCSHANDLE nPosHandle );
//批量移动数据。
//	如果把第三个参数设定为1，则移动之后不改变任何节目的移动标志位；
//	设定为0，移动之后自动清空节目的移动标志位（具体动作跟CSUSPDMMoveBatchDataHandleExt相同）
BOOL CSUSPDMMoveBatchDataHandleAd( HCSHANDLE hDataContainerHandle, HCSHANDLE nPosHandle, DWORD dwMoveType );


// 把数据容器里内存中的数据同步到存储体
BOOL CSUSPDMSyncToFlash( HCSHANDLE hDataContainerHandle );
// 把由 hStorageHandle 指示的数据从内存同步到Flash
BOOL CSUSPDMRefresh( HCSHANDLE hStorageHandle );

//删除某颗卫星下的所有节目（没有同步Flash）
BOOL CSUSPDMDeleteServiceBlgedSat( HCSHANDLE hDataContainerHandle, HCSHANDLE hSatellite );
//删除某颗转发器下的所有节目（没有同步Flash）
BOOL CSUSPDMDeleteServiceBlgedTP( HCSHANDLE hDataContainerHandle, HCSHANDLE hTransponder );
//删除某颗卫星下的所有TP（会连带删除TP下的所有节目）（没有同步Flash）
BOOL CSUSPDMDeleteTPBlgedSat( HCSHANDLE hDataContainerHandle, HCSHANDLE hSatellite );


//删除所有节目（没有同步Flash）
BOOL CSUSPDMDeleteAllService( HCSHANDLE hDataContainerHandle );
// 清空Container中的所有节目和频点
BOOL CSUSPDMCleanContainer( HCSHANDLE hDataContainerHandle );
// 用Flash中的数据替换内存中的数据（节目，频点，卫星，天线）
BOOL CSUSPDMReLoadAllData( HCSHANDLE hDataContainerHandle );


// 创建关系数据句柄
HCSHANDLE CSUSPDMCreateAntSatRelationDataHandle( HCSHANDLE hDataContainerHandle );
// 查找关系数据句柄
HCSHANDLE CSUSPDMFindAntSatRelation( DWORD dwSatelliteID, DWORD dwAntennaID, HCSHANDLE hContainer );
// 删除用户指定的某一关系
BOOL CSUSPDMDeleteAntSatRelation( DWORD dwSatelliteID, DWORD dwAntennaID, HCSHANDLE hContainer );
// 直接添加一个关系到数据容器
DWORD CSUSPDMAddAntSatRelation( DWORD dwSatelliteID, DWORD dwAntennaID, HCSHANDLE hContainer );
// 获取关系句柄的相关字段
int CSUSPDMGetAntSatRelationData( DWORD * pdwSatelliteID, DWORD * pdwAntennaID, HCSHANDLE hRSDataHandle );
// 设定关系句柄的相关字段
int CSUSPDMSetAntSatRelationData( DWORD dwSatelliteID, DWORD dwAntennaID, HCSHANDLE hRSDataHandle );

// 创建 用于遍历所有地面节目的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateAllTerService( BOOL bFreeMode, BOOL bFreeModeMask, BYTE bServiceType, HCSHANDLE hDataContainerHandle );

// 创建 用于遍历所有卫星的节目的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateAllSatService( BOOL bFreeMode, BOOL bFreeModeMask, BYTE bServiceType, HCSHANDLE hDataContainerHandle );

// 创建 用于遍历所有有线的节目的 搜索句柄
HCSHANDLE CSUSPDMCreateIterateAllCabService( BOOL bFreeMode, BOOL bFreeModeMask, BYTE bServiceType, HCSHANDLE hDataContainerHandle );

/*
Function: 设定节目所属频点类型的过滤条件

Para:
	hDataContainerHandle[in]：输入参数，Container句柄
	hIterateDataHandle[in]:输入参数，结果集句柄
	dwServiceTPType[in]:输入参数，节目所属频点类型

Return:
	SUCCESS: 返回TRUE
	FAILURD: 返回FALSE

Description:
	1) 
	2) 
*/
BOOL CSUSPDMSetServiceTPTypeFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, DWORD dwServiceTPType );

/*
Function: 设定节目加锁状态过滤条件

Para:
	bLockState[in]:加锁状态，TURE为加锁的，FALSE为不加锁的

Return:
	SUCCESS: 返回TRUE
	FAILURD: 返回FALSE
 */
BOOL CSUSPDMSetServiceLockStateFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, BOOL bLockState );

/*
Function: 设定节目类型过滤条件

Para:
	ucServiceType[in]:输入参数，节目类型

 */
BOOL CSUSPDMSetServiceTypeFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, BYTE ucServiceType );

/*
Function: 设定UniNum = 0的过滤条件

Para:
	wUniNum[in]:输入参数，一般为0

Description:
	1) 保留UniNum的值不为0的节目（用于支持RAPS）
	2) 
*/
BOOL CSUSPDMSetUniNumFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, WORD wUniNum );

/* 
Function: 设定skip过滤条件

Para:
	bSkipMask[in]:输入参数，TURE为需要过滤，FALSE为不需要过滤(不调用此函数时默认为不需要过滤)

Description:
	1) 会过滤掉所有skip标记的节目
	2) 
*/
BOOL CSUSPDMSetSkipFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, BOOL bSkipMask );

/*
Function: 设置某种FAVlevel的过滤条件

Para:
	wFAVIndex[in]:输入参数，FAVLevle

*/
BOOL CSUSPDMSetFAVIndexFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, WORD wFAVIndex );

/* 
Function: 设定加密类型过滤条件

Para:
	wCAType[in]:输入参数，加密类型

*/
BOOL CSUSPDMSetCATypeFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, WORD wCAType );

/* 
Function: 将结果集中的节目按指定方式排序

Para:
	nSortKey[in]:输入参数，排序关键字
	nSortType[in]:输入参数，排序方式（升序或降序）
*/
BOOL CSUSPDMIterateSortService( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, int nSortKey, int nSortType );
BOOL CSUSPDMIterateSortServiceByUserFunc( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, USPDMCompareServiceEx	pUserCompareFunc, void* pUserData );

BOOL CSUSPDMRestoreDefaultDataForSymbol( HCSHANDLE hDataContainerHandle, const BYTE * pAddr, int nLen, BOOL bServiceLimitFlag, DWORD dwHMinFre, DWORD dwHMaxFre, DWORD dwVMinFre, DWORD dwVMaxFre, BOOL bDefaultLNBMask, BOOL ucDefaultLNBType, BOOL bDefaultLNBPowerMask, BYTE ucDefaultLNBPower, DWORD dwSymbol );


//创建属于某频点下所有节目的结果集，其中第二个参数即可以是当前频点的句柄，也可以是当前频点下某节目的句柄
HCSHANDLE CSUSPDMCreateIterateAllServiceBlgedTP( HCSHANDLE hContainerHandle, HCSHANDLE hDataHandle );

BOOL CSUSPDMSetServiceSatIDFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, DWORD dwSatID, BOOL bSatIDMask );


//获取已经存在的Iterate的个数
int CSUSPDMGetExistAllIterateCount( HCSHANDLE hDataContainerHandle );

//创建bouquet句柄
HCSHANDLE CSUSPDMCreateBouquetHandle(HCSHANDLE hContainerHandle);

HCSHANDLE CSUSPDMCreateIterateAllBouquet(HCSHANDLE hContainerHandle);

BOOL CSUSPDMSetBouquetFilter(HCSHANDLE hContainerHandle, HCSHANDLE hIterateHandle, HCSHANDLE hBouquetHandle, BOOL bMask);
BOOL CSUSPDMSetBouquetIDFilter(HCSHANDLE hContainerHandle, HCSHANDLE hIterateHandle, UINT nBouquetId, BOOL bMask);

BOOL CSUSPDMGetNextPrevServiceNumberArray(HCSHANDLE hContainerHandle, HCSHANDLE hSerIterate, int listType, int subType, int channelNumber, int numbers[], int count, int nextFlag);

BOOL CSUSPDMIsIterateChanged(HCSHANDLE hConatinerHandle, HCSHANDLE hSerIterate);
BOOL CSUSPDMSetIterateChangeState(HCSHANDLE hConatinerHandle, HCSHANDLE hSerIterate, BOOL changed);

/*
 *	以下函数未实现
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
Function: 设置节目排序条件，按用户自定义的比较函数进行比较排序。

Para:
	nSortType[in]:输入参数，排序方式（升序或降序）
	pUserCompareFunc[in]:输入参数，排序比较函数(传入的是节目句柄)

PS:902256 added 

*/
BOOL CSUSPDMIterateUserSortService( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, int nSortType, USPDMCompareService	pUserCompareFunc);

/*
Function: 设置某种UserData的过滤条件

Para:
	pbMatch[in]:输入参数，pbMatch
	pbMask[in]:输入参数，pbMask
	pbNegate[in]:输入参数，pbNegate
	nFilterDepth[in]:输入参数，nFilterDepth

PS:902256 added 
*/
BOOL CSUSPDMSetUserDataFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle,const BYTE* pbMatch,const BYTE* pbMask,const BYTE* pbNegate, int nFilterDepth );

BOOL CSUSPDMSetUserFuncFilter( HCSHANDLE hContainerDataHandle, HCSHANDLE hIterateDataHandle, USPDMFilterService fnFilterService, void* pUserData );

#ifdef __cplusplus
}
#endif


#endif

