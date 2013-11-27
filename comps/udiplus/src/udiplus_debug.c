#include "udiplus_debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udi2_os.h"
#include "udi2_typedef.h"
#include "udi2_debug.h"

#define CSSTD_UNUSED(x) ((void)x)
#define USE_HASH
#define MAX_MODULE_CNT	(4096)

#ifdef AUTHEN_FOR_CONAX
#define DEBUG_OUTPUTSTR(str)	
#else
#define DEBUG_OUTPUTSTR(str) {if(s_pfOutputFunction) s_pfOutputFunction(str);}
#endif

#ifdef _DEBUG
	#define DEBUGASSERT(expression)  \
	do{ \
		if(!(expression)) \
		DEBUG_OUTPUTSTR("<CSDebug> Assertion: \""#expression"\" failed, in file "__FILE__" inner debug mode\r\n"); \
	}while(0)
#else
	#define DEBUGASSERT(x)
#endif
#define DEBUG_LOCK(handle)	{if(handle != CSUDI_NULL){CSUDIOSMutexWait(handle,5000);}}//必须分开写否则在release版本时tRe会是已定义，未使用
#define DEBUG_FREE(handle)	{if(handle != CSUDI_NULL) {CSUDIOSMutexRelease(handle);}}

#ifdef _WIN32
#define CSDBG_LIGHT_RED     ""
#define CSDBG_NONE          ""
#else
#define CSDBG_LIGHT_RED     "\033[31m\033[1m"
#define CSDBG_NONE          "\033[0m"
#endif
#define CSDBG_ERROR_PREFIX		" [ERROR] "

#ifdef __GNUC__
#define WEAK	__attribute__ ((weak))
int CSDebugSetDefaultLevel(int nDefaultLevel) WEAK;
CSUDI_Error_Code CSDebugSet(const char* pcModuleName,int nOutputLevel) WEAK;
void CSDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat, ...) WEAK;
void  CSSetDebugLevel(int nLevel) WEAK;
void USPPrint(int nLevel,const char *format, ...) WEAK;
CSUDI_Error_Code CSDebugRegisterOutputCallback(CSDebugOutPutString OutputCallback) WEAK;
int CSDebugGetModuleInfo (const char** ppModules,int nMaxCount) WEAK;
void CSVDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat,va_list arg) WEAK;
#else /* !__GNUC__ */
#endif /* __GNUC__ */

typedef struct _CSDebugModule_S
{
	const char	*m_pcModuleName;
	int			m_nLevle;
}CSDebugModule_S;

static int s_nDefaultLevle = ERROR_LEVEL;
static CSUDI_HANDLE s_hMute = CSUDI_NULL;
static CSUDI_BOOL s_bModulesInited = CSUDI_FALSE;
static CSDebugModule_S s_asAllModulesInfo[MAX_MODULE_CNT] ;
static CSDebugOutPutString s_pfOutputFunction = CSDebugDefaultOutputString;
static CSUDI_BOOL s_bInNewLine = CSUDI_TRUE;

//递归调用保护部分
static CSUDI_HANDLE s_hDebugRecursionMutex = CSUDI_NULL;
static int	s_nRecursionCount = 0;

static CSUDI_BOOL DebugCanPrint(const char *pModuleName, int nOutputLevel);
static CSDebugModule_S* DebugSearchModule(const char * pModuleName);

#ifdef USE_HASH
static unsigned int DebugHashString(const char *pcString,unsigned int nStringLength);
static unsigned int DebugGetHashTablePos(const char *pcString,unsigned int nStringLength);
#endif

static CSUDI_BOOL CSUDIPlusDebugInit(void)
{
	CSUDI_Error_Code tErr;
	
	if (s_hMute == CSUDI_NULL)
	{
		tErr = CSUDIOSMutexCreate("Lock",0,&s_hMute);
		DEBUGASSERT(tErr == CSUDI_SUCCESS && "Create hMute Error");
		if(tErr != CSUDI_SUCCESS)
		{
			s_hMute = CSUDI_NULL;			
		}
	}

	if(s_hDebugRecursionMutex == CSUDI_NULL)
	{
		s_nRecursionCount = 0;
		tErr = CSUDIOSMutexCreate("RecursionMutex",0,&s_hDebugRecursionMutex);
		DEBUGASSERT(tErr == CSUDI_SUCCESS && "Create RecursionMutex Error");
		if(tErr != CSUDI_SUCCESS)
		{
			s_hDebugRecursionMutex = CSUDI_NULL;			
		}
	}
	
	if(!s_bModulesInited)
	{
		memset(s_asAllModulesInfo,0,sizeof(s_asAllModulesInfo));
		s_bModulesInited = CSUDI_TRUE;
	}

	return (s_hMute != CSUDI_NULL);	
}

int CSDebugSetDefaultLevel(int nDefaultLevel)
{
	int nOldLevel = s_nDefaultLevle;
	s_nDefaultLevle = nDefaultLevel;
	
	CSUDIPlusDebugInit();

	return nOldLevel;
}


CSUDI_Error_Code CSDebugSet(const char* pcModuleName,int nOutputLevel)
{
	CSDebugModule_S* pModule;
	
	if (pcModuleName == CSUDI_NULL)
	{
		return CSUDIDEBUGPLUS_ERROR_BAD_PARAMETER;
	}
	
	CSUDIPlusDebugInit();
	
	DEBUG_LOCK(s_hMute);

    if(strcmp(pcModuleName,MODULE_NAME_ALL) == 0)
    {
        int i;
        for (i=0;i<MAX_MODULE_CNT;i++)
        {
        	if (s_asAllModulesInfo[i].m_pcModuleName != NULL)
        	{
                s_asAllModulesInfo[i].m_nLevle = nOutputLevel;
        	}
#ifndef USE_HASH
        	else
        	{
        		break;
        	}
#endif
        }
    }
    else
    {	
    	pModule = DebugSearchModule(pcModuleName);
    	if (pModule == CSUDI_NULL)
    	{
    		DEBUG_OUTPUTSTR("<CSDEBUG> Warning ! Too many modules registed in CSDebug\r\n");

    		DEBUG_FREE(s_hMute);
    		return CSUDIDEBUGPLUS_ERROR_NO_MEMORY;
    	}

    	if (pModule->m_pcModuleName == CSUDI_NULL)//new module,init it
    	{
			// pModule->m_pcModuleName = pModuleName;注释掉是为了防止m_pcModuleName在其他地方
			// 释放掉，引起死机 eg：jsprint
			char * pcTemp = (char * ) malloc(strlen(pcModuleName)+1);
			if (pcTemp == NULL)
			{
					DEBUG_OUTPUTSTR("<CSDEBUG> ERROR ! CSDebugSet pModule->m_pcModuleName MALLOC mmemory failed!\r\n"); 
				return CSUDI_FAILURE;
			}
		
			strcpy(pcTemp,pcModuleName);
			
			pModule->m_pcModuleName = pcTemp;
    	}

    	pModule->m_nLevle = nOutputLevel;

    	DEBUGASSERT(pModule->m_pcModuleName);
    }
	
	DEBUG_FREE(s_hMute);
	return CSUDI_SUCCESS;	
}

void CSDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat, ...)
{
	if(pcFormat)
	{
		va_list Argument;
		va_start(Argument, pcFormat);
		CSVDebug(pcModuleName,nOutputLevel, pcFormat, Argument);
		va_end(Argument);
	}
}

void CSVDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat,va_list arg)
{
	//CSUDI_Error_Code tErr = CSUDI_FAILURE;
	CSUDI_BOOL bNeedPrint = CSUDI_TRUE;
	
	CSUDIPlusDebugInit();

	if(s_hDebugRecursionMutex)
	{		
		CSUDIOSMutexWait(s_hDebugRecursionMutex,5000);
	//	tErr = CSUDIOSMutexWait(s_hDebugRecursionMutex,5000);
	//	DEBUGASSERT(tErr == CSUDI_SUCCESS && "CSVDebug Wait1 Error");

		if(s_nRecursionCount++ >= 20)//20是个经验值
		{
	        //递归情况下不再继续打印，因为很可能是DEBUG_OUTPUTSTR造成的递归          
			bNeedPrint = CSUDI_FALSE;
			DEBUG_OUTPUTSTR("<CSDEBUG> Warning ! CSDebug Recursion Called! Pls check your source code!\r\n");		
		}

		CSUDIOSMutexRelease(s_hDebugRecursionMutex);
	}
	
	//不允许使用DISABLE_LEVEL打印!
	if(nOutputLevel == DISABLE_LEVEL)
	{
		nOutputLevel = CLI_LEVEL;
	}

	//如果bNeedPrint不成立,bNeedPrint = CSUDI_FALSE语句是不需要的,但它可以让后续的DebugCanPrint不执行
	if(!bNeedPrint || !pcFormat || !DebugCanPrint(pcModuleName,nOutputLevel))
	{
		bNeedPrint = CSUDI_FALSE;
	}

	if(bNeedPrint)
	{
		int nLen = 0;
		int nOutLen;
		char strOutBuff[1024] = {0};
		int nOutBuffLen = sizeof(strOutBuff) - 1;
		int nRemainLen = 0;
		//add head of print
		if (s_bInNewLine && nOutputLevel == ERROR_LEVEL)
		{
			int nTemOutLen = 0;

			nLen = sprintf(strOutBuff,CSDBG_LIGHT_RED"<%s>"CSDBG_ERROR_PREFIX, pcModuleName);

			nRemainLen = nOutBuffLen - nLen - strlen(CSDBG_NONE);
			nOutLen = vsnprintf(strOutBuff + nLen, nRemainLen, pcFormat, arg);		
			nOutLen = (nOutLen >= nRemainLen) ? nRemainLen : nOutLen;

			nRemainLen = nOutBuffLen - nLen - nOutLen;
			nTemOutLen = snprintf(strOutBuff + nLen + nOutLen, nRemainLen, "%s", CSDBG_NONE);
			nTemOutLen = (nTemOutLen >= nRemainLen) ? nRemainLen : nTemOutLen;
			
			nOutLen += nTemOutLen;
		}
		else if(nOutputLevel == ERROR_LEVEL)
		{
			int nTemOutLen = 0;

			nLen = sprintf(strOutBuff,"%s ", CSDBG_LIGHT_RED);

			nRemainLen = nOutBuffLen - nLen - strlen(CSDBG_NONE);
			nOutLen = vsnprintf(strOutBuff + nLen, nRemainLen, pcFormat, arg);
			nOutLen = (nOutLen >= nRemainLen) ? nRemainLen : nOutLen;

			nRemainLen = nOutBuffLen - nLen - nOutLen;
			nTemOutLen = snprintf(strOutBuff + nLen + nOutLen, nRemainLen, "%s", CSDBG_NONE);
			nTemOutLen = (nTemOutLen >= nRemainLen) ? nRemainLen : nTemOutLen;
			
			nOutLen += nTemOutLen;	
		}
		else 
		{
			if(s_bInNewLine)
			{
				nLen = sprintf(strOutBuff,"<%s> ",pcModuleName);
			}

			nRemainLen = nOutBuffLen - nLen;
			nOutLen = vsnprintf(strOutBuff + nLen, nRemainLen, pcFormat, arg);
			nOutLen = (nOutLen >= nRemainLen) ? nRemainLen : nOutLen;
		}

		DEBUG_OUTPUTSTR(strOutBuff);
		
		nLen = (int)strlen(strOutBuff);		
		DEBUGASSERT(nLen > (int)strlen(CSDBG_NONE));
		DEBUGASSERT(nLen < nOutBuffLen);
		DEBUGASSERT(nOutLen < nOutBuffLen);
		s_bInNewLine = (nLen > 0 ? (strOutBuff[nLen-1] == '\n'||strOutBuff[nLen-strlen(CSDBG_NONE)-1] == '\n'):CSUDI_FALSE);
	}

	if(s_hDebugRecursionMutex)
	{
		CSUDIOSMutexWait(s_hDebugRecursionMutex,5000);
//		tErr = CSUDIOSMutexWait(s_hDebugRecursionMutex,5000);
//		DEBUGASSERT(tErr == CSUDI_SUCCESS && "CSVDebug Wait2 Error");
		
		s_nRecursionCount--;
		DEBUGASSERT(s_nRecursionCount >= 0);
		if(s_nRecursionCount < 0)
		{
			s_nRecursionCount = 0;
		}
		
		CSUDIOSMutexRelease(s_hDebugRecursionMutex);
	}
}

int CSDebugGetModuleInfo (const char** ppModules,int nMaxCount)
{
	int i;
	int nIndex = 0;
	int nCount = 0;
	if (!s_bModulesInited)
	{
		if(ppModules && nMaxCount >0)
		{
			ppModules[0] = CSUDI_NULL;
		}
		return 0;
	}
	
	DEBUG_LOCK(s_hMute);

	nMaxCount--;

	DEBUG_OUTPUTSTR("<CSDebug> List All Modules Registed:\n");
	
	for (i=0;i<MAX_MODULE_CNT;i++)
	{
		if (s_asAllModulesInfo[i].m_pcModuleName != CSUDI_NULL)
		{
			DEBUG_OUTPUTSTR(s_asAllModulesInfo[i].m_pcModuleName);
			DEBUG_OUTPUTSTR("\n");
			nCount++;

			if(ppModules && nMaxCount > nIndex)
			{
				ppModules[nIndex++] = s_asAllModulesInfo[i].m_pcModuleName;
			}
		}
#ifndef USE_HASH
		else
		{
			break;
		}
#endif
	}
	DEBUG_FREE(s_hMute);
	
	if(ppModules && nMaxCount >= 0)
	{
		DEBUGASSERT(nIndex <= nMaxCount);
		ppModules[nIndex] = NULL;
	}

	return nCount;
}

CSUDI_Error_Code CSDebugRegisterOutputCallback(CSDebugOutPutString OutputCallback)
{
	s_pfOutputFunction = OutputCallback;
	return CSUDI_SUCCESS;
}

static CSUDI_BOOL DebugCanPrint(const char * pModuleName, int nOutputLevel)
{
	CSDebugModule_S* pModule;
	if (!s_bModulesInited || pModuleName == NULL)
	{
		return CSUDI_FALSE;
	}
	
	DEBUG_LOCK(s_hMute);

	pModule = DebugSearchModule(pModuleName);
	if (pModule == NULL)
	{
		DEBUG_OUTPUTSTR("<CSDEBUG> Warning ! Too many modules registed in CSDebug\r\n");

		DEBUG_FREE(s_hMute);
		return CSUDI_FALSE;
	}

	if (pModule->m_pcModuleName == CSUDI_NULL)//new module,init it
	{
		// pModule->m_pcModuleName = pModuleName;注释掉是为了防止m_pcModuleName在其他地方
		// 释放掉，引起死机 eg：jsprint
		char * pcTemp = (char * ) malloc(strlen(pModuleName)+1);
		if (pcTemp == NULL)
		{
			DEBUG_OUTPUTSTR("<CSDEBUG> ERROR ! DebugCanPrint pModule->m_pcModuleName MALLOC mmemory failed!\r\n"); 
			return CSUDI_FAILURE;
		}
		
		strcpy(pcTemp,pModuleName);
		
		pModule->m_pcModuleName = pcTemp;
		pModule->m_nLevle = s_nDefaultLevle;
	}

	DEBUGASSERT(pModule->m_pcModuleName);

	if (nOutputLevel < pModule->m_nLevle)
	{
		DEBUG_FREE(s_hMute);
		return CSUDI_FALSE;
	}

	DEBUG_FREE(s_hMute);
	return CSUDI_TRUE;
}

static CSDebugModule_S* DebugSearchModule(const char * pModuleName)
{
	CSDebugModule_S* pModule = CSUDI_NULL;
	DEBUGASSERT (pModuleName);
	
	if(pModuleName != CSUDI_NULL)
	{
#ifdef USE_HASH
		{
			unsigned int nIndex = DebugGetHashTablePos(pModuleName,strlen(pModuleName));
			if (nIndex != ~0)
			{
				DEBUGASSERT (nIndex < MAX_MODULE_CNT);

				pModule = &s_asAllModulesInfo[nIndex];
			}
		}
#else
		for (int i=0;i<MAX_MODULE_CNT;i++)
		{
			pModule = &s_asAllModulesInfo[i];
			DEBUGASSERT(pModule);

			if (pModule->m_pcModuleName == CSUDI_NULL || strcmp(pModule->m_pcModuleName,pModuleName) == 0)
			{
				break;
			}
		}
#endif
	}
	return pModule;
}

#ifdef USE_HASH
//hash function from lua
static unsigned int DebugHashString(const char *pcString,unsigned int nStringLength)
{
	int i;
	unsigned int nRe = nStringLength;
	unsigned int nStep = (nStringLength >> 5) + 1;

	for(i=nStringLength; i>=nStep; i-=nStep)
	{
		/*lint -e571  need to confirm with lihui*/
		nRe = nRe ^ ((nRe<<5)+(nRe>>2)+(unsigned int)pcString[i-1]);
	}
   	return nRe;
}

static unsigned int DebugGetHashTablePos(const char *pcString,unsigned int nStringLength)
{
	unsigned int dwHash = DebugHashString(pcString,nStringLength);
	unsigned int dwHashStart = dwHash % MAX_MODULE_CNT;
	unsigned int dwHashPos = dwHashStart;
	
	if(pcString != NULL)
	{
		while(s_asAllModulesInfo[dwHashPos].m_pcModuleName != NULL)
		{ 
			if (strcmp(s_asAllModulesInfo[dwHashPos].m_pcModuleName,pcString) == 0) 
			{
				break;
			}
			else 
			{
				dwHashPos = (dwHashPos + 1) % MAX_MODULE_CNT;
			}

			if (dwHashPos == dwHashStart)//full
			{
				dwHashPos = ~0;
				break; 
			}
		}
	}
	return dwHashPos;
} 

#endif

void  CSSetDebugLevel(int nLevel)
{
	//DEBUG_OUTPUTSTR("<CSDEBUG> Warning ! This is a Deprecated function which not suggested to use! Pls use CSDebug instead! See cs_assert.h for more details\r\n");
	/*lint -e534*/
	CSDebugSetDefaultLevel(nLevel);
}

void USPPrint(int nLevel,const char *format, ...)
{
	va_list Argument;
	va_start(Argument, format);
	CSVDebug("Deprecated Print",nLevel,format,Argument);
	va_end(Argument);
}

