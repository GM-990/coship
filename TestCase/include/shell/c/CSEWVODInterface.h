/**@defgroup EW200VODExtern EW200 VOD扩展

@brief 本模块为百汇200中间件VOD扩展接口,若需为百汇200增加新的VOD支持,请使用本模块
@note 整个百汇200依赖UDI,部分未标注申明,请参见UDI文档
@note 原则上该模块仅为一个VOD的具体实现,与其管理者无关,
实现时不建议调用EW200及EW200VOD模块的接口(但需要引用其定义的结构等)
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
@brief 获取百汇200中的VOD Listener实例

作为创建VODInterface实例时的必要参数,使得百汇200成为扩展VOD模块的监听者
@return VOD Listener实例
*/
VODListener& CSEWGetListenerInstance(void);

/**
@brief 将一个具体的VOD添加到百汇200中

使得百汇200可以支持该类型的VOD节目的播放
@param[in] cVodSa 具体的VOD实例,负责具体完成VOD的认证
@param[in] cVodPlay 具体的VOD实例,负责具体完成VOD的点播
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWExternAddVOD(VODInterface &cVodSa,VODInterface &cVodPlay);

/**
@brief VOD接口

所有具体VOD实现类的父类,将这样的类加入到百汇200中可让百汇200支持该类型的VOD
*/
class VODInterface
{
protected:
    /**
    @brief VOD状态监听者,VOD实现需要在响应所有操作后将执行状态通过Listener通知出去
    */
    VODListener &m_Listener;

public:

    /**
    @brief VOD控制命令枚举

    即IOCtrl的nCmd参数,详细作用请参考应用层接口文档中相关接口
    */
    enum
    {
        EM_CTL_FAST_LOGIN,///<仅仅登陆，不获取区域码
        EM_CTL_GETAREACODE,///<获取区域码
        EM_CTL_LOGIN,           ///< 登陆,无参数
        EM_CTL_GETHOMEURL,      ///< 获取主页URL,过期，可不实现。保留为兼容以前的接口
        EM_CTL_IDENTIFY,        ///< 快速识别是否支持URL一个,p0=URL(const char*)
        EM_CTL_PLAY,            ///< 播放,p0=playId(int),p1=URL(const char*),p2=timeout(int)
        EM_CTL_STOP,            ///< 停止播放,p0=playId(int)
        EM_CTL_SET_RATE,        ///< 设置播放速度,p0=playId(int);p1=nRate(int)
        EM_CTL_PAUSE,           ///< 暂停,p0=playId(int)
        EM_CTL_RESUME,          ///< 恢复播放,p0=playId(int)
        EM_CTL_SEEK,            ///< 定位播放,p0=playId(int);p1=time(int)
        EM_CTL_TELL,            ///< 获取当前位置,p0=playId(int);p1=time output(int)
        EM_CTL_GETEVENT,        ///< 停止播放,p0=playId(int);p1=EpgInfo output(CSEWVODEpgInfo_S*)
        EM_CTL_GETEVENTBYURL, ///< 由URL得到EPG信息, p0=URL(const char*), p1=EpgInfo output(CSEWVODEpgInfo_S*)
        EM_CTL_GETCORRELATEOFURL,  ///< 由URL和index得到关联节目信息，p0=URL(const char*), p1=nIndex(int),p2=psCorrelateEvent(CSEWVODCorrelateEvtInfo_S *)
        EM_CTL_GETPARAINFO,         ///< 由配置类型得到配置信息，p0=sParaKey(CSEWVODPARAType_E),p1=pcParaValue(char *),p2=nParaValeLen(int)
        EM_CTL_GETERRORINFO,           ///< 获得错误信息，p0=nErrorCode(int*)
        EM_CTL_LOGOUT,		///< 退出登录，清空所有的已保存的认证信息，无参数
        EM_CTL_GETPARAINFO_BYKEY, ///< 由配置类型得到配置信息，p0=sParaKey("iEPG_URL"),p1=pcParaValue(char *),p2=nParaValeLen(int)
        EM_CTL_ORDERED    ///< 快速识别是否已经购买URL一个,p0=URL(const char*)
    };
    VODInterface(VODListener &cListener): m_Listener(cListener) {}
    virtual ~VODInterface(void) {}

    /**
    @brief VOD控制接口

    用于向VOD发送各种控制命令,具体命令参数见CTL系列枚举.
    @param[in] nCmd 命令类型,即本类的CTL系列枚举.
    @return 成功执行命令返回CSUDI_TRUE,否则返回CSUDI_FALSE
    @note 之所以采取IOCtrl接口的形式是为了旧插件能兼容新版本的百汇考虑
    */
    virtual CSUDI_BOOL IOCtrl(int nCmd, CSEWParam_S *psParam) = 0;
};

#ifdef __cplusplus
}
#endif

#endif  //_VODINTERFACE_H_

/** @} */

