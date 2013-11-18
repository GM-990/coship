/** @addtogroup CSFLASH
@{
*/

#ifndef _CSFLASH_PLUS_H_
#define _CSFLASH_PLUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include  "udiplus_typedef.h"
#include  "cs_hdicommon_plus.h"
typedef struct 
{
	char m_szPTName[32];               /**< 分区名字 */
    	unsigned int m_uoffset;        /**<Flash的物理偏移，从0开始*/
	unsigned int m_uPTSize;            /**< 物理分区大小，包括spare_size的大小，单位为BYTE */
	unsigned int m_uSpareSize;         /**< 分区冗余空间的大小，单位为BYTE*/
} CSFlashPartition_S;


typedef enum
{
	EM_FLASH_GOOD_BLOCK,    /**< 好块*/
	EM_FLASH_BAD_BLOCK       /**< 坏块*/
}CSNandFLS_BLK_Status_E;

/**
@brief FLASH API 初始化。

@param[in] bVPPGPIOIndex 硬件保护电路GPIO口索引号。通过赋值正确的索引号，以控制硬件保护电路，来实现硬件写保护。
@return CSHDI_SUCCESS 表示初始化成功，CSHDI_FAILURE表示初始化失败。
@note  此接口默认注册了全部CFI三种命令集，即AMD命令集，SST命令集，Intel命令集，相当于CSFLSRegisterAMDCmdSet()+CSFLSRegisterSSTCmdSet()+CSFLSRegisterInterCmdSet()+CSFLSInitializeCFI
*/
CSHDI_Error_t CSFLSInitialize(BYTE bVPPGPIOIndex);

/**
@brief CSFLSSetFLSCount():Config Used Flash Chip Count	

@param[in] nCurFLSCount 指定系统应用中用到的FLASH片数。
@return  CSHDI_SUCCESS表示设置参数成功，CSHDI_FAILURE表示设置参数失败
@note 
*/					 							    
CSHDI_Error_t CSFLSSetFLSCount(int nCurFLSCount) ;

/**
@brief 设置指定FLASH的地址空间。 

@param[in] nFlashIndex 当前FLASH序号（从０开始）边界： nFlashIndex >= 0 && nFlashIndex < 配置的flash片数
@param[in] dwBaseAddress 当前FLASH的基地址边界：必须是偶地址
@param[in] dwBufferSize 当前FLASH的空间大小（单位：BYTE）边界：dwBufferSize <= 32*1024*1024;
@return CSHDI_SUCCESS表示设置参数成功，CSHDI_FAILURE表示设置参数失败
*/
CSHDI_Error_t CSFLSSetSpace(int nFlashIndex, DWORD dwBaseAddress, DWORD dwBufferSize);

/**
@brief 用于Flash软件写保护。成功调用此函数之后，禁止对相应的扇区进行写操作。

@param[in] nChipNo 待上锁的Flash芯片号。nChipNo为0时表示第1块Flash。
@param[in] nBlockNo 待上锁芯片的扇区号。nBlockNo为0时表示第1个扇区。
@return CSHDI_SUCCESS 表示上锁操作成功，CSHDI_FAILURE表示上锁操作失败。
*/								 	    
CSHDI_Error_t CSFLSBlockLock(int nChipNo, int nBlockNo);

/**
@brief 用于Flash软件写保护。成功调用此函数之后，允许对相应的扇区进行写操作。

@param[in] nChipNo 待解锁的Flash芯片号。nChipNo为0时表示第1块Flash。
@param[in] nBlockNo 待解锁芯片的扇区号。nBlockNo为0时表示第1个扇区。
@return CSHDI_SUCCESS 表示解锁操作成功，CSHDI_FAILURE表示解锁操作失败。
*/
CSHDI_Error_t CSFLSBlockUnLock(int nChipNo, int nBlockNo);

/**
@brief  同CSFLSBlockLock，CSFLSBlockUnLock一样，用于Flash软件写保护。只是一旦成功调用此函数之后，相应扇区的状态变为blocked -down状态，并且该状态改变不能仅由软件来实现：而只可通过复位或断电操作还原为初始状态，即非blocked -down状态。Blocked -down状态下，调用函数CSFLSBlockLock和CSFLSBlockUnLock不能改变相应扇区的上锁或解锁的状态。

@param[in] nChipNo 芯片号。nChipNo为0时表示第1块Flash。
@param[in] nBlockNo 对应Flash芯片的扇区号。nBlockNo为0时表示第1个扇区。
@return CSHDI_SUCCESS 表示函数调用成功，CSHDI_FAILURE表示函数调用失败。
@note 一旦成功调用此函数之后，相应扇区的状态变为blocked -down状态，并且要改变该状态不能仅由软件来实现：而只可通过复位或断电操作还原为初始状态，即非blocked -down状态。Blocked -down状态下，调用函数CSFLSBlockLock和CSFLSBlockUnLock不能改变相应扇区的上锁或解锁的状态。就是说，Blocked -down状态下，某块Flash芯片相应扇区保持上锁或解锁状态。
*/
CSHDI_Error_t CSFLSBlockLockDown(int nChipNo, int nBlockNo);

/**
@brief 为了提高存储数据的安全性，防止被意外改写，部分flash型号支持该只写一次的功能。对某一区域成功使用了该只写一次的功能之后，相应区域数据不再变化，就再也不能对该区域成功执行擦除或改写操作了。对该函数、OTP相关功能、待OTP区域及测试或使用方法有清晰认识之前，请慎重调用此函数，否则很容易报废Flash。该函数只有需要OTP功能的项目才可调用，做应用集成时，应用工程师必须对Flash的扇区结构有清醒的认识，以免出现事故。

@param[in] dwStartAddr 待OTP空间的起始地址（绝对地址），此起始地址地址必须恰好为某个扇区的首地址；否则的话，驱动对这个地址值进行判断结果为非法输入地址，以确保安全性。
@param[in] dwOTPSize 待OTP的区域的大小，单位为BYTE。该区域大小必须恰好为OTP的扇区大小之和，并且要符合使用的相应Flash OTP扇区分组的概念。对于不同类型的Flash，由于OTP的扇区分组不同。具体请参考相应的Datasheet. 驱动会对这个输入值（dwOTPSize）进行合法性判断，如果不符合Remarks中描述的要求，就会返回错误，以确保安全性。
@return CSHDI_SUCCESS 表示写操作成功，CSHDI_FAILURE表示写操作失败。 
@note
- 1,对于ST M28W320FS、M28W640FS型号的Flash,其8(*8K Bytes)个Parameter Block和靠近它的一个Main Block可分别单独进行设置；其他Main Block必须以组为单位进行OTP, 其每组为两个main Block,即128Kbytes.
- 2,对于ST M58LW032D型号的Flash,其每个扇区都为128K Bytes,每个扇区都可以单独进行OTP操作；
- 3，对于Spansion S29GL064M11FAIS1、S29GL064M11FAIS2型号的Flash,其8(*8K Bytes)个Parameter Block和靠近它的一个Main Block分为一组，通过Permanent Lock操作进行真正意义上的OTP，更进一步地说，该9个扇区被一次性OTP掉，不会象ST M28WXXFS系列的那样，可以一个一个地进行操作；靠近上述9个扇区的两个扇区即128K Bytes的区域分为一组进行OTP；其他的扇区都以4个扇区即256Kbytes为一个扇区组进行OTP。
- 4，对于Top型的，如果有Parameter Block，则这些Block为最顶端的几个Block；对于Bottom型的，如果有Parameter Block，则这些Block为最低端的几个Block；比如M28W320FS分为M28W320FST、M28W320FSB两种。对于M28W320FST，是Top型，有8个Parameter Block，为最后8个扇区；对于M28W320FSB，是Bottom型，有8个Parameter Block，为第零到第七这八个扇区。S29GL064M11FAIS1是Top型，有8个Parameter Block，为最后8个扇区；S29GL064M11FAIS2是Bottom型，有8个Parameter Block，为第零到第七这八个扇区。
- 5, 对于S29GL128N10FFIS1，每个扇区都可以单独OTP，每个扇区128KB；   
*/
CSHDI_Error_t CSFLSSetBlockToOtp(DWORD dwStartAddr, DWORD dwOTPSize);

/**
@brief 获得初始化成功后FLASH的基地址和剩余空间

@param[in] nChipIndex FLASH芯片索引值，一般为0或1
@param[out] dwBaseAddress 初始化成功后FLASH基地址指针
@param[out] dwSpaceSize 初始化成功后FLASH的剩余空间
@return 如果成功，则返回CSHDI_SUCCESS，否则返回CSHDI_FAILURE
*/
CSHDI_Error_t CSFLSGetFlashInfo (int nChipIndex, DWORD *dwBaseAddress, DWORD *dwSpaceSize);

/**
@brief CSFLSGetVersion():用于获取当前Flash所用库的版本号。

@param[in] nSize The size of Version Buffer. Must be less than 256.
@param[out] pchVer The top address of Version Buffer.
@return 
- 1>If [pchVer] is NULL,return the actual length of  version information,including "\0"
- 2>If [pchVer] is not NULL,and the call is success,
   return the length of version information returned.
- 3>If the call is failure,return 0.   
*/
int CSFLSGetVersion( PCHAR pchVer, int nSize );


/**
@brief 注册AMD命令集的接口，调用CSFLSInitializeCFI之前请先调用

@return 如果成功，则返回TRUE，否则返回FALSE
*/
BOOL CSFLSRegisterAMDCmdSet(void);

/**
@brief 注册SST命令集的接口，调用CSFLSInitializeCFI之前请先调用

@return 如果成功，则返回TRUE，否则返回FALSE
*/
BOOL CSFLSRegisterSSTCmdSet(void);

/**
@brief 注册Inter命令集的接口，调用CSFLSInitializeCFI之前请先调用

@return 如果成功，则返回TRUE，否则返回FALSE
*/
BOOL CSFLSRegisterInterCmdSet(void);

/**
@brief 注册28系列OTP命令集的接口，调用CSFLSInitializeCFI之前请先调用

@return 如果成功，则返回TRUE，否则返回FALSE
*/
BOOL CSFLSRegInter28SeriesOtp(void);

/**
@brief 注册29系列OTP命令集的接口，调用CSFLSInitializeCFI之前请先调用

@return 如果成功，则返回TRUE，否则返回FALSE
*/
BOOL CSFLSRegAMD29SeriesOtp(void);

/**
@brief FLASH API 初始化,主要是针对特定一种命令集的初始化接口,在CSFLSRegisterAMDCmdSet(),CSFLSRegisterSSTCmdSet(),CSFLSRegisterInterCmdSet()之后调用

@param[in] bVPPGPIOIndex 硬件保护电路GPIO口索引号。通过赋值正确的索引号，以控制硬件保护电路，来实现硬件写保护。
@return CSHDI_SUCCESS 表示初始化成功，CSHDI_FAILURE表示初始化失败。
@note 此接口在采用了基于CFI的驱动后新增加的，即CS_FLASH_T_02.00.00后才有
*/
CSHDI_Error_t CSFLSInitializeCFI(BYTE bVPPGPIOIndex);


/**
@brief Flash分区设置，该接口必须在初始化之后调用

@param[in] pPartitionInfo: flash分区设置
@param[in] nPartitionCnt: flash分区个数
@return CSHDI_SUCCESS 表示设置成功，CSHDI_FAILURE表示设置失败。
@note 此接口目前只用于Nand flash,可重复调用，每次调用后，flash中的数据是不确定的
@note 要求分区的划分是顺序的，地址是连续的
*/
CSHDI_Error_t CSFLSSetPartition(CSFlashPartition_S *pPartitionInfo, int nPartitionCnt);


/**
@brief 获取Flash分区设置，必须在设置分区后调用

@param[in] pPartitionInfo: flash分区设置存放地址
@param[in] nPartitionCnt: 分区buffer大小，单位为sizeof(CSFlashPartition_S)
@param[out]pnActualCnt:实际分区大小
@return CSHDI_SUCCESS 表示获取成功，CSHDI_FAILURE表示获取失败。
@note 此接口目前只用于Nand flash
*/
CSHDI_Error_t CSFLSGetPartition(CSFlashPartition_S *pPartitionInfo, int nPartitionCnt, int *pnActualCnt);



/**
@brief 把block标记为坏块或好块

@param[in] unBlkNum: block的编号
@param[in] eBlkStatus:block的状态
@return CSHDI_SUCCESS 表示设置成功，CSHDI_FAILURE表示设置失败。
@note 此接口为测试接口，目前只用于Nand flash的测试
*/
CSHDI_Error_t CSNandFLSSetBlockStatus(unsigned int unBlkNum, CSNandFLS_BLK_Status_E eBlkStatus);



/**
@brief 获取block状态

@param[in] unBlkNum: block的编号
@param[out] peBlkStatus:block的状态存放地址
@return CSHDI_SUCCESS 表示获取成功，CSHDI_FAILURE表示获取失败。
@note 此接口为测试接口，目前只用于Nand flash的测试
*/
CSHDI_Error_t CSNandFLSGetBlockStatus(unsigned int unBlkNum, CSNandFLS_BLK_Status_E *peBlkStatus);



/**
@brief 从FLASH物理地址中读取保存的数据 

@param[in] dwStartAddress 存储数据的FLASH空间的物理地址
@param[in] pbValue 保存读出数据的内存空间地址
@param[in] dwLength 要读取的数据长度(以字节为单位)
@return CSHDI_SUCCESS表示读取成功，CSHDI_FAILURE表示读取失败
*/
CSHDI_Error_t CSFLSReadFlashPhysics (DWORD dwStartAddress, PBYTE pbValue, DWORD dwLength);
#ifdef __cplusplus
}
#endif

/** @} */

#endif
