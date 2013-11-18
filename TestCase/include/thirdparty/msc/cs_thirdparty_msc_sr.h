#ifndef LISTEN_H_
#define LISTEN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define DBG_THIRDPART_MSC "thirdparty.msc"

typedef enum _CSSRErrorCode_E
{
    EM_SR_ERROR_RECDEV = 1,    // 录音采集失败
    EM_SR_ERROR_NET,            // 网络错误,连接不上,连接超时等
    EM_SR_ERROR_SERVER,        // 语音云服务错误
    EM_SR_ERROR_NOSPEAK,        //没有录到声音数据
    EM_SR_ERROR_MISS            //没有识别出来
}CSSRErrorCode_E;

typedef enum _CSSREventType_E
{
    EM_SR_EVENT_REC_START,    // 开始录音
    EM_SR_EVENT_SPEECH_END,   // 检测到语音输入结束
    EM_SR_EVENT_ERROR,        // 出错,此时dwParam1表示出错码,是CSSRErrorCode_E类型
    EM_SR_EVENT_RESULT,       // 识别结果,此时dwParam1为char *类型,代表语音识别的结果字符串的首地址,dwParam2表示字符串的长度，字符串编码格式是ascii码
    EM_SR_EVENT_VOLUME
}CSSREventType_E;

/**
@brief 	用户回调,语音引擎把消息通过此回调通知给用户
@param[in] enMsg:消息类型
@param[in] dwPara1:其意义请参见CSSREventType_E中针对每个消息的描述
@param[in] dwPara2:其意义请参见CSSREventType_E中针对每个消息的描述
@param[in] pUserData:用户参数，在调用CSSRAddCallback的时候传入的
@return:
@note:
*/
typedef void (* OnSREvent_F)(CSSREventType_E enMsg, unsigned int dwParam1, unsigned int dwParam2, void *pUserData);

/**
@brief 	设置语音相关权限

@param[in] nMode:语音业务类型
@param[in] nAuth:0有权限，-1无权限
@return:
@note:
*/
int CSSRAcessCtr(int nMode, int nAuth);

/**
@brief 	设置语音识别的配置参数

@param[in] pcConfigs:配置参数字符串，配置如服务器地址，网络超时时间等，格式如下"key=value,key2=value2"
@return 0表示成功；其他值表示失败
@note:
*/
int CSSRSetConfig(const char *pcConfigs, int nLength);

/**
@brief 	启动语音识别功能

@param[in] nMode:启动模式，取值为0表示要获取输入语音的原始文本
            取值为1表示要获取经过语义分析之后的json字符串数据，json字符串可由业务定义，中间件不关心其格式，只做透传
@return 0表示成功；其他值表示失败
@note:
*/
int CSSROpen(int nMode, const char *str);

/**
@brief 	关闭语音识别功能

@return 0表示成功；其他值表示失败
@note:
*/
int CSSRClose(void);

/**
@brief 	关闭语音采集

@return 0表示成功；其他值表示失败
@note:
*/
int CSSRCloseRec(void);

/**
@brief 	注册回调函数

@param[in] fnSRCallback:用户回调函数
@return 0表示成功；其他值表示失败
@note:
*/
int CSSRAddCallback(OnSREvent_F fnSRCallback, void *pUserData);

/**
@brief 	删除回调函数

@param[in] fnSRCallback:用户回调函数
@return 0表示成功；其他值表示失败
@note:
*/
int CSSRRemoveCallback(OnSREvent_F fnSRCallback);


#ifdef __cplusplus
}
#endif

#endif /* LISTEN_H_ */
