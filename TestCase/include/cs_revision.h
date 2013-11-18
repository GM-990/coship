#ifndef _CS_REVISION_H_
#define _CS_REVISION_H_

#ifdef __cplusplus
extern "C" {
#endif




/* Function����ȡxxxģ��汾��
Syntax��
	int CSxxxGetVersion( 
		char* 		pcVersion, 
		int 		nSize
		 );
Parameters:
	pcVersion
		�����������Ű汾��Ϣ�Ļ������׵�ַ��
		���ذ汾��Ϣ�ַ�����ʽΪ "xx.xx.xx"��
	nSize
		���������������pcVersion�ĳ���
Return Value:
	1) �������pcVersionΪ�գ����ذ汾��Ϣ��ʵ�ʳ��ȡ� 
	2) �������pcVersion��Ϊ�գ��Һ������óɹ������ذ汾��Ϣ��ʵ�ʳ��ȣ�
	3) ���ʧ�ܣ�����0��
Remarks��
    ��һ�ַ���ֵ��������û��ɲ��Ըú�������Ϊ�ڶ�������nSize�ṩ�ο���
	����û�����ĵڶ�������С�ڰ汾��Ϣ��ʵ�ʳ��ȣ�β��������ֽڱ��ص���
*/

#ifdef MMCP_UDI2
#define UDIVERSTR	"forUDI2.0"
#else
#define UDIVERSTR	"forUDI1.0"
#endif

#define VERSION_FUNCTION(mode,ver)	\
	int CS##mode##GetVersion( char * pcVersion, int nSize )	\
	{	\
		const char* pVer = "MMCP_"#mode"_V"ver"_svn"SRC_VERSION" "UDIVERSTR" "__DATE__" "__TIME__;\
		int nRet = 0, nCopy = 0; \
		int nRealLen = strlen(pVer);\
		\
		if (pcVersion == NULL) \
		{ \
			nRet = nRealLen; \
		} \
		else if (pcVersion!= 0 && nSize > 0)\
		{\
			nCopy = (nSize > nRealLen? nRealLen:nSize-1); \
			memcpy(pcVersion, pVer, nCopy); \
			pcVersion[nCopy] = '\0'; \
			nRet = nRealLen; \
		} \
		return nRet;	\
	}

int CSMMCPGetVersion( char * pcVersion, int nSize );



int CSPortingGetVersion( char * pcVersion, int nSize );

/**
@brief ��ȡ�м��VR�汾��
 ���� : "V100R002"  ����汾����˵��

@param[out] �������汾�ŵĻ������׵�ַ��
@param[in] ������pcVersion�ĳ���
@return 1) �������pcVersionΪ�գ����ذ汾��Ϣ��ʵ�ʳ��ȡ� 
		2) �������pcVersion��Ϊ�գ��Һ������óɹ������ذ汾��Ϣ��ʵ�ʳ��ȣ�
		3) ���ʧ�ܣ�����0��
	��һ�ַ���ֵ��������û��ɲ��Ըú�������Ϊ�ڶ�������nSize�ṩ�ο���
	����û�����ĵڶ�������С�ڰ汾��Ϣ��ʵ�ʳ��ȣ�β��������ֽڱ��ص���
*/
int CSMMCPGetVRVersion( char * pcVersion, int nSize );



#ifdef __cplusplus
}
#endif
/** @} */
#endif

