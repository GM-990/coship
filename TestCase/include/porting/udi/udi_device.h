
/*
 *该文件用于管理和提供芯片设备接口
 *
*/

#ifndef _UDI_DEVICE_H_
#define _UDI_DEVICE_H_

#include "udi_typedef.h"

typedef enum _CSSERVICEModeType
{
    CSSERVICE_MODE_NONE,	/**< 需添加注释*/
    CSSERVICE_MODE_DVB		/**< 需添加注释*/
} CSSERVICEModeType;

/* -------------------------- CLKRV --------------------------------------- */
typedef enum _CSCLKRVDeviceType
{
    CSCLKRV_0 = 0,			/**< 需添加注释*/
    CSCLKRV_1 = 1,			/**< 需添加注释*/
    CSCLKRV_MAXDEVICE		/**< 需添加注释*/
} CSCLKRVDeviceType;

#define CSCLKRV_MAXOPEN 	2	/*指每一个设备最大打开的句柄个数*/
/* -------------------------------------------------------------------------- */

/* -------------------------- FDMA ---------------------------------------- */
typedef enum _CSFDMADeviceType
{
    CSFDMA_AVMEM = 0,	/**< 需添加注释*/
    CSFDMA_MAXDEVICE	/**< 需添加注释*/
} CSFDMADeviceType;

#define CSFDMA_MAXOPEN 1
/* ------------------------------------------------------------------------ */

/* -------------------------- TSMERGE --------------------------------------- */
typedef enum _CSMERGEDeviceType
{
    CSMERGE_0 = 0,
    CSMERGE_MAXDEVICE
} CSMERGEDeviceType;

#define CSMERGE_MAXOPEN   1
/* ------------------------------------------------------------------------ */

/* -------------------------- DMX --------------------------------------- */
typedef enum _CSDMXDeviceType
{
    CSDMX_0,                   	/**< demux 0 */       
    CSDMX_1,                     /**< demux 1 */	  
    CSDMX_2,                   	/**< demux 2 */       
    CSDMX_3,                    	/**< demux 3 */      
    CSDMX_4,							/**< demux 4 */  
    CSDMX_TSOUT_0,		 	/**< ts output port 0 */   
    CSDMX_TSOUT_1,		 	/**< ts output port 1 */   
    CSDMX_MAXDEVICE		/**< max number of demux */
} CSDMXDeviceType;

typedef enum _CSDMXProcessStreamIdType
{
    CSDMX_PS_TSI    = 0,                  /**<  For TSI managment (PAT, PMT, SDT ...) */
    CSDMX_PS_AV1    = 1,                  /**<  Decode on AV CD1  */
    CSDMX_PS_AV2    = 2,                  /**<  Decode on AV CD2  */
    CSDMX_PS_PES    = 3,                  /**<  For PES managment (TTX, AC3...) */
    CSDMX_PS_MAXDEVICE
} CSDMXProcessStreamIdType;

#define CSDMX_MAXOPEN	CSDMX_PS_MAXDEVICE    /* ProcessStream ==== Handle */
/* ------------------------------------------------------------------------ */


/* -------------------------- AUD ----------------------------------------- */

typedef enum _CSAUDDeviceType
{
    CSAUD_INT    = 0,	/**< 需添加注释*/
    CSAUD_EXT    = 1,	/**< 需添加注释*/
    CSAUD_MAXDEVICE
} CSAUDDeviceType;

#define CSAUD_MAXOPEN	1
/* ------------------------------------------------------------------------ */

/* -------------------------- VID ----------------------------------------- */
typedef enum _CSVIDDeviceType
{
    CSVID_1_MPEG = 0,	/**< 需添加注释*/
    CSVID_MAXDEVICE		/**< 需添加注释*/
} CSVIDDeviceType;

#define CSVID_MAXOPEN	2
/* ------------------------------------------------------------------------ */

/* -------------------------- DENC ------------------------------------------ */
typedef enum _CSDENCDeviceType
{
    CSDENC_INT = 0,	/**< 需添加注释*/
    CSDENC_MAXDEVICE	/**< 需添加注释*/
} CSDENCDeviceType;

#define CSDENC_MAXOPEN	3
/* ------------------------------------------------------------------------ */

/* -------------------------- VTG ----------------------------------------- */
typedef enum _CSVTGDeviceType
{	
    CSVTG_MAIN = 0,		/**< 需添加注释*/
    CSVTG_AUX = 1,		/**< 需添加注释*/
    CSVTG_MAXDEVICE
} CSVTGDeviceType;

#define CSVTG_MAXOPEN	3
/* ------------------------------------------------------------------------ */

/* -------------------------- LAYER --------------------------------------- */
typedef enum _CSLAYERDeviceType
{
    CSLAYER_CURSOR,		/**< 需添加注释*/
    CSLAYER_GFX1,		/**< 需添加注释*/
    CSLAYER_GFX2,		/**< 需添加注释*/
    CSLAYER_VIDEO1,		/**< 需添加注释*/
    CSLAYER_VIDEO2,		/**< 需添加注释*/
    CSLAYER_STILL1,		/**< 需添加注释*/
    CSLAYER_STILL2,		/**< 需添加注释*/
    CSLAYER_MAXDEVICE 	/**< 需添加注释*/
} CSLAYERDeviceType;

#define CSLAYER_MAXOPEN	10
/* ------------------------------------------------------------------------ */

/* -------------------------- VOUT ---------------------------------------- */
typedef enum _CSVOUTDeviceType
{
    CSVOUT_MAIN = 0,		/**< 需添加注释*/
    CSVOUT_AUX = 1,		/**< 需添加注释*/
    CSVOUT_MAXDEVICE
} CSVOUTDeviceType;

#define CSVOUT_MAXOPEN	3
/* ------------------------------------------------------------------------ */

/* --------------------------- MIX ---------------------------------------- */
typedef enum _CSMIXDeviceType
{
    CSMIX_MAIN = 0,  /**<  HD */
    CSMIX_AUX = 1,    /**<  SD */
    CSMIX_MAXDEVICE
} CSMIXDeviceType;

/* -------------------------- VMIX ---------------------------------------- */
typedef enum _CSVMIXDeviceType
{
    CSVMIX_MAIN = 0,		/**< 需添加注释*/
    CSVMIX_AUX = 1,		/**< 需添加注释*/
    CSVMIX_MAXDEVICE
} CSVMIXDeviceType;

#define CSVMIX_MAXOPEN	2
/* ------------------------------------------------------------------------ */

/* -------------------------- BLITTER ------------------------------------- */
typedef enum _CSBLITDeviceType
{
    CSBLIT_ENGINE = 0,             /**<  eg : Unique blitter engine */
    CSBLIT_MAXDEVICE		/**< 需添加注释*/
} CSBLITDeviceType;

#define CSBLIT_MAXOPEN 	3
/* ------------------------------------------------------------------------ */


/* -------------------------- GFX ------------------------------------- */
typedef enum _CSGFXDeviceType
{
    CSGFX_ENGINE = 0,               /**<  eg : Unique blitter engine */
    CSGFX_MAXDEVICE			/**< 需添加注释*/
} CSGFXDeviceType;

#define CSGFX_MAXOPEN 	3
/* ------------------------------------------------------------------------ */

/* -------------------------- EVT ----------------------------------------- */
typedef enum _CSEVTDeviceType
{
    CSEVT_BACK = 0,	/**< 需添加注释*/
    CSEVT_MAXDEVICE	/**< 需添加注释*/
} CSEVTDeviceType;

#define CSEVT_MAXOPEN	20
/* ------------------------------------------------------------------------ */

/* -------------------------- VBI ----------------------------------------- */
typedef enum _CSVBIDeviceType
{
    CSVBI_0 = 0,		/**< 需添加注释*/
    CSVBI_MAXDEVICE	/**< 需添加注释*/
} CSVBIDeviceType;	

#define CSVBI_MAXOPEN	1	/**< 需添加注释*/
/* ------------------------------------------------------------------------ */

/* -------------------------- PWM ----------------------------------------- */
typedef enum _CSPWMDeviceType
{
    CSPWM_0 = 0,			/**< 需添加注释*/
    CSPWM_MAXDEVICE	/**< 需添加注释*/
} CSPWMDeviceType;

#define CSPWM_MAXOPEN	2
/* ------------------------------------------------------------------------ */

/* -------------------------- CFG ----------------------------------------- */
typedef enum _CSCFGDeviceType
{
    CSCFG_0 = 0,			/**< 需添加注释*/
    CSCFG_MAXDEVICE		/**< 需添加注释*/
} CSCFGDeviceType;

#define CSCFG_MAXOPEN	2
/* ------------------------------------------------------------------------ */
typedef enum _CSTUNERDeviceType
{
	CSTUNER_0,
	CSTUNER_1,
	CSTUNER_2,
    	CSTUNE_MAXDEVICE
} CSTUNERDeviceType;

typedef enum _CSTSINDeviceType
{
	CSTSIN_0,	/**< DVB*/
	CSTSIN_1,	/**< DVB*/
	CSTSIN_2,	/**< DVB*/
	CSTSIN_3,	/**< DVB*/
	CSTSIN_4,	/**< DVB*/
	CSTSIN_5,	/**< soft demux*/
	CSTSIN_6,	/**< soft demux*/
	CSTSIN_7,	/**< soft demux*/
	CSTSIN_8,	/**< soft demux*/
    	CSTSIN_MAXDEVICE	/**< 需添加注释*/
} CSTSINDeviceType;

typedef enum _CSHDMIDeviceType
{
	CSHDMI_0,		/**< 需添加注释*/
	CSHDMI_1,		/**< 需添加注释*/
	CSHDMI_2,		/**< 需添加注释*/
    	CSHDMI_MAXDEVICE	/**< 需添加注释*/
} CSHDMIDeviceType;


typedef enum _CSI2CDeviceType
{
	CSI2C_0,		/**< 需添加注释*/
	CSI2C_1,		/**< 需添加注释*/
	CSI2C_2,		/**< 需添加注释*/
	CSI2C_3,		/**< 需添加注释*/
	CSI2C_4,		/**< 需添加注释*/
    	CSI2C_MAXDEVICE	/**< 需添加注释*/
} CSI2CDeviceType;

#define CSDEVICE_IGNORE 0xFFFFFFFF	/**< 需添加注释*/

#endif

