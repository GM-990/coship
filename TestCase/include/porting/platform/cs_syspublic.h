#ifndef _CS_SYSPUBLIC_H_
#define _CS_SYSPUBLIC_H_

#include "cs_typedef.h"
#include "cs_assert.h"
#include "cs_device.h"
#include "udi_public.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ͬ����ʽ*/
typedef enum _CSSYSSync_E
{
	EM_SYNC_NONE = 0, 	/**< û������ͬ��*/
	EM_SYNC_PCR, 		/**< ����Ƶ��PCRͬ��*/
	EM_SYNC_VIDPTS,  	/**< ����Ƶ����Ƶͬ�� */
	EM_SYNC_AUDPTS	    /**< ����Ƶ����Ƶͬ��,�ݲ�֧��*/
}CSSYSSync_E;

/**@brief ��Ҫ���˵��*/
typedef enum
{
	DATA_VIDEO_OVERFLOW,	/**<  ��Ƶ����������*/
	DATA_VIDEO_UNDERFLOW,	/**<  ��Ƶ����������*/
	DATA_AUDIO_OVERFLOW,	/**<  ��Ƶ����������*/
	DATA_AUDIO_UNDERFLOW	/**<  ��Ƶ����������*/
}CSESDataInjectCallbackType;

/**@brief ��Ҫ���˵��*/
typedef struct _CSESBufferInfo
{
	PBYTE   pucVideoBuffer;			/**<  ��Ƶ��������������ַ*/
	int     nVideoBufferSize;		/**<  ��Ƶ����������������*/
	int     nVideoPreInjectTime;	/**<  ��Ҫ���˵��*/
	int     nVideoPreInjectSize;	/**<  ��Ҫ���˵��*/

	PBYTE   pucAudioBuffer;			/**<  ��Ƶ��������������ַ*/
	int     nAudioBufferSize;		/**<  ��Ƶ����������������*/
	int     nAudioPreInjectTime;	/**<  ��Ҫ���˵��*/
	int     nAudioPreInjectSize;	/**<  ��Ҫ���˵��*/
}CSESBufferInfo;

/**@brief ������32λ���ϱ�ʾ64λ��ֵ�Ľṹ*/
typedef struct _U64_clock_t
{
	DWORD  dwHigh;		/**<  64λclock�еĸ�32λ*/
	DWORD  dwLow;		/**<  64λclock�еĵ�32λ*/
}U64CLock_t;

/**@brief ��Ҫ���˵��*/
typedef struct _CSESSyncInfoEx
{
	BOOL        isSync; 		/**<  ����Ƶ�Ƿ�ͬ��*/
	U64CLock_t  pts;			/**< ��ǰPTS(��Ŀʱ���)*/
	U64CLock_t  video_pts; 		/**< video PTS*/
	U64CLock_t  audio_pts; 		/**< audio PTS*/
}CSESSyncInfoEx;

/**@brief ��Ҫ���˵��*/
typedef enum CSSYS_Memory_Type_e
{
    CSSYS_MEM_TYPE_SYSPART, /**< ��Ӧ��system partition*/
    CSSYS_MEM_TYPE_NONCACHE,/**< ��Ӧ��noncache partition*/
    CSSYS_MEM_TYPE_AVMEM, 	/**< ��Ӧ��av memory*/
    CSSYS_MEM_TYPE_OS,		/**< ����ϵͳʹ�õĶѿռ䣬һ��malloc�ڴ˿ռ��з���*/
    CSSYS_MEM_TYPE_DRIVER,	/**< Ƭ��/Ƭ��������ʹ�õĶѿռ�*/
    CSSYS_MEM_TYPE_AVMEM_USER	/**< ��Ӧ��ʹ�õ�AVMEM������Graphic��IFrame,GUI Buffer�ȵ� */
} CSSYSMemoryType;

typedef struct _CSSYSMemoryRange
{
	CSSYSMemoryType m_eMemoryType;/**< �ڴ���������*/
	void* m_pStartAddr;/**< �ڴ�������ʼ��ַ*/
	DWORD m_dwSize;/**< �ڴ������С*/
}CSSYSMemoryRange;

/**@brief ��Ҫ���˵��*/
typedef enum _CSSYSCfg_E
{
	CSCFG_TUNER_TSIN, 		/**< ����TUNER��Ӧ��TSIN, CSSYSChipInit֮ǰ���ã�dwDeviceΪCSTUNERDeviceType��dwValueΪCSTSINDeviceType,dwRservedĿǰ����Ϊ0 */
	CSCFG_TSIN_DMX,		/**< ����TSIN��Ӧ��DMX, CSSYSChipInit֮ǰ���ã�dwDeviceΪCSTSINDeviceType��dwValueΪCSDMXDeviceType,dwRservedĿǰ����Ϊ0 */
	CSCFG_HDMI_I2C,		/**< ����HDMI��Ӧ��I2C, CSSYSChipInit֮ǰ���ã�dwDeviceΪCSHDMIDeviceType��dwValueΪCSI2CDeviceType��dwRservedĿǰ����Ϊ0 */
	CSCFG_HDMI_PIO,		/**< ����HDMI��Ӧ��PIO����, CSSYSChipInit֮ǰ���ã�dwDeviceΪCSHDMIDeviceType��dwValueΪGPIO��0��ʼ��index��dwRservedĿǰ����Ϊ0 */
	CSCFG_TUNER_MODE, 	/**< Tuner���в���, CSSYSChipInit֮ǰ���ã�dwDeviceΪCSTUNERDeviceType��dwValueΪCSSYSTsMode_E��dwRservedĿǰ����Ϊ0 */
	CSCFG_TUNER_TYPE, 	 	/**< TUNER_C, TUNER_S, TUNER_T, CSSYSChipInit֮ǰ���ã�dwDeviceΪCSTUNERDeviceType��dwValueΪCSSYSTunerType_E��dwRservedĿǰ����Ϊ0 */
	CSCFG_SD_TIMING_MODE, 	/**< ����ƽ̨��ʼ��ʱ�ı���ֱ���, CSSYSChipInit֮ǰ���ã�dwDevice=> CSDEVICE_IGNORE, dwValueΪCSVIDTimingMode��dwRservedĿǰ����Ϊ0 */
	CSCFG_HD_TIMING_MODE,		/**< ����ƽ̨��ʼ��ʱ�ĸ���ֱ���, CSSYSChipInit֮ǰ���ã�dwDevice=> CSDEVICE_IGNORE, dwValueΪCSVIDTimingMode��dwRservedĿǰ����Ϊ0 */
	CSCFG_CSOS_USEPOSIX,		/**< ����ƽ̨�Ƿ�ʹ��Posix,Ŀǰֻ��st7100ƽ̨��Ч��dwDevice=> CSDEVICE_IGNORE, dwValueΪTRUE/FALSE��dwRservedĿǰ����Ϊ0 */
	CSCFG_HDMI_HDCP,                     /**<  ����ƽ̨��ʼ��ʱ�Ƿ��HDCP ���ܣ�dwDeviceΪCSHDMIDeviceType��dwValueΪCSSYSHdcpMode_E��dwRservedĿǰ����Ϊ0 */
	CSCFG_OSD_ADJUST_BOUNDS, /**<  ��СOSD��st7100ƽ̨��������dwDevice => CSLAYERDeviceType, dwValue TRUE or FALSE, dwRserved 0*/
	CSCFG_MEM_PROFILE,   		/**< �����ڴ����ã���Ӧ�����ݽṹCSSYSMemoryRange,Ŀǰֻ��st71xxƽ̨��Ч��dwDevice=> CSDEVICE_IGNORE, dwValueΪCSSYSMemoryRange�ṹָ�룬dwRservedĿǰ����ΪMemoryRange���� */
	CSCFG_VID_OUTPUT_MODE, 	/**< ����video output device��ģʽ,dwDevice=> CSVIDOutputDevice, dwValue=> CSVIDOutputMode, dwRserved 0*/
	CSCFG_VID_REINIT_WHEN_PLAY, 		/**< ��̨ʱ����ض�stream type�Ƿ�ǿ�����³�ʼ��video��dwDevice=>CSVIDDeviceType, dwValue=>CSVIDStreamType,dwRserved=>TRUE or FALSE, Ŀǰ��7100ƽ̨��Ч */
	CSCFG_OSD_REFRESH_ANTIALIASING, 	/**< ����OSD�Ƿ�֧��ȥˢ�¾��,dwDevice=> CSDEVICE_IGNORE, dwValue=> TREU or FALSE, dwRserved 0, ϵͳĬ���ǲ�֧�֣�Ŀǰ��7100ƽ̨��Ч*/
	CSCFG_SET_CSAVSTANDINIT_API, 	/**< ����ԭ��Ϊtypedef  void ( *CSPlatAvStandInit)(HCSHANDLE)��API������Ŀǰ��7100ƽ̨��Ч*/
	CSCFG_VID_DECODE_FRAME_NUM, /**<  ������Ƶ����֡��,Ŀǰ��7100ƽ̨��Ч,dwDevice=>CSDEVICE_IGNORE,dwValue=>CSVIDStreamTypeĿǰֻ֧��H264,dwRserved=>����֡buffer����,һ��Ϊ6~8*/
	CSCFG_POWER_CTL_GPIO, /**< ���ô���ʱ��ָ��gpio�Ŀ���--ǿ�����ߡ����ͻ��߲�����,Ŀǰ��Hisi3110eƽ̨��Ч,dwDeviceΪҪ������gpio�����ţ�dwRservedΪFALSEʱ��ʾǿ�Ʋ��Ը�gpio���в���(����������������жԸ�gpio�Ĳ���)����ʱdwValue��Ч��dwRservedΪTRUEʱ��ʾ����ʱǿ�Ʋ���gpio��dwValueָ��Ҫ���еĲ�����0Ϊ���ͣ�1������>*/
	CSCFG_TS_INJECT_SUPPORT, /**< �����Ƿ�֧��Tsע�빦�ܣ�dwDevice=> CSDEVICE_IGNORE, dwValueΪ TRUE or FALSE,dwRservedĿǰ����Ϊ0 */
	CSCFG_STORAGE_HDD_WA, /**< �����Ƿ�򿪶������ƶ�Ӳ�̵�workaround������st71xxƽ̨��Ч, dwDevice=> CSDEVICE_IGNORE, dwValueΪ TRUE or FALSE,TRUEΪenable��dwRservedĿǰ����Ϊ0  */
	CSCFG_AUD_AUTOVOLUME, /**< �����Ƿ����������������st5197 A37ƽ̨��Ч, dwDeviceΪCSAUDDeviceType�� dwValueΪ TRUE or FALSE,TRUEΪenable��dwRservedĿǰ����Ϊ0  */
	CSCFG_TSIN_SYNC_MODE/**< ����PTI ģ�����TS ����ģʽ��DEMO����DEMUXģ��İ���ʽ��Ϊ�첽ģʽ��ͬ��ģʽ���֣�dwDevice=> CSTSINDeviceType, dwValueΪ CSSYS_TS_PARALLEL ���� CSSYS_TS_SERIAL,dwRservedĿǰ����Ϊ0 **/
}CSSYSCfg_E;

/**@brief ��Ҫ���˵��*/
typedef enum _CSSYSHdcpMode_E
{
        CSSYS_HDCP_OPEN,		/**< ��Ҫ���˵��*/
        CSSYS_HDCP_CLOSE		/**< ��Ҫ���˵��*/
}CSSYSHdcpMode_E;

/**@brief ��Ҫ���˵��*/
typedef enum _CSSYSTunerType_E
{
	CSSYS_TUNER_C, 	/**< ����*/
	CSSYS_TUNER_S, 	/**< ����*/
	CSSYS_TUNER_T  	/**< ����*/
}CSSYSTunerType_E;

/**@brief ����ע������Ƶ�¼���ע����*/
typedef struct  _CSSYSEventItem
{
	int 		m_nNormalEvent;	    /**< �����¼�,��CSSYSEVENT_SIGNAL_LOCK*/
	int 		m_nAbnormalEvent;	/**< �쳣ʱ��,��CSSYSEVENT_SIGNAL_LOST*/
	BOOL 	    m_bServiceRelated;	/**< �¼��Ƿ�Service���*/
}CSSYSEventItem;

/**@brief ��Ҫ���˵��*/
typedef enum _CSSYSTsMode_E
{
	CSSYS_TS_PARALLEL,		/**< ��Ҫ���˵��*/
	CSSYS_TS_SERIAL		/**< ��Ҫ���˵��*/
}CSSYSTsMode_E;

/**@brief ��Ҫ���˵��*/
typedef enum _CSSYSEventMask
{
	CSSYSEVENT_MASK_ACTIVE = 0x01, 			/**< ��ʾ���¼��Ƿ�ΪActive*/
	CSSYSEVENT_MASK_SERVICE_RELATED = 0x02 	/**< ��ʾ���¼��Ƿ�Ϊ��Ŀ���,����������ִ�л�̨���ź����Ч��*/
}CSSYSEventMask;

/**@brief audio��video�����еĻ�����ע�빦���õ���Callbackԭ��*/
typedef void (* CSESDataInjectCallback)(CSESDataInjectCallbackType enmType, PVOID pvParam );

#ifdef  __cplusplus
}
#endif

#endif


