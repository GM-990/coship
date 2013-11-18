#ifndef LISTEN_H_
#define LISTEN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define DBG_THIRDPART_MSC "thirdparty.msc"

typedef enum _CSSRErrorCode_E
{
    EM_SR_ERROR_RECDEV = 1,    // ¼���ɼ�ʧ��
    EM_SR_ERROR_NET,            // �������,���Ӳ���,���ӳ�ʱ��
    EM_SR_ERROR_SERVER,        // �����Ʒ������
    EM_SR_ERROR_NOSPEAK,        //û��¼����������
    EM_SR_ERROR_MISS            //û��ʶ�����
}CSSRErrorCode_E;

typedef enum _CSSREventType_E
{
    EM_SR_EVENT_REC_START,    // ��ʼ¼��
    EM_SR_EVENT_SPEECH_END,   // ��⵽�����������
    EM_SR_EVENT_ERROR,        // ����,��ʱdwParam1��ʾ������,��CSSRErrorCode_E����
    EM_SR_EVENT_RESULT,       // ʶ����,��ʱdwParam1Ϊchar *����,��������ʶ��Ľ���ַ������׵�ַ,dwParam2��ʾ�ַ����ĳ��ȣ��ַ��������ʽ��ascii��
    EM_SR_EVENT_VOLUME
}CSSREventType_E;

/**
@brief 	�û��ص�,�����������Ϣͨ���˻ص�֪ͨ���û�
@param[in] enMsg:��Ϣ����
@param[in] dwPara1:��������μ�CSSREventType_E�����ÿ����Ϣ������
@param[in] dwPara2:��������μ�CSSREventType_E�����ÿ����Ϣ������
@param[in] pUserData:�û��������ڵ���CSSRAddCallback��ʱ�����
@return:
@note:
*/
typedef void (* OnSREvent_F)(CSSREventType_E enMsg, unsigned int dwParam1, unsigned int dwParam2, void *pUserData);

/**
@brief 	�����������Ȩ��

@param[in] nMode:����ҵ������
@param[in] nAuth:0��Ȩ�ޣ�-1��Ȩ��
@return:
@note:
*/
int CSSRAcessCtr(int nMode, int nAuth);

/**
@brief 	��������ʶ������ò���

@param[in] pcConfigs:���ò����ַ������������������ַ�����糬ʱʱ��ȣ���ʽ����"key=value,key2=value2"
@return 0��ʾ�ɹ�������ֵ��ʾʧ��
@note:
*/
int CSSRSetConfig(const char *pcConfigs, int nLength);

/**
@brief 	��������ʶ����

@param[in] nMode:����ģʽ��ȡֵΪ0��ʾҪ��ȡ����������ԭʼ�ı�
            ȡֵΪ1��ʾҪ��ȡ�����������֮���json�ַ������ݣ�json�ַ�������ҵ���壬�м�����������ʽ��ֻ��͸��
@return 0��ʾ�ɹ�������ֵ��ʾʧ��
@note:
*/
int CSSROpen(int nMode, const char *str);

/**
@brief 	�ر�����ʶ����

@return 0��ʾ�ɹ�������ֵ��ʾʧ��
@note:
*/
int CSSRClose(void);

/**
@brief 	�ر������ɼ�

@return 0��ʾ�ɹ�������ֵ��ʾʧ��
@note:
*/
int CSSRCloseRec(void);

/**
@brief 	ע��ص�����

@param[in] fnSRCallback:�û��ص�����
@return 0��ʾ�ɹ�������ֵ��ʾʧ��
@note:
*/
int CSSRAddCallback(OnSREvent_F fnSRCallback, void *pUserData);

/**
@brief 	ɾ���ص�����

@param[in] fnSRCallback:�û��ص�����
@return 0��ʾ�ɹ�������ֵ��ʾʧ��
@note:
*/
int CSSRRemoveCallback(OnSREvent_F fnSRCallback);


#ifdef __cplusplus
}
#endif

#endif /* LISTEN_H_ */
