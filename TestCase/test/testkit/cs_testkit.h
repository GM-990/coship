#ifndef _CS_ITEST_KIT_H_
#define _CS_ITEST_KIT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mmcp_typedef.h"
#include "cs_revision.h"
#include "version.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSTTPrint CSTCPrint    ///< ���ݶ��壬֮ǰ����������ʹ��CSTTPrint

enum
{
	EM_AUTO_ANWSER_MASK		= 	1 << 0,		///< �Զ��ش�ģʽ
	EM_AUTO_NO_MANUL_MASK	=   1 << 1,		///< �������ֶ�����ģʽ

	EM_AUTO_MODE_MAX
};

typedef enum
{
	CS_TK_CONFIG_SUCCESS,					///< ��ȡ�����ļ��ɹ�
	CS_TK_CONFIG_FAILURE,					///< ��ȡ�����ļ�ʧ��
	CS_TK_CONFIG_CTROFILE_ERROR,			///< �����ļ�����
	CS_TK_CONFIG_READBUFFER_ERROR,			///< �ļ���ȡʧ��

	CS_TK_CONFIG_MAX
}CSTKCfgError_E;

///< �μ�CSTestCase_S
#define CS_TK_AUTOCASE(tc) 		#tc, tc, EM_TK_NOT_TEST,EM_TK_AUTO,  -1,0	///< �Զ�����Զ����ԣ�һ���������Щ������������
#define CS_TK_MANUALCASE(tc) 	#tc, tc, EM_TK_NOT_TEST,EM_TK_MANUAL,-1,0	///< �ֶ����ԣ�һ����˵Ϊ�ƻ������������ر�ע��������ֻ����Ҫ�˻���������ָ��Ϊ�Զ�����
#define CS_TK_ENDCASE			{"", NULL,EM_TK_NOT_TEST,EM_TK_AUTO,  -1,0}	///< ����������־
#define CS_TESTCASE				CS_TK_AUTOCASE  ///<���ݶ��壬֮ǰ��������ʹ��CS_TESTCASE

/**
@brief ��������ʧ������㶨��
������������Ϊ��_FATAL��β�Ķ�����CSTK_ASSERT_TRUE_FATALʧ�ܣ����Զ���ת����λ�ã�ִ�в���������β�������绷����ԭ���ͷ���Դ�ȣ�����λ��֮��Ķ��Գ���
����Ҫ�ڲ���������ʹ����_FATAL��β�Ķ��ԣ������ʹ�øö��壬�����������
���ж���ֻ�����ڲ��Ժ����߳������У������ڻص����̵߳���ʹ��
*/
#define CSTK_FATAL_POINT	CSTK_FATAL_POINT_IMP

/**
@brief ��������ʧ�ܶ���
����valueΪ�棬����˵������decript��ԭ�򣬱�����ʧ���ˡ�
�Ƽ����ؼ�ִ�д���ֱ��д��value��
�˶��Ա������CSTK_FATAL_POINTʹ��
������ʧ�ܣ�����������ת��CSTK_FATAL_POINT��������Դ���յ��ƺ�������CSTK_FATAL_POINT�����˴����Ͷ����򲻻���ת��
�������²������������棬����ִ��ʧ�ܣ�Ҳ����˵�����������������·�ʽд��
@code
CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == TestFunction1(params1),"����nʧ��"��;
CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == TestFunction2(params2),"����n+1ʧ��"��;
CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == TestFunction3(params3),"����n+2ʧ��"��;
CSTK_FATAL_POINT
CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == DestroyFunction(params),"������Դʧ��"��;
return CSUDI_TRUE;
@codeend
@note ���Կ�ܽ��洢ָ�룬�����豣֤�ַ���һֱ��Ч���Ƽ�ʹ�þ�̬�ַ���(��ο�������ʾ)
���ж���ֻ�����ڲ��Ժ����߳������У������ڻص����̵߳���ʹ��
*/
#define CSTK_ASSERT_TRUE_FATAL(value,descript)	CSTK_ASSERT_TRUE_IMP(value,descript,CSUDI_TRUE)

///< ����CSTK_ASSERT_TRUE_FATAL����ʧ�ܺ���ת��CSTK_FATAL_POINT��������Ҫ����CSTK_FATAL_POINT
///< ���ж���ֻ�����ڲ��Ժ����߳������У������ڻص����̵߳���ʹ��
#define CSTK_ASSERT_TRUE(value,descript)	CSTK_ASSERT_TRUE_IMP(value,descript,CSUDI_FALSE)

///< ���������õȴ��û������κμ��ӿڣ�����ʹ�øýӿڽ��еȴ��û�ȷ�ϣ���������ʵ�֣����Զ��ش�ģʽ�£��ýӿڻ����Ϸ���
void CSTKWaitAnyKey(void);

///< ���������õȴ��û�ȷ���Ƿ�ѡ��ӿڣ������øýӿڽ����û��Ƿ�ѡ�񣬲�������ʵ�֣����Զ��ش�ģʽ�£��ýӿڻ����Ϸ��ط񶨻ش�
CSUDI_BOOL CSTKWaitYes(void);

///< ����������ӡ�ӿڣ����в������������øýӿڽ��д�ӡ
void CSTCPrint(char *format, ...);

/**
@brief �������ļ��ж�ȡ��Ϣ�ӿ�

���������ļ��ĸ�ʽΪ:
@code
#����һ��ע��
[Section]
key=Result	#��Ҳ��һ��ע��
@codeend
���и�ʽΪ�ַ�����ʽ
*/
CSTKCfgError_E CSTKGetConfigInfo(const char *pSection,const char *pKey, char *pResult,int nResultSize);

/********************************************************************
  Function:       CSTKGetConfigInfoFromFile	     			// ��������
  Description:    ��ָ���ļ��ж�ȡ������Ϣ			// �������ܡ����ܵȵ�����
  Calls:                           					// �����������õĺ����嵥
  Input:											// �����������˵��
  pFile				�����ļ�������
  pSection			�����ļ��еĶ���
  pKey				�������keyֵ
  pResult			�������vallueֵ,�ռ����û�����
  nResultSize		������valueֵ�������󳤶�
  Return:      		�����ɹ�����CS_TK_CONFIG_SUCCESS		    	// ��������ֵ��˵��
  					ʧ�ܷ�������������           
  Others:                          					// ����˵��
**********************************************************************/

int CSTKGetConfigInfoFromFile(const char * pFile,const char *pSection,const char *pKey, char *pResult,int nResultSize);
///< �����ļ��������������ڽ��ַ���ת��Ϊ����
int CSTKGetIntFromStr(char * SourceStr,int base);


/*---------���¶��弰�ӿڽ��������ɲ���������������д�������ر��ע------------*/
/**@cond */
typedef struct
{
	int nAll;			    /*���в��������ĸ���*/
	int nSuccess;		    /*���в�������ͨ���ĸ���*/
	int nFailed;		    /*���в�������ʧ�ܵĸ���*/
	int nAutoCase;          /*ȫ�Զ����������ĸ���*/
	int nAutoSuccess;	    /*ȫ�Զ���������ͨ���ĸ���*/
	int nAutoFailed;	    /*ȫ�Զ���������δͨ���ĸ���*/
	int nManulCase;         /*�ֶ����������ĸ���*/
	int nManulSuccess;	    /*�ֶ���������ͨ���ĸ���*/
	int nManulFailed;	    /*�ֶ���������δͨ���ĸ���*/
	int nSemiautoCase;      /*���Զ�������������*/
	int nSemiautoSuccess;   /*���Զ����������ĸ���*/
	int nSemiautoFailed;	/*���Զ���������ͨ���ĸ���*/
}CSTestReport_S;

typedef CSUDI_BOOL(*CSTestCase_F)(void);	///< ��������ԭ��
typedef void(*CSTestEntry_F)(void);			///< ģ�����ԭ��
typedef void(*CSTestRun_F)(int nLevel);			///< ģ�����к���ԭ��
typedef void(*CSTestSaveCase_F)(int nLevel);
typedef void(*CSTestGetReport_F)(CSTestReport_S* psReport);			///< ��ȡģ����Ա���ṹ��ԭ��
typedef void(*CSTestShowOut_F)(const char *szOut);					///< ģ�����ɲ��Ա���ԭ��
typedef void(*CSTestShowResult_F)(int nLevel,int nMode,CSUDI_BOOL bWithDetails);
typedef int(*CSTestGetVersion_F)(char* pcVer, int nSize);

typedef struct _CSTestObj_S
{
	const char *szName;
	CSTestEntry_F fnEntry;
	CSTestRun_F fnRun;
	CSTestSaveCase_F fnSaveCases;
	CSTestGetReport_F fnGetReport;
	CSTestShowResult_F fnShowResult;
}CSTestObj_S;

typedef CSTestObj_S*(*CSTestGetObj_F)(void);


typedef enum
{
	EM_TK_NOT_TEST = 0,
	EM_TK_SUCCESS,
	EM_TK_FAILED
}CSTestResult_E;

typedef enum
{
	EM_TK_AUTO,		///< ע�⣬�Զ������Զ����Զ��л�
	EM_TK_SEMIAUTO,	///< ע�⣬�Զ������Զ����Զ��л�
	EM_TK_MANUAL
}CSTestCaseType_E;

typedef struct
{
	const char *szName;
	CSTestCase_F fnTestCase;
	CSTestResult_E eResult;
	CSTestCaseType_E eCaseType;
	int nDesStart;
	int nDesCnt;
}CSTestCase_S;

typedef struct _CSTestInit_S
{
	CSTestCase_F	 fnInit;
	CSTestCase_F	 fnUnInit;
}CSTestInit_S;

typedef struct
{
	CSTestGetObj_F *pfnTestGetObj;
	int nTestGetObjCount;
	CSTestCase_S *psTestCase;
	int nTestCount;
	CSTestObj_S *psTestObj;
	CSTestReport_S *psTestReport;
	CSTestInit_S *psInit;
}CSTestKitHelper_S;

#define CS_TEST_BUILER_HELPER(h) \
	CSTestKitHelper_S h; \
	h.pfnTestGetObj = s_afnTestGetObj; \
	h.nTestGetObjCount = sizeof(s_afnTestGetObj) / sizeof(CSTestGetObj_F); \
	h.psTestCase = s_asTestCase; \
	h.nTestCount = sizeof(s_asTestCase) / sizeof(CSTestCase_S) - 1; \
	h.psTestObj = &s_sTestObj; \
	h.psTestReport = &s_sTestReport; \
	h.psInit = &s_sTestInit;

#define CS_TEST_BUILD_FRAME_CODE(mod)  \
	static void CS##mod##TEST_iEntry(void); \
	static void CS##mod##TEST_iRun(int nLevel); \
	static void CS##mod##TEST_iSaveCases(int nLevel); \
	static void CS##mod##TEST_iGetReport(CSTestReport_S* psReport);\
	static void CS##mod##TEST_iShowResult(int nLevel,int nMode,CSUDI_BOOL bWithDetails);\
	VERSION_FUNCTION(mod, "");\
	\
	static CSTestReport_S s_sTestReport = \
	{ \
		0, 0 ,0,0,0,0,0,0,0 \
	}; \
	\
	static CSTestObj_S s_sTestObj = \
	{\
		#mod,\
		CS##mod##TEST_iEntry,\
		CS##mod##TEST_iRun,\
		CS##mod##TEST_iSaveCases,\
		CS##mod##TEST_iGetReport,\
		CS##mod##TEST_iShowResult\
	};\
	\
	CSTestObj_S* CS##mod##TESTGetObj(void) \
	{ \
		return &s_sTestObj; \
	}\
	\
	void CS##mod##TESTEntry(void) \
	{ \
		CS##mod##TEST_iEntry(); \
	}\
	\
	static void CS##mod##TEST_iEntry(void) \
	{\
		CS_TEST_BUILER_HELPER(sHelper);	\
		CSTKEntry(&sHelper);\
	}\
	static void CS##mod##TEST_iRun(int nLevel)\
	{\
		CS_TEST_BUILER_HELPER(sHelper);\
		CSTKRun(&sHelper,nLevel);\
	}\
	\
	static void CS##mod##TEST_iSaveCases(int nLevel)\
	{\
		CS_TEST_BUILER_HELPER(sHelper);\
		CSTKSaveCases(&sHelper,nLevel);\
	}\
	\
	static void CS##mod##TEST_iGetReport(CSTestReport_S* psReport)\
	{\
		*psReport = s_sTestReport;\
	}\
	\
	static void CS##mod##TEST_iShowResult(int nLevel,int nMode,CSUDI_BOOL bWithDetails)\
	{\
		CS_TEST_BUILER_HELPER(sHelper);\
		CSTKShowResult(&sHelper, nLevel,nMode,bWithDetails);\
	}

void CSTKShowResult(CSTestKitHelper_S* psHelper, int nLevel,int nMode,CSUDI_BOOL bWithDetails);
void CSTKEntry(CSTestKitHelper_S* psHelper);
void CSTKSaveCases(CSTestKitHelper_S* psHelper,int nLevel);
void CSTKRun(CSTestKitHelper_S* psHelper,int nLevel);


///< ��������ģʽ
void CSTKSetAutoRun(CSTestKitHelper_S* psHelper);

//ѡ��Ҫ���Ե�������
void CSTKSelecteCases(CSTestKitHelper_S* psHelper, int nLevel);

//ѡ��Ҫ���Ե�ģ����
void CSTKSelecteModules(CSTestKitHelper_S* psHelper, int nLevel);

CSUDI_BOOL CSTKGetString(char *s,int* pnVal);

//���ڸ�ƽ̨��дʵ�ֿ��ܲ�ͬ(���ܲ�֧��fopen��),�����������ͳһ��װ
//��װ�󲻵�ֱ�ӵ���fopen�Ⱥ��������º�������������ֵ����stdio�е�һ��
#define CSTK_SEEK_SET   0
#define CSTK_SEEK_CUR   1
#define CSTK_SEEK_END   2

HCSHANDLE CSTKFOpen(const char* filename,const char* mode);
int CSTKFClose(HCSHANDLE hFile);
int CSTKFRead(void * ptr,int size,int nmemb,HCSHANDLE hFile);
int CSTKFWrite(const void * ptr,int size,int nmemb,HCSHANDLE hFile);
int CSTKFFlush(HCSHANDLE hFile);
int CSTKFSeek(HCSHANDLE hFIle,long int offset,int whence);
long int CSTKFTell(HCSHANDLE hFile);
extern CSUDI_BOOL g_bAssertBeforeFatalPoint;
CSUDI_BOOL CSTKAssertImp(CSUDI_BOOL bValue,const char* pDescript);

#define CSTK_FATAL_POINT_IMP	\
{\
	__TESTCASE_FATAL_OUT_POINT:\
		g_bAssertBeforeFatalPoint = CSUDI_FALSE;\
}

#define CSTK_ASSERT_TRUE_IMP(value,descript,fatal)		\
{\
		if (!CSTKAssertImp(value,"["#value"] Failed ["descript"]")	\
			&& fatal && g_bAssertBeforeFatalPoint)	\
		{	\
			goto __TESTCASE_FATAL_OUT_POINT;	\
		}	\
}

/**@endcond*/
/*---------���϶��弰�ӿڽ��������ɲ���������������д�������ر��ע------------*/


#ifdef __cplusplus
}
#endif

#endif

