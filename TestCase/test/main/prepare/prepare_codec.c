
#include "setup.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "irdecode.h"
#include "CS_OS.h"
#include "udi_init.h"
#include "cs_testkit.h"
#include "udi_rm.h"
#include "cs_syspublic.h"

 
#if defined(PLATFORM_ST40)
extern BOOL CSESInit(HCSHANDLE  hHandle, CSESBufferInfo* pstESBufferInfo);
BOOL iInitEs(void)
{
	BOOL bRet;
	
	CSRMDeviceReq_S sReq = {EM_RESOURCE_AUDIO,0};
	HCSHANDLE hAvHandle[2]; 
	HCSHANDLE hPath = (HCSHANDLE)NULL;
	CSESBufferInfo 		sESBufferInfo;
	
	sReq.m_eResType = EM_RESOURCE_AUDIO;
	sReq.m_nIndex = 0;
	bRet = CSRMAlloc(sReq, &hAvHandle[0]);
	if(CSASSERT_FAILED(bRet && hAvHandle[0])) return FALSE;
	
	sReq.m_eResType = EM_RESOURCE_VIDEO;
	sReq.m_nIndex = 0;
	bRet = CSRMAlloc(sReq, &hAvHandle[1]);
	if(CSASSERT_FAILED(bRet && hAvHandle[1])) return FALSE;
	
	hPath = CSRMConnect(hAvHandle,2);
	if(CSASSERT_FAILED(hPath)) return FALSE;
	
	sESBufferInfo.nAudioBufferSize =  512*1024;
	sESBufferInfo.nAudioPreInjectSize = 100;
	sESBufferInfo.nAudioPreInjectTime = 1;
	sESBufferInfo.nVideoBufferSize = 260*1024;
	sESBufferInfo.nVideoPreInjectSize = 100;
	sESBufferInfo.nVideoPreInjectTime = 1;
	sESBufferInfo.pucAudioBuffer = NULL;
	sESBufferInfo.pucVideoBuffer = NULL;

	bRet = CSESInit((HCSHANDLE)NULL, &sESBufferInfo);
	if(CSASSERT_FAILED(bRet)) return FALSE;

	bRet = CSRMDisconnect(hPath);
	CSASSERT(bRet);

	bRet = CSRMFree(hAvHandle[0]);
	CSASSERT(bRet);
	bRet = CSRMFree(hAvHandle[1]);
	CSASSERT(bRet);

	return TRUE;
}
#endif
void PrepareTestCodec(void)
{	
	
	//SetupPortingDriver();
	udi_init();
	SetupFS();

	
#if defined(PLATFORM_ST40)
	{
		const char *szMoutPoint = "/workdir/HDD0";
		iInitEs();
		CSFSMkdir(szMoutPoint, 0);
		CSSleep(10000);
		CSFSMount("/volume0", szMoutPoint, "fat", 0, NULL);
	}
#endif
}


