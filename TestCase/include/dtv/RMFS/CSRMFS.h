
#ifndef _CS_RMFS_H
#define _CS_RMFS_H

#ifdef __cplusplus
extern "C" {
#endif

#define		SHARE_COMPAT		( 0x01 )
#define		CREATE_NEW			( 0x02 )
#define		OPEN_EXISTING		( 0x04 )
#define		OPEN_ALWAYS			( 0x08 )

#define		CSSEEK_CUR			( 1 )
#define		CSSEEK_END			( 2 )
#define		CSSEEK_SET			( 0 )

BOOL		InitRMFS( PBYTE pbDataBuf, DWORD dwDataBufLen );
/*
	初始化文件系统.	根据需要分配给文件系统一块内存.
*/

BOOL	CSDestroyRMFS(void);
/*
	销毁文件系统
 */

HCSHANDLE	CSCreateFile( const char * lpFileName, DWORD dwCreateType );
/*
	dwCreateType有以下属性

	SHARE_COMPAT	：	共享打开	写文件时，不能共享打开，读文件时，建议使用该项
	CREATE_NEW		：	Creates a new file. The function fails if the specified file already exists.
	OPEN_EXISTING	：	Opens the file. The function fails if the file does not exist
	OPEN_ALWAYS		：	Opens the file, if it exists. If the file does not exist, the function creates the file as if CREATE_NEW
*/

BOOL		CSDeleteFile( const char * lpFileName ); 
/*
	永久删除文件,当有打开的文件句柄没有完全释放时，删除文件不成功
*/

BOOL		CSGetFileSize( HCSHANDLE hFile, DWORD* pdwFileLen );
/*
	获取文件的长度
*/

BOOL		CSSeekFile( HCSHANDLE hFile, int nOffSet, DWORD dwFrom );
/*
	移动文件当前位置指针
 */

BOOL		CSWriteFile( HCSHANDLE hFile, PBYTE pucData, DWORD dwDataLen );
/*
	写文件，如果nOffSet大于文件的长度，将添加到文件的尾部
*/


BOOL		CSReadFile( HCSHANDLE hFile, PBYTE pucOutData, DWORD dwReadDataLen, DWORD* pdwReadDataLen );
/*
	读文件，pdwReadDataLen返回实际读取文件的字节数
*/

BOOL		CSIsEofFile( HCSHANDLE hFile );
/*
	判断文件是否结束
*/

BOOL		CSCloseFile( HCSHANDLE hFile );
/*
	释放文件句柄
*/

int			CSDeleteLongTimeNotUsedFile( DWORD dwDeleteSize );
/*
	删除长时间没有使用的文件使之释放内存空间，dwDeleteSize为释放内存空间的字节数，返回值为实际释放的内存空间大小
*/

int			CSRMFSGetAviSize(void);
/*
返回文件系统可用的内存空间大小。包括被释放的group中的内存
*/

int			CSRMFSGetFreeSize(void);
/*
	返回文件系统还可分配的内存空间大小。不包括被释放的group中的内存。
*/
int CSUSPRMFSGetVersionInfo( char * pcVersionInfo, int nSize );
/*
获得文件系统版本
*/
#ifdef __cplusplus
}
#endif

#endif
