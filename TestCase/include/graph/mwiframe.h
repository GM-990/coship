/**@defgroup CSIFRAME iframe I֡��ʾ��ؽӿ�
@brief I֡��ʾ��ؽӿ�

@version 0.3
- ϸ��I֡��ʽ����ϸ˵��

@version 0.2
- ��Ӳ�����ϸ�����Ͳ��ֽӿں������ṹ

@version 0.1
- ��ʼ�汾
@{
*/

#ifndef _MW_IFRAME_H_
#define _MW_IFRAME_H_

#include "udiplus_typedef.h"
//#include "mwscreen.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef MMCP_UDI2

/**@brief I֡��ʾ�� */
typedef enum _CSIFrameLayerType_E
{
	CSIFAME_LAYER_BACKGROUND,		/**<  Background��*/
	CSIFAME_LAYER_STILLPICTURE,		/**<  Still ��*/
	CSIFAME_LAYER_VIDEO,			/**<  Video ��*/	
	CSIFAME_LAYER_GRAPHIC,     		/**<  OSD  ��*/
	CSIFAME_LAYER_CURSOR,			/**<  ����*/
    CSIFAME_LAYER_NUM				/**<  ���� */
}CSIFrameLayerType_E;


/**@brief I֡���� */
typedef enum _CSIFrameSourceFormat_E
{
	CSIFRAME_DATA_MPEG2_I ,				/**< MPEG2 I frame,both ES and PES is OK*/
	CSIFRAME_DATA_BITMAP_FILE ,			/**< Bitmap�ļ�*/
	CSIFRAME_DATA_PIXMAP		   		/**< CSGraphPixmap struct����ͼ*/
}CSIFrameSourceFormat_E;

/**@brief I֡�洢�ṹ */
typedef struct _CSIFrameBuf_T
{
	PBYTE		pucData;				/**< ���nDataSourceFormat��˵��*/
	int 		nDataLen;				/**< ���nDataSourceFormat��˵��*/
	int 		nDataSourceFormat;	    /**< CSIFRAME_DATA_MPEG2_I:pucDataָ��MPEG��ʽ���ݵ�ַ,nDataLen���ݳ���;
															CSIFRAME_DATA_BITMAP_RAW:pucDataָ��BMPͼƬ��ʽ���ݵ�ַ,nDataLen���ݳ���;
															CSIFRAME_DATA_BITMAP:pucDataָ��CSGraphPixmap�ṹ, nDataLenΪsizeof(CSGraphPixmap)*/
	int   		nIndex;					/**< IFrame ����ֵ, �����ظ�,CSIFrameShow��CSIFrameTerm�ĵ�һ������Ҫʹ��*/
}CSIFrameBuf_t;


/**@brief ������εĽṹ��*/
typedef struct _MWOSGRect
{
  	int left; 
  	int top;
  	int Width;
  	int Height;
}MWOSGRect;

/**
@brief ����I֡����

���ϲ㽫����IFrame Buffer�����ݵ�ַ���ݸ�ģ��ӿڣ�
��Buffer���ɶ��I_Frame �������ɣ������ɲ���nCount����֪ģ�顣
ÿ��I_Frame�Ľṹ����ΪCSIFrameBuf_t 			
@param[in] pstUserData IFrame����������׵�ַ
@param[in] nCount IFrame��Ŀ����������Ԫ�صĸ���
@return TRUEʱ���سɹ���FALSEʱ����ʧ��
@note CSIFrameBuf_t�ṹ�е�pucDataָ����ڴ��Ƿ�����ͷŴ��飬��pstUserData��������ͷ�
*/
BOOL MWIFrameInit( CSIFrameBuf_t* pstUserData, int nCount );

/**
@brief ������ֵ����IFrame��ʾ�Ĳ�.

@param[in] nIndex ��Ҫ��ʾ��I֡����
@param[in] eLayer I֡��Ҫ��ʾ�Ĳ�
@return TRUEʱ���سɹ���FALSEʱ����ʧ��
@note ֻ�Ե�ǰ���õ�nIndex��Ч�������I֡������ʾ״̬����ʱ������Ч�������������ʾ״̬�����´���ʾ��ʱ����Ч
*/
BOOL MWIFrameSetDisplayLayer(int nIndex, CSIFrameLayerType_E eLayer);

/**
@brief �����ڳ�ʼ�����������е�����ֵ��Ӧ��IFrame��

�˺��������������ͷ�nIndexָʾ��ͼƬ��
@param[in] nIndex Ҫ���ٵ�iframe�ڳ�ʼ�����������е�����ֵ
@return TRUEʱ���سɹ���FALSEʱ����ʧ��
@note ���nIndex��ֵ�д�С��0������ָʾ��ͼƬ�����ڣ��򷵻�FALSE��
@note �����ʱI֡����ʾ״̬���������أ����ͷ�
*/
BOOL MWIFrameTerm(int	nIndex);

/**
@brief ��ʾ/���� ������ֵ�洢��IFrame

@param[in] nIndex Ҫ��ʾ/���ص�iframe�ڳ�ʼ�����������е�����ֵ
@param[in] bIsShow TRUE:��ʾiframe;FALSE:�ر�iframe
@return �ɹ� TRUE;ʧ�� FALSE
@note ��ͬһ����ʾ���I֡������ʾ�ĸ�������ʾ�ģ���ʱ���غ���ʾ��I֡��
@note ����ʾ�Ĳ����Զ��ָ���ʾ
@note ���ڲ�����Ƶ��I֡���붼��Ҫ�õ���������Դ��������ֽ�������Դ��ͻ��
@note Cache��û�����е�����£��ýӿڿ��������Ƶ��ʱ���ڲ����������
@note ��I֡Ӧ���ܳɹ����롢��ʾ
*/ 
BOOL MWIFrameShow( int nIndex, BOOL bIsShow );

/**
@brief ������ӦIFRAME��ĳ�������͸����

@param[in] nIndex Ŀ��iframe�ڳ�ʼ�����������е�����ֵ
@param[in] pRegion ����
@param[in] nAlpha Ҫ���õ�͸����ֵ
@return �ɹ� TRUE;ʧ�� FALSE
@note �����ַΪNULLʱ��Ϊȫ��ģʽ��
@note �����СԽ��ʱ��ȡ��Ч�������ݡ�
@note nAlpha��ΧΪ0-100��͸��Ч������ɫģʽ��أ�����͸���ȣ���0Ϊȫ͸����100Ϊ��͸��
*/ 
BOOL MWIFrameFillRect(int nIndex, MWOSGRect * pRegion, int nAlpha);

#else

#define MWOSGRect           CSOSGRect
#define MWIFrameInit    CSIFrameInit
#define MWIFrameTerm    CSIFrameTerm
#define MWIFrameShow    CSIFrameShow
#define MWIFrameFillRect        CSIFrameFillRect
#define MWIFrameSetDisplayLayer CSIFrameSetDisplayLayer

#endif

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* _MW_IFRAME_H_ */

/* End of File */

