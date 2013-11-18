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

#define CSTTPrint CSTCPrint    ///< 兼容定义，之前大量用例均使用CSTTPrint

enum
{
	EM_AUTO_ANWSER_MASK		= 	1 << 0,		///< 自动回答模式
	EM_AUTO_NO_MANUL_MASK	=   1 << 1,		///< 不测试手动用例模式

	EM_AUTO_MODE_MAX
};

typedef enum
{
	CS_TK_CONFIG_SUCCESS,					///< 读取配置文件成功
	CS_TK_CONFIG_FAILURE,					///< 读取配置文件失败
	CS_TK_CONFIG_CTROFILE_ERROR,			///< 配置文件错误
	CS_TK_CONFIG_READBUFFER_ERROR,			///< 文件读取失败

	CS_TK_CONFIG_MAX
}CSTKCfgError_E;

///< 参见CSTestCase_S
#define CS_TK_AUTOCASE(tc) 		#tc, tc, EM_TK_NOT_TEST,EM_TK_AUTO,  -1,0	///< 自动或半自动测试，一般情况下这些用例都会运行
#define CS_TK_MANUALCASE(tc) 	#tc, tc, EM_TK_NOT_TEST,EM_TK_MANUAL,-1,0	///< 手动测试，一般来说为破坏性用例，需特别注意若用例只需需要人机交互，请指定为自动用例
#define CS_TK_ENDCASE			{"", NULL,EM_TK_NOT_TEST,EM_TK_AUTO,  -1,0}	///< 用例结束标志
#define CS_TESTCASE				CS_TK_AUTOCASE  ///<兼容定义，之前的用例均使用CS_TESTCASE

/**
@brief 测试用例失败跳入点定义
若测试用例因为以_FATAL结尾的断言如CSTK_ASSERT_TRUE_FATAL失败，会自动跳转到此位置，执行测试用例收尾工作，如环境还原，释放资源等，但此位置之后的断言除外
若需要在测试用例中使用以_FATAL结尾的断言，则必须使用该定义，否则会编译出错
所有断言只允许在测试函数线程中运行，不得在回调、线程等中使用
*/
#define CSTK_FATAL_POINT	CSTK_FATAL_POINT_IMP

/**
@brief 测试用例失败断言
断言value为真，否则说明由于decript的原因，本用例失败了。
推荐将关键执行代码直接写在value中
此断言必须配合CSTK_FATAL_POINT使用
若断言失败，则用例会跳转到CSTK_FATAL_POINT处进行资源回收等善后工作（在CSTK_FATAL_POINT后发生了此类型断言则不会跳转）
并且哪怕测试用例返回真，仍算执行失败，也就是说测试用例可以以如下方式写：
@code
CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == TestFunction1(params1),"步骤n失败"）;
CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == TestFunction2(params2),"步骤n+1失败"）;
CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == TestFunction3(params3),"步骤n+2失败"）;
CSTK_FATAL_POINT
CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == DestroyFunction(params),"销毁资源失败"）;
return CSUDI_TRUE;
@codeend
@note 测试框架仅存储指针，用例需保证字符串一直有效，推荐使用静态字符串(如参考代码所示)
所有断言只允许在测试函数线程中运行，不得在回调、线程等中使用
*/
#define CSTK_ASSERT_TRUE_FATAL(value,descript)	CSTK_ASSERT_TRUE_IMP(value,descript,CSUDI_TRUE)

///< 类似CSTK_ASSERT_TRUE_FATAL，但失败后不跳转到CSTK_FATAL_POINT，但仍需要定义CSTK_FATAL_POINT
///< 所有断言只允许在测试函数线程中运行，不得在回调、线程等中使用
#define CSTK_ASSERT_TRUE(value,descript)	CSTK_ASSERT_TRUE_IMP(value,descript,CSUDI_FALSE)

///< 测试用例用等待用户输入任何键接口，必须使用该接口进行等待用户确认，不得自行实现，在自动回答模式下，该接口会马上返回
void CSTKWaitAnyKey(void);

///< 测试用例用等待用户确认是非选择接口，必须用该接口进行用户是非选择，不得自行实现，在自动回答模式下，该接口会马上返回否定回答
CSUDI_BOOL CSTKWaitYes(void);

///< 测试用例打印接口，所有测试用例必须用该接口进行打印
void CSTCPrint(char *format, ...);

/**
@brief 从配置文件中读取信息接口

其中配置文件的格式为:
@code
#这是一样注释
[Section]
key=Result	#这也是一行注释
@codeend
所有格式为字符串形式
*/
CSTKCfgError_E CSTKGetConfigInfo(const char *pSection,const char *pKey, char *pResult,int nResultSize);

/********************************************************************
  Function:       CSTKGetConfigInfoFromFile	     			// 函数名称
  Description:    从指定文件中读取配置信息			// 函数功能、性能等的描述
  Calls:                           					// 被本函数调用的函数清单
  Input:											// 对输入参数的说明
  pFile				配置文件的名字
  pSection			配置文件中的段名
  pKey				配置项的key值
  pResult			配置项的vallue值,空间由用户申请
  nResultSize		配置项value值允许的最大长度
  Return:      		读到成功返回CS_TK_CONFIG_SUCCESS		    	// 函数返回值的说明
  					失败返回其他错误码           
  Others:                          					// 其它说明
**********************************************************************/

int CSTKGetConfigInfoFromFile(const char * pFile,const char *pSection,const char *pKey, char *pResult,int nResultSize);
///< 配置文件辅助函数，用于将字符串转换为整数
int CSTKGetIntFromStr(char * SourceStr,int base);


/*---------以下定义及接口仅用于生成测试用例，用例编写者无需特别关注------------*/
/**@cond */
typedef struct
{
	int nAll;			    /*所有测试用例的个数*/
	int nSuccess;		    /*所有测试用例通过的个数*/
	int nFailed;		    /*所有测试用例失败的个数*/
	int nAutoCase;          /*全自动测试用例的个数*/
	int nAutoSuccess;	    /*全自动测试用例通过的个数*/
	int nAutoFailed;	    /*全自动测试用例未通过的个数*/
	int nManulCase;         /*手动测试用例的个数*/
	int nManulSuccess;	    /*手动测试用例通过的个数*/
	int nManulFailed;	    /*手动测试用例未通过的个数*/
	int nSemiautoCase;      /*半自动测试用例个数*/
	int nSemiautoSuccess;   /*半自动测试用例的个数*/
	int nSemiautoFailed;	/*半自动测试用例通过的个数*/
}CSTestReport_S;

typedef CSUDI_BOOL(*CSTestCase_F)(void);	///< 测试用例原型
typedef void(*CSTestEntry_F)(void);			///< 模块入口原型
typedef void(*CSTestRun_F)(int nLevel);			///< 模块运行函数原型
typedef void(*CSTestSaveCase_F)(int nLevel);
typedef void(*CSTestGetReport_F)(CSTestReport_S* psReport);			///< 获取模块测试报告结构体原型
typedef void(*CSTestShowOut_F)(const char *szOut);					///< 模块生成测试报告原型
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
	EM_TK_AUTO,		///< 注意，自动，半自动会自动切换
	EM_TK_SEMIAUTO,	///< 注意，自动，半自动会自动切换
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


///< 测试启动模式
void CSTKSetAutoRun(CSTestKitHelper_S* psHelper);

//选择要测试的用例组
void CSTKSelecteCases(CSTestKitHelper_S* psHelper, int nLevel);

//选择要测试的模块组
void CSTKSelecteModules(CSTestKitHelper_S* psHelper, int nLevel);

CSUDI_BOOL CSTKGetString(char *s,int* pnVal);

//由于各平台读写实现可能不同(可能不支持fopen等),固在这里进行统一封装
//封装后不得直接调用fopen等函数，以下函数参数、返回值均与stdio中的一致
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
/*---------以上定义及接口仅用于生成测试用例，用例编写者无需特别关注------------*/


#ifdef __cplusplus
}
#endif

#endif

