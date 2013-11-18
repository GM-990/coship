
#ifndef CSSHELL_ADSDATA_H
#define CSSHELL_ADSDATA_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**@brief Ads初始化参数 */
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
        char m_szSatDelivery[1024];  //satellite 存储json字符串格式描述的卫星频点信息
    } CSShellAdsInitParam_S;


    /**
    @brief 获取Ads初始化参数
    @param[in] psAdsParam Ads初始化结构体指针
    @param[in] pcFilePath 配置文件路径
    @param[in] nFilePathLen 配置文件路径长度
    @return 成功返回TRUE,失败返回FALSE
    @notes 如果pcFilePath为NULL或nFilePathLen为0，则默认配置文件路径为ROOT_PATH"config/adsdata.properties"
    @notes 调用此接口时psAdsParam参数的m_nSize需赋值为sizeof(CSShellAdsInitParam_S)，否则可能获取不到数据
    */
    BOOL CSShellGetAdsData(CSShellAdsInitParam_S* psAdsParam, const char* pcFilePath, int nFilePathLen);

    /**
    @brief 获取Ads默认初始化参数，读取配置项时调用
    @param[in] psAdsParam Ads初始化结构体指针
    @return 成功返回TRUE,失败返回FALSE
    @notes 调用此接口时psAdsParam参数的m_nSize需赋值为sizeof(CSShellAdsInitParam_S)，否则可能获取不到数据
    */
    BOOL CSShellGetDefaultAdsData(CSShellAdsInitParam_S* psAdsParam);

#ifdef __cplusplus
}
#endif

#endif
