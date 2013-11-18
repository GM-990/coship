#include "cs_testkit_porting.h"

#include "udi2_fs.h"

/******USB文件系统模式接口*****/
static CSUDI_HANDLE iUsbFSOpen(const char* filename,const char* mode)
{
	CSUDI_UINT32 dwIds[32] = {0};
	CSUDI_BOOL SuccessFounded = CSUDI_FALSE;
	int nActIdCnt = 0;
	int i = 0;
	unsigned char  acName[128]={0};
	CSUDIFSDeviceInfo_S sDeviceInfo;

	memset(&sDeviceInfo,0,sizeof(CSUDIFSDeviceInfo_S));
	if((CSUDI_SUCCESS == CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt)))
	{
		for(i = 0; i < nActIdCnt; i ++)
		{
			if(CSUDI_ISREMOVEABLE(dwIds[i]))
			{
				if(CSUDI_SUCCESS != CSUDIFSGetDeviceInfo(dwIds[i], &sDeviceInfo))
				{
					  	break;
				}
				if(EM_UDIFS_DEVTYPE_VOLUME == sDeviceInfo.m_eDeviceType)
				{       							
						/***找到能识别的设备*****/
						SuccessFounded  = CSUDI_TRUE;
						break;
				}
        		}	
         	}
	}
	if(SuccessFounded == CSUDI_TRUE)
	{
		/****所有资源文件放在U盘的testdata下*****/
		sprintf((char*)acName, "%s/testdata/%s", sDeviceInfo.m_szMountPoint, filename);
	}
	else
	{
		sprintf((char*)acName,"NoRemoveableDevice");
	}
	
	return CSUDIFSOpen((const char*)acName, mode);
}

static int iUsbFSClose(CSUDI_HANDLE hFile)
{
	return CSUDIFSClose(hFile);
}

static int iUsbFSRead(void * ptr,int nSize,int nMemb,CSUDI_HANDLE hFile)
{
	unsigned int unReadLen=0;
	int nRet;
	
	unReadLen = nSize*nMemb;
	nRet = CSUDIFSRead(hFile, (char * )ptr, unReadLen);
	if( nRet>=0 && nSize>0 )
	{
		nRet /= nSize;
	}
	
	return nRet;
}

static int iUsbFSWrite(const void * ptr,int nSize,int nmemb,CSUDI_HANDLE hFile)
{
	unsigned int unWriteLen=0;
	int nRet;
	
	unWriteLen = nSize*nmemb;
	nRet = CSUDIFSWrite(hFile, (char *)ptr, unWriteLen);
	if( nRet>=0 && nSize>0 )
	{
		nRet /= nSize;
	}
	
	return nRet;
}

static int iUsbFSFlush(CSUDI_HANDLE hFile)
{
	int nRet = CSUDIFSFlush(hFile);	
	
	return nRet;
}

static int iUsbFSSeek(CSUDI_HANDLE hFile,long int offset,int whence)
{
	int nRet;
	
	nRet = CSUDIFSSeek(hFile, offset,whence);	
	
	return nRet;
}

static int iUsbFSTell(CSUDI_HANDLE hFile)
{
	int nRet = CSUDIFSTell(hFile);	
	
	return nRet;
}

void CSTKPRegisterUsbFs(void)
{
	CSTKPFSOperations_S* psTKPFs=NULL;
	
	if(!CSTKPGetFsRegisterState())
	{	
		
		psTKPFs = CSTKPFsGetOps();
	
		psTKPFs->Open = iUsbFSOpen;
		psTKPFs->Close = iUsbFSClose;
		psTKPFs->Read = iUsbFSRead;
		psTKPFs->Write = iUsbFSWrite;
		psTKPFs->Flush = iUsbFSFlush;
		psTKPFs->Seek = iUsbFSSeek;
		psTKPFs->Tell = iUsbFSTell;
		
		CSTKPSetFsRegisterState(CSUDI_TRUE);
		CSTKPSetFSType(EM_TKP_FS_USB);
		
		CSTCPrint( "Register Usb Fs Success!\n");
	}	
}


