/**@defgroup CHECKDECRYPT CHECKDECRYPT 提供校验解密接口
@brief CHECKDECRYPT模块对应头文件<csbl_checkdecrypt.h>，定义了各种校验解密的接口

@version 1.0.0 2011/3/30 创建校验解密模块,增加conax规范的校验解密
@{
*/
#ifndef _CSBL_CHECK_DECRYPT_H_
#define _CSBL_CHECK_DECRYPT_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "csbl_gconfig.h"

#define AES128LEN 16		//AES128 密钥长度或向量长度

typedef struct
{
    BYTE m_aucKeyData[AES128LEN];	//AES 128 密钥或向量实体
}CSBLAESData_S;

typedef struct		
{
	BOOL m_bValid;		//密钥是否有效标志，无效为0，其它为有效
	CSBLAESData_S m_sKey;	//AES 解密密钥
	CSBLAESData_S m_sIV;	//AES 解密向量	
}CSBLAESKeyInfo_S;

typedef struct 
{
	CSBLDataPos_S *m_pSrcPos;	//APP 解密前地址
	CSBLDataPos_S *m_pDstPos;	//APP 解密后地址
}CSBLDataAndBootPos_S;


/**
@brief 获取Conax规范校验解密相关数据信息的函数
@param[in] Param1 输入的校验解密数据。见CSBLDataPos_S定义。
@param[out] Param2 获取的解密数据的信息。见CSBLCheckDecryptInfo_S定义。

@return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
@note 见EM_CSBL_CONFIG_CHECKDECRYPT_BOOT或EM_CSBL_CONFIG_CHECKDECRYPT_LOAD的说明
*/
int CSBLGetCheckDecryptInfoConax(PVOID Param1, PVOID Param2);

/**
@brief Conax规范启动时使用的校验解密函数
@param[in] Param1 输入的校验解密数据。见CSBLDataPos_S定义，必须为内存中数据。
@param[out] Param2 输出的校验解密后数据。见CSBLDataPos_S定义，必须为内存中数据。

@return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
@note 见EM_CSBL_CONFIG_CHECKDECRYPT_BOOT的说明
*/
int CSBLCheckDecryptConaxBoot(PVOID Param1, PVOID Param2);

/**
@brief Conax规范下载时使用的校验解密函数
@param[in] Param1 输入的校验解密数据。见CSBLDataPos_S定义，必须为内存中数据。
@param[out] Param2 输出的校验解密后数据。见CSBLDataPos_S定义，必须为内存中数据。

@return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
@note 见EM_CSBL_CONFIG_CHECKDECRYPT_LOAD的说明
@note load与boot函数的区别在于多了一个将码流中的软件版本号与下载的loader/app中的软件版本号比较的过程，不相同则判定为校验失败
*/
int CSBLCheckDecryptConaxLoad(PVOID Param1, PVOID Param2);

/**
@brief Unitend规范启动app时，使用的校验函数
@param[in] Param1、Param2暂未使用

@return如果成功函数返回SUCCESS;如果失败返回FAILURE;
*/
int CSBLCheckUnitendBoot(PVOID Param1, PVOID Param2);

//暂时未使用
int CSBLGetCheckDecryptInfoUnitend(PVOID Param1, PVOID Param2);
/**
@brief 获取Verimatrix规范校验解密相关数据信息的函数
@param[in] Param1 输入的校验解密数据。见CSBLDataPos_S定义。
@param[out] Param2 获取的解密数据的信息。见CSBLCheckDecryptInfo_S定义。

@return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
@note 见EM_CSBL_CONFIG_CHECKDECRYPT_BOOT或EM_CSBL_CONFIG_CHECKDECRYPT_LOAD的说明
*/
int CSBLGetCheckDecryptInfoVerimatrix(PVOID Param1, PVOID Param2);

/**
@brief Verimatrix规范启动时使用的校验解密函数
@param[in] Param1 输入的校验解密数据。见CSBLDataPos_S定义，必须为内存中数据。
@param[out] Param2 输出的校验解密后数据。见CSBLDataPos_S定义，必须为内存中数据。

@return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
@note 见EM_CSBL_CONFIG_CHECKDECRYPT_BOOT的说明
*/
int CSBLCheckDecryptVerimatrixBoot(PVOID Param1, PVOID Param2);

/**
@brief Verimatrix规范下载时使用的校验解密函数
@param[in] Param1 输入的校验解密数据。见CSBLDataPos_S定义，必须为内存中数据。
@param[out] Param2 输出的校验解密后数据。见CSBLDataPos_S定义，必须为内存中数据。

@return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
@note 见EM_CSBL_CONFIG_CHECKDECRYPT_LOAD的说明
@note load与boot函数的区别在于多了一个将码流中的软件版本号与下载的loader/app中的软件版本号比较的过程，不相同则判定为校验失败
*/
int CSBLCheckDecryptVerimatrixLoad(PVOID Param1, PVOID Param2);

/**
@brief Conax规范启动时使用的校验解密函数
@param[in/out] Param1 实际使用类型为CSBLDataAndBootPos_S,  m_pSrcPos 为输入的待校验解密数据地址，
			   m_pSrDst 为校验解密后的数据地址。 见CSBLDataPos_S定义。
@param[in] Param2 为AES 解密密钥，见CSBLAESKeyInfo_S 定义。

@return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
@note 见EM_CSBL_CONFIG_CHECKDECRYPT_BOOT的说明
*/
int  CSBLCheckDecrypUseOOBConaxBoot(PVOID Param1, PVOID Param2);

/**
@brief Conax规范下载时使用的校验解密函数
@param[in] Param1 输入的校验解密数据。见CSBLDataPos_S定义，必须为内存中数据。
@param[out] Param2 输出的校验解密后数据。见CSBLDataPos_S定义，必须为内存中数据。

@return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
@note 见EM_CSBL_CONFIG_CHECKDECRYPT_LOAD的说明
*/
int CSBLCheckDecryptUseOOBConaxLoad(PVOID Param1, PVOID Param2);


/**
 @bried Conax 规范booter 启动时使用的校验函数，版本V2
 @Param1[in]		: CSBLDataAndBootPos_S 类型，见CSBLDataAndBootPos_S 定义
 @Param2[out]	: CSBLAESKeyInfo_S 类型，见CSBLAESKeyInfo_S 定义

 @return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
 @note 见EM_CSBL_CONFIG_CHECKDECRYPT_BOOT的说明
 */
int  CSBLCheckDecrypConaxBoot_V2(PVOID Param1, PVOID Param2);


/**
 @bried Conax 规范下载时使用的校验函数，版本V2
 @Param1[in]		: CSBLDataPos_S *类型，表示校验后的数据，必须为内存中的数据
 @Param2[out]	: CSBLDataPos_S *类型，表示校验后的数据，必须为内存中的数据

 @return 如果成功函数返回SUCCESS;如果失败返回FAILURE;
 @note 见EM_CSBL_CONFIG_CHECKDECRYPT_LOAD的说明
 */
int CSBLCheckDecryptConaxLoad_V2(PVOID Param1, PVOID Param2);



#ifdef  __cplusplus
}
#endif

#endif



