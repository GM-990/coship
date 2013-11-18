/**
@defgroup cs_adssys cs_adssys-���ϵͳ�ն�������ȡ����
@{
*/

#ifndef _CS_ADSSYS_H_
#define _CS_ADSSYS_H_

#include "cs_adsdata.h"


#ifdef __cplusplus
extern "C" {
#endif


///ȫ�ֵ�service ID
#define CS_ADSSYS_GLOBAL_SERVICE_ID 0xffff

///ֹͣģʽ@see CSAdsSys_Stop
typedef enum
{
	EM_ADSSYS_STOPMODE_CLEAR_NONE,///<ֹͣ���չ�����ݣ����ǲ��������
	EM_ADSSYS_STOPMODE_CLEAR_DATA///<ֹͣ���չ��������ͬʱ�������
}CSAdsSysStopMode_E; 

///�����������ģ��ʵ���Ĳ���@see CSAdsSys_Create
typedef struct
{
	CSAdsSysResolution_E eResolution;	///< �ɽ��յ����ݵĸ߱�������
	WORD wPid;	///< ����PID
	BYTE ucTableId;	///< ����ID
}CSAdsSysParam_S;

///CSAdsSys_Start�Ĳ���@see CSAdsSys_Start
typedef struct
{
	DWORD dwDemuxId; ///< demux��ţ���demux/��tuner�Ļ�����ͨ��Ϊ0
	CSAdsSysService_S sService; ///< Ƶ��
}CSAdsSysStart_S;

/**
���ģ���¼��ص�����
@param psEvent ����������¼�
@param pUserData ����������û�ע��ص����Զ������
@param pSender ����������¼��ķ�����(CSAdsSys_Create�ķ���ֵ)
@note �����ڱ��ص���ִ�й�����룬�������¼��������̴߳���
*/
typedef void (*CSOnAdsSysCallback_F)(CSAdsSysEvent_S * psEvent, void *pUserData, void* pSender);

typedef void (*CSOnAdsSmartCardCallBack_F)(void *pUserData);

///CSAdsSys_SetCommonParam�Ĳ���@see CSAdsSys_SetCommonParam
typedef struct
{
	/**
	���ģ���ȡʱ��Ļص�����������Ҫ��
	@param bOffset ����������Ƿ�Ҫ��ʱ��ƫ��,TRUEҪ����FALSE����
	@param psDateTime �����������ǰʱ��
	*/	
	void (*fnGetDateTime)(BOOL bOffset, CSUDIPLUSOSSysTime_S *psDateTime);
	/**
	���ģ������ڴ�Ļص�������Ҫ��
	@param nSize ���������Ҫ������ڴ��С
	@return ������ڴ��ַ
	*/
	void* (*fnMalloc)(int nSize);
	/**
	���ģ���ͷ��ڴ�Ļص�
	@param p ���������Ҫ�ͷŵ��ڴ�
	*/	
	void (*fnFree)(void *p);
	 /**
	 ���еĹ������������õ��ڴ�
	 */
	int nMaxMemorySizeLimite;
}CSAdsSysCommonParam_S;

/**
@brief �趨���еĹ��������Ĺ�������
@param psParam �����������������
@note ������CSAdsSys_Create֮ǰ���ã���ֻ�ܱ�����һ��
*/
void CSAdsSys_SetCommonParam(CSAdsSysCommonParam_S *psParam);


/**
@brief ����һ��������������
@param psParam �����������ʼ������Ĳ���
@retval һ��������������ʧ�ܷ���NULL
*/
void* CSAdsSys_Create(CSAdsSysParam_S *psParam);

/**
@brief ����һ��������������
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@retval �ɹ�ΪTRUE��ʧ��ΪFALSE
*/
BOOL CSAdsSys_Destroy(void* pASObj);

/**
@brief ����stb id
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param pucStbId ���������������ID��ͨ��ΪSmart����
@param nSize ���������pucStbId���ֽڸ���
@retval �ɹ�ΪTRUE��ʧ��ΪFALSE
@note ��CSAdsSys_Create�����̵���
*/
BOOL CSAdsSys_SetStbId(void* pASObj, BYTE *pucStbId, int nSize);

/**
@brief ���ûص��¼�֪ͨ
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param fnCallback ����������ͻ��˵Ļص�����
@param pUserData ����������Զ������ݣ��ڻص������лش����û�
*/
void CSAdsSys_RegisterCallback(void* pASObj, CSOnAdsSysCallback_F fnCallback, void *pUserData);

/**
@brief ��ȡCSAdsSys_RegisterCallback��pUserDataֵ
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@retval �û��Զ������,CSAdsSys_RegisterCallback�е�pUserDataֵ
*/
void* CSAdsSys_GetUserData(void* pASObj);

/**
@brief ��ʼ���ؽ���ĳ����Ŀ�Ĺ������
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param psStart ���������ָ����ʼ���صĽ�Ŀ����,�ý�Ŀ����Ϊ��ǰ��Ŀ
@retval �ɹ�ΪTRUE��ʧ��ΪFALSE
@note ͨ���ڻ�̨����ã�ע�⻻ֻ̨��Ҫ���ñ��������ɣ�ͨ�����õ���CSAdsSys_Stop
@note ÿ�ε��ñ�API���ᷢ��EM_ADSSYS_CURRENT_CONTENT_CHANGE_EVENT
@code
	CSAdsSysStart_S sStart;
	sStart.sService.wOnId = 100;
	sStart.sService.wTsId = 1;
	sStart.sService.wServiceId= 4;
	sStart.dwDemuxId = 0;
	USPPrint(ADS_DEMO_LEVEL, "CSAdsSys_Start_____________________________________________\n");
	bRet = CSAdsSys_Start(psAS, &sStart);
@endcode
*/
BOOL CSAdsSys_Start(void* pASObj, CSAdsSysStart_S *psStart);

/**
@brief ��ʼ���ؽ���ĳ����Ŀ�Ĺ������
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param psStart ���������ָ����ʼ���صĽ�Ŀ����,�ý�Ŀ����Ϊ��ǰ��Ŀ
@retval �ɹ�ΪTRUE��ʧ��ΪFALSE
@note ������ִ����CSAdsSys_Start��ͬ�Ĺ��ܣ������ᷢ��EM_ADSSYS_CURRENT_CONTENT_CHANGE_EVENT
*/
BOOL CSAdsSys_StartStilly(void* pASObj, CSAdsSysStart_S *psStart);

/**
@brief ֹͣ������ݵļ���
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param eStopMode ���������ָ��ֹͣģʽ
@retval �ɹ�ΪTRUE��ʧ��ΪFALSE
@note ��̨(����Ƶ��仯�����)��ͨ�����õ���CSAdsSys_Stop������ȷ�б�Ҫֹͣ�������/���ݰ汾���ӣ���ɵ��á�
*/
BOOL CSAdsSys_Stop(void* pASObj, CSAdsSysStopMode_E eStopMode);

/**
@brief ��ȡ����е��ı���Ϣ��Block����
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param ucTypeIndex ���������_CSADS_IMAGE_TYPE_CONST������Ĺ��λ�ñ�ʶ���
@param psSrvTime ���������ָ����Ŀ��ʱ�̣�ΪNULL��ʹ�õ�ǰʱ��͵�ǰ��Ŀ
@param psDisplayDscrpt ��������������ʼʱ�䡢����ʱ�����Ϣ
@retval һ���ı���������ΪNULL��ʾû��
@note ʹ����Ϻ󣬱������ϵ���һ��CSAdsSys_ReturnBlock�黹�ö���
@see _CSADSSYS_IMAGE_TYPE_INDEX_CONST
*/
CSAdsSysTextBlock_S*  CSAdsSys_TakeTextBlock(void* pASObj, BYTE ucTypeIndex, CSAdsSysDisplay_S *psSrvTime, CSAdsSysDisplayDscrpt_S* psDisplayDscrpt);

/**
@brief ��ȡ����е�ͼƬ��Ϣ��Block����
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param ucTypeIndex ���������_CSADS_IMAGE_TYPE_CONST������Ĺ��λ�ñ�ʶ���
@param psSrvTime ���������ָ����Ŀ��ʱ�̣�ΪNULL��ʹ�õ�ǰʱ��͵�ǰ��Ŀ
@param psDisplayDscrpt ��������������ʼʱ�䡢����ʱ�����Ϣ
@retval һ��ͼƬ��������ΪNULL��ʾû��
@note ʹ����Ϻ󣬱������ϵ���һ��CSAdsSys_ReturnBlock�黹�ö���
@see _CSADSSYS_IMAGE_TYPE_INDEX_CONST
*/
CSAdsSysImageBlock_S*  CSAdsSys_TakeImageBlock(void* pASObj, BYTE ucTypeIndex, CSAdsSysDisplay_S *psSrvTime, CSAdsSysDisplayDscrpt_S* psDisplayDscrpt);

/**
@brief ��ȡ����е�Video��Ϣ��Block����
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param ucTypeIndex ���������_CSADS_IMAGE_TYPE_CONST������Ĺ��λ�ñ�ʶ���
@param psSrvTime ���������ָ����Ŀ��ʱ�̣�ΪNULL��ʹ�õ�ǰʱ��͵�ǰ��Ŀ
@param psDisplayDscrpt ��������������ʼʱ�䡢����ʱ�����Ϣ
@retval һ��ͼƬ��������ΪNULL��ʾû��
@note ʹ����Ϻ󣬱������ϵ���һ��CSAdsSys_ReturnBlock�黹�ö���
@see _CSADSSYS_IMAGE_TYPE_INDEX_CONST
*/
CSAdsSysVideoBlock_S*  CSAdsSys_TakeVideoBlock(void* pASObj, BYTE ucTypeIndex, CSAdsSysDisplay_S *psSrvTime, CSAdsSysDisplayDscrpt_S* psDisplayDscrpt);

/**
@brief ��ȡ����е��ı���Ϣ��Block����
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param dwBlockId ������������ݿ�ID
@retval һ���ı���������ΪNULL��ʾû��
@note ʹ����Ϻ󣬱������ϵ���һ��CSAdsSys_ReturnBlock�黹�ö���
*/
CSAdsSysTextBlock_S*  CSAdsSys_TakeTextBlockById(void* pASObj, DWORD dwBlockId);

/**
@brief ��ȡ����е�ͼƬ��Ϣ��Block����
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param dwBlockId ������������ݿ�ID
@retval һ��ͼƬ��������ΪNULL��ʾû��
@note ʹ����Ϻ󣬱������ϵ���һ��CSAdsSys_ReturnBlock�黹�ö���
*/
CSAdsSysImageBlock_S*  CSAdsSys_TakeImageBlockById(void* pASObj, DWORD dwBlockId);

/**
@brief ��ȡ����е�Video��Ϣ��Block����
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param dwBlockId ������������ݿ�ID
@retval һ��ͼƬ��������ΪNULL��ʾû��
@note ʹ����Ϻ󣬱������ϵ���һ��CSAdsSys_ReturnBlock�黹�ö���
*/
CSAdsSysVideoBlock_S*  CSAdsSys_TakeVideoBlockById(void* pASObj, DWORD dwBlockId);

/**
@brief ǿ���ͷ�ĳ��Block��ռ�õ��ڴ棬ͨ�������ͷ���Ҫ�洢��flash��block
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param dwBlockId ������������ݿ�ID
@retval TRUE�ɹ�,FALSEʧ��
@note ��ȷ���ڵ��ñ�����֮ǰ������ʹ��block������,������CSAdsSys_ReturnBlock֮�����
*/
BOOL CSAdsSys_ForceFreeBlockById(void* pASObj, DWORD dwBlockId);

/**
@brief ͬ����block���Զ�Ӧ������λ��ÿ�����λ��location�ǲ�ͬ�ġ�
��������block�����л�ȡucTypeIndexָ����location��������
ͨ����Take֮�����
@param psBlock ���������һ�����ݿ�
@param ucTypeIndex ������������λ���
@param pbExsit ����������Ƿ���ڸ�������
@param psLocationDscrpt ���������location��������
@retval TRUE�����ɹ���FALSEʧ��
@code
CSAdsSysTextBlock_S *psTxtBlk;
psTxtBlk = CSAdsSys_TakeTextBlock(pASObj, CS_ADSSYS_FLOAT_TEXT, NULL, NULL);
if(psTxtBlk != NULL)
{
	BOOL bExist;
	CSAdsSysLocationDscrpt_S sLocation;
	CSAdsSys_GetLocationDscrpt((CSAdsSysBlock_S *)psTxtBlk, CS_ADSSYS_FLOAT_TEXT, &bExist, &sLocation);
}
CSAdsSys_Return(psTxtBlk);
@endcode
*/
BOOL CSAdsSys_GetLocationDscrpt(CSAdsSysBlock_S* psBlock, BYTE ucTypeIndex,BOOL *pbExsit, CSAdsSysLocationDscrpt_S *psLocationDscrpt);


/**
@brief ͬ����block���Զ�Ӧ������λ��ÿ�����λ�Ĳ�ͬ�İ������Թ�����ͬ��Ӧ��
��������block�����л�ȡucTypeIndex��wKeyCodeָ����Ӧ�����ơ�
ͨ����Take֮�����
@param psBlock ���������һ�����ݿ�
@param ucTypeIndex ������������λ���
@asActivationInfoArray ���������Ӧ����Ϣ(Ӧ�����Ʒ�����ڴ����>= 256Bytes)
@pnArraySize ����������� ���ṹ��ĸ���
@retval TRUE�����ɹ���FALSEʧ��
@note �ͻ��˴���������256�ֽ������Ӧ������
@code
CSAdsSysImageBlock_S psImgBlk;
psImgBlk = CSAdsSys_TakeImageBlock(pASObj, CS_ADSSYS_CHANNEL_BAR, NULL, NULL);
if(psImgBlk != NULL)
{
	CSAdsSysActivationDscrpt_S asActivationInfoArray[4];
	memset(&asActivationInfoArray,0,sizeof(asActivationInfoArray));
	int pnArraySize = 4;
	CSAdsSys_GetLocationDscrpt((CSAdsSysBlock_S *)psImgBlk, CS_ADSSYS_CHANNEL_BAR,asActivationInfoArray,&pnArraySize);
}
CSAdsSys_Return(psImgBlk);
@endcode
*/
BOOL CSAdsSys_GetActivationDscrpt(CSAdsSysBlock_S* psBlock, BYTE ucTypeIndex, CSAdsSysActivationDscrpt_S *asActivationInfoArray,int *pnArraySize);

/**
@brief ��ȡblock�е�Triger������,tag 0xb7
@param psBlock ���������һ�����ݿ�
@param pbExsit ����������Ƿ����Ӧ������
@param psTrigerDscrpt ���������Triger����������
@retval TRUE�����ɹ���FALSEʧ��
@code
CSAdsSysImageBlock_S *psImgBlk;
psImgBlk = CSAdsSys_TakeImageBlock(pASObj, CS_ADSSYS_FLOAT_IMAGE1, NULL, NULL);
if(psImgBlk != NULL)
{
	CSAdsSysTrigerDscrpt_S sTrigerDscrpt
	CSAdsSys_GetLocationDscrpt((CSAdsSysBlock_S *)psImgBlk,  &bExist, &sTrigerDscrpt);
}
CSAdsSys_Return(psImgBlk);
@endcode
*/
BOOL CSAdsSys_GetTrigerDscrpt(CSAdsSysBlock_S* psBlock, BOOL *pbExsit, CSAdsSysTrigerDscrpt_S *psTrigerDscrpt);

/**
@brief �黹�ӹ���������ȡ�ĸ���Block����
@param pBlock �������������CSAdsSys_TakeXXXX����ȡ��ͼƬ���ı�����
@retval TRUE�����ɹ���FALSEʧ��
@note ÿһ�ε���CSAdsSys_TakeXXXX���������Ӧ����һ��CSAdsSys_ReturnBlock����������ڴ�й©
*/
BOOL CSAdsSys_ReturnBlock(void *pBlock);

/**
@brief ��ȡ�������ݿ��е�Descriptors
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param psInfo ��������� �������ݿ��е�Descriptors ����
@retval TRUE�����ɹ���FALSEʧ��
@note ����EM_ADSSYS_DESCRIPTOR_LOOP_COMPLETE�¼������
@see EM_ADSSYS_DESCRIPTOR_LOOP_COMPLETE
@code
case EM_ADSSYS_DESCRIPTOR_LOOP_COMPLETE:
{
	CSAdsSysDescriptorLoop_S sInfo;
	bRet = CSAdsSys_GetDescriptorLoopInfo(psAS, &sInfo);
	if(CSASSERT_FAILED(bRet)) return;

	if(sInfo.bExistAppDispTimeout)
	{
		USPPrint(ADS_DEMO_LEVEL, "descriptor loop,find app disptimeout\n");
	}
	if(sInfo.bExistInvalidation)
	{
		USPPrint(ADS_DEMO_LEVEL, "descriptor loop,find invalidation,DispLocationMask=%xh\n", sInfo.sInvalidation.dwImgDispLocationMask);
	}
	if(sInfo.bExistMainDeliver)
	{
		USPPrint(ADS_DEMO_LEVEL, "descriptor loop,find main deliver,freq=%xh\n", sInfo.sMainDeliver.unDeliver.m_CableDeliver.m_dwFrequency);
	}				
}
break;	
@endcode
 */
 BOOL CSAdsSys_GetDescriptorLoopInfo(void* pASObj, CSAdsSysDescriptorLoop_S *psInfo);

/**
@brief ��ȡĳ��block tag��Ӧ����Ҫ�洢��һϵ��BlockId
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param ucBlockTag �����������Tag���ɷ��ʳ���CS_ADSSYS_TAGS���
@param psStoredId ���������Ҫ�洢��BlockId
@retval TRUE�����ɹ���FALSEʧ��
@note ����ȡ��EM_ADSSYS_SCAN_COMPLETE�¼���ͨ�����ø�api���Ի�ȡ��Ҫ�洢��block
@see EM_ADSSYS_SCAN_COMPLETE
@code
case EM_ADSSYS_SCAN_COMPLETE:
{
	CSAdsSysBlockStoredId_S sStoredId;
	int i;
	CSAdsSys_PrintDebugInfo(psAS);
	bRet = CSAdsSys_GetStoredBlockIds(psAS,  CS_ADSSYS_IMAGE_BLOCK_TAG, &sStoredId);
	if(CSASSERT_FAILED(bRet)) return;

	for(i=0; i<sStoredId.nValidCount; ++i)
	{
		CSAdsSysImageBlock_S *psImgBlk;
		USPPrint(ADS_DEMO_LEVEL, "stored block id=%d, typeidx=%d\n", sStoredId.adwBlockId[i], sStoredId.aucTypeIndex[i]);
		psImgBlk = CSAdsSys_TakeImageBlockById(psAS, sStoredId.adwBlockId[i]);
		if(psImgBlk != NULL)
		{
			///save psImgBlk->pucImageData to flash
		}
		CSAdsSys_ReturnBlock(psImgBlk);
	}		
}
break;
@endcode
 */
BOOL CSAdsSys_GetStoredBlockIds(void* pASObj, BYTE ucBlockTag, CSAdsSysBlockStoredId_S *psStoredId);

#if 0
/**
@brief ���ݳ�������(Triger Descriptor)��ʼ���ӵ�ǰƵ���Ĺ��λ���ݡ�
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param dwTypeMask ������������λ�����룬ָ����Ҫ������Щ���λ,0ֹͣ����
@retval TRUE-�����ɹ�,FALSE-����ʧ��
@note ����Triger�����������ڣ�������ø�API����ܻ�����ݿ飬û��Triger������
�����ڣ����Ըú����ĵ��á�
�����õ�dwTypeMask���滻��һ�����õĹ��λ����Ϊ0��ֹͣ���й��λ�ļ��ӡ�
�����ڽ���ĳ�������ʱ�򣬸��ݸý������еĹ��λ���ø�API�����������
�Ĺҽ�ͼƬ�͸�����Ļ�Ĵ�������,
@code
CSAdsSys_MonitorTriger(pASObj,  
	(1<<CS_ADSSYS_FLOAT_TEXT) |
	(1<<CS_ADSSYS_FLOAT_IMAGE1) |
	(1<<CS_ADSSYS_FLOAT_IMAGE2) |
	(1<<CS_ADSSYS_FLOAT_IMAGE3) |
	(1<<CS_ADSSYS_FLOAT_IMAGE4));
@endcode
ֻ����ӵ�ǰ��Ƶ����صĹ��λ�����ø�API�󣬼��ӵ���ʼʱ�䱻��Ϊ��ǰʱ�䣬
�����ӵ�����������ʾ��Ȼ������ʧ��Ȼ������ʾ���Դ����ơ�
*/
BOOL CSAdsSys_MonitorTriger(void* pASObj, DWORD dwTypeMask);
#endif

/**
@brief ��ӡ�ڲ�������Ϣ
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@note ���λ��ӡ�������룬��Ϊһ�����ݿ���Զ�Ӧ������λ
*/
void CSAdsSys_PrintDebugInfo(void* pASObj);

/**
@brief ���ù�����ɷ�����ڴ�
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param nMaxSize �������֧�ֵ��ڴ��С
@retval TRUE�����ɹ���FALSEʧ��
@note ȷ������ʱ���ɹ����������Ѿ�ʹ�õ��ڴ���������õ��ڴ��С�����ý���ʧ��
*/
BOOL CSAdsSys_SetMaxMemSize(void *pASObj, int nMaxSize);

/**
@brief ���ù����Ĵ���ʱ��
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param nTime ����ʱ�䣬��λ:��
@retval TRUE�����ɹ���FALSEʧ��
@note ȷ������ʱ���ɹ���
*/
BOOL CSAdsSys_SetCheckTrigeTime(void *pASObj, BYTE nTime);
	
/**
@brief ��ȡ�汾
@retval �汾��
*/
const char* CSAdsSys_GetVersion(void);

void CSAdsSys_SetAdsFilterMask(void *pASObj,  DWORD dwMask);

DWORD CSAdsSys_GetAdsFilterMask(void *pASObj);



/**
@brief ��ȡĳ�����λ��ĳ��Ƶ�������ж�Ӧ��ĳ�����͵Ĺ������й��������ݿ��id
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param ucTypeIndex �������, ���λ���
@param psService Ƶ����Ϣ������NULLʱ��Ĭ��Ϊ��ǰ��Ŀ
@param psShareId �����������¼�������������й��������ݿ��id
@retval TRUE�����ɹ���FALSEʧ��
@note ȷ������ʱ���ɹ���
*/
BOOL CSAdsSys_GetShareBlockIds(void *pASObj, BYTE ucTypeIndex, CSAdsSysService_S *psService, CSAdsSysBlockShareId_S* psShareId); 



/**
@brief ��ù������������Ϣ tag=0xba
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param psBlock ����������blockԭʼ����
@param ucTypeIndex ������������λ���
@param psService  ���������Ƶ����Ϣ������NULLʱ��Ĭ��Ϊ��ǰ��Ŀ
@param pbExsit �������, �Ƿ���ڸ�����
@param psGroupDscrpt �����������¼�������������Ϣ
@retval TRUE�����ɹ���FALSEʧ��
@note ȷ������ʱ���ɹ���
*/
BOOL CSAdsSys_GetGroupDscrpt(void *pASObj, CSAdsSysBlock_S *psBlock, BYTE ucTypeIndex, CSAdsSysService_S *psService, BOOL *pbExsit, CSAdsSysGroupDescriptor_S* psGroupDscrpt);


/**
@brief ͨ������Ψһid����ȡ�������
@param pASObj ���������CSAdsSys_Create�������ĵĹ�������
@param dwIdentificationid �������������Ψһid��ͨ������Ψһ�����ӻ��
@retval һ��ͼƬ��������ΪNULL��ʾû��
@note ʹ����Ϻ󣬱������ϵ���һ��CSAdsSys_ReturnBlock�黹�ö���

*/
CSAdsSysImageBlock_S*  CSAdsSys_TakeImageBlockByIdentificationId(void* pASObj, DWORD dwIdentificationId);


#ifdef __cplusplus
}
#endif

#endif

/**
@}
*/
