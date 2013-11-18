#include "cs_testkit_porting.h"
#include "stdio.h"
#include "udi2_typedef.h"

static CSUDI_BOOL s_bTKPInputRegisterState=CSUDI_FALSE;
static CSUDI_BOOL s_bTKPFsRegisterState=CSUDI_FALSE;
static CSTKPFSType_E s_eFSType = EM_TKP_FS_SYS;

/****系统文件模式接口(操作系统自带文件系统)***/
static CSUDI_HANDLE iSysFSOpen(const char* filename,const char* mode)
{
	char acFileBase[256] = "testdata/";
	if(filename[0] == '/')
	{
		return (CSUDI_HANDLE)fopen(filename,mode);		
	}
	else
	{
		strcat( acFileBase,  filename);
		return (CSUDI_HANDLE)fopen(acFileBase,mode);
	}
}

static int iSysFSClose(CSUDI_HANDLE hFile)
{
	return fclose((FILE*)hFile);
}

static int iSysFSRead(void * ptr,int nSize,int nMemb,CSUDI_HANDLE hFile)
{
	return (int)fread(ptr,(size_t)nSize,(size_t)nMemb,(FILE*)hFile);
}

static int iSysFSWrite(const void * ptr,int nSize,int nMemb,CSUDI_HANDLE hFile)
{
	return (int)fwrite(ptr,(size_t)nSize,(size_t)nMemb,(FILE*)hFile);
}

static int iSysFSFlush(CSUDI_HANDLE hFile)
{
	return fflush((FILE*)hFile);
}

static int iSysFSSeek(CSUDI_HANDLE hFile,long int offset,int whence)
{
	return fseek((FILE*)hFile,offset,whence);
}

static int iSysFSTell(CSUDI_HANDLE hFile)
{
	return ftell((FILE*)hFile);
}

static CSTKPFSOperations_S s_stTKP_Fs=
	{
		iSysFSOpen,
		iSysFSClose,
		iSysFSRead,
		iSysFSWrite,
		iSysFSFlush,
		iSysFSSeek,
		iSysFSTell
	};

void CSTKPRegisterSysFs(void)
{
	CSTKPFSOperations_S* psTKPFs;
	
	if(!CSTKPGetFsRegisterState())
	{
		psTKPFs = CSTKPFsGetOps();
	
		psTKPFs->Open = iSysFSOpen;
		psTKPFs->Close = iSysFSClose;
		psTKPFs->Read = iSysFSRead;
		psTKPFs->Write = iSysFSWrite;
		psTKPFs->Flush = iSysFSFlush;
		psTKPFs->Seek = iSysFSSeek;
		psTKPFs->Tell = iSysFSTell;
		
		CSTKPSetFsRegisterState(CSUDI_TRUE);
		CSTKPSetFSType(EM_TKP_FS_SYS);
		
		CSTCPrint( "Register Sys Fs Success!\n");
	}
}

/*****************************UartSys************************/
static char* iUartSysGets(char *s,int nSize)
{
	if(s && nSize > 0)
	{
		return fgets(s,nSize,stdin);
	}
	return CSUDI_NULL;
}
static int iUartSysVPrintf(char *format,va_list arg)
{
	return vprintf(format,arg);
}

static void iUartSysFlush(void)
{
	fflush(stdout);
}

static void iUartSysExit(void)
{
	exit(-1);
}

static CSTKPInputOperations_S s_stTKP_Input=
	{
		iUartSysGets,
		iUartSysVPrintf,
		iUartSysFlush,
		iUartSysExit		
	};
/************************注册系统串口输入接口*******************/
 void CSTKPRegisterUartSysInput(void)
{
	CSTKPInputOperations_S* psTKPInput;
	
	if(!CSTKPGetInputRegisterState())
	{	
		psTKPInput= CSTKPInputGetOps();
	
		psTKPInput->Gets      = iUartSysGets;
		psTKPInput->VPrintf   = iUartSysVPrintf;
		psTKPInput->Flush     = iUartSysFlush;
		psTKPInput->Exit       = iUartSysExit;
		
		CSTKPSetInputRegisterState(CSUDI_TRUE);
		
		CSTCPrint( "Register SysUart for input Success!\n");
	}
}

/*******输入方式注册总接口：nType=0:UartSys,1:UDI2,2:UDI1,3:Remote*******/
void  CSTKPRegisterInput(CSTKPInputType_E nType)
{
	switch(nType)
	{
		case EM_INPUT_UART_SYS:
			CSTKPRegisterUartSysInput();
			break;
		case EM_INPUT_UART_UDI2:
			CSTKPRegisterUartUDI2Input();
			break;
		case EM_INPUT_UART_UDI1:
			CSTKPRegisterUartUDI1Input();			
			break;
		case EM_INPUT_REMOTE:
			CSTKPRegisterRemoteInput();
			break;
		default:
			CSTKPRegisterUartSysInput();
			break;
	}
}

/*******其它对Adapter模块接口*************************/
CSTKPInputOperations_S* CSTKPInputGetOps(void)
{
	return &s_stTKP_Input;
}

CSUDI_BOOL CSTKPGetInputRegisterState(void)
{
	return s_bTKPInputRegisterState;
}

CSUDI_BOOL CSTKPSetInputRegisterState(CSUDI_BOOL bState)
{
	s_bTKPInputRegisterState = bState;
	
	return CSUDI_TRUE;	
}

CSTKPFSOperations_S* CSTKPFsGetOps(void)
{
	return &s_stTKP_Fs;
}

CSUDI_BOOL CSTKPGetFsRegisterState(void)
{
	return s_bTKPFsRegisterState;
}

CSUDI_BOOL CSTKPSetFsRegisterState(CSUDI_BOOL bState)
{
	s_bTKPFsRegisterState = bState;

	return CSUDI_TRUE;
}

CSUDI_BOOL CSTKPSetFSType(CSTKPFSType_E eType)
{
	s_eFSType = eType;

	return CSUDI_TRUE;
}

CSTKPFSType_E CSTKPGetFSType(void)
{
	return s_eFSType;
}

//I/O相关接口
int CSTKP_VPrintf(char *format,va_list arg)
{
	return s_stTKP_Input.VPrintf(format,arg);
}

char* CSTKP_Gets(char *s,int nSize)
{
	return s_stTKP_Input.Gets(s,nSize);
}

void CSTKP_Flush(void)
{
	s_stTKP_Input.Flush();
}

void CSTKP_Exit(void)
{
	s_stTKP_Input.Exit();
}

//文件系统接口


CSUDI_HANDLE CSTKP_FOpen(const char* filename,const char* mode)
{
	return s_stTKP_Fs.Open(filename,mode);
}

int CSTKP_FClose(CSUDI_HANDLE hFile)
{
	return s_stTKP_Fs.Close(hFile);
}

int CSTKP_FRead(void * ptr, int nSize, int nMemb, CSUDI_HANDLE hFile)
{
	return s_stTKP_Fs.Read(ptr,nSize,nMemb,hFile);
}

int CSTKP_FWrite(const void * ptr,int nSize,int nMemb,CSUDI_HANDLE hFile)
{
	return s_stTKP_Fs.Write(ptr,nSize,nMemb,hFile);
}
int CSTKP_FFlush(CSUDI_HANDLE hFile)
{	
	return s_stTKP_Fs.Flush(hFile);
}

int CSTKP_FSeek(CSUDI_HANDLE hFile,long int offset,int whence)
{	
    return s_stTKP_Fs.Seek(hFile,offset,whence);
}

long CSTKP_FTell(CSUDI_HANDLE hFile)
{	
    return s_stTKP_Fs.Tell(hFile);   
}


