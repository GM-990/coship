/**@NDS CA ICAM�ӿ�����
@brief ����NDS CAʱ����ICAMģ��ķ�װ

@version 2.0.1 2010/02/23 UDI2����
@{
*/

#include "udi2_typedef.h"
#include "udi2_error.h"

typedef enum
{
	CSUDIICAM_XConnectionType_TunnerPlay,    
	CSUDIICAM_XConnectionType_TunnerRecord,  
	CSUDIICAM_XConnectionType_DiskPlay,    
	CSUDIICAM_XConnectionType_DiskRecord  
}CSUDIICAMXConnectionType_E;

typedef enum
{
	CSUDIICAM_DMXOutPutType_PLAY   = 0x01,   //ֱ��
	CSUDIICAM_DMXOutPutType_RECORC = 0x02,   //¼��
	CSUDIICAM_DMXOutPutType_CAT    = 0x04,   //���⴦��
}CSUDIICAMDmxOutputType_E;

/**
@brief NDS ICAM��ʼ��

@param:��
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ���CSUDI_FAILURE
*/
int CSUDIICAMInit(void);

/**
@brief �������ӣ����Ž�Ŀʱ���øú���
@(Ŀǰ��BCMƽ̨������Ҫ���ô˺������������ƽ̨��ֱ�ӷ���CSUDI_SUCCESS)

@param[in]:
@ulXConnection     : ����ͨ����
@emDmxType         : ��ǰ��ֱ������¼��(Ԥ������ʱ����)
@wDemuxId          : Ŀǰ��Boardcomƽ̨���õ���Ϊinputband
@ulParserBand      : Ŀǰ��Boardcomƽ̨���õ���Ϊparserband
@nHwModule         : ����ͨ�����
@emXConnectionType : ö������
@param[out]:��
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ���CSUDI_FAILURE
*/
int CSUDIICAMOpenXconnection(
      unsigned long         ulXConnection,
      EM_DmxOutputType      emDmxType,
      unsigned short        wDemuxId,
      unsigned long         ulParserBand,
      int                   nHwModule,      
      EM_XConnectionType    emXConnectionType
);

/**
@brief �Ͽ����ӣ�ֹͣ��Ŀ����ʱ���øú���
@(Ŀǰ��BCMƽ̨������Ҫ���ô˺������������ƽ̨��ֱ�ӷ���CSUDI_SUCCESS)

@param[in]:
@ulXConnection     : ����ͨ����
@param[out]:��
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
*/
int CSUDIICAMCloseXconnection(unsigned long  ulXConnection);

/**
@brief ��ȡChip Id

@param[in/out]:
@paucChipData : �ϲ����ռ䣬�ռ䲻С��8���ֽڣ����ڴ洢Chip Id����(ǰ��4���ֽ�Ϊchip id)
@pnLen        : ����ʵ�ʻ�ȡ�������ݳ���
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ���CSUDI_FAILURE
*/
int CSUDIGetChipId(unsigned char *paucChipData, int *pnLen);

/**
@brief ��ȡDevice Id

@param[in/out]:
@paucDeviceData : �ϲ����ռ䣬�ռ䲻С��8���ֽڣ����ڴ洢Device Id����
@pnLen        : ����ʵ�ʻ�ȡ�������ݳ���
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ���CSUDI_FAILURE
*/
int CSUDIGetDeviceId(unsigned char *paucDeviceData, int *pnLen);

