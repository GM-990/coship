/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "udi2_error.h"
#include "udi2_demux.h"
#include "udi2_tuner.h"
#include "udi2_player.h"
#include "udi2_video.h"

#include "udi2_os.h"
#include "cs_testkit.h"
#include "cs_demuxtestcase.h"
#include "../cs_udi2testcase.h" 
#include "udi2_demux.h"

#include "cs_workermanager.h"


CSUDI_BOOL CSTC_DEMUX_Init(void)
{
	//在本测试用例集执行前调用
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_DEMUX_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIDEMUXGetCount 
//@DESCRIPTION: 测试函数CSUDIDEMUXGetCount在pnDemuxCount参数正确情况下的执行结果
//@PRECONDITION:None
//@INPUT:pnDemuxCount=整型指针
//@EXPECTATION:成功获取底层驱动存在的Demux资源数目，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount 获取Demux资源数目
//@EXECUTIONFLOW: 2、判断获取的Demux数目是否大于0
//@EXECUTIONFLOW: 3、得到Config中的Demux数目
//@EXECUTIONFLOW: 4、将得到的Demux数目与config中Demux比较，如果相等返回CSUDI_SUCCESS，否则测试失败
CSUDI_BOOL CSTC_DEMUX_IT_GetCount_0001(void)
{	
	char *pcSection = "DEMUX";
	char *pckey =  "DEMUX_NUMBER" ;
	char cResult[16];
	int  nResultSize = 16;
       int nDemuxCount = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey,cResult,nResultSize), "返回参数失败\n");		
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount == CSTKGetIntFromStr(cResult,10),"返回参数失败\n");
      	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
	

}

//@CASEGROUP:CSUDIDEMUXGetCount 
//@DESCRIPTION: 测试CSUDIDEMUXGetCount在pnDemuxCount参数为空情况下的执行结果
//@PRECONDITION:None
//@INPUT:1、pnDemuxCount=CSUDI_NULL
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、以pnDemuxCount为NULL，调用CSUDIDEMUXGetCount函数
//@EXECUTIONFLOW: 2、判断函数返回是否为非CSUDI_SUCCESS
//@REMARK:pnDemuxCount为NULL的情况下，系统不能出现死机，底层能正确分辨参数错误
CSUDI_BOOL CSTC_DEMUX_IT_GetCount_0002(void)
{
       CSUDI_BOOL bRet = CSUDI_FALSE;
	   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXGetCount(CSUDI_NULL),"步骤1失败\n");
       bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

static int GetConfigCapability(int nIndex,CSUDIDEMUXCapability_S *capability)
{
	char *pcSection = "DEMUX";
	char cResult[16];
	int  nResultSize = 16;
	char buffer[80];
	int nRet = -1;

      sprintf(buffer,"DEMUX%d_CONNECT_TUNER",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	capability->m_dwDMAConnectSupportTunersMask = CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_dwDMAConnectSupportTunersMask is %d\n",capability->m_dwDMAConnectSupportTunersMask);
	
	sprintf(buffer,"DEMUX%d_FilterNum",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	capability->m_nFilterNum = CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_nFilterNum is %d\n",capability->m_nFilterNum);
       
	sprintf(buffer,"DEMUX%d_keyNum",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	capability->m_nkeyNum = CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_nkeyNum is %d\n",capability->m_nkeyNum);
     
	sprintf(buffer,"DEMUX%d_WorkTypeMask",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	capability->m_dwWorkTypeMask= CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_dwWorkTypeMask is %ld\n",capability->m_dwWorkTypeMask);
      
	sprintf(buffer,"DEMUX%d_ConTypeMask",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize); 
	capability->m_dwConTypeMask = CSTKGetIntFromStr(cResult,10);
       //printf("capability->m_dwConTypeMask is %ld\n",capability->m_dwConTypeMask);
       
       CSTK_ASSERT_TRUE_FATAL((capability !=CSUDI_NULL),"参数获取错误!"); 
       nRet = 0;
	  
	CSTK_FATAL_POINT;

	return nRet;
}


//@CASEGROUP:CSUDIDEMUXGetCapability 
//@DESCRIPTION: 测试CSUDIDEMUXGetCapability在所有参数均正确的情况下，函数执行的结果，并对返回的能力进行正确性检查
//@PRECONDITION:Demux设备的数目不为0
//@INPUT:1、nDemuxIndex = [0..nDemux-1]
//@INPUT:2、psCapabilityInfo = CSUDIDEMUXCapability_S结构指针
//@EXPECTATION: 返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到Demux的数目nDemux.
//@EXECUTIONFLOW: 2、nDemuxIndex取值[0..nDemux-1]内各值，执行3-6
//@EXECUTIONFLOW: 3、调用CSUDIDEMUXGetCapability，获取第nDemuxIndex个Demux的能力并判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、获取config的中第nDemuxIndex个Demux的能力
//@EXECUTIONFLOW: 5、比较3和4所得到的能力，看是否相同
//@EXECUTIONFLOW: 6、如果不相同，则测试失败
//@REMARK:获取的Demux能力数据结构各字段请参考CSUDIDEMUXCapability_S的定义
CSUDI_BOOL CSTC_DEMUX_IT_GetCapability_0001(void)
{
       int nDemuxCount =0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int i =0;
       CSUDIDEMUXCapability_S capability;

       memset(&capability,0,sizeof(CSUDIDEMUXCapability_S));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"步骤1失败\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i = 0; i < nDemuxCount; i++)
	{     
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIDEMUXGetCapability(i,&sCapabilityInfo),"步骤3失败\n");
		GetConfigCapability(i,&capability);
		CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_nFilterNum== capability.m_nFilterNum, "比较错误!");
		CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_nkeyNum== capability.m_nkeyNum, "比较错误!");
		CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_dwWorkTypeMask== capability.m_dwWorkTypeMask, "比较错误!");
		CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_dwConTypeMask== capability.m_dwConTypeMask, "比较错误!");	
              CSTK_ASSERT_TRUE_FATAL(sCapabilityInfo.m_dwDMAConnectSupportTunersMask==capability.m_dwDMAConnectSupportTunersMask, "比较错误!");
       }	
       bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
	
}


//@CASEGROUP:CSUDIDEMUXGetCapability 
//@DESCRIPTION: 测试CSUDIDEMUXGetCapability在nDemuxIndex参数错误情况下的执行结果
//@PRECONDITION:Demux设备的数目不为0
//@INPUT:1、nDemuxIndex={-1,nDemux,nDemux+1}
//@INPUT:2、psCapabilityInfo = CSUDIDEMUXCapability_S结构指针
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到Demux的数目nDemux
//@EXECUTIONFLOW: 2、nDemuxIndex取值{-1,nDemux,nDemux+1}，分别调用CSUDIDEMUXGetCapability，判断返回值是否为非CSUDI_SUCCESS
//@REMARK:获取nDemuxIndex={-1,nDemux,nDemux+1}的各Demux的能力，CSUDIDEMUXGetCapability函数均应返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DEMUX_IT_GetCapability_0002(void)
{
	int nDemuxCount =0;
	CSUDIDEMUXCapability_S sCapabilityInfo;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"步骤1失败\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetCapability(-1,&sCapabilityInfo),"步骤2失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetCapability(nDemuxCount,&sCapabilityInfo),"步骤2失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetCapability(nDemuxCount+1,&sCapabilityInfo),"步骤2失败\n");

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}


//@CASEGROUP:CSUDIDEMUXGetCapability 
//@DESCRIPTION: 测试CSUDIDEMUXGetCapability在psCapabilityInfo参数为空情况下的执行结果
//@PRECONDITION:Demux设备的数目不为0
//@INPUT:1、nDemuxIndex =  [0..nDemux-1]
//@INPUT:2、psCapabilityInfo = CSUDI_NULL
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到Demux的数目nDemux
//@EXECUTIONFLOW: 2、获取[0..nDemux-1]内各Demux的能力，
//@EXECUTIONFLOW: 3、调用CSUDIDEMUXGetCapability，判断返回值是否为非CSUDI_SUCCESS
//@REMARK:如果psCapabilityInfo参数为空,则无论其它参数如何，CSUDIDEMUXGetCapability函数均返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DEMUX_IT_GetCapability_0003(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int  i =0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"步骤1失败\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i = 0; i < nDemuxCount; i++)
	{ 
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXGetCapability(i,CSUDI_NULL),"步骤3失败\n");
	}

	 bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;     	 
}

//@CASEGROUP:CSUDIDEMUXGetCapability 
//@DESCRIPTION: 测试CSUDIDEMUXGetCapability获取到的demux能力应该满足最小应用的要求
//@PRECONDITION:Demux设备的数目不为0
//@INPUT:1、nDemuxIndex =  [0..nDemux-1]
//@EXPECTATION: 1.能成功获取到每个demux的能力
//@EXPECTATION: 2.至少有一个demux满足下面的条件(即可以作为播放demux)
//@EXPECTATION: 2.1 具有EM_UDI_DEMUX_PLAY的能力
//@EXPECTATION: 2.2 具有EM_UDI_DEMUX_DMA的属性，即可以连接外部硬件tuner
//@EXPECTATION: 2.3 能和外部硬件tuner连接(即m_dwDMAConnectSupportTunersMask应该大于0)
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到Demux的数目nDemux
//@EXECUTIONFLOW: 2、获取[0..nDemux-1]内各Demux的能力，期望返回成功
//@EXECUTIONFLOW: 3、查找满足期望条件2的demux数量,期望至少存在1个
CSUDI_BOOL CSTC_DEMUX_IT_GetCapability_0004(void)
{
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	 int nDemuxCount =0;
	 int i = 0;
	 BOOL nFindRightDemux = FALSE;
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i=0;i<nDemuxCount;i++)
	{
		memset(&sCapabilityInfo,0,sizeof(sCapabilityInfo));
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "step 2 获取Demux能力失败!");
		if (sCapabilityInfo.m_dwWorkTypeMask&(EM_UDI_DEMUX_PLAY|EM_UDI_DEMUX_DMA))
		{
			if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask > 0)
			{
				CSTCPrint("find player demux:%d\n",i);
				nFindRightDemux = TRUE;
				break;
			}
		}
	}

	CSTK_FATAL_POINT;

	if (nFindRightDemux)
	{
		return CSUDI_TRUE;
	}

	return CSUDI_FALSE;
}

static int GetConfigTunerInfo(int nIndex, int *pnTunerInfo)
{
  	char *pcSection = "DEMUX";
	char cResult[16];
	char buffer[80];
	int nRet = -1;
	
	sprintf(buffer,"DEMUX%d_CONNECT_TUNER",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,sizeof(cResult));
	*pnTunerInfo = CSTKGetIntFromStr(cResult,10);
	/*有些注入demux是不支持连接tuner的,所以应该是大于等于0*/
       CSTK_ASSERT_TRUE_FATAL((*pnTunerInfo >=0),"参数获取错误!");
	nRet = 0;
	  
	CSTK_FATAL_POINT;

	return nRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: 测试Demux设备与Tuner设备能连接和断开连接的情况
//@PRECONDITION:存在Demux设备
//@PRECONDITION:存在Tuner设备
//@PRECONDITION:在config文件中已有Demux与Tuner的连接矩阵表
//@INPUT:1、nTunerIndex索引值= [0..nTuner-1]
//@INPUT:2、nDemuxIndex索引值= [0..nDemux-1]
//@EXPECTATION: 1个Demux与Tuner的连接矩阵中，具备连接能力的一对Demux与Tuner设备，一定能连接成功并能被正确断开。
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目
//@EXECUTIONFLOW: 2、通过config文件得到Demux与Tuner的连接矩阵表
//@EXECUTIONFLOW: 3、针对连接矩阵表中的每一对Demux与Tuner设备，如果这对设备具备连接能力，则执行4-5
//@EXECUTIONFLOW: 4、调用CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex)，判断能否正确返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXDisconnectTuner(nTunerIndex,nDemuxIndex)，判断能否正确返回CSUDI_SUCCESS
//@REMARK:配置表中具备连接能力的Demux与Tuner设备对，一定能正确连接(CSUDIDEMUXConnectTuner)和断开连接(CSUDIDEMUXDisconnectTuner)
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0001(void)
{
       int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i =0;
       int   nTunerInfo = 0;
	int   j = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i = 0; i < nDemuxCount; i++)
	{
             CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "获取Demux能力失败!");
             
	      if(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId)
	      {
                  GetConfigCapability(i,&sCapabilityInfo);
                  if(sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA	)
                  {
                        GetConfigTunerInfo(i,&nTunerInfo);
                        for (j=0; j< 16;j++)
                        {
                            int pos = 1 << j;
                            if (nTunerInfo & pos)
                            {
                                       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i),"步骤4失败!");	
                            	    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"步骤8 失败!");
                            }
                        }
                   }
	      }
	}

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"恢复默认连接失败");//恢复默认连接Modified by qLuo 2011-8-22

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: 测试Demux设备与Tuner设备不能连接的情况
//@PRECONDITION:存在Demux设备
//@PRECONDITION:存在Tuner设备
//@PRECONDITION:在config文件中已有Demux与Tuner的连接矩阵表
//@INPUT:1、nTunerIndex索引值= [0..nTuner-1]
//@INPUT:2、nDemuxIndex索引值= [0..nDemux-1]
//@EXPECTATION: 1个Demux与Tuner的连接矩阵中，不具备连接能力的一对Demux与Tuner设备，一定不能连接成功。
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目
//@EXECUTIONFLOW: 2、通过config文件得到Demux与Tuner的连接矩阵表
//@EXECUTIONFLOW: 3、针对连接矩阵表中的每一对Demux与Tuner设备，如果这对设备不具备能连接能力，则执行4
//@EXECUTIONFLOW: 4、调用CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex)，判断能否正确返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXDisConnectTuner(nTunerIndex,nDemuxIndex),恢复现场
//@REMARK:配置表中不具备连接能力的Demux与Tuner设备对，一定不能正确连接
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0002(void)
{
       int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int i =0;
       int   nTunerInfo = 0;
	int   j = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i = 0; i < nDemuxCount; i++)
	{
		GetConfigTunerInfo(i,&nTunerInfo);
		
		for (j=0; j< 16;j++)
		{
			int pos = 1 << j;
			if (!(nTunerInfo & pos))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXConnectTuner(j, i),"步骤4失败!");	
			}
		}
	}

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: 测试一个Demux设备同时最多只能与一个Tuner设备连接
//@PRECONDITION:存在Demux设备
//@PRECONDITION:存在不止一个Tuner设备
//@PRECONDITION:在config文件中已有Demux与Tuner的连接矩阵表
//@PRECONDITION:在Demux与Tuner的连接矩阵表中，存在某个Demux设备能够与多个Tuner连接的配置
//@INPUT:1、nTunerIndex索引值= [0..nTuner-1]
//@INPUT:2、nDemuxIndex索引值= [0..nDemux-1]
//@EXPECTATION: 一个Demux设备最多只能同时与一个Tuner设备连接，
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目
//@EXECUTIONFLOW: 2、通过config文件得到Demux与Tuner的连接矩阵表，选取一个nDemuxIndex，该Demux能与多个Tuner相连
//@EXECUTIONFLOW: 3、调用CSUDIDEMUXConnectTuner(nTunerIndex0,nDemuxIndex)，判断能否正确返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIDEMUXConnectTuner(nTunerIndex1,nDemuxIndex)，判断能否正确返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXDisConnectTuner(nTunerIndex0,nDemuxIndex)，判断能否正确返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIDEMUXConnectTuner(nTunerIndex1,nDemuxIndex)，判断能否正确返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIDEMUXDisConnectTuner(nTunerIndex1,nDemuxIndex)，恢复现场
//@REMARK:
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0003(void)
{
       int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i =0;
       int   nTunerInfo = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i = 0; i < nDemuxCount; i++)
	{
              CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "获取Demux能力失败!");
             
	       if(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId)
             {
                GetConfigCapability(i,&sCapabilityInfo);
                  if(sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA	)
    		    {
                         GetConfigTunerInfo(i,&nTunerInfo);
            		if (nTunerInfo == 3)
            		{		
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, i),"步骤3失败!");
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXConnectTuner(1, i),"步骤4失败!");	 
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXDisconnectTuner(0, i),"步骤5失败!");
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(1, i),"步骤6失败!");
            			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXDisconnectTuner(1, i),"步骤7失败!");
            		}
                  }
	       }
	}

      bRet = CSUDI_TRUE;
		 
      CSTK_FATAL_POINT
	  	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"恢复默认连接失败");//恢复默认连接Modified by qLuo 2011-8-22
	
	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: 测试一个Tuner设备同时能够与多个Demux连接
//@PRECONDITION:存在不止一个Demux设备
//@PRECONDITION:存在Tuner设备
//@PRECONDITION:在config文件中已有Demux与Tuner的连接矩阵表
//@PRECONDITION:在Demux与Tuner的连接矩阵表中，存在某个Tuner设备能够与多个Demux连接的配置
//@INPUT:1、nTunerIndex索引值= [0..nTuner-1]
//@INPUT:2、nDemuxIndex索引值= [0..nDemux-1]
//@EXPECTATION: 一个Demux设备最多只能同时与一个Tuner设备连接
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目
//@EXECUTIONFLOW: 2、通过config文件得到Demux与Tuner的连接矩阵表，选取一个nTunerIndex，该Tuner能与多个Demux相连
//@EXECUTIONFLOW: 3、调用CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex0)，判断能否正确返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex1)，判断能否正确返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXDisConnectTuner(nTunerIndex,nDemuxIndex0)，恢复现场
//@EXECUTIONFLOW: 6、调用CSUDIDEMUXDisConnectTuner(nTunerIndex,nDemuxIndex1)，恢复现场
//@REMARK:
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0004(void)
{
       int nDemuxCount =0;
	int nTunerInfo1=0;
	int nTunerInfo2=0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       CSUDIDEMUXCapability_S  sCapabilityInfo1;
       CSUDIDEMUXCapability_S  sCapabilityInfo2;
	int i =0;
	int j =0;
	int z =0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for(i = 1; i < nDemuxCount; i++)
	{    
              CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo2)), "获取Demux能力失败!");
             
	       if(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo2.m_dwDMAConnectedTunerId)
		{
                   for(j = 0; j < nDemuxCount; j++)
    		     {
                          GetConfigCapability(j,&sCapabilityInfo1);
                          GetConfigCapability(i,&sCapabilityInfo2);
                          if((sCapabilityInfo1.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA)&&(sCapabilityInfo2.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA))
                          {
                                 GetConfigTunerInfo(j,&nTunerInfo1);
        			     GetConfigTunerInfo(i,&nTunerInfo2);

        			     if ((nTunerInfo1 == nTunerInfo2)&&(i !=j))	
        			     {  
        			           for (z=0; z< 16;z++)
        			           {
        			               int pos = 1 << z;
        	                             if (nTunerInfo1& pos)
                		               {
                                           	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(z, j),"步骤3失败!");
                				 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(z, i),"步骤4失败!");	 
                				 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXDisconnectTuner(z, j),"步骤5失败!");
                				 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXDisconnectTuner(z, i),"步骤6失败!");	 
                		               }
        			           }
        			      }
                              else
                              {
                                   break; 
                              }
                          }
    		      }
	       }
	}
	
	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT
	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"恢复默认连接失败");//恢复默认连接Modified by qLuo 2011-8-22
	
	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: 测试非法的nDemuxIndex不能与任何Tuner设备(无论是合法的还是非法的nTunerIndex)相连接
//@INPUT:1、nTunerIndex索引值= [0..nTuner-1]
//@INPUT:2、nDemuxIndex索引值= {-1,nDemux,nDemux+1}
//@EXPECTATION: 非法的nDemuxIndex不能与任何Tuner设备连接
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目
//@EXECUTIONFLOW: 2、nTunerIndex取[0..nTuner-1]内的值
//@EXECUTIONFLOW: 3、nDemuxIndex取值{-1,nDemux,nDemux+1}
//@EXECUTIONFLOW: 4、调用CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex),判断能否正确返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、重复执行4, 直到nDemuxIndex取遍{-1,nDemux,nDemux+1}中的所有值
//@EXECUTIONFLOW: 6、重复执行2, 直到nTunerIndex取遍[0..nTuner-1]内的所有值
//@REMARK:某个非法Demux设备不能与任何合法的Tuner设备正确连接
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0005(void)
{
       int nDemuxCount = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int i =0;
       int   nTunerInfo = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i = 0; i < nDemuxCount; i++)
	{
	       GetConfigTunerInfo(i,&nTunerInfo);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXConnectTuner(nTunerInfo,-1),"步骤4失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXConnectTuner(nTunerInfo,nDemuxCount),"步骤4失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXConnectTuner(nTunerInfo,nDemuxCount+1),"步骤4失败\n");     	
	}	
	
	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@CASEGROUP:CSUDIDEMUXGetCapability
//@DESCRIPTION: m_dwDMAConnectedTunerId不为CSUDIDEMUX_NOT_CONNECTED_ID，能否断开成功
//@INPUT:1、nDemuxIndex索引值= [0..nDemux-1]
//@INPUT:2、nTunerIndex索引值= m_dwDMAConnectedTunerId
//@EXPECTATION: 能断开成功
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目
//@EXECUTIONFLOW: 2、nDemuxIndex取[0..nDemux-1]内的值
//@EXECUTIONFLOW: 3、调用CSUDIDEMUXGetCapability判断sCapabilityInfo中的m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 4、如果sCapabilityInfo.m_dwDMAConnectedTunerId不为CSUDIDEMUX_NOT_CONNECTED_ID,则执行下面的步骤
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXDisconnectTuner断开nDemuxIndex和m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 6、重复执行2, 直到nDemuxIndex取遍[0..nDemux-1]内的所有值
//@REMARK:
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0006(void)
{
       int nDemuxCount = 0;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDI_Error_Code bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)), "获取Demux数量失败");
	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux数量应该大于0");

	for(; nDemuxIndex < nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "获取Demux能力失败!");
		
		if(CSUDIDEMUX_NOT_CONNECTED_ID != sCapabilityInfo.m_dwDMAConnectedTunerId)
		{
			CSTCPrint("DemuxID:%d, TunerID:%d\n\n", nDemuxIndex, sCapabilityInfo.m_dwDMAConnectedTunerId);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(sCapabilityInfo.m_dwDMAConnectedTunerId, nDemuxIndex), "断开连接失败");
                     CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "获取Demux能力失败!"); 
                     CSTK_ASSERT_TRUE_FATAL(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId,"比较失败!");
		}
             else
             {
                break;
             }         
	}

       bResult = CSUDI_TRUE;
      
	CSTK_FATAL_POINT;
 	
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"恢复默认连接失败");//恢复默认连接Modified by qLuo 2011-8-22
    
	return bResult;
}


//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@CASEGROUP:CSUDIDEMUXGetCapability
//@DESCRIPTION: m_dwDMAConnectedTunerId不为CSUDIDEMUX_NOT_CONNECTED_ID，能否断开成功
//@INPUT:1、nDemuxIndex索引值= [0..nDemux-1]
//@INPUT:2、nTunerIndex索引值= m_dwDMAConnectedTunerId
//@EXPECTATION: 能断开成功
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目
//@EXECUTIONFLOW: 2、nDemuxIndex取[0..nDemux-1]内的值
//@EXECUTIONFLOW: 3、调用CSUDIDEMUXGetCapability判断sCapabilityInfo中的m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 4、如果sCapabilityInfo.m_dwDMAConnectedTunerId为CSUDIDEMUX_NOT_CONNECTED_ID,则执行下面的步骤
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXConnectTuner连接nDemuxIndex和m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXDisconnectTuner断开nDemuxIndex和m_dwDMAConnectedTunerId
//@EXECUTIONFLOW: 7、重复执行2, 直到nDemuxIndex取遍[0..nDemux-1]内的所有值
//@REMARK:
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0007(void)
{
       int nDemuxCount = 0;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	CSUDI_Error_Code bResult = CSUDI_FALSE;
       int   nTunerInfo = 0;
       int j = 0;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)), "获取Demux数量失败");
	CSTK_ASSERT_TRUE_FATAL((nDemuxCount > 0), "Demux数量应该大于0");

	for(; nDemuxIndex < nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "获取Demux能力失败!");
		
		if(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId)
		{
			 GetConfigCapability(nDemuxIndex,&sCapabilityInfo);
                      if(sCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_DMA	)
                      {
                            GetConfigTunerInfo(nDemuxIndex,&nTunerInfo);
                            for (j=0; j< 16;j++)
                            {
                                int pos = 1 << j;
                                if (nTunerInfo & pos)
                                {
                                          CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, nDemuxIndex),"步骤4失败!");	
                                          CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "获取Demux能力失败!"); 
                                          CSTK_ASSERT_TRUE_FATAL(CSUDIDEMUX_NOT_CONNECTED_ID != sCapabilityInfo.m_dwDMAConnectedTunerId,"比较失败!");
                                		CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, nDemuxIndex),"步骤8 失败!");
                                          CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sCapabilityInfo)), "获取Demux能力失败!"); 
                                          CSTK_ASSERT_TRUE_FATAL(CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId,"比较失败!");
                                }
                            }
                       }
		}
             else 
            {
                break;
            }
	}

       bResult = CSUDI_TRUE;
      
	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"恢复默认连接失败");//恢复默认连接Modified by qLuo 2011-8-22
    
	return bResult;
}



//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: 测试Demux设备与Tuner设备能连接连接的情况,当通过demux能力属性m_dwDMAConnectedTunerId去判断是否需要连接demux和tuner时,如果不具备连接能力,则不能连接
//@PRECONDITION:存在Demux设备
//@PRECONDITION:存在Tuner设备
//@PRECONDITION:在config文件中已有Demux与Tuner的连接矩阵表
//@INPUT:1、nTunerIndex索引值= [0..nTuner-1]
//@INPUT:2、nDemuxIndex索引值= [0..nDemux-1]
//@EXPECTATION: 1个Demux与Tuner的连接矩阵中，具备连接能力的一对Demux与Tuner设备，一定能连接成功并能被正确断开。
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目
//@EXECUTIONFLOW: 2、调用CSUDIDEMUXGetCapability获取指定Demux设备的能力
//@EXECUTIONFLOW: 3、调用CSUDIDEMUXDisconnectTuner，断开指定demux与指定tuner的连接
//@EXECUTIONFLOW: 4、针对连接矩阵表中的每一对Demux与Tuner设备，如果这对设备没有断开，则重复步骤3
//@EXECUTIONFLOW: 5、通过config文件得到Demux与Tuner的连接矩阵表
//@EXECUTIONFLOW: 6、针对连接矩阵表中的每一对Demux与Tuner设备，如果这对设备具备连接能力，则执行7-8
//@EXECUTIONFLOW: 7、调用CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex)，判断能否正确返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSUDIDEMUXDisconnectTuner(nTunerIndex,nDemuxIndex)，判断能否正确返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、针对连接矩阵表中的每一对Demux与Tuner设备，如果这对设备不具备连接能力,则执行8
//@EXECUTIONFLOW: 10、调用CSUDIDEMUXConnectTuner(nTunerIndex,nDemuxIndex)，返回CSUDI_SUCCESS则测试失败,否则测试用例成功
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0008(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i = 0;
	int nTunerInfo = 0;
	int j = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败!");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0, "返回参数失败!");
    
    for (i = 0; i < nDemuxCount; i++)
    {
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "步骤2获取Demux能力失败!");

        if (CSUDIDEMUX_NOT_CONNECTED_ID != sCapabilityInfo.m_dwDMAConnectedTunerId)
        {    
			if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask)
			{
				for (j = 0; j < 16; j++)
				{
					if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask&(1<<j))
					{
						if (sCapabilityInfo.m_dwDMAConnectedTunerId == j)
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i), "步骤3失败!");
						}
					}
				}
			}
        }
	}
    
    for (i = 0; i < nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "获取Demux能力失败!");
             
		if (CSUDIDEMUX_NOT_CONNECTED_ID == sCapabilityInfo.m_dwDMAConnectedTunerId)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == GetConfigCapability(i, &sCapabilityInfo), "步骤5失败!");
            
			if (sCapabilityInfo.m_dwWorkTypeMask & EM_UDI_DEMUX_DMA)
			{
				GetConfigTunerInfo(i, &nTunerInfo);
				for (j = 0; j < 16; j++)
				{
					int pos = 1 << j;
					if (nTunerInfo & pos)
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i), "步骤7失败!");	
						CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i), "步骤8失败!");
					}
					else
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXConnectTuner(j, i), "步骤10失败!");						
					}
				}
			}
		}
	}

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0), "恢复默认连接失败");

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: 测试demux 和tuner 在已经连接的情况再将两者连接,或者是已经断开的情况下再断开的场景
//@PRECONDITION:存在demux设备
//@INPUT:1、能与tuner连接的demux id
//@EXPECTATION: 1. demux 和tuner 在已经连接的情况再将两者连接应该返回成功
//@EXPECTATION: 2. demux 和tuner 已经断开的情况下再断开应该返回成功
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目,对demux [0,nDemux-1]做如下步骤
//@EXECUTIONFLOW: 2、调用CSUDIDEMUXGetCapability获取demux n的能力期望成功
//@EXECUTIONFLOW: 3、如果该demux的m_dwDMAConnectSupportTunersMask大于0,则找到能与其连接的tuner m
//@EXECUTIONFLOW: 4、如果demux n已经和tuner连接,则断开demux与tuner的连接,期望返回成功
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXConnectTuner(m,n)，期望返回成功
//@EXECUTIONFLOW: 6、再次调用CSUDIDEMUXConnectTuner(m,n)，期望返回成功
//@EXECUTIONFLOW: 7、调用CSUDIDEMUXDisconnectTuner(m,n)，期望返回成功
//@EXECUTIONFLOW: 8、再次调用CSUDIDEMUXDisconnectTuner(m,n)，期望返回成功
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0009(void)
{
	 int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       	CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i =0,j=0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i = 0; i < nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "step 2 获取Demux能力失败!");
		if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask)
		{
			for (j=0;j<16;j++)
			{
				if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask&(1<<j))
				{
					if (sCapabilityInfo.m_dwDMAConnectedTunerId == j)
					{
						CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 4 失败!");
					}

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i),"step 5 失败!");	
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i),"step 6 失败!");	
					CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 7 失败!");
					CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 8 失败!");
					bRet = CSUDI_TRUE;
				}
			}
		}
	}

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"恢复默认连接失败");//恢复默认连接Modified by qLuo 2011-8-22
	
	return bRet;
}

//@CASEGROUP:CSUDIDEMUXConnectTuner
//@CASEGROUP:CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: 测试demux 和tuner 进行反复的连接和断开
//@INPUT:1、能与tuner连接的demux id
//@EXPECTATION: 1. demux 和tuner 进行反复的连接和断开500次每次都能返回成功
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到nDemux的数目,对demux [0,nDemux-1]做如下步骤
//@EXECUTIONFLOW: 2、调用CSUDIDEMUXGetCapability获取demux n的能力期望成功
//@EXECUTIONFLOW: 3、如果该demux的m_dwDMAConnectSupportTunersMask大于0,则针对每个能连接的tuner m做如下操作
//@EXECUTIONFLOW: 4、如果demux n已经和tuner连接,则断开demux与tuner的连接,期望返回成功
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXConnectTuner(m,n)，期望返回成功
//@EXECUTIONFLOW: 6、调用CSUDIDEMUXDisconnectTuner(m,n)，期望返回成功
//@EXECUTIONFLOW: 7、重复步骤5和6一共500次,每次都期望返回成功
CSUDI_BOOL CSTC_DEMUX_IT_ConnectTuner_0010(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
       	CSUDIDEMUXCapability_S  sCapabilityInfo;
	int i =0,j=0,m=0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount), "步骤1失败\n");
  	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i = 0; i < nDemuxCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sCapabilityInfo)), "step 2 获取Demux能力失败!");
		if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask)
		{
			for (j=0;j<16;j++)
			{
				if (sCapabilityInfo.m_dwDMAConnectSupportTunersMask&(1<<j))
				{
					if (sCapabilityInfo.m_dwDMAConnectedTunerId == j)
					{
						CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 4 失败!");
					}

					for(m=0;m<500;m++)
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(j, i),"step 5 失败!");	
						CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(j, i),"step 8 失败!");
					}
				}
			}
		}
	}

	bRet = CSUDI_TRUE;

	CSTK_FATAL_POINT;
		
	CSTK_ASSERT_TRUE(CSUDI_SUCCESS ==  CSUDIDEMUXConnectTuner(0, 0),"恢复默认连接失败");//恢复默认连接Modified by qLuo 2011-8-22
	
	return bRet;
}

static int GetConfigFilterCount(int nIndex, int  *pnFilterCount)
{
  	char *pcSection = "DEMUX";
	char cResult[16];
	int  nResultSize = 16;
	char buffer[80];
	int nRet = -1;
	
	sprintf(buffer,"DEMUX%d_FilterNum",nIndex);
	memset(cResult,0,sizeof(cResult));
	CSTKGetConfigInfo(pcSection,buffer,cResult,nResultSize);
	*pnFilterCount = CSTKGetIntFromStr(cResult,10);
       CSTK_ASSERT_TRUE_FATAL((pnFilterCount !=CSUDI_NULL),"参数获取错误!");
	nRet = 0;
	  
	CSTK_FATAL_POINT;

	return nRet;
}

//@CASEGROUP:CSUDIDEMUXGetFreeFilterCount
//@DESCRIPTION: 测试指定Demux设备的空闲Filter总数
//@PRECONDITION:在config文件中，存在配置Demux能力配置表
//@INPUT:1、nDemuxIndex= [0..nDemux-1]
//@INPUT:2、pnFreeFilterCount=无符号整型指针
//@EXPECTATION: 成功获取指定Demux设备的空闲Filter总数，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetFreeFilterCount得到pnFreeFilterCount的数目
//@EXECUTIONFLOW: 2、通过config文件，得到该Demux的Filter总数
//@EXECUTIONFLOW: 3、对通过CSUDIDEMUXGetFreeFilterCount得到的空闲数目进行合法性检查
//@EXECUTIONFLOW: 4、如果数目合法，并返回CSUDI_SUCCESS。否则测试失败
CSUDI_BOOL CSTC_DEMUX_IT_GetFreeFilterCount_0001(void)
{
	int nDemuxCount =0;
	int nFreeFilterCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int  i =0;
       int nFilterCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"步骤1失败\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");

	for (i = 0; i < nDemuxCount; i++)
	{ 
        	GetConfigFilterCount(i,&nFilterCount);
	 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(i,&nFreeFilterCount),"步骤1失败\n");
	 	CSTK_ASSERT_TRUE_FATAL(nFreeFilterCount <= nFilterCount, "比较错误!");
	 }

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXGetFreeFilterCount
//@DESCRIPTION: 测试CSUDIDEMUXGetFreeFilterCount中nDemuxIndex参数错误情况下的执行结果
//@PRECONDITION:None
//@INPUT:1、nDemuxIndex= {-1,nDemux,nDemux+1}
//@INPUT:2、pnFreeFilterCount=无符号整型指针
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount得到Demux的数目nDemux
//@EXECUTIONFLOW: 2、取值nDemuxIndex= {-1,nDemux,nDemux+1}
//@EXECUTIONFLOW: 3、调用CSUDIDEMUXGetFreeFilterCount得到pnFreeFilterCount的数目
//@EXECUTIONFLOW: 4、判断能否正确返回非CSUDI_SUCCESS
//@REMARK:nDemuxIndex={-1,nDemux,nDemux+1}，CSUDIDEMUXGetFreeFilterCount函数均应返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DEMUX_IT_GetFreeFilterCount_0002(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int nFreeFilterCount=0;
	
       CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"步骤1失败\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetFreeFilterCount(-1,&nFreeFilterCount),"步骤3失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetFreeFilterCount(nDemuxCount,&nFreeFilterCount),"步骤3失败\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIDEMUXGetFreeFilterCount(nDemuxCount+1,&nFreeFilterCount),"步骤3失败\n");

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}

//@CASEGROUP:CSUDIDEMUXGetFreeFilterCount
//@DESCRIPTION: 测试CSUDIDEMUXGetFreeFilterCount在参数pnFreeFilterCount为空情况下执行结果
//@PRECONDITION:None
//@INPUT:1、nDemuxIndex= [0..nDemux-1]
//@INPUT:2、pnFreeFilterCount=CSUDI_NULL
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetFreeFilterCount
//@EXECUTIONFLOW: 2、判断返回值是否是非CSUDI_SUCCESS
//@REMARK:1、pnFreeFilterCount为NULL的情况下，系统不能出现死机，底层能正确分辨参数错误
//@REMARK:2、如果psFreeFilterCount参数为空,则无论其它参数如何，CSUDIDEMUXGetFreeFilterCount函数均返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_DEMUX_IT_GetFreeFilterCount_0003(void)
{
	int nDemuxCount =0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int  i =0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIDEMUXGetCount(&nDemuxCount),"步骤1失败\n");	
	CSTK_ASSERT_TRUE_FATAL(nDemuxCount > 0,"返回参数失败\n");
		
	for (i = 0; i < nDemuxCount; i++)
	{ 
	 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIDEMUXGetFreeFilterCount(i,CSUDI_NULL),"步骤2失败\n");
	}

	bRet = CSUDI_TRUE;
		 
	CSTK_FATAL_POINT;

	return bRet;
}


