#include "cs_testkit_porting.h"
#include "stdio.h"
#include <string.h>


#include "udi2_input.h"//注意遥控器依赖UDI的Input模块
#include "udi2_os.h"//依赖OS的sleep函数

static CSUDI_BOOL s_bKeyPress = CSUDI_FALSE;
static int s_nKeyValue = -1;
static CSUDI_BOOL s_bInputInit_Remote = CSUDI_FALSE;



/*此函数是供遥控器输入使用*/
static char iKeyToChar(CSUDI_UINT16 nKey)
{
	char cChar = 0;
	int i = 0;

	if (nKey>=CSUDI_VK_0 && nKey<=CSUDI_VK_9)
	{
		i = nKey-CSUDI_VK_0;
		cChar += '0' + i;
	}
	
	switch(nKey)
	{	
		case CSUDI_VK_MENU:
		{
			cChar = 'l';
			break;
		}
		case CSUDI_VK_COLORED_KEY_2:
		{
			cChar = 'y';
			break;
		}
		case CSUDI_VK_COLORED_KEY_3:
		{
			cChar = 'n';
			break;
		}
		case CSUDI_VK_EXIT:
		{
			cChar = 'b';
			break;
		}
		case CSUDI_VK_COLORED_KEY_1://解决无q的定义
		{
			cChar = 'q';
			break;
		}
		default:
			cChar = nKey;
			break;
	}

	return cChar;
}

static void iRemoteControlUserProc(CSUDIInputInfo_S *psInputInfo ,void * pvUserData)
{
	if(psInputInfo->m_eType ==  EM_UDIINPUT_REMOTECTRL &&
	   psInputInfo->m_eStatus == EM_UDIINPUT_KEY_RELEASE)//added by QLuo2011-8-8,for Ali project
	{
		s_bKeyPress = CSUDI_TRUE;
		s_nKeyValue = psInputInfo->m_nKeyCode;
		CSTCPrint("callback:0x%x\n", s_nKeyValue);
	}
}

static CSUDI_Error_Code iInputInit(void)
{
//	return CSUDIINPUTAddCallback(iRemoteControlUserProc, (void*)3);	
return 0;
}

/****************************Remote*************************/
static char* iRemoteGets(char *s,int size)
{
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	int nInputCount=0;

	if(!s_bInputInit_Remote)
	{
		eRet = iInputInit();
		CSTCPrint("function InputInit 's nRet is :%d\n",eRet );
		s_bInputInit_Remote = CSUDI_TRUE;
	}
	
	CSTCPrint("[%s, %d]\n", __FUNCTION__, __LINE__);
	
	if((CSUDI_NULL==s) || (size<4) )
	{
		CSTCPrint("Notice:&s=0x%x,size=%d\n",s,size);
		return CSUDI_NULL;
	}

	while(1)
	{
		if(CSUDI_FALSE == s_bKeyPress)
		{
			CSUDIOSThreadSleep(50);
			continue;
		}
				
		s[nInputCount]=iKeyToChar(s_nKeyValue);
		CSTCPrint("You have input nKey:%d\n", s_nKeyValue);
		
		s_bKeyPress = CSUDI_FALSE;
		s_nKeyValue = -1;
		
		if(s[nInputCount] == CSUDI_VK_OK)//按确认结束输入
		{
			s[nInputCount] = '\0';
			break;
		}
		nInputCount++;
		if(nInputCount > (size-1))
		{
			CSTCPrint("InputedCount will be over size,InputedCount:%d,size:%d\n", nInputCount,size);
			memset(s, 0, size);
			return CSUDI_NULL;
		}
	}

	CSTCPrint("You have input s:%s\n", s);
	CSTCPrint("[%s, %d]\n", __FUNCTION__, __LINE__);
	
	return s;
}

static int iRemoteVPrintf(char *format,va_list arg)
{
	return vprintf(format,arg);
}

static void iRemoteFlush(void)
{
	fflush(stdout);
}

static void iRemoteExit(void)
{
	exit(-1);
}
void CSTKPRegisterRemoteInput(void)
{

	CSTKPInputOperations_S* psTKPInput=CSUDI_NULL;
	
	if(!CSTKPGetInputRegisterState())
	{
		psTKPInput = CSTKPInputGetOps();
	
		psTKPInput->Gets      = iRemoteGets;
		psTKPInput->VPrintf   = iRemoteVPrintf;
		psTKPInput->Flush     = iRemoteFlush;
		psTKPInput->Exit       = iRemoteExit;
		
		CSTKPSetInputRegisterState(CSUDI_TRUE);
		
		CSTCPrint("Register Remote for input Success!\n");
	}

}

