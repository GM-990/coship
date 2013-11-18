#ifndef _CS_REVISION_H_
#define _CS_REVISION_H_

#ifdef __cplusplus
extern "C" {
#endif




/* Function：获取xxx模块版本号
Syntax：
	int CSxxxGetVersion( 
		char* 		pcVersion, 
		int 		nSize
		 );
Parameters:
	pcVersion
		输出参数，存放版本信息的缓冲区首地址。
		返回版本信息字符串格式为 "xx.xx.xx"。
	nSize
		输入参数，缓冲区pcVersion的长度
Return Value:
	1) 如果参数pcVersion为空，返回版本信息的实际长度。 
	2) 如果参数pcVersion不为空，且函数调用成功，返回版本信息的实际长度；
	3) 如果失败，返回0。
Remarks：
    第一种返回值的情况，用户可测试该函数，并为第二个参数nSize提供参考。
	如果用户输入的第二个参数小于版本信息的实际长度，尾部多余的字节被截掉。
*/

#ifdef MMCP_UDI2
#define UDIVERSTR	"forUDI2.0"
#else
#define UDIVERSTR	"forUDI1.0"
#endif

#define VERSION_FUNCTION(mode,ver)	\
	int CS##mode##GetVersion( char * pcVersion, int nSize )	\
	{	\
		const char* pVer = "MMCP_"#mode"_V"ver"_svn"SRC_VERSION" "UDIVERSTR" "__DATE__" "__TIME__;\
		int nRet = 0, nCopy = 0; \
		int nRealLen = strlen(pVer);\
		\
		if (pcVersion == NULL) \
		{ \
			nRet = nRealLen; \
		} \
		else if (pcVersion!= 0 && nSize > 0)\
		{\
			nCopy = (nSize > nRealLen? nRealLen:nSize-1); \
			memcpy(pcVersion, pVer, nCopy); \
			pcVersion[nCopy] = '\0'; \
			nRet = nRealLen; \
		} \
		return nRet;	\
	}

int CSMMCPGetVersion( char * pcVersion, int nSize );



int CSPortingGetVersion( char * pcVersion, int nSize );

/**
@brief 获取中间件VR版本号
 例如 : "V100R002"  详见版本定义说明

@param[out] 存放输出版本号的缓冲区首地址。
@param[in] 缓冲区pcVersion的长度
@return 1) 如果参数pcVersion为空，返回版本信息的实际长度。 
		2) 如果参数pcVersion不为空，且函数调用成功，返回版本信息的实际长度；
		3) 如果失败，返回0。
	第一种返回值的情况，用户可测试该函数，并为第二个参数nSize提供参考。
	如果用户输入的第二个参数小于版本信息的实际长度，尾部多余的字节被截掉。
*/
int CSMMCPGetVRVersion( char * pcVersion, int nSize );



#ifdef __cplusplus
}
#endif
/** @} */
#endif

