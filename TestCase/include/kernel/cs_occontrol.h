#ifndef _CS_OCCONTROL_H_
#define _CS_OCCONTROL_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"


typedef enum _CSOCType
{
	EM_DEC,	//coship dvb url 10进制
	EM_HEX   //ipanel dvb url 16进制
}e_OCType;

typedef enum _eSourceMsg
{
	EM_SOURCE_SIGNAL_CONNECTED,           //频点锁定成功
 	EM_SOURCE_SIGNAL_DROPPED,             //由频点锁定成功转为锁定失败
 	EM_SOURCE_SIGNAL_LOST      //频点锁定失败
}eSourceMsg;


/*浏览器发动给应用的消息类型*/
typedef enum 
{
	EM_BRW_OCCONTROL 
}eMsgType;


typedef enum _selectStatus_E
{
	EM_OC_SELECT_WAIT = 1,
	EM_OC_SELECT_READY,
	EM_OC_SELECT_FAIL,
	EM_OC_SELECT_NO_OBJ,
	EM_OC_SELECT_NO_ERROR,
	EM_OC_SELECT_START_LOAD
}selectStatus_E;


/*************************************************
接口函数:OCMsgNotify_F
函数说明:eMsg: EM_BRW_OC nParam is eOCMsgBoxType

返回: 
**************************************************/
typedef void* ( *OCMsgNotify_F)(eMsgType eMsg, int nParam, void *pvUserData);


/*************************************************
接口函数:fnDVBConnectSourcCallBack
函数说明:将CSOCPortingConnectSource下发的serviceid,tsid,ornetid返回
		给ocControl
返回: 无
**************************************************/
typedef void ( *DVBConnectSourceCallBack_F)(eSourceMsg eMsg, int nOrnetID, int nTsID, int nServiceID);

/*************************************************
接口函数:OCConnectSourceCallBack_F
函数说明:OC锁频函数指针， 接口需要通过ornetid、tsid、serverid获取频点信息进行锁频，并通过fnConnectSouceCB通知给occontrol，fnConnectSouceCB允许为NULL
返回: 无
**************************************************/
typedef BOOL(*OCConnectSourceCallBack_F)(int nOrNetId, int nTsId, int nServiceId, DVBConnectSourceCallBack_F fnConnectSouceCB);

/***************************************************
				C Control API
****************************************************/

/*************************************************
函数:CSMidpOCInit
参数说明:midp使用OCControl模块，应用需要初始化之后调用
		该函数注册
返回: 0成功，-1失败
**************************************************/
int CSMidpOCInit(void);

/*************************************************
函数:CSDvbOCInit
参数说明:向浏览器注册dvb协议
返回: 0成功，-1失败
**************************************************/
int CSDvbOCInit(void);

/*************************************************
函数:CSDeliveryOCInit
说明:向浏览器注册delivery://Frequency.SymbolRate.Modulation.serviceId.componentTag/xx/xx.txt协议
返回: 0成功，-1失败
**************************************************/
int CSDeliveryOCInit(void);

/*************************************************
函数:CSOCInit(int nOCBufSizem, e_OCType type)
参数说明:nOCBufSizem 用于OC模块内存大小,单位为K
		type 描述前端下发的dvb url为十进制还是十六进制
返回: 0成功，-1失败
**************************************************/
int CSOCInit(int nOCBufSizem, e_OCType type);

/*************************************************
函数:CSOCSetChangeSourceInfo
参数说明: 在数据广播中，应用锁频成功通过
		该接口将频点信息传给oc control模块，
		包括在数据广播中的视频播放频点,如果不知道networkid，tsid和serviceid传回0
返回: 
**************************************************/
void CSOCSetChangeSourceInfo(int nFrequency, int nNetworkId, int nTsId, int nServiceId);

/**************************************************
函数:  注册OC Control锁频回调处理函数

参数说明: OC Control锁频回调处理函数
		  pvUserData 用户数据
返回: SUCCESS :  TRUE
**************************************************/
BOOL CSRegisterOCConnectSource(OCConnectSourceCallBack_F fnConnectSouceCB);
 
/***************************************************
			应用帮助OC Control实现PORTING接口
****************************************************/

/*************************************************
接口函数:CSOCPortingConnectSource
函数说明:OC Control下发OrNetId nTsId nServiceId，实现
		函数查找当前Service所在频点，并锁频，pConnectSouceCB
		通知OC Control锁频状态
返回: TRUE成功，FALSE失败
**************************************************/
extern BOOL CSOCPortingConnectSource(int nOrNetId, int nTsId, int nServiceId, DVBConnectSourceCallBack_F fnConnectSouceCB);


/*************************************************
函数:CSOCdestroy
	 退出OC应用调用销毁OCControl数据，浏览器退出不需要调用
返回: 无
**************************************************/
void CSOCdestroy(void);

/*************************************************
注册OCMsgNotify_F回调函数，用于OCControl消息通知应用
返回: TRUE成功，FALSE失败
**************************************************/
BOOL CSOCControlRegisterMsgNotifyCB(OCMsgNotify_F fnMsgNotify);

/*************************************************
函数:CSDvbOpen
参数说明: pcUrl 传入URL以dvb://开头
		  pcMode 文件格式(未使用)
		  bNeedUpdate TRUE 需要监控实时更新，FALSE不监控更新
		  dwUpdateInterval更新间隔时间
返回: > 0 索引，<=0错误
**************************************************/
int CSDvbOpen(const char * pcUrl, const char * pcMode, BOOL bNeedUpdate, DWORD dwUpdateInterval);

/*************************************************
函数:CSDvbOpenEx
参数说明: pcUrl 传入URL以dvb://开头
		  pcMode 文件格式(未使用)
		  bNeedUpdate TRUE 需要监控实时更新，FALSE不监控更新
		  dwUpdateInterval更新间隔时间
		  bReload 是否重新下载，不管文件是否已在缓存中
返回: > 0 索引，<=0错误
**************************************************/
int CSDvbOpenEx(const char * pcUrl, const char * pcMode, BOOL bNeedUpdate, DWORD dwUpdateInterval, BOOL bReload);

/*************************************************
函数:CSDvbSelect
参数说明: nIndex CSDvbOpen返回值
		  ntimeout 未使用
		  监控OC数据下载情况
返回: selectStatus_E
**************************************************/
selectStatus_E CSDvbSelect(int nIndex, int ntimeout);

/*************************************************
函数:CSDvbAvailable
参数说明: nIndex CSDvbOpen返回值
CSDvbSelect成功之后调用该接口可以得到文件大小
返回: 文件大小
**************************************************/
int CSDvbAvailable(int nIndex); 

/*************************************************
函数:CSDvbRecv
参数说明: nIndex CSDvbOpen返回值
		  pBuffer 数据存放地址
		  size：pBuffer大小
CSDvbSelect成功之后调用该接口可以得到文件数据
返回: 接收数据大小
**************************************************/
int CSDvbRecv(int nIndex, void *pBuffer, unsigned int size);

/*************************************************
函数:CSDvbClose
参数说明: nIndex CSDvbOpen返回值
	下载完成关闭索引
返回: 无
**************************************************/
int CSDvbClose(int nIndex);

/*************************************************
函数:CSDvbPreLoad
参数说明: pszUrl 预取URL
预取接口，预取过程不进行锁频，只对本频点有效
返回: 无
**************************************************/
void CSDvbPreLoad(const char * pszUrl);

/*************************************************
函数:CSStartPreLoad
参数说明: 开始预取子页面资源
返回: 无
**************************************************/
void CSStartPreLoad();

/*************************************************
函数:CSCancelPreLoad
参数说明: 取消预取
返回: 无
**************************************************/
void CSCancelPreLoad();

/*************************************************
函数:CSOCSetTotalTimeOut
参数说明: 设置超时时间，默认时间40s
返回: 无
**************************************************/
void CSOCSetTotalTimeOut(DWORD dwTotalTimeout);

/*************************************************
函数:CSDvbSeek
参数说明:打开的文件中指定当前的读/写位置
返回: 无
**************************************************/
int CSDvbSeek(int nIndex, int nOffset, int nOrigin);

#ifdef  __cplusplus
}
#endif

#endif 


