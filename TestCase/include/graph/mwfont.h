/**
文件名称：ADAPTERINTERFACE.H
版权所有(c)  2009， 同洲电子。
 
@version  Ver 1.0    
@author   龙式荣 (longshirong@coship.com)
@file
		文件功能说明
 
History:
版本号		日期		作者		修改内容
---------	     --------        -------   	-------------
1.0			2009-9-17	龙式荣		创建
1.0                2010-7-27   龙式荣           增加接口注释
*/

#ifndef _MWFONT_H_
#define _MWFONT_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 头文件引用                                 *
 ******************************************************************************/

/******************************************************************************
 *                                 宏、定义、结构体                           *
 ******************************************************************************/
#define GR_SET_GRAY_MONOCHROME                  0
//#define GR_SET_GRAY_GRAYSCALE_4                     1
#define GR_SET_GRAY_GRAYSCALE_256                 2

#define GR_FONT_STYLE_NORMAL 	0x00		//普通
#define GR_FONT_STYLE_ITALIC 		0x01		//斜体
#define GR_FONT_STYLE_BOLD   		0x02		//粗体
#define GR_FONT_STYLE_OTHER  		0x04		//扩展使用

/**@brief 字模信息结构体

描述获取字模时的输入参数和输出参数
*/
typedef struct
{
	//input
	char*				m_pcBuff;			///< 字模缓冲区内存
	unsigned short		m_usBuffPitch;		///< 字模缓冲区每行字节数
	unsigned short		m_usBuffHeight;	///< 字模缓冲区的高度

	//output
	unsigned short		m_usOffX;			///< 字模数据相对缓冲区的X方向偏移
	unsigned short		m_usOffY;			///< 字模数据相对缓冲区的Y方向偏移
	int					m_nAdvance;		///< 字符的实际宽度
	int 					m_nGrayLevel;	 	///< 字模数据的灰度
}GR_FONT_CHAR_INFO;

typedef struct 
{
	/**
	@brief  功能描述:
			从文件中创建字体
	 
	@param[in]
			pcFileName字体数据文件路径

	@param[in]
			nFontSize字体大小，取值范围为5~100

	@param[in]
			nMode灰度级别，为适配层中使用的灰度级别，与字库引擎本身使用的灰度级别标志可能有出入，需要重新进行匹配，比如文鼎字库只支持0和2两个灰度级别，分别代码2级灰度和256级灰度，而华文字库的灰度级别是用位深度来表示灰度级别，因此需转换成统一的适配层中支持的格式。（当前适配层使用的是文鼎字库格式来表示灰度级别，这是需要完善的地方）
	 
	@param[out] 
			无
	 
	@return
	 		字体句柄，创建失败返回0
	@exception
	@note
	*/
	int (*Font_CreateFromFile_Func)(const char *pcFileName, int nFontSize, int nMode);

	/**
	@brief  功能描述:
			从内存中创建字体
	 
	@param[in]
			pcFontData字体数据缓冲区指针

	@param[in]
			nDataLen字体数据缓冲区大小

	@param[in]
			nFontSize字体大小，取值范围为5~100

	@param[in]
			nMode灰度级别
	 
	@param[out] 
			无
	 
	@return
	 		字体句柄，创建失败返回0
	@exception
	@note
	*/
	int (*Font_CreateFromMem_Func)(const char *pcFontData, int nDataLen,int nFontSize, int nMode);	

	/**
	@brief  功能描述:
			销毁字体句柄
	 
	@param[in]
			nFontHandle字体句柄
	 
	@param[out] 
			无
	 
	@return
	 		成功销毁字体返回0，否则返回非0负值
	@exception
	@note
	*/
	int (*Font_Destroy_Func)(int nFontDescriptor);	

	/**
	@brief  功能描述:
			设置字体大小
	 
	@param[in]
			nFontDescriptor字体句柄
			
	@param[in]
			nSize字体大小，取值范围为5~100
	 
	@param[out] 
			无
	 
	@return
	 		成功设置返回0，否则返回非0负值
	@exception
	@note
	*/
	int (*Font_SetSize_Func)(int nFontDescriptor, int nFontSize);	

	/**
	@brief  功能描述:
			设置字体风格
	 
	@param[in]
			nFontDescriptor字体句柄
			
	@param[in]
			nStyle风格类型，主要有:
			GRA_STYLE_NORMAL        	0x00
			GRA_STYLE_BOLD          		0x01
			GRA_STYLE_ITALIC        		0x02
			GRA_STYLE_UNDERLINE     	0x04
			GRA_STYLE_FIXEDWIDTH   	0x08
	 
	@param[out] 
			无
	 
	@return
	 		成功设置返回0，否则返回非0负值
	@exception
	@note
			字体风格的变化需要第三方字库的支持
	*/
	int (*Font_SetAttr_Func)(int nFontDescriptor, int nFontStyle);	

	/**
	@brief  功能描述:
			获取单个字符的宽度 
			
	@param[in] 
			int nFontDescriptor 当前字体的句柄
			
	@param[in] 
			unsigned short usCharID 字符的编码，为UNICODE编码值 
			
	@param[out] 
			GR_FONT_CHAR_INFO *psCharInfo字模信息参数返回 
			
	@return 		
			获取字模的状态，成功则返回0，否则返回非0负值
			
	@exception
	@note
			该函数现在需要实现两个功能：
			当GR_FONT_CHAR_INFO结构体成员m_pcBuff为NULL时，此时是用于获取当前字库的灰度级别值，当m_pcBuff不为NULL时，才是正常情况下获取字符字模数据的功能。
	*/

	int (*Font_GetFont_Func)(int nFontDescriptor, unsigned short usCharID, GR_FONT_CHAR_INFO *psCharInfo);

	/**
	@brief  功能描述:
			获取字符宽度
	 
	@param[in]
			nFontHandle字体句柄
			
	@param[in]
			usCharID字符编码,为UNICODE编码值 
	 
	@param[out] 
			pnAdvance字符的实际宽度，如果是 错误值则返回0
	 
	@return
	 		成功获取宽度返回0，否则返回非0负值

	@exception
	@note
	*/
	int (*Font_GetAdvance_Func)(int nFontDescriptor, unsigned short usCharID, int *pnAdvance);	

	/**
	@brief  功能描述:		
			获取当前字体的基线 
	@param[in] 
			int nFontDescriptor当前字体的句柄 

	@param[out] 
			int *pnBaseLine 当前字体的基线 

	@return 		
			获取基线后的状态，成功则返回0，否则返回非0负值	

	@exception
	@note
	*/
	int (*Font_GetBaseLine_Func)(int nFontDescriptor, int *pnBaseLine);	

	/**
	@brief  功能描述:		
			判断当前字库数据是否与引擎是一致  

	@param[in] 
			const char *pcMarker   当前字库的特征字缓冲区 

	@param[out] 		无 

	@return 		
			当前字库于引擎一致则返回0，否则返回非0负值	

	@exception
	@note
			这个函数很重要，必须通过该函数，才能确定数据与引擎的一致。实现逻辑：
			将pcMarker与引擎使用的数据关键字进行比较，如果是引擎所使用的数据，则返回0，否则返回非0值
	*/
	int (*Font_IsMine_Func)(const char *pcMarker);												
}GR_FONT_Driver_Proc;

/******************************************************************************
 *                                 函数声明                                   *
 ******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif  /* _MWFONT_H_ */

/* 文件结束 */

