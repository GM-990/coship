#ifndef _CS_STRINGS_H_
#define _CS_STRINGS_H_
#include "mmcp_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif


/**
@brief 定义各种编码的CodePage
@note 供MMCP_Str2Unicode 和MMCP_Unicode2Str使用
*/
#define CP_DEFAULT	0
#define CP_GB2312	936			/*简体中文字符集的codepage(大陆、新加坡等地区使用)*/
#define CP_BIG5		950		/*台湾地区使用的繁体字符集的codepage*/
#define CP_UTF8		65001	/*UTF8编码字符集的codepage*/

#define CP_ISO8859_1 28591   /*ISO8859系列codepage*/
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

#define CP_WIN1251  1251	/*CP_WIN1251和CP_1251K都是俄语系codepage，视国家不同使用不同*/	
#define CP_1251K    1351	
#define CP_1252     1252	/*Windows Latin-1,是CP_ISO8859_1的一个超集，CP_ISO8859_1占用了1252这个codepage值，所以定义为此值*/
#define CP_PORTUGUEUE 28606	/*葡语系codepage，非标准*/

#define CP_UNICODE_LE	1200		/*Unicode字符集，大端字节序*/
#define CP_UNICODE_BE	1201		/*Unicode字符集，小端字节序*/

#define ENCODE_BIG 0x10000		/*大端字节序*/
#define ENCODE_LITTLE 0		/*小端字节序*/

/**
@brief 设置默认转码方式,不支持设置Unicode的大小端输入输出
@param[in] nCodePage	取上面的宏定义，指明转换的字符串的编码方式，例如:big5 UTF ASCII 码等
@param[out] 无
@return 无
@note 如果传入的codePage传入非上面定义的值，则按ASCII(本地码)转换
*/
void MMCP_SetDefaultCodePage(int nCodePage);

/**
@brief 获得默认转码方式

@param[in] 无
@param[out] 无
@return 返回默认的编码方式
@note无
*/
int MMCP_GetDefaultCodePage(void);

/**
@brief 转换指定的编码方式的字符串为Unicode 编码字符串

@param[in] flag	指定convertString的编码方式和UNICODE输出字节序，取值为（编码方式|大小端），
		取值范围是ENCODE_BIG|codePage,或ENCODE_LITTLE|codePage,或直接取codePage，其中codePage取值可以为0。
@param[in] convertString    需要转码的字符串的编码序列。
@param[in] convertStrLen   需要转码的字符串长度（也就是占多少个字节）。
@param[out] unicodeStringBuffer  存放转码后Uniocde编码的buffer。
@param[in] maxUnicodeCnt  描述上面的unicodeStringBuffer最多能存的unicode编码的个数，也就是unicodeStringBuffer.length/2;
						(每个Unicode编码占两个字节，取值范围maxUnicodeCnt >= convertStrLen+1)

@return 实际转码后的字符的个数（即Unicode编码的个数，每个Unicode编码占两个字节）。

@note:
	例如:
	使用CP_BIG5方式转码,要求输出大端字节序，则flag取值CP_BIG5|ENCODE_BIG 
	使用CP_BIG5方式转码,要求输出小端字节序，则flag取值CP_BIG5|ENCODE_LITTLE 
	如果flag取值CP_BIG5，则默认使用小端形式 
	如果flag取值0，而且设置了默认编码方式，则使用设置的默认编码方式转换。 
	如果flag取值0，而且没有设置默认编码方式，则默认使用本地码（CP_GB2312）转换
*/
int MMCP_Str2Unicode(unsigned int flag, const char *convertString, int convertStrLen, unsigned short *unicodeStringBuffer, int maxUnicodeCnt);

/**
@brief 将Unicode编码的字符串转换为指定编码方式的字符串

@param[in] flag       指定编码码方式和输入的Unicode编码的字节序（大小端），取值为（转码方式|大小端），
		取值范围是ENCODE_BIG|codePage,或ENCODE_LITTLE|codePage,或直接取codePage，其中codePage取值可以为0。
						
@param[in] unicodeString    需要转码的Unicode字符串的编码序列。
@param[in] unicodeCnt  需要转换的Unicode编码个数(每个Unicode编码占两个字节)。
@param[out] convertStringBuff  存放转换后的编码序列的buffer。
@param[in] convertBufferLen  存放转换后的编码序列的buffer的大小
							(也就是占多少个字节,取值范围:如果转码方式是UTF8,则convertBufferLen >= 3*unicodeCnt+2,
																			其他:convertBufferLen >= 2*unicodeCnt+2,)。

@return 实际转码后的字符串的长度(也就是占多少个字节)。

@note 
	例如: 
	使用CP_BIG5转码方式,UNICODE输入为大端，则flag取值CP_BIG5|ENCODE_BIG 
	使用CP_BIG5编码方式,UNICODE输入为小端，则flag取值CP_BIG5|ENCODE_LITTLE 
	如果flag取值CP_BIG5，则默认使用小端形式 
	如果flag取值0，而且设置了默认编码方式，则使用设置的默认编码方式转换 
	如果flag取值0，而且没有设置默认编码方式。则默认使用本地码（CP_GB2312）转换
*/
int MMCP_Unicode2Str(unsigned int flag, const unsigned short *unicodeString, int unicodeCnt, char *convertStringBuff, int convertBufferLen);

/**
@brief 将一种非unicode编码的单字节字符串转换为另一种非unicode编码的单字节字符串

@param[in] srcCodePage  源字符串的编码方式和字节序（大小端），取值为（转码方式|大小端），
		取值范围是ENCODE_BIG|codePage,或ENCODE_LITTLE|codePage,或直接取codePage，其中codePage取值可以为0，代表本地码。
						
@param[in] srcStr       需要转码的字符串。
@param[in] desCodePage  目标字符串的编码方式和字节序（大小端），取值为（转码方式|大小端），
		取值范围是ENCODE_BIG|codePage,或ENCODE_LITTLE|codePage,或直接取codePage，其中codePage取值可以为0，代表本地码。
@param[out] desStr      存放转换编码后的字符串。
@param[in] desBufSize   存放转换编码后的字符串的缓存大小。

@return 转换是否成功。

*/
BOOL MMCP_Str2Str(int srcCodePage, const char * srcStr, int desCodePage, char * desStr, int desBufSize);


/**
@brief 将一种非unicode编码的单字节字符串转换为另一种非unicode编码的单字节字符串

@param[in] srcCodePage  源字符串的编码方式和字节序（大小端），取值为（转码方式|大小端），
		取值范围是ENCODE_BIG|codePage,或ENCODE_LITTLE|codePage,或直接取codePage，其中codePage取值可以为0，代表本地码。
						
@param[in] srcStr       需要转码的字符串。
@param[in] srcStrLen    需要转码的字符串长度(字符个数)
@param[in] desCodePage  目标字符串的编码方式和字节序（大小端），取值为（转码方式|大小端），
		取值范围是ENCODE_BIG|codePage,或ENCODE_LITTLE|codePage,或直接取codePage，其中codePage取值可以为0，代表本地码。
@param[out] desStr      存放转换编码后的字符串。
@param[in] desBufSize   存放转换编码后的字符串的缓存大小。

@return 转换是否成功。

*/
BOOL MMCP_String2String(int srcCodePage, const char * srcStr, int srcStrLen, int desCodePage, char * desStr, int desBufSize);

/************************************************************************************************************************************************************************/
/*****************************以下和字符转码相关的接口为兼容项目保留，不建议使用********************************************************/
/************************************************************************************************************************************************************************/
/**
@brief 获取以UTF编码的Unicode字符串的长度
!!!!!!!!!!!!!!!!!!!!!!!接口为兼容项目保留，不建议使用

@param[in] unicodeStr   Unicode的字符串
@param[in] count        Unicode字符串所对应的字符数

@return Unicode码字符串以UTF编码的长度

@note Count不是uptr的字节长度
*/
int MMCP_StrGetUnicode2UTFLength(const unsigned short * unicodeStr, 
					unsigned int count);

/**
@brief 把Unicode字符串转换成UTF编码的字符串
!!!!!!!!!!!!!!!!!!!!!!!!!接口为兼容项目保留，不建议使用

@param[in]  unicodeStr  要转换的Unicode字符串
@param[in]  count       Unicode字符串的字符数
@param[out] pUtf        转换后的Utf字符串的存储Buffer
@param[in]  pUtfLen     存储Utf字符串的Buffer长度

@return 返回转换后的Utf字符串的字符长度

@note Count不是uptr的字节长度
*/
int MMCP_StrUnicode2UTF(const unsigned short * unicodeStr, 
			       unsigned int count,
				char * pUtf, int pUtfLen);

/**
@brief ASCII码字符串转换为Unicode字符串
!!!!!!!!!!!!!!!!!!!!!!!!!接口为兼容项目保留，不建议使用

@param[in]  ascii            要转换的ASCII码字符串
@param[out] unicodeString    存储转换后的Unicode字符串,Unicode对应的buffer至少是ASCII码字符串buffer的2倍
@param[in]  asciilen         要转换的ASCII码字符串的有效长度

@return 返回转换后的Unicode码字符串的字符长度

@note 支持转换任意长度的ASCII码的字符串。
@note 返回的是转换后Unicode码字符串字符的个数
*/
int MMCP_StrASCII2Unicode(const char *ascii, 
				 unsigned short *unicodeString, 
				 int asciilen);

/**
@brief Unicode字符串转换为ASCII码字符串
!!!!!!!!!!!!!!!!!!!!!!!!!接口为兼容项目保留，不建议使用

@param[in]  unicodeString   要转换的Unicode字符串
@param[out] asciiString     存储转换后的ASCII字符串
@param[in]  unicodelen      要转换的Unicode码字符串的有效长度

@return 返回转换后的ASCII码字符串的长度

@note 返回的是转换后ASCII码字符串字符的个数
*/
int MMCP_StrUnicode2ASCII(unsigned short * unicodeString, 
				 char * asciiString, 
				 int unicodelen);

/**
@brief 得到一个UTF字符串的字符长度
!!!!!!!!!!!!!!!!!!!!!!!!!!!接口为兼容项目保留，不建议使用

@param[in]  utfString   要得到长度的UTF字符串
@param[in]  len         该UTF字符串的字节长度，可以传0

@return 返回UTF字符串的字符个数

@note len可以传0
*/
int MMCP_StrGetUTFStringLength(const char *utfString, int len);

/**
@brief UTF编码字符串转换为Unicode字符串
!!!!!!!!!!!!!!!!!!!!!!!!!!!接口为兼容项目保留，不建议使用

@param[in]  utfString        要转换的utf码字符串
@param[out] unicodeString    存储转换后的Unicode字符串
@param[in]  UtfLen           该UTF字符串的字节长度

@return 返回转换后的Unicode码字符串的长度

@note Unicode对应的buffer至少是utf码字符串buffer的大小
@note 返回的是转换后Unicode码字符串字符的个数
*/
int MMCP_StrUTF2Unicode(const char *utfString, 
				 unsigned short *unicodeString, 
				 int UtfLen);

/**
@brief 解析数字电视中的字符串，把o_name解析成

@param[in]  o_name   要解析的名字字符串，默认ASCII码格式
@param[in]  o_len    要解析的名字长度
@param[out] s_name   输出名字中的短名，以Unicode码存储
@param[out] s_len    短名的长度   
@param[out] name     o_name以Unicode码存储
@param[out] len      对应name的长度

@return 成功 0， 失败 -1

@note 输出的都是Unicode字符串，用MMCP_ReleaseDVBNames来释放其对应内存
*/
int MMCP_ParseDVBNames(
	char *o_name,
	unsigned char o_len,
	unsigned short **s_name,
	unsigned char *s_len,
	unsigned short **name,
	unsigned char *len);

/**
@brief 释放MMCP_ParseDVBNames得到的两个名字字符串的空间

@param[in] s_name   MMCP_ParseDVBNames得到的字符串之一
@param[in] name     MMCP_ParseDVBNames得到的字符串之一

@return 无

@note 无
*/

void MMCP_ReleaseDVBNames(unsigned short *s_name, unsigned short *name);

/**
@brief 去除字符串前后的空格

@param[in] src  源字符串。

@return 处理后的字符串。
*/
char * MMCP_StrTrim(char * src);

/**
@brief 比较两个字符串，忽略大小写

@param[in] s1   
@param[in] s2  

@return 比较结果，0为两个字符串相等,s1 若大于s2则返回大于0的值，s1若小于s2则返回小于0 的值

@note 无
*/
int MMCP_Strcasecmp(const char *s1, const char *s2);

/**
@brief 比较两个字符串的前nLen个字符，忽略大小写

@param[in] s1   
@param[in] s2  
@param[in] nLen

@return 比较结果，0为两个字符串前nLen个字符相等,s1 若大于s2则返回大于0的值，s1若小于s2则返回小于0 的值

@note 无
*/
int MMCP_Strncasecmp(const char *s1, const char *s2, int nLen);


#ifdef __cplusplus
}
#endif

#endif /*_MMCP_STRINGS_H_*/



