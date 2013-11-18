/**@defgroup CS_ZipUtilģ����kernel��zlib��ӿڵķ�װ�㣬�Ա�����ṩ���õĿ����ԡ�
@brief zip/jar�ļ���ȡ֧��ģ��
 
@version 1.0 2010/04/27 Initial Version
@version 1.1 2010/06/02 comment�޸�Ϊ����
@{
*/

#ifndef _CSZIPUTIL_H_
#define _CSZIPUTIL_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"

/**@brief 
 ģ��˵����֧��zip/jar��ʽ���ļ���ȡ��ע������������ܵ����쳣��
 - zip�ļ���С����INT_MAX(0x7FFFFFFF)ʱ����֧�ֶ�ȡ.
 - zip�ļ������entry(entry, ��ѹ���������һ��ʵ��)��С����INT_MAXʱ����֧�ֶ�ȡ.
 - zip�ļ���entry�ĸ�������INT_MAX.
 - m_nTime��m_nCrc�ֶ�Ϊ�з�����, ��Ȼʵ�������Ǳ���Ϊ�޷�����.
 - ���csize(entryѹ����Ĵ�С)ֵΪ0�����ʾentryδ��ѹ��.
 - ���m_pcExtra�ֶβ�����NULL, ��m_pcExtra��ͷ�����ֽڱ�ʾextra���ݵĳ���
      (С���ֽ���).
 - ���m_nPosֵΪ��, ���ֵ��ʾentryLOCͷ��λ��. ��entry�״α���ȡʱ, m_nPos����ֵΪ
       entry����(entry data)�ĵ�λ��.
 */

typedef struct _CSZIPUTILEntry_S {  /**< zip�ļ�entry(ѹ�����е�һ��ʵ��) */
    char *m_pcName;	  	  			/**< ʵ���� */
    int   m_nTime;            		/**< �޸�ʱ�� */
    int   m_nSize;	  	  			/**< ѹ��ǰ���ݴ�С */
    int   m_nCSize;  	  			/**< ѹ�������ݴ�С (���Ϊ0, ��ʾδѹ��) */
    int   m_nCrc;		  			/**< δѹ�����ݵ�CRCֵ */
    char *m_pcComment;	  			/**< zip�ļ�ע��(��ѡ) */
    char *m_pcExtra;	  			/**< ��������(��ѡ) */
    int   m_nPos;	  	  			/**< LOCͷ��λ�� (���ȡ��), ��data��λ��(ȡ��) */
}CSZIPUTILEntry_S;

/**@brief 
 �ڴ��е�hash��Ԫ��
 
 ��һ������ϵͳ��, ��������CSZIPUTILCell_S����, ���Ƕ�ÿһ����Ծ״̬��JAR/ZIP�ļ�
 �е�ÿ��ʵ���Ӧһ��CSZIPUTILCell_S����.

 ע��: Ϊ��ʡ�ռ�, CSZIPUTILCell_S�в��洢entry������, ����ֻ�洢һ��32λ��hashֵ.
*/
typedef struct _CSZIPUTILCell_S {
    int 		   m_nPos;          /**< LOCͷ��zip�ļ��е�ƫ���� */
    unsigned int   m_unHash;		/**< ����entry���ֵ�32λhashֵ */
    unsigned short m_usNelen;       /**< ����(name)�Ͷ�������(extra data)�ĳ��� */
    unsigned short m_usNext;      	/**< hash��, ������CSZIPUTILFile_S->m_psEntries */
    int            m_nSize;			/**< δѹ����С */
    int            m_nCSize;		/**< ѹ�����С */
    int            m_nCrc;
}CSZIPUTILCell_S;

/**@brief zip�ļ������ṹ��. */
typedef struct _CSZIPUTILFile_S {
    char *m_pcName;	  	  			/**< zip�ļ��� */
    int m_nRefs;		  			/**< ��Ծ�������� */
    int m_nFd;		  				/**< ���ļ������� */
    void *m_pvLock;		  			/**< �ļ���ȡ�� */
    char *m_pcComment; 	  			/**< zip�ļ�ע�� */
    char *m_pcMsg;		  			/**< zip������Ϣ */
    CSZIPUTILCell_S *m_psEntries;   /**< hash CSZIPUTILCell_SԪ������ */
    int m_nTotal;	  	  			/**< entryʵ������ */
    unsigned short *m_pusTable;    	/**< Hash��ͷָ��: ָ��m_psEntries���� */
    int m_nTablelen;	  			/**< number of hash eads */
    struct _CSZIPUTILFile_S *m_psNext;  /**< ָ��������������һ��zip�ļ� */
    CSZIPUTILEntry_S *m_psCache;     /**< (����)������ͷŵ�entryʵ��ָ�� */
    /**< �洢��META_INFĿ¼�µ�, ��metadata����ص���Ϣ. */
    char **m_ppCMetanames;     		/**< meta������ (����������ΪNULL) */
    int m_nMetacount;	  			/**< meta������Ԫ�ظ��� */
    /**< �����pre-entryע��, ��������������� */
    char **m_ppcComments;
}CSZIPUTILFile_S;

/**
@brief
 ��ʼ��cs_ziputilģ��, ����0��ʾ�ɹ�, -1��ʾ���ܱ���ʼ��.
 
Detail Info:

@return
	 0: �ɹ�
	-1: ʧ��
@note 
*/
int CSZIPUTILInitialize();

/**
@brief
 ����cs_ziputilģ��, ��CSZIPUTILInitialize��Ӧ.
 
Detail Info:

@return
@note 
*/
void CSZIPUTILDestroy();

/**@brief 
 ����ָ����ģʽ(���ļ��Ĵ򿪷�ʽ, ��ɶ�,��д, ��д��. ��ȡ"CSFS_O_ACCMODE", 
 ȡֵ�ɲο� cs_fsapi.h ͷ�ļ�����)��һ��zip/jar�ļ�, ����CSZIPUTILFile_S����, 
 ����򿪹��̳���, �򷵻�NULL 
 
Detail Info:
 �����zip�ļ������г���, (*ppMsg)�����ش�����Ϣ�ַ���(���ppMsg!=NULL�Ļ�),
 ���������, (*ppMsg)������ֵΪNULL.

@param[in] pcName: zip�ļ���(URL+�ļ���, ��ֻ֧�־���·��)
@param[in/out]  ppMsg: ������Ϣ�ַ���
@param[in] nMode: ָ����zip/jar�ļ��򿪷�ʽ.�� CSFS_O_ACCMODE. 
                  �ο�cs_fsapi.hͷ�ļ�.
@return: �򿪵�CSZIPUTILFile�ṹ��ָ��, �����ʧ��, ����NULL.
@note 
*/
HCSHANDLE CSZIPUTILOpenGeneric(const char *pcName, char *ppMsg, int nMode);

/**
@brief
 ��ָ����zipʵ���ж�ȡָ��λ�úʹ�С��bytes.
 
Detail Info:
 ��ȷ��zip�ļ�Ԥ�ȱ�Lock.(������ʵ�ֹ��������Զ�Lock, �ɲ���ע) 
 ���ض�ȡ����ʵ���ֽ���, �����ȡ�����򷵻�-1.
 �������CSZIPUTILGetZipFileInfo(hZip)��ȡ��m_pcMsg��ֵ��ΪNULL,
 �������ȡ�����г���. m_pcMsg�а����г�����ص�����.
 
@param[in] hZip: zip�ļ����(handle)
@param[in] hEntry: zipʵ����
@param[in] nPos: ��ȡ����ʼλ��
@param[in] nLen: ���ȡ�����ݳ���
@return 
     ��ֵ: ʵ�ʶ�ȡ���ֽ���
       -1: ��ȡ����
@note 
*/
int CSZIPUTILRead(HCSHANDLE hZip, HCSHANDLE hEntry, 
	int nPos, void *pvBuf, int nLen);

/**
@brief 
 �ر�ָ����zip�ļ�����
 
Detail Info:

@param[in] hZip: ��رյ�zip�ļ����.
@return 
@note 
*/
void CSZIPUTILClose(HCSHANDLE hZip);

/**
@brief
 ���ִ�д˺���, �ɽ�ָ����zipʵ����뵽ָ����Byte������.
 ע��: �ú����Ὣ�����hEntry��Ӧ��CSZIPUTILEntry_S�ṹ���ͷŵ�.
 
Detail Info:

@param[in] hZip: zip�ļ����
@param[in] hEntry: zipʵ����
@param[int/out] pucBuf: ���ڴ洢ָ��zipʵ�����ݵ�Buffer��ַ
@param[out] pcEntrynm: ���ָ��zipʵ�������
@return 
	0: OK
   -1: error
@note
*/
int CSZIPUTILReadEntry(HCSHANDLE hZip, HCSHANDLE hEntry, 
	unsigned char *pucBuf, char *pcEntrynm);

/**
@brief 
 ��ָ��zip�ļ��л�ȡָ�����ֶ�Ӧ��zipʵ��, ���δ�ҵ����ֶ�Ӧ��ʵ��,�򷵻�NULL
 
Detail Info:

@param[in] hZip: zip�ļ����
@param[in] pcName: ����ҵ�zipʵ����
@return 
    Normal: �ҵ���zipʵ����
	  NULL: δ�ҵ�
@note
*/
HCSHANDLE CSZIPUTILGetEntry(HCSHANDLE hZip, const char *pcName);

/**
@brief
 ��ȡָ��zip�ļ��е�N��(��0��ʼ)zipʵ��, ���ָ��ʵ�ϵ�����ֵ������Χ
 (��zip�ļ�����3��ʵ��,����3���Խ��),�򷵻�NULL.
 
Detail Info:

@param[in] hZip: zip�ļ����. 
@param[in] nIndex: ����ҵ�zipʵ������ֵ.
@return
	Normal: �ҵ���zipʵ����
	  NULL: ����������Χ
@note 
*/
HCSHANDLE CSZIPUTILGetNextEntry(HCSHANDLE hZip, int nIndex);

/**
@brief
 �ͷ�ָ��zip�ļ��е�ָ��zipʵ��.
 
Detail Info:
 ʵ����ϵͳ��Ϊÿһ��zip�ļ�ά��һ�������ʹ�ù���zipʵ��Ļ���, ���Ż���
 �����ģʽ.

@param[in] hZip: zip�ļ����.  
@param[in] hEntry: ���ͷŵ�zipʵ����
@return 
@note 
*/
void CSZIPUTILFreeEntry(HCSHANDLE hZip, HCSHANDLE hEntry);

/**
@brief
 ��ѹ��ָ����zip�ļ���ָ��·����
 
Detail Info:
@param[in] pcZippath: zip�ļ�����·��(URL+�ļ���)
@param[in] pcTarget: ��ѹ�����ļ��洢��λ��.
@param[out] pnSize: ��ѹ����������ܴ�С
@return 
	 TRUE: OK
	FALSE: FAILED
@note 
*/
BOOL CSZIPUTILUnzip(char* pcZippath, char* pcTarget, int *pnSize);

/**
@brief
 ��ȡָ��zip�ļ������������Ϣ�ṹ��.
 
Detail Info:
@param[in] hZip: zip�ļ����.
@param[out] psZipfileInfo: zip�ļ�������Ϣ�ṹ��
@return 
	 0: OK
	-1: Error
@note 
*/
int CSZIPUTILGetZipFileInfo(HCSHANDLE hZip, CSZIPUTILFile_S *psZipfileInfo);

/**
@brief
 ��ȡָ��zipʵ������������Ϣ�ṹ��.
 
Detail Info:
@param[in] hZip: zipʵ����.
@param[out] psZipEntryInfo: zipʵ��������Ϣ�ṹ��
@return 
	 0: OK
	-1: Error
@note 
*/
int CSZIPUTILGetZipEntryInfo(HCSHANDLE hEntry, CSZIPUTILEntry_S *psZipEntryInfo);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* !_CSZIPUTIL_H_ */ 
