/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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
	//�ڱ�����������ִ��ǰ����	
	return TRUE;
}

BOOL CSTC_BOOTSHOW_UnInit(void)
{
	//�ڱ�����������ִ�к����        
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
//@DESCRIPTION: ���Բ���pnOutCountΪCSUDI_NULL����º���CSUDIBOOTSHOWGetUnitCount��ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@INPUT: 1��pnOutCount = CSUDI_NULL
//@EXPECTATION: �������ز����쳣����CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount��������������CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_GetUnitCount_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWGetUnitCount(CSUDI_NULL), "����1����ȡ��ʾ��Ԫ����ʧ��" );

    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetUnitCount(unsigned int* pnOutCount)
//@DESCRIPTION: �����ڲ���pnOutCount��ȷ����£�����CSUDIBOOTSHOWGetUnitCount��ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2����ʾ��Ԫ�����������0
//@INPUT: pnOutCount = ����ָ��(�Ѵ洢����ʾ��Ԫ������
//@EXPECTATION: ����ȷ��ȡboot�Ѵ洢����������ʾ��Ԫ����
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���жϲ���1�л�ȡ��ֵ�Ƿ����0
CSUDI_BOOL CSTC_BOOTSHOW_GetUnitCount_0002(void)
{
    unsigned int uBootShowUnitCount = 0;
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "����1����ȡ��ʾ��Ԫ����ʧ��" );

    CSTCPrint( "��ʾ��Ԫ����Ϊ: %d\n", uBootShowUnitCount );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "����2����ʾ��Ԫ����������0" );

    CSTK_FATAL_POINT
   
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetCapability(CSUDIBOOTSHOWCapability* psOutCapability)
//@DESCRIPTION: ���Բ���ΪCSUDI_NULL������£�����CSUDIBOOTSHOWGetCapability��ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@INPUT: 1��psOutCapability = CSUDI_NULL
//@EXPECTATION: �������ش�����
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetCapability����������CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_GetCapability_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWGetCapability( CSUDI_NULL ), "����1����ȡ����ֵʧ��" );
	
    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetCapability(CSUDIBOOTSHOWCapability* psOutCapability)
//@DESCRIPTION: ���Բ�����ȷ����£�����CSUDIBOOTSHOWGetCapability��ִ�н��
//@DESCRIPTION: ����ȡ������ֵ�������ļ�������������ֵ���бȽϣ��ؼ���ΪCS_BOOTSHOW_CAPABILITY, ����ֵ
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1 
//@INPUT: 1��psOutCapabilityΪCSUDIBOOTSHOWCapability�ṹָ��
//@EXPECTATION: �ܹ���ȷ��ȡ����ֵ
//@EXPECTATION: ��ȡ��������ֵ�������ļ��е�����ֵ��ͬ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���������ļ��л�ȡ����ֵ��ϢCS_BOOTSHOW_DECODE_CAPABILITY����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3���Ƚϲ���1�Ͳ���2����ȡ����Ϣ,������ͬ
//@EXECUTIONFLOW: 4���������ļ��л�ȡ����ֵ��ϢCS_BOOTSHOW_SHOWFALSH_TOTALSIZE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5���Ƚϲ���1�Ͳ���4����ȡ����Ϣ,������ͬ
//@EXECUTIONFLOW: 6���������ļ��л�ȡ����ֵ��ϢCS_BOOTSHOW_UNIT_HEADSIZE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���Ƚϲ���1�Ͳ���6����ȡ����Ϣ,������ͬ
CSUDI_BOOL CSTC_BOOTSHOW_GetCapability_0002(void)
{
    char cResult[255];
    int nResult = 0;    
    CSUDIBOOTSHOWCapability sUnitCapability ;
    
    memset( &sUnitCapability, 0, sizeof(sUnitCapability) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetCapability(&sUnitCapability), "����1����ȡ����ֵʧ��" );

    memset( cResult, 0, sizeof(cResult) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSTKGetConfigInfo("BOOTSHOW", "CS_BOOTSHOW_DECODE_CAPABILITY", cResult, sizeof(cResult)), "����2����ȡ�����ļ���Ϣʧ��" );

    nResult = CSTKGetIntFromStr( cResult, 16 );
    
    CSTK_ASSERT_TRUE_FATAL( sUnitCapability.nBootDecodeCapablity == nResult, "����3���Ƚ�ʧ��" );

    memset( cResult, 0, sizeof(cResult) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSTKGetConfigInfo("BOOTSHOW", "CS_BOOTSHOW_SHOWFALSH_TOTALSIZE", cResult, sizeof(cResult)), "����4����ȡ�����ļ���Ϣʧ��" );
    
    nResult = CSTKGetIntFromStr( cResult, 16 );

    CSTCPrint( "FlashTotalSize = 0x%x, ConfigResult= 0x%x\n", sUnitCapability.nBooterShowFlashTotalSize, nResult );
    CSTK_ASSERT_TRUE_FATAL( sUnitCapability.nBooterShowFlashTotalSize == nResult, "����5���Ƚ�ʧ��" );
     
    memset( cResult, 0, sizeof(cResult) );
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSTKGetConfigInfo("BOOTSHOW", "CS_BOOTSHOW_UNIT_HEADSIZE", cResult, sizeof(cResult)), "����6����ȡ�����ļ���Ϣʧ��" );
        
    nResult = CSTKGetIntFromStr( cResult, 16 );
    CSTCPrint( "UnitHeadSize = 0x%x, ConfigResult= 0x%x\n\n", sUnitCapability.nBooterUnitHeadSize, nResult );
    CSTK_ASSERT_TRUE_FATAL( sUnitCapability.nBooterUnitHeadSize == nResult, "����7���Ƚ�ʧ��" );

    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psOutInfo)
//@DESCRIPTION: ���Բ���nIndex�Ƿ���psOutInfo��ȷ������£�����CSUDIBOOTSHOWGetShowUnitInfo��ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@INPUT: 1�� nIndex �Ƿ�ȡֵΪ-1���Ѵ洢��ʾ��Ԫ����+1
//@INPUT: 2�� psOutInfo ΪCSUDIBOOTSHOWUnitInfo�ṹ��ָ��
//@EXPECTATION: �ܹ���ȷ��ȡbooter��ʾ�õ����Ѵ洢��ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount���������سɹ�
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3��nIndex=-1������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 4��nIndex=uBootShowUnitCount+1������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
CSUDI_BOOL CSTC_BOOTSHOW_GetShowUnitInfo_0001(void)
{
    unsigned int uBootShowUnitCount = 0;
	CSUDIBOOTSHOWUnitInfo sUnitInfo;
            
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "����1����ȡ��Ԫ����ʧ��" );
    
    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "����2����ʾ��Ԫ����������0" );

    memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWGetShowUnitInfo(-1, &sUnitInfo), "����3����ȡ��ʾ��Ԫ��Ϣʧ��" );
        
	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWGetShowUnitInfo(uBootShowUnitCount+1, &sUnitInfo), "����4����ȡ��ʾ��Ԫ��Ϣʧ��" );
        
	CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psOutInfo)
//@DESCRIPTION: ���Բ���nIndex��ȷ��psOutInfoΪCSUDI_NULL������£�����CSUDIBOOTSHOWGetShowUnitInfo��ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@INPUT: 1�� nIndex ȡֵΪ0--�Ѵ洢��ʾ��Ԫ����-1
//@INPUT: 2�� psOutInfo = CSUDI_NULL
//@EXPECTATION: �ܹ���ȷ��ȡbooter��ʾ�õ����Ѵ洢��ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount��ȡ��ʾ��Ԫ����,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������Ϊ�գ�����CSUDIBOOTSHOWGetShowUnitInfo����������CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_GetShowUnitInfo_0002(void)
{
	int nIndex = 0;
    unsigned int uBootShowUnitCount = 0;
    
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "����1����ȡ��Ԫ����ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "����2����ʾ��Ԫ����������0" );

	CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, CSUDI_NULL), "����3����ȡ��ʾ��Ԫ��Ϣʧ��" );

	CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

static CSUDI_BOOL BOOTSHOW_iPrintUnitType( int nIndex, CSUDIBOOTSHOWUnitInfo* sUnitInfo )
{
	CSUDI_BOOL ret = CSUDI_TRUE;

    if( nIndex < 0 || CSUDI_NULL == sUnitInfo )
    {
    	CSTCPrint( "����Ĳ�������\n" );
		return CSUDI_FALSE;
    }

    switch( sUnitInfo->nUnitType )
    {
        case CSUDIBOOTSHOW_UNITTYPE_BMPBITS :
            CSTCPrint( "������Ϊ%d����ʾ��Ԫ�����ݸ�ʽΪ: ��������, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_BMP :
            CSTCPrint( "������Ϊ%d����ʾ��Ԫ�����ݸ�ʽΪ: BMPͼƬ, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_PNG:
            CSTCPrint( "������Ϊ%d����ʾ��Ԫ�����ݸ�ʽΪ: PNGͼƬ, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_JPG:
            CSTCPrint( "������Ϊ%d����ʾ��Ԫ�����ݸ�ʽΪ: JPGͼƬ, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_MPEG:
            CSTCPrint( "������Ϊ%d����ʾ��Ԫ�����ݸ�ʽΪ: MPGͼƬ, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_SIGALGIF:
            CSTCPrint( "������Ϊ%d����ʾ��Ԫ�����ݸ�ʽΪ: GIFͼƬ, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_BMPZIPS :
            CSTCPrint( "������Ϊ%d����ʾ��Ԫ�����ݸ�ʽΪ: ѹ����������, ", nIndex );  
            break;
        case CSUDIBOOTSHOW_UNITTYPE_DVBLIVE :
            CSTCPrint( "������Ϊ%d����ʾ��Ԫ�����ݸ�ʽΪ: ֱ����Ƶ, ", nIndex );
            break;
        case CSUDIBOOTSHOW_UNITTYPE_TSFILE :
            CSTCPrint( "������Ϊ%d����ʾ��Ԫ�����ݸ�ʽΪ: TS��Ƶ�ļ�, ", nIndex );
            break;
        default :
            CSTCPrint( "δ֪����ʾ��Ԫ��ʽ(nUnitType=0x%x)\n", sUnitInfo->nUnitType );
            ret = CSUDI_FALSE;
            break;
    }
                
    CSTCPrint("��ʾ��Ԫ�����ݴ�СΪ: 0x%x, ", (int)sUnitInfo->nDataSize );
        
    CSTCPrint("��ʾ��Ԫ�ı�ʶΪ: %u\n", sUnitInfo->nUnitId );

    return ret;
}

//@CASEGROUP: CSUDIBOOTSHOWGetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psOutInfo)
//@DESCRIPTION: ���Բ�������ȷ����£�����CSUDIBOOTSHOWGetShowUnitInfo��ִ�н��
//@DESCRIPTION: ����ȡ����ʾ��Ԫ����Ϣ�������ļ��е�������Ϣ���бȽϣ��ؼ���ΪCS_BOOTSHOW_CAPABILITY, ����ֵ
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@INPUT: 1��nIndexΪ�洢��ʾ��Ԫ��������ȡֵΪ0--�Ѵ洢��ʾ��Ԫ����-1��
//@INPUT: 2��psOutInfoΪCSUDIBOOTSHOWUnitInfo�ṹָ��
//@EXPECTATION: �ܹ���ȡ��booter��ʾ�õ����Ѵ洢��ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4����ӡ������3��ȡ��������ֵ�����ж��Ƿ�Ϊ�Ѷ������ʾ��Ԫ��ʽ������Ϊ�Ѷ������ʾ��Ԫ��ʽ
//@EXECUTIONFLOW: 5��nIndex��0��(�Ѵ洢��ʾ��Ԫ����-1),�ظ�����3��4
CSUDI_BOOL CSTC_BOOTSHOW_GetShowUnitInfo_0003(void)
{
    int nIndex = 0;
    unsigned int uBootShowUnitCount = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "����1����ȡ��Ԫ����ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "����2����ʾ��Ԫ����������0" );

    for( nIndex = 0; nIndex < uBootShowUnitCount; nIndex++ )
    { 
		memset( &sUnitInfo, 0, sizeof(sUnitInfo) );        
        CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sUnitInfo), "����3����ȡ��ʾ��Ԫ��Ϣʧ��" );

        CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iPrintUnitType(nIndex, &sUnitInfo), "δ֪����ʾ��Ԫ��ʽ" );
    }
    
    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetFreeSize(unsigned int* pnOutSize)
//@DESCRIPTION: ���Բ���ΪCSUDI_NULL������£�����CSUDIBOOTSHOWGetFreeSize��ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@INPUT: 1��pnOutSize = CSUDI_NULL
//@EXPECTATION: �ܹ���ȷ��ȡbooter�ɴ洢��ʾ���ݿռ��С
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetFreeSize����������CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_GetFreeSize_0001(void)
{
    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWGetFreeSize(CSUDI_NULL), "����1����ȡʣ��ռ�ʧ��" );

    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWGetFreeSize(unsigned int* pnOutSize)
//@DESCRIPTION: ���Բ�����ȷ������£�����CSUDIBOOTSHOWGetFreeSize��ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability�ӿ�ʵ��û����
//@INPUT: 1��pnOutSize Ϊʣ��ռ��С����λΪBYTE
//@EXPECTATION: �ܹ���ȷ��ȡbooter�ɴ洢��ʾ���ݿռ��С
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4��������Ѵ洢����ʾ��Ԫ���ܿռ��С(ÿ����ʾ��Ԫ���ݴ�С֮��)
//@EXECUTIONFLOW: 5������CSUDIBOOTSHOWGetCapability��ȡ���ڴ洢��Ԫ��Ϣ��flash�ռ�Ĵ�С
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize��ȡʣ��ռ�Ĵ�С����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ж�(����6�Ľ��)==(����5�Ľ��-����4�Ľ��)��������ʽ����
//@REMARK: ʣ��ռ�Ϊflash��ַ������ʣ����ÿռ��С
CSUDI_BOOL CSTC_BOOTSHOW_GetFreeSize_0002(void)
{
    unsigned int uBootShowUnitCount = 0;
    unsigned int uFreeSize = 0;
	int nIndex = 0;
    int nAllUnitSize = 0;
    CSUDIBOOTSHOWCapability sUnitCapability;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;   
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "����1����ȡ��Ԫ����ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "����2����ʾ��Ԫ����������0" );

    for( nIndex = 0; nIndex < uBootShowUnitCount; nIndex++ )
    {	    	
        memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
        CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sUnitInfo), "����3����ȡ��ʾ��Ԫʧ��" ); 
        
        nAllUnitSize += sUnitInfo.nDataSize;
    }

    memset( &sUnitCapability, 0, sizeof(sUnitCapability) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetCapability(&sUnitCapability), "����5����ȡ����ֵ��Ϣʧ��" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetFreeSize(&uFreeSize), "����6����ȡʣ��ռ�ʧ��" );

    CSTCPrint( "FreeSize=0x%x byte, FlashTotalSize=0x%x byte, AllUnitSize=0x%x byte\n", uFreeSize, sUnitCapability.nBooterShowFlashTotalSize, nAllUnitSize );

    CSTK_ASSERT_TRUE_FATAL( uFreeSize == (sUnitCapability.nBooterShowFlashTotalSize - nAllUnitSize), "����7��ʣ��ռ����ʧ��" );

    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//��ȡͼƬ��Ϣ
static unsigned int BOOTSHOW_iGetPhotoInfo( const char *photoName, unsigned char **pBuff )
{
    CSUDI_HANDLE fp = CSUDI_NULL;
    unsigned int nIframeLen = 0;
    CSUDI_BOOL bOpen = CSUDI_FALSE;

    if( CSUDI_NULL == photoName )
    {
		CSTCPrint("������ļ���ΪNULL\n");
        bOpen = CSUDI_FALSE;
    }

    if( CSUDI_NULL != (fp = CSTKP_FOpen(photoName, "rb")) )
    {
        CSTCPrint( "ͼƬ%s�򿪳ɹ�\n ", photoName );
        bOpen = CSUDI_TRUE;
    }

    else
    {
        CSTCPrint( "ͼƬ""%s ""��ʧ��\n", photoName );
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
            
            if( nIframeLen > 0 && nIframeLen < 4*1920*1080 )       //ͼƬ��С�Ϸ����ж�1920*1080�ֱ���ARGB8888 
            {
                *pBuff = CSUDIOSMalloc(nIframeLen); 
                if( *pBuff == CSUDI_NULL )
                {
                    CSTCPrint( "����ռ�ʧ��!\n" );
                    if( fp != CSUDI_NULL )
                    {
                        CSTKP_FClose( fp );
                    }
                    return 0;
                }
                memset( *pBuff, 0, nIframeLen );

                if( 0 != CSTKP_FSeek(fp,  0, CSTK_SEEK_SET) )
                {
                    CSTCPrint( "������ʼָ��ʧ��!\n" );
                    bOpen = CSUDI_FALSE;
                }
    
                if( nIframeLen != (CSTKP_FRead(*pBuff, sizeof(unsigned char),nIframeLen, fp)) )
                {
                    CSTCPrint( "��%s�ļ�ʧ��!\n", photoName );
                    bOpen = CSUDI_FALSE;
                }
            }
            else
            {
                CSTCPrint( "ͼƬ��С������ȷ��Χ��ʧ��!\n" );
                bOpen = CSUDI_FALSE;
            }
        }
        else
        {
            CSTCPrint( "Ѱ��ͼƬ%s ĩָ��ʧ��!\n", photoName );
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
//@DESCRIPTION: �������֧�ֵ����͵���ʾ��Ԫʧ��ʱ��flashʣ��ռ��С�Ƿ�ı�
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@INPUT: 1��pnOutSize Ϊʣ��ռ��С����λΪBYTE
//@EXPECTATION: flashʣ��ռ�Ӧ�ò����
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability�����жϳ�֧���������͵�ͼƬ
//@EXECUTIONFLOW: 4������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIBOOTSHOWSetShowUnitInfo��Ӳ���3��ȡ�����͵�ͼƬ
//@EXECUTIONFLOW: 6���������5����CSUDI_SUCCESS�����ظ�����4~5��ֱ������5���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 8���Ƚϲ���4�Ͳ���7�Ľ����������ͬ
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
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "����1����ȡ��Ԫ����ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "����2����ʾ��Ԫ����������0" );

	memset(&sUnitCapability, 0, sizeof(sUnitCapability));
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetCapability(&sUnitCapability), "����3����ȡ������Ϣʧ��" );
    
    while( i < PICTURECOUNT )
    {
    	  memset(&sUnitInfo, 0, sizeof(sUnitInfo));
        if( sUnitCapability.nBootDecodeCapablity & sPicInfo[i].sUnitInfo.nUnitType )
        {
            nImageLen = BOOTSHOW_iGetPhotoInfo( sPicInfo[3].acPicName, &g_pcPicBuff );
            sPicInfo[3].sUnitInfo.nDataSize = nImageLen;
            CSTK_ASSERT_TRUE_FATAL( 0 != nImageLen, "��ȡͼƬ��Ϣʧ��" );
            break;
        }
		CSTK_ASSERT_TRUE_FATAL( i != PICTURECOUNT, "����3����֧���κ����͵Ľ���" );
		i++;
    }

    while( 1 )
    {
    	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetFreeSize(&uFreeSize1), "����4����ȡʣ��ռ�ʧ��" );
		
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
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetFreeSize(&uFreeSize2), "����7����ȡʣ��ռ�ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( uFreeSize1 == uFreeSize2, "����8�������ʾ��Ԫʧ��ʱ��ʣ��ռ䲻Ӧ�øı�" );

    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//Ѱ�ҵ�һ��֧�ֵ����͵�ͼƬ������������
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
        CSTCPrint( "����Ĳ���ΪNULL\n" );
        return nRet;
    }
    
	if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount) )
    {
    	CSTCPrint( "����1����ȡ��Ԫ����ʧ��\n" );
        return nRet;
    }

   	if( uBootShowUnitCount <= 0 )
    {
		CSTCPrint( "����2����ʾ��Ԫ����������0\n" );
        return nRet;
    }

    memset( &sUnitCapability, 0, sizeof(sUnitCapability) );
    if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetCapability(&sUnitCapability) )
	{
        CSTCPrint( "����3����ȡ����ֵʧ��\n" );
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
			CSTCPrint( "����4����֧���κ����͵Ľ���\n" );        
			return nRet;
		}
	}
   
	if( (g_pcPicBuff == CSUDI_NULL) && nImageLen == 0 )
    {
		CSTCPrint( "����5����ȡͼƬ��Ϣʧ��\n" );        
        return nRet;
    }

	if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetFreeSize(&uFreeSize) )
    {
		CSTCPrint( "����6����ȡʣ��ռ��Сʧ��\n" );
		return nRet;
    }
    
	if( sPicInfo[i].sUnitInfo.nDataSize > (uFreeSize - sizeof(sPicInfo[i].sUnitInfo)) )
	{
		CSTCPrint( "����7��ʣ��ռ䲻��: uFreeSize = 0x%x, nDataSize = 0x%x\n", uFreeSize - sizeof(sPicInfo[i].sUnitInfo), sPicInfo[i].sUnitInfo.nDataSize );
        return nRet;
	}
    nRet = i;
    
    return nRet;        
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ����nIndex�Ƿ�������������ȷ������£�����CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@PRECONDITION: 3��CSUDIBOOTSHOWGetFreeSize �ӿ�ʵ��û������
//@INPUT: 1��nIdex �Ƿ�ȡֵΪ-1���Ѵ洢��ʾ��Ԫ����+1
//@INPUT: 2��psInfo Ϊ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: ��������CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�booter֧����������ͼƬ�Ľ���ģʽ��һ������һ�־�ִ�����²���
//@EXECUTIONFLOW: 5����ȡ����4����֧�ֵ�����ͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���6��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8��nIndex=-1������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 9��nIndex=uBootShowUnitCount+1������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@REMARK: �ɴ洢ͼƬ��С = ʣ��ռ��С - �洢ÿ��BootͼƬ����洢��ͼƬ��Ϣͷ����С
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0001(void)
{
	unsigned int uBootShowUnitCount = 0;
    int nReturn = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;

    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "���ҵ�һ��֧�ֵ�ͼƬ����ʧ��" );

	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWSetShowUnitInfo(-1, &sUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );

    CSUDIBOOTSHOWGetUnitCount( &uBootShowUnitCount );

    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWSetShowUnitInfo(uBootShowUnitCount+1, &sUnitInfo, g_pcPicBuff), "����9��������ʾ��Ԫ��Ϣʧ��" );

    CSTK_FATAL_POINT
    
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ����psInfo=CSUDI_NULL,����������ȷ������£�����CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@PRECONDITION: 3��CSUDIBOOTSHOWGetFreeSize �ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo ΪCSUDI_NULL
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: ����booter��ʾ�õ���ÿ����ʾ��Ԫ����Ϣʧ��
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�booter֧����������ͼƬ�Ľ���ģʽ��һ������һ�־�ִ�����²���
//@EXECUTIONFLOW: 5����ȡ����4����֧�ֵ�����ͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���6��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDIBOOTSHOW_ERROR_BAD_PARAMETER
//@REMARK: �ɴ洢ͼƬ��С = ʣ��ռ��С - �洢ÿ��BootͼƬ����洢��ͼƬ��Ϣͷ����С
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0002(void)
{
    int nIndex = 1;
    int nReturn = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;
    
    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "���ҵ�һ��֧�ֵ�ͼƬ����ʧ��" );

	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWSetShowUnitInfo( nIndex, CSUDI_NULL, g_pcPicBuff ), "����6��������ʾ��Ԫ��Ϣʧ��" );
    
    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ����pData=CSUDI_NULL,����������ȷ������£�����CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@PRECONDITION: 3��CSUDIBOOTSHOWGetFreeSize �ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: ����booter��ʾ�õ���ÿ����ʾ��Ԫ����Ϣʧ��
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�booter֧����������ͼƬ�Ľ���ģʽ��һ������һ�־�ִ�����²���
//@EXECUTIONFLOW: 5����ȡ����4����֧�ֵ�����ͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���6��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8��pData=CSUDI_NULL,����CSUDIBOOTSHOWSetShowUnitInfo����������SUDIBOOTSHOW_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0003(void)
{
    int nIndex = 1;
    int nReturn = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;
    
    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "���ҵ�һ��֧�ֵ�ͼƬ����ʧ��" );

	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );

    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_BAD_PARAMETER == CSUDIBOOTSHOWSetShowUnitInfo( nIndex, &sUnitInfo, CSUDI_NULL ), "����5��������ʾ��Ԫ��Ϣʧ��" );
    
    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: �����������==�Ѵ洢��ʾ��Ԫ����ʱ������CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@PRECONDITION: 3��CSUDIBOOTSHOWGetShowUnitInfo�ӿ�ʵ��û����
//@PRECONDITION: 4��CSUDIBOOTSHOWGetFreeSize �ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ�Ѵ洢��ʾ��Ԫ����
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: �ܹ��ɹ�����һ����ʾ��Ԫ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�booter֧����������ͼƬ�Ľ���ģʽ��һ������һ�־�ִ�����²���
//@EXECUTIONFLOW: 5����ȡ����4����֧�ֵ�����ͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���6��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8��nIndex==����1��ȡ���ĸ���������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10��������1�벽��9��ȡ�õ�ֵ���бȽϣ���������1ֵ==����9ֵ-1
//@EXECUTIONFLOW: 11������CSUDIBOOTSHOWGetShowUnitInfo���������سɹ�
//@EXECUTIONFLOW: 12��������8��ȡ��ֵ�벽��11���õ�ֵ���бȽϣ�������ͬ
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0004(void)
{
    unsigned int uBootShowUnitCount_1 = 0;
    unsigned int uBootShowUnitCount_2 = 0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo_1;
    CSUDIBOOTSHOWUnitInfo sUnitInfo_2;
    int nReturn = 0;

    CSTCPrint( "ע��:�����������һ����ʾ��Ԫ��������л�ʹflash�ռ䲻�����������ʧ��\n" );

    nReturn = BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo);

    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != nReturn, "���ҵ�һ��֧�ֵ�ͼƬ����ʧ��" );

	memset( &sUnitInfo_1, 0, sizeof(sUnitInfo_1) );
	memcpy( &sUnitInfo_1, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );
    
	CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount_1);

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(uBootShowUnitCount_1, &sUnitInfo_1, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount_2), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount_1 == (uBootShowUnitCount_2 - 1), "����10���Ƚ�ʧ��" );

    memset( &sUnitInfo_2, 0, sizeof(sUnitInfo_2) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(uBootShowUnitCount_1, &sUnitInfo_2), "����11����ȡ��ʾ��Ԫʧ��" ); 

    if( sUnitInfo_2.bNeedShow != sUnitInfo_1.bNeedShow || sUnitInfo_2.nDataSize != sUnitInfo_1.nDataSize
        || sUnitInfo_2.nUnitId != sUnitInfo_1.nUnitId || sUnitInfo_2.nUnitType != sUnitInfo_1.nUnitType
        )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����12���Ƚ�ʧ��" );
    }

    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ������ʾ��Ԫ����Ϣ�ǹ̶��ģ�����CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@PRECONDITION: 3��CSUDIBOOTSHOWGetFreeSize �ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: ����ʾ��Ԫ�ڲ������޸ĵ�����£�������ʾ��Ԫ��Ϣʧ��
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�booter֧����������ͼƬ�Ľ���ģʽ��һ������һ�־�ִ�����²���
//@EXECUTIONFLOW: 5����ȡ����4����֧�ֵ�����ͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���6��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDIBOOTSHOW_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0005(void)
{
    int nIndex = 0;
    int nReturn =  0;
    CSUDIBOOTSHOWUnitInfo sUnitInfo;

    CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "���ҵ�һ��֧�ֵ�ͼƬ����ʧ��" );

	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_FEATURE_NOT_SUPPORTED == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );
    
    CSTK_FATAL_POINT
        
    return CSUDI_TRUE;
}

//�Ƿ�֧��ָ�����͵�ͼƬ����
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
        CSTCPrint( "����Ĳ���ΪNULL\n" );
        return bRet;
    }
    
	if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount) )
    {
    	CSTCPrint( "����1����ȡ��Ԫ����ʧ��\n" );
        return bRet;
    }

   	if( uBootShowUnitCount <= 0 )
    {
		CSTCPrint( "����2����ʾ��Ԫ����������0\n" );
        return bRet;
    }
    
    memset( &sUnitCapability, 0, sizeof(sUnitCapability) );
    if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetCapability(&sUnitCapability) )
	{
        CSTCPrint( "����3����ȡ����ֵʧ��\n" );
		return bRet;
    }
    
	if( sUnitCapability.nBootDecodeCapablity & sUnitInfo->sUnitInfo.nUnitType )
	{
		nImageLen = BOOTSHOW_iGetPhotoInfo( sUnitInfo->acPicName, &g_pcPicBuff );
		sUnitInfo->sUnitInfo.nDataSize = nImageLen;
	}
    else
    {
		CSTCPrint( "����4����֧�� %s ��������ʾ��Ԫ����\n", sUnitInfo->acPicName);
        return bRet;
    }

	if( (g_pcPicBuff == CSUDI_NULL) && nImageLen == 0 )
    {
		CSTCPrint( "����5����ȡͼƬ��Ϣʧ��\n" );        
        return bRet;
    }

	if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetFreeSize(&uFreeSize) )
    {
		CSTCPrint( "����6����ȡʣ��ռ��Сʧ��\n" );
		return bRet;
    }

    if( CSUDI_SUCCESS != CSUDIBOOTSHOWGetShowUnitInfo( uBootShowUnitCount - 1, &sGetUnitInfo) )
    {
	    CSTCPrint( "��ȡ��ʾ��Ԫ��Ϣʧ��\n" );		
        return bRet;
    }

    if( uBootShowUnitCount == 1 
        && sUnitInfo->sUnitInfo.nDataSize > uFreeSize - sizeof(sUnitInfo->sUnitInfo) )
    {
        CSTCPrint( "����7.1��ʣ��ռ䲻�� " );
        CSTCPrint( "nDataSize = 0x%x��FreeSize = 0x%x\n", sUnitInfo->sUnitInfo.nDataSize, uFreeSize - sizeof(sUnitInfo->sUnitInfo) );
        return bRet;
    }
	else if( uBootShowUnitCount > 1 
     	&& sUnitInfo->sUnitInfo.nDataSize > (uFreeSize + sGetUnitInfo.nDataSize - sizeof(sUnitInfo->sUnitInfo)) )
	{
		CSTCPrint( "����7.2��ʣ��ռ䲻�� " );
        CSTCPrint( "nDataSize = 0x%x��FreeSize = 0x%x\n", sUnitInfo->sUnitInfo.nDataSize, uFreeSize + sGetUnitInfo.nDataSize - sizeof(sUnitInfo->sUnitInfo) );
        return bRet;
	}
    else
    {
    
    }

    return CSUDI_TRUE;    
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ������ʾ��Ԫ�����ݸ�ʽΪ��������ʱ������CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: �ܹ��ɹ�����booter��ʾ�õ���ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧�ֵ���������룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ��ģʽΪ���������ͼƬ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���7�Ͳ���8����Ϣ��������ͬ
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0006(void)
{ 
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPBITS].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPBITS].sUnitInfo) );
	CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPBITS]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );
       
    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );

    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����10���Ƚ�ʧ��" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ������ʾ��Ԫ�����ݸ�ʽΪBMP��ͼƬʱ������CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: �ܹ��ɹ�����booter��ʾ�õ���ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧��BMPͼƬ���룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ������ΪBMP��ʽͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���7�Ͳ���8����Ϣ��������ͬ
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0007(void)
{ 
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
   
	memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
	memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMP].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMP].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMP]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );
       
	memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����10���Ƚ�ʧ��" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}
   
//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ������ʾ��Ԫ�����ݸ�ʽΪPNGͼƬʱ������CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: �ܹ��ɹ�����booter��ʾ�õ���ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧��PNGͼƬ���룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ������ΪPNG��ʽͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���7�Ͳ���8����Ϣ��������ͬ
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0008(void)
{
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;

    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_PNG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_PNG].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_PNG]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����10���Ƚ�ʧ��" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}
    
//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ������ʾ��Ԫ�����ݸ�ʽΪJPGͼƬʱ������CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: �ܹ��ɹ�����booter��ʾ�õ���ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧��JPGͼƬ���룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ������ΪJPG��ʽͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���7�Ͳ���8����Ϣ��������ͬ
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0009(void)
{
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����10���Ƚ�ʧ��" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ������ʾ��Ԫ�����ݸ�ʽΪMPGͼƬʱ������CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: �ܹ��ɹ�����booter��ʾ�õ���ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧��MPGͼƬ���룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ������ΪMPG��ʽͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���7�Ͳ���8����Ϣ��������ͬ
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0010(void)
{
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;

    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_MPEG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_MPEG].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_MPEG]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����10���Ƚ�ʧ��" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ������ʾ��Ԫ�����ݸ�ʽΪGIFͼƬʱ������CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: �ܹ��ɹ�����booter��ʾ�õ���ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧��GIFͼƬ���룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ������ΪGIF��ʽͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���7�Ͳ���8����Ϣ��������ͬ
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0011(void)
{
	int nIndex = 1;
	CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
	CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
  
	memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
	memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_SIGALGIF].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_SIGALGIF].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_SIGALGIF]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );
   
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );
   
	if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
		|| sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
	{
		CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����10���Ƚ�ʧ��" );
	}
   
	CSTK_FATAL_POINT
         
	return CSUDI_TRUE;   
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ������ʾ��Ԫ�����ݸ�ʽΪѹ����������ʱ������CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: �ܹ��ɹ�����booter��ʾ�õ���ָ����ʾ��Ԫ����Ϣ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧��ѹ����������ͼƬ���룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ��ѹ����������ͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���7�Ͳ���8����Ϣ��������ͬ
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0012(void)
{
	int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPZIPS].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPZIPS].sUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_BMPZIPS]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����10���Ƚ�ʧ��" );
    }
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ����CSUDIBOOTSHOWUnitInfo.bNeedShow=trueʱ������CSUDIBOOTSHOWSetShowUnitInfo�Ƿ�����ʾ�����õ���ʾ��Ԫ
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: ��������ʾ��Ԫ�����óɹ��������ܳɹ���ʾ���õ���ʾ��Ԫ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧��JPGͼƬ���룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ������ΪJPG��ʽͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���7�Ͳ���8����Ϣ��������ͬ
//@EXECUTIONFLOW: 11���ֶ��������ӣ����۲�����
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0013(void)
{
    int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo) );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����10���Ƚ�ʧ��" );
    }

    CSTCPrint( "���ֶ����������У����������ʾ��ͼƬΪ��%s����������PASS\n", g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].acPicName );

    CSTCPrint( "�������������\n" );
    
    CSTKWaitAnyKey();

    CSTK_FATAL_POINT
          
    return CSUDI_FALSE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ����ͼƬ��С����100Kʱ������CSUDIBOOTSHOWSetShowUnitInfo�Ƿ���������ʾ�����õ���ʾ��Ԫ
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����1
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: ��������ʾ��Ԫ�����óɹ��������ܳɹ���ʾ���õ���ʾ��Ԫ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧��JPGͼƬ���룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ������ΪJPG��ʽͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���7�Ͳ���8����Ϣ��������ͬ
//@EXECUTIONFLOW: 11���ֶ��������ӣ����۲�����
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0014(void)
{
    int nIndex = 1;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;
    CSUDIBOOTSHOWUnitInfo sGetUnitInfo;
    
    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_100JPG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_100JPG].sUnitInfo) );

    sSetUnitInfo.bNeedShow = CSUDI_TRUE;

    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_100JPG]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );

    memset( &sGetUnitInfo, 0, sizeof(sGetUnitInfo) );
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetShowUnitInfo(nIndex, &sGetUnitInfo), "����9����ȡ��ʾ��Ԫ��Ϣʧ��" );
    
    if( sSetUnitInfo.bNeedShow != sGetUnitInfo.bNeedShow || sSetUnitInfo.nDataSize != sGetUnitInfo.nDataSize
        || sSetUnitInfo.nUnitId != sGetUnitInfo.nUnitId || sSetUnitInfo.nUnitType != sGetUnitInfo.nUnitType )
    {
        CSTK_ASSERT_TRUE_FATAL( CSUDI_FALSE, "����10���Ƚ�ʧ��" );
    }

    CSTCPrint( "���ֶ����������У����������ʾͼƬΪ��%s������������ȫ��ʾ�Ļ���������PASS\n", g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_100JPG].acPicName );

    CSTCPrint( "�������������\n" );

    CSTKWaitAnyKey();

    CSTK_FATAL_POINT
          
    return CSUDI_FALSE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo, void* pData)
//@DESCRIPTION: ��������ʾ��Ԫ�������������£�ѭ������CSUDIBOOTSHOWSetShowUnitInfo����ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount����Ҫ����2
//@PRECONDITION: 3��CSUDIBOOTSHOWGetCapability��CSUDIBOOTSHOWGetShowUnitInfo��CSUDIBOOTSHOWGetFreeSize�ӿ�ʵ��û������
//@INPUT: 1��nIdex Ϊ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
//@INPUT: 2��psInfo Ϊ��ʾ��Ԫ��Ϣ
//@INPUT: 3��pData Ϊ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
//@EXPECTATION: ÿ��������ʾ��Ԫ���ܳɹ�
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������1
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�BOOTER�Ƿ�֧��JPGͼƬ���룬����ִ��һ�²���
//@EXECUTIONFLOW: 5����ȡһ������ΪJPG��ʽͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���5��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8������CSUDIBOOTSHOWSetShowUnitInfo����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10���Ƚϲ���1�Ͳ���9����ȡ����ֵ��������ͬ
//@EXECUTIONFLOW: 11���ظ�����1~����10(5��)
CSUDI_BOOL CSTC_BOOTSHOW_SetShowUnitInfo_0015(void)
{
    int nIndex = 1;
	int i = 0;
    unsigned int uBootShowUnitCount1 = 0;
    unsigned int uBootShowUnitCount2 = 0;
    CSUDIBOOTSHOWUnitInfo sSetUnitInfo;

	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount1), "����1����ȡ��ʾ��Ԫ����ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount1 > 1, "����2����ʾ��Ԫ��������Ϊ2" );

    for( i = 0; i < 5; i++ )
    {
	    memset( &sSetUnitInfo, 0, sizeof(sSetUnitInfo) );
	    memcpy( &sSetUnitInfo, &g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo, sizeof(g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG].sUnitInfo) );
	    CSTK_ASSERT_TRUE_FATAL( CSUDI_TRUE == BOOTSHOW_iIsPicTypeSupported(&g_sPictureInfo[EM_BOOTSHOW_UNITTYPE_JPG]), "�ж�ָ��ͼƬ�����Ƿ�֧��ʧ��" );
	    
	    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(nIndex, &sSetUnitInfo, g_pcPicBuff), "����8��������ʾ��Ԫ��Ϣʧ��" );
        
        CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount2), "����9����ȡ��ʾ��Ԫ����ʧ��" );

		CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount1 == uBootShowUnitCount2, "����10����ʾ��Ԫ������Ӧ������" );
	}
    
    CSTK_FATAL_POINT
          
    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow)
//@DESCRIPTION: ������nIndexΪ�Ƿ�ʱ������CSUDIBOOTSHOWSetNeedShow��ִ�н��
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@INPUT: 1��nIndex �Ƿ�ȡֵΪ-1���Ѵ洢����ʾ��Ԫ����+1
//@INPUT: 2��bNeedShow bNeedShow �Ƿ���Ҫ��ʾ
//@EXPECTATION: ��������CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount��ȡ��ʾ��Ԫ��������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3��nIndex=-1,����CSUDIBOOTSHOWSetNeedShow����������CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 4��nIndex=�Ѵ洢����ʾ��Ԫ����+1,����CSUDIBOOTSHOWSetNeedShow����������CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
CSUDI_BOOL CSTC_BOOTSHOW_SetNeedShow_0001(void)
{
    unsigned int uBootShowUnitCount = 0;

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "����1����ȡ��Ԫ����ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "����2����ʾ��Ԫ����������0" );

    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWSetNeedShow(-1, CSUDI_TRUE), "����3��������ʾ��Ԫʧ��" );

    CSTK_ASSERT_TRUE_FATAL( CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID == CSUDIBOOTSHOWSetNeedShow((uBootShowUnitCount+1), CSUDI_TRUE), "����4��������ʾ��Ԫʧ��" );

    CSTK_FATAL_POINT

    return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow)
//@DESCRIPTION: ���Դ���������ʾ��Ԫ�������ʱ������CSUDIBOOTSHOWSetNeedShow��ʾָ������ʾ��Ԫ
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@PRECONDITION: 3��CSUDIBOOTSHOWSetShowUnitInfo�ӿ�ʵ��û������
//@INPUT: 1��nIndex �Ƿ�ȡֵΪ-1���Ѵ洢����ʾ��Ԫ����+1
//@INPUT: 2��bNeedShow �Ƿ���Ҫ��ʾ
//@EXPECTATION: ��������CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWGetCapability����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ж�booter֧����������ͼƬ�Ľ���ģʽ��һ������һ�־�ִ�����²���
//@EXECUTIONFLOW: 5����ȡ����4����֧�ֵ�����ͼƬ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIBOOTSHOWGetFreeSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7���жϲ���6��ȡ����ʣ��ռ��Ƿ��㹻���㹻��ִ�����²���
//@EXECUTIONFLOW: 8�������ȡ������ʾ��Ԫ��������1��ִ�в���9������ִ�в���9
//@EXECUTIONFLOW: 9������CSUDIBOOTSHOWSetShowUnitInfo���һ����ʾ��Ԫ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10��bNeedShow = CSUDI_TRUE������CSUDIBOOTSHOWSetNeedShow����������Ϊ1����ʾ��Ԫ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 11��bNeedShow = CSUDI_TRUE������CSUDIBOOTSHOWSetNeedShow����������Ϊ0����ʾ��Ԫ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 12��bNeedShow = CSUDI_TRUE������CSUDIBOOTSHOWSetNeedShow����������Ϊ1����ʾ��Ԫ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 13���ָ�Ĭ�ϵ���ʾ��Ԫ�b
CSUDI_BOOL CSTC_BOOTSHOW_SetNeedShow_0002(void)
{
	CSUDI_BOOL bNeedShow = CSUDI_TRUE;
    unsigned int uBootShowUnitCount = 0;
    int nReturn = 0;
	CSUDIBOOTSHOWUnitInfo sUnitInfo;
    
	CSTK_ASSERT_TRUE_FATAL( EM_BOOTSHOW_UNITTYPE_MAX != BOOTSHOW_iFindPicTypeSupported(g_sPictureInfo), "���ҵ�һ��֧�ֵ�ͼƬ����ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "��ȡ��ʾ��Ԫ����ʧ��" );
    
	memset( &sUnitInfo, 0, sizeof(sUnitInfo) );
	memcpy( &sUnitInfo, &g_sPictureInfo[nReturn].sUnitInfo, sizeof(g_sPictureInfo[nReturn].sUnitInfo) );

    if( 1 == uBootShowUnitCount )
    {
		CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetShowUnitInfo(uBootShowUnitCount, &sUnitInfo, g_pcPicBuff), "����9�������ʾ��Ԫʧ��" );
    }

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(1, bNeedShow), "����10��������ʾ��Ԫʧ��" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(0, bNeedShow), "����11��������ʾ��Ԫʧ��" );

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(1, bNeedShow), "����12��������ʾ��Ԫʧ��" );
	 
    CSTCPrint( "���ֶ����������У����������ʾ����ʾ��Ԫ��Ĭ�ϵĿ�����ʾ��Ԫ��һ���Ļ���������PASS\n" );
    
    CSTCPrint( "�������������\n" );

    CSTKWaitAnyKey();

    CSTK_FATAL_POINT
	
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(0, bNeedShow), "����13���ָ�Ĭ����ʾ��Ԫ" );

    return CSUDI_FALSE;
}

//@CASEGROUP: CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow)
//@DESCRIPTION: ���Ե���CSUDIBOOTSHOWSetNeedShowʹ������ʾ��Ԫ������ʾ�����
//@PRECONDITION: 1��booter���������������ҿ�����ʾ��Ԫ��������ʾ�򲥷�
//@PRECONDITION: 2��CSUDIBOOTSHOWGetUnitCount ����Ҫ����1 
//@PRECONDITION: 3��CSUDIBOOTSHOWSetShowUnitInfo�ӿ�ʵ��û������
//@INPUT: 1��nIndex = �Ѵ洢��ʾ��Ԫ����-1
//@INPUT: 2��bNeedShow bNeedShow �Ƿ���Ҫ��ʾ
//@EXPECTATION: �ܹ��ɹ�����booter��ʾĳ��ָ�����Ѵ洢������ʾ��Ԫ�����ܿ�����ʾָ���ĵ�Ԫ
//@EXECUTIONFLOW: 1������CSUDIBOOTSHOWGetUnitCount��ȡ��ʾ��Ԫ��������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Բ���1�л�ȡ�ĸ��������жϣ���������0
//@EXECUTIONFLOW: 3������CSUDIBOOTSHOWSetNeedShow�����п�����ʾ��Ԫ���óɿ�������ʾ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4���ػ����ٿ���������������ʾ������ʾ��Ԫ
CSUDI_BOOL CSTC_BOOTSHOW_SetNeedShow_0003(void)
{
    unsigned int uBootShowUnitCount = 0;
    int nIndex = 0;
    
    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWGetUnitCount(&uBootShowUnitCount), "����1����ȡ��Ԫ����ʧ��" );

    CSTK_ASSERT_TRUE_FATAL( uBootShowUnitCount > 0, "����2����ʾ��Ԫ����������0" );

    for( nIndex = 0; nIndex < uBootShowUnitCount; nIndex++ )
    {    
    	CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIBOOTSHOWSetNeedShow(nIndex, CSUDI_FALSE), "����3�����ÿ�����ʾ��Ԫ��Ϣʧ��" );
    }
    
    CSTCPrint( "���ֶ����������У����û����ʾ�κ�ͼƬ��û�в����κ�����Ƶ������PASS\n" );

	CSTCPrint( "�������������\n" );

    CSTKWaitAnyKey();
        
    CSTK_FATAL_POINT
        
    return CSUDI_FALSE;
}


