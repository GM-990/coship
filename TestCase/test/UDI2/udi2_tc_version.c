#include "cs_testkit.h"
#include "cs_revision.h"

 /*
	00.00.01 : 第一次创建版本发布
	00.00.02:
		A)增加应用可用内存大小探测的测试用例：CSTC_OS_IT_Available_Memory（）。修改日期：2010-01-25
		B)增加频繁分配后内存碎片情况的测试用例：CSTC_OS_IT_Memory_Fragments（）。修改日期：2010-01-25
		C)增加OSG模块测试用例CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0018(void),测试CSUDIOSGDecodeImage函数中判断缓存页面的大小不超过输出窗口大小。修改日期：2010-01-25
		D)修改测试用例CSTC_DEMUX_IT_GetCapability_0001，增加对m_dwDMAConnectSupportTunersMask字段的判断。修改日期：2010-02-02
	00.00.03:
		增加查询UDI测试库svn版本号功能。修改日期：2011-11-30
*/
	
#define UDI2TC_VERSION	"00.00.03"

int CSUDIGetTestCaseVersion( char* pcVersionInfo, int nSize)
{
	int nRet = -1;
	char acVersion[256];
	int nRealLen = 0;

	if (pcVersionInfo != NULL && nSize > 0)
	{
		memset(acVersion, 0, sizeof(acVersion));
		sprintf(acVersion, UDI2TC_VERSION".%s ", SRC_VERSION);

		nRealLen = strlen(acVersion) <= nSize ? strlen(acVersion) : nSize;
		memcpy(pcVersionInfo, acVersion, nRealLen);

		nRet = nRealLen;

		pcVersionInfo[nRealLen] = '\0';
	}

	return nRet;
}

VERSION_FUNCTION(UDI2TC, UDI2TC_VERSION)

