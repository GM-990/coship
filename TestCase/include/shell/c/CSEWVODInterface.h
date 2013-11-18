/**@defgroup EW200VODExtern EW200 VOD��չ

@brief ��ģ��Ϊ�ٻ�200�м��VOD��չ�ӿ�,����Ϊ�ٻ�200�����µ�VOD֧��,��ʹ�ñ�ģ��
@note �����ٻ�200����UDI,����δ��ע����,��μ�UDI�ĵ�
@note ԭ���ϸ�ģ���Ϊһ��VOD�ľ���ʵ��,����������޹�,
ʵ��ʱ���������EW200��EW200VODģ��Ľӿ�(����Ҫ�����䶨��Ľṹ��)
@{
*/
#ifndef _VODINTERFACE_H_
#define _VODINTERFACE_H_

#include "udi2_typedef.h"
#include "CSEWPublic.h"
#include "CSEWVODListener.h"


#ifdef __cplusplus
extern "C"
{
#endif

class VODInterface;

/**
@brief ��ȡ�ٻ�200�е�VOD Listenerʵ��

��Ϊ����VODInterfaceʵ��ʱ�ı�Ҫ����,ʹ�ðٻ�200��Ϊ��չVODģ��ļ�����
@return VOD Listenerʵ��
*/
VODListener& CSEWGetListenerInstance(void);

/**
@brief ��һ�������VOD��ӵ��ٻ�200��

ʹ�ðٻ�200����֧�ָ����͵�VOD��Ŀ�Ĳ���
@param[in] cVodSa �����VODʵ��,����������VOD����֤
@param[in] cVodPlay �����VODʵ��,����������VOD�ĵ㲥
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWExternAddVOD(VODInterface &cVodSa,VODInterface &cVodPlay);

/**
@brief VOD�ӿ�

���о���VODʵ����ĸ���,������������뵽�ٻ�200�п��ðٻ�200֧�ָ����͵�VOD
*/
class VODInterface
{
protected:
    /**
    @brief VOD״̬������,VODʵ����Ҫ����Ӧ���в�����ִ��״̬ͨ��Listener֪ͨ��ȥ
    */
    VODListener &m_Listener;

public:

    /**
    @brief VOD��������ö��

    ��IOCtrl��nCmd����,��ϸ������ο�Ӧ�ò�ӿ��ĵ�����ؽӿ�
    */
    enum
    {
        EM_CTL_FAST_LOGIN,///<������½������ȡ������
        EM_CTL_GETAREACODE,///<��ȡ������
        EM_CTL_LOGIN,           ///< ��½,�޲���
        EM_CTL_GETHOMEURL,      ///< ��ȡ��ҳURL,���ڣ��ɲ�ʵ�֡�����Ϊ������ǰ�Ľӿ�
        EM_CTL_IDENTIFY,        ///< ����ʶ���Ƿ�֧��URLһ��,p0=URL(const char*)
        EM_CTL_PLAY,            ///< ����,p0=playId(int),p1=URL(const char*),p2=timeout(int)
        EM_CTL_STOP,            ///< ֹͣ����,p0=playId(int)
        EM_CTL_SET_RATE,        ///< ���ò����ٶ�,p0=playId(int);p1=nRate(int)
        EM_CTL_PAUSE,           ///< ��ͣ,p0=playId(int)
        EM_CTL_RESUME,          ///< �ָ�����,p0=playId(int)
        EM_CTL_SEEK,            ///< ��λ����,p0=playId(int);p1=time(int)
        EM_CTL_TELL,            ///< ��ȡ��ǰλ��,p0=playId(int);p1=time output(int)
        EM_CTL_GETEVENT,        ///< ֹͣ����,p0=playId(int);p1=EpgInfo output(CSEWVODEpgInfo_S*)
        EM_CTL_GETEVENTBYURL, ///< ��URL�õ�EPG��Ϣ, p0=URL(const char*), p1=EpgInfo output(CSEWVODEpgInfo_S*)
        EM_CTL_GETCORRELATEOFURL,  ///< ��URL��index�õ�������Ŀ��Ϣ��p0=URL(const char*), p1=nIndex(int),p2=psCorrelateEvent(CSEWVODCorrelateEvtInfo_S *)
        EM_CTL_GETPARAINFO,         ///< ���������͵õ�������Ϣ��p0=sParaKey(CSEWVODPARAType_E),p1=pcParaValue(char *),p2=nParaValeLen(int)
        EM_CTL_GETERRORINFO,           ///< ��ô�����Ϣ��p0=nErrorCode(int*)
        EM_CTL_LOGOUT,		///< �˳���¼��������е��ѱ������֤��Ϣ���޲���
        EM_CTL_GETPARAINFO_BYKEY, ///< ���������͵õ�������Ϣ��p0=sParaKey("iEPG_URL"),p1=pcParaValue(char *),p2=nParaValeLen(int)
        EM_CTL_ORDERED    ///< ����ʶ���Ƿ��Ѿ�����URLһ��,p0=URL(const char*)
    };
    VODInterface(VODListener &cListener): m_Listener(cListener) {}
    virtual ~VODInterface(void) {}

    /**
    @brief VOD���ƽӿ�

    ������VOD���͸��ֿ�������,�������������CTLϵ��ö��.
    @param[in] nCmd ��������,�������CTLϵ��ö��.
    @return �ɹ�ִ�������CSUDI_TRUE,���򷵻�CSUDI_FALSE
    @note ֮���Բ�ȡIOCtrl�ӿڵ���ʽ��Ϊ�˾ɲ���ܼ����°汾�İٻ㿼��
    */
    virtual CSUDI_BOOL IOCtrl(int nCmd, CSEWParam_S *psParam) = 0;
};

#ifdef __cplusplus
}
#endif

#endif  //_VODINTERFACE_H_

/** @} */

