#ifndef CSSHELL_GCABLECA_H
#define CSSHELL_GCABLECA_H

#include "csshell_ca.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define G_MAX_OPERATOR_NUM       (4)         /*最多的运营商个数*/
#define G_MAX_SLOT_NUM       (20)   /*卡存储的最大钱包数*/
#define G_MAX_OPERATORINFO_LEN (20)
typedef struct _G_CaEntitle
{
	DWORD 	m_dwProductID;
	char 	m_acProductName[32];
	char 	m_cStartTime[64];
	char 	m_cEndTime[64];
} G_CaEntitle_S;

typedef struct _CaOperatorInfo_S
{
	WORD m_dwOperatorid;
	BYTE	m_acOperatorInfo[G_MAX_OPERATORINFO_LEN+1];   /*运营商私有信息*/	
}G_CaOperatorInfo_S;

typedef struct _CaOperatorIds_S               /*运营商ID*/
{
	WORD	m_wOperatorIdNum;
	G_CaOperatorInfo_S m_stOperatorsInfo[G_MAX_OPERATOR_NUM];
} G_CaOperatorIds_S;

typedef struct _CaWallet_S               /*钱包*/
{
	WORD   m_nID;
	WORD	m_nBalance;
	WORD	m_nRemainder;
} G_CaWallet_S;

typedef struct _CaSlotId_S          /*钱包ID*/
{
	WORD	m_wSlotNum;
	G_CaWallet_S m_acSlotIds[G_MAX_SLOT_NUM];
}G_CaSlotId_S;

typedef struct _CaViewedIPP_S
{
	int m_nOperatorId;
	char m_acProductName[64];
	char m_acStartTime[64];
	char m_acEndTime[64];
    int m_nBookedPrice;
	int m_nBookedPriceType;
}G_CaIPPViewedInfo_S;

typedef struct _CaIPPInfo_S
{
	int m_nOperatorId;
	int m_nWalletId;
	int m_nProductId;	
	int m_nType; //ippv,ippt
	int m_nEcmPid;	
	int m_nPriceNum;
	int m_nPrice[4]; //价格相关属性。
	char m_acProductName[32];
	char m_acStartTime[32];
	char m_acEndTime[64];
}G_CaIPPInfo_S;


typedef struct 
{
	int m_nSize;	///<本结构大小，一定等于sizeof(CSShellGCableCAInitParam_S)
	
	/*	
	@brief 获取当前CA名称，用以区分CA厂商
	@param[out] pcCAName 存储ca厂商的名称
	@param[in/out] pnLen 传入参数时表示pcCAName指向的buffer长度，做传出参数是是 pcCAName的实际长度
	@return 成功返回0，失败-1
	*/
	int (* m_fnGCableCAGetName)(char* pcCAName, int *pnLen);

	/*	
	@brief 获取当前的智能卡卡号
	@param[out] pcCardId 存储获取到的智能卡卡号
	@param[in/out] pnLen 传入参数时表示pcCardId指向的buffer长度，做传出参数是是 pcCardId中卡号的实际长度
	@return 成功返回0，失败-1
	*/
	int (* m_fnGCableCAGetIcNo)(char* pcCardId, int *pnLen);

	/**
	@brief获取当前智能卡PIN码是否被锁定
	@return  1为智能卡被锁定，0为智能卡未锁定，-1为该终端的CA系统不支持PIN码锁定功能.
	*/
	int (* m_fnGCableCAPinLocked)();

	/**
	@brief 获取当前CA版本号
	@param[out] pcVersion 当前ca的版本号
	@param[in/out] pnLen 传入参数时表示pcVersion指向的buffer长度，做传出参数是是 pcVersion的实际长度
	@return  成功返回0，失败-1
	*/
	int (* m_fnGCableCAVersion)(char* pcVersion, int *pnLen);

	/**
	@brief 获取智能卡中的区域代码（取区域代码中的低16位）
	@return  智能卡中的区域码,返回0表示区域码无效
	*/
	int (* m_fnGCableCAGetRegionCode)();

	/**
	@brief 修改PIN码
	@param[in] nPinType: PIN码类型. 0为父母控制PIN（如设置观看时间和级别）；1为钱包管理PIN，如IPP购买时输入的PIN）；-1为该终端的CA系统不支持多种类型的PIN码。
	@param[in] nOldPin: 旧PIN码
	@param[in] nNewPin: 新PIN码
	@return  成功返回0，失败-1
	*/
	int  (* m_fnGCableCAChangePin)(int nPinType, int nOldPin,  int nNewPin);

	/**
	@brief 修改智能卡观看级别
	@param[in] dwPin: 智能卡PIN码，仅能是父母控制类型的PIN码
	@param[in] cRatingValue: 智能卡观看级别，范围3~18 
	@return  0表示成功，其他表示失败，-1表示不支持。
	*/
	int  (* m_fnGCableCASetRating)(DWORD dwPin, BYTE cRatingValue);

	/**
	@brief 获取当前智能卡观看级别 
	@return  大于等于0：获取成功返回的观看级别（范围一般为3-18）；-1为该终端的CA系统不支持;-2为获取失败
	*/
	int (* m_fnGCableCAGetRating)();

	/**
	@brief 修改智能卡观看级别
	@param[in] dwPin: 智能卡PIN码.
	@param[in] pcWorkTime: 智能卡观看时段.格式为"hh:mm-hh:mm"，如"07:30-22:59"表示从早上7点30分至晚上22点59分
	@param[in] nLen:输入参数pcWorkTime的长度
	@return   0表示成功，其他表示失败，-1表示不支持。
	*/
	int  (* m_fnGCableCASetWorkTime)(DWORD dwPin,  char*pcWorkTime, int nLen);

	/**
	@brief 获取当前智能卡观看时段
	@param[out] pcWorkTime: 智能卡观看时段.格式为"hh:mm-hh:mm" 如"07:30-22:59"，表示从早上7点30分至晚上22点59分
	@param[in/out] pnLen 传入参数时表示pcCardId指向的buffer长度，做传出参数是是 pcCardId中卡号的实际长度
	@return   0表示成功，其他表示失败，-1表示不支持。
	@note 
	*/
	int (* m_fnGCableCAGetWorkTime)(char *pcWorkTime, int* pnLen);

	/**
	@brief 取得所有运营商信息
	@param[out] pstOperatorId: 运营商信息
	@return  0表示成功，其他表示失败，-1表示不支持。
	@note
	*/
	int (* m_fnGCableCAGetOperators)(G_CaOperatorIds_S * pstOperatorId);

	/**
	@brief  获取指定运营商的所有钱包
	@param[in] wOperatorsId:指定运营商的ID。
	@param[out] pstSlotinfo: 钱包信息。
	@return  0表示成功，其他表示失败
	*/
	int (* m_fnGCableCAGetWallets)(WORD wOperatorsId,  G_CaSlotId_S * pstSlotinfo);

	int (* m_fnGCableCAGetEntitlesCount)(WORD wOperatorsId);

	/**
	@brief  根据运营商ID取得所有普通授权节目对象
	@param[in] nEntitleIndex:指定运营商的ID。
	@param[out] pstEntitleinfo: 授权信息。
	@return 0表示成功，其他表示失败
	*/
	int (* m_fnGCableCAGetEntitle)(int nEntitleIndex, G_CaEntitle_S * pstEntitleinfo);

	int (* m_fnGCableCAGetViewdIPPCount)();

	/**
	@brief  获取已观看过的Ipp节目
	@param[out] pstViewedIPP: 已经观看过的ipp节目。
	@return  0表示成功，其他表示失败
	*/
	int (* m_fnGCableCAGetViewdIPP)(int nIndex, G_CaIPPViewedInfo_S *pstViewedIPP);


	/**
	@brief  获取临时弹出请求购买的IPP节目
	@param[out] pstIPP: 获取Ipp信息
	@return 0表示成功，其他表示失败
	*/
	int (* m_fnGCableCAGetInquireIPP)(G_CaIPPInfo_S *pstIPP);

	/**
	@brief  获取指定类型的价格
	@param[int] nType: 使用此值指定购买的价格类型.0  不回传、能录像的价格;1  不回传、不可录像价格;2  要回传、能录像价格;3  要回传、不能录像价格
	@param[int] pstCaIPPInfo:ipp信息
	@return  0表示成功，其他表示失败
	*/
	int (* m_fnGCableCAIppGetPrice)(G_CaIPPInfo_S *pstIPP, int nType);

	/**
	@brief 订购当前的IPP节目
	@param[int] pcPin: 智能卡Pin码
	@param[int] nPriceType: 订购的价格类型
	@param[int] pstCaIPPInfo:ipp信息
	@return  成功返回0，已经被订购过返回1，失败返回负值（见错误值定义）。 
	@note 返回 0能录像的价格;1不可录像价格;2 如果该终端集成的CA系统不支持多种价格类型，则传该值
	*/
	int (* m_fnGCableCAIppBook)(G_CaIPPInfo_S *pstIPP, DWORD dwPin, int nPriceType);

	/**
	@brief  拒绝临时弹出的IPP节目购买请求
	@return  0表示成功，其他表示失败
	*/
	int (* m_fnGCableCACancelInquireIPP)();	

	/**
	@brief  子母卡功能, 当前插入是母卡时调用，读取母卡中需要喂养子卡的数据
	@param[int]:  运营商ID
	@param[out]:  母卡中需要喂养子卡的数据(传入时buffer大小至少分配256字节)
	@param[out]:  母卡中需要喂养子卡的数据的实际长度
	@return  0表示成功，其他表示失败
	*/
	int (*m_fnReadFeedDataFromParent)(WORD wOperatorId, BYTE* pcData, BYTE *pbLen);

	/**
	@brief  将子母卡配对信息写入子卡
	@param[int]:  要写入子卡的数据buffer
	@param[int]:  要写入子卡的数据实际长度
	@return  0表示成功，其他表示失败
	*/
	int (*m_fnWriteFeedDataToChild)(WORD wOperatorId, BYTE* pcData, BYTE bLen);

	/**
	@brief  得到母卡卡号
	@param[out]:  要写入母卡卡号buffer；0 表示当前为母卡；不为0 表示当前卡为子卡，返回值为子卡对应的母卡卡号
	@param[out]:  要写入母卡卡号的实际长度
	@return  0表示成功，其他表示失败
	*/
	int (*m_fnGetMotherInfo)(BYTE* pcMotherInfo, int *pnLen);

	/**
	@brief  机顶盒与卡配对状态
	@return 
	@ 0 表示智能卡对应当前机顶盒
	@ 1 表示无智能卡或无效卡
	@ 2 表示智能卡没有对应任何机顶盒
	@ 3 表示智能卡已对应别的机顶盒
	@ -1 表示此接口调用失败
	*/
	int (*m_fnIsPaired)();

	/**
	@brief  得到机顶盒bouquetId
	@return 
	@ 0 表示bouquetId获取失败
	*/
	int (*m_fnGCableCAGetBouquetID)();
	
	/**
	@brief	得到机顶盒的默认bouquetId(在m_fnGCableCAGetBouquetID 方法获取异常的时候调用)
	@return 
	*/

	int (*m_fnGCableCAGetDefaultBouquetID)();

	/**
	@brief	设置是否启动bouquetID监听的标志
	@return 
	*/

	int (*m_fnGCableCASetMonitorFlag)(BOOL bNeedMonitor);
	
}CSShellGCableCAInitParam_S;

/**
@brief 设置 CA 接口参数
@param psParam  CA  Shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSShellGCableCASetParam(CSShellGCableCAInitParam_S * psParam);

/**
@brief 获取 CA 接口参数
@param 无
@成功返回CSShellGCableInitParam_S,失败返回NULL
*/
const CSShellGCableCAInitParam_S* CSShellGCableCAgetParam();

#ifdef __cplusplus
}
#endif

#endif

