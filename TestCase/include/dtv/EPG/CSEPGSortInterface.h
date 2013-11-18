#ifndef CSEPGSortInterface_H
#define CSEPGSortInterface_H


#include "EPGReceiverInterface.h"
#include "EPGUserInterface.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum _EPGLevelCallbackType
{
	//插入ServiceHandle的回调类型
	EPGLEVEL_INSERT_USERDATA
	,EPGLEVEL_BUILDDATA_BEGIN	//开始build数据
	,EPGLEVEL_BUILDDATA_END		//结束build数据
}EPGLevelCallbackType;

typedef enum _DurationType
{
    EPGSORT_INCLUDE_ALL,          //与设定的Duration有交叉部分的全部Event（默认处理状态）
    EPGSORT_EXCLUDE_START_NOT_IN  //排除开始时间不在设定的Duration内的Event
}EPGSortDurationType;

//回调函数
typedef DWORD(*EPGLevelGCallBack)( EPGLevelCallbackType enmType, void* pParam );

/*==================================================
功能：创建Schedule EPG 分类句柄
===================================================*/
HCSHANDLE CSEPGScheduleLevelInit( HCSHANDLE hEPG, HCSHANDLE hMemoryPool, EPGLevelGCallBack pCallBack );

/*==================================================
功能：创建Simple EPG 分类句柄
===================================================*/
HCSHANDLE CSEPGSimpleLevelInit( HCSHANDLE hEPG, HCSHANDLE hMemoryPool, EPGLevelGCallBack pCallBack );

/*==================================================
功能：创建Simple follow EPG 分类句柄
===================================================*/
HCSHANDLE CSEPGSimpleFollowLevelInit( HCSHANDLE hEPG, HCSHANDLE hMemoryPool, EPGLevelGCallBack pCallBack );

/*==================================================
功能：销毁分类句柄，Schedule和Simple通用
===================================================*/
BOOL CSEPGLevelDestroy( HCSHANDLE hEPGLevelHandle );

/*==================================================
功能：删除所用的分类信息，Schedule和Simple通用
===================================================*/
BOOL CSEPGLevelDeleteAllDate( HCSHANDLE hEPGLevelHandle );

/*==================================================
功能：对Schedule句柄设置时间段，对Schedule分类有用
参数：
   pDurTime 间隔时间，两个字节开始时间的MJD, 三个字节为开始时间的UTC,
            最后三个字节为间隔时间的UTC。此时间是标准时间，没有加时区。
   nTimeLen pDurTime的长度，应设置为8。
===================================================*/
BOOL CSEPGLevelSetDuration( HCSHANDLE hScheduleLevelHandle, BYTE * pDurTime, int nTimeLen );

/*==================================================
功能：重建EPG分类数据表，首先删除所有数据，再重新构建数据表，
      Schedule和Simple通用。根据分类句炳加以区别。
===================================================*/
BOOL CSEPGLevelRebuild( HCSHANDLE hEPGLevelHandle );

/*==================================================
功能：对指定分类执行排序，Schedule和Simple通用
===================================================*/
BOOL CSEPGLevelSort( HCSHANDLE hEPGLevelHandle, int nContentLevel1 );

/*==================================================
功能：获取指定分类的节点个数，Schedule和Simple通用
===================================================*/
int CSEPGGetLevel1Count( HCSHANDLE hEPGLevelHandle, int nContentLevel1 );

/*==================================================
功能：获取指定分类指定索引的数据内容，Schedule和Simple通用
参数：
   [in]nIndex：索引值
   [out]pEPGInfo: EPG数据
   [out]pdwUserData: 用户数据，例如ServiceHandle
返回值：
   等于FALSE，表示数据不可用。
===================================================*/
BOOL CSEPGLevelGetEPGbyIndex( HCSHANDLE hEPGLevelHandle, int nContentLevel1, int nIndex, EPGINFO * pEPGInfo, DWORD * pdwUserData );

/*==================================================
功能：将EPG模块回调的Simple EPG数据插入Simplw分类句柄，对Simple有用。
参数
   pData：EPG返回的结构体。
   dwUserData：用户数据，ServiceHandle.
注意：用户数据ServcieHandle一定要传入，分类模块使用它做插入关键字。
===================================================*/
BOOL CSEPGSimpleLevelInsert( HCSHANDLE hSimpleLevelHandle, EPGContentTypeData * pData, DWORD dwUserData );


/*==================================================
功能：获取指定二级分类的节点个数，Schedule和Simple通用
===================================================*/
int CSEPGGetLevel2Count( HCSHANDLE hEPGLevelHandle, int nContentLevel1,int nContentLevel2 );
//////////////////////////////////////////////////////////////////////////
/*==================================================
功能：获取指定二级分类指定索引的数据内容，Schedule和Simple通用
参数：
[in]nContentLevel1:一级分类
[in]nContentLevel2:二级分类
[in]nIndex：索引值
[out]pEPGInfo: EPG数据
[out]pdwUserData: 用户数据，例如ServiceHandle
返回值：
等于FALSE，表示数据不可用。
===================================================*/
BOOL CSEPGLevel2GetEPGbyIndex( HCSHANDLE hEPGLevelHandle, int nContentLevel1,int nContentLevel2, int nIndex, EPGINFO * pEPGInfo, DWORD * pdwUserData );
//////////////////////////////////////////////////////////////////////////

/*==================================================
功能：对Schedule句柄设置时间段，对Schedule分类有用
参数：
    [in]hScheduleLevelHandle:   Schedule句柄
    [in]enmDurationType      :   设置持续时间的类型，
                                可取值：EPGSORT_INCLUDE_ALL（与设定的Duration有交叉部分的全部Event（默认处理状态））,
                                        EPGSORT_EXCLUDE_START_NOT_IN(排除开始时间不在设定的Duration内的Event),
                                        
===================================================*/
BOOL CSUSPEPGLevelSetDurationType( HCSHANDLE hScheduleLevelHandle, EPGSortDurationType enmDurationType );

/*==================================================
功能：获取模块版本号。
===================================================*/
int CSEPGSortGetVersionInfo( char * pcVersionInfo, int nSize );

#ifdef  __cplusplus
}
#endif


#endif /* CSEPGSortInterface_H */
