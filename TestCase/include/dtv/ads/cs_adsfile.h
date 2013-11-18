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
@brief ���õ������ͼƬ�洢���ֵ
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param dwSize ����ͼƬ�洢���ֵ����λ:Byte
@param typeIndex ���λ��Ӧ������ֵ
@retval TRUE�����ɹ���FALSEʧ��
@note ȷ������ʱ���ɹ���
*/
BOOL CSAdsFile_SetMaxStoreSize(void *pASObj, int nTypeIndex, DWORD dwSize);

/**
@brief ��ȡ�������ͼƬ�洢���ֵ
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param typeIndex ���λ��Ӧ������ֵ
@retval �洢���ͼƬ���ݵ����ֵ
@note ȷ������ʱ���ɹ���
*/
DWORD CSAdsFile_GetMaxStoreSize(void *pASObj, int nTypeIndex);

#ifdef __cplusplus
}
#endif

#endif
