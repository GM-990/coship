#include "cs_testkit.h"
#include "cs_flashtestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_flashtestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_FLASH_Init, CSTC_FLASH_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	//{CS_TK_AUTOCASE(CSTC_FLASH_MT_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashGetInfo_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashGetInfo_0002)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashGetInfo_0003)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashRead_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashRead_0002)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashRead_0003)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashRead_0004)},
	//{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashRead_0005)},

	
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWrite_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWrite_0002)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWrite_0003)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWrite_0004)},
	//{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWrite_0005)},
	
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_EraseSpace_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_EraseSpace_0002)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_EraseSpace_0003)},
	//{CS_TK_AUTOCASE(CSTC_FLASH_IT_EraseSpace_0004)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_EraseSpace_0005)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_EraseSpace_0006)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_EraseSpace_0007)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_EraseSpace_0008)},

	{CS_TK_AUTOCASE(CSTC_FLASH_IT_EraseSpace_0009)},
	
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutErase_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutErase_0002)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutErase_0003)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutErase_0004)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutErase_0005)},
	

	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0002)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0003)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0004)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0005)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0006)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0007)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0008)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0009)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0010)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0011)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0012)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0013)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteRead_0014)},
	
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0002)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0003)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0004)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0005)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0006)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0007)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0008)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0009)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0010)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0011)},
	
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteReadTime_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteReadTime_0002)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteReadTime_0003)},	
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashWriteReadTime_0004)},

	{CS_TK_AUTOCASE(CSTC_FLASH_IT_Performance_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_Performance_0005)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_Performance_0006)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_Performance_0007)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0001)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0002)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0003)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0004)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0005)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0006)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0007)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0008)},
	{CS_TK_AUTOCASE(CSTC_FLASH_IT_FlashNeighborBlockWriteNoErase)},

	//{CS_TK_MANUALCASE(CSTC_FLASH_MT_0002)},//CS_TK_MANUALCASE�����Թ�����������߽���
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSFLASHTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(FLASH)
/*end don't change*/

