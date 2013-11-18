#ifndef _CS_SYSAUDIO_H_
#define _CS_SYSAUDIO_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "udi_audio.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ����*/
typedef enum _CSSYSAudioOpenType
{
	CS_MODE_AUD_UNMUTE,			/**< ȡ������*/
	CS_MODE_AUD_MUTE			/**< ����*/
}CSSYSAudioOpenType;

/**@brief ��Ƶ�¼����� */
 typedef enum _CSSYSAUDEvent_E
{
	EM_AUD_NO_EVT			=0x00000000,	/**< û���κ��·���*/
	EM_AUD_SOURCE_CHANGED_EVT	=0x00000001,	/**< ����Դ�����仯*/
	EM_AUD_OVERFLOW_EVT		=0x00000002,	/**< �������*/
	EM_AUD_UNDERFLOW_EVT		=0x00000004,	/**< �������ݺľ�����û�����ݲ���*/
	EM_AUD_PAKET_START 		=0x000000008,	/**< todo �����ע��*/
	EM_AUD_PAKET_STOP 		=0x000000010,	/**< todo �����ע��*/
	EM_AUD_ALL_EVT			=0x0000001f	/**< �¼�����*/
}CSSYSAUDEvent_E;

/**
@brief audioģ��ص�����

@param[in] dwEvent �������¼���CSSYSAUDEvent_E�е�һ���¼������
@param[in] pvParam ��ʱ������Ŀǰ��NULL
@param[in] pvUserData �û�����CSSYSAUDRegisterEventע��ʱ���Զ�������pvUserData
@note ���û�ע����¼������Ǳ�����
*/
typedef void (*FAudioCallback)(DWORD dwEvent,PVOID pvParam,PVOID pvUserData);


/**
@brief AudioDecoder��Ҫ���ݵĻص�����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������������ο��豸�����ӿ��ĵ�����
@param[in] pvParam ��ʱ������Ŀǰ����ΪNULL
@note �ýӿ�ʵ������Ƶ����Trick Modeʱ���õĺ���������֧�ֻطź�ʱ�ơ�ʵʱ������			�˺�����Ч��
@note		struct _OnAudioNeedData_S
@note		{ 
@note		int    nJumpTimeM;      nJumpTimeMs ��Ծ���
@note	    UINT  uNeedDataSize;  uNeedDataSize ����ע�����������BYTEΪ��λ
@note       void *  pUserData;     pUserData �û��Զ������
@note		}OnAudioNeedData_S 
@note		pvParam = &(OnAudioNeedData_S);
@note nJumpTimeMs��ָ��һ��ע�������β���뱾��ע������ͷ�м��ʱ���������ڻطŵ�Trick��
@note �����ø����ݾ�����ȡ���ݵ�λ�ã���ΪֵΪ0��ʾ��������ע�롣
*/
typedef void(*FOnAudioNeedData)(HCSHANDLE hHandle,  PVOID pvParam);

/**
@brief AudioDecoder����һ����֡�Ļص�����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������������ο��豸�����ӿ��ĵ����塣
@param[in] pUserData �û��Զ��������Ŀǰ����ΪNULL
@note ���û�ע����¼������Ǳ�����
*/
typedef void(*FOnAudFrameCome)(HCSHANDLE hHandle, PVOID pUserData);



/*----------------------------------��Ƶ��ػص�������----------------------------------*/

/**
@brief ע��AudioDecoder��֡�Ļص���

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] fnCallback �ص�
@param[in] hReserved �û��Զ������
*/
void CSSYSAUDRegNewFrameCome(HCSHANDLE hHandle, FOnAudFrameCome fnCallback, HCSHANDLE hReserved);

/**
@brief ͨ����ͬ�¼���������Ƶ����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] enmEventMsg ��Ӧ���¼����͡��ο� SYS Event Msg �Ķ��塣
@param[in] enmAudioOpenType  ������Ƶ��������͡��򿪻��ǽ���
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSYSOpenAudioByEventMsg( HCSHANDLE hHandle, CSSYSEventMsg enmEventMsg, CSSYSAudioOpenType enmAudioOpenType );

/**
@brief ���ö�audio������������õ��¼���Mask�����mask�ɿ������¼�������

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] nEvent ��Ӧ���¼����ͣ��쳣�������¼�����Tuner�¼�����CSSYSEVENT_SIGNAL_LOCK��CSSYSEVENT_SIGNAL_LOST����
@param[in] dwMask �¼���Mask������˵�����ԣ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ʹ�øýӿڿ���ʵ�ֽ��û�����һ��Event���Ͷ�audio���صĿ��ƣ�Ҳ�������ø�Event�����Ƿ��Ŀ��ء�
@note һ��Event���ͽ���֮���������Ը�Event����CSSYSOpenAudioByEventMsg
@note ��Щ�¼���״̬��Ȼ�ᱻ��¼������������Ƶ�Ŀ��ؿ��ơ�
@note ����һ���¼����û����ã���Ƶ�Ŀ���״̬�����������ı䣬
@note ֱ��CSSYSOpenAudioByEventMsg�����á�
*/
BOOL CSSYSOpenAudioSetEventMask(HCSHANDLE hHandle, int nEvent, DWORD dwMask); 

/**
@brief ��ȡ��audio������������õ��¼���Mask

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] nEvent 	��Ӧ���¼����ͣ��쳣�������¼�����Tuner�¼�����CSSYSEVENT_SIGNAL_LOCK��CSSYSEVENT_SIGNAL_LOST����
@return  �¼���Mask������˵�����ԣ�
*/
DWORD CSSYSOpenAudioGetEventMask(HCSHANDLE hHandle, int nEvent); 

/**
@brief ע��audioģ���Ӧ�Ļص�����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] fnCallback �ص�����������Ϊ��
@param[in] pvUserData �û��Զ������ݣ�����fnCallback��������ʱ��pvUserData���ظ��û�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ɱ���ε��ã������һ�ε��ý��Ϊ׼��fnCallbackΪNULLΪ��ע��
*/
BOOL CSSYSAUDRegisterEvent(HCSHANDLE hHandle,FAudioCallback fnCallback,PVOID pvUserData);

/**
@brief ����Auideoģ���ص��¼�

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] dwEventMask ���õ�ǰģ��Ҫ��ص��¼�,ΪCSSYSAUDEvent_E��ö�ٱ�������ϻ�ֵ
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ����˵��:���dwEventMask=EM_AUD_SOURCE_CHANGED_EVT|EM_AUD_OVERFLOW_EVT,
@note ��ô������ĿԴ�����仯����audio buffer ovferlow��CSSYSAUDRegisterEvent����ע��Ļص������ᱻ����
*/
BOOL CSSYSAUIDSetEventMask(HCSHANDLE hHandle,DWORD dwEventMask);

/**
@brief ��ȡAudioģ���¼��������

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[out]  pdwEventMask ��ǰAudioģ���ص��¼���ΪCSSYSAUDEvent_E��ö�ٱ�������ϻ�ֵ
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ӦλΪ1����˵��ϵͳ���ڼ�ظ��¼�
*/
BOOL CSSYSAUDGetEventMask(HCSHANDLE hHandle,DWORD* pdwEventMask);






/**
@brief ��ʼ��MP3�����ģ��
@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������������ο��豸�����ӿ��ĵ����塣
@param[in]  userData �������ݣ���չ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSYSAUDSoftMP3Init(HCSHANDLE hHandle,HCSHANDLE userData);

/*----------------------------------ESע��ӿ�----------------------------------*/

/**
@brief ΪAudio ESע������ʼ������

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] pstESBufferInfo Audio ESע���Buffer����ص���Ϣ��������ƵBuffer�ĵ�ַ�ʹ�С�ȡ��ο�Audio ES  Buffer Info ���ݽṹ�� 
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��ʼ��ESע������Ҫ��Audio Buffer����ص���Ϣ��������buffe����Сͨ����ʼ���������룬Ĭ����Ƶλ64k��
*/
BOOL CSAUDESInit(HCSHANDLE hHandle, CSESBufferInfo* pstESBufferInfo);

/**
@brief ��ֹAUD ES��ע��

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()������� 
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��ֹAUD ES��ע�룬������buffer��
*/
BOOL CSAUDESTerm(HCSHANDLE hHandle);

/**
@brief ���ESע��AudioBuffer�Ŀ��пռ�Ĵ�С

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] pnBufSize ESע������AudioBuffer�Ĵ�С��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSESGetAudioBufferFreeSize(HCSHANDLE hHandle, int * pnBufSize);

/**
@brief ���ע��ռ����ʼ��ַ�ʹ�С

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] nBufSize Ҫ��ȡ��AudioBuffer�Ĵ�С
@param[out] ppucBuffer ESע�������AudioBuffer�ĵ�ַ
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSESGetAvailAudioBuffer(HCSHANDLE hHandle, int  nBufSize, PBYTE *  ppucBuffer);

/**
@brief ���ESע�������AudioBuffer

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSESAudioBufferReset(HCSHANDLE hHandle);

/**
@brief ��Audiobuffer��д������

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] pucBuffer Ҫ����ESע���audio���ݵĴ�ŵĵ�ַ��
@param[in] nBufSize Ҫ����ESע���audio���ݵĴ�С��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSESAudioBufferPut(HCSHANDLE  hHandle,  PBYTE pucBuffer, int nBufSize);

/**
@brief ����Audio����ע���ͬ����Ϣ

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] nFirstInjectSize ��һ�ν���ESע���Audio���ݵĴ�С
@param[in] nFirstMilliSeconds ��һ�ν���Audio����ESע���ʱ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSESSetAudioBufferSyncInfo(HCSHANDLE hHandle,  int  nFirstInjectSize,int  nFirstMilliSeconds);

/**
@brief ���ע���AV ES��PTS����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] syncInfo  ���صĵİ���Audio ES����PTS���顣
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSESGetAudioCurrentPTS(HCSHANDLE hHandle, CSESSyncInfoEx* syncInfo);

/**
@brief ����ES��ע�����Ƶ�������ģУԣ�

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] un64PTS  [in]: �����õ���clock_t���͵���Ƶ���Уԣ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ú����ӿ�����IPTV�ģţ���ע��ͬ������.
*/
BOOL CSESSetAudioPTSEx(HCSHANDLE hHandle, U64CLock_t un64PTS);

/**
@brief ��ȡ��ǰע���ES�����Ƿ������Ƶ��

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ú����ӿ�����IPTV��
*/
BOOL CSESIsHaveAudio(HCSHANDLE hHandle );

/**
@brief ��֪��Ƶ����������ע�������ͷ���ϴ�ע�������β�Ƿ��ǲ�����ע��

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] bDiscontinuity TRUE��������FALSE����
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ�ʵ������Ƶ����Trick Modeʱ���õĺ���������֧�ֻطź�ʱ�ơ�ʵʱ�����Ŵ˺�����Ч��
*/
BOOL CSSYSAUDDiscontinuity(HCSHANDLE hHandle, BOOL bDiscontinuity);

/**
@brief �ڽ��յ�FOnAudioNeedData�ص�ע�����ݺ󣬵��øú�����֪��Ƶ������ע����ɡ�

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ�ʵ������Ƶ����Trick Modeʱ���õĺ���������֧�ֻطź�ʱ�ơ�ʵʱ�����Ŵ˺�����Ч��
*/
BOOL CSSYSAUDInjectComplete(HCSHANDLE hHandle);

/**
@brief ����ES����ע�����Ƶ�ص�������

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] pfnCallback �ϲ�ע������ڻ��decoder ��Ϣ�ص�����ָ�롣
@param[in] pvUserData �ṩ���ص������Ĳ���ָ�롣
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ûص���������IPTVӦ�ã���Ҫ����֪ͨ STB IP�˿ͻ���ES��Decoder������״̬�����������磬
@note ����ȣ��Ա�IP�ͻ�����ǰ��IPTV����������Э��������ǰ��IP���ݷ��͵����ʣ��ﵽ�����Ĳ���Ч����
*/
BOOL CSESSetAUDDataInjectCallback(HCSHANDLE hHandle, CSESDataInjectCallback pfnCallback, PVOID pvUserData);

/**
@brief ע��AudioDecoder��Ҫ���ݵĻص�������

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] fnCallback �ص�
@param[in] hReserved �û��Զ������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ�ʵ������Ƶ����Trick Modeʱ���õĺ���������֧�ֻطź�ʱ�ơ�ʵʱ�����Ŵ˺�����Ч��
*/
void CSSYSAUDRegNeedDataCallback(HCSHANDLE hHandle, FOnAudioNeedData fnCallback, HCSHANDLE hReserved);

/**
@brief ������������豸

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] dwOutputDevice ��Ƶ����˿ڣ���ΪCSAUDOutputDevice�и������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���û��趨���ú�����Ƶ����豸
*/
BOOL CSSYSAUDSetOutputDevice(HCSHANDLE hHandle, DWORD dwOutputDevice, HCSHANDLE hReserved);

/**@brief ���Audio Decode Buffer�е�����

@param[in] hHandle[in]: �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������������ο��豸�����ӿ��ĵ�����
@param[in] hReserved[in]: �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�Ĭ��ֵ��ΪNULL
@return �����ɹ�����TRUE�� ����ʧ�ܷ���FALSE
*/
BOOL CSSYSAUDClearDecodeBuffer(HCSHANDLE hHandle, HCSHANDLE hReserved);

#ifdef  __cplusplus
}
#endif

#endif


