#ifndef _CS_ADSFILE_H_
#define _CS_ADSFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

DWORD CSAdsFile_GetFrequency(void);
DWORD CSAdsFile_GetSymbolRate(void);
DWORD CSAdsFile_GetModulation(void);
BOOL CSAdsFile_SetFrequency(DWORD dwFrequency);
BOOL CSAdsFile_SetSymbolRate(DWORD dwSymbolRate);
BOOL CSAdsFile_SetModulation(DWORD dwModulation);

DWORD CSAdsFile_GetTerFrequency(void);
DWORD CSAdsFile_GetTerBandwidth(void);
BOOL CSAdsFile_SetTerFrequency(DWORD dwFrequency);
BOOL CSAdsFile_SetTerBandwidth(DWORD dwBandwidth);

int CSAdsFile_GetTimeout(void);

int CSAdsFile_GetDefinition(void);
void CSAdsFile_StoreBlockIds(void *pASObj);
BOOL CSAdsFile_SetStbIdOpen(void *pAds, BOOL bOpen);
void CSAdsFile_InitAds(void *dwUserData);
WORD CSAdsFile_GetPid(int nDefinition);
BYTE CSAdsFile_GetTableId(void);

/**
@brief 设置单个广告图片存储最大值
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param dwSize 单个图片存储最大值，单位:Byte
@param typeIndex 广告位对应的索引值
@retval TRUE操作成功，FALSE失败
@note 确保广告初时化成功。
*/
BOOL CSAdsFile_SetMaxStoreSize(void *pASObj, int nTypeIndex, DWORD dwSize);

/**
@brief 获取单个广告图片存储最大值
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param typeIndex 广告位对应的索引值
@retval 存储广告图片数据的最大值
@note 确保广告初时化成功。
*/
DWORD CSAdsFile_GetMaxStoreSize(void *pASObj, int nTypeIndex);

#ifdef __cplusplus
}
#endif

#endif
