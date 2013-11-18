#ifndef _CS_STRINGS_H_
#define _CS_STRINGS_H_
#include "mmcp_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif


/**
@brief ������ֱ����CodePage
@note ��MMCP_Str2Unicode ��MMCP_Unicode2Strʹ��
*/
#define CP_DEFAULT	0
#define CP_GB2312	936			/*���������ַ�����codepage(��½���¼��µȵ���ʹ��)*/
#define CP_BIG5		950		/*̨�����ʹ�õķ����ַ�����codepage*/
#define CP_UTF8		65001	/*UTF8�����ַ�����codepage*/

#define CP_ISO8859_1 28591   /*ISO8859ϵ��codepage*/
#define CP_ISO8859_2 28592	
#define CP_ISO8859_3 28593
#define CP_ISO8859_4 28594
#define CP_ISO8859_5 28595
#define CP_ISO8859_6 28596
#define CP_ISO8859_7 28597
#define CP_ISO8859_8 28598
#define CP_ISO8859_9 28599
#define CP_ISO8859_10 28600
#define CP_ISO8859_13 28603
#define CP_ISO8859_14 28604
#define CP_ISO8859_15 28605

#define CP_ISO6937 6937

#define CP_WIN1251  1251	/*CP_WIN1251��CP_1251K���Ƕ���ϵcodepage���ӹ��Ҳ�ͬʹ�ò�ͬ*/	
#define CP_1251K    1351	
#define CP_1252     1252	/*Windows Latin-1,��CP_ISO8859_1��һ��������CP_ISO8859_1ռ����1252���codepageֵ�����Զ���Ϊ��ֵ*/
#define CP_PORTUGUEUE 28606	/*����ϵcodepage���Ǳ�׼*/

#define CP_UNICODE_LE	1200		/*Unicode�ַ���������ֽ���*/
#define CP_UNICODE_BE	1201		/*Unicode�ַ�����С���ֽ���*/

#define ENCODE_BIG 0x10000		/*����ֽ���*/
#define ENCODE_LITTLE 0		/*С���ֽ���*/

/**
@brief ����Ĭ��ת�뷽ʽ,��֧������Unicode�Ĵ�С���������
@param[in] nCodePage	ȡ����ĺ궨�壬ָ��ת�����ַ����ı��뷽ʽ������:big5 UTF ASCII ���
@param[out] ��
@return ��
@note ��������codePage��������涨���ֵ����ASCII(������)ת��
*/
void MMCP_SetDefaultCodePage(int nCodePage);

/**
@brief ���Ĭ��ת�뷽ʽ

@param[in] ��
@param[out] ��
@return ����Ĭ�ϵı��뷽ʽ
@note��
*/
int MMCP_GetDefaultCodePage(void);

/**
@brief ת��ָ���ı��뷽ʽ���ַ���ΪUnicode �����ַ���

@param[in] flag	ָ��convertString�ı��뷽ʽ��UNICODE����ֽ���ȡֵΪ�����뷽ʽ|��С�ˣ���
		ȡֵ��Χ��ENCODE_BIG|codePage,��ENCODE_LITTLE|codePage,��ֱ��ȡcodePage������codePageȡֵ����Ϊ0��
@param[in] convertString    ��Ҫת����ַ����ı������С�
@param[in] convertStrLen   ��Ҫת����ַ������ȣ�Ҳ����ռ���ٸ��ֽڣ���
@param[out] unicodeStringBuffer  ���ת���Uniocde�����buffer��
@param[in] maxUnicodeCnt  ���������unicodeStringBuffer����ܴ��unicode����ĸ�����Ҳ����unicodeStringBuffer.length/2;
						(ÿ��Unicode����ռ�����ֽڣ�ȡֵ��ΧmaxUnicodeCnt >= convertStrLen+1)

@return ʵ��ת�����ַ��ĸ�������Unicode����ĸ�����ÿ��Unicode����ռ�����ֽڣ���

@note:
	����:
	ʹ��CP_BIG5��ʽת��,Ҫ���������ֽ�����flagȡֵCP_BIG5|ENCODE_BIG 
	ʹ��CP_BIG5��ʽת��,Ҫ�����С���ֽ�����flagȡֵCP_BIG5|ENCODE_LITTLE 
	���flagȡֵCP_BIG5����Ĭ��ʹ��С����ʽ 
	���flagȡֵ0������������Ĭ�ϱ��뷽ʽ����ʹ�����õ�Ĭ�ϱ��뷽ʽת���� 
	���flagȡֵ0������û������Ĭ�ϱ��뷽ʽ����Ĭ��ʹ�ñ����루CP_GB2312��ת��
*/
int MMCP_Str2Unicode(unsigned int flag, const char *convertString, int convertStrLen, unsigned short *unicodeStringBuffer, int maxUnicodeCnt);

/**
@brief ��Unicode������ַ���ת��Ϊָ�����뷽ʽ���ַ���

@param[in] flag       ָ�������뷽ʽ�������Unicode������ֽ��򣨴�С�ˣ���ȡֵΪ��ת�뷽ʽ|��С�ˣ���
		ȡֵ��Χ��ENCODE_BIG|codePage,��ENCODE_LITTLE|codePage,��ֱ��ȡcodePage������codePageȡֵ����Ϊ0��
						
@param[in] unicodeString    ��Ҫת���Unicode�ַ����ı������С�
@param[in] unicodeCnt  ��Ҫת����Unicode�������(ÿ��Unicode����ռ�����ֽ�)��
@param[out] convertStringBuff  ���ת����ı������е�buffer��
@param[in] convertBufferLen  ���ת����ı������е�buffer�Ĵ�С
							(Ҳ����ռ���ٸ��ֽ�,ȡֵ��Χ:���ת�뷽ʽ��UTF8,��convertBufferLen >= 3*unicodeCnt+2,
																			����:convertBufferLen >= 2*unicodeCnt+2,)��

@return ʵ��ת�����ַ����ĳ���(Ҳ����ռ���ٸ��ֽ�)��

@note 
	����: 
	ʹ��CP_BIG5ת�뷽ʽ,UNICODE����Ϊ��ˣ���flagȡֵCP_BIG5|ENCODE_BIG 
	ʹ��CP_BIG5���뷽ʽ,UNICODE����ΪС�ˣ���flagȡֵCP_BIG5|ENCODE_LITTLE 
	���flagȡֵCP_BIG5����Ĭ��ʹ��С����ʽ 
	���flagȡֵ0������������Ĭ�ϱ��뷽ʽ����ʹ�����õ�Ĭ�ϱ��뷽ʽת�� 
	���flagȡֵ0������û������Ĭ�ϱ��뷽ʽ����Ĭ��ʹ�ñ����루CP_GB2312��ת��
*/
int MMCP_Unicode2Str(unsigned int flag, const unsigned short *unicodeString, int unicodeCnt, char *convertStringBuff, int convertBufferLen);

/**
@brief ��һ�ַ�unicode����ĵ��ֽ��ַ���ת��Ϊ��һ�ַ�unicode����ĵ��ֽ��ַ���

@param[in] srcCodePage  Դ�ַ����ı��뷽ʽ���ֽ��򣨴�С�ˣ���ȡֵΪ��ת�뷽ʽ|��С�ˣ���
		ȡֵ��Χ��ENCODE_BIG|codePage,��ENCODE_LITTLE|codePage,��ֱ��ȡcodePage������codePageȡֵ����Ϊ0���������롣
						
@param[in] srcStr       ��Ҫת����ַ�����
@param[in] desCodePage  Ŀ���ַ����ı��뷽ʽ���ֽ��򣨴�С�ˣ���ȡֵΪ��ת�뷽ʽ|��С�ˣ���
		ȡֵ��Χ��ENCODE_BIG|codePage,��ENCODE_LITTLE|codePage,��ֱ��ȡcodePage������codePageȡֵ����Ϊ0���������롣
@param[out] desStr      ���ת���������ַ�����
@param[in] desBufSize   ���ת���������ַ����Ļ����С��

@return ת���Ƿ�ɹ���

*/
BOOL MMCP_Str2Str(int srcCodePage, const char * srcStr, int desCodePage, char * desStr, int desBufSize);


/**
@brief ��һ�ַ�unicode����ĵ��ֽ��ַ���ת��Ϊ��һ�ַ�unicode����ĵ��ֽ��ַ���

@param[in] srcCodePage  Դ�ַ����ı��뷽ʽ���ֽ��򣨴�С�ˣ���ȡֵΪ��ת�뷽ʽ|��С�ˣ���
		ȡֵ��Χ��ENCODE_BIG|codePage,��ENCODE_LITTLE|codePage,��ֱ��ȡcodePage������codePageȡֵ����Ϊ0���������롣
						
@param[in] srcStr       ��Ҫת����ַ�����
@param[in] srcStrLen    ��Ҫת����ַ�������(�ַ�����)
@param[in] desCodePage  Ŀ���ַ����ı��뷽ʽ���ֽ��򣨴�С�ˣ���ȡֵΪ��ת�뷽ʽ|��С�ˣ���
		ȡֵ��Χ��ENCODE_BIG|codePage,��ENCODE_LITTLE|codePage,��ֱ��ȡcodePage������codePageȡֵ����Ϊ0���������롣
@param[out] desStr      ���ת���������ַ�����
@param[in] desBufSize   ���ת���������ַ����Ļ����С��

@return ת���Ƿ�ɹ���

*/
BOOL MMCP_String2String(int srcCodePage, const char * srcStr, int srcStrLen, int desCodePage, char * desStr, int desBufSize);

/************************************************************************************************************************************************************************/
/*****************************���º��ַ�ת����صĽӿ�Ϊ������Ŀ������������ʹ��********************************************************/
/************************************************************************************************************************************************************************/
/**
@brief ��ȡ��UTF�����Unicode�ַ����ĳ���
!!!!!!!!!!!!!!!!!!!!!!!�ӿ�Ϊ������Ŀ������������ʹ��

@param[in] unicodeStr   Unicode���ַ���
@param[in] count        Unicode�ַ�������Ӧ���ַ���

@return Unicode���ַ�����UTF����ĳ���

@note Count����uptr���ֽڳ���
*/
int MMCP_StrGetUnicode2UTFLength(const unsigned short * unicodeStr, 
					unsigned int count);

/**
@brief ��Unicode�ַ���ת����UTF������ַ���
!!!!!!!!!!!!!!!!!!!!!!!!!�ӿ�Ϊ������Ŀ������������ʹ��

@param[in]  unicodeStr  Ҫת����Unicode�ַ���
@param[in]  count       Unicode�ַ������ַ���
@param[out] pUtf        ת�����Utf�ַ����Ĵ洢Buffer
@param[in]  pUtfLen     �洢Utf�ַ�����Buffer����

@return ����ת�����Utf�ַ������ַ�����

@note Count����uptr���ֽڳ���
*/
int MMCP_StrUnicode2UTF(const unsigned short * unicodeStr, 
			       unsigned int count,
				char * pUtf, int pUtfLen);

/**
@brief ASCII���ַ���ת��ΪUnicode�ַ���
!!!!!!!!!!!!!!!!!!!!!!!!!�ӿ�Ϊ������Ŀ������������ʹ��

@param[in]  ascii            Ҫת����ASCII���ַ���
@param[out] unicodeString    �洢ת�����Unicode�ַ���,Unicode��Ӧ��buffer������ASCII���ַ���buffer��2��
@param[in]  asciilen         Ҫת����ASCII���ַ�������Ч����

@return ����ת�����Unicode���ַ������ַ�����

@note ֧��ת�����ⳤ�ȵ�ASCII����ַ�����
@note ���ص���ת����Unicode���ַ����ַ��ĸ���
*/
int MMCP_StrASCII2Unicode(const char *ascii, 
				 unsigned short *unicodeString, 
				 int asciilen);

/**
@brief Unicode�ַ���ת��ΪASCII���ַ���
!!!!!!!!!!!!!!!!!!!!!!!!!�ӿ�Ϊ������Ŀ������������ʹ��

@param[in]  unicodeString   Ҫת����Unicode�ַ���
@param[out] asciiString     �洢ת�����ASCII�ַ���
@param[in]  unicodelen      Ҫת����Unicode���ַ�������Ч����

@return ����ת�����ASCII���ַ����ĳ���

@note ���ص���ת����ASCII���ַ����ַ��ĸ���
*/
int MMCP_StrUnicode2ASCII(unsigned short * unicodeString, 
				 char * asciiString, 
				 int unicodelen);

/**
@brief �õ�һ��UTF�ַ������ַ�����
!!!!!!!!!!!!!!!!!!!!!!!!!!!�ӿ�Ϊ������Ŀ������������ʹ��

@param[in]  utfString   Ҫ�õ����ȵ�UTF�ַ���
@param[in]  len         ��UTF�ַ������ֽڳ��ȣ����Դ�0

@return ����UTF�ַ������ַ�����

@note len���Դ�0
*/
int MMCP_StrGetUTFStringLength(const char *utfString, int len);

/**
@brief UTF�����ַ���ת��ΪUnicode�ַ���
!!!!!!!!!!!!!!!!!!!!!!!!!!!�ӿ�Ϊ������Ŀ������������ʹ��

@param[in]  utfString        Ҫת����utf���ַ���
@param[out] unicodeString    �洢ת�����Unicode�ַ���
@param[in]  UtfLen           ��UTF�ַ������ֽڳ���

@return ����ת�����Unicode���ַ����ĳ���

@note Unicode��Ӧ��buffer������utf���ַ���buffer�Ĵ�С
@note ���ص���ת����Unicode���ַ����ַ��ĸ���
*/
int MMCP_StrUTF2Unicode(const char *utfString, 
				 unsigned short *unicodeString, 
				 int UtfLen);

/**
@brief �������ֵ����е��ַ�������o_name������

@param[in]  o_name   Ҫ�����������ַ�����Ĭ��ASCII���ʽ
@param[in]  o_len    Ҫ���������ֳ���
@param[out] s_name   ��������еĶ�������Unicode��洢
@param[out] s_len    �����ĳ���   
@param[out] name     o_name��Unicode��洢
@param[out] len      ��Ӧname�ĳ���

@return �ɹ� 0�� ʧ�� -1

@note ����Ķ���Unicode�ַ�������MMCP_ReleaseDVBNames���ͷ����Ӧ�ڴ�
*/
int MMCP_ParseDVBNames(
	char *o_name,
	unsigned char o_len,
	unsigned short **s_name,
	unsigned char *s_len,
	unsigned short **name,
	unsigned char *len);

/**
@brief �ͷ�MMCP_ParseDVBNames�õ������������ַ����Ŀռ�

@param[in] s_name   MMCP_ParseDVBNames�õ����ַ���֮һ
@param[in] name     MMCP_ParseDVBNames�õ����ַ���֮һ

@return ��

@note ��
*/

void MMCP_ReleaseDVBNames(unsigned short *s_name, unsigned short *name);

/**
@brief ȥ���ַ���ǰ��Ŀո�

@param[in] src  Դ�ַ�����

@return �������ַ�����
*/
char * MMCP_StrTrim(char * src);

/**
@brief �Ƚ������ַ��������Դ�Сд

@param[in] s1   
@param[in] s2  

@return �ȽϽ����0Ϊ�����ַ������,s1 ������s2�򷵻ش���0��ֵ��s1��С��s2�򷵻�С��0 ��ֵ

@note ��
*/
int MMCP_Strcasecmp(const char *s1, const char *s2);

/**
@brief �Ƚ������ַ�����ǰnLen���ַ������Դ�Сд

@param[in] s1   
@param[in] s2  
@param[in] nLen

@return �ȽϽ����0Ϊ�����ַ���ǰnLen���ַ����,s1 ������s2�򷵻ش���0��ֵ��s1��С��s2�򷵻�С��0 ��ֵ

@note ��
*/
int MMCP_Strncasecmp(const char *s1, const char *s2, int nLen);


#ifdef __cplusplus
}
#endif

#endif /*_MMCP_STRINGS_H_*/



