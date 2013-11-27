/**@defgroup TYPEDEF TYPEDEF ����UDI��������EastWinϵ������еĻ�����������

@brief Typedefģ���Ӧͷ�ļ�<udi2_typedef.h>����Ҫ����UDI��������Eastwinϵ�������
�Ļ����������ͣ���CSUDI_BOOL��CSUDI_HANDLE�ȣ�û�нӿ���Ҫʵ�֡��ֶԶ����32λ��
���ͱ�����64λ�����ͱ����Ķ���������˵����

- ��ģ����Ȼ�ж���CSUDI_INT32��CSUDI_UINT32��32λ�����ͱ�������������Ҫ��ȷ����
�����ĳ��ȣ���λ���У�����Eastwin�ᾡ��ʹ��C����ԭ�������ݽṹ����������Ķ��塣\n
- ���ƽ̨��֧��ԭ��64λ������������ƽ̨�Ѿ�Խ��Խ���ˣ������߶�64λ���������㲻
����C99�淶������Ҫ����MMCP_SUPPORT_LL�꣬���������UDI��ͨ��һ��Structģ
��64λ����������Eastwin�ڲ��ṩ��ģ���64λ��ѧ���㷽������Ҫ�ر�ע����ǣ�����
UDI�ӿ����Ѱ�����CSUDI_UINT64���͵Ĳ�������ʵ����Щ�ӿ�ʱ������Ҫ�漰64λ������
���㣬����ֲ������ʵ�֡�\n

@note ��ģ�������ṹ��û�нӿ���Ҫʵ�֡�
@note �����ر�˵��������UDI�ӿڱ��뱣֤�̰߳�ȫ��

@{
*/

//������������UDI�������������汾��Ϣ�������漰UDI�ĸĶ������������¼

/**@mainpage
@version <B>UDI_V2.2.13</B> 2013/05/22
-����udi2_smcģ�������ö�����ͣ���������ܿ���֧��ĳ����ʱ��û��׼ȷ�����뷵�ص�����

@version <B>UDI_V2.2.12</B> 2013/03/21
-����CSUDISYSSetStandbyAttribute �ӿڣ��������ô�������

@version <B>UDI_V2.2.11</B> 2013/03/20
- �޸�CSUDIWLanApInfo_S �ṹ��ע�ͣ���ȷ��nSignalStrength ��nLinkQuality ��Ա��ȡֵ��Χ

@version <B>UDI_V2.2.10</B> 2013/02/23
- ����CSUDIOTAWIFIStart�ӿڣ�����wifi����������
- ����EM_UDIOTA_WIFI_ALL_FINISH��Ϣ�����ڻ�ȡwifi����״̬

@version <B>UDI_V2.2.9</B> 2013/01/23
- ����CSUDIRECPause ��CSUDIRECResume�ӿڣ�����¼�ƹ���

@version <B>UDI_V2.2.8</B> 2013/01/09
- �޸�CSUDIOTAWIFICakllback_F���������ӷ��ؾ�����Ϣ���͵Ĳ���
- �޸�CSUDIEtherCallback_F���������ӷ��������������ֵĲ���
- �޸�CSUDIIPCFGGetAPSignalQulityStrengthע�ͣ�����������˽ӿ�
- ����CSUDIOTASetInfo�����url����

@version <B>UDI_V2.2.7</B> 2012/11/23
- ���ӿɰβ�������Ϣ�����ӿ�

@version <B>UDI_V2.2.6</B> 2012/10/15
- ���ӻ�ȡ�����ļ����ԵĽӿ�
- �������ò����ļ����ԵĽӿ�

@version <B>UDI_V2.2.5</B> 2012/08/13
- ����WIFI������ؽӿ�

@version <B>UDI_V2.2.4</B> 2012/07/09
- CSUDIFilePlayerEventType_E��������ʾ��Ļ����¼�����

@version <B>UDI_V2.2.3</B> 2012/06/05
- �����ļ�����ר�õĻص��͸����¼�����

@version <B>UDI_V2.2.2</B> 2012/05/05
- ���ӻ�ȡ�����ô������ԵĽӿ�

@version <B>UDI_V2.2.1</B> 2012/04/18
- CSUDIPLAYERType_E����EM_UDIPLAYER_PVR(PVR¼�Ʋ���ģʽ)

@version <B>UDI_V2.2.0</B> 
- ����OS,HDMI,USB�ӿ�
- ���ӿ���boot��ʾ��ؽӿ�
- ��������������ؽӿ�
- ����AP������ؽӿ�

@version <B>UDI_V2.1.3</B> 2010/11/11
- ����PPPOE������ؽӿ�

@version <B>UDI_V2.1.2</B> 
- ����BooterLoader��ؽӿڣ����ֽӿڱ���עΪBooterר�á�
- ����NDS OTA����״̬ѡ�����NDS����״̬��OTA��
- ����EW200Service������������Ϣѡ�ȥ��VOD�汾��ģʽѡ�
- ����Injectģ��PCMע���С��Ĭ��ֵ˵��
- ����IPv6��ؽӿ�
- �������������������ϲ�ʹ��ָ���ĵ�
- ϸ��Player��Inject�豸ѡȡ�����һЩԭ��˵��
- ϸ������ʱPID������ص�˵��
- �����κ�ʱ��Ҫ�ܹ���ȡ�ֱ��ʵ�˵��
- �޸�help���ļ�ֵ
- ����������֤��IPQAM�Ƚ���ѡ��
- ����I֡ע��ר�ýӿ�
- ���Ӽٴ����ӿڣ�ԭ�нӿڽ����������

@version <B>UDI_V2.1.1</B> 2009/09/26
- ���Ӹ���ϸ��ģ������˵����Ҫ��˵��
- ���������ӿ���������ʹ֮���������������
- ȥ��ö�����һ����Ա�Ķ��ţ��Խ������ƽ̨�޷�����ͨ��������
- ���Tuner,Demux����״̬��ѯ���ܣ����Tuner��Demux�������ӵĴ������˵��
- �޸Ļ�ȡTuner��Ϣ�ӿڣ�ʹ�����ڿ��Ե�����ȡ������Ϣ�����Ч��
- ����һЩ�ĵ����������ԵĴ���

@version <B>UDI_V2.1.0</B> 2009/09/09
- ���ӿ��ƶ��豸��֧�֣�����Tuner,�洢�豸;ͳһ�˿��ƶ��洢�豸�Ľӿ�

@version <B>UDI_V2.0.3</B> 2009/08/28
- ������ͼƬӲ����ӿڣ�������demux����tuner�Ľӿ�

@version <B>UDI_V2.0.2</B> 2009/08/25
- ������һ���ۺ�����

@version <B>UDI_V2.0.1</B> 2009/08/16
- ��ʼ�汾
*/

#ifndef _UDI2_TYPEDEF_H_
#define _UDI2_TYPEDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********���������ھ������ƽ̨��,����Ҫ�Ķ�*********************/

typedef int                 CSUDI_BOOL;           ///< ��������
typedef void *			    CSUDI_HANDLE;         ///< �������,�䳤�ȵ���ָ�볤�ȡ�@note Eastwin�ὫCSUDI_NULL����һ���Ƿ��ľ��������ʵ��ʱ�ر�ע������Handle��ȡֵ��Χ

//������Ҫ��ȷ���Ƴ���,���򲻽���ʹ�����¶���
typedef signed char	        CSUDI_INT8;             ///< 8λ�з�����
typedef unsigned char	    CSUDI_UINT8;            ///< 8λ�޷�����

typedef signed short	    CSUDI_INT16;            ///< 16λ�з�����
typedef unsigned short	    CSUDI_UINT16;           ///< 16λ�޷�����

typedef signed long	        CSUDI_INT32;            ///< 32λ�з�����
typedef unsigned long	    CSUDI_UINT32;           ///< 32λ�޷�����,ע���ֹʹ��CSUDI_UINT32�洢ָ��

//gcc,VC�ṩ��ͷ�ļ������˷�ʽ����
#ifdef __cplusplus
#define CSUDI_NULL    		0
#else
#define CSUDI_NULL    		((void *)0)
#endif

#define CSUDI_FALSE         (0 == 1)
#define CSUDI_TRUE          (!(CSUDI_FALSE))


/***********�������ݿ��ܻ����ƽ̨�Ĳ�ͬ,�����ı�,�������½ṹ��Ϊ���ݲ��ֵ������������,�뾡����Ҫʹ��*********************/


/**@brief ��ƽ̨֧��64λ��,����붨����������,����ɲ���ע

@note �ú궨�������ƽ̨�����Ƿ�֧�֣������ж���
*/

//#define MMCP_SUPPORT_LL

#ifdef MMCP_SUPPORT_LL

typedef long long           CSUDI_INT64;            ///< 64λ�з�����,������MMCP_SUPPORT_LLʱ��Ч
typedef unsigned long long  CSUDI_UINT64;          ///< 64λ�޷�����,������MMCP_SUPPORT_LLʱ��Ч

#else

/**@brief 64λ�з������ṹ��*/
typedef	struct
{
	CSUDI_UINT32	low; ///< ��32λ
	CSUDI_INT32	    high;///< ��32λ
}CSUDI_INT64;

/**@brief 64λ�޷������ṹ��*/
typedef	struct
{
	CSUDI_UINT32	low;///< ��32λ
	CSUDI_UINT32	high;///< ��32λ
}CSUDI_UINT64;

#endif

#ifdef __cplusplus
}
#endif
/** @} */
#endif  //_UDI2_TYPEDEF_H_

