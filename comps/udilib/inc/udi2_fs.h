/**@defgroup FS FS ����ƽ̨�ļ�ϵͳ��صĽӿ�,��Ҫ��U�̡��ƶ�Ӳ�̵ȵķ��ʽӿ�
@brief FS���ļ�ϵͳģ�飬��������ֲ���ļ�ϵͳ�Ķ�д���豸���ֵȽӿڣ���Ӧͷ�ļ�<udi2_fs.h >��

��ģ����Ҫ�������Ͳ������ô洢�豸����U�̡��ƶ�Ӳ�̡�����Ӳ���Լ�Flash�ϵ��ļ�ϵͳ��

��MMCP��CloudTV ���м��ƽ̨�������߳�ʼ����ɺ󣬽����ƶ��Ĵ洢�豸���뵽�ն˵�USB��ʱ����ģ��ͨ��ע��Ļص�֪ͨMMCP��CloudTV ���м��ƽ̨��

��MMCP��CloudTV ���м��ƽ̨��û������������ɳ�ʼ��֮ǰ�������ƶ��Ĵ洢�豸���뵽�ն˵�USB��ʱ������MMCP��CloudTV ���м��ƽ̨���ڵ�һ������ʱͨ��GetAllDeviceId��ȡ�����Ѿ����ڵ��豸����������豸�����ô��ڵģ�������Ӳ�̡�Flash�е��ļ�ϵͳ���������豸ʱ������֪ͨ��

��ģ�鴦����豸��Ϊ�洢�豸��U�̡�Flash��Ӳ�̵ȣ��ͷ־��豸�����������֡�һ���洢�豸���ܰ���0��n���־��豸������0����ʾ�洢�豸δ��ʽ����

���ַ����豸ʱ���Ὣ������MountPoint����λ�÷�����MMCP��CloudTV ���м��ƽ̨��MMCP��CloudTV ���м��ƽ̨�ڸ�λ���ϼ����ļ������·������ͨ����ģ���ṩ���ļ������ӿڽ��ж�д�Ȳ�����
һ��������U�̵��豸�������Ϣ�׳�������Ӧ��Ϊ��Found(�洢�豸)'Ready(�洢�豸) 'Found(�־�1) 'Ready���־�1��'Found(�־�2) 'Ready���־�2��������洢�豸���ж���־��豸����˳���ǲ�ȷ���ģ�����ͬ�־��豸��Found��Ready֮����ڽ������U����Ҫ�Լ죬��־��豸��Found��Ready��Ϣ֮�䣬�ɲ������ɸ�Scaning��Ϣ�����U�̱������ĳ���־��޷�ʶ����δ��ʽ��������ڲ�ʶ����ļ�ϵͳ��ʽ�ȣ�������Ready��Ϣ��Error��Ϣ�����

һ��������U�̵��豸���γ�����Ϣ�׳�������Ӧ��Ϊ��Plugout(�־�1) ' Plugout(�־�2) '����'Plugout(�־�n)'Plugout���洢�豸)�����Ȱγ��־��豸���ٰγ��洢�豸��

������������������RDIģ�������Ĺ��򱣳�һ�£�����RDI�����Ͻ��е�ϸ����

ԭ���Ͻ���USB�豸�ķ��֡����ʺ��ļ�ϵͳ֮�䣬������ǿ��Ϲ�ϵ����Linux�Ȳ���ϵͳ�ϣ�U�̵�Ҳ���Կ��豸��д����ʽ�ṩ������ӿڣ�Ȼ���ɲ���ϵͳ����ļ�ϵͳ�Ľ����ȹ����������ǵ������ƽ̨�ϣ��ر���Linuxϵͳ�ϣ�оƬ�������ļ�ϵͳ����ʽ�ṩ�˴洢�豸�ķ��ʽӿڣ������Ѿ����úܺ��ˡ�������ϸ��Ҫ����USBͨ���豸����ʽ�ṩ���豸��д����ʽ��ԭʼ�ӿڣ�һ���治����ʵ�֣���һ����Ҳ���MMCP��CloudTV ���м��ƽ̨����������������ļ�ϵͳ�������룬�˷ѿռ䡣

@note ��ģ���ļ�������ؽӿ���ϸ����˵����ο�C99�淶���ƽӿ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.6 2009/10/22 �����־������ö�ٶ��岻ͳһ������
@version 2.0.5 2009/09/15 ����CSUDIFSGetAllDeviceId��pnActIdCnt����ΪCSUDI_NULL�Ĵ�������
@version 2.0.4 2009/09/08 �޸Ŀ��ƶ��豸�ķ��ֹ���ʹ֮����RDIģ��Ķ���
@version 2.0.3 2009/8/24 ��һ���ۺ�������޸ĵİ汾
@version 2.0.2 2009/07/27 UDI2�ڶ�������汾
@version 2.0.1 2009/04/02 UDI2���壬����ԭKernelģ������ش�����ȡ�޸Ķ���,���������۵İ汾
@{
*/

#ifndef _UDI2_FS_H_
#define _UDI2_FS_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDIFS_CP_DEFAULT		(0)           ///< Ĭ�ϱ��뷽ʽ,����ֲ�㲻ȷ���ļ�ϵͳʹ�õı��뷽ʽʱʹ�ø�ģʽ,��������¿��ܻ��������
#define CSUDIFS_CP_GB2312	    (936)         ///< GB2312����
#define CSUDIFS_CP_BIG5			(950)         ///< big5����
#define CSUDIFS_CP_UTF8			(65001)       ///< UTF8

#define CSUDIFS_ENCODE_BIG_ENDIAN     (0x10000)     ///< bigendian,����λ��ǰ
#define CSUDIFS_ENCODE_LITTLE_ENDIAN  (0)           ///< littelendian,����λ��ǰ

#define CSUDIFS_SEEK_SET 	(0)	///< ���ļ���ͷ��seek
#define CSUDIFS_SEEK_CUR 	(1)	///< ���ļ���ǰλ��seek
#define CSUDIFS_SEEK_END  	(2) 	///< ���ļ���β��seek

#define CSUDIFS_MODE_REG 	(0x1)		///< ��ͨ�ļ�����CSUDIFSFileStat_S�ṹ��m_lMode��
#define CSUDIFS_MODE_DIR	(0x1 << 1) 	///< �ļ��У�CSUDIFSFileStat_S�ṹ��m_lMode��

#define CSUDIFS_ISREG(m)  (((m) & CSUDIFS_MODE_REG) == CSUDIFS_MODE_REG)	///< �ж��Ƿ�Ϊ��ͨ�ļ�,����CSUDIFSFileStat_S�ṹ��m_lMode��
#define CSUDIFS_ISDIR(m)  (((m) & CSUDIFS_MODE_DIR) == CSUDIFS_MODE_DIR)	///< �ж��Ƿ�Ϊ�ļ���,����CSUDIFSFileStat_S�ṹ��m_lMode��


/**@brief �豸������ش������ֵ*/
enum
{
	CSUDIFS_ERROR_BAD_PARAMETER = CSUDI_FS_ERROR_BASE,    ///< �������
	CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED,				  ///< ��֧�ֲ���
	CSUDIFS_ERROR_UNKNOWN_ERROR,				 	 	  ///< һ�����
	CSUDIFS_ERROR_NO_MEMORY,							  ///< ���ڴ���ô���
	CSUDIFS_ERROR_INVALID_DEVICE_ID,	    			  ///< �Ƿ��豸ID
	CSUDIFS_ERROR_DEVICE_BUSY, 							  ///< �豸��æ״̬
	CSUDIFS_ERROR_NO_CALLBACK,                           ///<  û����Ӧ�Ļص���������ɾ��
	CSUDIFS_ERROR_ALREADY_ADDED							  ///< �ûص��Ѿ�ע��
};

/**@brief �־������*/
typedef enum 
{
	EM_UDIFS_PARTITION_AUTO = 0,    	///< �Զ�����,��ƽ̨����ʹ��֧�ֵ���õ����ͣ��ӵײ��ȡ���ļ�ϵͳ���Ͳ���Ϊ��ֵ
	
	EM_UDIFS_PARTITION_READONLY_START,///< ֻ���ļ�ϵͳ�߽磬����EM_UDIFS_PARTITION_READONLY_END֮����ļ�ϵͳ����Ϊֻ���ļ�ϵͳ����ֵ������һ���ļ�ϵͳ
	EM_UDIFS_PARTITION_CRAMFS,				///< CRAMFS�����
	EM_UDIFS_PARTITION_ROMFS,				  ///< ROMFS�����
	EM_UDIFS_PARTITION_READONLY_END = 0x100,///< ֻ���ļ�ϵͳ�߽磬����EM_UDIFS_PARTITION_READONLY_START֮����ļ�ϵͳ����Ϊֻ���ļ�ϵͳ����ֵ������һ���ļ�ϵͳ
	
	EM_UDIFS_PARTITION_RW_START,		///< �ɶ�д�ļ�ϵͳ�߽磬����EM_UDIFS_PARTITION_RW_END֮����ļ�ϵͳ����Ϊ�ɶ�д���ļ�ϵͳ����ֵ������һ���ļ�ϵͳ
	EM_UDIFS_PARTITION_FAT32 ,      ///< FAT32���ͷ���
	EM_UDIFS_PARTITION_EXT2,        ///< E2FS���ͷ���
	EM_UDIFS_PARTITION_EXT3,			///< EXT3���ͷ���
	EM_UDIFS_PARTITION_JFFS2,			///< JFFS2���ͷ���
	EM_UDIFS_PARTITION_NTFS,        ///< NTFS���ͷ���
	EM_UDIFS_PARTITION_UBIFS,        ///< UBIFS���ͷ���
	EM_UDIFS_PARTITION_YAFFS2,        ///< YAFFS2���ͷ���
	EM_UDIFS_PARTITION_RW_END = 0x200///< �ɶ�д�ļ�ϵͳ�߽磬����EM_UDIFS_PARTITION_RW_START֮����ļ�ϵͳ����Ϊ�ɶ�д���ļ�ϵͳ����ֵ������һ���ļ�ϵͳ
}CSUDIFSPartitionType_E;

/**@brief �豸����*/
typedef enum
{
    EM_UDIFS_DEVTYPE_UNKNOWN,      ///< �߽�ֵ
	EM_UDIFS_DEVTYPE_STORAGE,	   ///< �洢�豸
    EM_UDIFS_DEVTYPE_VOLUME       ///< �־��豸,��U�̵ķ�����(windows�ϵ�G��,H�̸���),һ���洢�豸���ܺ���1-n���־��豸��ֱ�֪ͨ
}CSUDIFSDeviceType_E;

/**@brief �豸��Ϣ�ṹ*/
typedef struct
{
    CSUDI_UINT32    	m_dwDeviceId;        ///< �豸Id,һ�ο����������豸IdΨһ��ʶһ���豸��һ�β���,ͬһ�豸���������豸Id�ŷ����仯	
	CSUDIFSDeviceType_E m_eDeviceType;       ///< �豸����,���ƶ��豸���豸���ͱ���Ҫ�豸Id����һ�£��̶��豸���豸���ʹӸ�ֵ��ȡ
    char      	m_szDeviceName[128]; ///< �豸��Ψһ�豸����
    CSUDI_UINT64    	m_dwDeviceSize;      ///< �豸��С(�ֽ�Ϊ��λ)
    CSUDI_UINT32    	m_dwParentDeviceId;  ///< ���豸Id,�洢�豸��ֵ��Ч,�־��豸Ϊ�������Ĵ洢�豸��Id
    char m_szMountPoint[256];     	 ///< ����Ǵ洢�豸�ĸ���Ϊ"";�־��豸��ʾ���豸��UDIFS�еĿɷ���·��,���Ϊ""(����ΪCSUDI_NULL��Ҳ����Ϊ�ո�),��ʾ����һ������ʶ��ķ־�
    								///<���ص㲻����"/"��β
}CSUDIFSDeviceInfo_S;	

/**@brief �ļ�����

@note ��windowsһ��,m_lSize��ʾ�ļ�ʵ�ʴ�С,m_lBlksize*m_lBlocks��ʾռ�ô�С
*/
typedef struct
{
	unsigned long   m_lMode;		///< �ļ����ͣ����ڱ�ʾ���ļ������ļ��еȣ���ʹ��CSUDIFS_ISREG��CSUDIFS_ISDIR������ж�
	unsigned long   m_lSize;     	///< �ļ�ʵ�ʴ�С
	unsigned long   m_lBlkSize;  	///< ���С
	unsigned long   m_lBlkCount;    ///< ������
	unsigned long   m_lAtime;    	///< ����ʱ��,��1970��1��1��0ʱ0��0�������������ʾ
	unsigned long   m_lMtime;    	///< �޸�ʱ��,��1970��1��1��0ʱ0��0�������������ʾ
	unsigned long   m_lCtime;    	///< ����ʱ��,��1970��1��1��0ʱ0��0�������������ʾ
}CSUDIFSFileStat_S;

/**@brief �ļ������ݽڵ�����,һ�����һ���ļ��л����ļ�*/
typedef struct 
{
	unsigned char	m_ucDirType;        ///< �ڵ�����0-���ļ���,1-�ļ���
	char		m_cDirName[256];    ///< �ڵ�����
}CSUDIFSDirent_S;

/**@brief �ļ�ϵͳ����*/
typedef struct
{
	        CSUDIFSPartitionType_E m_eFsType;  ///< �ļ�ϵͳ����	        
	        long m_lBsize;                     ///< optimal transfer block size
	        long m_lBlocks;                     ///< total data blocks in file system
	        long m_lBfree;                  ///< free blocks in fs
	        long m_lBavail;                 ///< free blocks avail to non-superuser
	        long m_lFiles;                  ///< total file nodes in file system
	        long m_lFfree;                  ///< free file nodes in fs
	        long m_lNamelen;                ///< maximum length of filenames
}CSUDIFSStatFs_S;

/**
@brief ����豸֪ͨ�ص�����

���RDIģ��CSUDIRDIAddCallback_F�ӿڶ��壬�����ڴ���STORAGE������VOLUME�����͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,���豸״̬�����仯ʱ���øú���
@param[in] pvUserData �û�����,����Ϊ�ص�������pUserData��������
@return �ɹ���ӷ���CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��ӻص�ǰ�������豸�仯��Ϣ,���Ჹ��֪ͨ.��������ʱ��Щ�豸�ʹ���,���ñ�����ʱ����õ�֪ͨ.
@note fnNotify��pUserDataһ��Ψһȷ��һ���ص�������������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIFS_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief ɾ���豸֪ͨ�ص�����

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ���STORAGE������VOLUME�����͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,֮ǰע��ĺ���
@param[in] pvUserData �û�����,������ע��ʱ��һ��
@return �ɹ�ɾ������CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note fnNotify��pUserDataһ��Ψһȷ��һ���ص�����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief ��ȫ�Ƴ����ƶ��洢�豸

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ���STORAGE������VOLUME�����͵Ŀ��ƶ��豸
@param[in] dwDeviceId Ŀ���豸ID
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ֻ��ɾ�����ƶ��豸����ͼɾ�������豸�����򷵻�CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED��ͨ���豸���豸Id���ж��豸����
@note ɾ���豸�󲻿���ͨ�����豸Id��ȡ�豸��Ϣ
@note �û����øýӿڻᴥ��EM_UDIRDI_PLUGOUT_SAFE���͵�EM_UDIRDI_EVENT_PLUGOUT��Ϣ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId);

/**
@brief ��ȡ�����豸��Id

�������ƶ��ĺ͹̶��Ĵ洢�豸�ͷ־��豸
@param[out] pdwIds ���ڴ洢���д��ڵ��豸��Id
@param[in] nMaxIdCnt pdwIds����Ĵ�С,������ܷ��ض����豸ID,һ����˵��һ���־��U�̻��ʾΪ2���豸,��ֵ����32��������������
@param[out] pnActIdCnt ʵ�ʵ��豸ID������ֵ��ʾ�ж����豸���뿽����pdwIds�еĸ�����ͬ
@return �ɹ�����CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ
@note �ýӿڽ������м�����ע���豸,��U��,�ƶ�Ӳ��,�����м���з��ʵ�cramfs�豸��,������һ����˵����ʵ�ʵ��豸��.
��UDI������Ҫ������Ŀ����һЩ���ù�����
@note Ϊ���ּ��ݣ����зǿ��ƶ��豸���豸���Դ�0��ʼ����
@note ����pdwIdsΪCSUDI_CSUDI_NULL����������±�ʾӦ�ý���ע�ж��ٸ��洢�豸�������������Ǿ����Id

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt);

/**
@brief �����豸ID��ȡ�豸��Ϣ

@param[in] dwDeviceId Ŀ���豸ID,��ֵ��Դ��CSUDIFSGetAllDeviceId��ص�����
@param[out] psDeviceInfo ָ�򷵻��豸��Ϣ�ڴ��ָ��
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo);

/**
@brief ʹ�豸�������״̬

@param[in] dwDeviceId Ŀ���豸ID
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��֧�ִ洢�豸����,�־��豸����ʧ��
@note �Ѿ��ڴ���״̬�µ��ñ��ӿ��Է���CSUDI_SUCCESS

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId);

/**
@brief �����豸

@param[in] dwDeviceId Ŀ���豸ID
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��֧�ִ洢�豸����,�־��豸����ʧ��
@note �Ѿ��ڻ���״̬�µ��ñ��ӿ��Է���CSUDI_SUCCESS

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId);

/**
@brief ��ʽ���豸

@param[in] dwDeviceId Ŀ���豸ID
@param[in] eType ���ʽ���ɵ��ļ�ϵͳ��ʽ
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ����־��豸����ʽ�������־�
@note ����洢�豸�������豸��ʽ��Ϊһ���־�,������ԭ�־�(�������)PLUGOUT,�·־����Ļص�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType);

/**
@brief ��ȡ·�����뷽ʽ

@param[out] pnCodePage �����ֽ�˳�� | ���뷽ʽ,������Ϻ궨��
ͨ���ú���ȷ����UDIFS������·���ı��뷽ʽ
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetCodePage(int * pnCodePage); 

/**
@brief ���ļ�

@param[in] pcFileName �ļ���
@param[in] pcMode �򿪷�ʽ,���C99��׼
@return �ļ����,ʧ�ܷ���CSUDI_NULL
@note ��������������C99��׼fopen

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode);

/**
@brief ���ļ�

@param[in] hFile �ļ����,CSUDIFS_Open�ķ���ֵ
@param[in] pcBuf ָ���Ŷ������ݵ��ڴ�
@param[in] uCount �������ֽ���
@return ʵ�ʶ������ֽ���,����������uCount
@note uCountΪ0ʱ����0,�ļ�״̬�������κα仯
@note ����ֵ<0��ʾ��������ļ�����β��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount);

/**
@brief д�ļ�

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] pcBuf ָ����д�����ݵ��ڴ�
@param[in] uCount �������ֽ���
@return ʵ��д����ֽ���,����������uCount
@note uCountΪ0ʱ����0,�ļ�״̬�������κα仯

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount);

/**
@brief �ر��ļ�

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSClose(CSUDI_HANDLE hFile);

/**
@brief seek�ļ�

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] lOffset �ƶ�ƫ��
@param[in] uOrigin �ƶ���ʼλ��,CSUDIFS_SEEK_SET,CSUDIFS_SEEK_CUR,CSUDIFS_SEEK_END
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin);

/**
@brief seek�ļ�64λ��

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] i64Offset �ƶ�ƫ��
@param[in] uOrigin �ƶ���ʼλ��,CSUDIFS_SEEK_SET,CSUDIFS_SEEK_CUR,CSUDIFS_SEEK_END
@return �ɹ�����0,�����������-1
@note ��ʹ�ļ�ϵͳ��֧�ֳ���2G���ļ�Ҳ����ʵ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin);

/**
@brief ��ȡ�ļ���ǰλ��

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@return �ɹ������ļ���ǰ�࿪ʼ����ƫ��,�����������-1
@note ��ǰƫ�ƴ���long�ܱ�ʾ�����Χʱ(32λ����Ϊ2G),����-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
long CSUDIFSTell(CSUDI_HANDLE hFile);

/**
@brief ��ȡ�ļ���ǰλ��64λ��

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@return �ɹ������ļ���ǰ�࿪ʼ����ƫ��,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile);

/**
@brief flush�ļ�

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFlush(CSUDI_HANDLE hFile);

/**
@brief ɾ���ļ�

@param[in] pcFileName �ļ���
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRemove(const char * pcFileName);

/**
@brief �������ļ�

@param[in] pcOldPath ���ļ���
@param[in] pcNewPath ���ļ���
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ�����Ч
@note �ýӿڿ��Ըı��ļ���Ŀ¼,�����ᴴ����Ŀ¼,����pNewPathָ����Ŀ¼�������򷵻�ʧ��
@note ���pNewPathָ����ļ��Ѵ���,����ǲ�ȷ����,�����ø�����.

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath);

/**
@brief �ض��ļ�

��һ���ļ��ض�Ϊһ������
@param[in] pcPathName �ļ���
@param[in] ulLength ���ļ�����
@return �ɹ�����0,�����������-1
@note ����³���С��ԭ�ļ�����,�����Ĳ��ֶ���;����³��ȴ���ԭ�ļ�����,������������0���
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength);

/**
@brief �ض��ļ���������

��һ���ļ��ض�Ϊһ������
@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] ulLength ���ļ�����
@return �ɹ�����0,�����������-1
@note ����³���С��ԭ�ļ�����,�����Ĳ��ֶ���;����³��ȴ���ԭ�ļ�����,������������0���
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength);

/**
@brief �ض��ļ�64λ��

��һ���ļ��ض�Ϊһ������
@param[in] pcPathName �ļ���
@param[in] u64Length ���ļ�����
@return �ɹ�����0,�����������-1
@note ����³���С��ԭ�ļ�����,�����Ĳ��ֶ���;����³��ȴ���ԭ�ļ�����,������������0���
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length);

/**
@brief �ض��ļ�������64λ��

��һ���ļ��ض�Ϊһ������
@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[in] u64Length ���ļ�����
@return �ɹ�����0,�����������-1
@note ����³���С��ԭ�ļ�����,�����Ĳ��ֶ���;����³��ȴ���ԭ�ļ�����,������������0���
@note �ýӿڶ��ļ�����Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length);

/**
@brief ��ȡ�ļ�����

@param[in] pcFileName �ļ���
@param[out] psFileStat ����ļ�����
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ�����Ч
@note psFileStat->m_lMode ֵ������ȷ��ֻ������CSUDIFS_ISREG��CSUDIFS_ISDIR����Ӧ��Ϊ����ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat);

/**
@brief ��ȡ�ļ������ļ���������

@param[in] hFile �ļ����,CSUDIFSOpen�ķ���ֵ
@param[out] psFileStat ����ļ�����
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ�����Ч
@note psFileStat->m_lMode ֵ������ȷ,��ΪCSUDIFS_ISREG��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat);

/**
@brief �����ļ���

@param[in] pcPathName �ļ�����
@param[in] nReserved �����ֶ�,���봫0,�����뷵��ʧ��
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSMkdir(const char * pcPathName, int nReserved);

/**
@brief ɾ���ļ���

@param[in] pcPathName �ļ�����
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRmdir(const char * pcPathName);

/**
@brief ���ļ���

@param[in] pcPathName �ļ�����
@return �ɹ������ļ��о��,ʧ�ܷ���CSUDI_NULL

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName);

/**
@brief �ر��ļ���

@param[in] hDir �ļ��о��,CSUDIFSOpendir�ķ���ֵ
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSCloseDir(CSUDI_HANDLE hDir);

/**
@brief ��ȡ�ļ���

һ�ζ�ȡ���ļ����µ�һ���ڵ����Ϣ,���ε��ÿɻ�ȡ���ļ����������ļ���Ϣ
@param[in] hDir �ļ��о��,CSUDIFSOpendir�ķ���ֵ
@return ����һ���ļ������ݽڵ�ָ��,û�и���ڵ�ʱ����CSUDI_NULL
@note
- �ýӿڽ��г������ļ����ļ��У����ݹ�
- ���ļ��а���"."��".."����Ŀ¼

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir);

/**
@brief ��ȡ�־���Ϣ

��ȡpcPathName���ڷ־�ķ־���Ϣ,����̴�С,���пռ��
@param[in] pcPathName �־�·��,��������Ϣ�ķ־��·��
@param[out] psFsStat �洢�־���Ϣ�ڴ�ָ��
@return �ɹ�����0,�����������-1
@note �ýӿڶ��ļ����ļ��о���Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat);

/**
@brief ͨ���ļ������ȡ�־���Ϣ

��ȡhFile��Ӧ���ļ����ڷ־�ķ־���Ϣ,����̴�С,���пռ��
@param[in] hFile �ļ����,��ȥ��ȡ���ļ����ڵķ־����Ϣ
@param[out] psFsStat �洢�־���Ϣ�ڴ�ָ��
@return �ɹ�����0,�����������-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

