#ifndef __CS_JSREGISTER_H__
#define __CS_JSREGISTER_H__

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
Function Name:    CSJSInit
 
Description:
    初始化jsext模块，此方法将会注册一些基础的js扩展对象
    对应的JS接口见<JavaScript扩展API接口规范>文档中的
 
Input:
    无

Output:
    无
Return Value:
    无

Other:
*******************************************************************************/
void CSJSInit(void);

/******************************************************************************
Function Name:    CSJSRegisterVOD
 
Description:
    注册双向视讯VOD前端页面用到的JS扩展功能
    对应的JS接口见<JavaScript扩展API接口规范>文档中的
 
Input:
    无

Output:
    无
Return Value:
    无

Other:
*******************************************************************************/
void CSJSRegisterVOD(void);

/******************************************************************************
Function Name:    CSJSRegister2DElement
 
Description:
    注册2D相关的JS功能
    对应的JS接口见<JavaScript扩展API接口规范>文档中的JS 基本2d接口，无特效 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegister2DElement(void);

/******************************************************************************
Function Name:    CSJSRegister2DElement
 
Description:
    注册2D相关的JS功能
    对应的JS接口见<JavaScript扩展API接口规范>文档.
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegister2D(void);

/******************************************************************************
Function Name:    CSJSRegisterDVB
 
Description:
    注册基于JS中间件的项目所用到的单向DVB业务功能相关的JS扩展
    对应的JS接口见<JavaScript扩展API接口规范>文档中的

    此方法必须在初始化完dtv、mediaplayer、fs之后调用，因为jsext dvb部分依赖
    于这些模块
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/

void CSJSRegisterDVB(void);

/******************************************************************************
Function Name:    CSJSRegisterCITV
 
Description:
    注册基于JS中间件的项目所用到的双向DVB业务功能相关的JS
    对应的JS接口见<JavaScript扩展API接口规范>文档中的同洲视讯VOD章节
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterCITV(void);

/******************************************************************************
Function Name:    CSJSRegisterTeletext
 
Description:
    注册subtitle/teletext/closecaption功能相关的JS扩展
    对应的JS接口见<JavaScript扩展API接口规范>文档中的Teletext章节、Subtitle章节
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterTeletext(void);


/******************************************************************************
Function Name:    CSJSRegisterPVR
 
Description:
    注册PVR功能相关的JS扩展
    对应的JS接口见<JavaScript扩展API接口规范>文档中的PVR管理章节
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterPVR(void);

/******************************************************************************
Function Name:    CSJSRegisterEW200VOD
 
Description:
    注册EW200VOD功能相关的JS扩展
    此接口给第三方集成EW200中间件的时候使用，此时，第三方
    只集成我们的VOD，不包括中间件中的播放模块(MP)，所以对于
    JS对象中的MediaPlayer的实现就不能像CSJSRegisterVOD中的直接调用中间件
    的播放模块(MP)来实现，而是需要调用给第三方提供的EW200的
    UDI接口来实现

    所以此接口与CSJSRegisterVOD是不能同时使用的，否则会产生不可预知
    的异常
    
    对应的JS接口见<JavaScript扩展API接口规范>文档中的
 
Input:
    无
 
Output:
    无
Return Value:
    无

Other:
*******************************************************************************/
void CSJSRegisterEW200VOD(void);

/******************************************************************************
Function Name:    CSJSRegisterTTS
 
Description:
    注册TTS功能所需要的JS扩展
    对应的JS接口见<JavaScript扩展API接口规范>文档中的
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterTTS(void);

/******************************************************************************
Function Name:    CSJSRegister3D
 
Description:
    注册3D特效功能所需要的JS扩展 基于opengl、opengles库实现的3D JS接口封装
    对应的JS接口见<JavaScript扩展API接口规范>文档中的
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegister3D(void);

/******************************************************************************
Function Name:    CSJSRegister3DMesa
 
Description:
    注册3DMesa特效功能所需要的JS扩展 基于Mesa库实现的3D应用 包括电视墙、地球仪等
    对应的JS接口见<JavaScript扩展API接口规范>文档中的
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegister3DMesa(void);

/******************************************************************************
Function Name:    CSJSRegisterUPNP
 
Description:
    注册UPNP功能所需要的JS扩展
    对应的JS接口见<JavaScript扩展API接口规范>文档中的
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterUPNP(void);

/******************************************************************************
Function Name:    CSJSRegisterCAppManager
 
Description:
    注册对C应用管理功能所需要的JS扩展
    对应的JS接口见<JavaScript扩展API接口规范>文档中的应用管理章节
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterCAppManager(void);

/******************************************************************************
Function Name:    CSJSRegisterJavaAppManager
 
Description:
    注册对Java应用管理功能所需要的JS扩展
    对应的JS接口见<JavaScript扩展API接口规范>文档中的应用管理章节
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterJavaAppManager(void);


/******************************************************************************
Function Name:    CSJSRegisterTJ
 
Description:
  注册对天津银联功能所需要的JS扩展
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/

void CSJSRegisterTJ(void);

/******************************************************************************
Function Name:    CSJSRegister2DFlatWall
 
Description:
    注册2D 平面特效相关的JS功能
    对应的JS接口见<JavaScript扩展API接口规范>文档中的JS 2D 平面特效章节
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegister2DFlatWall(void);

/******************************************************************************
Function Name:    CSJSRegisterSatellite
 
Description:
    注册Satellite功能相关的JS扩展
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterSatellite(void);

/******************************************************************************
Function Name:    CSJSRegisterIPTV1
 
Description:
    注册电视银行,Device对象。
Input:
    无

Output:
    无
Return Value:
    无
notes:
var device = new De
Other:
*******************************************************************************/
void CSJSRegisterIPTV1(void);


/******************************************************************************
Function Name:    CSJSRegisterAD
 
Description:
    注册广告模块所需要的jsext对象
    对应的JS接口见<JavaScript扩展API接口规范>文档中的
 
Input:
    无

Output:
    无
Return Value:
    无

Other:
*******************************************************************************/
void CSJSRegisterAD(void);

/******************************************************************************
Function Name: CSJSRegisterSTMosaic
 
Description:
    注册算通马赛克功能
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterSTMosaic(void);

/******************************************************************************
Function Name: CSJSRegisterHMMosaic
 
Description:
    注册哈雷马赛克功能
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterHMMosaic(void);

/******************************************************************************
Function Name: CSJSRegisterSaition
 
Description:
    注册甩信功能
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/


void CSJSRegisterSaition(void);
/**
@brief JS 类型
*/
typedef enum _CSJSPrototype_E {
	EM_JS_METHOD,   //类型为方法
	EM_JS_PROPERTY  //类型为属性
}CSJSPrototype_E;

/**
@brief JS返回值 类型
*/
typedef enum _CSJSResultBack_E {
     EM_JS_NUMBER,   //JS对象返回值的类型为number ,当为number的时候，传入
     EM_JS_STRING,  //类型为string
     EM_JS_BOOLEAN,//类型为boolean
     EM_JS_ARRAY //类型为Array
}CSJSResultBack_E;

/**
@brief JS对象调用信息描述
*/
typedef struct _CSJSObjectCallInfo_S {
	CSJSPrototype_E m_eType;    //调用的类型
	char m_acJSObjectStr[32];   //对象的名称
	char m_acPrototypeName[64]; //此对象对应的属性或者方法的名称
	char m_acArgs[512]; //属性或者方法所对应参数的值。格式为"{arg1:value1;arg2:value2...}", （对应在JS页面中写代码时JS对象的方法的输入参数）
}CSJSObjectCallInfo_S;


/**
@brief JS注册自定义回调函数
@param[in] pstJSObjectCallInfo:此对象的一些基本信息
@param[out] pOutResult:对象调用的返回值
@param[in] nResultSize:返回值的长度
@param[out]pResultType:对象调用返回值的类型,具体见CSJSResultBack_E的定义
*/
typedef void (*CSJSCallUserDefineObj_F)(CSJSObjectCallInfo_S *pstJSObjectCallInfo, CSJSResultBack_E *pResultType, void *pOutResult, int nResultSize);
 
/******************************************************************************
Function Name: CSJSRegisterJSObjectC
 
Description:
    注册自定义的JS扩展对象。
    注意，目前只支持注册内置对象。使用C的方法来实现jsext。
    调用完此接口之后，还需要调用CSJSAddJSObjectMethod或CSJSAddJSObjectProperty来添加此JS对象的方法和属性
    否则此JS对象将无任何属性和方法，没有意义了

Input:
    pcJSObjectStr : JS对象的名称（对应在JS页面中写代码时JS对象的字符串）
    fnJSCallback:回调函数指针，详见CSJSCallUserDefineObj_F定义
Output:
    无
Return Value:
    成功返回TRUE
    失败返回FALSE
 
Other:
*******************************************************************************/
BOOL CSJSRegisterJSObjectC(const char *pcJSObjectStr, CSJSCallUserDefineObj_F fnJSCallback);


/******************************************************************************
Function Name: CSJSRegisterJSObject
 
Description:
    注册自定义的JS扩展对象。注意，目前只支持注册内置对象

Input:
    pcJSObjectStr : JS对象的名称（对应在JS页面中写代码时JS对象的字符串）
    pcJavaClassPath : JS对象对应的Java对象的完整路径
 
Output:
    无
Return Value:
    成功返回TRUE
    失败返回FALSE
 
Other:
*******************************************************************************/
BOOL CSJSRegisterJSObject(const char *pcJSObjectStr, const char *pcJavaClassPath);

/******************************************************************************
Function Name: CSJSAddJSObjectMethod
 
Description:
    添加自定义JS对象的方法

Input:
    pcJSObjectStr : JS对象的名称（对应在JS页面中写代码时JS对象的字符串）
    pcMethodName : JS对象的方法名
 
Output:
    无
Return Value:
    成功返回TRUE
    失败返回FALSE
 
Other:
*******************************************************************************/
BOOL CSJSAddJSObjectMethod(const char *pcJSObjectStr, const char *pcMethodName);

/******************************************************************************
Function Name: CSJSAddJSObjectProperty
 
Description:
    添加自定义JS对象的属性

Input:
    pcJSObjectStr : JS对象的名称（对应在JS页面中写代码时JS对象的字符串）
    pcMethodName : JS对象的属性名
 
Output:
    无
Return Value:
    成功返回TRUE
    失败返回FALSE
 
Other:
*******************************************************************************/
BOOL CSJSAddJSObjectProperty(const char *pcJSObjectStr, const char *pcPropertyName);

/******************************************************************************
Function Name: CSJSRemoveJSObject
 
Description:
    删除之前添加的自定义JS对象

Input:
    pcJSObjectStr : JS对象的名称（对应在JS页面中写代码时JS对象的字符串）
 
Output:
    无
Return Value:
    成功返回TRUE
    失败返回FALSE
 
Other:
*******************************************************************************/
BOOL CSJSRemoveJSObject(const char *pcJSObjectStr);

/******************************************************************************
Function Name:    CSJSRegisterHWVOD
 
Description:
    注册基于JS中间件的项目所用到的双向DVB业务功能相关的JS扩展
    对应的JS接口见<JavaScript扩展API接口规范>文档中的同洲视讯VOD章节
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterHWVOD(void);

/**
@brief JS业务枚举
*/
typedef enum _CSJSServic_E{
    EM_JS_EPG,        //EPG ,回调对应的pcServiceData取值为NULL
    EM_JS_MOSIC,     //马赛克，回调中的pcServiceData格式:{mosaicIndex:value1, url:"value2"}";其中mosaicIndex对应的值为int类型，表示马赛克的索引;url对应的值为"dvb://OriginalNetworkID.TS_ID.ServiceID",表示当前马赛克对应的三要素。
    EM_JS_AD,		 //广告，回调中的pcServiceData的格式为"{typeIndex:value1, isShow:value2}"。typeIndex的值为int类型，表示广告位,具体描述参考<JavaScript扩展API接口规范>文档中的广告章节;isShow对应的值为int类型，当值为0的时候表示不显示，为其他表示显示该广告位。
    EM_JS_APP           //应用，回调中的pcServiceData的格式为"{appId:"value1"}。appId对应为字符串,表示应用的名称。
} CSJSServic_E;

/**
@brief 获取JS业务状态回调函数定义
@param[in] eService 详见CSJSServic_E说明
@param[in] pcServiceData  详见CSJSServic_E注释说明；注：编码格式需要是ascii
@param[in] pUserData 用户数据
*/
typedef void (*CSJSServiceStateCallback_F) (CSJSServic_E eService, const char* pcServiceData, void* pUserData);

/**
@brief 注册获取JS业务信息的回调函数。
@param[in] fnServiceCallback 指向回调函数的函数指针
@param[in] pUserData 用户数据，可以为NULL
@return 成功返回TRUE，失败返回FALSE
@note  最多可以允许注册5个回调
*/
BOOL CSJSAddServiceStateCallback(CSJSServiceStateCallback_F fnServiceCallback, void* pUserData);


/**
@brief 删除获取JS业务信息的回调函数。
@param[in] fnServiceCallback 指向回调函数的函数指针
@return 成功返回TRUE，失败返回FALSE
*/
BOOL CSJSRemoveServiceStateCallback(CSJSServiceStateCallback_F fnServiceCallback);

/******************************************************************************
Function Name:    CSJSRegisterLocalService
 
Description:
    注册localservice相关的JS扩展功能
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterLocalService(void);

/******************************************************************************
Function Name:    CSJSRegisterNVOD
 
Description:
    注册NVOD相关的JS扩展功能
    对应的JS接口见<JavaScript扩展API接口规范>文档中的NVOD章节	
	    
	此方法必须在初始化完dtv之后调用, 因为js nvod依赖于这些初始化的实现
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterNVOD(void);

/******************************************************************************
Function Name:    CSJSRegisterMotoVod

Description:
注册基于JS中间件的项目所用到的MOTO VOD业务功能相关的JS扩展

Input:
无

Output:
无
Return Value:
无

Other:
*******************************************************************************/
void CSJSRegisterMotoVod(void);
/******************************************************************************
Function Name:    CSJSRegisteGCable

Description:
注册基于JS中间件的项目所用到的广东项目业务功能相关的JS扩展

Input:
无

Output:
无
Return Value:
无

Other:
*******************************************************************************/
void CSJSRegisteGCable(void);


/******************************************************************************
Function Name:    CSJSRegisterSeachangeVod
 
Description:
    注册基于JS中间件的项目所用到的相关的JS扩展
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterSeachangeVod(void);

/******************************************************************************
Function Name:    CSJSRegisterHLJAD
 
Description:
    注册黑龙江广告
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterHLJAD(void);

/******************************************************************************
Function Name:    CSJSRegisterNrc
 
Description:
    注册基于JS中间件的项目所用到的双向DVB业务功能相关的JS扩展
    对应的JS接口见<JavaScript扩展API接口规范>文档中的同洲视讯VOD章节
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterNrc(void);

/******************************************************************************
Function Name:    CSJSRegisterVideoPhone

Description:
注册基于JS中间件的项目所用到的视频电话的相关JS对象。

Input:
无

Output:
无
Return Value:
无

Other:
*******************************************************************************/
void CSJSRegisterVideoPhone(void);

/******************************************************************************
Function Name:    CSJSRegisterUnionBank
 
Description:
    注册银联的JS扩展功能.
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterUnionBank(void);

/******************************************************************************
Function Name:    CSJSRegisterUSBKey
 
Description:
	注册usbkey支付对象
     
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterUSBKey(void);

/******************************************************************************
Function Name:    CSJSRegisterTSMEncryption
 
Description:
    注册天津支付TSM的所有js对象
   JS接口定义见<天津支付项目-终端安全模块中间件移植手册.pdf>文档中的TSM提供给中间接调用的函数接口章节
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterTSMEncryption(void);

/******************************************************************************
Function Name:    CSJSRegisteDVBWithoutCA
 
Description:
    注册基于JS中间件的项目所用到的单向DVB业务功能相关的JS扩展,除去ca的注册
    对应的JS接口见<JavaScript扩展API接口规范>文档中的

    此方法必须在初始化完dtv、mediaplayer之后调用，因为jsext dvb部分依赖
    于这些模块
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisteDVBWithoutCA(void);

/******************************************************************************
Function Name:    CSJSRegisteDVNCA
 
Description:
   注册天柏ca
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisteDVNCA(void);

/******************************************************************************
Function Name:    CSJSRegisterPayCard
 
Description:
   注册市民卡JS对象【目前仅用于江苏项目】
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisterPayCard(void);

/******************************************************************************
Function Name:    CSJSRegisterDLNA

Description:
    注册DLNA功能相关的JS对象

Input:
    无

Output:
    无
Return Value:
    无

Other:
*******************************************************************************/
void CSJSRegisterDLNA(void);

/******************************************************************************
Function Name:    CSJSRegisteRSAUtils
 
Description:
    RSA加密解密相关接口实现
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisteRSAUtils(void);

/******************************************************************************
Function Name:    CSJSRegisteSkyworthAD
 
Description:
    创维广告相关接口实现
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisteSkyworthAD(void);

/******************************************************************************
Function Name:    CSJSRegisteNGB
 
Description:
    注册NGB规范的接口
    对应的JS接口见<下一代广播电视网中间件技术规范-附件2：NGB-H应用编程接口（送审讨论稿）_v0.7.2012080900.doc>文档中的

    此方法必须在初始化完dtv、mediaplayer、fs之后调用，因为jsext dvb部分依赖
    于这些模块
 
Input:
    无
 
Output:
    无
Return Value:
    无
 
Other:
*******************************************************************************/
void CSJSRegisteNGB(void);


/******************************************************************************
Function Name:    CSJSRegisterMutiPageCtrl

Description:
注册多page的JS对象

Input:
无

Output:
无
Return Value:
无

Other:
*******************************************************************************/
BOOL CSJSRegisterMutiPageCtrl(void);
/******************************************************************************
Description:
	注册语音识别相关JS功能

Input:
无

Output:
无
Return Value:
无

Other:
*******************************************************************************/
void CSJSRegisterSR(void);

#ifdef __cplusplus
}
#endif

#endif
