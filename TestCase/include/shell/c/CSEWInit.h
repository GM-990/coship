/**@defgroup EW200Init EW200Init EW200��ʼ���ӿ�

@brief ��ģ��ΪEW200�м���ĳ�ʼ�����ýӿڣ�һ����˵���ɷ�ֱ��ʹ��MMCPInit���ɣ������ע��ģ�顣
@note ����EW200����UDI,����δ��ע����,��μ�UDI�ĵ�
@{
*/
#ifndef _CS_EW_INIT_H_
#define _CS_EW_INIT_H_

#include "udi2_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ����OC URL�ı��뷽ʽ
һ��OC��URL����:dvb://11.12.13/index.html
�ñ��ṹָ�������11��10����(��ʾ11)����16����(��ʾ17)
@see CSEWInitOc
*/
typedef enum
{
    EW_OC_TYPE_HEX,      ///< ʮ������
    EW_OC_TYPE_DEC       ///< ʮ����
}
EWOcType_E;

/**
@brief ����VOD����ģʽ
@see CSEWVODInitCITV3
*/
typedef enum
{
    EM_EW_ITV_MODE_IPTV,		///< iptv
    EM_EW_ITV_MODE_IPQAM,		///< ˫��
    EM_EW_ITV_MODE_DVBIP,		///< ˫ģ
    EM_EW_ITV_MODE_MAX
} CSEWMODE_E;

/**
@brief ����������汾��
@see CSEWVODInitCITV3
*/
typedef enum
{
    EM_EW_THREE_POINT_ZERO,		///< ��Ѷ3.0
    EM_EW_THREE_POINT_ONE     ///< ��Ѷ3.1
} CSEWVODServerVersion_E;

/**
@brief EW200�������ʼ������
@see CSEWInitGtools
*/
typedef struct
{
    int                  nStructSize;            ///< ���ṹ���С,�������sizeof(CSEWArgList_S)
    int                 nMemorySize;      ///< ��������ڴ��С������10M�����ִδ�CSMalloc����,��CSFree�ͷţ����ܴ�С���ᳬ����ֵ
    int                 nScreenWidth;	  ///< ��ʾ����ֱ���
    int                 nScreenHeight;    ///< ��ʾ����ֱ���
    int		    nCacheBufferSize; ///< ����������С.����Ĵ�С�ǰ�����nMemorySize����
    int 	            nX;			      ///< ��ʼ�����ڵ�Xλ��(�����ʾ�豸)
    int 	            nY;			      ///< ��ʼ�����ڵ�Yλ��(�����ʾ�豸)
    int 	            nWidth;           ///< ��ʼ�����ڵĿ��  ,������Ŀ����������ڴ�С,������Ŀ����̶���640,526,
    int 	            nHeight;	      ///< ��ʼ�����ڵĸ߶�
} CSEWArgList_S;

/**
@brief ��ʼ���ӿ�

EW200�������ʼ���ӿ�,�����нӿڵ���ǰ����.
@param[in] psArgList ��ʼ������,����ṹ����
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@note �ӿڽ��Խṹ����ǳ����,�������غ�pArgList�ṹ��������Ҫ
*/
CSUDI_BOOL CSEWInitGtools(CSEWArgList_S *psArgList);

/**
@brief ���ڰٻ�200����ѡ��
OC��ʼ���ӿ�.��ѡ,�����ҪOC����Ҫ���ô˽ӿڡ�
@param[in] nOcbufferSize �����OC���ڴ��С��ע��:��λΪK .��С2K�����ֵ�Ĵ�С����ϵͳ���ڴ��Լ�ǰ��OC�ĸ��Ӷ������䣬�Ƽ�ֵ:128M �ķ���5K;64M����2M����2K����
@param[in] eCode OC URL�ı��뷽ʽ��
@return CSUDI_TRUE :��ʾ�ɹ�;CSUDI_FALSE:��ʾʧ��
*/
CSUDI_BOOL CSEWInitOc(const int nOcbufferSize , EWOcType_E  eCode);

/**
@brief ����EW200����ѡ��
��ѶVOD��ʼ���ӿڡ���ѡ�������Ҫcitv3����Ҫ���ô˽ӿ�
@param[in] eMode vodģʽ���μ�CSEWMODE_E����
@param[in] eServerVer ǰ�˷������汾�� ���μ�CSEWVODServerVersion_E����
@return CSUDI_TRUE:��ʾ�ɹ�;CSUDI_FALSE:��ʾʧ��
*/
CSUDI_BOOL CSEWVODInitCITV3(CSEWMODE_E eMode , CSEWVODServerVersion_E eServerVer);

/**
@brief ����EW200����ѡ��
��ѶVOD��ʼ���ӿڡ���ѡ�����ʹ�õ������������ͬʱ����citv3����Ҫ���ô˽ӿ�
@note ���ʹ�õ��������������Ҫ�Լ�ȥʶ��Э��ͷ�����JS��������ο������ĵ���
@param[in] eMode vodģʽ���μ�CSEWMODE_E����
@param[in] eServerVer ǰ�˷������汾�� ���μ�CSEWVODServerVersion_E����
@return CSUDI_TRUE:��ʾ�ɹ�;CSUDI_FALSE:��ʾʧ��
*/
CSUDI_BOOL CSEWVODInitCITV3NoGT(CSEWMODE_E eMode , CSEWVODServerVersion_E eServerVer);

/**
@brief ����EW200����ѡ��
��ѶVOD��ʼ���ӿڡ���ѡ�����ֻ��Ҫ������Ѷvod��sa��֤����
@param[in] eMode vodģʽ���μ�CSEWMODE_E����
@param[in] eServerVer ǰ�˷������汾�� ���μ�CSEWVODServerVersion_E����
@return CSUDI_TRUE:��ʾ�ɹ�;CSUDI_FALSE:��ʾʧ��*/
CSUDI_BOOL CSEWCITVInitSA(CSEWMODE_E eMode ,CSEWVODServerVersion_E eServerVer);

/**
@brief ����EW200����ѡ��
��Ѷ�����ʼ���ӿڡ�
@param[in] eMode vodģʽ���μ�CSEWMODE_E����
@param[in] eServerVer ǰ�˷������汾�� ���μ�CSEWVODServerVersion_E����
@return CSUDI_TRUE:��ʾ�ɹ�;CSUDI_FALSE:��ʾʧ��*/
CSUDI_BOOL CSEWInitInShowMusic();

#ifdef __cplusplus
}

#endif
/** @} */
#endif//_CS_EW_INIT_H_
