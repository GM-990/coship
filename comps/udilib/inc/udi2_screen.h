/**@defgroup SCREEN  SCREEN ������Ƶ���ź�����ؽӿ�
@brief SCREENģ���Ӧͷ�ļ�<udi2_screen.h>��������Ƶ���ź�����ؽӿڡ�

Screen�ĸ����Ƕ��Video�������������ս���Ƶ��ϵ����ɸ���Ŀǰ��SD/HD������Screen�����������OSD����Screen�ٽ��������������VOUT������ӿ���HDMI������ȣ��ϡ������ԣ��������������ͱ����ϵ����ݣ���ʹ���ǲ��������ݵı仯Ҳ����ϸ΢����ģ�����ֱ��ʵȵȡ�Screen���Ƕ���Щ���ݽ��г��󣬲�������ƽӿ� ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.3 2009/09/24 ���ӻ�ȡScreen�����ӿ�˵��
@version 2.0.2 2009/08/25 ����Screen�����˵��
@version 2.0.1 2009/08/16 ����
@{
*/
#ifndef _UDI2_SCREEN_H_
#define _UDI2_SCREEN_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif
 

/**@brief ��Ƶ������ش������ֵ*/
enum
{
	CSUDISCREEN_ERROR_BAD_PARAMETER = CSUDI_SCREEN_ERROR_BASE,  ///< �����������
	CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED,      				 ///< ��֧�ִ˲���
	CSUDISCREEN_ERROR_ALREADY_ADDED							///< �ûص��Ѿ�ע��
};

/**@brief ��Ƶ���ͨ������*/
typedef enum
{
	EM_UDI_VOUT_DEVICE_SD	= 1,			///<�������ͨ��
	EM_UDI_VOUT_DEVICE_HD	= 1 << 1		///<�������ͨ��
}CSUDISCREENType_E;

/**@brief ��Ƶ����ֱ���*/
typedef enum 
{
	EM_UDISCREEN_RESOLUTION_INVALID,
	EM_UDISCREEN_RESOLUTION_NTSC,  			///<  SD: EM_UDICTL_MIX_ENCODING_MODE_NTSCM 
	EM_UDISCREEN_RESOLUTION_NTSC_443,		///<  NTSC M mode but with 4.43361875MHz color subcarrier frequency. 
	EM_UDISCREEN_RESOLUTION_NTSC_JAPAN,	///<  Japan NTSC, no pedestal level 
	EM_UDISCREEN_RESOLUTION_PAL, 		     	///<  SD: EM_UDICTL_MIX_ENCODING_MODE_PALBDGHI 
	EM_UDISCREEN_RESOLUTION_PAL_M,        	///<  PAL Brazil 
	EM_UDISCREEN_RESOLUTION_PAL_N,        	///< PAL_N 
	EM_UDISCREEN_RESOLUTION_PAL_NC,      	///<  PAL_N, Argentina (PAL N mode but with 3.58205625 MHz color subcarrier frequency and no pedestal (= 0 IRE blanking setup)) 
	EM_UDISCREEN_RESOLUTION_PAL_B,			///< Australia 
	EM_UDISCREEN_RESOLUTION_PAL_B1,		///< Hungary 
	EM_UDISCREEN_RESOLUTION_PAL_D,			///<  China ==EM_UDISCREEN_RESOLUTION_PAL 
	EM_UDISCREEN_RESOLUTION_PAL_D1,		///<  Poland 
	EM_UDISCREEN_RESOLUTION_PAL_G,			///< Europe. Same as EM_UDISCREEN_RESOLUTION_PAL. 
	EM_UDISCREEN_RESOLUTION_PAL_H,			///<  Europe 
	EM_UDISCREEN_RESOLUTION_PAL_K,	      		///< Europe 
	EM_UDISCREEN_RESOLUTION_PAL_I,			///<  U.K. 
	EM_UDISCREEN_RESOLUTION_SECAM,      		///< SECAM III B6 
	
	EM_UDISCREEN_RESOLUTION_480P,        ///< NTSC Progressive (27Mhz) 	
	EM_UDISCREEN_RESOLUTION_576P, 		///<  Australian mode 	
	EM_UDISCREEN_RESOLUTION_720P,  		///< HD: EIA770.3 #1 P60 = SMPTE 296M #1 P60 
	EM_UDISCREEN_RESOLUTION_720P_24HZ,    ///<  HD 720p 24Hz 	
	EM_UDISCREEN_RESOLUTION_720P_50HZ,	///< HD 720p 50Hz
	
	EM_UDISCREEN_RESOLUTION_1080I,  	   ///< HD: EIA770.3 #3 I60 = SMPTE274M #4 I60 		
	EM_UDISCREEN_RESOLUTION_1080I_50HZ, ///<  HD:  SMPTE 274M   I50 
	
	EM_UDISCREEN_RESOLUTION_1080P,     ///<  HD 1080 Progressive, 60Hz 
	EM_UDISCREEN_RESOLUTION_1080P_24HZ,///<  HD 1080 Progressive, 24Hz 
	EM_UDISCREEN_RESOLUTION_1080P_25HZ,	///<  HD 1080 Progressive, 25Hz 
	EM_UDISCREEN_RESOLUTION_1080P_30HZ,	///<  HD 1080 Progressive, 30Hz 
	EM_UDISCREEN_RESOLUTION_1080P_50HZ, ///<  HD 1080 Progressive, 50Hz 	
	EM_UDISCREEN_RESOLUTION_1250I_50HZ,   ///<  HD 1250 Interlaced, 50Hz 
	
	EM_UDISCREEN_RESOLUTION_VESA,			///<  PC monitor. Requires width, height and refresh rate parameters 
	EM_UDISCREEN_RESOLUTION_NUM
} CSUDISCREENResolution_E;

/**@brief ��Ļ�¼�����*/
typedef enum
{
    EM_UDISCREEN_RESOLUTION_CHANGED              ///<  ��Ļ�ֱ��ʸı��¼�
}CSUDISCREENEvent_E;

/**@brief ƽ̨����豸֧�ֵķֱ�������������Ϣ
@note �����У������������ΪEM_UDISCREEN_RESOLUTION_INVALID�����ʾ֧�ֵķֱ��������Ѿ��������������ݽ������ԡ�
*/
typedef struct
{
	  CSUDISCREENResolution_E   m_eResolution[EM_UDISCREEN_RESOLUTION_NUM];	   ///< �豸֧�ֵ���Ƶ�ֱ��ʸ�ʽ
} CSUDISCREENCapability_S;

/**
@brief ��ȡָ������豸�ķֱ�����������
@param[in] eScreenDevice ��Ƶ���ͨ������
@param[out] psCapabilityInfo  ���ͨ���豸������Ϣ,������μ�CSUDISCREENCapability_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psCapabilityInfo�ṹ������δ����
@note ��ƽ̨��֧�ָ����ͨ��(�粻֧�ָ���)�ýӿڷ���CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
@note �˽ӿ�һ��ֻ������һ��Screen�豸�����ö���豸��򣬷��򷵻�CSUDISCREEN_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDISCREENGetCapability(CSUDISCREENType_E eScreenDevice,CSUDISCREENCapability_S * psCapabilityInfo);

/**
@brief ��Ļ�ֱ��ʸı�ص���������

@param[in] eEvt ��Ļ�¼����ͣ����CSUDISCREENEvent_E����
@param[in] eScreenDevice ��Ƶ����ӿڣ����CSUDISCREENType_E����
@param[in] pvUserData �û�����
*/
typedef void (*CSUDISCREENCallback_F)(CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice,void * pvUserData);

 /**
@brief  ע��ָ���¼����͵Ļص�����

@param[in] fnScreenCallback �ص�����ָ�룬���CSUDISCREENCallback_F����
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ��ͬCSUDISCREENCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note �ص���������С���ע�����Ϊ32��������32������ʧ�ܲ����ش������ֵ
@note fnScreenCallback��pvUserDataΨһȷ��һ��ע�ᣬ������ע��������ȫһ��ʱ���ڶ��ν�����CSUDISCREEN_ERROR_ALREADY_ADDED����ֻ��֪ͨһ�Σ����ע���������������ȫһ��ʱ��֪ͨ���
*/
CSUDI_Error_Code CSUDISCREENAddCallback(CSUDISCREENCallback_F fnScreenCallback,void * pvUserData );

 /**
@brief ɾ��ָ���¼����͵Ļص�����

@param[in] fnScreenCallback �ص�����ָ�룬���CSUDISCREENCallback_F����
@param[in] pvUserData ͬCSUDISCREENCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ�����ش������ֵ
@note fnScreenCallback��pvUserDataΨһȷ��һ��ע�ᣬ����������ע��ʱ��ȫһ�����ܳɹ�ɾ��
*/
CSUDI_Error_Code CSUDISCREENDelCallback(CSUDISCREENCallback_F fnScreenCallback, void * pvUserData );
 
/**
@brief ��������ֱ��ʼ�ˢ��Ƶ��

����ָ����Ƶ����ӿڵķֱ���
@param[in] peScreenDevice ��ָ����Ƶ����ӿڵ�����
@param[in] peResolution ϵͳ����ֱ�������, 720*480,720*576, 1920*1080 etc����μ�CSUDISCREENResolution_E
@param[in] nCount peScreenDevice��peResolution����Ĵ�С��Ҫ�������������һ����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note Eastwin���ò�ͬ����ӿڵķֱ�����֡������þ��м����ԡ���CVBS�����PAL�ƺ͸����1080i 50Hz����,ͬ1080i 60Hz�����ݡ���������UDIӦ��������
@note Eastwin�ᾡ��һ�������ö��Screen�ķֱ��ʣ��Է���UDI����кϲ�����
@note ��ͬʱ���ö����Ƶ�豸���ͣ����κ�һ����֧��ʱ���ýӿڷ���ʧ�ܣ���������֧�ֵ����ò���Ч
*/
CSUDI_Error_Code CSUDISCREENSetResolution(CSUDISCREENType_E* peScreenDevice, CSUDISCREENResolution_E* peResolution,int nCount);

/**
@brief ��ȡ��ǰ��Ƶ�ֱ���

@param[in] eScreenDevice ָ����Ƶ����ӿ�
@param[out] peResolution ��ǰ��Ļ�ķֱ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note �˽ӿ�һ��ֻ������һ��Screen�豸�����ö���豸��򣬷��򷵻�CSUDISCREEN_ERROR_BAD_PARAMETER
@note ��Ļ�ֱ�����Player�����ŵ��޹أ����۵�ǰ��ʲô״̬����Ҫ�ܻ�ȡ����Ϣ��
*/
CSUDI_Error_Code CSUDISCREENGetResolution(CSUDISCREENType_E eScreenDevice,CSUDISCREENResolution_E * peResolution);

/**
@brief ����Screen OSD͸����

@param[in] nTransparency ͸���ȣ�ȡֵ[0��100];
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����OSD���͸���ȣ���ΧΪ0-100������0Ϊ��ȫ͸����100Ϊ��͸��
*/
CSUDI_Error_Code  CSUDISCREENSetOSDTransparency(int nTransparency);

/**
@brief ������Ƶ���������

@param[in] nBrightness ��Ƶ��������ȣ�ȡֵ[0��100]
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ������Ƶ��������ȣ�0Ϊȫ�ڣ�100Ϊ������ߡ�
@note ���ȵ�Ĭ��ֵΪ50,����ֵ����Ϊ50ʱ,�������Ŀ���ֳ����Ч��
*/
CSUDI_Error_Code CSUDISCREENSetBrightness(int nBrightness);

/**
@brief ������Ƶ����ĶԱȶ�

@param[in] nContrast ��Ƶ����ĶԱȶȣ�ȡֵ[0��100]��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ������Ƶ����Աȶȣ�0Ϊȫ�ڣ�100Ϊ�Աȶ���ߡ�
@note �Աȶȵ�Ĭ��ֵΪ50,����ֵ����Ϊ50ʱ,�������Ŀ���ֳ����Ч��
*/
CSUDI_Error_Code CSUDISCREENSetContrast( int nContrast);

/**
@brief ������Ƶ����ı��Ͷ�

@param[in] nSaturation ��Ƶ����˿ڵı��Ͷȣ�ȡֵ[0��100]��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����ָ����ı��Ͷȣ�0Ϊȫ�ڣ�100Ϊ���Ͷ���ߡ�
@note ���Ͷȵ�Ĭ��ֵΪ50,����ֵ����Ϊ50ʱ,�������Ŀ���ֳ����Ч��
*/
CSUDI_Error_Code CSUDISCREENSetSaturation( int nSaturation);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

