#include "cs_testkit.h"
#include "cs_testkit_porting.h"
#include <stdarg.h>
#include <mmcp_debug.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "version.h"
#include "mmcp_os.h"
#include <assert.h>

#define TEST_CFG_FILE  		 "test_config_"MMCP_PLATFORM".cfg"
#define TEST_REPORT_FILE	 "testReport_"MMCP_PLATFORM".txt"
#define TEST_CASE_SAVE_FILE	 "test_cases_Save.txt"

#define CSITK_VERSION        "CSITK_01.01.03_"SRC_VERSION
#define MODULE_NAME			 "ITK"

#define CSITK_PROMPT		 "Please select(b-Back,m-Menu,q-Quit,l-List cases,s-Save cases to a file,a-Change auto mode,c-Choose execute cases,e-Choose execute modules):"

///< todo
typedef enum
{
	EM_TEST_IGNORE,
	EM_TEST_CD,
	EM_TEST_RUN,
	EM_TEST_BACK,
	EM_TEST_MENU,
	EM_TEST_QUIT,
	EM_TEST_LISTCASES,
	EM_TEST_AUTOMANUAL,
	EM_TEST_RECRUSIVE_SAVECASES,
	EM_TEST_SELECTCASES,
	EM_TEST_SELECTMODULES,
	EM_TEST_INIT,
	EM_TEST_UNINIT
}CmdType_E;

// ��ܽ��漰�߼�����
static char s_szPwd[256] = "";
//static  CSTestCase_S * s_psCurTestCase;
static void CSTKPrint(char *format, ...);///< ���Կ�ܴ�ӡ���������Կ��ʹ�ã�������������ʹ��
static void CSTKPopMenu(CSTestKitHelper_S* psHelper);
static void CSTKPopTestCases(CSTestKitHelper_S* psHelper);

// �Զ����в���
static CSUDI_UINT32 s_dwTestMode = EM_AUTO_ANWSER_MASK;
static CSUDI_BOOL s_bAutoStartAndExit = CSUDI_FALSE;
static CSUDI_BOOL s_bSemiAutoFlag = CSUDI_FALSE;

// ���Ա��沿��
#define LINE_REPORT_LENGTH		(512)

#define OUTPUT_TO_FILE           (1 << 0)
#define OUTPUT_TO_PRINT          (1 << 1)
#define OUTPUT_TO_ALL            (~0)

//����
static int s_nWhereIam = 0;
static CSUDI_BOOL s_bInputInit = CSUDI_FALSE;
static CSUDI_BOOL s_bFirtstRunFlag = CSUDI_FALSE;
static int s_nMoudleCount = 0;

static CSUDI_BOOL s_bManulMode = CSUDI_TRUE;
static CSUDI_BOOL s_bAutoMode = CSUDI_TRUE;
static CSUDI_BOOL s_bShowAutoResult = CSUDI_FALSE;
static CSUDI_BOOL s_bShowManulResult = CSUDI_FALSE;

//ִ��ָ����Χ��case
static CSUDI_BOOL s_bSelectModuleFlag = CSUDI_FALSE;
static int s_nStartModule = 0;
static int s_nLastModule = 0;

typedef enum
{
	EM_BASE_DECIMAL=10,
	EM_BASE_HEX=16
}BaseType_E;

typedef enum
{
    EM_TESTMODE_AA=1,
    EM_TESTMODE_MM=2,
    EM_TESTMODE_AM=3
}TestMode_E;


#define MK_ONECASE_REPORT(buf,name,type,result,des)  \
		do{ \
			sprintf(buf,"%-54s\t%s\t%s\t<%s>\r\n", name,type,result,des);    \
		}while(0)

#define OUTPUT_RESULT(to,s) \
	do{	\
		if(to & OUTPUT_TO_PRINT)CSTKPrint("%s",s);\
		if(to & OUTPUT_TO_FILE)FileReportGenerater(s);\
	}while(0)

static char s_szCharNuffer[LINE_REPORT_LENGTH];		
static CSUDI_HANDLE s_pReportFile = CSUDI_NULL;

// ��������,assert,�������ֶ���
#define ASSERT_DES_COUNT		(8192)	//����ʧ����������
static const char* s_apAssertDes[ASSERT_DES_COUNT] = {0};
static int s_nAssertDesIndex = 0;
static int s_nAssertDesBefore = -1;
CSUDI_BOOL g_bAssertBeforeFatalPoint = CSUDI_TRUE;

#define CSASSERT(x)

void CSDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat, ...)
{
	return;
}
void CSVDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat,va_list arg)
{
    
    CSTKP_VPrintf((char *)pcFormat,arg);
    return;
}

static void CSTKPrint(char *format, ...)
{	
	va_list arglist;
	
	va_start( arglist, format );
	CSTKP_VPrintf(format,arglist);
	va_end(arglist);
}

void CSTCPrint(char *format, ...)
{
	va_list arglist;
	
	va_start( arglist, format );
	CSVDebug("TKC",CLI_LEVEL,format,arglist);
	va_end(arglist);
}

static void FileReportGenerater(const char *szOut)
{
	//����ļ����ΪNULL���������Ա����ļ���û�д��������Դ�������szOutΪ������ģ��������ʱ����Ҫ���ļ���д����
	//���Ա�������Ϊ"ģ����_TEST_REPORT_FILE"
	if(s_pReportFile == CSUDI_NULL)
	{
		char acFileName[256];
		memset(acFileName, 0, sizeof(acFileName));
		sprintf(acFileName, "%s_%s", szOut, TEST_REPORT_FILE);

		s_pReportFile = CSTKP_FOpen(acFileName, "w");
		CSASSERT(s_pReportFile != CSUDI_NULL);	
		
		if(s_pReportFile != CSUDI_NULL)
		{			
			FileReportGenerater( "=========================================\r\n");
			FileReportGenerater( "               TEST REPORT\r\n");
			FileReportGenerater( "TEST PLATFORM:		"MMCP_PLATFORM"\r\n");
			FileReportGenerater( "TESTKit version:	"CSITK_VERSION"\r\n");
			FileReportGenerater( "TEST lib compiling DATE:"__DATE__" "__TIME__"\r\n");
			FileReportGenerater( "=========================================\r\n");
		}
	}
	else if(szOut != CSUDI_NULL)
	{
		CSTKP_FWrite(szOut,1,strlen(szOut),s_pReportFile);
		CSTKP_FFlush(s_pReportFile);
	}
}

static char* CSTKGetAutoModeString(CSUDI_UINT32 dwMode)
{
	static char acModeString[16];

	if(EM_AUTO_ANWSER_MASK & dwMode)
	{
		acModeString[0] = 'A';
	}
	else
	{
		acModeString[0] = 'M';
	}
	
	if(EM_AUTO_NO_MANUL_MASK & dwMode)
	{
		acModeString[1] = 'A';
	}
	else
	{
		acModeString[1] = 'M';
	}

	acModeString[2] = 0;

	return acModeString;	
}

static void CSTestKit_iLower(char* pKey)
{
	while(pKey != CSUDI_NULL && *pKey != '\0')
	{
		if(isupper(*pKey))//ֻ�д�д�Ž���ת��
		{
			*pKey = tolower(*pKey);
		}
		
		pKey++;
	}
}

static CSUDI_BOOL CSTestKit_iConvStringToInt(const char* szString, int *pnInt,char* pTail)
{
	int i;
	int nLen = strlen(szString);
	int nVal = 0;

	if(nLen <= 0)
	{
		return CSUDI_FALSE;
	}

	for(i=0; i<nLen; ++i)
	{
		if(szString[i]<'0' || szString[i]>'9')
		{
			if(pTail && i > 0 && i == nLen - 1)//only support last 1 char tail,must at least get 1 int
			{
				*pTail = szString[i];
				break;				
			}
			else
			{
				return CSUDI_FALSE;
			}
		}

		nVal = nVal * 10 + szString[i]-'0';
	}

	*pnInt = nVal;

	return CSUDI_TRUE;

}

static void CSTestKit_iGetCmd(CmdType_E *peType, int *pnNum,char* pCmdTail)
{
	static int s_nGetCmdCount = 0;
	
	int nVal;
	
	if(peType==CSUDI_NULL || pnNum==CSUDI_NULL)
	{
		return ;
	}
	
	++s_nGetCmdCount;
	*pnNum = 0;
	
	if(s_nGetCmdCount==1 && s_bAutoStartAndExit)
	{
		*peType = EM_TEST_RUN;
		return;
	}

	memset(s_szCharNuffer,0,sizeof(s_szCharNuffer));
	CSTKP_Gets(s_szCharNuffer,sizeof(s_szCharNuffer));
	CSTestKit_iLower(s_szCharNuffer);

	if(CSTestKit_iConvStringToInt(s_szCharNuffer, &nVal,pCmdTail))
	{
		if(nVal == 0)
		{
			*peType = EM_TEST_RUN;
		}
		else
		{
			*peType = EM_TEST_CD;
			*pnNum = nVal;
		}
		return ;
	}

	if(s_szCharNuffer[1] == 0 || s_szCharNuffer[1] == '\n' || s_szCharNuffer[1] == '\r')
	{
		switch(s_szCharNuffer[0])
		{
			case 'a':
				*peType = EM_TEST_AUTOMANUAL;
				return;
			case 'l':
				*peType = EM_TEST_LISTCASES;
				return;
			case 's':
				*peType = EM_TEST_RECRUSIVE_SAVECASES;
				return;
			case 'm':
				*peType = EM_TEST_MENU;
				return;
			case 'b':
				*peType = EM_TEST_BACK;
				return;
			case 'q':
				*peType = EM_TEST_QUIT;
				return;
			case 'i':
				*peType = EM_TEST_INIT;
				return;
			case 'u':
				*peType = EM_TEST_UNINIT;
				return;
			case 'c':
				*peType = EM_TEST_SELECTCASES;
				return;
			case 'e':
				*peType = EM_TEST_SELECTMODULES;
				return;
			default:
				break;
		}
	}

	*peType = EM_TEST_IGNORE;
}

static void CSTestKit_iClearDes(void)
{
	memset(s_apAssertDes,0,sizeof(s_apAssertDes));
	s_nAssertDesIndex = 0;
}


static void CSTestKit_iShowOneTestCase(int nCaseId, CSTestCase_S *psTC,int nLevel, int nOutMode)
{
	int i;
	char * pType = CSUDI_NULL;
	char* pResult = CSUDI_NULL;
	int nDesStart;
	int nDesCnt;
	for(i=0; i<nLevel; ++i)
	{
		s_szCharNuffer[i] = '\t';
	}

	if(psTC->eCaseType == EM_TK_AUTO)
	{
		pType = "A";
	}
	else if(psTC->eCaseType == EM_TK_SEMIAUTO)
	{		
		pType = "SA";
	}
	else if(psTC->eCaseType == EM_TK_MANUAL)
	{
		pType = "M";
	}
	else
	{
		CSASSERT(0);
		pType = "Unkonwn";
	}

	if(psTC->eResult==EM_TK_SUCCESS)
	{
		pResult = "Pass";
	}
	else if(psTC->eResult==EM_TK_FAILED)
	{
		pResult = "Fail";
	}
	else if(psTC->eResult==EM_TK_NOT_TEST)
	{
		pResult = "N/A";
	}
	else
	{
		CSASSERT(0);		
		pResult = "Unkonwn";			
	}
	
	sprintf(s_szCharNuffer+nLevel, "%d - %s:(%s) %s!\r\n",
		nCaseId, psTC->szName,pType,pResult);

	OUTPUT_RESULT(nOutMode,s_szCharNuffer);

	nDesStart = psTC->nDesStart;
	nDesCnt = psTC->nDesCnt;

	if(nDesStart>=0 && nDesCnt > 0)
	{
		int j;
		for(j=nDesStart;j<nDesStart+nDesCnt && j<ASSERT_DES_COUNT;++j)
		{
			sprintf(s_szCharNuffer+nLevel, "\t<%s>\r\n",s_apAssertDes[j]);
			OUTPUT_RESULT(nOutMode,s_szCharNuffer);
		}
	}
}

static void CSTestKit_iDoInit(CSTestKitHelper_S *psHelper,CmdType_E cmd)
{
	CSTestObj_S *psTestObj = (psHelper->pfnTestGetObj)[0]();	
	CSASSERT(psTestObj);
	
	CSASSERT(psHelper);
	CSASSERT(psHelper->psInit);	
	
	if(cmd == EM_TEST_INIT && psHelper->psInit->fnInit)
	{
		CSTKPrint("Doing %s init...\n",psTestObj->szName);
		psHelper->psInit->fnInit();
		CSTKPrint("Init %s Finished!\n",psTestObj->szName);
	}
	else if(cmd == EM_TEST_UNINIT && psHelper->psInit->fnUnInit)
	{
		CSTKPrint("Doing %s uninit...\n",psTestObj->szName);
		psHelper->psInit->fnUnInit();
		CSTKPrint("Uninit %s Finished!\n",psTestObj->szName);
	}
}

static void CSTestKit_iProcCd(CSTestKitHelper_S *psHelper, int nNum,char cTail)
{
	if(nNum <= 0)
	{
		return;
	}

	if(psHelper->nTestGetObjCount > 1 && nNum < psHelper->nTestGetObjCount)
	{
		int nLen = strlen(s_szPwd);
		CSTestObj_S *psObj = (psHelper->pfnTestGetObj)[nNum]();
		s_nWhereIam ++;
		psObj->fnEntry();
		s_szPwd[nLen] = 0;
		CSTKPopMenu(psHelper);
	}
	else
	{
		int i,jj;
		int nTestCaseCount = psHelper->nTestCount;
		CSUDI_BOOL bSuccess;
		CSTestCase_S *psTC;
		int nEndCase;

		i = nNum - psHelper->nTestGetObjCount;
		CSASSERT(i >= 0);

		if(i >= nTestCaseCount)
		{
			return;
		}

		if(cTail == '+')
		{
			nEndCase = nTestCaseCount;
		}
		else
		{
			nEndCase = i+1;
		}

		CSTestKit_iClearDes();

		if(psHelper->psInit->fnInit != CSUDI_NULL)
		{
			psHelper->psInit->fnInit();
		}

		for(jj=i;jj<nEndCase;jj++)
		{
			psTC = psHelper->psTestCase+jj;
			CSTKPrint("\nTesting %s..................................\n", psTC->szName);

			//s_psCurTestCase = psTC; //to make sure
			
			psTC->nDesStart= -1;
			psTC->nDesCnt = 0;
			
			s_bSemiAutoFlag = CSUDI_FALSE;
			
			s_nAssertDesBefore = s_nAssertDesIndex;
			g_bAssertBeforeFatalPoint = CSUDI_TRUE;
			
			bSuccess = psTC->fnTestCase();
			
			if(s_nAssertDesIndex > s_nAssertDesBefore)//��assert
			{
				bSuccess = CSUDI_FALSE;
				psTC->nDesStart = s_nAssertDesBefore;
				psTC->nDesCnt = s_nAssertDesIndex - s_nAssertDesBefore;
			}
			
			psTC->eResult = bSuccess ? EM_TK_SUCCESS: EM_TK_FAILED;
			//s_psCurTestCase = CSUDI_NULL;//to make sure
		}

		if(psHelper->psInit->fnUnInit != CSUDI_NULL)
		{
			psHelper->psInit->fnUnInit();
		}

		CSTKPrint("=========================================\r\n");
		for(jj=i;jj<nEndCase;jj++)
		{
			psTC = psHelper->psTestCase+jj;

			//���ʱ�򲻲������Ա��棬����ӡ
			CSTestKit_iShowOneTestCase(jj+1, psTC, 0, OUTPUT_TO_PRINT);
		}
		CSTKPrint("=========================================\r\n");
		CSTKPrint(CSITK_PROMPT);
		CSTKP_Flush();

	}
}

void CSTKPopMenu(CSTestKitHelper_S* psHelper)
{
	int i;
	CSTestObj_S *psTestObj = (psHelper->pfnTestGetObj)[0]();
		
	CSTKPrint("\n\n");
	CSTKPrint("Currrent Model:%s\n", s_szPwd);
	CSTKPrint("------------------------------------------------\n");
	CSTKPrint("0 - Test All	[%s]\n", CSTKGetAutoModeString(s_dwTestMode));
	
	CSASSERT(psTestObj);
	if(psHelper->psInit->fnInit)
	{
		CSTKPrint("i - do %s Init\n",psTestObj->szName);
	}
	if(psHelper->psInit->fnUnInit)
	{
		CSTKPrint("u - do %s UnInit\n",psTestObj->szName);
	}
	
	for(i=1; i<psHelper->nTestGetObjCount; ++i)
	{
		psTestObj = (psHelper->pfnTestGetObj)[i]();
		CSTKPrint("%d - Select %s\n", i, psTestObj->szName);
	}
	CSTKPrint("------------------------------------------------\n");
	CSTKPrint("Note:enter sub moudle will not auto call init,but Test All will do it\r\n");
	CSTKPrint(CSITK_PROMPT);
	CSTKP_Flush();
}

void CSTKPopTestCases(CSTestKitHelper_S* psHelper)
{
	int i;
	CSTestCase_S *psTestCase;
	int nStart;
	CSTKPrint("\n\n");
	CSTKPrint("All Test Case of Model:%s \n", s_szPwd);
	CSTKPrint("------------------------------------------------\n");
	CSTKPrint("0 - Test All	[%s]\n", CSTKGetAutoModeString(s_dwTestMode));
	nStart = psHelper->nTestGetObjCount;
	for(i=0; i<psHelper->nTestCount; ++i)
	{
		psTestCase =  &(psHelper->psTestCase[i]);
		CSTKPrint("%d - Test %s\n", nStart + i, psTestCase->szName);
	}
	CSTKPrint("------------------------------------------------\n");
	CSTKPrint(CSITK_PROMPT);
	CSTKP_Flush();
	
}

void CSTKShowResult(CSTestKitHelper_S* psHelper, int nLevel,int nMode,CSUDI_BOOL bWithDetails)
{
	int i;
	int nPercet;
	
	for(i=0; i<nLevel; ++i)
	{
		s_szCharNuffer[i] = '\t';
	}

	sprintf(s_szCharNuffer+i, "Sum of Module:[%s]\r\n",psHelper->psTestObj->szName);
	
	OUTPUT_RESULT(nMode,s_szCharNuffer);
	memset(s_szCharNuffer+i,0,sizeof(s_szCharNuffer)-i);
	if((psHelper->psTestReport->nSuccess + psHelper->psTestReport->nFailed) > 0)
	{
		nPercet = psHelper->psTestReport->nSuccess*100/(psHelper->psTestReport->nSuccess + psHelper->psTestReport->nFailed);
	}
	else
	{
		nPercet = 0;
	}
	
	sprintf(s_szCharNuffer+i, "	All:%d, Test:%d, Success:%d,Success Rate:%d%% \r\n",
		psHelper->psTestReport->nAll,
		psHelper->psTestReport->nFailed+psHelper->psTestReport->nSuccess,
		psHelper->psTestReport->nSuccess,
		nPercet
		);
	OUTPUT_RESULT(nMode,s_szCharNuffer);

    memset(s_szCharNuffer+i,0,sizeof(s_szCharNuffer)-i);
	if((psHelper->psTestReport->nAutoSuccess + psHelper->psTestReport->nAutoFailed) > 0)
	{
		nPercet = psHelper->psTestReport->nAutoSuccess*100/(psHelper->psTestReport->nAutoSuccess + psHelper->psTestReport->nAutoFailed);
	}
	else
	{
		nPercet = 0;
	}
	sprintf(s_szCharNuffer+i, "	AutoCase:%d, Test:%d,Success:%d,Success Rate:%d%% \r\n",
		psHelper->psTestReport->nAutoCase,
		psHelper->psTestReport->nAutoSuccess + psHelper->psTestReport->nAutoFailed,
		psHelper->psTestReport->nAutoSuccess,
		nPercet
		);
	OUTPUT_RESULT(nMode,s_szCharNuffer);
    
   	memset(s_szCharNuffer+i,0,sizeof(s_szCharNuffer)-i);
	if((psHelper->psTestReport->nSemiautoSuccess+ psHelper->psTestReport->nSemiautoFailed) > 0)
	{
		nPercet = psHelper->psTestReport->nSemiautoSuccess*100/(psHelper->psTestReport->nSemiautoSuccess + psHelper->psTestReport->nSemiautoFailed);
	}
	else
	{
		nPercet = 0;
	}
	sprintf(s_szCharNuffer+i, "	Semi-AutoCase:%d, Test:%d,Success:%d,Success Rate:%d%% \r\n",
		psHelper->psTestReport->nSemiautoCase,
		psHelper->psTestReport->nSemiautoSuccess + psHelper->psTestReport->nSemiautoFailed,
		psHelper->psTestReport->nSemiautoSuccess,
		nPercet
		);
	OUTPUT_RESULT(nMode,s_szCharNuffer);

    memset(s_szCharNuffer+i,0,sizeof(s_szCharNuffer)-i);
	if((psHelper->psTestReport->nManulSuccess + psHelper->psTestReport->nManulFailed) > 0)
	{
		nPercet = psHelper->psTestReport->nManulSuccess*100/(psHelper->psTestReport->nManulSuccess + psHelper->psTestReport->nManulFailed);
	}
	else
	{
		nPercet = 0;
	}
	sprintf(s_szCharNuffer+i, "	ManulCase:%d, Test:%d,Success:%d,Success Rate:%d%% \r\n",
		psHelper->psTestReport->nManulCase,
		psHelper->psTestReport->nManulSuccess + psHelper->psTestReport->nManulFailed,
		psHelper->psTestReport->nManulSuccess,
		nPercet
		);
	OUTPUT_RESULT(nMode,s_szCharNuffer);

	if(bWithDetails)
	{
		for(i=1; i<psHelper->nTestGetObjCount; ++i)
		{
			CSTestObj_S *psObj = psHelper->pfnTestGetObj[i]();

            if(s_bShowAutoResult || s_bShowManulResult)
			{
				psObj->fnShowResult(nLevel+1, nMode, bWithDetails);
			}
		}
	}

	if(bWithDetails && psHelper->nTestCount > 0)
	{
		for(i=0; i<psHelper->nTestCount; ++i)
		{
			CSTestCase_S *psTC = psHelper->psTestCase + i;

            if((s_bShowAutoResult && s_bShowManulResult)
				|| (s_bShowManulResult && (psTC->eCaseType == EM_TK_MANUAL))
				|| (s_bShowAutoResult && (psTC->eCaseType == EM_TK_AUTO))) 
            {
 			    CSTestKit_iShowOneTestCase(i+1, psTC, nLevel+1, nMode);
            }
        }
	}

	if(nLevel == 0)
	{
		OUTPUT_RESULT(nMode,"=========================================\r\n");

		if(nMode & OUTPUT_TO_FILE)
		{
			if(s_pReportFile != CSUDI_NULL)
			{
				CSTKP_FClose(s_pReportFile);

				s_pReportFile = CSUDI_NULL;
			}
		}
	}
}

void CSTKEntry(CSTestKitHelper_S* psHelper)
{
	CSTestObj_S *psTestObj = (psHelper->pfnTestGetObj)[0]();
	char acInputCfgBuf[32] = {0};
	int nTempRet = -1;
	int nCountEntry = 0;
//	char acVer[256] = {0};
	
//	psTestObj->fnGetVersion(acVer, sizeof(acVer));

	CSTCPrint("==============================================\n");
//	CSTCPrint("%s testlib version:\n %s\n", psTestObj->szName, acVer);
	CSTCPrint("==============================================\n");
	
	sprintf(s_szPwd, "%s/%s", s_szPwd, psTestObj->szName);
		
	if(s_bInputInit==CSUDI_FALSE)
	{
		memset(acInputCfgBuf, '\0', sizeof(acInputCfgBuf));
		
		//while������������ƽ̨��ע����ļ�ϵͳ���ܴ򿪲���ȷ��ȡTEST_CFG_FILE
		while(1)
		{
			if(CSTKGetConfigInfo("UDI2", "UDI2TEST_INPUT_METHOD", acInputCfgBuf, sizeof(acInputCfgBuf)) == CS_TK_CONFIG_SUCCESS)
			{
				nTempRet = CSTKGetIntFromStr(acInputCfgBuf,EM_BASE_DECIMAL);
				
				CSTKPRegisterInput(nTempRet);
				s_bInputInit = CSUDI_TRUE;
				
				break;
			}
			else if(CSTKPGetFSType()==EM_TKP_FS_USB) //ע����USBFS
			{		
				CSTCPrint( "Read InputMethod faliure!��ȷ�������ļ�����ȷ��ȡ!\n");
//				CSUDIOSThreadSleep(500);//500ms��Ҫ�����U��Ҫһ��ʱ��ʶ��
				nCountEntry++;

				if(nCountEntry>120)//1  �ӵ�׼��ʱ��
				{
					CSTCPrint("�ȴ�����ȷ��ȡ�����ļ���ʱ-�˳����ԣ�\n");
					return;
				}
			}
			else
			{
				break;
			}
		}

		if (!s_bInputInit)
		{
			CSTKPRegisterInput(EM_INPUT_UART_SYS);
			s_bInputInit = CSUDI_TRUE;
			CSTCPrint("default input method: EM_INPUT_UART_SYS\n");
		}
	}
	
	CSTKPopMenu(psHelper);

	for(;;)
	{
		CmdType_E eType;
		int nNum;
		char cCmdTail = 0;

		CSTestKit_iGetCmd(&eType, &nNum,&cCmdTail);
		if(eType == EM_TEST_BACK)
		{
			if(s_nWhereIam ==0)
			{
				CSTKPopMenu(psHelper);
				continue;
			}
			else
			{
				s_nWhereIam --;
				return;				
			}
		}

		switch(eType)
		{
		case EM_TEST_AUTOMANUAL:
          	CSTKSetAutoRun(psHelper);
			break;
            
        case EM_TEST_SELECTCASES:
            CSTKSelecteCases(psHelper, 0);
            break;
		case EM_TEST_SELECTMODULES:
			CSTKSelecteModules(psHelper, 0);
			break;
            
		case EM_TEST_INIT:
		case EM_TEST_UNINIT:
			CSTestKit_iDoInit(psHelper,eType);
			break;
			
		case EM_TEST_LISTCASES:			
			CSTKPopTestCases(psHelper);
			break;
			
		case EM_TEST_MENU:
			CSTKPopMenu(psHelper);
			break;
			
		case EM_TEST_RUN:
			{
				CSTestObj_S *psObj = (psHelper->pfnTestGetObj)[0]();

				CSTestKit_iClearDes();

				psObj->fnRun(0);
                
				psObj->fnShowResult(0, OUTPUT_TO_PRINT,CSUDI_TRUE);
				
				if(s_bAutoStartAndExit)
				{
					CSTKP_Exit();
				}
				CSTKPrint(CSITK_PROMPT);
				CSTKP_Flush();
			}
			break;
			
		case EM_TEST_CD:
			{
				CSTestKit_iProcCd(psHelper, nNum,cCmdTail);
			}
			break;
			
		case EM_TEST_QUIT:
			{
				CSTKPrint("Quit............\n\n");
				CSTKP_Exit();
			}
			break;
		
		case EM_TEST_RECRUSIVE_SAVECASES:
			{
				CSTestObj_S *psObj = (psHelper->pfnTestGetObj)[0]();

				psObj->fnSaveCases(0);
				CSTKPrint("Save cases OK\r\n");
			}
			break;
		
		default:
			CSTKPrint("Your select is not correct!\n");
			CSTKPrint(CSITK_PROMPT);
			CSTKP_Flush();
			break;

		}
	}
}

/*�ú������ڵݹ�Ѳ����е����в��������Ͳ���ģ����б��浽�ļ�CasesSavedFile.txt��ȥ*/
void CSTKSaveCases(CSTestKitHelper_S* psHelper,int nLevel)
{
	int i;
	static  CSUDI_HANDLE pCasesSavedFile = CSUDI_NULL;
	int nTestCaseCount = psHelper->nTestCount;
	
	if(pCasesSavedFile == CSUDI_NULL)
	{
		pCasesSavedFile = CSTKP_FOpen(TEST_CASE_SAVE_FILE,"w");
		if(pCasesSavedFile != CSUDI_NULL)
		{
			CSTKPrint(TEST_CASE_SAVE_FILE" create OK\n");
		}
		else
		{
			CSTKPrint(TEST_CASE_SAVE_FILE" create failed\n");
		}
	}
	
	if(pCasesSavedFile != CSUDI_NULL)
	{
		sprintf(s_szCharNuffer,"%s\r\n", psHelper->psTestObj->szName);
		CSTKP_FWrite(s_szCharNuffer,1,strlen(s_szCharNuffer),pCasesSavedFile);
		CSTKP_FFlush(pCasesSavedFile);
		if(psHelper->nTestGetObjCount > 1)
		{
			for(i=1; i<psHelper->nTestGetObjCount; ++i)
			{
				CSTestObj_S *psObj = psHelper->pfnTestGetObj[i]();
							
				psObj->fnSaveCases(nLevel+1);
			}
		}
		
		if(nTestCaseCount > 0)
		{
			for(i=0; i<nTestCaseCount; ++i)
			{
				sprintf(s_szCharNuffer,"%s\r\n", (psHelper->psTestCase)[i].szName);
				CSTKP_FWrite(s_szCharNuffer,1,strlen(s_szCharNuffer),pCasesSavedFile);
				CSTKP_FFlush(pCasesSavedFile);
			}
		}

		if(nLevel == 0)
		{
			CSTKP_FClose(pCasesSavedFile);
		}
	}
}

void CSTKRun(CSTestKitHelper_S* psHelper,int nLevel)
{
    int i;
    int nTestCaseCount = psHelper->nTestCount;
    CSUDI_BOOL bManulCase;
    CSUDI_BOOL bSemiAuto;
    CSUDI_BOOL bCaseTested = CSUDI_FALSE;
        
    psHelper->psTestReport->nAll = 0;
    psHelper->psTestReport->nSuccess = 0;
    psHelper->psTestReport->nFailed = 0;
    psHelper->psTestReport->nAutoCase = 0;
    psHelper->psTestReport->nAutoSuccess = 0;
    psHelper->psTestReport->nAutoFailed = 0;    
    psHelper->psTestReport->nSemiautoCase= 0;
    psHelper->psTestReport->nSemiautoSuccess= 0;
    psHelper->psTestReport->nSemiautoFailed= 0;
    psHelper->psTestReport->nManulCase = 0;
    psHelper->psTestReport->nManulSuccess = 0;
    psHelper->psTestReport->nManulFailed = 0;

    if(psHelper->psInit->fnInit != CSUDI_NULL)
    {
        psHelper->psInit->fnInit();
    }

    //���Ա���һ������һ�����ɣ��Է�ֹ���й�������������������Ա����޷�����
    
    if(nLevel == 0)
    {
        //��һ�ν��룬�������Ա����ļ�
        OUTPUT_RESULT(OUTPUT_TO_FILE, psHelper->psTestObj->szName);
    }
    
    for(i=0; i<nLevel; ++i)
    {
        s_szCharNuffer[i] = '\t';
    }
    
    sprintf(s_szCharNuffer+i, "Begin to test module \"%s\":\r\n", psHelper->psTestObj->szName);
    
    OUTPUT_RESULT(OUTPUT_TO_FILE, s_szCharNuffer);

	if( s_bSelectModuleFlag )
    {
        if( 0 == s_nLastModule )
        {
			s_nLastModule = psHelper->nTestGetObjCount-1;
        }

	    if(psHelper->nTestGetObjCount > 1)
	    {
	    	if( s_bFirtstRunFlag )
            {  
                s_bFirtstRunFlag = CSUDI_FALSE;
		        for(i=s_nStartModule; i<=s_nLastModule; ++i)
		        {
		            CSTestReport_S sReport;
		            CSTestObj_S *psObj = psHelper->pfnTestGetObj[i]();
		    
		            psObj->fnRun(nLevel+1);
		    
		            psObj->fnGetReport(&sReport);
		            psHelper->psTestReport->nAll += sReport.nAll;
		            psHelper->psTestReport->nSuccess+= sReport.nSuccess;
		            psHelper->psTestReport->nFailed+= sReport.nFailed;
		            psHelper->psTestReport->nAutoCase += sReport.nAutoCase ;
		            psHelper->psTestReport->nAutoSuccess += sReport.nAutoSuccess;
		            psHelper->psTestReport->nAutoFailed += sReport.nAutoFailed;         
		            psHelper->psTestReport->nSemiautoCase+= sReport.nSemiautoCase;
		            psHelper->psTestReport->nSemiautoSuccess+= sReport.nSemiautoSuccess;
		            psHelper->psTestReport->nSemiautoFailed+= sReport.nSemiautoFailed;            
		            psHelper->psTestReport->nManulCase += sReport.nManulCase ;
		            psHelper->psTestReport->nManulSuccess += sReport.nManulSuccess;
		            psHelper->psTestReport->nManulFailed += sReport.nManulFailed;
		        }
            }
            else 
            {	
                for(i=1; i<psHelper->nTestGetObjCount; ++i)
                {
                    CSTestReport_S sReport;
                    CSTestObj_S *psObj = psHelper->pfnTestGetObj[i]();
                
                    psObj->fnRun(nLevel+1);
                
                    psObj->fnGetReport(&sReport);
                    psHelper->psTestReport->nAll += sReport.nAll;
                    psHelper->psTestReport->nSuccess+= sReport.nSuccess;
                    psHelper->psTestReport->nFailed+= sReport.nFailed;
                    psHelper->psTestReport->nAutoCase += sReport.nAutoCase ;
                    psHelper->psTestReport->nAutoSuccess += sReport.nAutoSuccess;
                    psHelper->psTestReport->nAutoFailed += sReport.nAutoFailed;         
                    psHelper->psTestReport->nSemiautoCase+= sReport.nSemiautoCase;
                    psHelper->psTestReport->nSemiautoSuccess+= sReport.nSemiautoSuccess;
                    psHelper->psTestReport->nSemiautoFailed+= sReport.nSemiautoFailed;            
                    psHelper->psTestReport->nManulCase += sReport.nManulCase ;
                    psHelper->psTestReport->nManulSuccess += sReport.nManulSuccess;
                    psHelper->psTestReport->nManulFailed += sReport.nManulFailed;
                }

            }
	    }
    }
    else
    {
	    if(psHelper->nTestGetObjCount > 1)
	    {
	        for(i=1; i<psHelper->nTestGetObjCount; ++i)
	        {
	            CSTestReport_S sReport;
	            CSTestObj_S *psObj = psHelper->pfnTestGetObj[i]();

	            psObj->fnRun(nLevel+1);
			
	            psObj->fnGetReport(&sReport);
	            psHelper->psTestReport->nAll += sReport.nAll;
	            psHelper->psTestReport->nSuccess+= sReport.nSuccess;
	            psHelper->psTestReport->nFailed+= sReport.nFailed;
	            psHelper->psTestReport->nAutoCase += sReport.nAutoCase ;
	            psHelper->psTestReport->nAutoSuccess += sReport.nAutoSuccess;
	            psHelper->psTestReport->nAutoFailed += sReport.nAutoFailed;         
	            psHelper->psTestReport->nSemiautoCase+= sReport.nSemiautoCase;
	            psHelper->psTestReport->nSemiautoSuccess+= sReport.nSemiautoSuccess;
	            psHelper->psTestReport->nSemiautoFailed+= sReport.nSemiautoFailed;            
	            psHelper->psTestReport->nManulCase += sReport.nManulCase ;
	            psHelper->psTestReport->nManulSuccess += sReport.nManulSuccess;
	            psHelper->psTestReport->nManulFailed += sReport.nManulFailed;
	        }
	    }
	}
    
    if(nTestCaseCount > 0)
    {
        CSTKPrint("Now test %s\n", psHelper->psTestObj->szName);

        for(i = 0; i<nTestCaseCount; ++i)
        {
            CSUDI_BOOL bSuccess = CSUDI_FALSE;
            CSTestCase_S* psTC = psHelper->psTestCase+i;

            psTC->nDesStart = -1;
            psTC->nDesCnt = 0;

            bCaseTested = CSUDI_FALSE;
            bManulCase = psTC->eCaseType == EM_TK_MANUAL ? CSUDI_TRUE:CSUDI_FALSE;
            bSemiAuto = CSUDI_FALSE;

            psHelper->psTestReport->nAll++;

			//�����ֶ�������
          	if(s_bManulMode && !s_bAutoMode && bManulCase)
            {
			 	 s_nAssertDesBefore = s_nAssertDesIndex;
			     g_bAssertBeforeFatalPoint = CSUDI_TRUE;

                 psHelper->psTestReport->nManulCase++;
                 psTC->eCaseType = EM_TK_MANUAL;

			     CSTKPrint("\n\n%s testing...%u\n", (psHelper->psTestCase)[i].szName,clock());

                 bSuccess = (psHelper->psTestCase)[i].fnTestCase();

			     if(s_nAssertDesIndex > s_nAssertDesBefore)//��assert
			     {
			         bSuccess = CSUDI_FALSE;
			         psTC->nDesStart = s_nAssertDesBefore;
			         psTC->nDesCnt = s_nAssertDesIndex - s_nAssertDesBefore;
			     }       
     
				 CSTKPrint("%s tested...%u----%s----\n", (psHelper->psTestCase)[i].szName,clock(), (bSuccess == CSUDI_TRUE)?"success":"fail");

				if(bSuccess)
				{
					psTC->eResult = EM_TK_SUCCESS;
					psHelper->psTestReport->nManulSuccess++;
					psHelper->psTestReport->nSuccess++;
				}
				else
				{
					psTC->eResult = EM_TK_FAILED;
					psHelper->psTestReport->nManulFailed++; 
					psHelper->psTestReport->nFailed++;
				}
                
                s_bShowManulResult = CSUDI_TRUE;
                s_bShowAutoResult = CSUDI_FALSE;
                CSTestKit_iShowOneTestCase(i+1, psTC, nLevel, OUTPUT_TO_FILE);                
			}

			//�Զ�����
			if(s_bAutoMode && (!s_bManulMode) && (!bManulCase))	
           	{
            	s_nAssertDesBefore = s_nAssertDesIndex;
				g_bAssertBeforeFatalPoint = CSUDI_TRUE;
               
                psHelper->psTestReport->nAutoCase++;
                psTC->eCaseType = EM_TK_AUTO;
                
			    CSTKPrint("\n\n%s testing...%u\n", (psHelper->psTestCase)[i].szName,clock());
				
                bSuccess = (psHelper->psTestCase)[i].fnTestCase();

			    if(s_nAssertDesIndex > s_nAssertDesBefore)//��assert
			    {
			        bSuccess = CSUDI_FALSE;
			        psTC->nDesStart = s_nAssertDesBefore;
			        psTC->nDesCnt = s_nAssertDesIndex - s_nAssertDesBefore;
			    }       
     
			    CSTKPrint("%s tested...%u----%s----\n", (psHelper->psTestCase)[i].szName,clock(), (bSuccess == CSUDI_TRUE)?"success":"fail");

				if(bSuccess)
				{
					psTC->eResult = EM_TK_SUCCESS;
					psHelper->psTestReport->nAutoSuccess++;
					psHelper->psTestReport->nSuccess++;
				}
				else
				{
					psTC->eResult = EM_TK_FAILED;
					psHelper->psTestReport->nAutoFailed++; 
					psHelper->psTestReport->nFailed++;
				}
                
                s_bShowAutoResult = CSUDI_TRUE;
                s_bShowManulResult = CSUDI_FALSE;
                CSTestKit_iShowOneTestCase(i+1, psTC, nLevel, OUTPUT_TO_FILE);
            }

			//ȫ������
            if(s_bAutoMode && s_bManulMode)	
           	{
            	s_nAssertDesBefore = s_nAssertDesIndex;
				g_bAssertBeforeFatalPoint = CSUDI_TRUE;
               
                psHelper->psTestReport->nAutoCase++;
                
			    CSTKPrint("\n\n%s testing...%u\n", (psHelper->psTestCase)[i].szName,clock());
				
                bSuccess = (psHelper->psTestCase)[i].fnTestCase();

			    if(s_nAssertDesIndex > s_nAssertDesBefore)//��assert
			    {
			        bSuccess = CSUDI_FALSE;
			        psTC->nDesStart = s_nAssertDesBefore;
			        psTC->nDesCnt = s_nAssertDesIndex - s_nAssertDesBefore;
			    }       
     
			    CSTKPrint("%s tested...%u----%s----\n", (psHelper->psTestCase)[i].szName,clock(), (bSuccess == CSUDI_TRUE)?"success":"fail");

				if(bSuccess)
				{
					psTC->eResult = EM_TK_SUCCESS;
					psHelper->psTestReport->nAutoSuccess++;
					psHelper->psTestReport->nSuccess++;
				}
				else
				{
					psTC->eResult = EM_TK_FAILED;
					psHelper->psTestReport->nAutoFailed++; 
					psHelper->psTestReport->nFailed++;
				}

                s_bShowManulResult = CSUDI_TRUE;
                s_bShowAutoResult = CSUDI_TRUE;      
                
                CSTestKit_iShowOneTestCase(i+1, psTC, nLevel, OUTPUT_TO_FILE);
            }
		}
        CSASSERT(psHelper->psTestObj != CSUDI_NULL && psHelper->psTestObj->fnShowResult != CSUDI_NULL);
        psHelper->psTestObj->fnShowResult(nLevel, OUTPUT_TO_FILE, CSUDI_FALSE);

		if(nLevel == 2)
        {        	
            CSTKPrint("=======================================================\n");
            
	        psHelper->psTestObj->fnShowResult(nLevel, OUTPUT_TO_PRINT, CSUDI_TRUE);
            
            CSTKPrint("=======================================================\n");
        }
        
        if((nLevel == 1) && (s_nMoudleCount <= s_nLastModule - s_nStartModule))
        {
        	s_nMoudleCount++;
            CSTKPrint("=======================================================\n");

        	psHelper->psTestObj->fnShowResult(nLevel, OUTPUT_TO_PRINT, CSUDI_TRUE);

            CSTKPrint("=======================================================\n");
		}

        CSASSERT(psHelper->psInit && psHelper->psInit->fnUnInit != CSUDI_NULL);
        if(psHelper->psInit->fnUnInit != CSUDI_NULL)
        {
            psHelper->psInit->fnUnInit();
        }
	}
}

void CSTKWaitAnyKey(void)
{
    CSTKPrint("Press any key then enter to continue...\n");
    CSTKP_Gets(s_szCharNuffer,sizeof(s_szCharNuffer));
    CSTestKit_iLower(s_szCharNuffer);
    return ;
}

char  CSTKWaitAnyKey_UDI_1_0(void)
{
    CSTKPrint("Press any key then enter to continue...\n");
    CSTKP_Gets(s_szCharNuffer,sizeof(s_szCharNuffer));
    CSTestKit_iLower(s_szCharNuffer);
    return s_szCharNuffer[0];
}
CSUDI_BOOL CSTKWaitYes(void)
{
    CSTKPrint("Please seclect YES-'y' or green key, NO-'n' or red key:\n");
	memset(s_szCharNuffer,0,sizeof(s_szCharNuffer));
	CSTKP_Gets(s_szCharNuffer,sizeof(s_szCharNuffer));
	CSTestKit_iLower(s_szCharNuffer);
	while(s_szCharNuffer[0] != 'y' && s_szCharNuffer[0] != 'n')
	{
		CSTKPrint("Your select is not correct!\n");
		CSTKPrint("Please seclect YES-'y' or green key, NO-'n' or red key:\n");
		CSTKP_Gets(s_szCharNuffer,sizeof(s_szCharNuffer));
		CSTestKit_iLower(s_szCharNuffer); 
	} 
	return (s_szCharNuffer[0] == 'y');
}

void CSTKSetAutoRun( CSTestKitHelper_S* psHelper)
{
    int nVal = 0;
    char cTail = 0;
    CSUDI_BOOL bFlag = CSUDI_TRUE;
    
    s_bShowAutoResult = CSUDI_FALSE;
    s_bShowManulResult = CSUDI_FALSE;
    
    CSTKPrint("******************************\n");
    CSTKPrint("<1> [AA] Excute Auto   Cases\n");
    CSTKPrint("<2> [MM] Excute Manual Cases\n");
    CSTKPrint("<3> [AM] Excute All    Cases\n");
    CSTKPrint("******************************\n");
    CSTKPrint("Please select one mode(q-Quit):");
        
    while(bFlag)
    {
        memset(s_szCharNuffer, 0, sizeof(s_szCharNuffer));
        CSTKP_Gets(s_szCharNuffer, sizeof(s_szCharNuffer));
        if(!strncmp(s_szCharNuffer, "q", 1) || !strncmp(s_szCharNuffer, "Q", 1))
        {
            break;
        }
        CSTestKit_iConvStringToInt(s_szCharNuffer, &nVal, &cTail);
        if(nVal < 1 || nVal > 3)
        {
            CSTKPrint(">>> Input Error!!!\n");
            CSTKPrint("Please select again(q-Quit):");
        }
        else 
        {
            bFlag = CSUDI_FALSE;
        }
    }    
    
    switch(nVal)
    {
        case EM_TESTMODE_AA: 
            CSTKPrint("Current Mode is: [AA]");
            s_dwTestMode = EM_AUTO_ANWSER_MASK | EM_AUTO_NO_MANUL_MASK;
            s_bManulMode = CSUDI_FALSE;
            s_bAutoMode = CSUDI_TRUE;
            break;
        case EM_TESTMODE_MM: 
            CSTKPrint("Current Mode is: [MM]");
            s_dwTestMode = EM_AUTO_ANWSER_MASK & EM_AUTO_NO_MANUL_MASK;
            s_bManulMode = CSUDI_TRUE;
            s_bAutoMode = CSUDI_FALSE;
            break;
        case EM_TESTMODE_AM: 
            CSTKPrint("Current Mode is: [AM]");
            s_dwTestMode = EM_AUTO_ANWSER_MASK;
            s_bManulMode = CSUDI_TRUE; 
            s_bAutoMode = CSUDI_TRUE;
            break;
        default: 
            break;
    }
    
    CSTKPopMenu(psHelper);
}

void CSTKSelecteCases( CSTestKitHelper_S* psHelper, int nLevel )
{
    char cTail = 0;
    CSUDI_BOOL bRunTestCaseFlag = CSUDI_TRUE;
    CSUDI_BOOL bRunEndNumFlag = CSUDI_FALSE;
    int i;
	int nTestCaseCount = psHelper->nTestCount;
    int nBeginNum = 0;
    int nEndNum = 0;
    char cCharBuffer[12];
    while( 1 )
    {  
    	if( nTestCaseCount <= 1)
       	{
       		CSTKPrint("----------------------------------------------------------\n");
            CSTKPrint( " >>> Sorry! you should selcet 'e' to choose Modules here.\n");
            CSTKPrint("----------------------------------------------------------\n");
            CSTKPrint(CSITK_PROMPT);
            
            bRunTestCaseFlag = CSUDI_FALSE;
            break;
        }
        CSTCPrint( "Please Input Start Case's Number(q - Quit): " );
        
        memset( s_szCharNuffer, 0, sizeof(s_szCharNuffer) );
        CSTKP_Gets( s_szCharNuffer, sizeof(s_szCharNuffer) );
        
        if( !strncmp(s_szCharNuffer, "q", 1) || !strncmp(s_szCharNuffer, "Q", 1) )
        {
            CSTKPopMenu( psHelper );
            bRunTestCaseFlag = CSUDI_FALSE;
            break;
        }
        
        CSTestKit_iConvStringToInt( s_szCharNuffer, &nBeginNum, &cTail );
            
        if( nBeginNum > nTestCaseCount || nBeginNum <= 0 )
        {
            CSTCPrint( "The Number Is Invalid, Input again!\n" );
            continue;               
        }
        else
        {
        	bRunEndNumFlag = CSUDI_TRUE;
			break;
        }
    }
    
    while( bRunEndNumFlag )
    {
        CSTCPrint( "Please Input The Last Case's Number(e - to the end, q - Quit): " );
        
        memset( s_szCharNuffer, 0, sizeof(s_szCharNuffer) );
        CSTKP_Gets(s_szCharNuffer, sizeof(s_szCharNuffer));
        
        if( !strncmp(s_szCharNuffer, "q", 1) || !strncmp(s_szCharNuffer, "Q", 1) )
        {
            CSTKPopMenu( psHelper );
            bRunTestCaseFlag = CSUDI_FALSE;
            break;
        }
        else if( !strncmp(s_szCharNuffer, "e", 1) || !strncmp(s_szCharNuffer, "E", 1) )
        {
            break;
        }
        else 
        {    
            CSTestKit_iConvStringToInt( s_szCharNuffer, &nEndNum, &cTail );
            
            if( nEndNum > nTestCaseCount || nEndNum <= 0 )
            {
                CSTCPrint( "The Number Is Invalid, Input again!\n" );
                continue;               
            }

            CSTKWaitAnyKey();
            break;
        }
    }    
   
    memset( cCharBuffer, 0, sizeof(cCharBuffer) );
    memcpy( cCharBuffer, s_szCharNuffer, 2 );
    
	if( bRunTestCaseFlag )
    {
    	psHelper->psTestReport->nAll = 0;
		psHelper->psTestReport->nSuccess = 0;
		psHelper->psTestReport->nFailed = 0;
		psHelper->psTestReport->nAutoCase = 0;
		psHelper->psTestReport->nAutoSuccess = 0;
		psHelper->psTestReport->nAutoFailed = 0;    
		psHelper->psTestReport->nSemiautoCase= 0;
		psHelper->psTestReport->nSemiautoSuccess= 0;
		psHelper->psTestReport->nSemiautoFailed= 0;
		psHelper->psTestReport->nManulCase = 0;
		psHelper->psTestReport->nManulSuccess = 0;
		psHelper->psTestReport->nManulFailed = 0;
	    
		if( psHelper->psInit->fnInit != CSUDI_NULL )
		{
			psHelper->psInit->fnInit();
		}
	    
		//���Ա���һ������һ�����ɣ��Է�ֹ���й�������������������Ա����޷�����
		if( nLevel == 0 )
		{
			//��һ�ν��룬�������Ա����ļ�
			OUTPUT_RESULT( OUTPUT_TO_FILE, psHelper->psTestObj->szName );
		}
	   
		for( i = 0; i < nLevel; ++i )
		{
			s_szCharNuffer[i] = '\t';
		}
	   
		sprintf( s_szCharNuffer+i, "Begin to test module \"%s\":\r\n", psHelper->psTestObj->szName );
	   
		OUTPUT_RESULT( OUTPUT_TO_FILE, s_szCharNuffer );

		if( nTestCaseCount > 0 )
		{
			CSTKPrint( "Now test %s\n", psHelper->psTestObj->szName );
	        
            if( strncmp(cCharBuffer, "e", 1) == 0 || strncmp(cCharBuffer, "E", 1) == 0 )
			{
				nEndNum = nTestCaseCount;
            }

            for( i = nBeginNum-1; i < nEndNum; ++i )
			{
				CSUDI_BOOL bSuccess = CSUDI_FALSE;
				CSTestCase_S* psTC = psHelper->psTestCase+i;

				psTC->nDesStart = -1;
				psTC->nDesCnt = 0;

				psHelper->psTestReport->nAll++;
                psHelper->psTestReport->nAutoCase++;

                s_nAssertDesBefore = s_nAssertDesIndex;
                g_bAssertBeforeFatalPoint = CSUDI_TRUE;
               
                CSTKPrint("\n\n%s testing...%u\n", (psHelper->psTestCase)[i].szName,clock());
               
                bSuccess = (psHelper->psTestCase)[i].fnTestCase();

                if(s_nAssertDesIndex > s_nAssertDesBefore)//��assert
				{
                    bSuccess = CSUDI_FALSE;
                    psTC->nDesStart = s_nAssertDesBefore;
                    psTC->nDesCnt = s_nAssertDesIndex - s_nAssertDesBefore;
				}       
     
                CSTKPrint("%s tested...%u----%s----\n", (psHelper->psTestCase)[i].szName,clock(), (bSuccess == CSUDI_TRUE)?"success":"fail");

                if(bSuccess)
                {
                    psTC->eResult = EM_TK_SUCCESS;
                    psHelper->psTestReport->nAutoSuccess++;
                    psHelper->psTestReport->nSuccess++;
                }
				else
				{
					psTC->eResult = EM_TK_FAILED;
                    psHelper->psTestReport->nAutoFailed++; 
                    psHelper->psTestReport->nFailed++;
				}

				CSTestKit_iShowOneTestCase(i+1, psTC, nLevel, OUTPUT_TO_FILE);
			}
            
			CSASSERT(psHelper->psTestObj != CSUDI_NULL && psHelper->psTestObj->fnShowResult != CSUDI_NULL);

            psHelper->psTestObj->fnShowResult(nLevel, OUTPUT_TO_PRINT, CSUDI_FALSE);
            psHelper->psTestObj->fnShowResult(nLevel, OUTPUT_TO_FILE, CSUDI_FALSE);
            
            for( i = nBeginNum-1; i < nEndNum; ++i )
            {            	
                CSTestCase_S* psTC = psHelper->psTestCase+i;
				CSTestKit_iShowOneTestCase( i+1, psTC, nLevel+1, OUTPUT_TO_PRINT );
            }
            CSTKPrint("=========================================\n");
	        CSTKPrint(CSITK_PROMPT);
            CSTKP_Flush();
            
			CSASSERT(psHelper->psInit && psHelper->psInit->fnUnInit != CSUDI_NULL);
			if(psHelper->psInit->fnUnInit != CSUDI_NULL)
			{
				psHelper->psInit->fnUnInit();
			}
		}
    }
}

void CSTKSelecteModules( CSTestKitHelper_S* psHelper, int nLevel )
{
	char cTail = 0;
    CSUDI_BOOL bRunTestCaseFlag = CSUDI_TRUE;
    CSUDI_BOOL bRunEndNumFlag = CSUDI_FALSE;
    
    while( 1 )
    {
        if( psHelper->nTestGetObjCount <= 1)
	    {
			CSTKPrint("----------------------------------------------------------\n");
	        CSTKPrint( " >>> Sorry! you should selcet 'c' to choose cases here.  \n");
            CSTKPrint("----------------------------------------------------------\n");
            CSTKPrint(CSITK_PROMPT);

            bRunTestCaseFlag = CSUDI_FALSE;
	        break;
	    }
        
        CSTCPrint( "Please Input Start Module's Number(q - Quit): " );
        
        memset( s_szCharNuffer, 0, sizeof(s_szCharNuffer) );
        CSTKP_Gets( s_szCharNuffer, sizeof(s_szCharNuffer) );
        
        if( !strncmp(s_szCharNuffer, "q", 1) || !strncmp(s_szCharNuffer, "Q", 1) )
        {
            CSTKPopMenu( psHelper );
            bRunTestCaseFlag = CSUDI_FALSE;
            break;
        }
        
        CSTestKit_iConvStringToInt( s_szCharNuffer, &s_nStartModule, &cTail );
            
        if( s_nStartModule > psHelper->nTestGetObjCount || s_nStartModule <= 0 )
        {
            CSTCPrint( "The Number Is Invalid, Input again!\n" );
            continue;               
        }
        else
        {
        	bRunEndNumFlag = CSUDI_TRUE;
			break;
        }
    }
    
    while( bRunEndNumFlag )
    {
        CSTCPrint( "Please Input The Last Module's Number(e - to the end, q - Quit): " );
        
        memset( s_szCharNuffer, 0, sizeof(s_szCharNuffer) );
        CSTKP_Gets(s_szCharNuffer, sizeof(s_szCharNuffer));
        
        if( !strncmp(s_szCharNuffer, "q", 1) || !strncmp(s_szCharNuffer, "Q", 1) )
        {
            CSTKPopMenu( psHelper );
            bRunTestCaseFlag = CSUDI_FALSE;
            break;
        }
        else if( !strncmp(s_szCharNuffer, "e", 1) || !strncmp(s_szCharNuffer, "E", 1) )
        {
            break;
        }
        else 
        {    
            CSTestKit_iConvStringToInt( s_szCharNuffer, &s_nLastModule, &cTail );

            if( s_nLastModule > psHelper->nTestGetObjCount || s_nLastModule <= 0 )
            {
                CSTCPrint( "The Number Is Invalid, Input again!\n" );
                continue;               
            }

            CSTKWaitAnyKey();
            break;
        }
    } 

    if( bRunTestCaseFlag )
    {
		CSTestObj_S *psObj = (psHelper->pfnTestGetObj)[0]();

		s_bSelectModuleFlag = CSUDI_TRUE;
    	s_bFirtstRunFlag = CSUDI_TRUE;
                    
		CSTestKit_iClearDes();

		psObj->fnRun(0);
        
		psObj->fnShowResult(0, OUTPUT_TO_PRINT, CSUDI_TRUE);
		
		CSTKPrint(CSITK_PROMPT);
        
		CSTKP_Flush();
        
		s_nMoudleCount = 0;
        s_bSelectModuleFlag = CSUDI_FALSE;
	}
}

/*-------�����ļ���ȡ����-----------*/

static char s_szbuffer[64*1024];   			/*����ļ�����*/
static char  acReadFile[1024] = {0}; 		/*��ŵ�ǰ�Ѿ���ȡ�ļ����ļ���*/
static int 	i_buffer_len=0;                 	/*����ļ���С*/

/********************************************************************
  Function:       GetIniString     		// ��������
  Description:    ��ȡ�ַ�������   		// �������ܡ����ܵȵ�����
  Calls:                           		// �����������õĺ����嵥
  Input:                           		// �����������˵��
    char *s_section : Section Name  	// ����
    char *s_key: Identity Name    		// �ؼ�����
  Output:                          		// �����������˵��
    char *s_result: Returned string 	// ����ֵ
  Return:                          		// ��������ֵ��˵��
             CS_TK_CONFIG_SUCCESS -- ��ȡ��Ӧֵ�ɹ�
             CS_TK_CONFIG_FAILURE -- ��ȡ��Ӧֵʧ��
  Others:                          		// ����˵��
**********************************************************************/
static int get_config_string(const char *s_section,const char *s_key, char *s_result,int n_size)
{
    int i=0;                    /*���ڿ��ƶ�ȡ�ַ�*/
    int j=0;                    /*�����ҵ�ֵ���ָ�븳ֵ*/
    int min;                    /*���ڴ�Ŷ�ȡֵ��λ��*/
    int i_key_found=-1;         /*�Ƿ��ҵ������ı�־*/
    
    /*���������ؼ��ֻ��ļ�Ϊ��,����-1*/
    if (!strlen(s_section) || !strlen(s_key))
    {
        return CS_TK_CONFIG_FAILURE; 
    }

    while ( i < i_buffer_len ) 
    { 
        /*��ȡ��ͷΪ' '��'\t')�ж���λ,ָ������ƶ�iλ*/
        while ((i<i_buffer_len) && ((s_szbuffer[i]==' ') || (s_szbuffer[i]=='\t')))
        {
            i++; 
        }

        /*���i�������ļ��ܳ���,�򷵻�-1*/
        if (i>=i_buffer_len) 
        {
            return CS_TK_CONFIG_FAILURE; 
        }

        if ( '#' == s_szbuffer[i]) 
        {    /*������'#'��ͷ�����,ֱ��\n*/
            while ((i<i_buffer_len) && (s_szbuffer[i] != '\n')&&(s_szbuffer[i]!='\r')) 
            {
                i++; 
            }

            if (i>=i_buffer_len) 
            {
                return CS_TK_CONFIG_FAILURE; 
            }

            /*�����\n,��������һ��*/
            i++; 
        } 
        else 
        { 
            if ('[' == s_szbuffer[i]) 
            { 
                i++; 
                /*���[]��ǰ�����''��\t��������*/
                while ((i<i_buffer_len) &&((s_szbuffer[i]==' ') || (s_szbuffer[i]=='\t'))) 
                {
                    i++; 
                }

                if (i>=i_buffer_len) 
                {
                    return CS_TK_CONFIG_FAILURE; 
                }

                if (0 == strncmp(s_szbuffer+i, s_section, strlen(s_section))) 
                { 
                    /*���ļ����ݵ�iλ��ʼ������Ķ����Ƚ�,�����ͬ*/
                    i += strlen(s_section); 
                    /*���[]�к������''��\t��������*/
                    while ((i<i_buffer_len) && ((s_szbuffer[i]==' ') || (s_szbuffer[i]=='\t'))) 
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len)
                    {
                        return CS_TK_CONFIG_FAILURE; 
                    }

                    /*�����ȡ����']',��ı�i_key_found=0*/
                    if (']' == s_szbuffer[i]) 
                    {    
                        i_key_found=0; 
                    }

                    i++;
                    
                    /*�ƶ�ָ��ֱ����ȡ��'\n'*/
                    while ((i<i_buffer_len) && (s_szbuffer[i]!='\n')&&(s_szbuffer[i]!='\r'))
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len) 
                    {
                        return CS_TK_CONFIG_FAILURE; 
                    }
                    
                    /*����һ��*/
                    i++; 
                } 
                else 
                {   /*���û���ҵ��Ͷ���ƥ��,��һֱ�ƶ�ָ�뵽'\n'*/
                    while ((i<i_buffer_len) && (s_szbuffer[i]!='\n')&&(s_szbuffer[i]!='\r')) 
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len) 
                    {
                        return CS_TK_CONFIG_FAILURE; 
                    }

                    /*����һ��*/
                    i++; 
                } 
            } 
            else 
            { 
                if (0 != i_key_found) 
                {   
                    /*���i_key_foundΪ0��˵��û���ҵ�ƥ��Ķ���,�������Ƶ�'\n'*/
                    while ((i<i_buffer_len) && (s_szbuffer[i] != '\n')&&(s_szbuffer[i]!='\r')) 
                    {
                        i++; 
                    }

                    if (i>=i_buffer_len) 
                    {    
                        return CS_TK_CONFIG_FAILURE; 
                    }

                    /*����һ��*/
                    i++; 
                } 
                else 
                {
                    if (0 == strncmp(s_szbuffer+i, s_key, strlen(s_key)))
                    { 
                        i += strlen(s_key); 
                        if (i>=i_buffer_len)
                        {
                            return CS_TK_CONFIG_FAILURE; 
                        }
                        while ((i<i_buffer_len) && ((s_szbuffer[i]=='\t') || (s_szbuffer[i]==' '))) 
                        {
                            i++; 
                        }

                        if ( '=' == s_szbuffer[i]) 
                        {   /*��ȡ=��˵���ҵ��˹ؼ�����*/
                            i++; 

                            /*��Ҫ��ȡ��ֵǰ���''��'\t'�����˵�*/
                            while ((i<i_buffer_len) && 
                                   ((s_szbuffer[i]=='\t') || 
                                   (s_szbuffer[i]==' '))) 
                            {
                                i++; 
                            }

                            if (i>=i_buffer_len) 
                            {
                                return CS_TK_CONFIG_FAILURE; 
                            }

                            j=i; 
                            /*��һָ�벻Ϊ'\n'���Ҳ�Ϊ'#',�����¶�ȡ*/
                            while ((j<i_buffer_len) && 
                                   (s_szbuffer[j]!='\n')&&
                                   (s_szbuffer[j]!='\r')&& 
                                   (s_szbuffer[j]!='#'))
                            {
                                j++; 
                            }

                            j--; 
                            while ((s_szbuffer[j]==' ') || 
                                   (s_szbuffer[j]=='\t')) 
                            {
                                 j--; 
                            }
                            /*�����ȡ����ֵ���ܳ���*/
                            min = j-i+1; 
                            /*�����copy��s_result��,����ĩβ��'\0',����0,�ɹ�*/

                            min = min < (n_size-1) ? min : (n_size-1);
                            strncpy(s_result, s_szbuffer+i, min); 
                            *(s_result+min) = '\0'; 
                            return CS_TK_CONFIG_SUCCESS; 
                        } 
                        else 
                        {   /*���û���ҵ�=��������ָ��*/
                            while ((i<i_buffer_len) && (s_szbuffer[i]!='\n')&&(s_szbuffer[i]!='\r')) 
                            {
                               i++; 
                            }

                            if (i>=i_buffer_len) 
                            {
                               return CS_TK_CONFIG_FAILURE; 
                            }

                            /*����һ��*/
                            i++; 
                        }    /*û���ҵ�'='��*/ 
                    }
                    else 
                    {    /*û���ҵ��ؼ���,������ָ��*/
                        while ((i<i_buffer_len) && (s_szbuffer[i]!='\n')&& (s_szbuffer[i]!='\r'))
                        {
                            i++; 
                        }
                        
                        if (i>=i_buffer_len||s_szbuffer[i+1] == '[') /*���������һ���ο�ʼ�ĵط���û���ҵ��ؼ��֣�����û�ҵ�*/
                        {
                            return CS_TK_CONFIG_FAILURE; 
                        }

                        /*�Ƶ���һ��*/
                        i++; 
                    } 
            	}
            } 
        } 
    } /* while (i<i_buffer_len)  */
    /*���û��return0��˵���ҵ�,����-1*/
    return CS_TK_CONFIG_FAILURE; 
}

CSTKCfgError_E CSTKGetConfigInfo(const char *pSection,const char *pKey, char *pResult,int nResultSize)
{
	int ret,nFileLen = 0;
	if(strncmp(TEST_CFG_FILE,acReadFile,sizeof(acReadFile)) != 0)
	{
	    CSUDI_HANDLE fp=CSUDI_NULL;

		
		 CSTCPrint("Open file: %s \n",TEST_CFG_FILE);
	    if ( CSUDI_NULL == (fp=CSTKP_FOpen(TEST_CFG_FILE, "rb")) ) 
	    {
	        CSTCPrint("�����ļ������ڻ���ļ�����!\n");
	        return CS_TK_CONFIG_CTROFILE_ERROR; 
	    }

		
		if(CSTKP_FSeek(fp,0,SEEK_END)==0)
		{
			nFileLen = CSTKP_FTell(fp);
			CSTKP_FSeek(fp,0,SEEK_SET);
		}
		
		CSASSERT(sizeof(s_szbuffer) >= nFileLen);
		if(sizeof(s_szbuffer) < nFileLen)
		{
	        CSTCPrint("�����ļ�̫��!\n");
	        return CS_TK_CONFIG_CTROFILE_ERROR; 
		}
	    
		memset(s_szbuffer,0,sizeof(s_szbuffer));
		
	    if ( 1 != CSTKP_FRead(s_szbuffer,nFileLen,1,fp))//read all file as a "struct",and nmemb = 1
	    {
	    	CSTCPrint("�����ļ���ȡʧ��!\n");
	        if (fp != CSUDI_NULL) 
	        {
	            CSTKP_FClose(fp); 
	        }
	        return CS_TK_CONFIG_READBUFFER_ERROR; 
	    }
	    
	    if (fp != CSUDI_NULL)
	    {
	        CSTKP_FClose(fp);
	    }
	    
	    i_buffer_len = nFileLen;
		
		strncpy(acReadFile,TEST_CFG_FILE,sizeof(acReadFile)-1);
		acReadFile[sizeof(acReadFile)-1] =  '\0';
	}
	
    /*�����ַ���*/
    ret = get_config_string(pSection,pKey,pResult,nResultSize);
	if (CS_TK_CONFIG_SUCCESS != ret)
    {        
        CSTCPrint("�����ļ��е�[%s]��û���ҵ�%s��ֵ.\n",pSection,pKey);
        return CS_TK_CONFIG_FAILURE;
    }
    return CS_TK_CONFIG_SUCCESS;
}

int CSTKGetConfigInfoFromFile(const char * pFile,const char *pSection,const char *pKey, char *pResult,int nResultSize)
{
	int ret,nFileLen = 0;
	CSTKPrint("%s enter!\n",__FUNCTION__);
	if(strncmp(pFile,acReadFile,sizeof(acReadFile)) != 0)
	{
	    CSUDI_HANDLE fp=CSUDI_NULL;


	    if ( CSUDI_NULL == (fp=CSTKP_FOpen(pFile, "rb")) ) 
	    {
	        CSTKPrint("�����ļ������ڻ���ļ�����!\n");
	        return CS_TK_CONFIG_CTROFILE_ERROR; 
	    }
	    
		if(CSTKP_FSeek(fp,0,SEEK_END)==0)
		{
			nFileLen = CSTKP_FTell(fp);
			CSTKP_FSeek(fp,0,CSTK_SEEK_SET);
		}
		
		CSASSERT(sizeof(s_szbuffer) >= nFileLen);
		if(sizeof(s_szbuffer) < nFileLen)
		{
	        CSTKPrint("�����ļ�̫��!\n");
	        return CS_TK_CONFIG_CTROFILE_ERROR; 
		}
	    
		memset(s_szbuffer,0,sizeof(s_szbuffer));
		
	    if ( 1 != CSTKP_FRead(s_szbuffer,nFileLen,1,fp))//read all file as a "struct",and nmemb = 1
	    {
	        if (fp != CSUDI_NULL) 
	        {
	            CSTKP_FClose(fp); 
	        }
	        return CS_TK_CONFIG_READBUFFER_ERROR; 
	    }
	    
	    if (fp != CSUDI_NULL)
	    {
	        CSTKP_FClose(fp);
	    }
	    
	    i_buffer_len = nFileLen; 
		
		strncpy(acReadFile,pFile,sizeof(acReadFile)-1);
		acReadFile[sizeof(acReadFile)-1] =  '\0';
	}
	//CSTKPrint("[%s] s_szbuffer = %s nFileLen = %d\n",__FUNCTION__,s_szbuffer,nFileLen);
    /*�����ַ���*/
    ret = get_config_string(pSection,pKey,pResult,nResultSize);
	if (CS_TK_CONFIG_SUCCESS != ret)
    {        
        CSTKPrint("�����ļ��е�[%s]��û���ҵ�%s��ֵ.\n",pSection,pKey);
        return CS_TK_CONFIG_FAILURE;
    }
    CSTKPrint("%s SUCCESS!\n",__FUNCTION__);
	return CS_TK_CONFIG_SUCCESS;
}

/********************************************************************
  Function:       CSTKGetIntFromStr	     			// ��������
  Description:    ת���������ַ���Ϊ������			// �������ܡ����ܵȵ�����
  Calls:                           					// �����������õĺ����嵥
  Input:                           					// �����������˵��
    char * SourceStr	��ת���ַ���			    // 
    int base            ���ƣ���10,16
  Return:      			ת�����������		    	// ��������ֵ��˵��
             
  Others:                          					// ����˵��
**********************************************************************/
int CSTKGetIntFromStr(char * SourceStr,int base)
{	
	return strtoul(SourceStr,CSUDI_NULL,base);
}

CSUDI_BOOL CSTKGetString(char *s,int* pnVal)
{
	char ctail;
	
	CSTKP_Gets(s,sizeof(s));

	CSTestKit_iLower(s);
	
	return CSTestKit_iConvStringToInt(s, pnVal, &ctail);
}


///< �ַ����ָ��ӿڣ��ݲ��ṩ
int CSTKGetStrFromStr(char ** SourceStr,char Symble,int nlen,char *sOut)
{	
	int i;
	
	for(i=0; i<nlen; i++)
	{
		if(*SourceStr[0]!=Symble)
		{			
			sOut[i] = *SourceStr[0];
			(*SourceStr)++;
		}
		else
		{
			(*SourceStr)++;
			sOut[i] = '\0';
			return 0;
		}		
	}
	CSTKPrint("�����lenֵ̫С�˻����Ѿ�û������Ҫ�Ķ�����.\n");

	return -1;
}


//�ļ�ģʽ
HCSHANDLE CSTKFOpen(const char* filename,const char* mode)
{
#if (FILE_MODE == FILE_STDIO_MODE)
	char fileBase[256] = "testdata/";
	if(filename[0] == '/')
	{
		return (HCSHANDLE)fopen(filename,mode);		
	}
	else
	{
		strcat( fileBase,  filename);
		return (HCSHANDLE)fopen(fileBase,mode);
	}
#endif
}

int CSTKFClose(HCSHANDLE hFile)
{
#if (FILE_MODE == FILE_STDIO_MODE)
    return fclose((FILE*)hFile);
#endif
}

int CSTKFRead(void * ptr,int size,int nmemb,HCSHANDLE hFile)
{
#if (FILE_MODE == FILE_STDIO_MODE)
    return (int)fread(ptr,(size_t)size,(size_t)nmemb,(FILE*)hFile);
#endif
}

int CSTKFWrite(const void * ptr,int size,int nmemb,HCSHANDLE hFile)
{
#if (FILE_MODE == FILE_STDIO_MODE)
    return (int)fwrite(ptr,(size_t)size,(size_t)nmemb,(FILE*)hFile);
#endif
}

int CSTKFFlush(HCSHANDLE hFile)
{
#if (FILE_MODE == FILE_STDIO_MODE)
    return fflush((FILE*)hFile);
#endif
}

int CSTKFSeek(HCSHANDLE hFile,long int offset,int whence)
{
#if (FILE_MODE == FILE_STDIO_MODE)
    int fwh = whence;
    if(whence == CSTK_SEEK_CUR)
    {
        fwh = SEEK_CUR;
    }
    else if(whence == CSTK_SEEK_SET)
    {
        fwh = SEEK_SET;
    }
    else if(whence == CSTK_SEEK_END)
    {
        fwh = SEEK_END;
    }

    return fseek((FILE*)hFile,offset,fwh);
#endif
}

long int CSTKFTell(HCSHANDLE hFile)
{
#if (FILE_MODE == FILE_STDIO_MODE)
    return ftell((FILE*)hFile);
#endif
}



	
/*-----------------Assert����------------------*/
CSUDI_BOOL CSTKAssertImp(CSUDI_BOOL bValue,const char* pDescript)
{
	if(!bValue)
	{
		CSTKPrint("Assert %s\n",pDescript);
		if(s_nAssertDesIndex < ASSERT_DES_COUNT)
		{
			s_apAssertDes[s_nAssertDesIndex++] = pDescript;
		}
	}

	return bValue;
}

