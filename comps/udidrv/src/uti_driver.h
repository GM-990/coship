
#ifndef _UTI_DRIVER_SYSTEM_API_H_
#define _UTI_DRIVER_SYSTEM_API_H_

#include "basetype.h"
#include "kal_types.h"
#include "kal.h"
#include <signal.h>
#include "status_codes.h"

#include "unitend.h"

// driver needed api
UTI_SINT32 UTIDriverReadRegU8(UTI_UINT32 address, UTI_UINT8 *u8_data);
UTI_SINT32 UTIDriverWriteRegU8(UTI_UINT32 address, UTI_UINT8 u8_data);
UTI_SINT32 UTIDriverReadRegU16(UTI_UINT32 address, UTI_UINT16 *u16_data);
UTI_SINT32 UTIDriverWriteRegU16(UTI_UINT32 address, UTI_UINT16 u16_data);
UTI_SINT32 UTIDriverReadRegU32(UTI_UINT32 address, UTI_UINT32 *u32_data);
UTI_SINT32 UTIDriverWriteRegU32(UTI_UINT32 address, UTI_UINT32 u32_data);

UTI_SINT32 UTIDriverReadRegBuffer(UTI_UINT32 address, UTI_UINT8 *u8_data, UTI_UINT32 buffer_len);
UTI_SINT32 UTIDriverWriteRegBuffer(UTI_UINT32 address, UTI_UINT8 *u8_data, UTI_UINT32 buffer_len);

UTI_SINT32 UTIDriverSetBaseAddress(void *address);

CNXT_STATUS cs_cnxt_utidriver_init(void);


// init udrm driver
extern UTI_SINT32 UTIDriverInit(void);

#endif
