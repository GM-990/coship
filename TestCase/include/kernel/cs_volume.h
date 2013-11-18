#ifndef _CS_VOLUME_H_
#define _CS_VOLUME_H_

#include "mmcp_typedef.h"
#include  "udi2_fs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@�Ƿ����߼�����Ŀ*/
#define CSVLM_INVALID_COUNT (-1)
/**@�߼���������󳤶�*/
#define CSVLM_MAX_NAME_LEN (256)

#define CSVLM_MAX_PARTITION_NAME (128)

/**@brief �߼����豸�¼�*/
typedef enum
{
	EM_VLM_FOUND,          /**<�����¼�,pEventDataΪCSVLMInfo_S��nEventDataδ����*/
	EM_VLM_UNMOUNT,     /**<ж���¼�,pEventDataΪCSVLMInfo_S��nEventDataδ����*/
	EM_VLM_MOUNT_FAIL, /**<����ʧ��,pEventDataΪCSVLMInfo_S��nEventDataδ����*/
	EM_VLM_MOUTED,          /**<�����¼�,pEventDataΪCSVLMInfo_S��nEventDataδ����*/
	EM_VLM_FORMAD	/**<���̱���ʽ���¼�,pEventDataΪCSVLMInfo_S��nEventDataδ����*/
}CSVLMEvent_E;

/**@brief �߼���ķ�������*/
typedef enum
{
	EM_VLM_PARTITION_UNKNOW = 0,/**< ��ʶ�������*/
	EM_VLM_PARTITION_FAT32 ,/**< FAT32���ͷ���*/
	EM_VLM_PARTITION_EXT2,   /**< EXT2���ͷ���*/
	EM_VLM_PARTITION_NTFS,    /**< NTFS���ͷ���*/
	EM_VLM_PARTITION_EXT3,   /**< EXT3���ͷ���*/
	EM_VLM_PARTITION_JFFS,   /**< JFFS���ͷ���*/
	EM_VLM_PARTITION_JFFS2,  /**< JFFS2���ͷ���*/
	EM_VLM_PARTITION_RAMFS,  /**< RAMFS���ͷ���*/
	EM_VLM_PARTITION_YAFFS2,  /**<YAFFS2���ͷ���*/

	
	EM_VLM_PARTITION_CRAMFS, /**< CRAMFS���ͷ���*/
	EM_VLM_PARTITION_ROMFS,	/**< ROMFS���ͷ���*/
	EM_VLM_PARTITION_UBIFS /**< UBIFS���ͷ���*/
}CSVLMPartitionType_E;

typedef int(*CSVLMNotify_F)(const CSVLMEvent_E eVlmEvent, const void * pEventData, const int nEventData, void *pUserData);


/**@brief �߼�����Ϣ*/
typedef struct  
{
	char m_szPartitionName[CSVLM_MAX_PARTITION_NAME];/**< �߼������ڵķ�����*/
	char m_szMountPoint[CSVLM_MAX_NAME_LEN];/**< �߼�����ص�·����*/
	CSVLMPartitionType_E m_ePartitionType;      /**< �߼���ķ�������*/
	BOOL m_bMounted;   /**< �߼����Ƿ��Ѿ����ص��ļ�ϵͳ*/
	int m_nStorageId;     /**< �߼������ڵĴ洢�豸��ʶ*/
	int m_nDeviceId; /**< �߼�����豸��ID ��ʶ*/
}CSVLMInfo_S;

/**
@brief �߼���ģ���ʼ��

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSVLMInit(void);

/**
@brief �û�ע���߼����¼�

@param[in] fnNotify ע���߼����¼��Ļص�����
@param[in] pUserData ע����û�˽������

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note fnNotify����Ϊ�գ�fnNotify��pUserDataΨһ��ʾһ��ע��
*/
BOOL CSVLMRegNotify(const CSVLMNotify_F fnNotify, const void *pUserData);

/**
@brief �û�ע���߼����¼�

@param[in] fnNotify ע���߼����¼�ʱ���ݵĻص�����
@param[in] pUserData ע��ʱ������û�˽������

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note fnNotify��pUserDataΨһ��ʾһ��ע��
*/
BOOL CSVLMUnregNotify(const CSVLMNotify_F fnNotify, const void *pUserData);

/**
@brief ��ȡϵͳ���ѹ��ص��߼�������

@return ʧ�ܷ���CSVLM_INVALID_COUNT�����򷵻��ѹ����߼���ĸ���
*/
int CSVLMGetVolumeCount(void);

/**
@brief ����������ȡ�߼�����Ϣ

@param[in] nIndex ָ�����߼�������
@param[out] psVolumeInfo �洢�߼�����Ϣ�ĵ�ַ

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSVLMGetVolumeInfo(const int nIndex,CSVLMInfo_S * psVolumeInfo);

/**
@brief �߼���ĸ�ʽ��

@param[in] szMountPoint �߼���Ĺ��ص�
@param[out] eVlmFormat ������ʽ���߼���ĸ�ʽ

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSVLMFormat(char * szMountPoint, CSVLMPartitionType_E eVlmFormat);

/**
@brief ж�ط���

@paramp[in] szMountPoint �߼���Ĺ��ص�

@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ж�ط���ʱ��ģ���ڻᷢ��umount�¼�
*/
BOOL CSVLMUmount(char* szMountPoint);

#ifdef __cplusplus
}
#endif

#endif

