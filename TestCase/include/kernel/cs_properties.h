/******************************************************************************
FileName:  properities_parse.h
Copyright (c) 2009， Shen Zhen Coship Electronics CO.,LTD.

Version:       Ver 1.0
Author:       
description: 
	注意：
	1、创建有多个接口，但推荐使用句柄创建和Load的方法，这样可以用一个句柄Load多个属性文件。
	2、存储文件时不建议使用CSPROStore()，推荐使用CSPROStoreToFile()

modify:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0          2009.08.26                     create
1.1          2010.03.05                     增加存到指定文件和删除结点的功能
*******************************************************************************/

#ifndef _PROPERTIES_PARSE_H_
#define _PROPERTIES_PARSE_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "mmcp_typedef.h"

/**
 * @brief  创建properities配置文件解析器
 * @param[in]  pcFileName  传入要打开的配置文件
 * @param[out]   phHandle   创建成功的解析器句柄，用于后续操作
 * @return  TRUE表示成功，FALSE表示失败，失败时phHandle的值是无效的
 * @note  如果文件不存在会创建文件
 * @note  需要调用CSPRODestory释放
 */
BOOL CSPROCreate(char *pcFileName, HCSHANDLE *phHandle);
/**
 * @brief       从buf创建properties配置文件解析器
 * @param[in]   pcBuf   要解析的buf
 * @param[in]   dwSize  buf的长度
 * @param[out]   phHandle   创建成功的解析器句柄，用于后续操作
 * @return  TRUE表示成功，FALSE表示失败，失败时phHandle的值是无效的
 * @note  调用本方法创建的解析器不能调用CSPROStore保存到文件
 * @note  需要调用CSPRODestory释放
 */
BOOL CSPROCreateFromBuf(char* pcBuf, DWORD dwSize, HCSHANDLE *phHandle);
/**
 * @brief  创建句柄
 * @param[out]  phHandle  创建的句柄
 * @return  TRUE表示成功，FALSE表示失败，失败时phHandle的值是无效的
 * @note  可以只创建句柄，之后再装载数据
 * @note  推荐使用该方法和CSPROLoadFromBuf
 */
 BOOL   CSPROCreateHandle(HCSHANDLE *phHandle);
/**
 * @brief       装载数据到解析器
 * @param[in]   hHandle   解析器句柄
 * @param[in]   pcBuf   要解析的buf
 * @param[in]   dwSize  buf的长度
 * @return  TRUE表示成功，FALSE表示失败
 * @note  hHandle可以是任意方法创建的句柄
 * @note  可以多次调用本方法，这样可以实现将多个配置合并的功能
 */
BOOL CSPROLoadFromBuf(HCSHANDLE  hHandle, char* pcBuf, DWORD dwSize);

/**
 * @brief       从文件装载数据到解析器
 * @param[in]   hHandle   解析器句柄
 * @param[in]   pcFileName  要装载的文件
 * @return  TRUE表示成功，FALSE表示失败
 * @note  hHandle可以是任意方法创建的句柄
 * @note  可以多次调用本方法，这样可以实现将多个配置合并的功能
 */
BOOL CSPROLoadFromFile(HCSHANDLE  hHandle, char* pcFileName);


/**
 * @brief  销毁
 * @param[in]  hHandle  句柄
 * @return  TRUE表示成功，FALSE表示失败
 * @note   销毁CSPROCreate()时创建的句柄，销毁之后不能再使用
*/
BOOL CSPRODestory(HCSHANDLE hHandle);

/**
 * @brief  获取属性值
 * @param[in]  hHandle  句柄
 * @param[in]  pcKey  要获取的属性名
 * @param[out]   pcValue   属性值，内存由外部申请
 * @param[in/out]   pdwLen   属性值的长度，作为输入参数时表示pcValue的长度，作为输出参数时表示实际写入pcValue的字节数
 * @return  TRUE表示成功，FALSE表示未找到，未找到时pcValue和pdwLen均无效
 * @note  请特别注意pwdLen是可读可写参数，因此在每次调用CSPROGetProperty之前都要重新赋值
 */
BOOL CSPROGetProperty(HCSHANDLE hHandle, char *pcKey, char *pcValue, DWORD *pdwLen);

/**
 * @brief  获取属性值的长度
 * @param[in]  hHandle  句柄
 * @param[in]  pcKey  要获取的属性名
 * @return  
 	如果key没有对应的环境变量，返回0；
	错误、文件不存在、key不存在，返回-1；
 * @note  
 */
int CSPROGetValueLen(HCSHANDLE hHandle, char *pcKey);

/**
 * @brief  设置属性值
 * @param[in]  hHandle  句柄
 * @param[in]  pcKey  要设置的属性名
 * @param[out]   pcValue   要设置的属性值，传以'\0'结尾的字符串
 * @return  TRUE表示设置成功，FALSE表示设置失败
 * @note    pcValue可以传空，表示设置对应的属性为空
 * @note    设置属性只是修改了内存中的值，不会修改文件，要将修改保存到文件请调用CSPROStore()或CSPROStoreToFile()
 * @note    如果结点不存在，会创建该结点
 */
BOOL CSPROSetProperty(HCSHANDLE hHandle, char* pcKey, char *pcValue);

/**
 * @brief  删除一个结点
 * @param[in]  hHandle  句柄
 * @param[in]  pcKey  要删除的键名
 * @return  TRUE表示设置成功，FALSE表示设置失败
 * @note    删除结点只是删除了内存中的结点，不会删除文件，要将删除同步到文件请调用CSPROStore()或CSPROStoreToFile()
 * @note    当结点不存在时返回成功
 */
BOOL CSPRODelItem(HCSHANDLE hHandle, char* pcKey);

/**
 * @brief  获取属性的数目
 * @param[in]  hHandle  句柄
 * @param[in]   pnCount  属性的个数
 * @return  TRUE表示获取成功，FALSE表示获取失败 
 */
BOOL  CSPROGetCount(HCSHANDLE hHandle, int  *pnCount);

/**
 * @brief  根据索引号获取属性名和属性值
 * @param[in]  hHandle  句柄
 * @param[in]  nIndex   索引号，取值范围为0到Key的最大数目减1，即如果总共有10个key，则合法的索引为0到9
 * @param[out] pckey    属性名，以'\0'结束。内存需要外部去分配，分配的大小应当比实际属性名长度加1大
        如果分配不足无法存属性名会获取失败，如果不知道属性名有多长，应尽可能分配大一些
 * @param[int] dwKeyLen 表示内存分配的大小，包含为'\0'分配的内存
 * @param[out] pcValue   属性值，以'\0'结束。内存需要外部去分配，分配的大小应当比实际属性值长度加1大
        如果分配不足无法存属性值会获取失败，如果不知道属性值有多长，应尽可能分配大一些
 * @param[int] dwValueLen 表示内存分配的大小，包含为'\0'分配的内存
 * @return  TRUE表示获取成功，FALSE表示获取失败 
 * @note  本方法与CSPROGetCount()配合使用，可以遍历出所有的属性
 * @note  注意在遍历的过程应该是连续的，不可增删键，例如调用CSPROSetProperty创建一个不存在的属性，其结果将是不可预期的
          因为增删键会影响到键的数目和索引
*/
BOOL CSPROGetItem(HCSHANDLE hHandle, int  nIndex, char *pckey, DWORD dwKeyLen, char *pcValue, DWORD dwValueLen);

/**
 * @brief  存储到文件
 * @param[in]  hHandle  句柄
 * @return  TRUE表示成功，FALSE表示失败
 * @note   保存的文件为CSPROCreate()时传入的文件
 * @note   仅对文件解析器有效，即CSPROCreate有效，而CSPROParseFromBuf无效
 * @note   本接口的功能将逐步被CSPROStoreToFile取代，不推荐使用本方法
 */
BOOL CSPROStore(HCSHANDLE hHandle);

/**
 * @brief  存储到指定文件
 * @param[in]  hHandle  句柄
 * @param[in]  pcFileName  指定文件路径
 * @return  TRUE表示成功，FALSE表示失败
 * @note   利用本接口可以实现多个属性文件合并后存到同一个文件
 * @note   如果传进来的文件已经存在则会删除原文件，因此用户需要确定原文件是可以被覆盖的
 */
BOOL CSPROStoreToFile(HCSHANDLE hHandle, char * pcFileName);


#ifdef __cplusplus
}
#endif
#endif

