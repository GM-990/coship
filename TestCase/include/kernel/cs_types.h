/***********************************
 *
 * $Id$
 **********************************
 */
#ifndef  _CS_TYPES_H_
#define  _CS_TYPES_H_

#include <stdio.h>
#include <string.h>

#include "mmcp_typedef.h"
#include "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
	#error pls check 'make menuconfig' to contain LITTLE_ENDIAN or BIG_ENDIAN
#endif

typedef float		float32;
typedef double	float64;
typedef CSUDI_INT64		int64;
typedef CSUDI_UINT64 	uint64;


#ifdef __cplusplus
}
#endif

#endif /*_CS_TYPES_H_*/
