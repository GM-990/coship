/**@defgroup booter显示信息接口
Booter显示广告图片\n
	1. 默认广告写入到index=0的位置，在出厂前烧录好。\n
	2. 中间件 需要保存一个booter显示广告时:\n
		a 调用CSUDIBOOTSHOWGetCapability 获取驱动booter的能力(支持哪些图片解码格式?)，中间件将图片转码成booter所支持的图片编码格式\n
		b 调用CSUDIBOOTSHOWGetFreeSize 获取booter剩余可用空间大小，判定空间是否足够:\n
			可存储图片大小 = 剩余空间大小 - 存储每个Boot图片所需存储的图片信息头部大小\n
		c 调用CSUDIBOOTSHOWSetShowUnitInfo 接口保存booter显示图片。\n
 	3. 中间件需要修改某个已存储单元格的显示属性时:\n
 		a 调用CSUDIBOOTSHOWGetPicCount 和CSUDIBOOTSHOWGetShowUnitInfo接口遍历已存储的存储图片，查找需修改的nPicId。\n
 		b 调用CSUDIBOOTSHOWSetNeedShow 接口修改该存储单元的显示属性(显示or 不显示)

booter可支持多种形式的信息显示，比如可以支持显示开机图片，开机播放视频等等。可以记录多个显示单元的信息\n
显示单元 = 显示单元信息头 + 显示单元数据区
每条显示单元必须有信息头区，可选有数据区。对于图片，视频文件的显示，必须有数据区，对于直播视频，则可以不要显示数据区。\n
显示数据的存放对用户是透明的，其内部存放什么格式，什么地址，只需要由驱动或booter负责。应用只需要调用CSUDIBOOTSHOWSetShowUnitInfo设置就可以了。

booter可支持多个显示单元，但只能设置一个开机显示。如果设置了多个，则以index最小的单元为准。

@version 2.0.2 2011/12/21 增加了错误代码枚举定义
@version 2.0.1 2011/06/21 初稿

@{
*/
#ifndef _UDI2_BOOTSHOW_H_
#define _UDI2_BOOTSHOW_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_tuner.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 返回错误代码枚举定义*/
enum {
	CSUDIBOOTSHOW_ERROR_BAD_PARAMETER = CSUDI_BOOTSHOW_ERROR_BASE,	///< 参数异常错误 
	CSUDIBOOTSHOW_ERROR_FEATURE_NOT_SUPPORTED,						///< 不支持此操作 
	CSUDIBOOTSHOW_ERROR_INVALID_UNIT_ID,								///< 非法单元ID
	CSUDIBOOTSHOW_ERROR_UNKNOWN_ERROR								///< 未知错误
};

/**@brief Booter 显示单元格式宏 */
#define CSUDIBOOTSHOW_UNITTYPE_BMPBITS   	(0x01)//点阵数组,	CSUDIBOOTSHOWUnitInfo中uDetailInfo为CSUDIBOOTSHOWPicInfo类型
#define CSUDIBOOTSHOW_UNITTYPE_BMP    	(0x02)//bmp图片，	CSUDIBOOTSHOWUnitInfo中uDetailInfo为CSUDIBOOTSHOWPicInfo类型
#define CSUDIBOOTSHOW_UNITTYPE_PNG    	(0x04)//png图片，CSUDIBOOTSHOWUnitInfo中uDetailInfo无意义
#define CSUDIBOOTSHOW_UNITTYPE_JPG    	(0x08)//jpg图片，CSUDIBOOTSHOWUnitInfo中uDetailInfo无意义
#define CSUDIBOOTSHOW_UNITTYPE_MPEG    	(0x10)//mpg图片，CSUDIBOOTSHOWUnitInfo中uDetailInfo无意义
#define CSUDIBOOTSHOW_UNITTYPE_SIGALGIF   (0x20)//gif图片，CSUDIBOOTSHOWUnitInfo中uDetailInfo无意义
#define CSUDIBOOTSHOW_UNITTYPE_BMPZIPS   	(0x40)//压缩点阵数组，CSUDIBOOTSHOWUnitInfo中uDetailInfo为CSUDIBOOTSHOWPicInfo类型
#define CSUDIBOOTSHOW_UNITTYPE_DVBLIVE   	(0x0100)//直播视频，CSUDIBOOTSHOWUnitInfo中uDetailInfo为CSUDIBOOTSHOWDvbLiveInfo类型
#define CSUDIBOOTSHOW_UNITTYPE_TSFILE		(0x0200)//ts视频文件，CSUDIBOOTSHOWUnitInfo中uDetailInfo为CSUDIBOOTSHOWAVInfo类型

/**@brief 驱动Booter在显示方面的能力 */
typedef struct {
	unsigned int nBootDecodeCapablity; 				///<  Boot支持的解码模式，范围为CSUDIBOOTSHOW_UNITTYPE_BMPBITS至CSUDIBOOTSHOW_UNITTYPE_TSFILE， 支持范围内数据的与运算
	unsigned int nBooterShowFlashTotalSize;///<  用于存储Boot显示的flash空间总大小，单位为BYTE
	unsigned int nBooterUnitHeadSize;		///<  存储每个Boot显示单元所需存储的显示单元信息头部大小，单位为BYTE
}CSUDIBOOTSHOWCapability;

/**@brief 节目音视频信息*/
typedef struct {
	unsigned int nVideoPid;							///<  节目视频pid
	CSUDIVIDStreamType_E eVideoType;		///<	节目视频格式
	unsigned int nAudioPid;							///<  节目音频pid
	CSUDIAUDStreamType_E eAudioType;		///<	节目音频格式
}CSUDIBOOTSHOWAVInfo;

/**@brief 直播节目信息*/
typedef struct {
	CSUDITunerType_E eType;							///<  tuner类型
	CSUDITunerSRCDeliver_U uDeliver;		///<  频点信息
	CSUDIBOOTSHOWAVInfo sAvInfo;			///<  节目音视频信息
}CSUDIBOOTSHOWDvbLiveInfo;

/**@brief 图片信息*/
typedef struct {
	unsigned int nWidth;   							///<  图片的宽度
	unsigned int nHigth;   							///<  图片的高度
	unsigned int nPitch;   							///<  图片的一行占用的字节数
}CSUDIBOOTSHOWPicInfo;

/**@brief 显示单元的信息
	@note 显示单元=显示单元信息头+显示单元数据。 显示单元数据对用户是透明的，放在哪里由驱动或booter维护*/
typedef struct {
	CSUDI_BOOL bNeedShow;							///<  是否需要显示
	unsigned int nUnitType;						///<  显示单元的数据格式
	unsigned int nDataSize;						///<  显示单元的数据大小
	unsigned int nUnitId;							///<  显示单元唯一标识id，用于应用上唯一确定一个显示单元。应用上一般不会记录index，用此id来判断此单元是否自己关注的单元。
	union{
		CSUDIBOOTSHOWDvbLiveInfo sDvbLiveInfo;	///< 当nUnitType==CSUDIBOOTSHOW_UNITTYPE_DVBLIVE时，详细信息为CSUDIBOOTSHOWDvbLiveInfo
		CSUDIBOOTSHOWPicInfo sPicInfo;					///< 当nUnitType为CSUDIBOOTSHOW_UNITTYPE_BMPBITS、CSUDIBOOTSHOW_UNITTYPE_BMP、CSUDIBOOTSHOW_UNITTYPE_BMPZIPS时，详细信息为CSUDIBOOTSHOWPicInfo
		CSUDIBOOTSHOWAVInfo sAvInfo;						///< 当nUnitType==CSUDIBOOTSHOW_UNITTYPE_TSFILE时,详细信息为CSUDIBOOTSHOWAVInfo
	}uDetailInfo;											///< 显示单元的详细信息，具体格式根据显示单元格式定义
}CSUDIBOOTSHOWUnitInfo;

/**
@brief 获取booter的显示相关的能力
@param[out] psCapability 获取booter的显示能力，详见结构体定义
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 详见结构体CSUDIBOOTSHOWCapability定义
@note 该接口根据项目需要可选实现
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetCapability(CSUDIBOOTSHOWCapability* psOutCapability);

/**
@brief 获得booter显示用到的已存储的每个显示单元的信息
@param[in] nIndex 存储显示单元的索引（取值为0--已存储显示单元个数-1）
@param[out] psOutInfo 显示单元信息
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetShowUnitInfo(int nIndex,CSUDIBOOTSHOWUnitInfo* psOutInfo);

/**
@brief 获取booter用于存储可显示单元数据的flash剩余空间大小
@param[out] pnOutSize 剩余空间大小，单位为BYTE
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 可存储显示数据大小 = 剩余空间大小 - 1个显示单元信息头部大小
@note 剩余空间为flash地址对齐后的剩余可用空间大小。
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetFreeSize(unsigned int* pnOutSize);

/**
@brief 获取boot已存储开机画面显示单元个数
@param[out] pnOutCount 已存储的显示单元个数
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetUnitCount(unsigned int* pnOutCount);

/**
@brief 设置booter显示某个指定的已存储开机显示单元
@param[in] nIndex 显示单元的索引（取值为大于等于0且小于已存储显示单元个数）
@param[in] bNeedShow 是否需要显示
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 设置某个显示单元显示后，其他的所有显示单元均不显示，开机只显示一个显示单元
@note 如果对多个单元均调用了该接口，则以index最小的单元为准，即其他单元及时设置了显示也是无效的
*/
CSUDI_Error_Code CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow);

/**
@brief 设置booter显示用到的每个显示单元的信息
@param[in] nIndex 显示单元的索引（取值为大于等于0且小于等于已存储显示单元个数）
@param[in] psInfo 显示单元信息
@param[in] pData  显示单元信息对应的数据区的起始地址
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 如果索引==已存储显示单元个数，则表示新增一个显示单元。
@note 如果修改后的显示单元大小与修改前的不一样，则需要进行flash整理重新分配等操作。
@note 某些显示单元的内容可能是固定的，不允许修改。此时，调用CSUDIBOOTSHOWSetShowUnitInfo返回错误。
*/
CSUDI_Error_Code CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo,void* pData);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



