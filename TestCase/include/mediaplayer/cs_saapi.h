/******************************************************************************
FileName:  cs_sa_api.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD。

Version:       Ver 1.0
Author:        Tong Fulei (tongfulei@coship.com)

File Description:
        citv server auth api.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2008-02-28  Tong Fulei      Create
2.0		    2008-06-28  Tong Fulei	    Add V3.0
*******************************************************************************/

#ifndef CS_SAAPI_H
#define CS_SAAPI_H
#ifdef __cplusplus
extern "C"
{
#endif
    /******************************************************************************
     *                                 头文件引用                                 *
     ******************************************************************************/
#include "mmcp_typedef.h"
#include "cs_utils.h"
#include "udi2_tuner.h"
    /******************************************************************************
     *                                 宏、定义、结构体                           *
     ******************************************************************************/
//存放相关节目信息
#define CSSA_MAX_Program_NUMBER 		(1000)
#define MAX_ERR_MSG_INFO_LEN   (128)
    typedef struct CSSAProgramInfo_S
    {
        char szChannelId[16];
        char szProgramName[64];
        char szProgramId[16];
        int nStartTime;
        int nEndTime;
    }CSSAProgramInfo_S;

    typedef struct CSSAProgramList_S
    {
        int nTotalCount;//psCorrelativeInfo所指向的空间能容纳CSSAProgramInfo_S的数量
        int nAvailCount;//有效节目的个数
        CSSAProgramInfo_S * psCorrelativeInfo;
    }CSSAProgramList_S;
//版本号，目前支持视讯2.3和3.0
    typedef enum CSITVServerVer_E
    {
        EM_ITV_SECOND_POINT_THREE,
        EM_ITV_THREE_POINT_ZERO,
        EM_ITV_THREE_POINT_ONE,
        EM_ITV_VER_COUNT
    }CSITVServerVer_E;
//机顶盒与前端的交互通道接入方式
    typedef enum CSITVMODE_E
    {
        EM_ITV_MODE_IPTV,//纯iptv
        EM_ITV_MODE_IPQAM,//双向
        EM_ITV_MODE_DVBIP,//双模
        EM_ITV_MODE_MAX
    }CSITVMODE_E;
    typedef enum CSITVDomainID_E
    {
        EM_ITV_UNDEFINED = 0,
        EM_ITV_NTPDOMAIN,
        EM_ITV_EPGDOMAIN,
        EM_ITV_UPGRADEDOMAIN,
        EM_ITV_MANAGEDOMAIN,
        EM_ITV_INFODOMAIN,//...5
        EM_ITV_GAMEDOMAIN,//游戏
        EM_ITV_PLAYBACKDOMAIN,
        EM_ITV_MOVIEDOMAIN,//点播
        EM_ITV_PORTALDOMAIN,
        EM_ITV_COLLECTIONDOMAIN,//...10
        EM_ITV_SEARCHDOMAIN,
        EM_ITV_PREFERDOMAIN,
        EM_ITV_VVDOMAIN,
        EM_ITV_SZDOMAIN,
        EM_ITV_ITVTVDOMAIN,//...15
        EM_ITV_MP3DOMAIN,
        EM_ITV_SHORTCUTDOMAIN,
        EM_ITV_TIMESHIFTDOMAIN,
        EM_ITV_PAYOUTDOMAIN,//支付
        EM_ITV_KLOKDOMAIN,//卡拉OK  20
        EM_ITV_STOCKDOMAIN//股票

    }CSITVDomainID_E;
//return value
    typedef enum CSITVRetVal_E
    {
        EM_ITV_SUCCESS = 0,
        EM_ITV_FAILURE,
        EM_ITV_NOAREACODE,//没有获取到区域码
        EM_ITV_SYNCFAIL,//NTP同步失败
        EM_ITV_NETFAULT,//网络故障
        EM_ITV_INVALIDSINA,//无效签名
        EM_ITV_TOKEN_OVERDUE,//签名过期
        EM_ITV_INVALIDUSER,//无效用户
        EM_ITV_NO_ORDER,
        EM_TTV_DATA_ERROR
    }CSITVRetVal_E;
//认证状态
    typedef enum CSITV_SA_STATE_E
    {
        EM_ITV_SASTATE_INIT = 0,//没有认证
        EM_ITV_SASTATE_FAIL ,//认证失败
        EM_ITV_SASTATE_SUCCESS ,//认证成功
        EM_ITV_SASTATE_NOTAREACODE, //认证成功,但没有请求到区域码
        EM_ITV_SASTATE_TIMESYNC

    }CSITV_SA_STATE_E;
//终端接入设备的类型
    typedef enum CSITV_SA_CPETYPE_E
    {
        EM_ITV_CPETYPE_STB,
        EM_ITV_CPETYPE_MOBILE,
        EM_ITV_CPETYPE_PC
    }CSITV_SA_CPETYPE_E;
//终端支持的视频类型
    typedef enum CSITV_SA_VIDEO_TYPE_E
    {
        EM_ITV_VIDEO_MPEG2_SD,
        EM_ITV_VIDEO_MPEG2_HD,
        EM_ITV_VIDEO_H264_SD,
        EM_ITV_VIDEO_H264_HD
    }CSITV_SA_VIDEO_TYPE_E;
//端设备显示模块支持的分辨率
    typedef enum CSITV_SA_RESOLUTION_E
    {
        EM_ITV_RESOLUTION_720_576,
        EM_ITV_RESOLUTION_1280_720
    }CSITV_SA_RESOLUTION_E;
    typedef int (* FOnCSSAGetMac)(char *pMacAdress,int nLen);
    typedef int (* FOnCSSAGetIP)(char *pIPAddress,int nLen);
    typedef struct  CSITVLoginInfo_S
    {
        char m_caSTBSMID[64]; //CA卡号
        char m_caSTBID[32]; //机顶盒序列号
        char m_caSTBPSWORD[32]; //机顶盒密码
        char m_caSTBProvider[32];//机顶盒供应商
        char m_caUserId[64];//第一次登陆成功后保存到flash中
        char m_caITVToken[36];//需要在机顶盒长期保存，每次登陆后更新
        //第一次登陆此字段必须为null
        char m_caServerIP[20];//服务器IP
        char m_caLoginKey[64];//加密因子
        char m_caAlias[64];//别名
        CSITVServerVer_E m_eSTBVersion;//服务器版本
        int  m_nServerPort;//服务器端口,默认80
        CSITVMODE_E m_eMode;//接入方式
        char m_caSTBModel[32];//机顶盒型号
        CSITV_SA_CPETYPE_E  m_eCPEType;//默认EM_ITV_CPETYPE_STB
		char m_caVideoType[32];//默认EM_ITV_VIDEO_MPEG2_SD
		CSITV_SA_RESOLUTION_E m_eResolution;//默认EM_ITV_RESOLUTION_720_576
        int m_nBandWith;//m_eMode为EM_ITV_MODE_IPTV时有效
        char  m_caBrowserVersion[64];//浏览器版本
        FOnCSSAGetMac m_fGetMac;
        FOnCSSAGetIP m_fGetIP;
		//以下为江苏省网认证新增字段，其他项目可以不填
		char m_caMiddleWareVer[32];  //中间件版本号
		char m_caSTBSoftWareVer[16];  //机顶盒软件版本号
		char m_caTVCode[16];  //电视号
    } CSITVLoginInfo_S;
   typedef struct CSSAErrorMsg_S
   {
   		int nErrorCode;
		char pcMsg[MAX_ERR_MSG_INFO_LEN];
   }CSSAErrorMsg_S;

	typedef struct CSSAPlaybackChannelInfo_S
  {
	 char m_caName[64];
	 char m_caResourceName[64];
	 char m_caChannelID[64];
	 char m_caMulticastIP[20];
	 int m_nFlag;
	 int m_nChannelType;
	 int m_nTSID;
	 int m_nServiceID;
	 int m_nMulticastPort;
 }CSSAPlaybackChannelInfo_S;
   //回调消息类型
	typedef enum CSSAMsg_E
	{
	    EM_ITV_GETAREACODE_SUCCESS = 0,//对应第一个参数是存储区域码的首地址，第二个参数是空间的大小
		EM_ITV_GETAREACODE_FAIL
	}CSSAMsg_E;
    /******************************************************************************
     *                                 函数声明                                   *
     ******************************************************************************/

    /******************************************************************************
    Function Name:    CSSA_Initialize

    Description:
      独立的初始化认证模块。初始化前需先注册依赖接口(SA_Interface)

    Input:
      无
    Output:

    Return Value:
      success : 	EM_ITV_SUCCESS
      fail :		EM_ITV_FAILURE

    Other:
    *******************************************************************************/
    int CSSA_Initialize();

    /******************************************************************************
    Function Name:    CSSA_Init

    Description:
      MMCP专用初始化认证模块。会完成以MMCP为依赖的SA_Interface的注册工作
      使用该接口时可不处理SA_Interface相关工作。

    Input:
      无
    Output:

    Return Value:
      success : 	EM_ITV_SUCCESS
      fail :		EM_ITV_FAILURE

    Other:
    *******************************************************************************/
    int CSSA_Init();

    /******************************************************************************
    函数名称:	CSSA_Login

    功能描述:
    		登陆认证服务器,在这之前保证已经设置了认证需要的信息。

    输入参数:
    		无

    输出参数:
    		无

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:同洲视讯SA认证,返回EM_ITV_FAILURE时可以用CSSA_GetErrorMsg接口去获得出错信息
    ******************************************************************************/
    int CSSA_Login();

	/******************************************************************************
    函数名称:	CSSA_Logout

    功能描述:
    		注销当前登录

    输入参数:
    		无

    输出参数:
    		无

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:同洲视讯SA认证,返回EM_ITV_FAILURE时可以用CSSA_GetErrorMsg接口去获得出错信息
    ******************************************************************************/
    int CSSA_Logout();
    /******************************************************************************
    函数名称:	CSSA_SetUserInfo

    功能描述:
    		设置用户信息

    输入参数:
    		psInfo:用户信息

    输出参数:
    		无

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:同洲视讯SA认证
    ******************************************************************************/
    int CSSA_SetUserInfo(CSITVLoginInfo_S* psInfo);

    /******************************************************************************
    函数名称:	CSSA_GetAuthResult

    功能描述:
    		获取认证结果

    输入参数:
    		无

    输出参数:
    		无

    返 回 值:
    		参考CSITV_SA_STATE_E.

    其他说明:同洲视讯SA认证
    ******************************************************************************/
    CSITV_SA_STATE_E CSSA_GetAuthResult();
    
    /******************************************************************************
    函数名称:	CSSA_GetIndexByTsIdAndServiceId

    功能描述:
    		认证通过后可以通过此函数判断（nTsId,nServiceId）当前节目是否支持
    		回放，如果支持则返回成功，同时获取所对应的索引。

    输入参数:
    		nTsId：
    		nServiceId：

    输出参数:
    		pnIndex

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:同洲视讯SA认证
    ******************************************************************************/
    int CSSA_GetIndexByTsIdAndServiceId(int nTsId, int nServiceId, int *pnIndex);
        
    /******************************************************************************
    函数名称:	CSSA_GetAuthenCode

    功能描述:
    		获取认证码.

    输入参数:
    		szAuthenCode:存储认证码,存储空由调用者申请。存储空间大于32个字节。

    输出参数:
    		无

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_GetAuthenCode(char *szAuthenCode);

	/******************************************************************************
    函数名称:	CSSA_SetAreaCode

    功能描述:
    		获取区域码.

    输入参数:
    		szAreaCode:	存储区域码,存储空由调用者申请。存储空间小于64个字节
    		nLen:		nLen为设置的区域码的长度,应该小于等于64

    输出参数:
    		无

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_SetAreaCode(char * szAreaCode,int nLen);
    /*****************************************************************************
    函数名称:	CSSA_GetAreaCode

    功能描述:
    		获取区域码.

    输入参数:
    		szAreaCode:存储区域码,存储空由调用者申请。存储空间大于32个字节。
			bBlock     是否强制扫描区域码bBlock为TRUE且底层没保存区域码的有效值时强制扫描区域码,
					   否则只返回底层保存的当前值（空或者有效值）
    输出参数:
    		无

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
	    如果用户调用bForce为true,强制同步获取区域码，此方法阻塞
	    如果用户调用bForce为FALSE,获取已有的区域码
	******************************************************************************/
    int CSSA_GetAreaCode(char * szAreaCode, BOOL bBlock);

/******************************************************************************
函数名称:	CSSA_GetAreaCodeByChannel

功能描述:
		根据输入的推流通道，获取相应的区域码，如果没有合法的区域码，则输出'\0'.

输入参数:
		pszAreaCode:存储区域码,存储空由调用者申请。存储空间大于32个字节。
		pszChannel: 推流通道，可选值:"IPTV","IPQAM"
输出参数:
		无

返 回 值:
		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

其他说明:
	此函数主要用于双模机顶盒可以做到通道协商的情况下，可以根据不同的通道选择获取对应的区域码
    如果用户获取IPQAM区域码,且是第一次获取或者之前没有获取成功，都将强制同步获取区域码，此方法阻塞
    如果用户获取IP区域码,则是返回前端下发的区域码
******************************************************************************/
int CSSA_GetAreaCodeByChannel(char *pszAreaCode,const char* pszChannel);

/******************************************************************************
    函数名称:	int CSSA_GetErrorMsg(CSSAErrorMsg_S * psErrorMsg)

    功能描述:
    		获取认证过程中的错误信息

    输入参数:
    		无

    输出参数:
    		psErrorMsg 指向应用分配的缓存区,用来存错误码和错误信息等.

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:nErrorCode参考
    
	    0000：激活成功 登录成功
		0602：用户参数不能为空
		0611：登录参数不能为空
		0612：用户签名不能为空
		0613：激活参数不能为空
		0614：用户别名不能为空
		0616：用户的标识id不能为空
		0617：用户标识id不是数字
		0619：硬件提供厂商不能为空
		0622：用户的接口版本不能为空
		0623：接口版本错误
		0633：激活失败
		0650：终端用户登录失败
		0661：指定终端用户不存在
		0662：指定用户在数据库中不存在
		0665：用户最新版本值不存在
		0668：终端用户还没有激活，请先激活
		0669：输入密码错误
		0671：检查签名失败
		0676：版本信息不能为空
		0678：Stb厂商提供者不能为空
		0690：3.1认证关键字配置错误
		0691：用户不唯一
		0692：必需有在sic,stb,alies,mac字段中取其中一个作为身份标识
    ******************************************************************************/
	int 	CSSA_GetErrorMsg(CSSAErrorMsg_S * psErrorMsg);
	/******************************************************************************
    函数名称:	CSSA_GetSmcId

    功能描述:
    		获取smart card id.

    输入参数:
    		szSmcId:存储smart card id,存储空由调用者申请。存储空间大于32个字节。

    输出参数:
    		无

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_GetSmcId(char *szSmcId);
    
    /******************************************************************************
    函数名称:	CSSA_SetSmcId

    功能描述:
    		设置smart card id,作为认证的关键字使用.

    输入参数:
    		szSmcId:存储smart card id,存储空由调用者申请。

    输出参数:
    		无

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_SetSmcId(char *szSmcId);
    
    /******************************************************************************
    函数名称:	CSSA_GetCurChannelProgramList

    功能描述:
    		获取由tsid和serviceid指定频道的节目列表
    输入参数:
    		nTsId:
    		nServiceId :
    输出参数:
    		pProList:存储与指定频道的节目列表
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_GetCurChannelProgramList(int nTsId, int nServiceId, CSSAProgramList_S * pProList);
	/******************************************************************************
    函数名称:	CSSA_GetChannelProgListBat

    功能描述:
    		获取指定的几个频道的回放节目列表
    输入参数:
    		psLctDVBInfo 	指向一个CSLocatorDVBInfo_S的数组，存储了指定的频道的TsId,ServiceID
			pnChannelCount	指向psLctDVBInfor指向数组的大小，也就是指定的频道个数,当请求的频道中有重复时，函数会将它指向的值改为实际频道数
								
    输出参数:
    		pProList  		存储指定几个频道的回放节目列表
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:返回EM_ITV_FAILURE时可以用CSSA_GetErrorMsg接口去获得出错信息
    ******************************************************************************/
	int CSSA_GetChannelProgListBat(CSLocatorDVBInfo_S  * psLctDVBInfo,CSSAProgramList_S * pProList,int * pnChannelCount);
    /******************************************************************************
    函数名称:	CSSA_GetCorrelativeProgramList

    功能描述:
    		获取与指定节目关联的节目列表
    输入参数:
    		pcChannelId:频道的id
    		pcProgramName :节目名称
    		pcStartTime:节目开始时间
    		pcProgramId:节目的id,2.3版本可以为NULL
    输出参数:
    		pCorrProList:存储与指定节目关联的节目列表
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:返回EM_ITV_FAILURE时可以用CSSA_GetErrorMsg接口去获得出错信息
    ******************************************************************************/
    int CSSA_GetCorrelativeProgramList(char * pcChannelId, char * pcProgramName, char * pcStartTime,char *pcProgramId , CSSAProgramList_S * pCorrProList);
    
    /******************************************************************************
    函数名称:	CSSA_GetCorrRtspUrlByIndex

    功能描述:
    		获取关联列表中指定索引的url
    输入参数:
    		Index:关联节目的索引号
    		nLen :szUrl的长度
    输出参数:
    		szUrl:存储指定节目的url
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:返回EM_ITV_FAILURE时可以用CSSA_GetErrorMsg接口去获得出错信息
    ******************************************************************************/
    int CSSA_GetCorrRtspUrlByIndex(int nIndex, char * szUrl,int nLen);
    
    /******************************************************************************
    函数名称:	CSSA_GetChannelProgRtspUrlByIndex

    功能描述:
    		获取频道列表中指定索引的url
    输入参数:
    		Index:频道节目的索引号
    		nLen :szUrl的长度
    输出参数:
    		szUrl:存储指定节目的url
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_GetChannelProgRtspUrlByIndex(int nIndex, char * szUrl,int nLen);
    
    /******************************************************************************
    函数名称:	CSSA_GetPBRtspUrl

    功能描述:
    		获取指定频道指定时间的url
    输入参数:
    		nTsId:   tsID
    		nServiceId:  节目serviceID
    		dwStartTime :指定的开始时间,单位s
    		nUrlLen:pDstUrl的存储空间长度
    输出参数:
    		pDstUrl:指向存储该节目的url的buffer


    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_GetPBRtspUrl(int nTsId, int nServiceId, DWORD dwStartTime, char *pDstUrl, int nUrlLen);
	/******************************************************************************
	函数名称:	CSSA_GetMultTimeShiftUrl

	功能描述:
			根据组播地址、组播端口、时移开始时间获取组播时移的url.

	输入参数:
			pcMultIP:组播IP,格式如224.0.1.10
			wMultPort:组播端口
			dwStartTime:时移开始时间,相对1970年的秒数
			nUrlLen:时移url存储空间的长度

	输出参数:
			pDstUrl:时移url存储空间，由调用者申请
	返 回 值:
			EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

	其他说明:
******************************************************************************/
	int CSSA_GetMultTimeShiftUrl(char* pcMultIP, WORD wMultPort,DWORD dwStartTime, char *pDstUrl, int nUrlLen);
	/******************************************************************************
	函数名称:	CSSA_ISSupportMultTimeShift

	功能描述:
			根据组播IP地址和组播端口。

	输入参数:
			pcMultIP:组播IP,格式如224.0.1.10
			wMultPort:组播端口
	输出参数:

	返 回 值:
			TRUE 支持时移，FALSE 不支持时移

	其他说明:
	******************************************************************************/
	BOOL CSSA_ISSupportMultTimeShift(char* pcMultIP, WORD wMultPort);
    /******************************************************************************
    函数名称:	CSSA_GetVersion

    功能描述:
    		获取当前使用的版本号
    输入参数:

    输出参数:
    返 回 值:
    		具体参考CSITVServerVer_E

    其他说明:
    ******************************************************************************/
    CSITVServerVer_E CSSA_GetVersion();
    
    /******************************************************************************
    函数名称:	CSSA_GetServerUrl

    功能描述:
    	获取镇定类型的服务器地址,为兼容以前的应用保留,建议使用CSSA_GetParameterInfo
    输入参数:
    	eDomainType:服务器类型
    	nServerLen:szServer的长度
    输出参数:
    	szServer:存储服务器的地址

    返 回 值:
    		具体参考CSITVServerVer_E

    其他说明:推荐调用CSSA_GetParameterInfo来获得服务器地址
    ******************************************************************************/
    int CSSA_GetServerUrl(CSITVDomainID_E eDomainType, char * szServer, int nServerLen);
    /******************************************************************************
    函数名称:	CSSA_GetParameterInfo

    功能描述:
    	获取前端发送的配置参数信息
    输入参数:
    	pcKey:配置参数的名称,请参考 CiTV平台认证接口规范.doc 最新版本的命名	
    	nLen: 用户申请空间的大小,当配置参数实际长度大于nLen-1时,返回EM_ITV_FAILURE.
    输出参数:
    	pcValue:存储配置参数的值,内存由应用申请

    返 回 值:
    	EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    	常用的KEY:iEPG_URL,NTP_SERVER详见<<CiTV平台认证接口规范.doc>>
    ******************************************************************************/
    int CSSA_GetParameterInfo(char * pcKey,char * pcValue,int nLen);
    /******************************************************************************
    函数名称:	CSSA_InsertStrToUrl

    功能描述:
    	对点播url处理,增加认证信息，用于3.0版本。
    输入参数:
    	pUrl:点播服务器下传的url.
    输出参数:
    	pDstUrl:处理后的url
    	nDstUrlLen:pDstUrl的长度
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_InsertStrToUrl(const char *pUrl, char *pDstUrl, int nDstUrlLen);
    
    /******************************************************************************
    函数名称:	CSSA_GetUserId

    功能描述:
    		获取用户的标识
    输入参数:
    		pUserId:存储用户标识
    		nLen:pUserId的长度
    输出参数:
    		pUserId:存储用户标识
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_GetUserId(char * pUserId,int nLen);
    
    /******************************************************************************
    函数名称:	CSSA_SetMode

    功能描述:
    		设置认证模式,默认EM_ITV_MODE_IPQAM_PES
    输入参数:
    		eMode:认证模式，主要区分IPTV和IPQAM
    输出参数:
    		无
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_SetMode(CSITVMODE_E eMode);
    
    /******************************************************************************
    函数名称:	CSSA_SetSAServer

    功能描述:
    		设置设置认证服务器，用于初始化时服务器设置错误，
    		认证前从前设置服务器地址。
    输入参数:
    		pSAServer:服务器地址
    输出参数:
    		无
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    ******************************************************************************/
    int CSSA_SetSAServer(char *pSAServer);

    int CSSA_GetFreListForAreacode(char *pFrequenceList,int nLen);
   /******************************************************************************
    函数名称:	CSSA_GetFreList

    功能描述:
    		获取认证前端下发的频点列表
    		
    输入参数:
    		psFreqList:频点列表指针
    		pnNum: 传入的CSUDITunerCableDeliver_S结构体的大小
    输出参数:
    		psFreqList:填充了服务器下发的频点列表指针
    		pnNum: 实际返回的CSUDITunerCableDeliver_S结构体的大小
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    		可以正确获取的前提是认证通过
    ******************************************************************************/
	int CSSA_GetFreList(CSUDITunerCableDeliver_S* psFreqList,int* pnNum);

    int CSSA_GetSignature(char *pSignature,int nLen);

    int CSSA_GetMac(char *pMac,int nLen);    

	int CSSA_Cleanup();
	/***下面函数是为dtvmx实现的,注释以后补充**/

	int  CSSA_GetChannelId(int networkId,int nTsId, int nServiceId,char *szChannelId ,int nLen );

	BOOL CSSA_ISSupportPlayBack(int nNetworkId,int nTsId,int nServiceId);

	int   CSSA_GetOrderFlag(int nNetworkId,int nTsId,int nServiceId,int * pnFlag);
	 /******************************************************************************
    函数名称:	CSSA_GetPlaybackURLByChannelID

    功能描述:
    		获取指定频道指定时间的url
    输入参数:
    		pChannelID:频道ID
    		dwStartTime :指定的开始时间,单位s
    		nUrlLen:pDstUrl的存储空间长度
    输出参数:
    		pDstUrl:指向存储该节目的url的buffer

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    		此函数完成的功能和CSSA_GetPBRtspUrl相同，区别在于CSSA_GetPlaybackURLByChannelID要输入pChannelID,
    		而不是三要素中的tsID  serviceID
    ******************************************************************************/
	int   CSSA_GetPlaybackURLByChannelID(char *pChannelID, DWORD dwStartTime, char *pDstUrl, int nUrlLen);
	 /******************************************************************************
    函数名称:	CSSA_StartScanAreaCode

    功能描述:
    		启动扫描区域码
    输入参数:
    
    输出参数:
    		

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    	CSSA_StartScanAreaCode和CSSA_StopScanAreaCode一定要成对调用
    ******************************************************************************/
	int   CSSA_StartScanAreaCode();
	 /******************************************************************************
    函数名称:	CSSA_StopScanAreaCode

    功能描述:
    		启动扫描区域码
    输入参数:
    
    输出参数:
    		

    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

    其他说明:
    	CSSA_StartScanAreaCode和CSSA_StopScanAreaCode一定要成对调用
    ******************************************************************************/
	int   CSSA_StopScanAreaCode();
	/******************************************************************************
	Function Name:    FOnCSSACallback
	 
	Description:
		用户回调,SA把消息返回给用户
	 
	Input:
		dwUserData: 用户在创建SA时传入的用户数据
		nMsg : 消息类型
		dwPara1 : 	参数1 
		dwPara2 :	参数2 
	 
	Output:
	 
	Return Value:
	 
	Other:
	*******************************************************************************/
	typedef void (*FOnCSSACallback)(DWORD dwUserData, CSSAMsg_E enMsg, DWORD dwPara1, DWORD dwPara2);	
	 /******************************************************************************
	函数名称:	CSSA_AddCallback

	功能描述:
			
	输入参数:
		dwUserData:用户参数
		callback:回调函数
	输出参数:
			

	返 回 值:
			EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

	其他说明:
	******************************************************************************/
	int CSSA_AddCallback(DWORD dwUserData, FOnCSSACallback callback);
	 /******************************************************************************
	函数名称:	CSSA_RemoveCallback

	功能描述:
		dwUserData:用户参数
		callback:回调函数
	输入参数:

	输出参数:
			

	返 回 值:
			EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败

	其他说明:
	******************************************************************************/
	int CSSA_RemoveCallback(DWORD dwUserData, FOnCSSACallback callback);
	/******************************************************************************
    函数名称:	CSSA_GetChannelList

    功能描述:
    		
    输入参数:
    			
    输出参数:			
    			psChannelInfo     频道列表缓存区首地址
    			pnChannelNum	  频道数目,输出结果为实际频道数
    返 回 值:
    		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败
    其他说明:
    ******************************************************************************/
    int CSSA_GetChannelList(CSSAPlaybackChannelInfo_S * psChannelInfo,int * pnChannelNum);

/******************************************************************************
函数名称:	CSSA_GetStreamChannel

功能描述:
		获取前端下发的通道选择策略
输入参数:
			
输出参数:			
			pcKey     通道策略关键字，如"HD","SD"
			pcValue	  pcKey对应的前端下发的通道策略
			nLen      pcValue的所占字节大小    
返 回 值:
		EM_ITV_SUCCESS 成功，EM_ITV_FAILURE 失败
其他说明:
******************************************************************************/
int CSSA_GetStreamChannel(char * pcKey,char * pcValue,int nLen);

#ifdef __cplusplus
}
#endif

#endif /* CS_SAAPI_H */

/* 文件结束 */

