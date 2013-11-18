#ifndef CSEPGSortInterface_H
#define CSEPGSortInterface_H


#include "EPGReceiverInterface.h"
#include "EPGUserInterface.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum _EPGLevelCallbackType
{
	//����ServiceHandle�Ļص�����
	EPGLEVEL_INSERT_USERDATA
	,EPGLEVEL_BUILDDATA_BEGIN	//��ʼbuild����
	,EPGLEVEL_BUILDDATA_END		//����build����
}EPGLevelCallbackType;

typedef enum _DurationType
{
    EPGSORT_INCLUDE_ALL,          //���趨��Duration�н��沿�ֵ�ȫ��Event��Ĭ�ϴ���״̬��
    EPGSORT_EXCLUDE_START_NOT_IN  //�ų���ʼʱ�䲻���趨��Duration�ڵ�Event
}EPGSortDurationType;

//�ص�����
typedef DWORD(*EPGLevelGCallBack)( EPGLevelCallbackType enmType, void* pParam );

/*==================================================
���ܣ�����Schedule EPG ������
===================================================*/
HCSHANDLE CSEPGScheduleLevelInit( HCSHANDLE hEPG, HCSHANDLE hMemoryPool, EPGLevelGCallBack pCallBack );

/*==================================================
���ܣ�����Simple EPG ������
===================================================*/
HCSHANDLE CSEPGSimpleLevelInit( HCSHANDLE hEPG, HCSHANDLE hMemoryPool, EPGLevelGCallBack pCallBack );

/*==================================================
���ܣ�����Simple follow EPG ������
===================================================*/
HCSHANDLE CSEPGSimpleFollowLevelInit( HCSHANDLE hEPG, HCSHANDLE hMemoryPool, EPGLevelGCallBack pCallBack );

/*==================================================
���ܣ����ٷ�������Schedule��Simpleͨ��
===================================================*/
BOOL CSEPGLevelDestroy( HCSHANDLE hEPGLevelHandle );

/*==================================================
���ܣ�ɾ�����õķ�����Ϣ��Schedule��Simpleͨ��
===================================================*/
BOOL CSEPGLevelDeleteAllDate( HCSHANDLE hEPGLevelHandle );

/*==================================================
���ܣ���Schedule�������ʱ��Σ���Schedule��������
������
   pDurTime ���ʱ�䣬�����ֽڿ�ʼʱ���MJD, �����ֽ�Ϊ��ʼʱ���UTC,
            ��������ֽ�Ϊ���ʱ���UTC����ʱ���Ǳ�׼ʱ�䣬û�м�ʱ����
   nTimeLen pDurTime�ĳ��ȣ�Ӧ����Ϊ8��
===================================================*/
BOOL CSEPGLevelSetDuration( HCSHANDLE hScheduleLevelHandle, BYTE * pDurTime, int nTimeLen );

/*==================================================
���ܣ��ؽ�EPG�������ݱ�����ɾ���������ݣ������¹������ݱ�
      Schedule��Simpleͨ�á����ݷ�������������
===================================================*/
BOOL CSEPGLevelRebuild( HCSHANDLE hEPGLevelHandle );

/*==================================================
���ܣ���ָ������ִ������Schedule��Simpleͨ��
===================================================*/
BOOL CSEPGLevelSort( HCSHANDLE hEPGLevelHandle, int nContentLevel1 );

/*==================================================
���ܣ���ȡָ������Ľڵ������Schedule��Simpleͨ��
===================================================*/
int CSEPGGetLevel1Count( HCSHANDLE hEPGLevelHandle, int nContentLevel1 );

/*==================================================
���ܣ���ȡָ������ָ���������������ݣ�Schedule��Simpleͨ��
������
   [in]nIndex������ֵ
   [out]pEPGInfo: EPG����
   [out]pdwUserData: �û����ݣ�����ServiceHandle
����ֵ��
   ����FALSE����ʾ���ݲ����á�
===================================================*/
BOOL CSEPGLevelGetEPGbyIndex( HCSHANDLE hEPGLevelHandle, int nContentLevel1, int nIndex, EPGINFO * pEPGInfo, DWORD * pdwUserData );

/*==================================================
���ܣ���EPGģ��ص���Simple EPG���ݲ���Simplw����������Simple���á�
����
   pData��EPG���صĽṹ�塣
   dwUserData���û����ݣ�ServiceHandle.
ע�⣺�û�����ServcieHandleһ��Ҫ���룬����ģ��ʹ����������ؼ��֡�
===================================================*/
BOOL CSEPGSimpleLevelInsert( HCSHANDLE hSimpleLevelHandle, EPGContentTypeData * pData, DWORD dwUserData );


/*==================================================
���ܣ���ȡָ����������Ľڵ������Schedule��Simpleͨ��
===================================================*/
int CSEPGGetLevel2Count( HCSHANDLE hEPGLevelHandle, int nContentLevel1,int nContentLevel2 );
//////////////////////////////////////////////////////////////////////////
/*==================================================
���ܣ���ȡָ����������ָ���������������ݣ�Schedule��Simpleͨ��
������
[in]nContentLevel1:һ������
[in]nContentLevel2:��������
[in]nIndex������ֵ
[out]pEPGInfo: EPG����
[out]pdwUserData: �û����ݣ�����ServiceHandle
����ֵ��
����FALSE����ʾ���ݲ����á�
===================================================*/
BOOL CSEPGLevel2GetEPGbyIndex( HCSHANDLE hEPGLevelHandle, int nContentLevel1,int nContentLevel2, int nIndex, EPGINFO * pEPGInfo, DWORD * pdwUserData );
//////////////////////////////////////////////////////////////////////////

/*==================================================
���ܣ���Schedule�������ʱ��Σ���Schedule��������
������
    [in]hScheduleLevelHandle:   Schedule���
    [in]enmDurationType      :   ���ó���ʱ������ͣ�
                                ��ȡֵ��EPGSORT_INCLUDE_ALL�����趨��Duration�н��沿�ֵ�ȫ��Event��Ĭ�ϴ���״̬����,
                                        EPGSORT_EXCLUDE_START_NOT_IN(�ų���ʼʱ�䲻���趨��Duration�ڵ�Event),
                                        
===================================================*/
BOOL CSUSPEPGLevelSetDurationType( HCSHANDLE hScheduleLevelHandle, EPGSortDurationType enmDurationType );

/*==================================================
���ܣ���ȡģ��汾�š�
===================================================*/
int CSEPGSortGetVersionInfo( char * pcVersionInfo, int nSize );

#ifdef  __cplusplus
}
#endif


#endif /* CSEPGSortInterface_H */
