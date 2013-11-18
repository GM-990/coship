/**
�ļ����ƣ�ADAPTERINTERFACE.H
��Ȩ����(c)  2009�� ͬ�޵��ӡ�
 
@version  Ver 1.0    
@author   ��ʽ�� (longshirong@coship.com)
@file
		�ļ�����˵��
 
History:
�汾��		����		����		�޸�����
---------	     --------        -------   	-------------
1.0			2009-9-17	��ʽ��		����
1.0                2010-7-27   ��ʽ��           ���ӽӿ�ע��
*/

#ifndef _MWFONT_H_
#define _MWFONT_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 ͷ�ļ�����                                 *
 ******************************************************************************/

/******************************************************************************
 *                                 �ꡢ���塢�ṹ��                           *
 ******************************************************************************/
#define GR_SET_GRAY_MONOCHROME                  0
//#define GR_SET_GRAY_GRAYSCALE_4                     1
#define GR_SET_GRAY_GRAYSCALE_256                 2

#define GR_FONT_STYLE_NORMAL 	0x00		//��ͨ
#define GR_FONT_STYLE_ITALIC 		0x01		//б��
#define GR_FONT_STYLE_BOLD   		0x02		//����
#define GR_FONT_STYLE_OTHER  		0x04		//��չʹ��

/**@brief ��ģ��Ϣ�ṹ��

������ȡ��ģʱ������������������
*/
typedef struct
{
	//input
	char*				m_pcBuff;			///< ��ģ�������ڴ�
	unsigned short		m_usBuffPitch;		///< ��ģ������ÿ���ֽ���
	unsigned short		m_usBuffHeight;	///< ��ģ�������ĸ߶�

	//output
	unsigned short		m_usOffX;			///< ��ģ������Ի�������X����ƫ��
	unsigned short		m_usOffY;			///< ��ģ������Ի�������Y����ƫ��
	int					m_nAdvance;		///< �ַ���ʵ�ʿ��
	int 					m_nGrayLevel;	 	///< ��ģ���ݵĻҶ�
}GR_FONT_CHAR_INFO;

typedef struct 
{
	/**
	@brief  ��������:
			���ļ��д�������
	 
	@param[in]
			pcFileName���������ļ�·��

	@param[in]
			nFontSize�����С��ȡֵ��ΧΪ5~100

	@param[in]
			nMode�Ҷȼ���Ϊ�������ʹ�õĻҶȼ������ֿ����汾��ʹ�õĻҶȼ����־�����г��룬��Ҫ���½���ƥ�䣬�����Ķ��ֿ�ֻ֧��0��2�����Ҷȼ��𣬷ֱ����2���ҶȺ�256���Ҷȣ��������ֿ�ĻҶȼ�������λ�������ʾ�Ҷȼ��������ת����ͳһ���������֧�ֵĸ�ʽ������ǰ�����ʹ�õ����Ķ��ֿ��ʽ����ʾ�Ҷȼ���������Ҫ���Ƶĵط���
	 
	@param[out] 
			��
	 
	@return
	 		������������ʧ�ܷ���0
	@exception
	@note
	*/
	int (*Font_CreateFromFile_Func)(const char *pcFileName, int nFontSize, int nMode);

	/**
	@brief  ��������:
			���ڴ��д�������
	 
	@param[in]
			pcFontData�������ݻ�����ָ��

	@param[in]
			nDataLen�������ݻ�������С

	@param[in]
			nFontSize�����С��ȡֵ��ΧΪ5~100

	@param[in]
			nMode�Ҷȼ���
	 
	@param[out] 
			��
	 
	@return
	 		������������ʧ�ܷ���0
	@exception
	@note
	*/
	int (*Font_CreateFromMem_Func)(const char *pcFontData, int nDataLen,int nFontSize, int nMode);	

	/**
	@brief  ��������:
			����������
	 
	@param[in]
			nFontHandle������
	 
	@param[out] 
			��
	 
	@return
	 		�ɹ��������巵��0�����򷵻ط�0��ֵ
	@exception
	@note
	*/
	int (*Font_Destroy_Func)(int nFontDescriptor);	

	/**
	@brief  ��������:
			���������С
	 
	@param[in]
			nFontDescriptor������
			
	@param[in]
			nSize�����С��ȡֵ��ΧΪ5~100
	 
	@param[out] 
			��
	 
	@return
	 		�ɹ����÷���0�����򷵻ط�0��ֵ
	@exception
	@note
	*/
	int (*Font_SetSize_Func)(int nFontDescriptor, int nFontSize);	

	/**
	@brief  ��������:
			����������
	 
	@param[in]
			nFontDescriptor������
			
	@param[in]
			nStyle������ͣ���Ҫ��:
			GRA_STYLE_NORMAL        	0x00
			GRA_STYLE_BOLD          		0x01
			GRA_STYLE_ITALIC        		0x02
			GRA_STYLE_UNDERLINE     	0x04
			GRA_STYLE_FIXEDWIDTH   	0x08
	 
	@param[out] 
			��
	 
	@return
	 		�ɹ����÷���0�����򷵻ط�0��ֵ
	@exception
	@note
			������ı仯��Ҫ�������ֿ��֧��
	*/
	int (*Font_SetAttr_Func)(int nFontDescriptor, int nFontStyle);	

	/**
	@brief  ��������:
			��ȡ�����ַ��Ŀ�� 
			
	@param[in] 
			int nFontDescriptor ��ǰ����ľ��
			
	@param[in] 
			unsigned short usCharID �ַ��ı��룬ΪUNICODE����ֵ 
			
	@param[out] 
			GR_FONT_CHAR_INFO *psCharInfo��ģ��Ϣ�������� 
			
	@return 		
			��ȡ��ģ��״̬���ɹ��򷵻�0�����򷵻ط�0��ֵ
			
	@exception
	@note
			�ú���������Ҫʵ���������ܣ�
			��GR_FONT_CHAR_INFO�ṹ���Աm_pcBuffΪNULLʱ����ʱ�����ڻ�ȡ��ǰ�ֿ�ĻҶȼ���ֵ����m_pcBuff��ΪNULLʱ��������������»�ȡ�ַ���ģ���ݵĹ��ܡ�
	*/

	int (*Font_GetFont_Func)(int nFontDescriptor, unsigned short usCharID, GR_FONT_CHAR_INFO *psCharInfo);

	/**
	@brief  ��������:
			��ȡ�ַ����
	 
	@param[in]
			nFontHandle������
			
	@param[in]
			usCharID�ַ�����,ΪUNICODE����ֵ 
	 
	@param[out] 
			pnAdvance�ַ���ʵ�ʿ�ȣ������ ����ֵ�򷵻�0
	 
	@return
	 		�ɹ���ȡ��ȷ���0�����򷵻ط�0��ֵ

	@exception
	@note
	*/
	int (*Font_GetAdvance_Func)(int nFontDescriptor, unsigned short usCharID, int *pnAdvance);	

	/**
	@brief  ��������:		
			��ȡ��ǰ����Ļ��� 
	@param[in] 
			int nFontDescriptor��ǰ����ľ�� 

	@param[out] 
			int *pnBaseLine ��ǰ����Ļ��� 

	@return 		
			��ȡ���ߺ��״̬���ɹ��򷵻�0�����򷵻ط�0��ֵ	

	@exception
	@note
	*/
	int (*Font_GetBaseLine_Func)(int nFontDescriptor, int *pnBaseLine);	

	/**
	@brief  ��������:		
			�жϵ�ǰ�ֿ������Ƿ���������һ��  

	@param[in] 
			const char *pcMarker   ��ǰ�ֿ�������ֻ����� 

	@param[out] 		�� 

	@return 		
			��ǰ�ֿ�������һ���򷵻�0�����򷵻ط�0��ֵ	

	@exception
	@note
			�����������Ҫ������ͨ���ú���������ȷ�������������һ�¡�ʵ���߼���
			��pcMarker������ʹ�õ����ݹؼ��ֽ��бȽϣ������������ʹ�õ����ݣ��򷵻�0�����򷵻ط�0ֵ
	*/
	int (*Font_IsMine_Func)(const char *pcMarker);												
}GR_FONT_Driver_Proc;

/******************************************************************************
 *                                 ��������                                   *
 ******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif  /* _MWFONT_H_ */

/* �ļ����� */

