#include "cs_testkit_porting.h"
#include "cs_testkit.h"
#include "cs_fsapi.h"

/*******ROM文件系统模式接口****/
static CSUDI_HANDLE iRomFSOpen(const char* filename,const char* mode)
{
	int nRet = -1;
	char fileBase[256] = "/rom/";
	
	if(filename[0] == '/')
	{
		nRet=CSFSOpen(filename,CSFS_O_CREAT|CSFS_O_RDWR, CSFS_O_MODE);		
	}
	else
	{
		strcat( fileBase,  filename);
		nRet=CSFSOpen(fileBase,CSFS_O_CREAT|CSFS_O_RDWR, CSFS_O_MODE);
	}
	
	return (CSUDI_HANDLE)(nRet+1);
}

static int iRomFSClose(CSUDI_HANDLE hFile)
{
	int nRet;
	
	nRet = CSFSClose((int)hFile-1);

	return nRet;
}

static int iRomFSRead(void * ptr,int nSize,int nMemb,CSUDI_HANDLE hFile)
{
	unsigned int unReadLen=0;
	int nRet;
	
	unReadLen = nSize*nMemb;

	nRet = CSFSRead((int)hFile-1, (char *)ptr, unReadLen);
	if( nRet>=0 && nSize>0 )
	{
		nRet /= nSize;
	}
	
	return nRet;
}

static int iRomFSWrite(const void * ptr,int nSize,int nMemb,CSUDI_HANDLE hFile)
{
	unsigned int unWriteLen=0;
	int nRet;
	
	unWriteLen = nSize*nMemb;
	nRet = CSFSWrite((int)hFile-1, (char *)ptr, unWriteLen);
	if( nRet>=0 && nSize>0 )
	{
		nRet /= nSize;
	}
	
	return nRet;
}

static int iRomFSFlush(CSUDI_HANDLE hFile)
{
	int nRet;
	
	nRet = CSFSFflush((int)hFile-1);	
	
	return nRet;
}

static int iRomFSSeek(CSUDI_HANDLE hFile,long int offset,int whence)
{
	int nRet = -1;
	
	nRet = CSFSLseek((int)hFile-1, offset, whence);

	if (nRet >= 0)
	{
		nRet = 0;
	}
	
    	return nRet;
}

static int iRomFSTell(CSUDI_HANDLE hFile)
{
	int nRet;
	
	nRet = CSFSFtell((int)hFile-1);
	
	return nRet;
}

void CSTKPRegisterRomFs(void)
{
	int nError;
	CSFSRomfsInfo_t sRomfsInfo;
	extern int MMCP_ROMSizePerUnit; 
	extern int MMCP_ROMUnits; 
	extern char * MMCP_ROMArray[];
	
	CSTKPFSOperations_S* psTKPFs=CSUDI_NULL;
	
	if(!CSTKPGetFsRegisterState())
	{
		extern int MMCP_ROMSizePerUnit ; 
		extern int MMCP_ROMUnits; 
		extern char * MMCP_ROMArray[];
		
		psTKPFs = CSTKPFsGetOps();
	
		CSFSInit(0x0100, 2*1024);

		memset(&sRomfsInfo, 0, sizeof(sRomfsInfo));
		sRomfsInfo.m_nROMSizePerUnit = MMCP_ROMSizePerUnit;
		sRomfsInfo.m_nROMUnits = MMCP_ROMUnits;
		sRomfsInfo.m_ppcROMUnitsArray = MMCP_ROMArray;
		CSFSRegisterRomFs(&sRomfsInfo);	
		
		nError = CSFSMkdir("/rom", 0);
		CSTCPrint("[CSTKPRegisterRomFs]CSFSMkdir = %d\n", nError);
		
		CSFSSetCWD("/rom");
		
		nError = CSFSMount("rom0", "/rom", "romfs", 0, 0);	
		CSTCPrint("[CSTKPRegisterRomFs]CSFSMount = %d\n", nError);
	
		psTKPFs->Open = iRomFSOpen;
		psTKPFs->Close = iRomFSClose;
		psTKPFs->Read = iRomFSRead;
		psTKPFs->Write = iRomFSWrite;
		psTKPFs->Flush = iRomFSFlush;
		psTKPFs->Seek = iRomFSSeek;
		psTKPFs->Tell = iRomFSTell;
		
		CSTKPSetFsRegisterState(CSUDI_TRUE);
		CSTKPSetFSType(EM_TKP_FS_ROM);
		
		CSTCPrint( "Register Rom Fs Success!\n");
	}
}


