#ifndef _CS_NDS_ADSYS_H_
#define _CS_NDS_ADSYS_H_

#include"udiplus_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CS_NDSAD_RESOLUTION  "ndsAd.Resolution"
#define CS_NDSAD_TABLEID "ndsAd.TableId"
#define CS_NDSAD_PID "ndsAd.Pid"
#define CS_NDSAD_FREQUENCE "ndsAd.Frequence"
#define CS_NDSAD_MODULATION "ndsAd.Modulation"
#define CS_NDSAD_SYMBOL "ndsAd.Symbol"
#define CS_NDSAD_TIMEOUT "ndsAd.Timeout"

#define CS_NDSAD_TABLE_VERSION "ndsAd.TableVersion"

#define CS_NDSAD_ZIP_CODE "ndsAd.zipCode"

//需要设置env
#define CS_NDSAD_CONFIG "ndsAd.configFile"
#define CS_NDS_AD_STORE_PATH "ndsAd.storeDir"
#define CS_NDS_AD_STORE_BAK_PATH "ndsAd.storeBakDir"
#define CS_NDS_AD_DEFAULT_STARTAD_BAK_PATH "ndsAd.defaultAdBakDir"

#define CS_NDS_AD_DEFAULT_STARTPIC "omp.mpeg"
	
#define	AD_BOOT_ADS    5000
#define	AD_MAIN_MANU   5001
#define	AD_EPG_DOWN    5002
#define	AD_CHANNELBAR  5006


//启动广告搜索
BOOL CSNDSADSysStart();

//锁主频点， 并启动广告搜索
BOOL CSNDSADSysStartByTune();

BOOL CSNDSADSysDestroy();


#ifdef __cplusplus
}
#endif
#endif
