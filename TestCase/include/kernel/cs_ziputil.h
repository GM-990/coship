/**@defgroup CS_ZipUtil模块是kernel对zlib库接口的封装层，以便对外提供更好的可用性。
@brief zip/jar文件读取支持模块
 
@version 1.0 2010/04/27 Initial Version
@version 1.1 2010/06/02 comment修改为中文
@{
*/

#ifndef _CSZIPUTIL_H_
#define _CSZIPUTIL_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"

/**@brief 
 模块说明：支持zip/jar格式的文件读取，注意以下情况可能导致异常：
 - zip文件大小大于INT_MAX(0x7FFFFFFF)时，不支持读取.
 - zip文件的里的entry(entry, 即压缩包里面的一个实体)大小大于INT_MAX时，不支持读取.
 - zip文件中entry的个数超过INT_MAX.
 - m_nTime和m_nCrc字段为有符号数, 虽然实际上它们表现为无符号数.
 - 如果csize(entry压缩后的大小)值为0，则表示entry未被压缩.
 - 如果m_pcExtra字段不等于NULL, 则m_pcExtra的头两个字节表示extra数据的长度
      (小端字节序).
 - 如果m_nPos值为负, 则该值表示entryLOC头的位置. 当entry首次被读取时, m_nPos被赋值为
       entry数据(entry data)的的位置.
 */

typedef struct _CSZIPUTILEntry_S {  /**< zip文件entry(压缩包中的一个实体) */
    char *m_pcName;	  	  			/**< 实体名 */
    int   m_nTime;            		/**< 修改时间 */
    int   m_nSize;	  	  			/**< 压缩前数据大小 */
    int   m_nCSize;  	  			/**< 压缩后数据大小 (如果为0, 表示未压缩) */
    int   m_nCrc;		  			/**< 未压缩数据的CRC值 */
    char *m_pcComment;	  			/**< zip文件注释(可选) */
    char *m_pcExtra;	  			/**< 额外数据(可选) */
    int   m_nPos;	  	  			/**< LOC头的位置 (如果取负), 或data的位置(取正) */
}CSZIPUTILEntry_S;

/**@brief 
 内存中的hash表元素
 
 在一个典型系统中, 会存在许多CSZIPUTILCell_S对象, 我们对每一个活跃状态的JAR/ZIP文件
 中的每个实体对应一个CSZIPUTILCell_S对象.

 注意: 为节省空间, CSZIPUTILCell_S中不存储entry的名字, 而是只存储一个32位的hash值.
*/
typedef struct _CSZIPUTILCell_S {
    int 		   m_nPos;          /**< LOC头在zip文件中的偏移量 */
    unsigned int   m_unHash;		/**< 基于entry名字的32位hash值 */
    unsigned short m_usNelen;       /**< 名字(name)和额外数据(extra data)的长度 */
    unsigned short m_usNext;      	/**< hash链, 索引到CSZIPUTILFile_S->m_psEntries */
    int            m_nSize;			/**< 未压缩大小 */
    int            m_nCSize;		/**< 压缩后大小 */
    int            m_nCrc;
}CSZIPUTILCell_S;

/**@brief zip文件描述结构体. */
typedef struct _CSZIPUTILFile_S {
    char *m_pcName;	  	  			/**< zip文件名 */
    int m_nRefs;		  			/**< 活跃的引用数 */
    int m_nFd;		  				/**< 打开文件描述符 */
    void *m_pvLock;		  			/**< 文件读取锁 */
    char *m_pcComment; 	  			/**< zip文件注释 */
    char *m_pcMsg;		  			/**< zip错误消息 */
    CSZIPUTILCell_S *m_psEntries;   /**< hash CSZIPUTILCell_S元素数组 */
    int m_nTotal;	  	  			/**< entry实体总数 */
    unsigned short *m_pusTable;    	/**< Hash链头指针: 指向m_psEntries数组 */
    int m_nTablelen;	  			/**< number of hash eads */
    struct _CSZIPUTILFile_S *m_psNext;  /**< 指向搜索链表中下一个zip文件 */
    CSZIPUTILEntry_S *m_psCache;     /**< (缓存)最近被释放的entry实体指针 */
    /**< 存储于META_INF目录下的, 与metadata名相关的信息. */
    char **m_ppCMetanames;     		/**< meta名数组 (可能有名字为NULL) */
    int m_nMetacount;	  			/**< meta名数组元素个数 */
    /**< 如果有pre-entry注释, 将存放在以下数组 */
    char **m_ppcComments;
}CSZIPUTILFile_S;

/**
@brief
 初始化cs_ziputil模块, 返回0表示成功, -1表示不能被初始化.
 
Detail Info:

@return
	 0: 成功
	-1: 失败
@note 
*/
int CSZIPUTILInitialize();

/**
@brief
 销毁cs_ziputil模块, 与CSZIPUTILInitialize对应.
 
Detail Info:

@return
@note 
*/
void CSZIPUTILDestroy();

/**@brief 
 采用指定的模式(即文件的打开方式, 如可读,可写, 读写等. 如取"CSFS_O_ACCMODE", 
 取值可参考 cs_fsapi.h 头文件定义)打开一个zip/jar文件, 返回CSZIPUTILFile_S对象, 
 如果打开过程出错, 则返回NULL 
 
Detail Info:
 如果打开zip文件过程中出错, (*ppMsg)将返回错误信息字符串(如果ppMsg!=NULL的话),
 如果正常打开, (*ppMsg)将被赋值为NULL.

@param[in] pcName: zip文件名(URL+文件名, 暂只支持绝对路径)
@param[in/out]  ppMsg: 错误信息字符串
@param[in] nMode: 指定的zip/jar文件打开方式.如 CSFS_O_ACCMODE. 
                  参考cs_fsapi.h头文件.
@return: 打开的CSZIPUTILFile结构体指针, 如果打开失败, 返回NULL.
@note 
*/
HCSHANDLE CSZIPUTILOpenGeneric(const char *pcName, char *ppMsg, int nMode);

/**
@brief
 从指定的zip实体中读取指定位置和大小的bytes.
 
Detail Info:
 需确保zip文件预先被Lock.(函数在实现过程中已自动Lock, 可不关注) 
 返回读取到的实际字节数, 如果读取出错则返回-1.
 如果调用CSZIPUTILGetZipFileInfo(hZip)获取到m_pcMsg的值不为NULL,
 则表明读取过程中出错. m_pcMsg中包含有出错相关的内容.
 
@param[in] hZip: zip文件句柄(handle)
@param[in] hEntry: zip实体句柄
@param[in] nPos: 读取的起始位置
@param[in] nLen: 需读取的数据长度
@return 
     正值: 实际读取的字节数
       -1: 读取出错
@note 
*/
int CSZIPUTILRead(HCSHANDLE hZip, HCSHANDLE hEntry, 
	int nPos, void *pvBuf, int nLen);

/**
@brief 
 关闭指定的zip文件对象
 
Detail Info:

@param[in] hZip: 需关闭的zip文件句柄.
@return 
@note 
*/
void CSZIPUTILClose(HCSHANDLE hZip);

/**
@brief
 如果执行此函数, 可将指定的zip实体读入到指定的Byte数组中.
 注意: 该函数会将传入的hEntry对应的CSZIPUTILEntry_S结构体释放掉.
 
Detail Info:

@param[in] hZip: zip文件句柄
@param[in] hEntry: zip实体句柄
@param[int/out] pucBuf: 用于存储指定zip实体内容的Buffer地址
@param[out] pcEntrynm: 输出指定zip实体的名字
@return 
	0: OK
   -1: error
@note
*/
int CSZIPUTILReadEntry(HCSHANDLE hZip, HCSHANDLE hEntry, 
	unsigned char *pucBuf, char *pcEntrynm);

/**
@brief 
 从指定zip文件中获取指定名字对应的zip实体, 如果未找到名字对应的实体,则返回NULL
 
Detail Info:

@param[in] hZip: zip文件句柄
@param[in] pcName: 需查找的zip实体名
@return 
    Normal: 找到的zip实体句柄
	  NULL: 未找到
@note
*/
HCSHANDLE CSZIPUTILGetEntry(HCSHANDLE hZip, const char *pcName);

/**
@brief
 获取指定zip文件中第N个(从0开始)zip实体, 如果指事实上的索引值超出范围
 (如zip文件包含3个实体,传入3则会越界),则返回NULL.
 
Detail Info:

@param[in] hZip: zip文件句柄. 
@param[in] nIndex: 需查找的zip实体索引值.
@return
	Normal: 找到的zip实体句柄
	  NULL: 索引超出范围
@note 
*/
HCSHANDLE CSZIPUTILGetNextEntry(HCSHANDLE hZip, int nIndex);

/**
@brief
 释放指定zip文件中的指定zip实体.
 
Detail Info:
 实际上系统将为每一个zip文件维护一个最近被使用过的zip实体的缓存, 以优化常
 规访问模式.

@param[in] hZip: zip文件句柄.  
@param[in] hEntry: 需释放的zip实体句柄
@return 
@note 
*/
void CSZIPUTILFreeEntry(HCSHANDLE hZip, HCSHANDLE hEntry);

/**
@brief
 解压缩指定的zip文件到指定路径下
 
Detail Info:
@param[in] pcZippath: zip文件所在路径(URL+文件名)
@param[in] pcTarget: 解压缩后文件存储和位置.
@param[out] pnSize: 解压缩后的数据总大小
@return 
	 TRUE: OK
	FALSE: FAILED
@note 
*/
BOOL CSZIPUTILUnzip(char* pcZippath, char* pcTarget, int *pnSize);

/**
@brief
 获取指定zip文件句柄的描述信息结构体.
 
Detail Info:
@param[in] hZip: zip文件句柄.
@param[out] psZipfileInfo: zip文件描述信息结构体
@return 
	 0: OK
	-1: Error
@note 
*/
int CSZIPUTILGetZipFileInfo(HCSHANDLE hZip, CSZIPUTILFile_S *psZipfileInfo);

/**
@brief
 获取指定zip实体句柄的描述信息结构体.
 
Detail Info:
@param[in] hZip: zip实体句柄.
@param[out] psZipEntryInfo: zip实体描述信息结构体
@return 
	 0: OK
	-1: Error
@note 
*/
int CSZIPUTILGetZipEntryInfo(HCSHANDLE hEntry, CSZIPUTILEntry_S *psZipEntryInfo);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* !_CSZIPUTIL_H_ */ 
