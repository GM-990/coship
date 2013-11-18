/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_bootshowtestcase.h"
#include "udi2_bootshow.h"

#include "cs_testkit.h"
#include "cs_testkit_porting.h"
#include "udi2_os.h"
#include "udi2_osg.h"

#define PICTURECOUNT 8

static unsigned char* g_pcPicBuff = CSUDI_NULL;

typedef struct _PictureInfo{
	char* acPicName;
	CSUDIBOOTSHOWUnitInfo sUnitInfo;
}PictureInfo_S;

typedef enum 
{
    EM_BOOTSHOW_UNITTYPE_BMPBITS,
    EM_BOOTSHOW_UNITTYPE_BMP,
    EM_BOOTSHOW_UNITTYPE_PNG,
    EM_BOOTSHOW_UNITTYPE_JPG,
    EM_BOOTSHOW_UNITTYPE_MPEG,
    EM_BOOTSHOW_UNITTYPE_SIGALGIF,
    EM_BOOTSHOW_UNITTYPE_BMPZIPS,
    EM_BOOTSHOW_UNITTYPE_100JPG,
    EM_BOOTSHOW_UNITTYPE_DVBLIVE,
    EM_BOOTSHOW_UNITTYPE_TSFILE,
    EM_BOOTSHOW_UNITTYPE_MAX
}UnitTypeIndex_E;

PictureInfo_S g_sPictureInfo[PICTURECOUNT] = {
    {
      "bootshow_bits.bmpbits",
      {CSUDI_TRUE, 0x01, 0x0, 0x00}
    },
    {
      "bootshow_bmp.bmp",
      {CSUDI_TRUE, 0x02, 0x23aba, 0x01}
    },
    {
      "bootshow_png.png",
      {CSUDI_TRUE, 0x04, 0xdc785, 0x02}
    },
    {
      "bootshow_jpg.jpg",
      {CSUDI_TRUE, 0x08, 283232, 0x03}
    },
    {
      "bootshow_mpeg.mpeg",
      {CSUDI_TRUE, 0x10, 0x0, 0x04}
    },
    {
      "bootshow_gif.gif",
      {CSUDI_TRUE, 0x20, 0x0, 0x05}
    },
    {
      "bootshow_zips.bmpzips",
      {CSUDI_TRUE, 0x40, 0x0, 0x06}
    },  
    {
      "bootshow_jpg100.jpg",
      {CSUDI_TRUE, 0x08, 0x29501, 0x07}
    }
};

BOOL CSTC_BOOTSHOW_Init(void)
{
	//在本测试用例集执行前调用	
	return TRUE;
}

BOOL CSTC_BOOTSHOW_UnInit(void)
{
	//在本测试用例集执行后调用        
	return TRUE;
}

BOOL CSTC_BOOTSHOW_MT_0001(void)
{
	return TRUE;
}

BOOL CSTC_BOOTSHOW_MT_0002(void)
{
	return TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetUnitCount(unsigned int* pnOutCount)
//@DESCRIPTION: 测试参数pnOutCount为CSUDI_NULL情况下函数CSUDIBOOTSHOWGetUnitCount的执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@INPUT: 1、pnOutCount = CSUDI_NULL
//@EXPECTATION: 期望返回参数异常错误CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount函数，期望返回CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_GetUnitCount_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWGetUnitCount(CSUDI_NULL), "步骤1，获取显示单元个数失败" );

    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetUnitCount(unsigned int* pnOutCount)
//@DESCRIPTION: 测试在参数pnOutCount正确情况下，函数CSUDIBOOTSHOWGetUnitCount的执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、显示单元个数必须大于0
//@INPUT: pnOutCount = 整型指针(已存储的显示单元个数）
//@EXPECTATION: 能正确获取boot已存储开机画面显示单元个数
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、判断步骤1中获取的值是否大于0
CSUDI_BOOL CSTC_BOOTSHOW_GetUnitCount_0002(void)
{
    unsigned int uBootShowUnitCount = 0;
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "步骤1，获取显示单元个数失败" );

    CSTCPrint( "显示单元个数为: %d\n", uBootShowUnitCount );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "步骤2，显示单元个数不大于0" );

    CSTK_FATAL_POINT
   
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetCapability(CSUDIBOOTSHOWCapability* psOutCapability)
//@DESCRIPTION: 测试参数为CSUDI_NULL的情况下，函数CSUDIBOOTSHOWGetCapability的执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@INPUT: 1、psOutCapability = CSUDI_NULL
//@EXPECTATION: 期望返回错误码
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_GetCapability_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWGetCapability( CSUDI_NULL ), "步骤1，获取能力值失败" );
	
    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetCapability(CSUDIBOOTSHOWCapability* psOutCapability)
//@DESCRIPTION: 测试参数正确情况下，函数CSUDIBOOTSHOWGetCapability的执行结果
//@DESCRIPTION: 将获取的能力值与配置文件中描述的能力值进行比较，关键字为CS_BOOTSHOW_CAPABILITY, 它的值
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1 
//@INPUT: 1、psOutCapability为CSUDIBOOTSHOWCapability结构指针
//@EXPECTATION: 能够正确获取能力值
//@EXPECTATION: 获取到的能力值与配置文件中的能力值相同
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、从配置文件中获取能力值信息CS_BOOTSHOW_DECODE_CAPABILITY，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、比较步骤1和步骤2所获取的信息,期望相同
//@EXECUTIONFLOW: 4、从配置文件中获取能力值信息CS_BOOTSHOW_SHOWFALSH_TOTALSIZE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、比较步骤1和步骤4所获取的信息,期望相同
//@EXECUTIONFLOW: 6、从配置文件中获取能力值信息CS_BOOTSHOW_UNIT_HEADSIZE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、比较步骤1和步骤6所获取的信息,期望相同
CSUDI_BOOL CSTC_BOOTSHOW_GetCapability_0002(void)
{
    char cResult[255];
    int nResult = 0;    
    CSUDIBOOTSHOWCapability sUnitCapability ;
    
    memset( &sUnitCapability, 0, sizeof(sUnitCapability) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetCapability(&sUnitCapability), "步骤1，获取能力值失败" );

    memset( cResult, 0, sizeof(cResult) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSTKGetConfigInfo("BOOTSHOW", "CS_BOOTSHOW_DECODE_CAPABILITY", cResult, sizeof(cResult)), "步骤2，获取配置文件信息失败" );

    nResult = CSTKGetIntFromStr( cResult, 16 );
    
    CSTK_ASSERT_TRUE_FATAL( sUnitCapability.nBootDecodeCapablity == nResult, "步骤3，比较失败" );

    memset( cResult, 0, sizeof(cResult) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSTKGetConfigInfo("BOOTSHOW", "CS_BOOTSHOW_SHOWFALSH_TOTALSIZE", cResult, sizeof(cResult)), "步骤4，获取配置文件信息失败" );
    
    nResult = CSTKGetIntFromStr( cResult, 16 );

    CSTCPrint( "FlashTotalSize = 0x%x, ConfigResult= 0x%x\n", sUnitCapability.nBooterShowFlashTotalSize, nResult );
    CSTK_ASSERT_TRUE_FATAL( sUnitCapability.nBooterShowFlashTotalSize == nResult, "步骤5，比较失败" );
     
    memset( cResult, 0, sizeof(cResult) );
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSTKGetConfigInfo("BOOTSHOW", "CS_BOOTSHOW_UNIT_HEADSIZE", cResult, sizeof(cResult)), "步骤6，获取配置文件信息失败" );
        
    nResult = CSTKGetIntFromStr( cResult, 16 );
    CSTCPrint( "UnitHeadSize = 0x%x, ConfigResult= 0x%x\n\n", sUnitCapability.nBooterUnitHeadSize, nResult );
    CSTK_ASSERT_TRUE_FATAL( sUnitCapability.nBooterUnitHeadSize == nResult, "步骤7，比较失败" );

    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psOutInfo)
//@DESCRIPTION: 测试参数nIndex非法，psOutInfo正确的情况下，函数CSUDIBOOTSHOWGetShowUnitInfo的执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@INPUT: 1、 nIndex 非法取值为-1和已存储显示单元个数+1
//@INPUT: 2、 psOutInfo 为CSUDIBOOTSHOWUnitInfo结构体指针
//@EXPECTATION: 能够正确获取booter显示用到的已存储的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回成功
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、nIndex=-1，调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 4、nIndex=uBootShowUnitCount+1，调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
CSUDI_BOOL CSTC_BOOTSHOW_GetShowUnitInfo_0001(void)
{
    unsigned int uBootShowUnitCount = 0;
	CSUDIBOOTSHOWUnitInfo sUnitInfo;
            
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "步骤1，获取单元个数失败" );
    
    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "步骤2，显示单元个数不大于0" );

    memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWGetShowUnitInfo(-1, &sUnitInfo), "步骤3，获取显示单元信息失败" );
        
	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWGetShowUnitInfo(uBootShowUnitCount+1, &sUnitInfo), "步骤4，获取显示单元信息失败" );
        
	CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psOutInfo)
//@DESCRIPTION: 测试参数nIndex正确，psOutInfo为CSUDI_NULL的情况下，函数CSUDIBOOTSHOWGetShowUnitInfo的执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@INPUT: 1、 nIndex 取值为0--已存储显示单元个数-1
//@INPUT: 2、 psOutInfo = CSUDI_NULL
//@EXPECTATION: 能够正确获取booter显示用到的已存储的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount获取显示单元个数,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、参数为空，调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_GetShowUnitInfo_0002(void)
{
	int nIndex = 0;
    unsigned int uBootShowUnitCount = 0;
    
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "步骤1，获取单元个数失败" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "步骤2，显示单元个数不大于0" );

	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, CSUDI_NULL), "步骤3，获取显示单元信息失败" );

	CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

static CSUDI_BOOL BOOTSHOW_iPrintUnitType( int nIndex, CSUDIBOOTSHOWUnitInfo* sUnitInfo )
{
	CSUDI_BOOL ret = CSUDI_TRUE;

    if( nIndex < 0 || CSUDI_NULL == sUnitInfo )
    {
    	CSTCPrint( "传入的参数错误\n" );
		return CSUDI_FALSE;
    }

    switch( sUnitInfo->nUnitType )
    {
        case CSUDIBOOTSHOW_UNITTYPE_BMPBITS :
            CSTCPrint( "索引号为%d的显示单元的数据格式为: 点阵数组, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_BMP :
            CSTCPrint( "索引号为%d的显示单元的数据格式为: BMP图片, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_PNG:
            CSTCPrint( "索引号为%d的显示单元的数据格式为: PNG图片, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_JPG:
            CSTCPrint( "索引号为%d的显示单元的数据格式为: JPG图片, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_MPEG:
            CSTCPrint( "索引号为%d的显示单元的数据格式为: MPG图片, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_SIGALGIF:
            CSTCPrint( "索引号为%d的显示单元的数据格式为: GIF图片, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_BMPZIPS :
            CSTCPrint( "索引号为%d的显示单元的数据格式为: 压缩点阵数组, ", nIndex );  
            break;
        case CSUDIBOOTSHOW_UNITTYPE_DVBLIVE :
            CSTCPrint( "索引号为%d的显示单元的数据格式为: 直播视频, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_TSFILE :
            CSTCPrint( "索引号为%d的显示单元的数据格式为: TS视频文件, ", nIndex );
            break;
        default :
            CSTCPrint( "未知的显示单元格式(nUnitType=0x%x)\n", sUnitInfo->nUnitType );
            ret = CSUDI_FALSE;
            break;
    }
                
    CSTCPrint("显示单元的数据大小为: 0x%x, ", (int)sUnitInfo->nDataSize );
        
    CSTCPrint("显示单元的标识为: %u\n", sUnitInfo->nUnitId );

    return ret;
}

//@CASEGROUP: CSUDIBOOTSHOWGetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psOutInfo)
//@DESCRIPTION: 测试参数都正确情况下，函数CSUDIBOOTSHOWGetShowUnitInfo的执行结果
//@DESCRIPTION: 将获取的显示单元的信息与配置文件中的描述信息进行比较，关键字为CS_BOOTSHOW_CAPABILITY, 它的值
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@INPUT: 1、nIndex为存储显示单元的索引（取值为0--已存储显示单元个数-1）
//@INPUT: 2、psOutInfo为CSUDIBOOTSHOWUnitInfo结构指针
//@EXPECTATION: 能够获取到booter显示用到的已存储的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、打印出步骤3获取到的能力值，并判断是否为已定义的显示单元格式，期望为已定义的显示单元格式
//@EXECUTIONFLOW: 5、nIndex从0到(已存储显示单元个数-1),重复步骤3、4
CSUDI_BOOL CSTC_BOOTSHOW_GetShowUnitInfo_0003(void)
{
    int nIndex = 0;
    unsigned int uBootShowUnitCount = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "步骤1，获取单元个数失败" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "步骤2，显示单元个数不大于0" );

    for( nIndex = 0; nIndex < uBootShowUnitCount; nIndex++ )
    { 
		memset( &sUnitInfo, 0, sizeof(sUnitInfo) );        
        CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sUnitInfo), "步骤3，获取显示单元信息失败" );

        CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iPrintUnitType(nIndex, &sUnitInfo), "未知的显示单元格式" );
    }
    
    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetFreeSize(unsigned int* pnOutSize)
//@DESCRIPTION: 测试参数为CSUDI_NULL的情况下，函数CSUDIBOOTSHOWGetFreeSize的执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@INPUT: 1、pnOutSize = CSUDI_NULL
//@EXPECTATION: 能够正确获取booter可存储显示数据空间大小
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_GetFreeSize_0001(void)
{
    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWGetFreeSize(CSUDI_NULL), "步骤1，获取剩余空间失败" );

    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetFreeSize(unsigned int* pnOutSize)
//@DESCRIPTION: 测试参数正确的情况下，函数CSUDIBOOTSHOWGetFreeSize的执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability接口实现没问题
//@INPUT: 1、pnOutSize 为剩余空间大小，单位为BYTE
//@EXPECTATION: 能够正确获取booter可存储显示数据空间大小
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、计算出已存储的显示单元的总空间大小(每个显示单元数据大小之和)
//@EXECUTIONFLOW: 5、调用CSUDIBOOTSHOWGetCapability获取用于存储单元信息的flash空间的大小
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize获取剩余空间的大小，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断(步骤6的结果)==(步骤5的结果-步骤4的结果)，期望等式成立
//@REMARK: 剩余空间为flash地址对齐后的剩余可用空间大小
CSUDI_BOOL CSTC_BOOTSHOW_GetFreeSize_0002(void)
{
    unsigned int uBootShowUnitCount = 0;
    unsigned int uFreeSize = 0;
	int nIndex = 0;
    int nAllUnitSize = 0;
    CSUDIBOOTSHOWCapability sUnitCapability;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;   
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "步骤1，获取单元个数失败" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "步骤2，显示单元个数不大于0" );

    for( nIndex = 0; nIndex < uBootShowUnitCount; nIndex++ )
    {	    	
        memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
        CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sUnitInfo), "步骤3，获取显示单元失败" ); 
        
        nAllUnitSize += sUnitInfo.nDataSize;
    }

    memset( &sUnitCapability, 0, sizeof(sUnitCapability) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetCapability(&sUnitCapability), "步骤5，获取能力值信息失败" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetFreeSize(&uFreeSize), "步骤6，获取剩余空间失败" );

    CSTCPrint( "FreeSize=0x%x byte, FlashTotalSize=0x%x byte, AllUnitSize=0x%x byte\n", uFreeSize, sUnitCapability.nBooterShowFlashTotalSize, nAllUnitSize );

    CSTK_ASSERT_TRUE_FATAL( uFreeSize == (sUnitCapability.nBooterShowFlashTotalSize - nAllUnitSize), "步骤7，剩余空间计算失败" );

    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//获取图片信息
static unsigned int BOOTSHOW_iGetPhotoInfo( const char *photoName, unsigned char **pBuff )
{
    CSUDI_HANDLE fp = CSUDI_NULL;
    unsigned int nIframeLen = 0;
    CSUDI_BOOL bOpen = CSUDI_FALSE;

    if( CSUDI_NULL == photoName )
    {
		CSTCPrint("传入的文件名为NULL\n");
        bOpen = CSUDI_FALSE;
    }

    if( CSUDI_NULL != (fp = CSTKP_FOpen(photoName, "rb")) )
    {
        CSTCPrint( "图片%s打开成功\n ", photoName );
        bOpen = CSUDI_TRUE;
    }

    else
    {
        CSTCPrint( "图片""%s ""打开失败\n", photoName );
        bOpen = CSUDI_FALSE;
    }
    
    if ( *pBuff != CSUDI_NULL )
    {
        CSUDIOSFree( *pBuff );
        *pBuff = CSUDI_NULL;
    }

    if( bOpen )
    {
        if( 0 == CSTKP_FSeek(fp, 0, CSTK_SEEK_END) )
        {
            nIframeLen = CSTKP_FTell( fp );
            CSTCPrint( "iframeLen = 0x%x\n", nIframeLen );
            
            if( nIframeLen > 0 && nIframeLen < 4*1920*1080 )       //图片大小合法性判断1920*1080分辨率ARGB8888 
            {
                *pBuff = CSUDIOSMalloc(nIframeLen); 
                if( *pBuff == CSUDI_NULL )
                {
                    CSTCPrint( "分配空间失败!\n" );
                    if( fp != CSUDI_NULL )
                    {
                        CSTKP_FClose( fp );
                    }
                    return 0;
                }
                memset( *pBuff, 0, nIframeLen );

                if( 0 != CSTKP_FSeek(fp,  0, CSTK_SEEK_SET) )
                {
                    CSTCPrint( "返回起始指针失败!\n" );
                    bOpen = CSUDI_FALSE;
                }
    
                if( nIframeLen != (CSTKP_FRead(*pBuff, sizeof(unsigned char),nIframeLen, fp)) )
                {
                    CSTCPrint( "读%s文件失败!\n", photoName );
                    bOpen = CSUDI_FALSE;
                }
            }
            else
            {
                CSTCPrint( "图片大小超出正确范围，失败!\n" );
                bOpen = CSUDI_FALSE;
            }
        }
        else
        {
            CSTCPrint( "寻找图片%s 末指针失败!\n", photoName );
            bOpen = CSUDI_FALSE;
        }
    }
    
    if( fp != CSUDI_NULL )
    {
        CSTKP_FClose( fp );
    }

    if ( !bOpen )
    {
        nIframeLen = 0;

        if ( *pBuff != CSUDI_NULL )
        {
            CSUDIOSFree( *pBuff );
            *pBuff = CSUDI_NULL;
        }
    }
    
    return nIframeLen;
}

//@CASEGROUP: CSUDIBOOTSHOWGetFreeSize(unsigned int* pnOutSize)
//@DESCRIPTION: 测试添加支持的类型的显示单元失败时，flash剩余空间大小是否改变
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@INPUT: 1、pnOutSize 为剩余空间大小，单位为BYTE
//@EXPECTATION: flash剩余空间应该不会变
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，并判断出支持那种类型的图片
//@EXECUTIONFLOW: 4、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIBOOTSHOWSetShowUnitInfo添加步骤3获取的类型的图片
//@EXECUTIONFLOW: 6、如果步骤5返回CSUDI_SUCCESS，则重复步骤4~5，直到步骤5返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、比较步骤4和步骤7的结果，期望相同
CSUDI_BOOL CSTC_BOOTSHOW_GetFreeSize_0003(void)
{
    unsigned int uBootShowUnitCount = 0;
    unsigned int uFreeSize1 = 0;
    unsigned int uFreeSize2 = 0;
    int i = 0;
    int nImageLen = 0;
    PictureInfo_S *sPicInfo = g_sPictureInfo;
    CSUDIBOOTSHOWCapability sUnitCapability;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;   
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "步骤1，获取单元个数失败" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "步骤2，显示单元个数不大于0" );

	memset(&sUnitCapability, 0, sizeof(sUnitCapability));
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetCapability(&sUnitCapability), "步骤3，获取能力信息失败" );
    
    while( i < PICTURECOUNT )
    {
    	  memset(&sUnitInfo, 0, sizeof(sUnitInfo));
        if( sUnitCapability.nBootDecodeCapablity & sPicInfo[i].sUnitInfo.nUnitType )
        {
            nImageLen = BOOTSHOW_iGetPhotoInfo( sPicInfo[3].acPicName, &g_pcPicBuff );
            sPicInfo[3].sUnitInfo.nDataSize = nImageLen;
            CSTK_ASSERT_TRUE_FATAL( 0 != nImageLen, "获取图片信息失败" );
            break;
        }
		CSTK_ASSERT_TRUE_FATAL( i != PICTURECOUNT, "步骤3，不支持任何类型的解码" );
		i++;
    }

    while( 1 )
    {
    	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetFreeSize(&uFreeSize1), "步骤4，获取剩余空间失败" );
		
        if( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(uBootShowUnitCount, &sPicInfo[3].sUnitInfo, g_pcPicBuff) )
       	{
			uBootShowUnitCount++;
            if( 5 == uBootShowUnitCount )
            {
				break;
            }
        }
        else
        {
			break;
        }
    }
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetFreeSize(&uFreeSize2), "步骤7，获取剩余空间失败" );

    CSTK_ASSERT_TRUE_FATAL( uFreeSize1 == uFreeSize2, "步骤8，添加显示单元失败时，剩余空间不应该改变" );

    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//寻找第一张支持的类型的图片，返回索引号
static int BOOTSHOW_iFindPicTypeSupported( PictureInfo_S* sPicInfo )
{	
    int i = 0;
    int nImageLen = 0;
    int nRet = EM_BOOTSHOW_UNITTYPE_MAX;    
    unsigned int uBootShowUnitCount = 0;
    unsigned int uFreeSize = 0;
    CSUDIBOOTSHOWCapability sUnitCapability;

    g_pcPicBuff = CSUDI_NULL;

    if( CSUDI_NULL == sPicInfo || CSUDI_NULL == sPicInfo->acPicName )
    {
        CSTCPrint( "传入的参数为NULL\n" );
        return nRet;
    }
    
	if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount) )
    {
    	CSTCPrint( "步骤1，获取单元个数失败\n" );
        return nRet;
    }

   	if( uBootShowUnitCount <= 0 )
    {
		CSTCPrint( "步骤2，显示单元个数不大于0\n" );
        return nRet;
    }

    memset( &sUnitCapability, 0, sizeof(sUnitCapability) );
    if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetCapability(&sUnitCapability) )
	{
        CSTCPrint( "步骤3，获取能力值失败\n" );
		return nRet;
    }

	while( (i++) < PICTURECOUNT )
	{
		if( sUnitCapability.nBootDecodeCapablity & sPicInfo[i].sUnitInfo.nUnitType )
		{
			nImageLen = BOOTSHOW_iGetPhotoInfo( sPicInfo[i].acPicName, &g_pcPicBuff );
			sPicInfo[i].sUnitInfo.nDataSize = nImageLen;
			break;
        }

		if( i == PICTURECOUNT )
		{
			CSTCPrint( "步骤4，不支持任何类型的解码\n" );        
			return nRet;
		}
	}
   
	if( (g_pcPicBuff == CSUDI_NULL) && nImageLen == 0 )
    {
		CSTCPrint( "步骤5，获取图片信息失败\n" );        
        return nRet;
    }

	if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetFreeSize(&uFreeSize) )
    {
		CSTCPrint( "步骤6，获取剩余空间大小失败\n" );
		return nRet;
    }
    
	if( sPicInfo[i].sUnitInfo.nDataSize > (uFreeSize - sizeof(sPicInfo[i].sUnitInfo)) )
	{
		CSTCPrint( "步骤7，剩余空间不足: uFreeSize = 0x%x, nDataSize = 0x%x\n", uFreeSize - sizeof(sPicInfo[i].sUnitInfo), sPicInfo[i].sUnitInfo.nDataSize );
        return nRet;
	}
    nRet = i;
    
    return nRet;        
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试nIndex非法，其他参数正确的情况下，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@PRECONDITION: 3、CSUDIBOOTSHOWGetFreeSize 接口实现没有问题
//@INPUT: 1、nIdex 非法取值为-1或已存储显示单元个数+1
//@INPUT: 2、psInfo 为单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 期望返回CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断booter支持哪种类型图片的解码模式，一旦存在一种就执行以下步骤
//@EXECUTIONFLOW: 5、获取步骤4中所支持的类型图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤6获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、nIndex=-1，调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 9、nIndex=uBootShowUnitCount+1，调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@REMARK: 可存储图片大小 = 剩余空间大小 - 存储每个Boot图片所需存储的图片信息头部大小
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0001(void)
{
	unsigned int uBootShowUnitCount = 0;
    int nReturn = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;

    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "查找第一张支持的图片类型失败" );

	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWSetShowUnitInfo(-1, &sUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );

    CSUDIBOOTSHOWGetUnitCount( &uBootShowUnitCount );

    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWSetShowUnitInfo(uBootShowUnitCount+1, &sUnitInfo, g_pcPicBuff), "步骤9，设置显示单元信息失败" );

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试psInfo=CSUDI_NULL,其他参数正确的情况下，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@PRECONDITION: 3、CSUDIBOOTSHOWGetFreeSize 接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为CSUDI_NULL
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 设置booter显示用到的每个显示单元的信息失败
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断booter支持哪种类型图片的解码模式，一旦存在一种就执行以下步骤
//@EXECUTIONFLOW: 5、获取步骤4中所支持的类型图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤6获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
//@REMARK: 可存储图片大小 = 剩余空间大小 - 存储每个Boot图片所需存储的图片信息头部大小
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0002(void)
{
    int nIndex = 1;
    int nReturn = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;
    
    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "查找第一张支持的图片类型失败" );

	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWSetShowUnitInfo( nIndex, CSUDI_NULL, g_pcPicBuff ), "步骤6，设置显示单元信息失败" );
    
    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试pData=CSUDI_NULL,其他参数正确的情况下，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@PRECONDITION: 3、CSUDIBOOTSHOWGetFreeSize 接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 设置booter显示用到的每个显示单元的信息失败
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断booter支持哪种类型图片的解码模式，一旦存在一种就执行以下步骤
//@EXECUTIONFLOW: 5、获取步骤4中所支持的类型图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤6获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、pData=CSUDI_NULL,调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回SUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0003(void)
{
    int nIndex = 1;
    int nReturn = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;
    
    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "查找第一张支持的图片类型失败" );

	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );

    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWSetShowUnitInfo( nIndex, &sUnitInfo, CSUDI_NULL ), "步骤5，设置显示单元信息失败" );
    
    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试如果索引==已存储显示单元个数时，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@PRECONDITION: 3、CSUDIBOOTSHOWGetShowUnitInfo接口实现没问题
//@PRECONDITION: 4、CSUDIBOOTSHOWGetFreeSize 接口实现没有问题
//@INPUT: 1、nIdex 为已存储显示单元个数
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 能够成功新增一个显示单元
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断booter支持哪种类型图片的解码模式，一旦存在一种就执行以下步骤
//@EXECUTIONFLOW: 5、获取步骤4中所支持的类型图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤6获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、nIndex==步骤1获取到的个数，调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、将步骤1与步骤9所取得的值进行比较，期望步骤1值==步骤9值-1
//@EXECUTIONFLOW: 11、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回成功
//@EXECUTIONFLOW: 12、将步骤8获取的值与步骤11设置的值进行比较，期望相同
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0004(void)
{
    unsigned int uBootShowUnitCount_1 = 0;
    unsigned int uBootShowUnitCount_2 = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo_1;
    CSUDIBOOTSHOWUnitInfo sUnitInfo_2;
    int nReturn = 0;

    CSTCPrint( "注意:该用例会添加一个显示单元，多次运行会使flash空间不足而导致用例失败\n" );

    nReturn = BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo);

    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != nReturn, "查找第一张支持的图片类型失败" );

	memset( &sUnitInfo_1, 0, sizeof(sUnitInfo_1) );
	memcpy( &sUnitInfo_1, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );
    
	CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount_1);

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(uBootShowUnitCount_1, &sUnitInfo_1, g_pcPicBuff), "步骤8，设置显示单元信息失败" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount_2), "步骤9，获取显示单元信息失败" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount_1 == (uBootShowUnitCount_2 - 1), "步骤10，比较失败" );

    memset( &sUnitInfo_2, 0, sizeof(sUnitInfo_2) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(uBootShowUnitCount_1, &sUnitInfo_2), "步骤11，获取显示单元失败" ); 

    if( sUnitInfo_2.bNeedShow != sUnitInfo_1.bNeedShow || sUnitInfo_2.nDataSize != sUnitInfo_1.nDataSize
        || sUnitInfo_2.nUnitId != sUnitInfo_1.nUnitId || sUnitInfo_2.nUnitType != sUnitInfo_1.nUnitType
        )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤12，比较失败" );
    }

    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试显示单元的信息是固定的，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@PRECONDITION: 3、CSUDIBOOTSHOWGetFreeSize 接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 在显示单元在不允许修改的情况下，设置显示单元信息失败
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断booter支持哪种类型图片的解码模式，一旦存在一种就执行以下步骤
//@EXECUTIONFLOW: 5、获取步骤4中所支持的类型图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤6获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDIBOOTSHOW_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0005(void)
{
    int nIndex = 0;
    int nReturn =  0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;

    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "查找第一张支持的图片类型失败" );

	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_FEATURE_NOT_SUPPORTED == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );
    
    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//是否支持指定类型的图片解码
static CSUDI_BOOL BOOTSHOW_iIsPicTypeSupported( PictureInfo_S* sUnitInfo )
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	unsigned int uBootShowUnitCount = 0;
    unsigned int uFreeSize = 0;
    int nImageLen = 0;
	CSUDIBOOTSHOWCapability sUnitCapability;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
        
    g_pcPicBuff = CSUDI_NULL; 

    if( CSUDI_NULL == sUnitInfo || CSUDI_NULL == sUnitInfo->acPicName )
    {
        CSTCPrint( "传入的参数为NULL\n" );
        return bRet;
    }
    
	if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount) )
    {
    	CSTCPrint( "步骤1，获取单元个数失败\n" );
        return bRet;
    }

   	if( uBootShowUnitCount <= 0 )
    {
		CSTCPrint( "步骤2，显示单元个数不大于0\n" );
        return bRet;
    }
    
    memset( &sUnitCapability, 0, sizeof(sUnitCapability) );
    if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetCapability(&sUnitCapability) )
	{
        CSTCPrint( "步骤3，获取能力值失败\n" );
		return bRet;
    }
    
	if( sUnitCapability.nBootDecodeCapablity & sUnitInfo->sUnitInfo.nUnitType )
	{
		nImageLen = BOOTSHOW_iGetPhotoInfo( sUnitInfo->acPicName, &g_pcPicBuff );
		sUnitInfo->sUnitInfo.nDataSize = nImageLen;
	}
    else
    {
		CSTCPrint( "步骤4，不支持 %s 该类型显示单元解码\n", sUnitInfo->acPicName);
        return bRet;
    }

	if( (g_pcPicBuff == CSUDI_NULL) && nImageLen == 0 )
    {
		CSTCPrint( "步骤5，获取图片信息失败\n" );        
        return bRet;
    }

	if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetFreeSize(&uFreeSize) )
    {
		CSTCPrint( "步骤6，获取剩余空间大小失败\n" );
		return bRet;
    }

    if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetShowUnitInfo( uBootShowUnitCount - 1, &sGetUnitInfo) )
    {
	    CSTCPrint( "获取显示单元信息失败\n" );		
        return bRet;
    }

    if( uBootShowUnitCount == 1 
        && sUnitInfo->sUnitInfo.nDataSize > uFreeSize - sizeof(sUnitInfo->sUnitInfo) )
    {
        CSTCPrint( "步骤7.1，剩余空间不足 " );
        CSTCPrint( "nDataSize = 0x%x，FreeSize = 0x%x\n", sUnitInfo->sUnitInfo.nDataSize, uFreeSize - sizeof(sUnitInfo->sUnitInfo) );
        return bRet;
    }
	else if( uBootShowUnitCount > 1 
     	&& sUnitInfo->sUnitInfo.nDataSize > (uFreeSize + sGetUnitInfo.nDataSize - sizeof(sUnitInfo->sUnitInfo)) )
	{
		CSTCPrint( "步骤7.2，剩余空间不足 " );
        CSTCPrint( "nDataSize = 0x%x，FreeSize = 0x%x\n", sUnitInfo->sUnitInfo.nDataSize, uFreeSize + sGetUnitInfo.nDataSize - sizeof(sUnitInfo->sUnitInfo) );
        return bRet;
	}
    else
    {
    
    }

    return CSUDI_TRUE;    
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试显示单元的数据格式为点阵数组时，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 能够成功设置booter显示用到的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持点阵数组解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张模式为点阵数组的图片，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤7和步骤8的信息，期望相同
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0006(void)
{ 
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPBITS].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPBITS].sUnitInfo) );
	CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPBITS]), "判断指定图片类型是否支持失败" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );
       
    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "步骤9，获取显示单元信息失败" );

    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤10，比较失败" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试显示单元的数据格式为BMP的图片时，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 能够成功设置booter显示用到的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持BMP图片解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张类型为BMP格式图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤7和步骤8的信息，期望相同
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0007(void)
{ 
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
   
	memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
	memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMP].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMP].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMP]), "判断指定图片类型是否支持失败" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );
       
	memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "步骤9，获取显示单元信息失败" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤10，比较失败" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}
   
//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试显示单元的数据格式为PNG图片时，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 能够成功设置booter显示用到的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持PNG图片解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张类型为PNG格式图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤7和步骤8的信息，期望相同
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0008(void)
{
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;

    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_PNG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_PNG].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_PNG]), "判断指定图片类型是否支持失败" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "步骤9，获取显示单元信息失败" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤10，比较失败" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}
    
//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试显示单元的数据格式为JPG图片时，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 能够成功设置booter显示用到的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持JPG图片解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张类型为JPG格式图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤7和步骤8的信息，期望相同
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0009(void)
{
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG]), "判断指定图片类型是否支持失败" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "步骤9，获取显示单元信息失败" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤10，比较失败" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试显示单元的数据格式为MPG图片时，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 能够成功设置booter显示用到的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持MPG图片解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张类型为MPG格式图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤7和步骤8的信息，期望相同
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0010(void)
{
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;

    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_MPEG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_MPEG].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_MPEG]), "判断指定图片类型是否支持失败" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "步骤9，获取显示单元信息失败" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤10，比较失败" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试显示单元的数据格式为GIF图片时，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 能够成功设置booter显示用到的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持GIF图片解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张类型为GIF格式图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤7和步骤8的信息，期望相同
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0011(void)
{
	int nIndex = 1;
	CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
	CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
  
	memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
	memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_SIGALGIF].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_SIGALGIF].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_SIGALGIF]), "判断指定图片类型是否支持失败" );
   
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "步骤9，获取显示单元信息失败" );
   
	if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
		|| sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
	{
		CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤10，比较失败" );
	}
   
	CSTK_FATAL_POINT
         
	return CSUDI_TRUE;   
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试显示单元的数据格式为压缩点阵数组时，调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 能够成功设置booter显示用到的指定显示单元的信息
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持压缩点阵数组图片解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张压缩点阵数组图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤7和步骤8的信息，期望相同
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0012(void)
{
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPZIPS].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPZIPS].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPZIPS]), "判断指定图片类型是否支持失败" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "步骤9，获取显示单元信息失败" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤10，比较失败" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试CSUDIBOOTSHOWUnitInfo.bNeedShow=true时，调用CSUDIBOOTSHOWSetShowUnitInfo是否能显示所设置的显示单元
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 期望能显示单元能设置成功，并且能成功显示设置的显示单元
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持JPG图片解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张类型为JPG格式图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤7和步骤8的信息，期望相同
//@EXECUTIONFLOW: 11、手动重启盒子，并观察现象
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0013(void)
{
    int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo) );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG]), "判断指定图片类型是否支持失败" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "步骤9，获取显示单元信息失败" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤10，比较失败" );
    }

    CSTCPrint( "请手动重启机顶盒，如果开机显示的图片为【%s】，则用例PASS\n", g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].acPicName );

    CSTCPrint( "任意键跳过用例\n" );
    
    CSTKWaitAnyKey();

    CSTK_FATAL_POINT
          
    return CSUDI_FALSE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试图片大小超过100K时，调用CSUDIBOOTSHOWSetShowUnitInfo是否能正常显示所设置的显示单元
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回1
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 期望能显示单元能设置成功，并且能成功显示设置的显示单元
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持JPG图片解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张类型为JPG格式图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤7和步骤8的信息，期望相同
//@EXECUTIONFLOW: 11、手动重启盒子，并观察现象
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0014(void)
{
    int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_100JPG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_100JPG].sUnitInfo) );

    sSetUnitInfo.bNeedShow = CSUDI_TRUE;

    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_100JPG]), "判断指定图片类型是否支持失败" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "步骤9，获取显示单元信息失败" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "步骤10，比较失败" );
    }

    CSTCPrint( "请手动重启机顶盒，如果开机显示图片为【%s】，并且能完全显示的话，则用例PASS\n", g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_100JPG].acPicName );

    CSTCPrint( "任意键跳过用例\n" );

    CSTKWaitAnyKey();

    CSTK_FATAL_POINT
          
    return CSUDI_FALSE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: 测试在显示单元索引不变的情况下，循环调用CSUDIBOOTSHOWSetShowUnitInfo函数执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount至少要返回2
//@PRECONDITION: 3、CSUDIBOOTSHOWGetCapability、CSUDIBOOTSHOWGetShowUnitInfo、CSUDIBOOTSHOWGetFreeSize接口实现没有问题
//@INPUT: 1、nIdex 为显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
//@INPUT: 2、psInfo 为显示单元信息
//@INPUT: 3、pData 为显示单元信息对应的数据区的起始地址
//@EXPECTATION: 每次设置显示单元都能成功
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于1
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断BOOTER是否支持JPG图片解码，是则执行一下步骤
//@EXECUTIONFLOW: 5、获取一张类型为JPG格式图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤5获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、调用CSUDIBOOTSHOWSetShowUnitInfo，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、比较步骤1和步骤9所获取到的值，期望相同
//@EXECUTIONFLOW: 11、重复步骤1~步骤10(5次)
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0015(void)
{
    int nIndex = 1;
	int i = 0;
    unsigned int uBootShowUnitCount1 = 0;
    unsigned int uBootShowUnitCount2 = 0;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;

	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount1), "步骤1，获取显示单元个数失败" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount1 > 1, "步骤2，显示单元个数至少为2" );

    for( i = 0; i < 5; i++ )
    {
	    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
	    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo) );
	    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG]), "判断指定图片类型是否支持失败" );
	    
	    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "步骤8，设置显示单元信息失败" );
        
        CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount2), "步骤9，获取显示单元个数失败" );

		CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount1 == uBootShowUnitCount2, "步骤10，显示单元个数不应该增加" );
	}
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow)
//@DESCRIPTION: 测试在nIndex为非法时，函数CSUDIBOOTSHOWSetNeedShow的执行结果
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@INPUT: 1、nIndex 非法取值为-1和已存储的显示单元个数+1
//@INPUT: 2、bNeedShow bNeedShow 是否需要显示
//@EXPECTATION: 期望返回CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount获取显示单元个数，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、nIndex=-1,调用CSUDIBOOTSHOWSetNeedShow，期望返回CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 4、nIndex=已存储的显示单元个数+1,调用CSUDIBOOTSHOWSetNeedShow，期望返回CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
CSUDI_BOOL CSTC_BOOTSHOW_SetNeedShow_0001(void)
{
    unsigned int uBootShowUnitCount = 0;

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "步骤1，获取单元个数失败" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "步骤2，显示单元个数不大于0" );

    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWSetNeedShow(-1, CSUDI_TRUE), "步骤3，设置显示单元失败" );

    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWSetNeedShow((uBootShowUnitCount+1), CSUDI_TRUE), "步骤4，设置显示单元失败" );

    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow)
//@DESCRIPTION: 测试存在两个显示单元的情况下时，调用CSUDIBOOTSHOWSetNeedShow显示指定的显示单元
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@PRECONDITION: 3、CSUDIBOOTSHOWSetShowUnitInfo接口实现没有问题
//@INPUT: 1、nIndex 非法取值为-1和已存储的显示单元个数+1
//@INPUT: 2、bNeedShow 是否需要显示
//@EXPECTATION: 期望返回CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWGetCapability，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、判断booter支持哪种类型图片的解码模式，一旦存在一种就执行以下步骤
//@EXECUTIONFLOW: 5、获取步骤4中所支持的类型图片的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIBOOTSHOWGetFreeSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、判断步骤6获取到的剩余空间是否足够，足够则执行以下步骤
//@EXECUTIONFLOW: 8、如果获取到的显示单元个数等于1则执行步骤9，否则不执行步骤9
//@EXECUTIONFLOW: 9、调用CSUDIBOOTSHOWSetShowUnitInfo添加一个显示单元，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、bNeedShow = CSUDI_TRUE，调用CSUDIBOOTSHOWSetNeedShow设置索引号为1的显示单元，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 11、bNeedShow = CSUDI_TRUE，调用CSUDIBOOTSHOWSetNeedShow设置索引号为0的显示单元，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 12、bNeedShow = CSUDI_TRUE，调用CSUDIBOOTSHOWSetNeedShow设置索引号为1的显示单元，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 13、恢复默认的显示单元
CSUDI_BOOL CSTC_BOOTSHOW_SetNeedShow_0002(void)
{
	CSUDI_BOOL bNeedShow = CSUDI_TRUE;
    unsigned int uBootShowUnitCount = 0;
    int nReturn = 0;
	CSUDIBOOTSHOWUnitInfo sUnitInfo;
    
	CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "查找第一张支持的图片类型失败" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "获取显示单元个数失败" );
    
	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );

    if( 1 == uBootShowUnitCount )
    {
		CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(uBootShowUnitCount, &sUnitInfo, g_pcPicBuff), "步骤9，添加显示单元失败" );
    }

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(1, bNeedShow), "步骤10，设置显示单元失败" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(0, bNeedShow), "步骤11，设置显示单元失败" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(1, bNeedShow), "步骤12，设置显示单元失败" );
	 
    CSTCPrint( "请手动重启机顶盒，如果开机显示的显示单元与默认的开机显示单元不一样的话，则用例PASS\n" );
    
    CSTCPrint( "任意键跳过用例\n" );

    CSTKWaitAnyKey();

    CSTK_FATAL_POINT
	
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(0, bNeedShow), "步骤13，恢复默认显示单元" );

    return CSUDI_FALSE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow)
//@DESCRIPTION: 测试调用CSUDIBOOTSHOWSetNeedShow使所有显示单元都不显示的情况
//@PRECONDITION: 1、booter能正常启动，并且开机显示单元能正常显示或播放
//@PRECONDITION: 2、CSUDIBOOTSHOWGetUnitCount 至少要返回1 
//@PRECONDITION: 3、CSUDIBOOTSHOWSetShowUnitInfo接口实现没有问题
//@INPUT: 1、nIndex = 已存储显示单元个数-1
//@INPUT: 2、bNeedShow bNeedShow 是否需要显示
//@EXPECTATION: 能够成功设置booter显示某个指定的已存储开机显示单元，并能开机显示指定的单元
//@EXECUTIONFLOW: 1、调用CSUDIBOOTSHOWGetUnitCount获取显示单元个数，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、对步骤1中获取的个数进行判断，期望大于0
//@EXECUTIONFLOW: 3、调用CSUDIBOOTSHOWSetNeedShow将所有开机显示单元设置成开机不显示，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、关机后再开机，期望不能显示开机显示单元
CSUDI_BOOL CSTC_BOOTSHOW_SetNeedShow_0003(void)
{
    unsigned int uBootShowUnitCount = 0;
    int nIndex = 0;
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "步骤1，获取单元个数失败" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "步骤2，显示单元个数不大于0" );

    for( nIndex = 0; nIndex < uBootShowUnitCount; nIndex++ )
    {    
    	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(nIndex, CSUDI_FALSE), "步骤3，设置开机显示单元信息失败" );
    }
    
    CSTCPrint( "请手动重启机顶盒，如果没有显示任何图片或没有播放任何音视频，用例PASS\n" );

	CSTCPrint( "任意键跳过用例\n" );

    CSTKWaitAnyKey();
        
    CSTK_FATAL_POINT
        
    return CSUDI_FALSE;
}


