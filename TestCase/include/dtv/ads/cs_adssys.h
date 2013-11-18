/**
@defgroup cs_adssys cs_adssys-广告系统终端数据收取解析
@{
*/

#ifndef _CS_ADSSYS_H_
#define _CS_ADSSYS_H_

#include "cs_adsdata.h"


#ifdef __cplusplus
extern "C" {
#endif


///全局的service ID
#define CS_ADSSYS_GLOBAL_SERVICE_ID 0xffff

///停止模式@see CSAdsSys_Stop
typedef enum
{
	EM_ADSSYS_STOPMODE_CLEAR_NONE,///<停止接收广告数据，但是不清除数据
	EM_ADSSYS_STOPMODE_CLEAR_DATA///<停止接收广告搜索，同时清除数据
}CSAdsSysStopMode_E; 

///创建广告数据模块实例的参数@see CSAdsSys_Create
typedef struct
{
	CSAdsSysResolution_E eResolution;	///< 可接收的数据的高标清类型
	WORD wPid;	///< 广告表PID
	BYTE ucTableId;	///< 广告表ID
}CSAdsSysParam_S;

///CSAdsSys_Start的参数@see CSAdsSys_Start
typedef struct
{
	DWORD dwDemuxId; ///< demux编号，单demux/单tuner的机顶盒通常为0
	CSAdsSysService_S sService; ///< 频道
}CSAdsSysStart_S;

/**
广告模块事件回调类型
@param psEvent 输入参数，事件
@param pUserData 输入参数，用户注册回调的自定义参数
@param pSender 输入参数，事件的发出者(CSAdsSys_Create的返回值)
@note 请勿在本回调中执行过多代码，建议抛事件到其它线程处理
*/
typedef void (*CSOnAdsSysCallback_F)(CSAdsSysEvent_S * psEvent, void *pUserData, void* pSender);

typedef void (*CSOnAdsSmartCardCallBack_F)(void *pUserData);

///CSAdsSys_SetCommonParam的参数@see CSAdsSys_SetCommonParam
typedef struct
{
	/**
	广告模块获取时间的回调函数，必须要有
	@param bOffset 输入参数，是否要带时区偏移,TRUE要带，FALSE不带
	@param psDateTime 输出参数，当前时间
	*/	
	void (*fnGetDateTime)(BOOL bOffset, CSUDIPLUSOSSysTime_S *psDateTime);
	/**
	广告模块分配内存的回调，必须要有
	@param nSize 输入参数，要分配的内存大小
	@return 分配的内存地址
	*/
	void* (*fnMalloc)(int nSize);
	/**
	广告模块释放内存的回调
	@param p 输入参数，要释放的内存
	*/	
	void (*fnFree)(void *p);
	 /**
	 所有的广告管理器最多可用的内存
	 */
	int nMaxMemorySizeLimite;
}CSAdsSysCommonParam_S;

/**
@brief 设定所有的广告管理器的公共设置
@param psParam 输入参数，公共参数
@note 必须在CSAdsSys_Create之前调用，且只能被调用一次
*/
void CSAdsSys_SetCommonParam(CSAdsSysCommonParam_S *psParam);


/**
@brief 创建一个广告管理器对象
@param psParam 输入参数，初始化所需的参数
@retval 一个广告管理器对象，失败返回NULL
*/
void* CSAdsSys_Create(CSAdsSysParam_S *psParam);

/**
@brief 销毁一个广告管理器对象
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@retval 成功为TRUE，失败为FALSE
*/
BOOL CSAdsSys_Destroy(void* pASObj);

/**
@brief 设置stb id
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param pucStbId 输入参数，机顶盒ID，通常为Smart卡号
@param nSize 输入参数，pucStbId的字节个数
@retval 成功为TRUE，失败为FALSE
@note 在CSAdsSys_Create后立刻调用
*/
BOOL CSAdsSys_SetStbId(void* pASObj, BYTE *pucStbId, int nSize);

/**
@brief 设置回调事件通知
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param fnCallback 输入参数，客户端的回调函数
@param pUserData 输入参数，自定义数据，在回调函数中回传给用户
*/
void CSAdsSys_RegisterCallback(void* pASObj, CSOnAdsSysCallback_F fnCallback, void *pUserData);

/**
@brief 获取CSAdsSys_RegisterCallback的pUserData值
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@retval 用户自定义参数,CSAdsSys_RegisterCallback中的pUserData值
*/
void* CSAdsSys_GetUserData(void* pASObj);

/**
@brief 开始加载解析某个节目的广告数据
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param psStart 输入参数，指定开始加载的节目参数,该节目将做为当前节目
@retval 成功为TRUE，失败为FALSE
@note 通常在换台后调用，注意换台只需要调用本函数即可，通常不用调用CSAdsSys_Stop
@note 每次调用本API，会发出EM_ADSSYS_CURRENT_CONTENT_CHANGE_EVENT
@code
	CSAdsSysStart_S sStart;
	sStart.sService.wOnId = 100;
	sStart.sService.wTsId = 1;
	sStart.sService.wServiceId= 4;
	sStart.dwDemuxId = 0;
	USPPrint(ADS_DEMO_LEVEL, "CSAdsSys_Start_____________________________________________\n");
	bRet = CSAdsSys_Start(psAS, &sStart);
@endcode
*/
BOOL CSAdsSys_Start(void* pASObj, CSAdsSysStart_S *psStart);

/**
@brief 开始加载解析某个节目的广告数据
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param psStart 输入参数，指定开始加载的节目参数,该节目将做为当前节目
@retval 成功为TRUE，失败为FALSE
@note 本函数执行与CSAdsSys_Start相同的功能，但不会发出EM_ADSSYS_CURRENT_CONTENT_CHANGE_EVENT
*/
BOOL CSAdsSys_StartStilly(void* pASObj, CSAdsSysStart_S *psStart);

/**
@brief 停止广告数据的加载
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param eStopMode 输入参数，指定停止模式
@retval 成功为TRUE，失败为FALSE
@note 换台(包括频点变化的情况)后通常不用调用CSAdsSys_Stop，除非确有必要停止广告搜索/数据版本监视，则可调用。
*/
BOOL CSAdsSys_Stop(void* pASObj, CSAdsSysStopMode_E eStopMode);

/**
@brief 获取广告中的文本信息的Block对象
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param ucTypeIndex 输入参数，_CSADS_IMAGE_TYPE_CONST所定义的广告位置标识序号
@param psSrvTime 输入参数，指定节目与时刻，为NULL则使用当前时间和当前节目
@param psDisplayDscrpt 输出参数，输出开始时间、结束时间等信息
@retval 一个文本块对象，如果为NULL表示没有
@note 使用完毕后，必须马上调用一次CSAdsSys_ReturnBlock归还该对象
@see _CSADSSYS_IMAGE_TYPE_INDEX_CONST
*/
CSAdsSysTextBlock_S*  CSAdsSys_TakeTextBlock(void* pASObj, BYTE ucTypeIndex, CSAdsSysDisplay_S *psSrvTime, CSAdsSysDisplayDscrpt_S* psDisplayDscrpt);

/**
@brief 获取广告中的图片信息的Block对象
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param ucTypeIndex 输入参数，_CSADS_IMAGE_TYPE_CONST所定义的广告位置标识序号
@param psSrvTime 输入参数，指定节目与时刻，为NULL则使用当前时间和当前节目
@param psDisplayDscrpt 输出参数，输出开始时间、结束时间等信息
@retval 一个图片块对象，如果为NULL表示没有
@note 使用完毕后，必须马上调用一次CSAdsSys_ReturnBlock归还该对象
@see _CSADSSYS_IMAGE_TYPE_INDEX_CONST
*/
CSAdsSysImageBlock_S*  CSAdsSys_TakeImageBlock(void* pASObj, BYTE ucTypeIndex, CSAdsSysDisplay_S *psSrvTime, CSAdsSysDisplayDscrpt_S* psDisplayDscrpt);

/**
@brief 获取广告中的Video信息的Block对象
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param ucTypeIndex 输入参数，_CSADS_IMAGE_TYPE_CONST所定义的广告位置标识序号
@param psSrvTime 输入参数，指定节目与时刻，为NULL则使用当前时间和当前节目
@param psDisplayDscrpt 输出参数，输出开始时间、结束时间等信息
@retval 一个图片块对象，如果为NULL表示没有
@note 使用完毕后，必须马上调用一次CSAdsSys_ReturnBlock归还该对象
@see _CSADSSYS_IMAGE_TYPE_INDEX_CONST
*/
CSAdsSysVideoBlock_S*  CSAdsSys_TakeVideoBlock(void* pASObj, BYTE ucTypeIndex, CSAdsSysDisplay_S *psSrvTime, CSAdsSysDisplayDscrpt_S* psDisplayDscrpt);

/**
@brief 获取广告中的文本信息的Block对象
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param dwBlockId 输入参数，数据块ID
@retval 一个文本块对象，如果为NULL表示没有
@note 使用完毕后，必须马上调用一次CSAdsSys_ReturnBlock归还该对象
*/
CSAdsSysTextBlock_S*  CSAdsSys_TakeTextBlockById(void* pASObj, DWORD dwBlockId);

/**
@brief 获取广告中的图片信息的Block对象
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param dwBlockId 输入参数，数据块ID
@retval 一个图片块对象，如果为NULL表示没有
@note 使用完毕后，必须马上调用一次CSAdsSys_ReturnBlock归还该对象
*/
CSAdsSysImageBlock_S*  CSAdsSys_TakeImageBlockById(void* pASObj, DWORD dwBlockId);

/**
@brief 获取广告中的Video信息的Block对象
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param dwBlockId 输入参数，数据块ID
@retval 一个图片块对象，如果为NULL表示没有
@note 使用完毕后，必须马上调用一次CSAdsSys_ReturnBlock归还该对象
*/
CSAdsSysVideoBlock_S*  CSAdsSys_TakeVideoBlockById(void* pASObj, DWORD dwBlockId);

/**
@brief 强制释放某个Block所占用的内存，通常用于释放需要存储到flash的block
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param dwBlockId 输入参数，数据块ID
@retval TRUE成功,FALSE失败
@note 请确保在调用本函数之前，不再使用block的数据,建议在CSAdsSys_ReturnBlock之后调用
*/
BOOL CSAdsSys_ForceFreeBlockById(void* pASObj, DWORD dwBlockId);

/**
@brief 同样的block可以对应多个广告位，每个广告位的location是不同的。
本函数从block对象中获取ucTypeIndex指定的location描述符。
通常在Take之后调用
@param psBlock 输入参数，一个数据块
@param ucTypeIndex 输入参数，广告位编号
@param pbExsit 输出参数，是否存在该描述符
@param psLocationDscrpt 输出参数，location描述内容
@retval TRUE操作成功，FALSE失败
@code
CSAdsSysTextBlock_S *psTxtBlk;
psTxtBlk = CSAdsSys_TakeTextBlock(pASObj, CS_ADSSYS_FLOAT_TEXT, NULL, NULL);
if(psTxtBlk != NULL)
{
	BOOL bExist;
	CSAdsSysLocationDscrpt_S sLocation;
	CSAdsSys_GetLocationDscrpt((CSAdsSysBlock_S *)psTxtBlk, CS_ADSSYS_FLOAT_TEXT, &bExist, &sLocation);
}
CSAdsSys_Return(psTxtBlk);
@endcode
*/
BOOL CSAdsSys_GetLocationDscrpt(CSAdsSysBlock_S* psBlock, BYTE ucTypeIndex,BOOL *pbExsit, CSAdsSysLocationDscrpt_S *psLocationDscrpt);


/**
@brief 同样的block可以对应多个广告位，每个广告位的不同的按键可以关联不同的应用
本函数从block对象中获取ucTypeIndex与wKeyCode指定的应用名称。
通常在Take之后调用
@param psBlock 输入参数，一个数据块
@param ucTypeIndex 输入参数，广告位编号
@asActivationInfoArray 输出参数，应用信息(应用名称分配的内存必须>= 256Bytes)
@pnArraySize 输入输出参数 ，结构体的个数
@retval TRUE操作成功，FALSE失败
@note 客户端代码必须分配256字节来存放应用名称
@code
CSAdsSysImageBlock_S psImgBlk;
psImgBlk = CSAdsSys_TakeImageBlock(pASObj, CS_ADSSYS_CHANNEL_BAR, NULL, NULL);
if(psImgBlk != NULL)
{
	CSAdsSysActivationDscrpt_S asActivationInfoArray[4];
	memset(&asActivationInfoArray,0,sizeof(asActivationInfoArray));
	int pnArraySize = 4;
	CSAdsSys_GetLocationDscrpt((CSAdsSysBlock_S *)psImgBlk, CS_ADSSYS_CHANNEL_BAR,asActivationInfoArray,&pnArraySize);
}
CSAdsSys_Return(psImgBlk);
@endcode
*/
BOOL CSAdsSys_GetActivationDscrpt(CSAdsSysBlock_S* psBlock, BYTE ucTypeIndex, CSAdsSysActivationDscrpt_S *asActivationInfoArray,int *pnArraySize);

/**
@brief 获取block中的Triger描述符,tag 0xb7
@param psBlock 输入参数，一个数据块
@param pbExsit 输出参数，是否存在应用名称
@param psTrigerDscrpt 输出参数，Triger描述符内容
@retval TRUE操作成功，FALSE失败
@code
CSAdsSysImageBlock_S *psImgBlk;
psImgBlk = CSAdsSys_TakeImageBlock(pASObj, CS_ADSSYS_FLOAT_IMAGE1, NULL, NULL);
if(psImgBlk != NULL)
{
	CSAdsSysTrigerDscrpt_S sTrigerDscrpt
	CSAdsSys_GetLocationDscrpt((CSAdsSysBlock_S *)psImgBlk,  &bExist, &sTrigerDscrpt);
}
CSAdsSys_Return(psImgBlk);
@endcode
*/
BOOL CSAdsSys_GetTrigerDscrpt(CSAdsSysBlock_S* psBlock, BOOL *pbExsit, CSAdsSysTrigerDscrpt_S *psTrigerDscrpt);

/**
@brief 归还从广告管理器获取的各种Block对象
@param pBlock 输入参数，调用CSAdsSys_TakeXXXX所获取的图片、文本对象
@retval TRUE操作成功，FALSE失败
@note 每一次调用CSAdsSys_TakeXXXX，都必须对应调用一次CSAdsSys_ReturnBlock，否则会有内存泄漏
*/
BOOL CSAdsSys_ReturnBlock(void *pBlock);

/**
@brief 获取参数数据块中的Descriptors
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param psInfo 输出参数， 参数数据块中的Descriptors 内容
@retval TRUE操作成功，FALSE失败
@note 可在EM_ADSSYS_DESCRIPTOR_LOOP_COMPLETE事件后调用
@see EM_ADSSYS_DESCRIPTOR_LOOP_COMPLETE
@code
case EM_ADSSYS_DESCRIPTOR_LOOP_COMPLETE:
{
	CSAdsSysDescriptorLoop_S sInfo;
	bRet = CSAdsSys_GetDescriptorLoopInfo(psAS, &sInfo);
	if(CSASSERT_FAILED(bRet)) return;

	if(sInfo.bExistAppDispTimeout)
	{
		USPPrint(ADS_DEMO_LEVEL, "descriptor loop,find app disptimeout\n");
	}
	if(sInfo.bExistInvalidation)
	{
		USPPrint(ADS_DEMO_LEVEL, "descriptor loop,find invalidation,DispLocationMask=%xh\n", sInfo.sInvalidation.dwImgDispLocationMask);
	}
	if(sInfo.bExistMainDeliver)
	{
		USPPrint(ADS_DEMO_LEVEL, "descriptor loop,find main deliver,freq=%xh\n", sInfo.sMainDeliver.unDeliver.m_CableDeliver.m_dwFrequency);
	}				
}
break;	
@endcode
 */
 BOOL CSAdsSys_GetDescriptorLoopInfo(void* pASObj, CSAdsSysDescriptorLoop_S *psInfo);

/**
@brief 获取某个block tag对应的需要存储的一系列BlockId
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param ucBlockTag 输入参数，块Tag，可访问常量CS_ADSSYS_TAGS获得
@param psStoredId 输出参数，要存储的BlockId
@retval TRUE操作成功，FALSE失败
@note 在收取到EM_ADSSYS_SCAN_COMPLETE事件后，通过调用该api可以获取需要存储的block
@see EM_ADSSYS_SCAN_COMPLETE
@code
case EM_ADSSYS_SCAN_COMPLETE:
{
	CSAdsSysBlockStoredId_S sStoredId;
	int i;
	CSAdsSys_PrintDebugInfo(psAS);
	bRet = CSAdsSys_GetStoredBlockIds(psAS,  CS_ADSSYS_IMAGE_BLOCK_TAG, &sStoredId);
	if(CSASSERT_FAILED(bRet)) return;

	for(i=0; i<sStoredId.nValidCount; ++i)
	{
		CSAdsSysImageBlock_S *psImgBlk;
		USPPrint(ADS_DEMO_LEVEL, "stored block id=%d, typeidx=%d\n", sStoredId.adwBlockId[i], sStoredId.aucTypeIndex[i]);
		psImgBlk = CSAdsSys_TakeImageBlockById(psAS, sStoredId.adwBlockId[i]);
		if(psImgBlk != NULL)
		{
			///save psImgBlk->pucImageData to flash
		}
		CSAdsSys_ReturnBlock(psImgBlk);
	}		
}
break;
@endcode
 */
BOOL CSAdsSys_GetStoredBlockIds(void* pASObj, BYTE ucBlockTag, CSAdsSysBlockStoredId_S *psStoredId);

#if 0
/**
@brief 根据出发条件(Triger Descriptor)开始监视当前频道的广告位内容。
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param dwTypeMask 输入参数，广告位的掩码，指定需要监视哪些广告位,0停止监视
@retval TRUE-操作成功,FALSE-操作失败
@note 带有Triger描述符的排期，必须调用该API后才能获得数据块，没有Triger描述符
的排期，忽略该函数的调用。
新设置的dwTypeMask将替换上一次设置的广告位，设为0则停止所有广告位的监视。
可以在进入某个界面的时候，根据该界面上有的广告位调用该API，如监视所有
的挂角图片和浮动字幕的代码如下,
@code
CSAdsSys_MonitorTriger(pASObj,  
	(1<<CS_ADSSYS_FLOAT_TEXT) |
	(1<<CS_ADSSYS_FLOAT_IMAGE1) |
	(1<<CS_ADSSYS_FLOAT_IMAGE2) |
	(1<<CS_ADSSYS_FLOAT_IMAGE3) |
	(1<<CS_ADSSYS_FLOAT_IMAGE4));
@endcode
只会监视当前的频道相关的广告位。调用该API后，监视的起始时间被设为当前时间，
被监视的内容马上显示，然后再消失，然后再显示，以此类推。
*/
BOOL CSAdsSys_MonitorTriger(void* pASObj, DWORD dwTypeMask);
#endif

/**
@brief 打印内部调试信息
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@note 广告位打印的是掩码，因为一个数据块可以对应多个广告位
*/
void CSAdsSys_PrintDebugInfo(void* pASObj);

/**
@brief 设置广告最大可分配的内存
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param nMaxSize 广告最大可支持的内存大小
@retval TRUE操作成功，FALSE失败
@note 确保广告初时化成功。如果广告已经使用的内存大于新设置的内存大小，设置将会失败
*/
BOOL CSAdsSys_SetMaxMemSize(void *pASObj, int nMaxSize);

/**
@brief 设置广告检测的触发时间
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param nTime 检测的时间，单位:秒
@retval TRUE操作成功，FALSE失败
@note 确保广告初时化成功。
*/
BOOL CSAdsSys_SetCheckTrigeTime(void *pASObj, BYTE nTime);
	
/**
@brief 获取版本
@retval 版本号
*/
const char* CSAdsSys_GetVersion(void);

void CSAdsSys_SetAdsFilterMask(void *pASObj,  DWORD dwMask);

DWORD CSAdsSys_GetAdsFilterMask(void *pASObj);



/**
@brief 获取某个广告位在某个频道分组中对应的某种类型的广告的所有共享广告数据快的id
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param ucTypeIndex 输入参数, 广告位编号
@param psService 频道信息，传入NULL时会默认为当前节目
@param psShareId 输出参数，记录符合条件的所有共享广告数据快的id
@retval TRUE操作成功，FALSE失败
@note 确保广告初时化成功。
*/
BOOL CSAdsSys_GetShareBlockIds(void *pASObj, BYTE ucTypeIndex, CSAdsSysService_S *psService, CSAdsSysBlockShareId_S* psShareId); 



/**
@brief 获得共享广告的描述信息 tag=0xba
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param psBlock 输入参数广告block原始数据
@param ucTypeIndex 输入参数，广告位编号
@param psService  输入参数，频道信息，传入NULL时会默认为当前节目
@param pbExsit 输出参数, 是否存在该描述
@param psGroupDscrpt 输出参数，记录共享广告的描述信息
@retval TRUE操作成功，FALSE失败
@note 确保广告初时化成功。
*/
BOOL CSAdsSys_GetGroupDscrpt(void *pASObj, CSAdsSysBlock_S *psBlock, BYTE ucTypeIndex, CSAdsSysService_S *psService, BOOL *pbExsit, CSAdsSysGroupDescriptor_S* psGroupDscrpt);


/**
@brief 通过广告的唯一id来获取广告数据
@param pASObj 输入参数，CSAdsSys_Create所创建的的广告管理器
@param dwIdentificationid 输入参数，广告的唯一id，通过广告的唯一描述子获得
@retval 一个图片块对象，如果为NULL表示没有
@note 使用完毕后，必须马上调用一次CSAdsSys_ReturnBlock归还该对象

*/
CSAdsSysImageBlock_S*  CSAdsSys_TakeImageBlockByIdentificationId(void* pASObj, DWORD dwIdentificationId);


#ifdef __cplusplus
}
#endif

#endif

/**
@}
*/
