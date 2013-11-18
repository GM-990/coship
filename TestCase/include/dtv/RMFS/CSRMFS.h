
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
	��ʼ���ļ�ϵͳ.	������Ҫ������ļ�ϵͳһ���ڴ�.
*/

BOOL	CSDestroyRMFS(void);
/*
	�����ļ�ϵͳ
 */

HCSHANDLE	CSCreateFile( const char * lpFileName, DWORD dwCreateType );
/*
	dwCreateType����������

	SHARE_COMPAT	��	�����	д�ļ�ʱ�����ܹ���򿪣����ļ�ʱ������ʹ�ø���
	CREATE_NEW		��	Creates a new file. The function fails if the specified file already exists.
	OPEN_EXISTING	��	Opens the file. The function fails if the file does not exist
	OPEN_ALWAYS		��	Opens the file, if it exists. If the file does not exist, the function creates the file as if CREATE_NEW
*/

BOOL		CSDeleteFile( const char * lpFileName ); 
/*
	����ɾ���ļ�,���д򿪵��ļ����û����ȫ�ͷ�ʱ��ɾ���ļ����ɹ�
*/

BOOL		CSGetFileSize( HCSHANDLE hFile, DWORD* pdwFileLen );
/*
	��ȡ�ļ��ĳ���
*/

BOOL		CSSeekFile( HCSHANDLE hFile, int nOffSet, DWORD dwFrom );
/*
	�ƶ��ļ���ǰλ��ָ��
 */

BOOL		CSWriteFile( HCSHANDLE hFile, PBYTE pucData, DWORD dwDataLen );
/*
	д�ļ������nOffSet�����ļ��ĳ��ȣ�����ӵ��ļ���β��
*/


BOOL		CSReadFile( HCSHANDLE hFile, PBYTE pucOutData, DWORD dwReadDataLen, DWORD* pdwReadDataLen );
/*
	���ļ���pdwReadDataLen����ʵ�ʶ�ȡ�ļ����ֽ���
*/

BOOL		CSIsEofFile( HCSHANDLE hFile );
/*
	�ж��ļ��Ƿ����
*/

BOOL		CSCloseFile( HCSHANDLE hFile );
/*
	�ͷ��ļ����
*/

int			CSDeleteLongTimeNotUsedFile( DWORD dwDeleteSize );
/*
	ɾ����ʱ��û��ʹ�õ��ļ�ʹ֮�ͷ��ڴ�ռ䣬dwDeleteSizeΪ�ͷ��ڴ�ռ���ֽ���������ֵΪʵ���ͷŵ��ڴ�ռ��С
*/

int			CSRMFSGetAviSize(void);
/*
�����ļ�ϵͳ���õ��ڴ�ռ��С���������ͷŵ�group�е��ڴ�
*/

int			CSRMFSGetFreeSize(void);
/*
	�����ļ�ϵͳ���ɷ�����ڴ�ռ��С�����������ͷŵ�group�е��ڴ档
*/
int CSUSPRMFSGetVersionInfo( char * pcVersionInfo, int nSize );
/*
����ļ�ϵͳ�汾
*/
#ifdef __cplusplus
}
#endif

#endif
