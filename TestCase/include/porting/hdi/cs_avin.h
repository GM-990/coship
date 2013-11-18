/**@defgroup CSI2C[CSI2C]ͬ����Ŀר��AVIN�����ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��AVIN�����ӿ�
@brief avģ���ź�ת���������źž���mpeg2ѹ�����������ts�����뵽��оƬ
 

@version 0.2 2010/01/06 �����ĵ�һ���汾
@version 0.1 2010/01/05��ʾ�汾
@{
*/

#ifndef _CS_AVIN_H_
#define _CS_AVIN_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "udi_hditypedef.h"


/**@brief Ŀǰ֧�ֶ�·�����û��ȡ���²���*/
typedef enum
{    
	CSAVIN_MUTE,			/**< ����Mute/Unmute ĳ��ͨ����Audio */
	CSAVIN_MACROVISION	/**< allows host software to get information about the macrovision type of the current video signal. */
} CSAVINPathParam_E;


/**@brief ����CSAVIN_MUTE��Ӧ�����ݽṹ*/
typedef struct
{
	BOOL m_bMute;	/**< False Unmute,TRUE mute*/
}CSAVINMute_S;


/**@brief ����CSAVIN_MACROVISION��Ӧ�����ݽṹ*/
typedef struct
{
	UINT16 m_wMacrovisionType;	/**< 0: Not_Protected*/
								/**< 1: Level1_Protected(Copy protected; no color stripe process present, typically VCR-tapes and DVD's)*/
								/**< 2: Level2_Protected(Copy protected with color stripe burst (two line version), typically DVD's)*/
								/**< 3: Level3_Protected(Copy protected with color stripe burst type3, typically DVD's)*/
								/**< 4: Level2_Cracked(Not protected, but color stripe burst still present, typically for cracked level2 sources)*/
								/**< 5: Level3_Cracked(Not protected, but color stripe burst type3 still present, typically for cracked level3 sources)*/	
}CSAVINMacroVision_S;


/**
@brief  ��ʼ��ANINģ�鲢�Զ����ø�·����,�������ȵ���

@param[in] eAVINIndex: AVIN������
@param[in] eI2cIndex: I2c������
@return: �ɹ�����CSHDI_SUCCESS������ֵʧ�ܣ���ο�ö��CSHDIComErrType���г�������ӡ��Ϣ
*/
CSHDI_Error_t CSAVINInitialize(CSHDIAVINIndex_E eAVINIndex, CSHDII2CIndex eI2cIndex);


/**
@brief  ����ĳһ·�Ĳ���

@param[in] eAVINIndex: AVIN������
@param[in] nPathIndex: path����
@param[in] eParam: �μ�CSAVINPathParam_E����,ָʾҪ���õ�����һ�����
@param[in] pvParam: ָ������ľ���ֵ
@return: �ɹ�����CSHDI_SUCCESS������ֵʧ�ܣ���ο�ö��CSHDIComErrType���г�������ӡ��Ϣ
@note:CSAVINInitialize���Զ����úø�·����,�����Ҫ���⻯�����ò���Ҫ���øú���
*/
CSHDI_Error_t CSAVINSetPathParam(CSHDIAVINIndex_E eAVINIndex, int nPathIndex, CSAVINPathParam_E eParam, void* pvParam);


/**
@brief  ��ȡĳһ·�Ĳ���

@param[in] eAVINIndex: AVIN������
@param[in] nPathIndex: path����
@param[in] eParam: �μ�CSAVINPathParam_E����,ָʾҪ��ȡ������һ�����
@param[out] pvParam: ָ������ľ���ֵ
@return: �ɹ�����CSHDI_SUCCESS������ֵʧ�ܣ���ο�ö��CSHDIComErrType���г�������ӡ��Ϣ
*/
CSHDI_Error_t CSAVINGetPathParam(CSHDIAVINIndex_E eAVINIndex, int nPathIndex, CSAVINPathParam_E eParam, void* pvParam);


/**
@brief  ����ָ��ANIN�豸��ĳһͨ��

@param[in] eAVINIndex: AVIN������
@param[in] nPathIndex :path����
@return : �ɹ�����CSHDI_SUCCESS������ֵʧ�ܣ���ο�ö��CSHDIComErrType���г�������ӡ��Ϣ
*/
CSHDI_Error_t CSAVINStartPath(CSHDIAVINIndex_E eAVINIndex, int nPathIndex);


/**
@brief  ָֹͣ��ANIN�豸��ĳһͨ��

@param[in] eAVINIndex: AVIN������
@param[in] nPathIndex :path����
@return  : �ɹ�����CSHDI_SUCCESS������ֵʧ�ܣ���ο�ö��CSHDIComErrType���г�������ӡ��Ϣ
*/
CSHDI_Error_t CSAVINStopPath (CSHDIAVINIndex_E eAVINIndex, int nPathIndex);


/**
@brief  ��ȡAVIN����ģ��İ汾��

@param[in]  nSize ����������
@param[out]  pcVer  ��Ű汾��Ϣ�Ļ������׵�ַ
@return  1> �����һ������pchVerΪ�գ����ذ汾��Ϣ��ʵ�ʳ���
  	     2> �����һ��������Ϊ�գ��Һ������óɹ����򷵻�ʵ
  	     �ʻ�ȡ�İ汾��Ϣ����
  	     3> ���ʧ�ܣ�����0	
*/
int CSAVINGetVersion(PCHAR pcVer, int nSize);


#ifdef __cplusplus
}
#endif


#endif /* !_CS_AVIN_H_ */

