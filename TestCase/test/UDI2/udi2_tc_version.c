#include "cs_testkit.h"
#include "cs_revision.h"

 /*
	00.00.01 : ��һ�δ����汾����
	00.00.02:
		A)����Ӧ�ÿ����ڴ��С̽��Ĳ���������CSTC_OS_IT_Available_Memory�������޸����ڣ�2010-01-25
		B)����Ƶ��������ڴ���Ƭ����Ĳ���������CSTC_OS_IT_Memory_Fragments�������޸����ڣ�2010-01-25
		C)����OSGģ���������CSUDI_BOOL CSTC_OSG_IT_DecodeImage_0018(void),����CSUDIOSGDecodeImage�������жϻ���ҳ��Ĵ�С������������ڴ�С���޸����ڣ�2010-01-25
		D)�޸Ĳ�������CSTC_DEMUX_IT_GetCapability_0001�����Ӷ�m_dwDMAConnectSupportTunersMask�ֶε��жϡ��޸����ڣ�2010-02-02
	00.00.03:
		���Ӳ�ѯUDI���Կ�svn�汾�Ź��ܡ��޸����ڣ�2011-11-30
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

