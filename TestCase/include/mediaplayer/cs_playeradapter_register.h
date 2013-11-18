/**@defgroup PlayerAdapter PlayerAdapter_Register �����˲���������ģ����,
       ����ע����صĽӿڼ��ṹ��
@brief ��ģ����Ҫ�����˲���������ģ�����ע�᷽�����ؽӿڡ�

@version 1.0.0 2011/01/11 
@{
*/

#ifndef	_CS_PLAYERADATPTER_REGISTER_H_
#define	_CS_PLAYERADATPTER_REGISTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"
#include "cs_selection.h"
#include "cs_mediaplayer.h"
#include "udi2_player.h"

/**@brief Player�������������ṹ��

����Player����㲥�ſ�����صĻص�����ԭ��.
*/
typedef struct _CSPlayerAdapterOpt_S {

	/**
	@brief ���븳ֵΪ���ṹ��Ĵ�С(sizeof(CSPlayerAdapterOpt_S))����������Ч���ж�
	@note 
	*/ 
	int  nSize;
	/**
	@brief �ж�ָ����url �Ƿ�֧�ֲ���
	@param[in] pcURL ��Ҫ���ŵ�URL��ַ
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 

	 int (*Probe)(const char * pcURL);
	/**
	@brief ����ָ����URL��һ��������.
	@param[in] pcURL ��Ҫ���ŵ�URL��ַ
	@param[in] pUserData ��Ҫ������û�����
	@param[out] phPlayer ���ش�����Player���
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 

	 int (*Open)(const char *pcURL,  HCSHANDLE hUserData, HCSHANDLE *phPlayer);

	/**
	@brief ��ʼ����.
	@param[in] hPlayer ���������
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	 int (*Start)(HCSHANDLE hPlayer);

	/**
	@brief ��ͣ����.
	@param[in] hPlayer ���������
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	 int (*Pause)(HCSHANDLE hPlayer);

	/**
	@brief �ָ�����.
	@param[in] hPlayer ���������
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	 int (*Resume)(HCSHANDLE hPlayer);

	/**
	@brief ��ת��ָ��ʱ��㲥��.
	@param[in] hPlayer ���������
	@param[in] nTime ��Ҫ��ת���Ĳ���ʱ��ʱ, ��λs
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	 int (*Seek)(HCSHANDLE hPlayer, DWORD nTime);

	/**
	@brief ���ò����ٶ�
	@param[in] hPlayer ���������
	@param[in] nTime ��Ҫ��ת���Ĳ���ʱ��ʱ, ��λs
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	 int (*SetSpeed)(HCSHANDLE hPlayer, int nRate);

	/**
	@brief ֹͣ����.
	@param[in] hPlayer ���������
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	 int (*Stop)(HCSHANDLE hPlayer);

	/**
	@brief �رղ�����.
	@param[in] hPlayer ���������
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	 int (*Close)(HCSHANDLE hPlayer);

	/**
	@brief ��ȡ��ǰ���ŵ�λ��
	@param[in] hPlayer ���������
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	int (*GetCurrentPlayTime)(HCSHANDLE hPlayer, int*pdwTime);

		/**
	@brief ��ȡ��Ŀ����ʱ��
	@param[in] hPlayer ���������
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	int (*GetDuration)(HCSHANDLE hPlayer, DWORD  *pdwDuration);


	/**
	@brief ���Ӳ��ſ�����Ϣ�ص�.
	@param[in] hPlayer ���������
	@param[in] dwUserData �û�����
	@param[in] pfCallback �ص�����ָ��
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	int (*AddCallBack)(HCSHANDLE hPlayer, DWORD dwUserData, FOnCSSELCallback fnCallback);

	/**
	@brief ɾ�����ſ�����Ϣ�ص�.
	@param[in] hPlayer ���������
	@param[in] dwUserData �û�����
	@param[in] pfCallback �ص�����ָ��
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	int (*RemoveCallBack)(HCSHANDLE hPlayer, DWORD dwUserData, FOnCSSELCallback fnCallback);

	/**
	@brief ��ȡ��������ǰ״̬
	@param[in] hPlayer ���������
	@param[out] peState ��������ǰ״ֵ̬
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	int (*GetCurrentState)(HCSHANDLE hPlayer, CSMPState_E *peState);

	/**
	@brief ���ò��ŵ���Ƶ������Ϊһ·��Ŀ���ܰ��������Ƶ����
        ���磺����������������Ӣ�����ȣ����Ը����û�������ѡ����Ҫ����
        ����Ƶ��
	@param[in] hPlayer ���������
	@param[in] nProgramId ��ĿID
	@param[in] nVideoPid ��ƵID
	@param[in] nAudioPid ��ƵID
	@param[in] nSubTitleId ��ĻID
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	int (*SwitchPidToPlay)(HCSHANDLE hPlayer, int nProgramId, int nVideoPid, int nAudioPid, int nSubTitleId);

	/**
	@brief ��ȡ��Ƶ�ļ�������
	@param[in] pcFileLocation �ļ���·��
	@param[out] pstFileInfo ��Ƶ�ļ�������ֵ
	@return �ɹ�����0, ʧ�ܷ���-1
	@note 
	*/ 
	int (*GetFileInfo)(const char *pcFileLocation, CSUDIPlayerFileInfo_S *pstFileInfo);

	/**
	@brief ��ȡ���������
	@return Adapter Player���
	@note 
	*/ 
	HCSHANDLE (*GetAdapterPlayer)(void);
}CSPlayerAdapterOpt_S;

/**
@brief ��ȡffmpeg�Ĳ�����
 

@return �ɹ�����ffmpeg �������ṹ��ָ��
@note ��������Ҫʹ��ffmpeg���䲥����ģ��ʱ, ���øú���, �������䲥����ģ��
	  ע�ᵽ������.
	  ʾ����������:
	  CSMPPlayerAdapterRegist(CSMPPlayerAdapterGetFfmpegOpts());
*/
const CSPlayerAdapterOpt_S * CSMPPlayerAdapterGetFfmpegOpts();
/**
@brief ��ȡuid2 ���ŵĲ�����
 

@return �ɹ�����uid2 �������ṹ��ָ��
@note ��������Ҫʹ��uid2���䲥����ģ��ʱ, ���øú���, �������䲥����ģ��
	  ע�ᵽ������.
	  ʾ����������:
	  CSMPPlayerAdapterRegist(CSMPPlayerAdapterGetUdi2Opts());
*/
const CSPlayerAdapterOpt_S* CSMPPlayerAdapterGetUdi2Opts();

/**
@brief �������䲥����ģ��ע�ắ��.
 
@param[in] pOpt  ���䲥��������ṹ��
@return �ɹ�����TRUE, ʧ�ܷ���FALSE
@note ��������Ҫʹ�����䲥����ģ��ʱ, ���øú���, �������䲥����ģ��
	  ע�ᵽ������.
	  ʾ����������:
	  CSMPPlayerAdapterRegist(CSMPPlayerAdapterGetUdi2Opts);
*/
BOOL CSMPPlayerAdapterRegist( const CSPlayerAdapterOpt_S  * pOpt );

/**
@brief ��ȡ�����ļ���Ϣ.
 
@param[in] pszLocator  �����ļ���url
@param[out] pstFileInfo  �����ļ���Ϣ
@return �ɹ�����TRUE, ʧ�ܷ���FALSE
*/
BOOL CSPlayerAdapterGetFileInfo(const char *pszLocator, CSUDIPlayerFileInfo_S *pstFileInfo);

#ifdef __cplusplus
	}
#endif

#endif /* _CS_PLAYERADATPTER_REGISTER_H_ */


