#ifndef CreateContainerInterface_H
#define CreateContainerInterface_H

#include "CSUSPDMCommonType.h"
#include "cs_fsapi.h"

#ifdef __cplusplus
extern "C"
{
#endif

//创建Cable数据容器
HCSHANDLE CSUSPDMCreateCableContainer( HCSHANDLE hMemoryPool, DATACONTAINERPAR * pDATACONTAINERPAR, int nDATACONTAINERPARCount, HCSHANDLE hFlashBlock );
//创建地面数据容器
HCSHANDLE CSUSPDMCreateTerrestriaContainer( HCSHANDLE hMemoryPool, DATACONTAINERPAR * pDATACONTAINERPAR, int nDATACONTAINERPARCount, HCSHANDLE hFlashBlock );
//创建Satellite数据容器
HCSHANDLE CSUSPDMCreateSatelliteContainer( HCSHANDLE hMemoryPool, DATACONTAINERPAR * pDATACONTAINERPAR, int nDATACONTAINERPARCount, HCSHANDLE hFlashBlock );

//容器创建
/*CSUSPDMCreateContainer
1）接口描述：
	创建容器，仅仅是创建了一个容器对象，而没有读Flash中存储的数据到DM
2）参数说明：
与上面接口CSUSPDMCreateCableContainer或CSUSPDMCreateTerrestriaContainer相比增加了一个参数dwContainerType，其余参数意义完全相同
dwContainerType ：在DM头文件CSUSPDMParseData.h中定义了三个宏分别表示三种容器类型：
#define 		USPDM_C_CONTAINER		(0x01)   //有线机
#define 		USPDM_T_CONTAINER		(0x02)   //地面机
#define 		USPDM_S_CONTAINER		(0x04)   //卫星机*/

HCSHANDLE CSUSPDMCreateContainer( HCSHANDLE hMemoryPool, DWORD dwContainerType, DATACONTAINERPAR * pDATACONTAINERPAR, int nDATACONTAINERPARCount, HCSHANDLE hFlashBlock );
//释放数据容器
BOOL CSUSPDMFreeDataContainer( HCSHANDLE hDataContainer );


//设定用户自定义节目数据的长度（只能在Container创建之前调用，并且只能被调用一次）
//nBufLen[in]: 用户自定义数据的长度（最大不能超过64字节）
//nCoverOffset[in]: 在添加重复时覆盖的偏移地址（相对于用户自定义数据首地址）
//nCoverLen[in]: 覆盖的长度
//nStoreFlashLen[in]: 要存储Flash的用户自定义数据段的长度，从自定义字段的第一个字节算起.（如果调用第一个函数，那么这个字段默认为第一个参数的大小）
BOOL CSUSPDMSetUserDataLen( int nBufLen, int nCoverOffset, int nCoverLen );
BOOL CSUSPDMSetUserDataLenEx( int nBufLen, int nCoverOffset, int nCoverLen, int nStoreFlashLen );



/*
	设定是否把Service的ChannelID作为节目的关键字。在默认情况下，这个字段是节目的关键字。
*/
//设定节目的逻辑频掉号不作为节目的关键字（在Container创建之前调用）
BOOL CSUSPDMSetServiceChannelNumNotKey( void );
//设定节目的逻辑频掉号作为节目的关键字（在Container创建之前调用,这个为DM的默认值）
BOOL CSUSPDMSetServiceChannelNumIsKey();



/*
	以下函数需在容器初始化之前调用
	设定TP的关键字。
	在默认的情况下，卫星频点以频率，极化方式，所属卫星作为关键字；地面和有线频点以TSID和OriNetworkID作为关键字
*/
//是否采用用户配置的频点关键字
BOOL CSUSPDMSetTPKeyIsUserConfig( BOOL bIsUserConfig );
BOOL CSUSPDMGetTPKeyIsUserConfig();
//是否以TSID作为频点关键字：
BOOL CSUSPDMSetTPKeyTSID( BOOL bTSIDIsKey );
BOOL CSUSPDMGetTPKeyTSID();
//是否以OriNetworkID作为频点关键字：
BOOL CSUSPDMSetTPKeyOriNetworkID( BOOL bOriNetworkIDIsKey );
BOOL CSUSPDMGetTPKeyOriNetworkID();
//是否以频率作为频点关键字：
BOOL CSUSPDMSetTPKeyFre( BOOL bFreIsKey );
BOOL CSUSPDMGetTPKeyFre();
//是否以频偏作为频点关键字的调整
BOOL CSUSPDMSetTPKeyFreOffset( BOOL bFreOffsetIsKey, DWORD dwFreOffset );
BOOL CSUSPDMGetTPKeyFreOffset( BOOL * pbFreOffsetIsKey, DWORD * pdwFreOffset );

//添加接口，是否以SymbolRate作为频点关键字：
BOOL CSUSPDMSetTPKeySymbolRate( BOOL bSymbolRateIsKey );
BOOL CSUSPDMGetTPKeySymbolRate();

int USPDMLocalSetServiceChannelKeyFlag(int nChannelNumKeySelectFlag);
int USPDMLocalGetServiceChannelKeyFlag();

//此函数必须在所有DM接口之前调用才有效
//为了解决ATI平台Flash读取速度问题增加这个接口
//在其他平台（非文件系统的Flash）不要调用此接口
//参数：
//	0: 普通Flash读写（默认值）
//	1: 文件系统Flash的加速方式读写（用于ATI平台）
//返回：
//	nFlashType的值
int CSUSPDMSetContainerFlashType( int nFlashType );


//以下关于设定最大元素数量的函数，当用户没有调用相应函数时，
//表示没有最大容量限制

//设定最大节目个数
BOOL CSUSPDMSetMaxServiceNum( int nMaxNum, HCSHANDLE hDataContainer );
//获取用户设定的最大节目个数
int CSUSPDMGetMaxServiceNum( HCSHANDLE hDataContainer );

//设定最大TPS个数
BOOL CSUSPDMSetMaxTransponderSNum( int nMaxNum, HCSHANDLE hDataContainer );
//获取用户设定的最大TPS个数
int CSUSPDMGetMaxTransponderSNum( HCSHANDLE hDataContainer );

//设定最大TPT个数
BOOL CSUSPDMSetMaxTransponderTNum( int nMaxNum, HCSHANDLE hDataContainer );
//获取用户设定的最大TPT个数
int CSUSPDMGetMaxTransponderTNum( HCSHANDLE hDataContainer );

//设定最大TPC个数
BOOL CSUSPDMSetMaxTransponderCNum( int nMaxNum, HCSHANDLE hDataContainer );
//获取用户设定的最大TPC个数
int CSUSPDMGetMaxTransponderCNum( HCSHANDLE hDataContainer );

//设定最大卫星个数
BOOL CSUSPDMSetMaxSatelliteNum( int nMaxNum, HCSHANDLE hDataContainer );
//获取用户设定的最大卫星个数
int CSUSPDMGetMaxSatelliteNum( HCSHANDLE hDataContainer );

//设定最大天线个数
BOOL CSUSPDMSetMaxAntennaNum( int nMaxNum, HCSHANDLE hDataContainer );
//获取用户设定的最大天线个数
int CSUSPDMGetMaxAntennaNum( HCSHANDLE hDataContainer );

//设定最大关系个数
BOOL CSUSPDMSetMaxRelationNum( int nMaxNum, HCSHANDLE hDataContainer );
//获取用户设定的最大关系个数
int CSUSPDMGetMaxRelationNum( HCSHANDLE hDataContainer );

//设定最大的Iterator个数
BOOL CSUSPDMSetMaxIteratorNum( int nMaxNum, HCSHANDLE hDataContainer );
//获取用户设定的最大Iterator个数
int CSUSPDMGetMaxIteratorNum( HCSHANDLE hDataContainer );

BOOL CSUSPDMSetAutoServiceChannelIDDispatchFlag( BOOL bAutoChannelIDFlag, WORD wMinID, WORD wMaxID );
BOOL CSUSPDMGetAutoServiceChannelIDDispatchFlag( BOOL * pbAutoChannelIDFlag, WORD * pwMinID, WORD * pwMaxID );


//根据用户设定个各种数据的最大值，计算出用户需要分配
//给数据管理的合适的内存使用量和FLS使用量
BOOL CSUSPDMGetMemFlsComfortableSize( int * pnMemSize, int * pnFlsSize, HCSHANDLE hDataContainer );

//获取版本信息
int CSUSPDMGetVersionInfo( char * pcVersionInfo, int nSize );
//获取DM的内存属性（最小内存和最大内存）
BOOL CSUSPDMGetMemInfo( int * pnMin, int * pnMax );

/*	设定由内存空间换搜索速度标识
	传入参数为TRUE时，设定标识为真，会使搜索换台速度加快
	调用比不调用多占用频点个数×4字节的内存，而速度经测试如果总频点个数是1000个的话要快500ms+
	此接口要在调用创建容器接口后紧跟着调用此函数  */
BOOL CSUSPDMSetMemSwapTimeFlag( HCSHANDLE hDataContainerHandle, BOOL bFlag );


/*CSUSPDMSetIsAttendUniNum
1）接口描述：
设置增加Audio_Type字段标志，即不关注Service属性中UniNum字段。在用接口CSUSPDMCreateContainer初始化Container后立即调用
如果没有调用此函数则DM内部关注UniNum字段，按原始Audio_Type字段长度为2个bit的处理方式
如果调用了此函数并且传入的参数为FALSE，那么说明在此工程中不关注Service属性中的UniNum字段属性，如果此时再在工程中调用与UniNum相关的接口就会有断言输出并返回FALSE，比如以下接口：
CSUSPDMGetServiceUniNum、CSUSPDMSetServiceUniNum、CSUSPDMCreateIterateUniNumService
2）参数说明：
hDataContainerHandle：输入参数，容器句柄，此容器句柄必须是调用接口CSUSPDMCreateContainer创建的容器句柄
bFlag：输入参数，为FALSE，说明不关注节目中的UniNum字段，需要将Audio_Type字段增加到8个bit。为TRUE，说明关注节目中的UniNum字段，将Audio_Type字段还是按原始2个bit处理
*/
BOOL CSUSPDMSetIsAttendUniNum( HCSHANDLE hDataContainerHandle, BOOL bIsAttendUniNum );

BOOL CSUSPDMSetClFlag( HCSHANDLE hDataContainer, BOOL bFlag );

HCSHANDLE CSUSPDMCreateFileSysContainer(HCSHANDLE hMemoryPool, DWORD dwContainerType, const char *pcDir);
BOOL CSUSPDMGetContainerHandle(HCSHANDLE **ppContainerHandle, int *pnContainerNum);

/*CSUSPDMSetCSFSFuns
1）接口描述：
针对DM 用文件系统写FLASH ，为避免用不到的项目编译链接不过，特增加此接口
将对文件的打开、关闭、读写、定位接口以参数的形式设置进DM

用到文件系统数据管理(CSUSPDMCreateFileSysContainer)的项目必须调用此方法来设置，否则读写Flash会失败
此接口需要在数据管理初始化之前调用

2）参数说明：
pvFSP[] : 输入参数，函数数组，如果用coiship文件系统，其值必须为void *pvFSP[] = {CSFSClose,CSFSOpen,CSFSRead,CSFSLseek,CSFSWrite};
nFucCount: 输入参数，意义是第一个参数数组的长度，在03.00.02版本的数据管理中，此值为5
*/
BOOL CSUSPDMSetCSFSFuns(CSFSOperation_t* pFucPort);

//added for yangjunjie 08.12.19
BOOL CSUSPDMLoadServiceFromFlash(HCSHANDLE hDataContainerHandle, BOOL bLoad );

/*CSUSPDMTakeSpace
1）接口描述：
据天威hmc2.0的需求，针对DM 用文件系统写FLASH ，为避免开机无节目情况空间可能被应用占满的情况
添加此接口，通过在DM初始化时调用此接口先占住足够空间供保存节目使用

此接口在CSUSPDMCreateFileSysContainer 之后调用，调用之前做好先调用CSUSPDMSetMaxServiceNum ，CSUSPDMSetMaxTransponderCNum等接口
设置节目最大个数等信息，并尽量设置合理数值，否则会造成资源浪费，因为内部实现是通过最大个数决定预留空间的

2）参数说明：
hDataContainerHandle : 输入参数，容器句柄，通过CSUSPDMCreateFileSysContainer创建
*/
BOOL CSUSPDMTakeSpace( HCSHANDLE hDataContainerHandle );
#ifdef __cplusplus
}
#endif

#endif
