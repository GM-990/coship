#ifndef CSSHELL_TR069_H
#define CSSHELL_TR069_H

#include "mmcp_typedef.h"
#include "cs_tr069.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief TR069类型枚举
@note 这里只是定了几种常见的TR069 标准类型，并不满足所有项目需求。项目可以定义自己的TR069类型。
项目定义TR069类型时一定要注意:
1.自定义TR069类型值一定要大于EM_CSSHELL_TR069_TYPE_USERSTART
*/
typedef enum
{
	EM_CSSHELL_TR069_TYPE_COSHIP = 1,
	EM_CSSHELL_TR069_TYPE_SHTEL,
	EM_CSSHELL_TR069_TYPE_HENAN,
	EM_CSSHELL_TR069_TYPE_OTHER,
	EM_CSSHELL_TR069_TYPE_USERSTART = 0x100
}CSShellTR069Type_E;


/*
*@brief TR069初始化参数,定义了一系列函数指针，这些函数指针由应用或者shell   的TR069实现(C代码)，然后注册给shell。
*/
typedef struct _CSShellTR069InitParam
{
	int m_nSize;	///<本结构大小，一定等于sizeof(CSShellTR069InitParam_S)
	CSShellTR069Type_E m_eType;	//TR069的类型
	/**
	@brief 前端网管系统的URL 
	*/
	char *m_pcUrl;
			/**
	@brief 前端网管系统的用户名
	*/
	char *m_pcUserName;
					/**
	@brief 前端网管系统的密码
	*/
	char *m_pcPassWord;
					
	/**
	@brief 机顶盒管理数据模型(结构体数组)
	*/
	CSTR069_Item_S *m_psItem;
	
	/**
	@brief 机顶盒管理数据模型中数据的个数
	*/
	int m_nItemLen;

	/**
	@brief 基本inform信息表(字符型指针数组)
	*/
	char **m_ppcInform;
	
	/**
	@brief 基本inform信息表中信息个数
	*/
	int m_nInformNum;

	/**
	@brief inform信息表中DeviceId信息 (字符型指针数组)
	*/
	char **m_ppcInformDeviceId;
	
	/**
	@brief inform信息表中DeviceId信息个数
	*/
	int m_nInformDeviceIdNum;

	/**
	@brief Period  inform信息表 (字符型指针数组)
	*/
	char **m_ppcInformPeriod;
	
	/**
	@brief Period  inform信息表中Period信息个数
	*/
	int m_nInformPeriodNum;
	
	/**
	@brief Trape  inform信息表 (字符型指针数组)
	*/
	char **m_ppcInformTrape;

	/**
	@brief Trape  inform信息表中Trape信息个数
	*/
	int m_nInformTrapeNum;
	
     	/**
     	@brief 设置被管理的数据模型的参数的回调函数
     	@param pcParaName 数据名称；pcParaValue 数据值；nParaLen 数据值长度;unUserData用户收数据
     	*/
	int (* m_fnSetPara)(char* pcParaName,char *pcParaValue,int nParaLen,unsigned int unUserData);

     	/**
     	@brief 获取被管理的数据模型的参数的回调函数
     	@param pcParaName 数据名称；pcParaValue 数据值；nParaLen 数据值长度(输入/输出);unUserData用户收数据
     	*/
	int (* m_fnGetPara)(char* pcParaName,char *pcParaValue,int *pnParaLen,unsigned int unUserData);

	/**
	@brief Trape  用户数据
	*/
	unsigned int m_unUserData;
}CSShellTR069InitParam_S;


/**
@brief 设置tr069 参数接口
@param psParam tr069 shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSShellTR069SetParam(CSShellTR069InitParam_S * psParam);


#ifdef __cplusplus
}
#endif

#endif

