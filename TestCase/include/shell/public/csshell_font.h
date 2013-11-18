/**@defgroup CSSHELL_FONT_H_
@brief ��ģ��Ϊ�ٻ��м����shell�ֿ�ӿ�
@note �û�����ʹ��shell�ڲ����ֿ⣬Ҳ����ʹ���û��Լ����ֿ�
@{
*/
#ifndef CSSHELL_FONT_H_
#define CSSHELL_FONT_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
	extern "C"
	{
#endif
/**
@brief �����ʼ�������õ��Ľṹ 
*/
typedef struct _CSShellFont
{
	const unsigned char* m_pcMatrixData;	///<��ģ����
	int m_nLength;			///<��ģ���ݵĳ���
}CSShellFont_S;

/**
@brief ���Ķ����������ʼ�������ֿ�
@param[in] �ֿ�
@note ������graph��ʼ��֮ǰ���øýӿ�
@note ͨ������£�Ӧ�ÿ���ͨ��get�ӿڻ�ȡ��Ӧ������ṹ�壬����Ҫ�Լ����ø�psFont�ṹ��ʱ����ʼ�������ͷŸýṹ��
@see CSShellFontGetDefaultArpFont,CSShellFontGetGBKArpFont,CSShellFontGetAsciiArpFont,CSShellFontGetBig5ArpFont
@code
//��ʹ��Ĭ�ϵ��Ķ��ֿ�Ϊ��
CSShellFontInitArpFont(CSShellFontGetDefaultArpFont());
@endcode
*/
void CSShellFontInitArpFont(CSShellFont_S * psFont);

/**
@brief �û������������ʼ�������ֿ�
@param[in] �ֿ�
@note ������graph��ʼ��֮ǰ���øýӿ�
@note ͨ������£�Ӧ�ÿ���ͨ��get�ӿڻ�ȡ��Ӧ������ṹ�壬����Ҫ�Լ����ø�psFont�ṹ��ʱ����ʼ�������ͷŸýṹ��
@note ���ڰ�Ȩ���⣬shellĬ�ϲ��ṩ�����ֿ⡣���ṩ�˳�ʼ�������ֿ�Ľӿڡ������ĿҪʹ�û����ֿ⣬��ȷ�ϻ����Ȩ�󣬽������ֿ������Ŀ�б��뼴��
@see CSShellFontGetDefaultStfFont
@code 
//��ʹ��Ĭ�ϵĻ����ֿ�Ϊ��
CSShellFontInitStfFont(CSShellFontGetDefaultStfFont());
@endcode
*/
void CSShellFontInitStfFont(CSShellFont_S * psFont);

/**
@brief ��ȡĬ�ϵ��Ķ������ֿ�,��GB2312Plus
@return �Ķ��ֿ�GB2312Plus
*/
CSShellFont_S * CSShellFontGetDefaultArpFont(void);	//

/**
@brief ��ȡ���Ķ������GBK�ֿ�
@return �Ķ��ֿ�GBK
*/
CSShellFont_S * CSShellFontGetGBKArpFont(void);

/**
@brief ��ȡ���Ķ������к������GB2312�ֿ�
@return �Ķ������к������GB2312�ֿ�
*/
CSShellFont_S * CSShellFontGetJXZHArpFont(void);

/**
@brief ��ȡ���Ķ�����Ascii�ֿ�
@return �Ķ��ֿ�Ascii
*/
CSShellFont_S * CSShellFontGetAsciiArpFont(void);

/**
@brief ��ȡ���Ķ�����Ascii�ֿ�
@return �Ķ��ֿ�Ascii
*/
CSShellFont_S * CSShellFontGetBig5ArpFont(void);

/**
@brief ��ȡĬ�ϵĻ����ֿ�
@return ����Ĭ���ֿ�
@note  �����ֿ�Ҫ���á��漰�������⡣������Ӫ��Ҫ��ʹ�ø��ֿ⣬���Ѹ��ѣ�������ʹ��
*/
CSShellFont_S * CSShellFontGetDefaultStfFont(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif//_CS_EW_FONTREGISTER_H_

