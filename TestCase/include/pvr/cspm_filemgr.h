/******************************************************************************
FileName:  cspm_filemgr.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

�ļ�����:
������PVR�洢�������PVR�ļ��������ؽӿ�
ģ����PVRFILE

�޸ļ�¼:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      �޽�           Create

*******************************************************************************/
#ifndef CSPM_FILEMGR_H
#define CSPM_FILEMGR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cspm_record.h"
#include "cspm_notifymgr.h"

/*! Pvr�ļ�Ĭ�Ϸ�������*/
#define CSPVRFILE_GROUP_ALL_IDX  0

/*!Pvr�ļ�·��������*/
#define CSPVRFILE_PATH_NAME_LEN 256

/*!
========================================= �洢�豸��صĽӿ� =========================================
*/
/*!
	��ȡϵͳ���д洢�豸�Ŀ��п����Ŀ
\return ϵͳ���д洢�豸�Ŀ��п�(���Сλ512B)����Ŀ
\note ���صĿ��п���Ŀ��Ӳ����ʵ�ʿ��п���Ŀ��ȥΪTShift��Ԥ������Ŀ��
\note ����Ӳ����ʵ�ʹ��п��п�10M��TShiftԤ������ĿΪ6M���򱾺������صĿ��п���ĿΪ4M
*/
DWORD CSPVRFILEGetAllStorageFreeBlocks(void);

/*!
	��ȡϵͳ���д洢�豸�Ŀ�����Ŀ
\return ϵͳ���д洢�豸�Ŀ�(���Сλ512B)����Ŀ
*/
DWORD CSPVRFILEGetAllStorageBlocks(void);
/*!
	��ʽ��ϵͳ�����д洢�豸
\return �ɹ�����TRUE; ʧ�ܷ���FALSE
\note ��ֹͣ����Pvr�ļ��طš�¼�ƺ��ٸ�ʽ��Ӳ��
\note ���ϵͳ����ʹ�ô洢�豸�����ʽ��ʧ��
*/
BOOL CSPVRFILEFormatAllStorage(void);
/*!
	 ��ȡϵͳ��ʶ��Ĵ洢�豸����
\return ϵͳ��ʶ��Ĵ洢�豸����
*/
DWORD CSPVRFILEGetStorageCount(void);
/*!
	��ȡ�洢�豸���
@paran[in] nIdx �洢�豸����
\return �洢�豸���
*/
HCSHANDLE CSPVRFILEGetStorageHandle(int nIdx);
/*!
	��ȡ�洢�豸���п���Ŀ
@param[in] hStorage �洢�豸���
\return ϵͳ�洢�豸��ʵ�ʿ��п�(���Сλ512B)����Ŀ
*/
DWORD CSPVRFILEGetStorageFreeBlocks(HCSHANDLE hStorage);
/*!
	��ȡ�洢�豸�ܿռ����Ŀ
@param[in] hStorage �洢�豸���
\return ϵͳ�洢�豸���ܿ���Ŀ
*/
DWORD CSPVRFILEGetStorageBlocks(HCSHANDLE hStorage);
/*!
	��ʽ���洢�豸
@param[in] hStorage �洢�豸���
\return �ɹ�����TRUE, ʧ�ܷ���FALSE
\note ��ֹͣ����Pvr�ļ��طš�¼�ƺ��ٸ�ʽ��Ӳ��
\note ���ϵͳ����ʹ�ô洢�豸�����ʽ��ʧ��
*/
BOOL CSPVRFILEFormatStorage(HCSHANDLE hStorage);

/*!
	��ȡ�洢�豸����
@param[in] hStorage �洢�豸���
\return ���ش洢�豸���ƣ�ASII���ַ���
*/
const char* CSPVRFILEGetStorageName(HCSHANDLE hStorage);

/*!
	����¼��Ŀ��洢�豸
@param[in] hStorage �洢�豸���
\return �ɹ�����TRUE��ʧ�ܷ���FALSE
\note Ŀ��洢�豸ʵ�ʿ��пռ�������TShiftԤ���ռ�
*/
BOOL CSPVRFILESetRecordTarget(HCSHANDLE hStorage);

/*!
	�Ƿ��������ɾ�����λ���ļ�
\return�ɹ�����TRUE; ʧ�ܷ���FALSE
\note ��Filelist�Ի����˳�ʱ�����ж��Ƿ��������ɾ������ļ�������ǣ�������ʾ�û�ɾ���ļ�
*/
BOOL CSPVRFILEIsExistDelFlag(void);

/*!
	ɾ�����д�ɾ����ǵ��ļ�(�Ӵ洢�豸��ɾ�������ɻָ�)
\return �ɹ�����TRUE; ʧ�ܷ���FALSE
\note Ӧ�þ�����ɾ���ļ����첽ִ�е�,��ɾ���ļ�������,����ɾ���ļ�����֪ͨ�û�
*/
BOOL CSPVRFILEConfirmDelFile(void);

/*!
	ȡ�������ļ���ɾ�����
*/
BOOL CSPVRFILECancelAllDelFlag(void);

/*!
========================================= �ļ�������ؽӿ� =========================================
\note ϵͳ��һ��Ĭ�Ϸ��� CSPVRFILE_GROUP_ALL_IDX, ����������Pvr�ļ��ļ�,�÷��鲻�ɱ༭
*/

/*!
	�½�һ������
@param[in] wsGroupName ��������WideWord���ַ���
\return �½�����ľ��
\note ���з�����Ϣ����籣��,�����ķ���,�ڻ����ؿ�����,ϵͳ���Զ������÷���Ķ���
*/
HCSHANDLE CSPVRFILENewGroup(const char *wsGroupName);

/*!
	�ļ���ӵ�����
@param[in] hFile Pvr�ļ����
@param[in] hGroup ������
\return �ɹ�����TRUE; ʧ�ܷ���FALSE
\note ����ļ�������ĳ���飬�ٵ��ñ�������ӵ��÷���ʱ����TRUE
\note ���ܽ��ļ���ӵ�Ĭ�Ϸ���CSPVRFILE_GROUP_ALL_IDX
*/
BOOL CSPVRFILEAddFileToGroup(HCSHANDLE hFile, HCSHANDLE hGroup);

/*��
	���ļ��ӷ������Ƴ�
@param[in] hFile Pvr�ļ�
@param[in] hGroup ������
\return �ɹ�����TRUE; ʧ�ܷ���FALSE
\note ����ļ�������ĳ���飬�ٵ��ñ������Ӹ÷����Ƴ��ļ�ʱ����TRUE
\note ���ܽ��ļ���Ĭ�Ϸ���CSPVRFILE_GROUP_ALL_IDX���Ƴ�
\note ������ֻ�ǽ��ļ��ӷ������Ƴ�����δɾ�����ļ�
*/
BOOL CSPVRFILERemoveFileFromGroup(HCSHANDLE hFile, HCSHANDLE hGroup);

/*!
	��ȡϵͳ���з������Ŀ
\return ����ϵͳ���з������Ŀ��ϵͳ����ʱ����0
\note ���������������һ��Ĭ��ALL����
*/
int CSPVRFILEGetGroupCount(void);

/*!
	��ȡ����
@param[in] nIdx ��������(���ڵ���ALL ��С��ϵͳ���з�����Ŀ)
\return ������
*/
HCSHANDLE CSPVRFILEGetGroup(int nIdx);
/*!
	�ж��ļ��Ƿ����ڷ���
@param[in] hFile Pvr�ļ�
@param[in] hGroup ������
\return �ļ����ڷ��鷵��TRUE; ���򷵻�FALSE
\note
*/
BOOL CSPVRFILEIsFileInGroup(HCSHANDLE hFile, HCSHANDLE hGroup);

/*!
	��ȡ������û���
@param[in] hGroup ������
@param[out] wsOutName �����������wideword���ͣ�
@param[in] nLen ����������ַ����Ĵ�С
\return �ɹ�����TRUE; ʧ�ܷ���FALSE
\note Ĭ�Ϸ���CSPVRFILE_GROUP_ALL_IDX�ķ�����Ϊ "ALL"
*/
BOOL CSPVRFILEGetGroupName(HCSHANDLE hGroup,char *wsName, int nLen);

/*!
	������������
@param[in] szGroupName �·�����(wideword �ַ���)
@param[in] nNameLen �·������ַ�������
@param[in] hGroup ������
\return �ɹ�����TRUE; ʧ�ܷ���FALSE
\note Ĭ�Ϸ���CSPVRFILE_GROUP_ALL_IDX�ķ����������޸�
*/
BOOL CSPVRFILERenameGroup(const char* wsGroupName, int nNameLen, HCSHANDLE hGroup);

/*!
	ɾ����Ŀ����
@param[in] hGroup ������
\return �ɹ�����TRUE; ʧ�ܷ���FALSE
\note ����ɾ��Ĭ�Ϸ���CSPVRFILE_GROUP_ALL_IDX
\note ɾ������󣬻�����������ķ��������ĸı�
*/
BOOL CSPVRFILEDelGroup(HCSHANDLE hGroup);

/*!
========================================= �ļ��б���ؽӿ� =========================================
\note �ļ��б�����б���������ͨ��Iterator��ɣ�
*/

/*��
	����������
@param[in] hGroup ������
\retuen ����������������������
\note 
*/
HCSHANDLE CSPVRFILECreateIterator(HCSHANDLE hGroup);

/*��
	��λ���������ĵ�һ��Ԫ��
@param[in] hIterator ���������
\note 
*/
void CSPVRFILEIteratorGoFirst(HCSHANDLE hIterator);

/*!
	��λ����������һ��Ԫ��
@param[in] hIterator ���������
\return �ɹ�����TRUE; ʧ�ܷ���FALSE
\note ���������ĵ�ǰԪ������ĩβԪ��ʱ����λ��һ��Ԫ��ʧ��
*/
BOOL CSPVRFILEIteratorGoNext(HCSHANDLE hIterator);

/*!
	��ȡ��������ǰԪ�ص�Pvr�ļ�
@param[in] hIterator ���������
\return ��������ǰԪ�ص�Pvr�ļ�
*/
HCSHANDLE CSPVRFILEIteratorGetCurrentFile(HCSHANDLE hIterator);

/*��
	��������ǰ�Ƿ������һ��Ԫ��
@param[in] hIterator ���������
\return ��������ǰ�����һ��Ԫ�ط���TRUE,���򷵻�FALSE
*/
BOOL CSPVRFILEIteratorIsTail(HCSHANDLE hIterator);

/*!
	���ٵ�����
@param[in] hIterator ���������
*/
void CSPVRFILEDestroyIterator(HCSHANDLE hIterator);

/*��
	�ļ��б��������͵�ö�ٶ���
*/
typedef enum _CSPVRFILESortType_E
{
	/*!����¼��ʱ������*/
	EM_PVRFILE_SORT_BY_RECTIME,
	/*!����¼���ļ��Ŀɲ���ʱ������*/
	EM_PVRFILE_SORT_BY_DUARATION,
	/*!����¼�Ƶ��û�����������*/
	EM_PVRFILE_SORT_BY_USERNAME,
	/*!����¼�Ƶ�Ƶ������������*/
	EM_PVRFILE_SORT_BY_CHNLNAME,
	/*!���ջطŴ�����������*/
	EM_PVRFILE_SORT_BY_PLAYCOUNT,
	/*!ϵͳ֧�ֵ������������*/
	EM_PVRFILE_TYPE_COUNT
}CSPVRFILESortType_E;

/*!
	������������δʵ�֣�
@param[in] hIterator �����Ŀ����������
@param[in] eSortType ����ķ�ʽ
@param[in] bIncrease �Ƿ���������
\return �ɹ�����TRUE; ʧ�ܷ���FALSE
\note ����󣬵������ĵ�ǰԪ��ָ���Pvr�ļ�����
\note ֧�ֶ������
*/
BOOL CSPVRFILEIteratorSort(HCSHANDLE hIterator, CSPVRFILESortType_E eSortType, BOOL bIncrease);

/*!
========================================= �ļ���ؽӿ� =========================================
\note ��Ӧÿ��Pvr�ļ���PVR�ڲ�����һ��Pvr�ļ�������֮��Ӧ������Pvr�ļ�����Ĵ�����PVR�ڲ�ִ�У�
\note �û�ֻ��ͨ���ļ��б��������PVR��ȡ�ļ����б༭�������û��޷�����PVR�ļ�
*/

/*!
	�ļ������ķ���ֵö�����Ͷ���
*/
typedef enum _CSPVRFILERet_E
{
	/*!�����ɹ�*/
	EM_PVRFILE_RET_SUCCESS = 0,
	/*!����ʧ��*/
	EM_PVRFILE_RET_FAILED,
	/*!��Ӧ���ļ��Ѳ�����*/
	EM_PVRFILE_RET_NOT_EXIST
}CSPVRFILERet_E;

/*!
	PVR�ļ���Ϣ
*/
typedef struct _CSPVRFILEFileInfo_S
{
	/*!¼���ļ���Ƶ������WideWord�ַ�����*/
	char		wsChnlName[CSPVR_NAME_SIZE];	
	/*!¼���ļ����û��Ѻ����ƣ�Ĭ��Ϊ[ChnlName+EventName]��WideWord�ַ���)*/
	char		wsUserName[CSPVR_NAME_SIZE];	
	/*!¼���ļ��Ŀɲ��ų���*/
	DWORD		dwDurationSec;	
	/*!¼���ļ���¼��ʱ��*/
	CSUDIPLUSOSSysTime_S sStartTime;		
}CSPVRFILEFileInfo_S;

/*!
	��ȡ�ļ���Ϣ
@param [in] hFile PVR�ļ����
@param [out] psInfo ���PVR�ļ���Ϣ
\return �������
\note
*/
CSPVRFILERet_E CSPVRFILEGetFileInfo(HCSHANDLE hFile, CSPVRFILEFileInfo_S *psInfo);

/*!
	������pvr�ļ�
\param[in] hFile PVR�ļ����
\param[out] wszUserName ���û��Զ������ƣ�WideWord�ַ�����
\return �������
*/
CSPVRFILERet_E CSPVRFILERename(HCSHANDLE hFile, const char *wsUserName);

/*!
	����ɾ�����
@param[in] hFile PVR�ļ����
@param[out] bDelFlag �µ�ɾ�����
\return �������
\note ɾ����ǲ�����籣��
*/
CSPVRFILERet_E CSPVRFILESetDelFlag(HCSHANDLE hFile, BOOL bDelFlag);

/*!
	��ȡɾ�����
@param[in] hFile PVR�ļ����
@param[out] pbOutDelFlag ����ļ���ɾ�����
\return �������
\note
*/
CSPVRFILERet_E CSPVRFILEGetDelFlag(HCSHANDLE hFile, BOOL *pbDelFlag);

/*!
	�����������
@param[in] hFile PVR�ļ����
@param[in] bLockFlag ���������
\return �������
\note ������ǻ���籣��
*/
CSPVRFILERet_E CSPVRFILESetLockFlag(HCSHANDLE hFile, BOOL bLockFlag);

/*!
	��ȡ�������
@param[in] hFile PVR�ļ����
@param[out] pbOutLockFlag ����Ƿ��������
\return �������
\note
*/
CSPVRFILERet_E CSPVRFILEGetLockFlag(HCSHANDLE hFile, BOOL *pbLockFlag);

/*!
	��ȡPVR�ļ�·��
@param[in] hFile PVR�ļ����
@param[out] szFilePath ���PVR�ļ���·����,���鳤�Ƚ�������Ϊ CSPVRFILE_PATH_NAME_LEN
@param[in] nPathLen �����ַ�������ĳ���,��������Ϊ CSPVRFILE_PATH_NAME_LEN
\return �������
\note �����PVR�ļ�·����ΪASII���ַ���
\note PVR·��������PVR�ļ��ط�
*/
CSPVRFILERet_E CSPVRFILEGetFilePath(HCSHANDLE hFile, char *szFilePath, int nPathLen);

/*!
	�ж��ļ��Ƿ�����¼����
@param[in] hFile PVR�ļ����
@param[out] pbOutIsRecording �����PVR�ļ��Ƿ�����¼��
\return 
*/
CSPVRFILERet_E CSPVRFILEIsRecording(HCSHANDLE hFile, BOOL *pbIsRecording);

/*!
	�ж��ļ��Ƿ����ڻط���
@param[in] hFile PVR�ļ����
@param[out] pbOutIsPlaying �����PVR�ļ��Ƿ����ڻط�
*/
CSPVRFILERet_E CSPVRFILEIsPlaying(HCSHANDLE hFile, BOOL *pbIsPlaying);

/*�ļ�����ģ��֧�ֵ���Ϣ֪ͨ����*/
typedef enum _CSPVRFILENotifyType_E
{
	/*!
		�ļ�����֪ͨɾ��
		\note	dwNotifyType:	EM_PVRFILE_DEL_FILE
		\note	pNotifyData:	ɾ�����Ȱٷֱ�(0--100)
		\note	hSender:		δ����
	*/
	EM_PVRFILE_DEL_PROGRESS = 0,
	/*!
		�ļ��б����֪ͨ
		\note	dwNotifyType:	EM_PVRFILE_UPDATE_ITR
		\note	pNotifyData:	δ����  
		\note	hSender:		δ����
	*/
	EM_PVRFILE_UPDATE_FILELIST,
	/*!
		�洢�豸����֪ͨ
		\note	dwNotifyType:	EM_PVRFILE_HDD_FULL
		\note	pNotifyData:	δ����  
		\note	hSender:		δ����
	*/
	EM_PVRFILE_HDD_FULL,
	/*!
		ʶ���´洢�豸֪ͨ
		\note	dwNotifyType:	EM_PVRFILE_INPLUG_STORAGE
		\note	pNotifyData:	δ����  
		\note	hSender:		δ����
	*/
	EM_PVRFILE_PLUGIN_STORAGE,
	/*!
		�洢�豸���γ�֪ͨ
		\note	dwNotifyType:	EM_PVRFILE_INPLUG_STORAGE
		\note	pNotifyData:	δ����  
		\note	hSender:		δ����
	*/
	EM_PVRFILE_PLUGOUT_STORAGE,
	/*!
		�ļ�����ģ��֪ͨ���͸���
	*/
	EM_PVRFILE_NOTIFY_TYPE_COUNT
}CSPVRFILENotifyType_E;

/*!
	��ȡ��Ϣ֪ͨ�������ľ��
\return ��Ϣ֪ͨ�������ľ��
*/
HCSHANDLE CSPVRFILEGetNotifyMgr(void);

/*!
	��ȡϵͳ��Ĭ�ϸ�Ŀ¼
\return ϵͳĬ�ϵĸ�Ŀ¼*/
const char* CSPVRFILEGetDefaultDir(void);

/*!
	ϵͳ�洢�豸״̬ö�����Ͷ���
*/
typedef enum _CSPVRFILEStorageStatus_E
{
	/*!�洢�豸������ʹ��*/
	EM_PVRFILE_STORAGE_USABLE =0,
	/*!�洢�豸������(��������TShift¼��)*/
	EM_PVRFILE_STORAGE_UNUSABLE,
	/*!�޿��õĴ洢�豸*/
	EM_PVRFILE_NO_STORAGE,
	/*!�洢�豸����(������TShift¼��)*/
	EM_PVRFILE_STORAGE_NOT_ENOUGH,
	/*!�洢�豸���ļ�ϵͳ��֧��PVR*/
	EM_PVRFILE_STORAGE_FSTYPE_NOT_SUPPORT
}CSPVRFILEStorageStatus_E;

/*!
	��ȡϵͳ�洢�豸״̬
\return ϵͳ�洢�豸״̬
*/
CSPVRFILEStorageStatus_E CSPVRFILEGetStorageStatus(void);


/*!
	��ȡ�ļ��ļ���key
@param[in] hFile PVR�ļ����
@param[out] pucCryptKey ��ż���key��buffer
@param[in] nKeySize ��ż���key��buffer��С
\return ���ؼ���key��ʵ�ʴ�С
*/
int CSPVRFILEGetEncryptKey(HCSHANDLE hFile, BYTE * pucEncryptKey, int nKeyBufferLen);

/*!
	��ȡ�ļ��ĸ�ĸ��
@param[in] hFile PVR�ļ����
\return �ļ��ĸ�ĸ��
\note ���޸�ĸ������Ϊ0
*/
int CSPVRFILEGetPgLevel(HCSHANDLE hFile);

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

