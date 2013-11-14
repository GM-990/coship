/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
		This module is used for the production test application. 
-------------------------------------------------------------------------------
*******************************************************************************/

/** @addtogroup CSFLASH
@{
*/

#ifndef _CS_FLASH2_PLUS_H_
#define _CS_FLASH2_PLUS_H_


#ifdef __cplusplus
extern "C" {
#endif

#include  "udiplus_typedef.h"
#include  "cs_hdicommon_plus.h"

typedef enum
{
	EM_FLASH2_GOOD_BLOCK,    /**< �ÿ�*/
	EM_FLASH2_BAD_BLOCK       /**< ����*/
}CSFLS2NandBlockStatus_E;

#define CSFLS2_MAX_PT_NAME_LEN (32)
typedef struct 
{
	char m_szPTName[CSFLS2_MAX_PT_NAME_LEN];               /**< �������� */
    unsigned int m_uOffset;        /**<Flash������ƫ�ƣ���0��ʼ*/
	unsigned int m_uPTSize;            /**< ���������С������spare_size�Ĵ�С����λΪBYTE */
	unsigned int m_uSpareSize;         /**< ��������ռ�Ĵ�С����λΪBYTE*/
} CSFLS2Partition_S;

#define CSFLS2_MAX_NAND_PARTITION_CNT (64)

typedef struct 
{
	int m_nVPPGPIOIndex; /**< Ӳ��������·GPIO�������� */
	DWORD m_dwBaseAddress; /**< FLASH�Ļ���ַ�߽磺������ż��ַ*/
	DWORD m_dwChipSize; /**< FLASH������ռ��С */
	DWORD m_dwReserved0; /**< ��������0 */
	DWORD m_dwReserved1; /**< ��������1 */
}CSFLS2InitParams_S;

typedef struct 
{
	DWORD m_dwBaseAddress; /**< FLASH�Ļ���ַ�߽磺������ż��ַ*/
	DWORD m_dwChipSize; /**< FLASH������ռ��С */
}CSFLS2FlashInfo_S;

/**
@brief ע��AMD����Ľӿڣ�����CSFLS2InitNor֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLS2RegisterAMDCmdSet(void);

/**
@brief ע��SST����Ľӿڣ�����CSFLS2InitNor֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLS2RegisterSSTCmdSet(void);

/**
@brief ע��Inter����Ľӿڣ�����CSFLS2InitNor֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLS2RegisterInterCmdSet(void);

/**
@brief ע��28ϵ��OTP����Ľӿڣ�����CSFLS2InitNor֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLS2RegInter28SeriesOtp(void);

/**
@brief ע��29ϵ��OTP����Ľӿڣ�����CSFLS2InitNor֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLS2RegAMD29SeriesOtp(void);

/**
@brief CFI Flash�豸��ʼ��
@param[in] nIndex flash�豸������
@param[in] psInitParams norflash��ʼ��ʱ����Ĳ���
@return CSHDI_SUCCESS ��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ��ʼ��ʧ�ܡ�
@note �˽ӿ�����ǰ��������CFIflash���������AMD�����SST�����Intel���
@note �������óɹ��󣬻�������ʵ��flash��С��
@note flash��init����˳�����,�����index���������˳�����
*/
CSHDI_Error_t CSFLS2InitNor(int nIndex, const CSFLS2InitParams_S* psInitParams);

/**
@brief SPI Flash�豸��ʼ��
@param[in] nIndex flash�豸������
@param[in] psInitParams norflash��ʼ��ʱ����Ĳ���
@return CSHDI_SUCCESS ��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ��ʼ��ʧ�ܡ�
@note �˽ӿ�����ǰ��������CFIflash���������AMD�����SST�����Intel���
@note �������óɹ��󣬻�������ʵ��flash��С��
@note flash��init����˳�����,�����index���������˳�����
*/
CSHDI_Error_t CSFLS2InitSpi(int nIndex, const CSFLS2InitParams_S* psInitParams);

/**
@brief Nand Flash�豸��ʼ��
@param[in] nIndex flash�豸������
@param[in] psInitParams norflash��ʼ��ʱ����Ĳ���
@return CSHDI_SUCCESS ��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ��ʼ��ʧ�ܡ�
@note �˽ӿ�����ǰ��������CFIflash���������AMD�����SST�����Intel���
@note �������óɹ��󣬻�������ʵ��flash��С��
@note flash��init����˳�����,�����index���������˳�����
@note �������óɹ��󣬻�������ʵ��flash�����С����ʵ���߼���С������ȷ����CSFLS2SetNandPartition�����ʹ�á�
*/
CSHDI_Error_t CSFLS2InitNand(int nIndex, const CSFLS2InitParams_S* psInitParams);

/**
@brief Emmc Flash�豸��ʼ��
@param[in] nIndex flash�豸������
@param[in] psInitParams Emmcflash��ʼ��ʱ����Ĳ���
@return CSHDI_SUCCESS ��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ��ʼ��ʧ�ܡ�
@note �������óɹ��󣬻�������ʵ��flash��С��
@note flash��init����˳�����,�����index���������˳�����
@note �������óɹ��󣬻�������ʵ��flash�����С��
*/
CSHDI_Error_t CSFLS2InitEmmc(int index, const CSFLS2InitParams_S* psInitParams);

/**
@brief Flash�������ã��ýӿڱ����ڳ�ʼ��֮�����

@param[in] nIndex: flash����
@param[in] psPartitionInfo: flash��������
@param[in] nPartitionCnt: flash��������
@return CSHDI_SUCCESS ��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
@note �˽ӿ�Ŀǰֻ����Nand flash,���ظ����ã�ÿ�ε��ú�flash�е������ǲ�ȷ����
@note Ҫ������Ļ�����˳��ģ���ַ��������
@note �������ֳɹ��󣬲ſ�������ʹ�ö�д���Ȳ�����������غ����᷵�ش���
*/
CSHDI_Error_t CSFLS2SetNandPartition(int nIndex, const CSFLS2Partition_S *psPartitionInfo, int nPartitionCnt);

/**
@brief ��ȡFlash�������ã����������÷��������

@param[in] nIndex: flash����
@param[out] psPartitionInfo: flash�������ô�ŵ�ַ;ΪNULLֵʱ,��ʾֻ��ȡ�����з����ĸ���
@param[in] nPartitionCnt: ����buffer��С����λΪsizeof(CSFlashPartition_S)
@param[out]pnActualCnt:ʵ�ʷ�����С
@return CSHDI_SUCCESS ��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ�ܡ�
@note �˽ӿ�Ŀǰֻ����Nand flash
@note �˽ӿ�����������1.��ȡ������nand�ķ�����Ϣ;2.��ȡ�����з�������(pPartitionInfo == NULL);
*/
CSHDI_Error_t CSFLS2GetNandPartition(int nIndex, CSFLS2Partition_S *psPartitionInfo, int nPartitionCnt, int *pnActualCnt);


/**
@brief ��ó�ʼ���ɹ���FLASH����Ϣ

@param[in] nIndex FLASHоƬ����ֵ��һ��Ϊ0��1
@param[out] psInfo flash����Ϣ
@return ����ɹ����򷵻�CSHDI_SUCCESS�����򷵻�CSHDI_FAILURE
*/
CSHDI_Error_t CSFLS2GetFlashInfo(int nIndex, CSFLS2FlashInfo_S* psInfo);

/**
@brief ����Flash���д�������ɹ����ô˺���֮�󣬽�ֹ����Ӧ����������д������

@param[in] dwStartAddr ��������Flash�ռ��ַ,����������
@param[in] dwSpaceSize ��������Flash�ռ��С,����������
@return CSHDI_SUCCESS ��ʾ���������ɹ���CSHDI_FAILURE��ʾ��������ʧ�ܡ�
*/								 	    
CSHDI_Error_t CSFLS2BlockLock(DWORD dwStartAddr, DWORD dwSpaceSize);

/**
@brief ����Flash���д�������ɹ����ô˺���֮���������Ӧ����������д������

@param[in] dwStartAddr ��������Flash�ռ��ַ,����������
@param[in] dwSpaceSize ��������Flash�ռ��С,����������
@return CSHDI_SUCCESS ��ʾ���������ɹ���CSHDI_FAILURE��ʾ��������ʧ�ܡ�
*/
CSHDI_Error_t CSFLS2BlockUnLock(DWORD dwStartAddr, DWORD dwSpaceSize);

/**
@brief  ͬCSFLSBlockLock��CSFLSBlockUnLockһ��������Flash���д������ֻ��һ���ɹ����ô˺���֮����Ӧ������״̬��Ϊblocked -down״̬�����Ҹ�״̬�ı䲻�ܽ��������ʵ�֣���ֻ��ͨ����λ��ϵ������ԭΪ��ʼ״̬������blocked -down״̬��Blocked -down״̬�£����ú���CSFLSBlockLock��CSFLSBlockUnLock���ܸı���Ӧ�����������������״̬��

@param[in] dwStartAddr ��������Flash�ռ��ַ,����������
@param[in] dwSpaceSize ��������Flash�ռ��С,����������
@return CSHDI_SUCCESS ��ʾ�������óɹ���CSHDI_FAILURE��ʾ��������ʧ�ܡ�
@note һ���ɹ����ô˺���֮����Ӧ������״̬��Ϊblocked -down״̬������Ҫ�ı��״̬���ܽ��������ʵ�֣���ֻ��ͨ����λ��ϵ������ԭΪ��ʼ״̬������blocked -down״̬��Blocked -down״̬�£����ú���CSFLSBlockLock��CSFLSBlockUnLock���ܸı���Ӧ�����������������״̬������˵��Blocked -down״̬�£�ĳ��FlashоƬ��Ӧ�����������������״̬��
*/
CSHDI_Error_t CSFLS2BlockLockDown(DWORD dwStartAddr, DWORD dwSpaceSize);

/**
@brief Ϊ����ߴ洢���ݵİ�ȫ�ԣ���ֹ�������д������flash�ͺ�֧�ָ�ֻдһ�εĹ��ܡ���ĳһ����ɹ�ʹ���˸�ֻдһ�εĹ���֮����Ӧ�������ݲ��ٱ仯������Ҳ���ܶԸ�����ɹ�ִ�в������д�����ˡ��Ըú�����OTP��ع��ܡ���OTP���򼰲��Ի�ʹ�÷�����������ʶ֮ǰ�������ص��ô˺�������������ױ���Flash���ú���ֻ����ҪOTP���ܵ���Ŀ�ſɵ��ã���Ӧ�ü���ʱ��Ӧ�ù���ʦ�����Flash�������ṹ�����ѵ���ʶ����������¹ʡ�

@param[in] dwStartAddr ��OTP�ռ����ʼ��ַ�����Ե�ַ��������ʼ��ַ��ַ����ǡ��Ϊĳ���������׵�ַ������Ļ��������������ֵַ�����жϽ��Ϊ�Ƿ������ַ����ȷ����ȫ�ԡ�
@param[in] dwOTPSize ��OTP������Ĵ�С����λΪBYTE���������С����ǡ��ΪOTP��������С֮�ͣ�����Ҫ����ʹ�õ���ӦFlash OTP��������ĸ�����ڲ�ͬ���͵�Flash������OTP���������鲻ͬ��������ο���Ӧ��Datasheet. ��������������ֵ��dwOTPSize�����кϷ����жϣ����������Remarks��������Ҫ�󣬾ͻ᷵�ش�����ȷ����ȫ�ԡ�
@return CSHDI_SUCCESS ��ʾд�����ɹ���CSHDI_FAILURE��ʾд����ʧ�ܡ� 
@note
- 1,����ST M28W320FS��M28W640FS�ͺŵ�Flash,��8(*8K Bytes)��Parameter Block�Ϳ�������һ��Main Block�ɷֱ𵥶��������ã�����Main Block��������Ϊ��λ����OTP, ��ÿ��Ϊ����main Block,��128Kbytes.
- 2,����ST M58LW032D�ͺŵ�Flash,��ÿ��������Ϊ128K Bytes,ÿ�����������Ե�������OTP������
- 3������Spansion S29GL064M11FAIS1��S29GL064M11FAIS2�ͺŵ�Flash,��8(*8K Bytes)��Parameter Block�Ϳ�������һ��Main Block��Ϊһ�飬ͨ��Permanent Lock�����������������ϵ�OTP������һ����˵����9��������һ����OTP����������ST M28WXXFSϵ�е�����������һ��һ���ؽ��в�������������9������������������128K Bytes�������Ϊһ�����OTP����������������4��������256KbytesΪһ�����������OTP��
- 4������Top�͵ģ������Parameter Block������ЩBlockΪ��˵ļ���Block������Bottom�͵ģ������Parameter Block������ЩBlockΪ��Ͷ˵ļ���Block������M28W320FS��ΪM28W320FST��M28W320FSB���֡�����M28W320FST����Top�ͣ���8��Parameter Block��Ϊ���8������������M28W320FSB����Bottom�ͣ���8��Parameter Block��Ϊ���㵽������˸�������S29GL064M11FAIS1��Top�ͣ���8��Parameter Block��Ϊ���8��������S29GL064M11FAIS2��Bottom�ͣ���8��Parameter Block��Ϊ���㵽������˸�������
- 5, ����S29GL128N10FFIS1��ÿ�����������Ե���OTP��ÿ������128KB��   
*/
CSHDI_Error_t CSFLS2SetBlockToOtp(DWORD dwStartAddr, DWORD dwOTPSize);

/**
@brief ��block���Ϊ�����ÿ�

@param[in] dwBlockAddr: block�Ŀ�ʼ��ַ
@param[in] eBlkStatus:block��״̬
@return CSHDI_SUCCESS ��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
@note �˽ӿ�Ϊ���Խӿڣ�Ŀǰֻ����Nand flash�Ĳ���
*/
CSHDI_Error_t CSFLS2SetBlockStatus(DWORD dwBlockAddr, CSFLS2NandBlockStatus_E eBlkStatus);

/**
@brief ��ȡblock״̬

@param[in] dwBlockAddr: block�Ŀ�ʼ��ַ
@param[out] peBlkStatus:block��״̬��ŵ�ַ
@return CSHDI_SUCCESS ��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ�ܡ�
@note �˽ӿ�Ϊ���Խӿڣ�Ŀǰֻ����Nand flash�Ĳ���
*/
CSHDI_Error_t CSFLS2GetBlockStatus(DWORD dwBlockAddr, CSFLS2NandBlockStatus_E *peBlkStatus);

/**
@brief ��FLASH�����ַ�ж�ȡ��������� 

@param[in] dwStartAddress �洢���ݵ�FLASH�ռ�������ַ
@param[in] pbValue ����������ݵ��ڴ�ռ��ַ
@param[in] dwLength Ҫ��ȡ�����ݳ���(���ֽ�Ϊ��λ)
@return CSHDI_SUCCESS��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ��
*/
CSHDI_Error_t CSFLS2ReadFlashPhysics(DWORD dwStartAddress, PBYTE pbValue, DWORD dwLength);

/**
@brief ��Flash�ж�ȡ��dwStartAddressΪ��ʼ��ַ,dwLength�ֽ�Ϊ���ȷ�Χ�ڵ�OOB����,��������������OOB���ݵĳ���(Bytes) 
@param[in] dwStartAddress �洢���ݵ�Flash�ռ���ʼ��ַ,�õ�ַΪ�߼���ַ�����밴block����
@param[out] pbBuf ����OOB���ݵĻ�������ʼ��ַ
@param[in] dwLength:����������(���ֽ�Ϊ��λ)�����밴block����
@param[out] pdwRetLen:���ڴ����������OOB���ݵĳ���(���ֽ�Ϊ��λ)
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���CSUDI_FAILURE��ʾ��ȡʧ��
@note �˽ӿ�Ŀǰֻ����nandflash
*/
CSHDI_Error_t CSFLS2ReadNandOOB(DWORD dwStartAddress, PBYTE pbBuf, DWORD dwLength, DWORD *pdwRetLen);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
