
#ifndef CSSHELL_ADSDATA_H
#define CSSHELL_ADSDATA_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**@brief Ads��ʼ������ */
    typedef struct _CSShellAdsInitParam
    {
        int m_nSize;
        int m_nFlag;
        int m_nFrequency;  //cable
        int m_nSymbolRate;
        int m_nModulation;
        int m_nPid;
        int m_nTableId;
        int m_nTimeout;
        DWORD m_dwScanDisableMask;
        DWORD m_dwStoreDisableMask;
        char m_acDefinition[4];
        int m_nTerFrequency;	//terrestrial
        int m_nTerBandwidth;
        char m_szSatDelivery[1024];  //satellite �洢json�ַ�����ʽ����������Ƶ����Ϣ
    } CSShellAdsInitParam_S;


    /**
    @brief ��ȡAds��ʼ������
    @param[in] psAdsParam Ads��ʼ���ṹ��ָ��
    @param[in] pcFilePath �����ļ�·��
    @param[in] nFilePathLen �����ļ�·������
    @return �ɹ�����TRUE,ʧ�ܷ���FALSE
    @notes ���pcFilePathΪNULL��nFilePathLenΪ0����Ĭ�������ļ�·��ΪROOT_PATH"config/adsdata.properties"
    @notes ���ô˽ӿ�ʱpsAdsParam������m_nSize�踳ֵΪsizeof(CSShellAdsInitParam_S)��������ܻ�ȡ��������
    */
    BOOL CSShellGetAdsData(CSShellAdsInitParam_S* psAdsParam, const char* pcFilePath, int nFilePathLen);

    /**
    @brief ��ȡAdsĬ�ϳ�ʼ����������ȡ������ʱ����
    @param[in] psAdsParam Ads��ʼ���ṹ��ָ��
    @return �ɹ�����TRUE,ʧ�ܷ���FALSE
    @notes ���ô˽ӿ�ʱpsAdsParam������m_nSize�踳ֵΪsizeof(CSShellAdsInitParam_S)��������ܻ�ȡ��������
    */
    BOOL CSShellGetDefaultAdsData(CSShellAdsInitParam_S* psAdsParam);

#ifdef __cplusplus
}
#endif

#endif
