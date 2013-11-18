  /******************************************************************************
 FileName:  cs_nes.h
Copyright (c) 2009， Shen Zhen Coship Electronics CO.,LTD.。
 
 Version:       Ver 1.0
 Author:        yinliming(yinliming@coship.com)
 
 File Description:
         Support NES (Nintendo Entertainment System) Interface
 
 Modify History:
 Version     	Date        		Author          	Description
 --------    	----------  	--------        ------------
 1.0         		2009-10-28 	yinliming         	Create
 *******************************************************************************/
 #ifndef CSNES_H
#define CSNES_H
 
#ifdef __cplusplus
 extern "C" {
#endif

#define CSNES "CSNES"

/*
@brief 定义NES模块初始化参数
*/
typedef struct _NESInitParam
 {
     int nClientId;                  ///<GrOpen返回的ClientID
     int nParentWinId;          ///<nes 窗口的父窗口ID, 小于等于0时，认为是根Window
     int nX;
     int nY; 
     int nWidth;
     int nHeight;                     ///<nes窗口的位置和大小,位置相对于其父窗口
 
     int nEvtFromFlag;     ///<nes系统接收事件的方式
                                      ///<0 -nes auto get event form microwin
                                     ///<1 caller post event to nes by CSNESHandEvent
 }CSNESInitParams_S;

typedef enum
{
	EM_NES_UNKNOW	///
}CSNesOption_E;

/**
@brief 模块初始化

@param[in] 初始化参数，详见CSNESInitParams_S的定义
@return 成功返回NES的窗口ID,失败返回-1
@note 进入NES是调用
*/
int CSNESInit(CSNESInitParams_S *pNesInitParams);

/**
@brief 加载启动游戏

@param[in] pNesFileName 传入游戏的路径
@return -1表示失败
@note 无
*/
int CSNESLoadGame(const char*pNesFileName); 

/**
@brief 通过此接口将按键信息传递给NES

@param [in] unKeyType: 按键类型，
					EM_UDIINPUT_KEY_PRESS		按键按下	
					EM_UDIINPUT_KEY_RELEASE	按键释放
@param [in] unKeyCode  对应键值,基于udi的键值
					CSUDI_VK_UP				对应游戏手柄【向左键】 
					CSUDI_VK_RIGHT			对应游戏手柄【向上键】
					CSUDI_VK_DOWN			对应游戏手柄【向右键】
					CSUDI_VK_LEFT			对应游戏手柄【向下键】
					
					CSUDI_VK_6				对应游戏手柄右半部份最上面的按键
					CSUDI_VK_8				对应游戏手柄右半部份最右边的按键
					CSUDI_VK_4				对应游戏手柄右半部份最下面的按键
					CSUDI_VK_2				对应游戏手柄右半部份最左面的按键
					
					???(需要确定)				对应游戏手柄【L1】键
					???(需要确定)				对应游戏手柄【R1】键
					???(需要确定)				对应游戏手柄【L2】键
					???(需要确定)				对应游戏手柄【L2】键

					???(需要确定)				对应游戏手柄【SELECT】键
					???(需要确定)				对应游戏手柄【START】键
					
@param [in] pReserve  保留参数，必须传NULL	
@return  成功返回0，否则返回其他值
@note 如果需要支持多个游戏手柄，则此接口可能需要修改添加参数
*/
int CSNESHandEvent(unsigned int unKeyType, unsigned int unKeyCode,void * pReserve);

/**
@brief 设置选项

@param[in] eOption 详见CSNesOption_E描述
@param[in] wParam key-value对中的key
@param[in] lParam 	key-value对中的value
@return 无
@note无
*/
void CSNESSetOption(CSNesOption_E eOption, void *wParam, void *lParam);

/**
@brief 退出游戏

@param[in] 无
@return 无
@note 此接口是同步的，只有游戏真正退出以后才返回
*/
void CSNESExitGame();

#ifdef __cplusplus
 }
#endif
#endif

