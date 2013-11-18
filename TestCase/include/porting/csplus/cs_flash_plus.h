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
	char m_szPTName[32];               /**< �������� */
    	unsigned int m_uoffset;        /**<Flash������ƫ�ƣ���0��ʼ*/
	unsigned int m_uPTSize;            /**< ���������С������spare_size�Ĵ�С����λΪBYTE */
	unsigned int m_uSpareSize;         /**< ��������ռ�Ĵ�С����λΪBYTE*/
} CSFlashPartition_S;


typedef enum
{
	EM_FLASH_GOOD_BLOCK,    /**< �ÿ�*/
	EM_FLASH_BAD_BLOCK       /**< ����*/
}CSNandFLS_BLK_Status_E;

/**
@brief FLASH API ��ʼ����

@param[in] bVPPGPIOIndex Ӳ��������·GPIO�������š�ͨ����ֵ��ȷ�������ţ��Կ���Ӳ��������·����ʵ��Ӳ��д������
@return CSHDI_SUCCESS ��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ��ʼ��ʧ�ܡ�
@note  �˽ӿ�Ĭ��ע����ȫ��CFI�����������AMD�����SST�����Intel������൱��CSFLSRegisterAMDCmdSet()+CSFLSRegisterSSTCmdSet()+CSFLSRegisterInterCmdSet()+CSFLSInitializeCFI
*/
CSHDI_Error_t CSFLSInitialize(BYTE bVPPGPIOIndex);

/**
@brief CSFLSSetFLSCount():Config Used Flash Chip Count	

@param[in] nCurFLSCount ָ��ϵͳӦ�����õ���FLASHƬ����
@return  CSHDI_SUCCESS��ʾ���ò����ɹ���CSHDI_FAILURE��ʾ���ò���ʧ��
@note 
*/					 							    
CSHDI_Error_t CSFLSSetFLSCount(int nCurFLSCount) ;

/**
@brief ����ָ��FLASH�ĵ�ַ�ռ䡣 

@param[in] nFlashIndex ��ǰFLASH��ţ��ӣ���ʼ���߽磺 nFlashIndex >= 0 && nFlashIndex < ���õ�flashƬ��
@param[in] dwBaseAddress ��ǰFLASH�Ļ���ַ�߽磺������ż��ַ
@param[in] dwBufferSize ��ǰFLASH�Ŀռ��С����λ��BYTE���߽磺dwBufferSize <= 32*1024*1024;
@return CSHDI_SUCCESS��ʾ���ò����ɹ���CSHDI_FAILURE��ʾ���ò���ʧ��
*/
CSHDI_Error_t CSFLSSetSpace(int nFlashIndex, DWORD dwBaseAddress, DWORD dwBufferSize);

/**
@brief ����Flash���д�������ɹ����ô˺���֮�󣬽�ֹ����Ӧ����������д������

@param[in] nChipNo ��������FlashоƬ�š�nChipNoΪ0ʱ��ʾ��1��Flash��
@param[in] nBlockNo ������оƬ�������š�nBlockNoΪ0ʱ��ʾ��1��������
@return CSHDI_SUCCESS ��ʾ���������ɹ���CSHDI_FAILURE��ʾ��������ʧ�ܡ�
*/								 	    
CSHDI_Error_t CSFLSBlockLock(int nChipNo, int nBlockNo);

/**
@brief ����Flash���д�������ɹ����ô˺���֮���������Ӧ����������д������

@param[in] nChipNo ��������FlashоƬ�š�nChipNoΪ0ʱ��ʾ��1��Flash��
@param[in] nBlockNo ������оƬ�������š�nBlockNoΪ0ʱ��ʾ��1��������
@return CSHDI_SUCCESS ��ʾ���������ɹ���CSHDI_FAILURE��ʾ��������ʧ�ܡ�
*/
CSHDI_Error_t CSFLSBlockUnLock(int nChipNo, int nBlockNo);

/**
@brief  ͬCSFLSBlockLock��CSFLSBlockUnLockһ��������Flash���д������ֻ��һ���ɹ����ô˺���֮����Ӧ������״̬��Ϊblocked -down״̬�����Ҹ�״̬�ı䲻�ܽ��������ʵ�֣���ֻ��ͨ����λ��ϵ������ԭΪ��ʼ״̬������blocked -down״̬��Blocked -down״̬�£����ú���CSFLSBlockLock��CSFLSBlockUnLock���ܸı���Ӧ�����������������״̬��

@param[in] nChipNo оƬ�š�nChipNoΪ0ʱ��ʾ��1��Flash��
@param[in] nBlockNo ��ӦFlashоƬ�������š�nBlockNoΪ0ʱ��ʾ��1��������
@return CSHDI_SUCCESS ��ʾ�������óɹ���CSHDI_FAILURE��ʾ��������ʧ�ܡ�
@note һ���ɹ����ô˺���֮����Ӧ������״̬��Ϊblocked -down״̬������Ҫ�ı��״̬���ܽ��������ʵ�֣���ֻ��ͨ����λ��ϵ������ԭΪ��ʼ״̬������blocked -down״̬��Blocked -down״̬�£����ú���CSFLSBlockLock��CSFLSBlockUnLock���ܸı���Ӧ�����������������״̬������˵��Blocked -down״̬�£�ĳ��FlashоƬ��Ӧ�����������������״̬��
*/
CSHDI_Error_t CSFLSBlockLockDown(int nChipNo, int nBlockNo);

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
CSHDI_Error_t CSFLSSetBlockToOtp(DWORD dwStartAddr, DWORD dwOTPSize);

/**
@brief ��ó�ʼ���ɹ���FLASH�Ļ���ַ��ʣ��ռ�

@param[in] nChipIndex FLASHоƬ����ֵ��һ��Ϊ0��1
@param[out] dwBaseAddress ��ʼ���ɹ���FLASH����ַָ��
@param[out] dwSpaceSize ��ʼ���ɹ���FLASH��ʣ��ռ�
@return ����ɹ����򷵻�CSHDI_SUCCESS�����򷵻�CSHDI_FAILURE
*/
CSHDI_Error_t CSFLSGetFlashInfo (int nChipIndex, DWORD *dwBaseAddress, DWORD *dwSpaceSize);

/**
@brief CSFLSGetVersion():���ڻ�ȡ��ǰFlash���ÿ�İ汾�š�

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
@brief ע��AMD����Ľӿڣ�����CSFLSInitializeCFI֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLSRegisterAMDCmdSet(void);

/**
@brief ע��SST����Ľӿڣ�����CSFLSInitializeCFI֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLSRegisterSSTCmdSet(void);

/**
@brief ע��Inter����Ľӿڣ�����CSFLSInitializeCFI֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLSRegisterInterCmdSet(void);

/**
@brief ע��28ϵ��OTP����Ľӿڣ�����CSFLSInitializeCFI֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLSRegInter28SeriesOtp(void);

/**
@brief ע��29ϵ��OTP����Ľӿڣ�����CSFLSInitializeCFI֮ǰ���ȵ���

@return ����ɹ����򷵻�TRUE�����򷵻�FALSE
*/
BOOL CSFLSRegAMD29SeriesOtp(void);

/**
@brief FLASH API ��ʼ��,��Ҫ������ض�һ������ĳ�ʼ���ӿ�,��CSFLSRegisterAMDCmdSet(),CSFLSRegisterSSTCmdSet(),CSFLSRegisterInterCmdSet()֮�����

@param[in] bVPPGPIOIndex Ӳ��������·GPIO�������š�ͨ����ֵ��ȷ�������ţ��Կ���Ӳ��������·����ʵ��Ӳ��д������
@return CSHDI_SUCCESS ��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ��ʼ��ʧ�ܡ�
@note �˽ӿ��ڲ����˻���CFI�������������ӵģ���CS_FLASH_T_02.00.00�����
*/
CSHDI_Error_t CSFLSInitializeCFI(BYTE bVPPGPIOIndex);


/**
@brief Flash�������ã��ýӿڱ����ڳ�ʼ��֮�����

@param[in] pPartitionInfo: flash��������
@param[in] nPartitionCnt: flash��������
@return CSHDI_SUCCESS ��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
@note �˽ӿ�Ŀǰֻ����Nand flash,���ظ����ã�ÿ�ε��ú�flash�е������ǲ�ȷ����
@note Ҫ������Ļ�����˳��ģ���ַ��������
*/
CSHDI_Error_t CSFLSSetPartition(CSFlashPartition_S *pPartitionInfo, int nPartitionCnt);


/**
@brief ��ȡFlash�������ã����������÷��������

@param[in] pPartitionInfo: flash�������ô�ŵ�ַ
@param[in] nPartitionCnt: ����buffer��С����λΪsizeof(CSFlashPartition_S)
@param[out]pnActualCnt:ʵ�ʷ�����С
@return CSHDI_SUCCESS ��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ�ܡ�
@note �˽ӿ�Ŀǰֻ����Nand flash
*/
CSHDI_Error_t CSFLSGetPartition(CSFlashPartition_S *pPartitionInfo, int nPartitionCnt, int *pnActualCnt);



/**
@brief ��block���Ϊ�����ÿ�

@param[in] unBlkNum: block�ı��
@param[in] eBlkStatus:block��״̬
@return CSHDI_SUCCESS ��ʾ���óɹ���CSHDI_FAILURE��ʾ����ʧ�ܡ�
@note �˽ӿ�Ϊ���Խӿڣ�Ŀǰֻ����Nand flash�Ĳ���
*/
CSHDI_Error_t CSNandFLSSetBlockStatus(unsigned int unBlkNum, CSNandFLS_BLK_Status_E eBlkStatus);



/**
@brief ��ȡblock״̬

@param[in] unBlkNum: block�ı��
@param[out] peBlkStatus:block��״̬��ŵ�ַ
@return CSHDI_SUCCESS ��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ�ܡ�
@note �˽ӿ�Ϊ���Խӿڣ�Ŀǰֻ����Nand flash�Ĳ���
*/
CSHDI_Error_t CSNandFLSGetBlockStatus(unsigned int unBlkNum, CSNandFLS_BLK_Status_E *peBlkStatus);



/**
@brief ��FLASH�����ַ�ж�ȡ��������� 

@param[in] dwStartAddress �洢���ݵ�FLASH�ռ�������ַ
@param[in] pbValue ����������ݵ��ڴ�ռ��ַ
@param[in] dwLength Ҫ��ȡ�����ݳ���(���ֽ�Ϊ��λ)
@return CSHDI_SUCCESS��ʾ��ȡ�ɹ���CSHDI_FAILURE��ʾ��ȡʧ��
*/
CSHDI_Error_t CSFLSReadFlashPhysics (DWORD dwStartAddress, PBYTE pbValue, DWORD dwLength);
#ifdef __cplusplus
}
#endif

/** @} */

#endif
