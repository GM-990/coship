/**@defgroup OSG OSG ����ƽ̨��ͼ�ӿ�,���׽ӿڵ����ܾ������м��ͼ��ˢ�µ����ܣ�����ʵ��ʱ���Ӷ��̱߳���
���Ǹ�ƽ̨���̲߳���ʱ��ͼ����Դ�г�ͻ���²�����ʾ�żӶ��̱߳���

@brief OSGģ���Ӧͷ�ļ�<udi2_osg.h>�������˲���OSG�豸��һϵ�нӿڡ�ģ������˼
�����ڻ���Framebuffer�Ļ����ϣ���������Ӳ�����ٵ����ԡ��ڱ�ģ���У�����ϵ������
��ʽΪX������������죬Y������������졣

@par �ڴ�Surface
ģ���е�Surface���������Ϊһ��Framebuffer������ʱ����ָ�����С����ɫģʽ�ȡ���
���ʾ���һ���ڴ棬�����ݾ���ƽ̨�Ĳ�ͬ���������һ������ģ���������Ӳ�����ٵ���
�棻Ҳ���ܾ���һ����ͨ���ڴ档Eastwin����ͼ��Ĳ��������ǰ�����д��Surface�Ĺ��̡�\n

@par �Դ�Surface
������ͨ��Surface�����м��������Surface�����Դ棬ͨ��CSUDIOSGGetDisplaySurface��
�ڿ��Ի�ȡ����ЩSurface����������ݿ������Դ��ϣ���ͼ���������ʾ����Ļ�ϡ����ƽ
̨��֧�������Ŀ���������Ҳ�ɰ��Դ�Surface��m_pvSurfaceMem�ֶ���ΪCSUDI_NULL������
�����Eastwin����ͨ��Blit�ӿڽ�ͼ����ʾ������\n

@note ʵ���ϣ�ֻҪƽ̨�ܹ�����Surface��Eastwin����ͨ��Blit�ӿ�����ʾͼ��\n

@par Blit�ӿ�
Blit�Ǳ�ģ��������Ҫ��һ���ӿڣ���ͨ��Ӳ�����ٵķ�ʽ����ʾ������Surface֮���ƶ���
�������ݵ�Surface�Լ���������֮�䣬���ܴ��ڸ��ָ����Ĳ��죬��Щ������Բ����ܶ���
���Ч��������ɫģʽת����ͼ�����š�Ť���ȡ����Ӳ����֧����Щ���ԣ����Է���
CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED����һ����˵����ƽ̨����֧�ֲ���Ӳ�����ٷ�ʽʵ
�ֵ�����Ч������Ϊ����ƽ̨����Ҫ��ͼ����ʾ�������ֱ��ʲ�ͬ����Ļ�ϣ������ô������
��ʵ�ֵ�����Ч�ʼ��ͣ�Eastwin����������������ĳ��ԡ�
Ϊ�˳����߻�ͼ���ܣ��ýӿ�������첽ģʽʵ�֡���Blit�ӿڷ��غ�Blit������һ����
����ɣ������������õĶ��Blit���������˺ϲ��Ŀ��ܡ�CSUDIOSGSync�ᴦ�����Ҫͬ����
��������������е�Blit������������CSUDIOSGSync�ӿڷ��غ���ɡ�����ζ����Blit֮��
CSUDIOSGSync����ǰ�ı����Surface�����ݣ���ͼЧ���ǲ�ȷ���ģ�����ʹ����Ҳ����������
�����������ɻָ����쳣�������첽ģʽ�������Ż�ʱҪ�ر�ע�⣬���պϲ����BlitЧ����Ӧ
����ͬ����BlitЧ������һ�£�Ӧ�ر�ע����Blit֮��ĸ��ǹ�ϵ�������û������Ĳü���
��Blit��ִ��˳���ǲ��ܷ����ı�ġ�\n

@par ͼƬӲ����ӿ�
OSGģ�黹����һ����ͼƬӲ���뵽һ����Surface�ϵĽӿڡ������������DVBƽ̨���Ǿ���Ӳ
������I֡�������ģ�Ҳ����˵�����������£�����ӿ����ٻ���һ�����ܣ�������I֡����
Ҫ�ر�ע����ǽ���I֡һ����Ҫ�õ�Video����������û�ж���I֡������������£������ƽ
̨û�У���ͻ�����Ƶ���ų�ͻ�����ӿ�Ӧ����ʹ������Video���������н��루����ж��
Video�������Ļ�������ʵ���Ҳ������ý������򷵻�CSUDIOSG_ERROR_DEVICE_BUSY�������ж�
��Ƶ���ţ����������Eastwin�����Ҫ���н��룬��֪ͨ�ϲ���ͣ��Ƶ���ź��ٴε��øýӿڡ�

@note ����һ��Ӳ����Ľӿڣ������зǳ���ֵ����ɣ�����Ҫ���׳����������ķ�ʽʵ�ָý�
�ڣ�Eastwin���Ѿ�������������ģ�飬���ȷʵ������뷽ʽʵ�ָýӿڣ��򷵻�
CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED���ɡ�

@note Booter��Loader�汾�У���ģ�����нӿڵ�ʵ�־��ɴӼ򣬶��ڱ���ƽ̨��ͱ�׼ֻ��Ҫʵ��
��ȡ�Դ�ӿڼ��ɣ�������汾ֻ��Ҫʵ���������Blitģʽ���ɡ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.6 2009/9/07 ����colorkeyflag ��alphaflag
@version 2.0.5 2009/8/28 �Ѿ�������ṹ������public����ͷ�ļ���
@version 2.0.4 2009/8/27 ����ͼƬӲ��
@version 2.0.3 2009/8/26 �޸�colorKey����Ϊ�޷�����
@version 2.0.2 2009/8/24 ��һ���ۺ�������޸ĵİ汾
@version 2.0.1 2009/07/08 ���������۹��İ汾
@{
*/

#ifndef _UDI2_OSG_H_
#define _UDI2_OSG_H_


#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief ���ش������ö�ٶ��塣*/
enum {
	CSUDIOSG_ERROR_BAD_PARAMETER = CSUDI_OSG_ERROR_BASE,                 ///< �����쳣���� 
    CSUDIOSG_ERROR_FEATURE_NOT_SUPPORTED,                                ///< ��֧�ִ˲��� 
    CSUDIOSG_ERROR_UNKNOWN_ERROR,                                        ///< δ֪����
    CSUDIOSG_ERROR_NO_MEMORY,                                             ///< �ڴ治����� 
    CSUDIOSG_ERROR_DEVICE_BUSY                                         ///< �豸æ
};

/**@brief ��ɫģʽö�ٶ���

�����峣����ɫģʽ��ֻ��ѡ����Ҫ��ģʽ(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555 
EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)֧�ּ��ɣ�����Ҫȫ��֧�֡�
*/
typedef enum {
    EM_UDIOSG_PIXEL_FORMAT_RGB_565,     ///< 16-bit, no per-pixel alpha 
    EM_UDIOSG_PIXEL_FORMAT_ARGB_1555,   ///<  16-bit 
    EM_UDIOSG_PIXEL_FORMAT_RGBA_5551,   ///<  16-bit 
    EM_UDIOSG_PIXEL_FORMAT_ARGB_4444,   ///<  16-bit 
    EM_UDIOSG_PIXEL_FORMAT_RGBA_4444,   ///<  16-bit 

    EM_UDIOSG_PIXEL_FORMAT_RGB_888,     ///<  24-bit 
    EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,   ///<  32-bit 
    EM_UDIOSG_PIXEL_FORMAT_PALETTE8,    ///<  8-bit Palette index��Ŀǰû�е�ɫ���Surface����ɫ��ĽṹԤ����

    EM_UDIOSG_PIXEL_FORMAT_YCBCR888_444, ///< Y, Cb, Cr signed components with 8 bit precision in a 4:4:4 sampling model
    EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422, ///< Y, Cb, Cr signed components with 8 bit precision in a 4:2:2 sampling model
	EM_UDIOSG_PIXEL_FORMAT_YCBCR888_420, ///< Y, Cb, Cr signed components with 8 bit precision in a 4:2:0 sampling model
    EM_UDIOSG_PIXEL_FORMAT_YCBCR888_411, ///< Y, Cb, Cr signed components with 8 bit precision in a 4:1:1 sampling model
    EM_UDIOSG_PIXEL_FORMAT_NUM            ///< ���ظ�ʽ������
} CSUDIOSGPixelFormat_E;


/**@brief ���ģʽö�ٶ��� */
typedef enum {
    EM_UDIOSG_OPMODE_COPY,               ///<  src 
    EM_UDIOSG_OPMODE_XOR,                ///<  src ^ dst 
    EM_UDIOSG_OPMODE_OR,                 ///<  src | dst 
    EM_UDIOSG_OPMODE_AND,                ///<  src & dst 
    EM_UDIOSG_OPMODE_CLEAR,             ///<  0 
    EM_UDIOSG_OPMODE_SET,                ///<  11111111 
    EM_UDIOSG_OPMODE_EQUIV,             ///<  ~(src ^ dst) 
    EM_UDIOSG_OPMODE_NOR,	               ///<  ~(src | dst) 
    EM_UDIOSG_OPMODE_NAND,               ///<  ~(src & dst) 
    EM_UDIOSG_OPMODE_INVERT,            ///<  ~dst 
    EM_UDIOSG_OPMODE_COPYINVERTED,      ///<  ~src 
    EM_UDIOSG_OPMODE_ORINVERTED,        ///<  ~src | dst 
    EM_UDIOSG_OPMODE_ANDINVERTED,          ///<  ~src & dst 
    EM_UDIOSG_OPMODE_ORREVERSE,            ///<  src | ~dst 
    EM_UDIOSG_OPMODE_ANDREVERSE,           ///<  src & ~dst 
    EM_UDIOSG_OPMODE_NOOP,                 ///<  dst 
    EM_UDIOSG_OPMODE_ALPHA                ///<  Alpha mode 
} CSUDIOSGOpMode_E;


/**@brief ��ͼƽ��ṹ���塣 */
typedef struct {
	CSUDIOSGPixelFormat_E m_ePixelFormat;   ///<  �û�ͼƽ��֧����ɫģʽ 
	int m_nWidth;                           ///<  �û�ͼƽ��Ŀ��  > 0
	int m_nHeight;                          ///<  �û�ͼƽ��ĸ߶� > 0

	int m_nPitch;                            ///<  �û�ͼƽ��ÿ���ֽ���,width*BytesPerPixel 
	int m_nColorKeyFlag;                    ///<  �ñ�־Ϊ 1 ʱ��ʾm_uColorKey �����ã���ʼ��Ϊ 0
	int m_nAlphaFlag;                       ///<  �ñ�־Ϊ 1 ʱ��ʾ��ƽ���ȫ��m_uAlpha�����ã���ʼ��Ϊ 0
    unsigned int m_uColorkey;                        ///<  �û�ͼƽ���colorKey ,AGRB888ģʽ������ֵ��ָ������ɫ���ڸû�ͼƽ�治����ʾ Ĭ��0x00000000
    unsigned int m_uAlpha;                            ///<  �û�ͼƽ���͸���ȣ�[0x00,0xFF],0x00��ʾ��ȫ͸��;0xFF��ʾ��͸����Ĭ��Ϊ0xFF
    void * m_pvSurfaceMem;                    ///<  �û�ͼƽ����ڴ��ַ ,�����Surface����ֱ�ӻ��ƣ���������Դ�Surface��,�븳CSUDI_NULL
    int m_nMemSize;                          ///<  �û�ͼƽ����ڴ��С 
    void *m_pvPalette;                       ///<  m_ePixelFormat ΪEM_UDIOSG_PIXEL_FORMAT_PALETTE8ʱָ���ĵ�ɫ����������Ŀǰû�е�ɫ���Surface,��ɫ��ĽṹԤ��������void *
} CSUDIOSGSurfaceInfo_S;

/**@brief ��������ṹ���� */
typedef struct {
    int m_nX;                ///< �����������ϽǺ����� 
    int m_nY;                ///< �����������Ͻ������� 
	int m_nWidth;            ///< ���������� > 0
	int m_nHeight;           ///< ��������߶� > 0
} CSUDIOSGRect_S;

/**@brief ͼƬ����ģʽö�ٶ��� */
typedef enum {
    EM_UDIOSG_SCALE_FULLSIZE,   ///< �����ţ���ԭʼ��С���
    EM_UDIOSG_SCALE_OPTIMUM,   	///< ����ƥ�䣬��ģʽ��ͼƬ����ԭʼ���������������ռ�����������������
    EM_UDIOSG_SCALE_FIXSIZE    ///< �����������ţ��������߾�Ϊָ��ֵ�������������ʵ�����ݲ�һ�£����������
} CSUDIOSGScalingMode_E;

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
CSUDI_Error_Code CSUDIOSGCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat, int nWidth, int nHeight, CSUDI_HANDLE * phSurface);


/**
@brief ����һ����ͼƽ�棬�ͷŸû�ͼƽ����ڴ档

@param[in] hSurface CSUDIOSGCreateSurface �ɹ����صľ����
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ 
@note 
- ��CSUDIOSGCreateSurface�����Ļ�������ʹ�ú�һ��Ҫ���ô˽ӿ������ͷš�\n
- ��������һ���Դ�ռ䣬������CSUDIOSG_ERROR_BAD_PARAMETER��
*/
CSUDI_Error_Code CSUDIOSGDestroySurface(CSUDI_HANDLE hSurface);


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
CSUDI_Error_Code CSUDIOSGGetDisplaySurface(int nIndex, CSUDI_HANDLE * phSurface);

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
CSUDI_Error_Code CSUDIOSGGetSurfaceInfo(CSUDI_HANDLE hSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo);

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
CSUDI_Error_Code CSUDIOSGSetSurfaceInfo(CSUDI_HANDLE hSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo);


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
                        CSUDI_HANDLE hSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode);


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
CSUDI_Error_Code CSUDIOSGFill(CSUDI_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int uPixel);

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
CSUDI_Error_Code CSUDIOSGSync(void);

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
CSUDI_Error_Code CSUDIOSGDecodeImage(const char * pszImageType, const void * pvImageMem, int nMemSize,CSUDIOSGScalingMode_E eScalingMode,const CSUDIOSGRect_S * psDstRect,CSUDI_HANDLE * phSurface);

/**
@brief ��ȡ3D��ʾ�����NativeDisplay���
@param[out] phNativeDisplay NativeDisplay�����
@return ���ش������͡�
@note 
-NativeDisplay��������ƽ̨��opengl es��ʵ����ء�
-������win32��hdc
*/
CSUDI_Error_Code  CSUDIOSGGetNativeDisplay(CSUDI_HANDLE * phNativeDisplay);

/**
@brief ��ȡhSurface��Ӧ��NativeWindow���
@param[in]  hSurface Surface���
@param[out] phNativeWindow NativeWindow���
@return ���ش������͡�
@note 
-NativeWindow��������ƽ̨��opengl es��ʵ����ء�
-������win32��window
*/
CSUDI_Error_Code CSUDIOSGGetNativeWindow (CSUDI_HANDLE hSurface, CSUDI_HANDLE *phNativeWindow);

#ifdef __cplusplus
}
#endif

/** @} */

#endif


