//booterError.h

#ifndef _BOOTER_ERROR_H_
#define _BOOTER_ERROR_H_

#ifdef __cplusplus
extern "C"{
#endif

typedef enum
{
    /* ... */
    EM_ERR_NO_MEMORY , /* Memory allocation failed */
    EM_ERR_ALREADY_INITIALIZED, /* Device already initialized */
    EM_ERR_FEATURE_NOT_SUPPORTED, /* Feature unavailable */
    EM_ERR_TIMEOUT, /* Timeout occurred */
    EM_ERR_CCRC,
    EM_ERR_OCRC,
    EM_BAD_DATA,
    EM_ERR_DECOMP,
    EM_ERR_PARAM,
    EM_FAILURE,
    EM_SUCCESS
}BLErrType_E;

#ifdef __cplusplus
}
#endif

#endif

