/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

#ifndef _UDI2_PANEL_PLUS_H_
#define _UDI2_PANEL_PLUS_H_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "udi2_typedef.h"


#define	UDIPANEL_LED_MAX_COM_PIN 6

/**@brief Panel模块Panel类型枚举定义*/
typedef enum
{
	EM_PANEL_TYPE_COMMON_LED = 0, ///<普通LED面板
	EM_PANEL_TYPE_MCU_LED,	///<使用单片机控制的LED面板
	EM_PANEL_TYPE_COMMON_VFD, ///<普通VFD面板
	EM_PANEL_TYPE_MCU_VFD,	///<使用单片机控制的VFD面板
	EM_PANEL_TYPE_SPI_LED,  ///<面板使用pt6964,其具有锁存功能，采用SPI协议
	EM_PANEL_TYPE_COMMON_NO_LED,  ///<普通面板使用74HC164，无LED
	EM_PANEL_TYPE_GPIO_LED, ///<普通面板，全部gpio控制，没有74H164，有LED
	EM_PANEL_TYPE_GPIO_NO_LED,///<普通面板，gpio控制按键，没有led不使用164。
	EM_PANEL_TYPE_MAX
}CSUDIPanelType_E;


/**@brief Panel模块普通LED面板硬件配置结构体(引脚复用)*/
typedef struct
{
	unsigned char m_aucGpioComPin[UDIPANEL_LED_MAX_COM_PIN]; 	///< LED面板位选线的GPIO口，参考硬件设计
	                             ///<pt6964面板  0: m_nGPIOSTB, 1: 中断引脚1, 2: 中断引脚2
	                             ///<其余备用
	                             ///<注意:不使用的pin配置请赋值为0xFF
	unsigned char m_ucGpioData;	 ///< 连接面板164芯片的Data引脚的GPIO
	                             ///<pt6964面板  m_nGPIOData
	                             ///<注意:不使用的pin配置请赋值为0xFF
	unsigned char m_ucGpioClk;	 ///< 连接面板164芯片的clock引脚的GPIO
	                             ///<pt6964面板  m_nGPIOCLK
	                             ///<注意:不使用的pin配置请赋值为0xFF
	unsigned char m_ucGpioKey; 	 ///< 面板扫描按键的 GPIO
	                             ///<pt6964面板  备用
	                             ///<注意:不使用的pin配置请赋值为0xFF
	unsigned char m_aucReserved[3];///<pt6964面板  备用
								   ///<注意:不使用的pin配置请赋值为0xFF
	                            
}CSUDIPanelCommonLedCfg_S;


/**@brief Panel模块普通VFD面板硬件配置结构体*/
typedef struct
{
	unsigned char m_ucGpioSTB;		///< 使能控制脚，不使用时一定设为0xff
	unsigned char m_ucGpioDIn; 		///< 数据输入脚，机顶盒通过该引脚从PT6315 读数据，不使用时一定设为0xff
	unsigned char m_ucGpioDout; 	///< 数据输出脚，机顶盒通过该引脚向PT6315写数据 ，不使用时一定设为0xff
	unsigned char m_ucGpioClk; 		///< 时钟 线，不使用时一定设为0xff
}CSUDIPanelCommonVfdCfg_S;


/**@brief Panel模块MCU控制的面板硬件配置结构体，单片机控制的LED和VFD面板均使用此数据结构*/
typedef struct
{
	int m_nUartIndex;	///<主芯片与单片机通信用的串口索引
	unsigned int m_unBaudrate;		///<串口使用的波特率
}CSUDIPanelMcuCfg_S;

/**@brief Panel模块，普通面板，由gpio控制的面板使用此数据结构
类型为EM_PANEL_TYPE_GPIO_NO_LED的面板使用该结构体的m_ulKeyNum和m_ucKeyPinGroup[8]两个成员，其余的成员设为0xff。
类型为EM_PANEL_TYPE_GPIO_LED的面板该结构体的所有成员都需要配置，不使用的gpio设置为0xff.*/
typedef struct
{
	unsigned char m_ucLedSegPinGroup[8];//<数码管seg管脚，不使用时一定设为0xff
	unsigned char m_ucLedComPinGroup[7];//<数码管com管脚,不使用时一定设为0xff
	unsigned char m_ucLedStandby;//<数码管standby管脚，不使用时一定设为0xff
	unsigned int  m_ulKeyNum;////按键数量，不使用时一定设为0xff
	unsigned char m_ucKeyPinGroup[8];//按键gpio配置，不使用时一定设为0xff
}CSUDIPanelGpioCfg_S;

/**@brief Panel模块硬件信息结构体定义*/
typedef struct
{
	CSUDIPanelCommonLedCfg_S m_sLEDCfg; ///<普通LED面板硬件配置，对应于EM_PANEL_TYPE_COMMON_LED
	CSUDIPanelCommonVfdCfg_S m_sVFDCfg; ///<普通VFD面板硬件配置，对应于EM_PANEL_TYPE_COMMON_VFD
	CSUDIPanelMcuCfg_S m_sMCUCfg; ///<MCU控制的面板硬件配置,对应于EM_PANEL_TYPE_MCU_LED 和 EM_PANEL_TYPE_MCU_VFD
}CSUDIPanelHWCfg_S;

/**@brief panel initilize param structure*/
typedef struct _CSUDIPanelParams_S
{
	int m_nMySize; ///<此结构体大小，sizeof(CSUDIPanelParams_S)
	CSUDIPanelType_E m_ePanelType;	///<panel类型
	CSUDIPanelHWCfg_S m_ePanelHwCfg;///<各种类型的panel硬件配置
}CSUDIPanelParams_S;

/**
@brief Panel模块初始化
@param[in] nIndex panel索引
@param[in] psParms 初始化Panel模块所需要的参数
@return CSUDI_SUCCESS表示成功,错误设备号返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID,初始化失败返回CSUDI_FAILURE
@note: 该接口已不推荐使用,请使用CSUDIPanelInitExt
*/
CSUDI_Error_Code CSUDIPanelInit(int nIndex, CSUDIPanelParams_S *psParms);

/**
@brief Panel模块初始化
@param[in] nIndex panel索引
@param[in] ePanelType panel类型
@param[in] pParms 该参数根据ePanelType指定的不同panel类型，而指向不同的数据结构，详细如下：
	EM_PANEL_TYPE_COMMON_LED：pParms为指向CSUDIPanelCommonLedCfg_S的指针
	EM_PANEL_TYPE_SPI_LED：pParms为指向CSUDIPanelCommonLedCfg_S的指针
	EM_PANEL_TYPE_COMMON_NO_LED：pParms为指向CSUDIPanelCommonLedCfg_S的指针
	EM_PANEL_TYPE_MCU_LED：pParms为指向CSUDIPanelMcuCfg_S的指针
	EM_PANEL_TYPE_GPIO_LED：pParms为指向CSUDIPanelGpioCfg_S的指针
	EM_PANEL_TYPE_GPIO_NO_LED：pParms为指向CSUDIPanelGpioCfg_S的指针
@return 
	-成功返回 CSUDI_SUCCESS
	-错误设备号返回 CSUDIPANEL_ERROR_INVALID_DEVICE_ID
	-参数错误返回 CSUDIPANEL_ERROR_BAD_PARAMETER
	-初始化失败返回 CSUDI_FAILURE
@note 
*/
CSUDI_Error_Code CSUDIPanelInitExt(int nIndex,CSUDIPanelType_E ePanelType,void *pParms);

/**
@brief 普通LED面板初始化,调用此接口初始化此类型面板可减小链接库的大小
*/
CSUDI_Error_Code CSUDIPanelInitCommonLed(int nIndex, CSUDIPanelParams_S *psParms);

/**
@brief 普通VFD面板初始化,调用此接口初始化此类型面板可减小链接库的大小
*/
CSUDI_Error_Code CSUDIPanelInitCommonVfd(int nIndex, CSUDIPanelParams_S *psParms);

/**
@brief 单片机控制的LED面板初始化,调用初始化此类型面板可减小链接库的大小
*/
CSUDI_Error_Code CSUDIPanelInitMCULed(int nIndex, CSUDIPanelParams_S *psParms);

/**
@brief 单片机控制的VFD面板初始化,调用初始化此类型面板可减小链接库的大小
*/
CSUDI_Error_Code CSUDIPanelInitMCUVfd(int nIndex, CSUDIPanelParams_S *psParms);


#ifdef __cplusplus
}
#endif

#endif

