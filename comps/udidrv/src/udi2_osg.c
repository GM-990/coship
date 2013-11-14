/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_os.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_osg.h"
/**
@brief ����һ����ͼƽ��

@param[in] ePixelFormat ��ͼƽ�����ɫģʽ��
@param[in] nWidth  ��ͼƽ��Ŀ�ȣ�������� 0��
@param[in] nHeight ��ͼƽ��ĸ߶ȣ�������� 0��
@param[out] phSurface �ɹ����ػ�ͼƽ��ľ����ʧ��Ϊ�գ���ϸ����μ��������ֵ��
@return �ɹ����� CSUDI_SUCCESS,���򷵻ش������ֵ�� 
@note 

- �ýӿڴ�����ʾ����ռ䣬�����Ͽ���֧�����޴�֧�ִ������޶��ֱ���ڴ治�㣬(�����ṩ16M���ڴ����ڴ���Surface)��������Ҫ����Surface������Ŀ����Ĳ�ͬ����ͬ��ĿǰӦ���õ��Ƚ϶����:\n
     1����ϷӦ���еĿɱ�ͼƬ���õ���Surface�� \n
     2��JavaӦ�����õ�BufferedImage�õ���Surface��\n 
     3��2D��Ч��ʵ�ֻ��õ���Surface)�� \n
- �˴���ɫģʽ������������ʾ����ɫģʽ��ͬ����Blitʱ�����⴦���ɣ����ƽ̨��֧�������Ĵ���Ҳ����Ӧ��ѡ����ɫģʽʱ���Ա��⡣\n
- ���ƽ̨��Դʵ�����ޣ�Ҳ�������ܴ����κ�Surface����������»�ֱ�Ӳ����Դ棬������Ҫ��������Դ�ķֱ��ʣ���ɫģʽ�ȱ���һ�¡�
- ���Ӳ����֧���ٴ���Surfaceʱ�����鷵��ʧ�ܡ���ʱ�м����ģ�ⴴ��surface�����в�����
*/
CSUDI_Error_Code CSUDIOSGCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat, int nWidth, int nHeight, CSUDI_HANDLE * phSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ����һ����ͼƽ�棬�ͷŸû�ͼƽ����ڴ档

@param[in] hSurface CSUDIOSGCreateSurface �ɹ����صľ����
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ 
@note 
- ��CSUDIOSGCreateSurface�����Ļ�������ʹ�ú�һ��Ҫ���ô˽ӿ������ͷš�\n
- ��������һ���Դ�ռ䣬������CSUDIOSG_ERROR_BAD_PARAMETER��
*/
CSUDI_Error_Code CSUDIOSGDestroySurface(CSUDI_HANDLE hSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	 
	return Retcode;
}


/**
@brief �õ��Դ�surface�����

@param[in] nIndex �Դ���������0��ʼֱ��û�и�����Դ棬����ʧ��Ϊֹ��
@param[out] phSurface �ɹ������Դ�Surface�����

@return �ɹ����� CSUDI_SUCCESS,���򷵻ش�����롣

@note 
- ���ַ�ʽ�õ���Surface����ʹ��CSUDIOSGDestroySurface���١�\n
- ����߱���ͬʱ֧�֣�nIndex Ϊ 0 ��ʾ�����Դ棻nIndex Ϊ 1 ��ʾ�����Դ档\n
- ֻ��Ҫ����������������OSD��ʾ���Դ�,������Դ棬�����Դ棬��Ҫ����������Դ�����㣬������ȡ�\n
- ���ǲ��ܴ����κ�����Surface�������м��һ�㲻��ֱ�Ӳ����Դ��ַ���л���,�þ�����������ֻ���������Ͻ���Blit��
*/
CSUDI_Error_Code CSUDIOSGGetDisplaySurface(int nIndex, CSUDI_HANDLE * phSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ��ͼƽ������Դ�ռ����Ϣ��

�м����ͨ���ýӿڻ�ȡ����Ϣ���л�ͼ������
@param[in] hSurface ��ͼƽ������Դ�ռ���.CSUDIOSGCreateSurface����CSUDIOSGGetDisplaySurface�ķ���ֵ��
@param[out] psSurfaceInfo ����õ��Ļ�ͼƽ������Դ�ռ���Ϣ��
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ ��
@note
- �˽ӿ��ܹ��õ��Դ���߻���Ŀռ��ַ
- Alpha����0xFF����CSUDIOSG_ERROR_BAD_PARAMETER
- ����֧��0xFF��͸���ȣ������������н�0-255ӳ�䵽��֧�ֵķ�Χ�ڣ����ý�����֧�ֲ��֡�
��ĳƽ̨��֧��0-127��͸���ȣ����ֵ����255Ӧ�൱��127������127Ӧ�൱��63
*/
CSUDI_Error_Code CSUDIOSGGetSurfaceInfo(CSUDI_HANDLE hSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ���û�ͼƽ�����Ϣ��ֻ�����û�ͼƽ���m_nAlphaFlag��m_uAlpha��m_nColorKeyFlag��m_uColorkey�ĸ���Ա����������Ա������Ч��

@param[in] hSurface ��ͼƽ����.CSUDIOSGCreateSurface����ֵ��
@param[in] psSurfaceInfo ����õ��Ļ�ͼƽ������Դ�ռ���Ϣ��
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ ��
@note 
- �˽ӿ���CSUDIOSGGetSurfaceInfo��ʹ�ã�ֻ�����û�ͼƽ�����Ϣ�����������Դ�ռ����Ϣ��\n
- �˽ӿ��ڽ������ڸı�Surface��m_uAlpha��m_uColorkey���ԣ����������ʾ�����仯��\n
- ��psSurfaceInfo��ͬʱ�г�m_nAlphaFlag��m_uAlpha��m_nColorKeyFlag��m_uColorkey����ĳ�Ա�ı�ʱ��������Ա�޸Ĳ�����Ч��������m_nAlphaFlag��m_uAlpha��m_nColorKeyFlag��m_uColorkey��Ч������CSUDI_SUCCESS��
- m_uAlpha����0xFF����CSUDIOSG_ERROR_BAD_PARAMETER
- ����֧��0xFF��͸���ȣ������������н�0-255ӳ�䵽��֧�ֵķ�Χ�ڣ����ý�����֧�ֲ��֡�
��ĳƽ̨��֧��0-127��͸���ȣ����ֵ����255Ӧ�൱��127������127Ӧ�൱��63
*/
CSUDI_Error_Code CSUDIOSGSetSurfaceInfo(CSUDI_HANDLE hSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief Blit�����ӿڣ�������Ӳ���������ܣ��첽������

@param[in] hDstSurface Ŀ���ͼƽ������Դ�ռ���.CSUDIOSGCreateSurface����CSUDIOSGGetDisplaySurface�ķ���ֵ��
@param[in] psDstRect Ŀ��surface����ʾ�ռ�hDstSurface�Ͼ�������,���Ϊ�գ���ȡhDstSurface��ȫ������
@param[in] hSrcSurface Դ��ͼƽ������Դ�ռ�����
@param[in] psSrcRect Դsurface����ʾ�ռ�hSrcSurface�Ͼ�������,���Ϊ�գ���ȡhSrcSurface��ȫ������
@param[in] eOpMode ���ģʽ����CSUDIOSGOpMode_E���塣
@return �ɹ����� CSUDI_SUCCESS���������ش�����롣
@note 
- �����alpha���ģʽ��ȫ��alphaֵ����ԴSurface��Ŀ��Surface��alpha flag ȷ�������Դ��Ŀ��Surface��alpha flagȫΪ1����ʹ��.���ʱ��alphaֵ����
      ���߱����ó�������:���ֻ��Դ��alpha flag Ϊ1�� ��ϵ�alphaΪ Sa * Pa / 255 ��SaΪԴsurface��ȫ��alpha��PaΪԴsurface��һ�����ص�alpha��
- ���Ӳ����֧�֣�����Ҫ���ģ��ʵ�֣�ֱ�ӷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED
- �����֧��Ӳ�������뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED���������и���Ч��ʵ�ַ�ʽ����������ֵ�����,����Ҫ�Ըò����������ģ�⡣\n
- Դ��Ŀ��������СpsSrcRect��psDstRect����Ϊ�գ����Ƿǿ�ʱ�������Ƕ�Ӧsurface�ϴ�С�ķ�Χ�ڣ���֧�ֲü��򷵻�CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ�������Ϊ�Դ�Surface������ͨSurface������֧���뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ�����Ϊͬһ��Surface������֧���뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ�����Ϊͬһ��Surfaceʱ��psDstRect��psSrcRect�������ص������򷵻�CSUDIOSG_ERROR_BAD_PARAMETER��\n
- Դ��Ŀ�����ɫģʽ���ܲ�һ�£�һ�㲻�Ƽ�����ʹ�ã�����֧�ֿɷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ����ֱܷ���YUV��RGB����ɫģʽ������֧�ֿɷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��\n
- Դ��Ŀ����εĴ�С���ܲ�һ��,����֧�ֿɷ���CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,��һ����˵����ƽ̨����֧�֣����������Ч�ʼ��ͣ��м�������ṩ��\n
- Դ��Ŀ��ľ��ο��������š�����ȴ���,����֧���뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED����֧��2D��Ч��ƽ̨����ʵ�ָù��ܡ�\n
- Ϊ�����߻�ͼ���ܣ��ýӿڿ�ʵ�ֳ��첽ģʽ��ͨ��CSUDIOSGSyncȷ���ò���ִ����ϡ�\n
- ��CSUDIOSGSync����ǰ�ı����Surface����ͼЧ���ǲ�ȷ���ģ�����������������������ɻָ����쳣��\n
- ���ƽ̨ͬʱ֧�ָ߱�����ʾ��Ҫ����������Դ�blitʱ��һ��Ҫ���÷���(���߷����)������\n
- ƽ̨֧�ֵ���������ű�������Ϊ: 1:15��\n
- ����blit����ʱ��ֻ��Դ��colorkey��Ч������Դ��ͼƽ���colorkey��Чʱ��Դƽ��������ɫΪcolorkey�����ؾ����ᱻblit��Ŀ��ƽ�档
*/
CSUDI_Error_Code CSUDIOSGBlit(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect,
                        CSUDI_HANDLE hSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ���Ŀ���ͼƽ������Դ�ռ���һ����Ρ�

@param[in] hDstSurface ��ͼƽ������Դ�ռ���.CSUDIOSGCreateSurface����CSUDIOSGGetDisplaySurface�ķ���ֵ��
@param[in] psDstRect Ҫ�����Ŀ���ͼƽ���ϵ�λ�úʹ�С��Ϊ��ʱ����ʾȫ����䡣
@param[in] uPixel Ҫ������ɫֵ��ARGB8888��ʽ������ֵ��
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ��
@note 
- Ϊ�����߻�ͼ���ܣ��ýӿ���Ҫʵ�ֳ��첽ģʽ��ͨ��CSUDIOSGSyncȷ���ò���ִ����ϣ���CSUDIOSGSync����ǰ�ı����Surface����ͼЧ���ǲ�ȷ���ģ�����������������������ɻָ����쳣��\n
- ����ʹ��Ӳ������Fill,���Ӳ����֧�֣��뷵��CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,�������и���Ч��ʵ�ַ�ʽ����������ֵ�����,����Ҫ�Ըò����������ģ�⡣\n
- psDstRect ������hDstSurface�Ĵ�С��Χ�ڣ��������hDstSurface���򷵻�CSUDIOSG_ERROR_BAD_PARAMETER��
- ��hDstSurface����ARGB8888��ʽ����������ֲ�����ת��������
*/
CSUDI_Error_Code CSUDIOSGFill(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int uPixel)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ͬ��CSUDIOSGBlit��CSUDIOSGFill��OSG������

@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ��
@note 
- ��CSUDIOSGBlit��CSUDIOSGFill��֮�����,��������ʱ��ǰ������ȫ����ɡ�\n
- �ýӿڲ�������ʾSurface�ϵ����ݡ� \n
- ���CSUDIOSGBlit��CSUDIOSGFill��ͬ���Ĳ���������ýӿڷſգ�ֱ�ӷ��سɹ��� \n
- �м�������������Surface���ڴ�ʱ��Ҳ���ô˽ӿ�ͬ��Surface���ڴ棬Ϊ�˱�֤�����ɹ�����ֹ���ݴ���cache״̬�������оƬ
- �޴�cache���⣬����ſգ�ֱ�ӷ��سɹ���
- �м���ڻ�ͼ�����л�Ƶ��ʹ�ô˽ӿڡ�
- �ýӿ���Blit�Ĺ�ϵ�ɲο�C99��׼fflush��
*/
CSUDI_Error_Code CSUDIOSGSync(void)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief Ӳ������ͼƬ,����I֡��ʽ(MPEG,BMP)/JPEG��ʽ/PNG��ʽ�ȡ�

@param[in] pszImageType ͼƬ������,ʹ���ַ��������˶�����ö�����͡���ʽ�����͵Ķ���: IFRAME - I֡��ʽ��BMP - bmp��ʽ, JPEG - jpeg��ʽ, PNG - png��ʽ
@param[in] pvImageMem  ͼƬ���ݵ�ַ��
@param[in] nMemSize   ͼƬ���ݴ�С��
@param[in] eScalingMode �������ģʽ�����ƽ��������
@param[in] psDstRect �������Surface�Ĵ�С�������������Ч������eScalingMode�Ĳ�ͬ��ֵ��Ч��������ͬ
@param[out] phSurface ���������ݴ�ŵ�surface��
@return �ɹ����� CSUDI_SUCCESS,���򷵻ش�����롣
@note 
- ����ʵ�ֶ�MPEG��ʽ��Ӳ�����룬������ʽ����֧�֣���ʱ����CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED��
- ���ӿڽ�����һ����ͼƽ�棬ͨ��phSurface���ظ��ϲ�ʹ�ã���ʹ����֮�󣬱������CSUDIOSGDestroySurface�ͷš�
- ������ӿڽ���ͼƬ(����I֡)�õ����豸����æ״̬���뷵��CSUDIOSG_ERROR_DEVICE_BUSY��
- ����I֡һ����Ҫ�õ�Video��������������Ƶ���ų�ͻ�����ӿ�Ӧ����ʹ������Video���������н��룬��ʵ���Ҳ������ý������򷵻�CSUDIOSG_ERROR_DEVICE_BUSY��
�������EastWin�����Ҫ���н��룬��Ӧ֪ͨ�ϲ���ͣ��Ƶ���ź��ٴε��øýӿڡ�
- ���ӿڽ������ڿ���ͨ��Ӳ����ʽ���ٽ��������������зǳ���ֵ����ɣ������벻Ҫ�����κ����ģ��ʵ�֡�
@note eScalingMode��psDstRect�Ĺ�ϵ
- EM_UDIOSG_SCALE_FULLSIZE��psDstRect����Ч������ΪCSUDI_NULL�������Surface�����ԭʼͼƬ����һ��
- EM_UDIOSG_SCALE_OPTIMUM�����ʺ����ţ�ͼƬ�ڱ��������ͱ�����ǰ���£�������ķ�����psDstRect�����У��������ߡ�\n
����������һ��ʱ���Surface�Ĵ�С����ĳһ�����򣨿��ߣ���psDstRectС��\n
�磺ͼƬԭʼ��СΪ1000*500������������Ϊ100*100���������Surface��СΪ100*50��ͼƬ������ռ������Surface��
- EM_UDIOSG_SCALE_FIXSIZE��������psDstRect.m_nWidth��psDstRect.m_nHeight����Ч�������Surface�߶�ΪpsDstRect.m_nHeight�����ΪpsDstRect.m_nWidth
*/
CSUDI_Error_Code CSUDIOSGDecodeImage(const char * pszImageType, const void * pvImageMem, int nMemSize,CSUDIOSGScalingMode_E eScalingMode,const CSUDIOSGRect_S * psDstRect,CSUDI_HANDLE * phSurface)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ3D��ʾ�����NativeDisplay���
@param[out] phNativeDisplay NativeDisplay�����
@return ���ش������͡�
@note 
-NativeDisplay��������ƽ̨��opengl es��ʵ����ء�
-������win32��hdc
*/
CSUDI_Error_Code  CSUDIOSGGetNativeDisplay(CSUDI_HANDLE * phNativeDisplay)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡhSurface��Ӧ��NativeWindow���
@param[in]  hSurface Surface���
@param[out] phNativeWindow NativeWindow���
@return ���ش������͡�
@note 
-NativeWindow��������ƽ̨��opengl es��ʵ����ء�
-������win32��window
*/
CSUDI_Error_Code CSUDIOSGGetNativeWindow (CSUDI_HANDLE hSurface, CSUDI_HANDLE *phNativeWindow)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

