/**@defgroup storage �洢�豸�ķ��֡�������ѯ�ӿ�
@brief �洢�豸�ķ��ֹ�����ѯ�ӿ�

@version 1.0 2008/12/12 ��ʼ�汾
to do : SetExistHdd ��Ϊ��Щ��Ŀ�����д��̣����̵ĳ�ʼ���ܺ�ʱ
@{
*/
#ifndef _CS_STORAGE_H_
#define _CS_STORAGE_H_

#include "udi_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@�洢�豸������󳤶�*/
#define CSSTRG_NAME_SIZE (128)
/**@�洢�豸��Ĵ�С*/
#define CSSTRG_BLOCK_SIZE  (512)  
/**@�洢�豸�Ƿ�����ֵ*/
#define CSSTRG_INVALID_INDEX (-1)
/**@�洢�豸�Ƿ���Ŀ*/
#define CSSTRG_INVALID_COUNT (-1)

/**@brief �洢�豸������*/
typedef enum
{
	EM_STRG_TYPE_ATA, /**< ATA�豸������SATA,PATA,CDROM��*/
	EM_STRG_TYPE_USB  /**< USB�豸������U�̺�USB�ƶ�Ӳ�̵�*/
}CSSTRGType_E;

/**@brief �洢�豸�з���������*/
typedef enum 
{
	EM_STRG_PARTITION_UNKNOW = 0,/**< ��ʶ�������*/
	EM_STRG_PARTITION_FAT32 ,/**< FAT32���ͷ���*/
	EM_STRG_PARTITION_E2FS,   /**< E2FS���ͷ���*/
	EM_STRG_PARTITION_NTFS,    /**< NTFS���ͷ���*/
	EM_STRG_PARTITION_E3FS   	/**< E3FS���ͷ���*/
}CSSTRGPartitionType_E;

/**@brief �洢�豸��Ϣ*/
typedef struct
{
	char m_szStorage[CSSTRG_NAME_SIZE];/**< �洢�豸���豸��*/
	int m_nIndex;/**< �洢�豸������ֵ*/
	int m_nId;      /**< �洢�豸��ID��ʶ*/
	CSSTRGType_E m_eType; /**< �洢�豸������*/
}CSSTRGInfo_S;

/**@brief �洢�豸�з�������Ϣ*/
typedef struct
{
	char m_szPartition[CSSTRG_NAME_SIZE];/**< �����ķ�����*/
	CSSTRGPartitionType_E m_ePartitionType;	/**< ����������*/
	DWORD m_dwBlockCount;/**< �����Ŀ�������512�ֽ�Ϊ��λ*/
}CSSTRGPartitionInfo_S;

/**@brief �洢�豸���¼�*/
typedef enum
{
	EM_STRG_EVT_CREATE, /**<�豸����,pEventDataΪCSSTRGInfo_S��nEventDataδ����*/
	EM_STRG_EVT_DELETE   /**<�豸�γ�,pEventDataΪCSSTRGInfo_S��nEventDataδ����*/
}CSSTRGEvent_E;



typedef int(*CSSTRGNotify_F)(const CSSTRGEvent_E eDevEvent, const void * pEventData, const int nEventData, void *pUserData);

/**
@brief ��������

@param[in] bIsInitSata �Ƿ��ʼ��SATAӲ��

@note ���ϵͳ������SATAӲ�̣����ʼSATAӲ�̻Ứ�ܳ�ʱ��
@note ���ڳ�ʼ��Storage֮ǰ���ñ��ӿ�
*/
void CSSTRGConfig(BOOL bIsInitSata);

/**
@brief ��ģ���ʼ��

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSTRGInit(void);

/**
@brief �û�ע��洢�豸�¼�

@param[in] fnNotify ע��洢�豸�¼��Ļص�����
@param[in] pUserData ע����û�˽������

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note fnNotify����Ϊ�գ�fnNotify��pUserDataΨһ��ʾһ��ע��
*/
BOOL CSSTRGRegNotify(const CSSTRGNotify_F fnNotify, const void *pUserData);

/**
@brief �û�ע���洢�豸�¼�

@param[in] fnNotify ע��洢�豸�¼�ʱ���ݵĻص�����
@param[in] pUserData ע��ʱ������û�˽������

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note fnNotify��pUserDataΨһ��ʾһ��ע��
*/
BOOL CSSTRGUnregNotify(const CSSTRGNotify_F fnNotify, const void *pUserData);


/**
@brief ��ȡϵͳ�д洢�豸������

@return ʧ�ܷ���CSSTRG_INVALID_COUNT�����򷵻�ϵͳ�д洢�豸����
*/
int CSSTRGGetStorageCount(void);

/**
@brief ����ID��ʶ��ȡ�洢�豸��Ϣ

@param[in] nStorageId �洢�豸ID��ʶ
@param[out] psStorageInfo �洢�豸��Ϣ�ĵ�ַ

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note һ���洢�豸��ϵͳ��⵽������ID��ʶ��Ψһ�ı�ʶ�˸��豸�����豸����������Ψһ�ģ������豸�Ĳ���Ͱγ������п��ܻᷢ���ı䡣
*/
BOOL CSSTRGGetStorageInfoById(const int nStorageId, CSSTRGInfo_S *psStorageInfo);

/**
@brief ����������ȡ�洢�豸��Ϣ

@param[in] nStorageIndex �洢�豸������
@param[out] psStorageInfo �洢�豸��Ϣ�ĵ�ַ

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note һ���洢�豸��ϵͳ��⵽������ID��ʶ��Ψһ�ı�ʶ�˸��豸�����豸����������Ψһ�ģ������豸�Ĳ���Ͱγ������п��ܻᷢ���ı䡣
*/
BOOL CSSTRGGetStorageInfoByIndex(const int nStorageIndex, CSSTRGInfo_S *psStorageInfo);

/**
@brief ��ȡ�洢�豸�ķ�������

@param[in] szStorage ָ���洢�豸����

@return ʧ�ܷ���CSSTRG_INVALID_COUNT�����򷵻�ָ���洢�豸�ĸ���
*/
int CSSTRGGetPartitionCount(const char* szStorage);

/**
@brief ��ȡ�洢���Ϸ�����Ϣ

@param[in] szStorage ָ���Ĵ洢�豸����
@param[in] nPartionIndex ָ���ķ�������
@param[out] psPartitionInfo ���������Ϣ

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSTRGGetPartionInfo(const char* szStorage, const int nPartionIndex, CSSTRGPartitionInfo_S *psPartitionInfo);

/**
@brief �洢�豸����

@param[in] szStorage ָ���Ĵ洢�豸����

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ִ�д洢�豸�Ĵ������ѿ��ܻ�����ִ��
*/
BOOL CSSTRGStandby(const char * szStorage);

/**
@brief �洢�豸����

@param[in] szStorage ָ���Ĵ洢�豸����

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ִ�д洢�豸�Ĵ������ѿ��ܻ�����ִ��
*/
BOOL CSSTRGWakeup (const char * szStorage);

/**
@brief ������ �洢�豸���и�ʽ��

@param[in] szStorage ָ���Ĵ洢�豸����
@param[in] eType       ָ���ĸ�ʽ��������

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSTRGFormat(const char * szStorage,CSSTRGPartitionType_E eType);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /*_CS_STORAGE_H_*/

