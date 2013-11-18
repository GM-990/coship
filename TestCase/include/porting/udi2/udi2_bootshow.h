/**@defgroup booter��ʾ��Ϣ�ӿ�
Booter��ʾ���ͼƬ\n
	1. Ĭ�Ϲ��д�뵽index=0��λ�ã��ڳ���ǰ��¼�á�\n
	2. �м�� ��Ҫ����һ��booter��ʾ���ʱ:\n
		a ����CSUDIBOOTSHOWGetCapability ��ȡ����booter������(֧����ЩͼƬ�����ʽ?)���м����ͼƬת���booter��֧�ֵ�ͼƬ�����ʽ\n
		b ����CSUDIBOOTSHOWGetFreeSize ��ȡbooterʣ����ÿռ��С���ж��ռ��Ƿ��㹻:\n
			�ɴ洢ͼƬ��С = ʣ��ռ��С - �洢ÿ��BootͼƬ����洢��ͼƬ��Ϣͷ����С\n
		c ����CSUDIBOOTSHOWSetShowUnitInfo �ӿڱ���booter��ʾͼƬ��\n
 	3. �м����Ҫ�޸�ĳ���Ѵ洢��Ԫ�����ʾ����ʱ:\n
 		a ����CSUDIBOOTSHOWGetPicCount ��CSUDIBOOTSHOWGetShowUnitInfo�ӿڱ����Ѵ洢�Ĵ洢ͼƬ���������޸ĵ�nPicId��\n
 		b ����CSUDIBOOTSHOWSetNeedShow �ӿ��޸ĸô洢��Ԫ����ʾ����(��ʾor ����ʾ)

booter��֧�ֶ�����ʽ����Ϣ��ʾ���������֧����ʾ����ͼƬ������������Ƶ�ȵȡ����Լ�¼�����ʾ��Ԫ����Ϣ\n
��ʾ��Ԫ = ��ʾ��Ԫ��Ϣͷ + ��ʾ��Ԫ������
ÿ����ʾ��Ԫ��������Ϣͷ������ѡ��������������ͼƬ����Ƶ�ļ�����ʾ��������������������ֱ����Ƶ������Բ�Ҫ��ʾ��������\n
��ʾ���ݵĴ�Ŷ��û���͸���ģ����ڲ����ʲô��ʽ��ʲô��ַ��ֻ��Ҫ��������booter����Ӧ��ֻ��Ҫ����CSUDIBOOTSHOWSetShowUnitInfo���þͿ����ˡ�

booter��֧�ֶ����ʾ��Ԫ����ֻ������һ��������ʾ����������˶��������index��С�ĵ�ԪΪ׼��

@version 2.0.2 2011/12/21 �����˴������ö�ٶ���
@version 2.0.1 2011/06/21 ����

@{
*/
#ifndef _UDI2_BOOTSHOW_H_
#define _UDI2_BOOTSHOW_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_tuner.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ���ش������ö�ٶ���*/
enum {
	CSUDIBOOTSHOW_ERROR_BAD_PARAMETER = CSUDI_BOOTSHOW_ERROR_BASE,	///< �����쳣���� 
	CSUDIBOOTSHOW_ERROR_FEATURE_NOT_SUPPORTED,						///< ��֧�ִ˲��� 
	CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID,								///< �Ƿ���ԪID
	CSUDIBOOTSHOW_ERROR_UNKNOWN_ERROR								///< δ֪����
};

/**@brief Booter ��ʾ��Ԫ��ʽ�� */
#define CSUDIBOOTSHOW_UNITTYPE_BMPBITS   	(0x01)//��������,	CSUDIBOOTSHOWUnitInfo��uDetailInfoΪCSUDIBOOTSHOWPicInfo����
#define CSUDIBOOTSHOW_UNITTYPE_BMP    	(0x02)//bmpͼƬ��	CSUDIBOOTSHOWUnitInfo��uDetailInfoΪCSUDIBOOTSHOWPicInfo����
#define CSUDIBOOTSHOW_UNITTYPE_PNG    	(0x04)//pngͼƬ��CSUDIBOOTSHOWUnitInfo��uDetailInfo������
#define CSUDIBOOTSHOW_UNITTYPE_JPG    	(0x08)//jpgͼƬ��CSUDIBOOTSHOWUnitInfo��uDetailInfo������
#define CSUDIBOOTSHOW_UNITTYPE_MPEG    	(0x10)//mpgͼƬ��CSUDIBOOTSHOWUnitInfo��uDetailInfo������
#define CSUDIBOOTSHOW_UNITTYPE_SIGALGIF   (0x20)//gifͼƬ��CSUDIBOOTSHOWUnitInfo��uDetailInfo������
#define CSUDIBOOTSHOW_UNITTYPE_BMPZIPS   	(0x40)//ѹ���������飬CSUDIBOOTSHOWUnitInfo��uDetailInfoΪCSUDIBOOTSHOWPicInfo����
#define CSUDIBOOTSHOW_UNITTYPE_DVBLIVE   	(0x0100)//ֱ����Ƶ��CSUDIBOOTSHOWUnitInfo��uDetailInfoΪCSUDIBOOTSHOWDvbLiveInfo����
#define CSUDIBOOTSHOW_UNITTYPE_TSFILE		(0x0200)//ts��Ƶ�ļ���CSUDIBOOTSHOWUnitInfo��uDetailInfoΪCSUDIBOOTSHOWAVInfo����

/**@brief ����Booter����ʾ��������� */
typedef struct {
	unsigned int nBootDecodeCapablity; 				///<  Boot֧�ֵĽ���ģʽ����ΧΪCSUDIBOOTSHOW_UNITTYPE_BMPBITS��CSUDIBOOTSHOW_UNITTYPE_TSFILE�� ֧�ַ�Χ�����ݵ�������
	unsigned int nBooterShowFlashTotalSize;///<  ���ڴ洢Boot��ʾ��flash�ռ��ܴ�С����λΪBYTE
	unsigned int nBooterUnitHeadSize;		///<  �洢ÿ��Boot��ʾ��Ԫ����洢����ʾ��Ԫ��Ϣͷ����С����λΪBYTE
}CSUDIBOOTSHOWCapability;

/**@brief ��Ŀ����Ƶ��Ϣ*/
typedef struct {
	unsigned int nVideoPid;							///<  ��Ŀ��Ƶpid
	CSUDIVIDStreamType_E eVideoType;		///<	��Ŀ��Ƶ��ʽ
	unsigned int nAudioPid;							///<  ��Ŀ��Ƶpid
	CSUDIAUDStreamType_E eAudioType;		///<	��Ŀ��Ƶ��ʽ
}CSUDIBOOTSHOWAVInfo;

/**@brief ֱ����Ŀ��Ϣ*/
typedef struct {
	CSUDITunerType_E eType;							///<  tuner����
	CSUDITunerSRCDeliver_U uDeliver;		///<  Ƶ����Ϣ
	CSUDIBOOTSHOWAVInfo sAvInfo;			///<  ��Ŀ����Ƶ��Ϣ
}CSUDIBOOTSHOWDvbLiveInfo;

/**@brief ͼƬ��Ϣ*/
typedef struct {
	unsigned int nWidth;   							///<  ͼƬ�Ŀ��
	unsigned int nHigth;   							///<  ͼƬ�ĸ߶�
	unsigned int nPitch;   							///<  ͼƬ��һ��ռ�õ��ֽ���
}CSUDIBOOTSHOWPicInfo;

/**@brief ��ʾ��Ԫ����Ϣ
	@note ��ʾ��Ԫ=��ʾ��Ԫ��Ϣͷ+��ʾ��Ԫ���ݡ� ��ʾ��Ԫ���ݶ��û���͸���ģ�����������������booterά��*/
typedef struct {
	CSUDI_BOOL bNeedShow;							///<  �Ƿ���Ҫ��ʾ
	unsigned int nUnitType;						///<  ��ʾ��Ԫ�����ݸ�ʽ
	unsigned int nDataSize;						///<  ��ʾ��Ԫ�����ݴ�С
	unsigned int nUnitId;							///<  ��ʾ��ԪΨһ��ʶid������Ӧ����Ψһȷ��һ����ʾ��Ԫ��Ӧ����һ�㲻���¼index���ô�id���жϴ˵�Ԫ�Ƿ��Լ���ע�ĵ�Ԫ��
	union{
		CSUDIBOOTSHOWDvbLiveInfo sDvbLiveInfo;	///< ��nUnitType==CSUDIBOOTSHOW_UNITTYPE_DVBLIVEʱ����ϸ��ϢΪCSUDIBOOTSHOWDvbLiveInfo
		CSUDIBOOTSHOWPicInfo sPicInfo;					///< ��nUnitTypeΪCSUDIBOOTSHOW_UNITTYPE_BMPBITS��CSUDIBOOTSHOW_UNITTYPE_BMP��CSUDIBOOTSHOW_UNITTYPE_BMPZIPSʱ����ϸ��ϢΪCSUDIBOOTSHOWPicInfo
		CSUDIBOOTSHOWAVInfo sAvInfo;						///< ��nUnitType==CSUDIBOOTSHOW_UNITTYPE_TSFILEʱ,��ϸ��ϢΪCSUDIBOOTSHOWAVInfo
	}uDetailInfo;											///< ��ʾ��Ԫ����ϸ��Ϣ�������ʽ������ʾ��Ԫ��ʽ����
}CSUDIBOOTSHOWUnitInfo;

/**
@brief ��ȡbooter����ʾ��ص�����
@param[out] psCapability ��ȡbooter����ʾ����������ṹ�嶨��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ����ṹ��CSUDIBOOTSHOWCapability����
@note �ýӿڸ�����Ŀ��Ҫ��ѡʵ��
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetCapability(CSUDIBOOTSHOWCapability* psOutCapability);

/**
@brief ���booter��ʾ�õ����Ѵ洢��ÿ����ʾ��Ԫ����Ϣ
@param[in] nIndex �洢��ʾ��Ԫ��������ȡֵΪ0--�Ѵ洢��ʾ��Ԫ����-1��
@param[out] psOutInfo ��ʾ��Ԫ��Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetShowUnitInfo(int nIndex,CSUDIBOOTSHOWUnitInfo* psOutInfo);

/**
@brief ��ȡbooter���ڴ洢����ʾ��Ԫ���ݵ�flashʣ��ռ��С
@param[out] pnOutSize ʣ��ռ��С����λΪBYTE
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �ɴ洢��ʾ���ݴ�С = ʣ��ռ��С - 1����ʾ��Ԫ��Ϣͷ����С
@note ʣ��ռ�Ϊflash��ַ������ʣ����ÿռ��С��
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetFreeSize(unsigned int* pnOutSize);

/**
@brief ��ȡboot�Ѵ洢����������ʾ��Ԫ����
@param[out] pnOutCount �Ѵ洢����ʾ��Ԫ����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetUnitCount(unsigned int* pnOutCount);

/**
@brief ����booter��ʾĳ��ָ�����Ѵ洢������ʾ��Ԫ
@param[in] nIndex ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
@param[in] bNeedShow �Ƿ���Ҫ��ʾ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ����ĳ����ʾ��Ԫ��ʾ��������������ʾ��Ԫ������ʾ������ֻ��ʾһ����ʾ��Ԫ
@note ����Զ����Ԫ�������˸ýӿڣ�����index��С�ĵ�ԪΪ׼����������Ԫ��ʱ��������ʾҲ����Ч��
*/
CSUDI_Error_Code CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow);

/**
@brief ����booter��ʾ�õ���ÿ����ʾ��Ԫ����Ϣ
@param[in] nIndex ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С�ڵ����Ѵ洢��ʾ��Ԫ������
@param[in] psInfo ��ʾ��Ԫ��Ϣ
@param[in] pData  ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �������==�Ѵ洢��ʾ��Ԫ���������ʾ����һ����ʾ��Ԫ��
@note ����޸ĺ����ʾ��Ԫ��С���޸�ǰ�Ĳ�һ��������Ҫ����flash�������·���Ȳ�����
@note ĳЩ��ʾ��Ԫ�����ݿ����ǹ̶��ģ��������޸ġ���ʱ������CSUDIBOOTSHOWSetShowUnitInfo���ش���
*/
CSUDI_Error_Code CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo,void* pData);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



