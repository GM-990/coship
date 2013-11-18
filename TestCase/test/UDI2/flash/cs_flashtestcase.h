#ifndef _CS_FLASH_TEST_CASE_H_
#define _CS_FLASH_TEST_CASE_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_FLASH_Init(void);
CSUDI_BOOL CSTC_FLASH_UnInit(void);

CSUDI_BOOL  CSTC_FLASH_IT_FlashGetInfo_0001(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashGetInfo_0002(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashGetInfo_0003(void);

CSUDI_BOOL CSTC_FLASH_IT_FlashRead_0001(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashRead_0002(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashRead_0003(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashRead_0004(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashRead_0005(void);

CSUDI_BOOL CSTC_FLASH_IT_FlashWrite_0001(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWrite_0002(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWrite_0003(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWrite_0004(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWrite_0005(void);

CSUDI_BOOL CSTC_FLASH_IT_EraseSpace_0001(void);
CSUDI_BOOL CSTC_FLASH_IT_EraseSpace_0002(void);
CSUDI_BOOL CSTC_FLASH_IT_EraseSpace_0003(void);
CSUDI_BOOL CSTC_FLASH_IT_EraseSpace_0004(void);
CSUDI_BOOL CSTC_FLASH_IT_EraseSpace_0005(void);
CSUDI_BOOL CSTC_FLASH_IT_EraseSpace_0006(void);
CSUDI_BOOL CSTC_FLASH_IT_EraseSpace_0007(void);
CSUDI_BOOL CSTC_FLASH_IT_EraseSpace_0008(void);
CSUDI_BOOL CSTC_FLASH_IT_EraseSpace_0009(void);

CSUDI_BOOL CSTC_FLASH_IT_FlashWriteWithoutErase_0001(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteWithoutErase_0002(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteWithoutErase_0003(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteWithoutErase_0004(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteWithoutErase_0005(void);

CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0001(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0002(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0003(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0004(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0005(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0006(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0007(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0008(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0009(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0010(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0011(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0012(void);
CSUDI_BOOL CSTC_FLASH_IT_FlashWriteRead_0013(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteRead_0014(void);

CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0001(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0002(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0003(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0004(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0005(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0006(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0007(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0008(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0009(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0010(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteWithoutEraseRead_0011(void);


CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0001(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0002(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0003(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashWriteReadTime_0004(void);

CSUDI_BOOL  CSTC_FLASH_IT_Performance_0001(void);
CSUDI_BOOL CSTC_FLASH_IT_Performance_0005(void);
CSUDI_BOOL  CSTC_FLASH_IT_Performance_0006(void);
CSUDI_BOOL CSTC_FLASH_IT_Performance_0007(void);


CSUDI_BOOL  CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0001(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0002(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0003(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0004(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0005(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0006(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0007(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashFewBytesStrengthWriteNoErase_0008(void);
CSUDI_BOOL  CSTC_FLASH_IT_FlashNeighborBlockWriteNoErase(void);


#ifdef __cplusplus
}
#endif

#endif

