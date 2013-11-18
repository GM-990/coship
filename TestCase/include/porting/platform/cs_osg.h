/** @addtogroup CSOSG
@{
*/
#ifndef _CS_OSG_H_
#define _CS_OSG_H_

#include "cs_device.h"
#include "udi_osg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief ������εĽṹ��*/
typedef struct _CSOSGRect
{
  	int left; 
  	int top;
  	int Width;
  	int Height;
}CSOSGRect;

/**@brief ����λͼ��ʽ�Ľṹ��*/
typedef struct _CSOSGPixelFormat {
  	BYTE		m_ucRloss;			/**< ��ɫ������ռBITλ��ʣ��λ��*/
 	BYTE		m_ucGloss;			/**< ��ɫ������ռBITλ��ʣ��λ��*/
 	BYTE		m_ucBloss;			/**< ��ɫ������ռBITλ��ʣ��λ��*/
	BYTE		m_ucAloss;			/**< ͸�����ӷ�����ռBITλ��ʣ��λ��*/
	
	BYTE		m_ucRshift;			/**< ��ɫ�����������λ��λ��*/
	BYTE		m_ucGshift;			/**< ��ɫ�����������λ��λ��*/
	BYTE		m_ucBshift;			/**< ��ɫ�����������λ��λ��*/
	BYTE		m_ucAshift;			/**< ͸�����ӷ����������λ��λ��*/
	
	DWORD		m_dwRmask;			/**< ��ɫ����������*/
	DWORD		m_dwGmask;			/**< ��ɫ����������*/
	DWORD		m_dwBmask;			/**< ��ɫ����������*/
	DWORD		m_dwAmask;			/**< ͸�����ӷ���������*/
	
	DWORD		m_dwColorkey;			/**< ָʾ��Ӧ��ɫΪ͸��ɫ */	
	BYTE		m_ucBitsPerPixel;		/**< ÿ�����ص���ʹռ�õı�����������Ϊ1��2��4��8��15��16��24��32*/
	BYTE		m_ucBytesPerPixel;		/**< ÿ�����ص���ռ�õ��ֽ���������Ϊ1��2��3��4*/
} CSOSGPixelFormat_S;

/**
@brief ��ʼ��ϵͳͼ�δ���ģ�� 

@param[in] enmLAYERDeviceId �豸���ͣ�����ȡֵΪCSLAYER_GFX1 ������HD��ʾ������ CSLAYER_GFX2������SD��ʾ��
@return �ɹ�����CSOSG_SUCCESS,ʧ�ܷ���ΪCSOSG_FAILURE;
@note ��ʼ��֮�󣬻�ͬʱ����ͼ���豸�Լ������Դ�
*/
int CSOSGInit(CSLAYERDeviceType enmLAYERDeviceId);

/**
@brief ����һ����ʾ���洰�ڻ�����ʾ����

��bDualBufferΪTRUE������һ����ʾ���洰�ڣ�����ֻ����һ����ʾ����	
@param[in] enmLAYERDeviceId �豸���ͣ�����ȡֵΪCSLAYER_GFX1 ������HD��ʾ������ CSLAYER_GFX2������SD��ʾ��
@param[in] dwUserGraphicsIndex Ҫ�������ڴ滺����ʾ������������֧���㵽�����ʾ���棬�ò���������ʶ�ǻ�����š�
@param[in] pstRegion ָ��һ���������򣬻�������ĳ�ʼ����Ҫ���ݸò���������������Ĳ���ΪNULL������{0,0,720,576}����{0,0,1920,1080}(����)����ʼ����
@param[in] bDualBuffer  �Ƿ񴴽����棬��ΪFALSE����Ϊ����Ҫ���档
@return �ɹ�����CSOSG_SUCCESS,ʧ�ܷ���ΪCSOSG_FAILURE;
@note �ýӿڿɶ�ε��ã�����ͬһ��enmGraphicsDevice������һ�ε��õ�ʱ�򴴽��Դ档
*/
int CSOSGOpen(CSLAYERDeviceType enmLAYERDeviceId,DWORD dwUserGraphicsIndex,const CSOSGRect* pstRegion,BOOL bDualBuffer);

/**
@brief ��ȡָ��������ڴ��ַ

��bDualBufferΪTRUE������һ����ʾ���洰�ڣ�����ֻ����һ����ʾ����	
@param[in] enmLAYERDeviceId �豸���ͣ�����ȡֵΪCSLAYER_GFX1 ������HD��ʾ������ CSLAYER_GFX2������SD��ʾ��
@param[in] dwUserGraphicsIndex �ڴ滺����ʾ��������,�������ڻ����򷵻���ʾ������ڴ��ַ��
@return �ɹ�����ָ��������ڴ��ַ,ʧ�ܷ���Ϊ��ָ��NULL;
*/
BYTE* CSOSGGetScreenAddress(CSLAYERDeviceType enmLAYERDeviceId, DWORD dwUserGraphicsIndex);

/**
@brief �������ݿ���

�����������ʾ�Ļ����ָ�����򿽱�����ʾ������ȥ��������ʾͼ��
@param[in] enmLAYERDeviceId �豸���ͣ�����ȡֵΪCSLAYER_GFX1 ������HD��ʾ������ CSLAYER_GFX2������SD��ʾ��
@param[in] dwUserGraphicsIndex �ڴ滺����ʾ����������
@param[in] pstRegion  ָ��һ��������������ָʾ��Ҫ��������������
@return 
@note 
*/
void  CSOSGBufferExchange(CSLAYERDeviceType enmLAYERDeviceId, DWORD dwUserGraphicsIndex,CSOSGRect* pstRegion);

/**
@brief ��ָ����ɫ��ָ��������㻭һ���㡣

@param[in] enmLAYERDeviceId �豸���ͣ�����ȡֵΪCSLAYER_GFX1 ������HD��ʾ������ CSLAYER_GFX2������SD��ʾ��
@param[in] dwUserGraphicsIndex ����������ԭ�нӿڣ�����ʵ�����塣
@param[in] bDisplay : ΪTRUE��ʾ��ʾenmGraphicsDevice��Ӧ���豸,ΪFALSE��ʾ����enmGraphicsDevice��Ӧ���豸
@return �ɹ�����CSOSG_SUCCESS,ʧ�ܷ���CSOSG_FAILURE
@note 
*/
int CSOSGShow(CSLAYERDeviceType enmLAYERDeviceId, DWORD dwUserGraphicsIndex, BOOL bDisplay);

/**
@brief �趨ָ���Դ�ͻ��������ģʽ

@param[in] enmLAYERDeviceId �豸���ͣ�����ȡֵΪCSLAYER_GFX1 ������HD��ʾ������ CSLAYER_GFX2������SD��ʾ��
@param[in] stPixelFormat Ҫ�������ظ�ʽ,��CSOSGPixelFormat_S���ݽṹ����ģ�Ĭ��ΪARGB1555ģʽ��
@return �ɹ�����CSOSG_SUCCESS,ʧ�ܷ���CSOSG_FAILURE
@note 
*/
int CSOSGSetGlobalPixelFormat(CSLAYERDeviceType enmLAYERDeviceId,CSOSGPixelFormat_S  stPixelFormat);

/**
@brief �趨ϵͳ�Դ����ظ�ʽģʽ��

@param[in] enmLAYERDeviceId �豸���ͣ�����ȡֵΪCSLAYER_GFX1 ������HD��ʾ������ CSLAYER_GFX2������SD��ʾ��
@param[in] dwUserGraphicsIndex dwUserGraphicsIndex
@param[in] stPixelFormat Ҫ�������ظ�ʽ,��CSOSGPixelFormat_S���ݽṹ����ģ�Ĭ��ΪARGB1555ģʽ��
@return �ɹ�����CSOSG_SUCCESS,ʧ�ܷ���CSOSG_FAILURE
@note 
*/
int CSOSGSetScreenPixelFormat(CSLAYERDeviceType enmLAYERDeviceId, DWORD dwUserGraphicsIndex,CSOSGPixelFormat_S  stPixelFormat);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

