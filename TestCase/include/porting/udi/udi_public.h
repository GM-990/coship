#ifndef _UDI_SYSPUBLIC_H_
#define _UDI_SYSPUBLIC_H_

#include "udi_typedef.h"
#include "udi_assert.h"
#include "udi_device.h"

#ifdef  __cplusplus
extern "C" {
#endif

//������������UDI�������������汾��Ϣ�������漰UDI�ĸĶ������������¼

/**@mainpage Unified Driver Interface(UDI) �ĵ�
@version <B>UDI_V0.3</B> ��δ����
- �ĵ���ʽ����ΪUDI
- ������ESע��Ľṹ���Ƴ�UDI
- �����©��CSSYSEventMsg��CSSYSRegisterMsgö�ٶ���
- ����CSDMXStart�ص�ʱ������ϸ˵��
- ����Filterֻ��֤16�ֽ���ȵ�����
- ϸ��I֡��ʽ����ϸ˵��

@version <B>UDI_V0.2</B> 2008/11/12
- ��������Ƶ���Žӿڣ��Ƴ������ظ����߲����Ƽ�ʹ�õĽӿ�
- ����pthreadģ�������
- ����udiͳһ��ʼ���ӿ�
- ����Ӧ�û�����Ϣ��ȡ�ӿ�
- ���Ӵ���ģ���ϸ���������Ƴ������ӿ�

@version <B>UDI_V0.1</B> 2008/10/21
- ��ʼ�汾
*/

/**@brief ����Ƶ���ݴ����ʽ����Դ*/
typedef enum _CSSYSServiceType
{	
	CSSYS_SERVICE_TS_HW,	 	/**< TSӲ��ע�룬һ��ΪTuner*/
	CSSYS_SERVICE_TS_SW,		/**< TSʹ��ע��ӿ����ע��,��IPTV��PVR*/
	CSSYS_SERVICE_ES_HW,		/**< ESӲ��ע�룬һ��ΪTuner*/
	CSSYS_SERVICE_ES_SW	 	    /**< ESʹ��ע��ӿ����ע��,��IPTV��PVR*/
}CSSYSServiceType;

/**@brief Input Vid stream content information*/
typedef struct _CSSYSPCRInputInfo
{
	int nPID;			/**< PCR Stream PID*/
}CSSYSPCRInputInfo;

/**@brief ����Ƶ�����¼�,2��Ϊһ��,С�����ȼ���*/
typedef enum _CSSYSEventMsg
{
	CSSYSEVENT_SYSTEM_RUNNING,		        /**< system run */
	CSSYSEVENT_SYSTEM_STANDBY,		        /**< system stanby */	
	CSSYSEVENT_SIGNAL_LOCK, 		        /**< tuner locked */
	CSSYSEVENT_SIGNAL_LOST,			        /**< tuner lost */	
	CSSYSEVENT_SMARTCARD_IN,  		        /**< insert smart card*/
	CSSYSEVENT_SMARTCARD_OUT,		        /**< remove smart card*/	
	CSSYSEVENT_FRONTTS_RUN,			        /**< front TS be running event*/
	CSSYSEVENT_FRONTTS_STOP,		        /**< front TS stop event*/	
	CSSYSEVENT_PG_UNLOCK,			        /**< video decoder stop event*/
	CSSYSEVENT_PG_LOCK,			 	        /**< video decoder be running event*/
	CSSYSEVETN_DEOCODE_FRAME_COUNT_CONTINUE,/**< 10 ��Ҫ���˵��*/
	CSSYSEVETN_DEOCODE_FRAME_COUNT_STOP,	/**< 11 ��Ҫ���˵��*/
	CSSYSEVENT_SERVICE_NOLOCK,				/**< 12 ��Ҫ���˵��*/
	CSSYSEVENT_SERVICE_LOCK,				/**< 13 ��Ҫ���˵��*/
	CSSYSEVENT_USER_OPEN,					/**< 14 ��Ҫ���˵��*/
	CSSYSEVENT_USER_CLOSE,					/**< 15 ��Ҫ���˵��*/
	CSSYSEVENT_SMS_OPEN,					/**< Ϊ�˼��� CSUSPSMSEnable/CSUSPSMSDisable*/
	CSSYSEVENT_SMS_CLOSE,					/**< ��Ҫ���˵��*/
	CSSYSEVENT_START_PLAY, 				    /**< ����*/
	CSSYSEVENT_STOP_PLAY,					/**< ��Ҫ���˵��*/
	CSSYSEVENT_VIDEO_IFRAME_SHOW,		    /**< video����ʾIframe*/
	CSSYSEVENT_VIDEO_IFRAME_HIDE,		    /**< video������Iframe*/
	CSSYSEVENT_MAX
}CSSYSEventMsg; 

/**@brief ��Ƶע���¼�����*/
typedef enum _CSSYSRegisterMsg
{
	CSSYS_MSG_FRONT_TS		/**< ǰ��ͣ������¼�*/
}CSSYSRegisterMsg;

#ifdef  __cplusplus
}
#endif

#endif


