#ifndef _UDIDRV_LOG_H_
#define _UDIDRV_LOG_H_

#define UDIDRV_LOG 1
#define UDIDRV_NOT_REQUIRED  "***NOT Required***"
#define UDIDRV_IMPLEMENTED  "***Implemented***"

#if (UDIDRV_LOG == 1)
#define UDIDRV_LOGE(x...)   printf("Udi Driver Error: "x);
#define UDIDRV_LOGW(x...)    printf("Udi Driver Warning: "x);
#define UDIDRV_LOGI(x...)    printf("Udi Driver Info: "x);
#define UDIDRV_LOGV(x...)    
void CSDEBUG(char *string, int level, ...);
#else
#define UDIDRV_LOGE(x...)   cnxt_kal_trace(CNXT_TRACE_LEVEL_ALWAYS, "Udi Driver ERROR: "x);
#define UDIDRV_LOGW(x...)    cnxt_kal_trace(CNXT_TRACE_LEVEL_ALWAYS, "Udi Driver WARNING: "x);
#define UDIDRV_LOGI(x...)    cnxt_kal_trace(CNXT_TRACE_LEVEL_ALWAYS, "Udi Driver INFO: "x);
#define UDIDRV_LOGV(x...)    cnxt_kal_trace(CNXT_TRACE_LEVEL_ALWAYS, "Udi Driver VERBOSE: "x);
#endif

#endif
