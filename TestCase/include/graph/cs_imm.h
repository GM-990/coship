/**
文件名称：CS_IMM.H
版权所有(c)  2009， 同洲电子。
 
@version  Ver 1.0    
@author   龙式荣 (longshirong@coship.com)
@file
		输入法接口，其使用方法如下:

		1、调用CSIMMCreate接口创建输入法管理器句柄，
		该接口传入的参数中配置文件的路径现在可以先不做设置，
		该功能还未完善；而创建时需要指定默认输入法，以便在未注册输入法时，
		可以使用默认的系统输入法；
		同时，也可以指定输入法管理器在创建之后是否可以立即生效；
		2、调用CSIMMStart启动输入法，该接口需要传入输入法管理器句柄，
		即调用CSIMMCreate后的返回值；
		3、这时，输入法已经启动，可以通过CSIMMSendEvent发送事件CSIMM_EVENT_IMM_START给输入法处理；
		4、关闭输入法，可以调用CSIMMDestroy接口，也可以先发送CSIMM_EVENT_IMM_CLOSE	事件关闭输入法
 
History:
版本号		日期		作者		修改内容
---------	     --------        -------   	-------------
1.0			2009-12-22	龙式荣		创建
1.1                   2010-11-17   龙式荣               修改注册接口，增加一个参数，用于指定输入法引擎中输入法个数
*/

#ifndef __CS_IMM_H_
#define __CS_IMM_H_

#include "mwapi.h"
#include "mwengine.h"
#include "cs_event.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 头文件引用                                 *
 ******************************************************************************/


/******************************************************************************
 *                                 宏、定义、结构体                           *
 ******************************************************************************/
 #define CSMIN(a,b)              ((a) < (b) ? (a) : (b))
#define CSMAX(a,b)              ((a) > (b) ? (a) : (b))

/**系统输入法标识符*/
#define CSIMM_IME_PINYIN 		0x00000001
#define CSIMM_IME_ENGLISH 	0x00000002
#define CSIMM_IME_SYMBOL		0x00000004
#define CSIMM_IME_DIGITAL		0x00000008
#define CSIMM_IME_ZHUYIN        0x00000010

//系统输入法标识符的掩码
#define CSIMM_SYS_IME			0x0000000F

//输入管理事件类型
#define CSIMM_EVENT_IMM_BEGAIN  	1			///<输入法使能事件
#define CSIMM_EVENT_IMM_CLOSE 		(CSIMM_EVENT_IMM_BEGAIN + 0)			///<关闭输入法事件
#define CSIMM_EVENT_IMM_REINIT 		(CSIMM_EVENT_IMM_BEGAIN + 1)			///<重新初始化输入法窗口，如果切换后的输入法使用的是之前输入法的窗体，必须要对该消息做处理
#define CSIMM_EVENT_IMM_MAX		(CSIMM_EVENT_IMM_BEGAIN + 2)

///<输入法内部处理的事件类型，值必须设置为大于输入法管理事件类型的值
#define CSIMM_IME_EVENT_OPEN		CSUDI_VK_INPUT				///<输入法打开事件与切换
#define CSIMM_IME_EVENT_CLOSE		CSUDI_VK_EXIT				///<输入法关闭事件

//在此添加事件

//输入法按键
#define CS_T9INPUT_KEY_LEFT           			CSUDI_VK_LEFT
#define CS_T9INPUT_KEY_UP             			CSUDI_VK_UP
#define CS_T9INPUT_KEY_RIGHT          		CSUDI_VK_RIGHT
#define CS_T9INPUT_KEY_DOWN           		CSUDI_VK_DOWN
#define CS_T9INPUT_KEY_PRIOR          		CSUDI_VK_PAGE_UP
#define CS_T9INPUT_KEY_NEXT           			CSUDI_VK_PAGE_DOWN
#define CS_T9INPUT_KEY_T9INPUT			CSUDI_VK_INPUT
#define CS_T9INPUT_KEY_BACK             		CSUDI_VK_TOGGLE

#define CSIMM				"IMM"

#define CSIMM_ENABLE			1			///<输入法使能
#define CSIMM_UNABLE			0			///<输入法使不能

#define CSIMM_MAX_MSG_NUM   3
#define CSIMM_THREAD_PRIORITY   2
#define CSIMM_DEFAULT_TIMEOUT 10

#define CSIMM_KEY_TYPE			GR_EVENT_TYPE_KEY_DOWN

#define	EVT_INPUT_IME    				(0x3005)

typedef struct  
{
    DWORD m_dwEvent;		 ///< 消息内容
    DWORD m_dwType;		 ///< 消息类型,主要为按键的keydown 或者keyup或者keypress
    DWORD m_dwPara1;		 ///<消息的附加参数1
    DWORD m_dwPara2; 		///<消息的附加参数2
}CSIMM_EVENT_S;

typedef INT (*IMMPostResultFunc)(const struct _MMCP_EvtEventEntry *event, void  *client_data,void  *object,void  *method );

enum
{
	E_IMM_RETURN_OK = 0,
	E_IMM_RETURN_FALSE = -1,
	E_IMM_RETURN_INITIALED = -2,	///<管理器实例已经初始化
	E_IMM_RETURN_UNINITIAL = -3,	///<输入法管理器未初始化
	E_IMM_RETURN_UNABLED = -4,		///<被禁止的
	E_IMM_RETURN_MAX
}; 

typedef struct _IME_WININFO_
{
	//调用者窗口传入的参数
	HCSHANDLE m_hUserClient;			///<调用者窗口Client
	HCSHANDLE m_hUserWindow;			///<调用者窗口句柄
	HCSHANDLE m_hUserGC;				///<调用者窗口GC

	//输入法窗口创建之后的参数
	HCSHANDLE m_hImeClient;			///<输入法窗口client
	HCSHANDLE m_hImeWindow;			///<输入法窗口句柄
	HCSHANDLE m_hImeGC;				///<输入法窗口GC

	//设置窗口的起始位置、宽度等信息
	GR_RECT m_sWinRect;					///<输入法窗口显示位置及大小
	GR_SIZE m_usBorderSize;				///<输入法窗口边框大小
	GR_COLOR m_unBackgroundColor;		///<输入法窗口背景颜色
	GR_COLOR m_unBorderColor;			///<输入法窗口边框颜色

	void * m_pvImeWinData;					///<输入法窗口的附件参数
} CSIME_WININFO_S;

typedef struct _IMM_INFO_
{
	DWORD m_dwCurImeID;				///<当前输入法标识符
	DWORD m_dwDefImeID;				///<默认输入法标识符
	DWORD m_dwImeMask;				///<当前所有输入法的掩码
	int m_nCount;							///<输入法个数
	int m_nProcRet;							///<输入法管理器返回值
	BOOL m_bUse;							///<输入法管理器是否可用
	BOOL m_bShowFlag;					///<输入法窗口是否显示
	BOOL m_bSetPostResult;				///< 是否设置了输入法返回值处理过程
	BOOL m_bPreserve;					
}CSIMM_INFO_S;

/**
@brief  功能描述:
		输入法的处理回调函数
 
@param[in]
		无
 
@param[out] 
		无
 
@return
 		无
@exception
@note
*/
typedef int (*IMEProcFunc)(CSIMM_EVENT_S* psEvent, CSIME_WININFO_S* psWinInfo);

/**
@brief  功能描述:
		该回调用于判断事件是否需要发送给输入法
 
@param[in]
		CSIMM_EVENT_S* psEvent发送的消息结构体
 
@param[out] 
		无
 
@return
 		如果事件为输入法处理的事件，则返回TRUE，否则返回FALSE
@exception
@note
*/
typedef BOOL (*IMEIsValidEventFunc)(CSIMM_EVENT_S* psEvent);

/******************************************************************************
 *                                 函数声明                                   *
 ******************************************************************************/

/**
@brief  功能描述:
		打开输入法,创建消息队列
 
 @param[in]
 		DWORD dwDefImeID创建时设置默认输入法，此时必须设置为系统的输入法，而不能是用户自定义的输入法，当使用配置文件创建时，该参数可以不做设置
 		
@param[out] 
		无
 
@return
 		如果初始化成功则返回E_IMM_RETURN_OK，否则，根据情况返回不同的值
@exception
@note
*/
int CSIMMInit (DWORD dwDefImeID);


/**
@brief  功能描述:
		销毁输入法
 
@param[in]
		无
 
@param[out] 
		无
 
@return
 		成功销毁后返回E_IMM_RETURN_OK，否则返回其他值
@exception
@note
		销毁失败后，请在输入法完成任务之后再调用此函数
*/
int CSIMMUnInit(void);

/**
@brief  功能描述:
		发送事件到消息队列
 
 @param[in]
 		CSIMM_EVENT_S* psEvent发送的消息结构体
 		
@param[out] 
		无
 
@return
 		成功发送返回E_IMM_RETURN_OK，否则返回其他值
@exception
@note
*/
int CSIMMSendEvent(CSIMM_EVENT_S *psEvent);

/**
@brief  功能描述:
		获取当前输入法管理器的一些信息，比如当前输入法、默认输入法、管理器是否有效、当前窗口是否显示等等
 
@param[in]
		无
 
@param[out] 
		CSIMM_INFO_S *psImmInfo输入法管理器信息结构
 
@return
 		成功获取返回E_IMM_RETURN_OK，否则返回其他值
@exception
@note
*/
int CSIMMGetImmInfo(CSIMM_INFO_S * psImmInfo);

/**
@brief  功能描述:
		如果输入法处于激活状态，则进行输入法的切换操作，否则，启动默认输入法。
 
@param[in]
		void* pvAddData输入法窗口创建参数，如果在切换过程中传入空值，将使用前一次
		输入法的窗口系统，否则，如果为第一次打开输入法窗口，必须要设置该值
 
@param[out] 
		无
 
@return
 		成功启动或切换返回E_IMM_RETURN_OK，否则返回其他值
@exception
@note
*/
int CSIMMOpenOrSwitchIme(void* pvAddData);

/**
@brief  功能描述:
		关闭当前激活状态输入法。
 
@param[in]
		无
 
@param[out] 
		无
 
@return
 		成功关闭返回E_IMM_RETURN_OK，否则返回其他值
@exception
@note
*/
 int CSIMMCloseIme(void);

/**
@brief  功能描述:
		设置返回输入法处理结果的回调函数，只有在设置了该参数之后，输入法的处理结果才会被调用者接收到。
 
@param[in]
		IMMPostResultFunc fnPostResultProc输入法的处理结果返回回调函数
 
@param[out] 
		无
 
@return
 		成功设置返回E_IMM_RETURN_OK，否则返回其他值
@exception
@note
*/
 int CSIMMSetResultProc(IMMPostResultFunc fnPostResultProc);

/**
@brief  功能描述:
		设置输入法能够处理事件的回调函数
 
@param[in]
		IMEIsValidEventFunc fnIsValidEventProc 注册的用于判断输入法是否能够处理事件的接口
 
@param[out] 
		无
 
@return
 		成功注册返回E_IMM_RETURN_OK，否则返回以下值
		E_IMM_RETURN_FALSE = -1,
		E_IMM_RETURN_INITIALED = -2,	///<管理器实例已经初始化
		E_IMM_RETURN_UNINITIAL = -3,	///<输入法管理器未初始化
		E_IMM_RETURN_UNABLED = -4,		///<被禁止的
@exception
@note
*/
 int CSIMMSetIsValidEventProc(IMEIsValidEventFunc fnIsValidEventProc);

 /**
 @brief  功能描述:
        注册输入法接口

 @param[in]
        IMEProcFunc fnImeProc新输入法节点的处理函数
 @param[in]
        IMEIsValidEventFunc fnImeIsValid用于判断事件是否为输入法能够处理的事件，
        该回调可以为NULL，当为NULL时，所有的事件均发送给输入法处理。
 @param[in]
       int nImeCount 输入法的个数

 @param[out] 
 无

 @return
        成功注册，返回E_IMM_RETURN_OK，否则返回其他值
 @exception
 @note
        2010.11.17对该接口进行扩充，增加一个nImeCount参数，
        用于告知管理器，当前注册的输入法引擎中，可能包含多个输入法
 */
 DWORD CSIMMRegisterIme(IMEProcFunc fnImeProc, IMEIsValidEventFunc fnImeIsValid, int nImeCount);

/**
@brief  功能描述:
		卸载指定的输入法
 
@param[in]
		DWORD dwImeID卸载的输入法标识符
 
@param[out] 
		无
 
@return
 		成功卸载，返回E_IMM_RETURN_OK，否则返回其他值
@exception
@note
		只能卸载调用CSIMMRegisterIme()函数注册的输入法，并且输入的节点要有调用者自己去释放
*/
int CSIMMUnRegisterIme(DWORD dwImeID);

/**
@brief  功能描述:
		得到宽字符串字符个数
 
@param[in]
		const char* pchwStr 宽字符字符串
 
@param[out] 
		无
 
@return
 		成功返回字符个数，否则返回0
@exception
@note
		返回字符个数，不是字符串字节长度
*/
int CSIMMwcslen(const char* pchwStr);

/**
@brief  功能描述:
		判断当前输入法是否已打开
 
@param[in]
		无
 
@param[out] 
		无
 
@return
 		如果处于打开状态返回E_IMM_RETURN_OK，否则返回E_IMM_RETURN_FALSE。
@exception
@note
*/
 int CSIMMIsImeActive(void);

#ifdef __cplusplus
}
#endif

#endif  /* __IMM_H_ */

/* 文件结束 */

