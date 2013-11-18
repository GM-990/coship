
#ifndef _CS_SMOOTHMOVE_H
#define _CS_SMOOTHMOVE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _CSSmoothMoveMsgType
{
	SMOOTHMOVE_MSG_HIDE, //�����������Ϣ
	SMOOTHMOVE_MSG_STOP, //�����ֹͣ��Ϣ
	SMOOTHMOVE_MSG_OVER  //����ƹ��������Ϣ
}CSSmoothMoveMsgType;

typedef void (*CSSmoothMoveCallback)(CSSmoothMoveMsgType emSmoothMoveMsgType, PVOID pvData);

/*����������������������������������������������
* ��������CSSmoothMoveRegisterCallback
* �������ܣ�ע������ƻص�����
* ����˵����
*		hSmoothMove����IN�������ʵ���ľ��
*		pfnCallback����IN���ص�����ָ��
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSSmoothMoveRegisterCallback(HCSHANDLE hSmoothMove, CSSmoothMoveCallback pfnCallback);


/*����������������������������������������������
* ��������CSSmoothMoveGetVersion
* �������ܣ��������ƿ�İ汾�ţ�
* ����˵����
*		pszVersionBuf����OUT����õİ汾�ŵ��׵�ַ
*		dwBufSize����IN�������ģ��汾�ŵ��ַ�������
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSSmoothMoveGetVersion(char *pszVersionBuf, DWORD dwBufSize);

/*����������������������������������������������
* ��������CSGetSmoothMoveMinMemory
* �������ܣ���ÿ���һ�������ʵ����Ҫ����С�ڴ�
* ����˵����
*		nMaxInputStrLen����IN�������ַ�������󳤶�
*		nStrWidht����IN��������
*		nStrHeight����IN������߶�
* ����ֵ��
*		����0
* ע�����
*		�ýӿ�Ϊ�����ӿڣ�����ֵ��ԶΪ��
����������������������������������������������*/
int CSGetSmoothMoveMinMemory(int nMaxInputStrLen, int nStrWidht, int nStrHeight);

/*����������������������������������������������
* ��������CSGetSmoothMoveMinMemoryEx
* �������ܣ���ÿ���һ�������ʵ����Ҫ����С�ڴ�
* ����˵����
*		nMaxInputStrLen����IN�������ַ�������󳤶�
*		nStrWidht����IN��������
*		nStrHeight����IN������߶�
* ����ֵ��
*		������Ҫ����С�ڴ�
* ע�����
*		��
����������������������������������������������*/
int CSGetSmoothMoveMinMemoryEx(int nMaxInputStrLen, int nStrWidth, int nStrHeight, DWORD dwOsdWidth, DWORD dwOsdHeight);

/*����������������������������������������������
* ��������CSGetSmoothMoveMinMemoryEx2
* �������ܣ���ÿ���һ�������ʵ����Ҫ����С�ڴ�
* ����˵����
*		nMaxInputStrLen����IN�������ַ�������󳤶�
*		nStrWidht����IN��������
*		nStrHeight����IN������߶�
*		dwOsdWidth����IN���������ʾ������
*		dwOsdHeight����IN���������ʾ����߶�
*		nMoveModel����IN��������ʽ����-�������У����������ϣ����������ң����������£�������������У�
* ����ֵ��
*		������Ҫ����С�ڴ�
* ע�����
*		��
����������������������������������������������*/
int CSGetSmoothMoveMinMemoryEx2(int nMaxInputStrLen, int nStrWidth, int nStrHeight, DWORD dwOsdWidth, DWORD dwOsdHeight, int nMoveModel);

/*����������������������������������������������
* ��������CSOpenSmoothMove
* �������ܣ�����һ�������ʵ��
* ����˵����
*		pucDataBuf����OUT�������ַ�������󳤶�
*		dwDataBufLen����IN�������˽���ڴ�Ĵ�С
*		nMaxInputStrLen����IN�������Ҫ��ʾ�Ŀ��ֽڵĸ���
*		nStrWidth����IN���ַ���ʾ�Ŀ��
*		nStrHeight����IN���ַ���ʾ�ĸ߶�
*		pucShowBuf����IN���Դ��ַ
*		nOneLineBufLen����IN���洢һ����ʾ������Ҫ���ֽ���
*		nShowBits����IN����λɫ
*		dwOsdHeight����IN���������ʾ����߶�
*		dwBKColor����IN������ɫRGBAֵ
*		dwTextColor����IN����ʾ���ֵ���ɫ
*		hFont����IN���ֿ�ʵ�����
*		dwStepTimeOut����IN��ˢ��ʱ����
* ����ֵ��
*		���������ʵ�����
* ע�����
*		��
����������������������������������������������*/
HCSHANDLE CSOpenSmoothMove(PBYTE pucDataBuf, DWORD dwDataBufLen, int nMaxInputStrLen, int nStrWidth, int nStrHeight, PBYTE pucShowBuf, int nOneLineBufLen, int nShowBits, DWORD dwOsdHeight, DWORD dwBKColor, DWORD dwTextColor, HCSHANDLE hFont, DWORD dwStepTimeOut);

/*����������������������������������������������
* ��������CSOpenSmoothMoveEx
* �������ܣ�����һ�������ʵ����չ�ӿ�
* ����˵����
*		pucDataBuf����OUT�������ַ�������󳤶�
*		dwDataBufLen����IN�������˽���ڴ�Ĵ�С
*		nMaxInputStrLen����IN�������Ҫ��ʾ�Ŀ��ֽڵĸ���
*		nStrWidth����IN���ַ���ʾ�Ŀ��
*		nStrHeight����IN���ַ���ʾ�ĸ߶�
*		pbShowAddr����IN���Դ��ַ
*		nShowBits����IN����λɫ
*		dwBKColor����IN������ɫRGBAֵ
*		dwTextColor����IN����ʾ���ֵ���ɫ
*		hFont����IN���ֿ�ʵ�����
*		dwStepTimeOut����IN��ˢ��ʱ����
*		prcSmoothMove����IN���������ʾ����
*		nMoveModel����IN������ƹ���ģʽ
*		nTaskPriority����IN�������ʵ�����ȼ�
* ����ֵ��
*		���������ʵ�����
* ע�����
*		��
����������������������������������������������*/
HCSHANDLE CSOpenSmoothMoveEx(PBYTE pucDataBuf, DWORD dwDataBufLen, int nMaxInputStrLen, int nStrWidth, int nStrHeight, PBYTE pbShowAddr, int nShowBits, DWORD dwBKColor, DWORD dwTextColor, HCSHANDLE hFont, DWORD dwStepTimeOut, CSRECT *prcSmoothMove, int nMoveModel, int nTaskPriority);

/*����������������������������������������������
* ��������CSCloseSmoothMove
* �������ܣ��ر������ʵ��
* ����˵����
*		hSmoothMove����IN�������ʵ�����
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSCloseSmoothMove(HCSHANDLE hSmoothMove);

/*����������������������������������������������
* ��������CSStartSmoothMove
* �������ܣ�������ʾ�����
* ����˵����
*		dwShowTime����IN������Ƴ�����ʾʱ��
*		hSmoothMove����IN�������ʵ�����
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSStartSmoothMove(DWORD dwShowTime, HCSHANDLE hSmoothMove);

/*����������������������������������������������
* ��������CSStartSmoothMoveEx
* �������ܣ�������ʾ�����
* ����˵����
*		hSmoothMove����IN�������ʵ�����
*		dwShowTime����IN������Ƴ�����ʾʱ��
*		dwMoveTimes����IN����������
*		bOnePerPage����IN���Ƿ�������
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		���������bOnePerPage ΪTRUE��������ư������ù���dwMoveTimes�����û����õĹ���ʱ�䲻������;
*		�������bOnePerPage ΪFALSE��������ƹ�����ʱ��ΪdwShowTime,�û����õĹ�������dwShowTime�������á�
����������������������������������������������*/
BOOL CSStartSmoothMoveEx(HCSHANDLE hSmoothMove, DWORD dwShowTime, DWORD dwMoveTimes, BOOL bOnePerPage);

/*����������������������������������������������
* ��������CSStopSmoothMove
* �������ܣ������ֹͣ��ʾ
* ����˵����
*		hSmoothMove����IN�������ʵ�����
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSStopSmoothMove(HCSHANDLE hSmoothMove);

/*����������������������������������������������
* ��������CSShowSmoothMove
* �������ܣ�Hide֮�����»ָ���ʾ
* ����˵����
*		hSmoothMove����IN�������ʵ�����
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSShowSmoothMove(HCSHANDLE hSmoothMove);

/*����������������������������������������������
* ��������CSHideSmoothMove
* �������ܣ������������ʾ
* ����˵����
*		hSmoothMove����IN�������ʵ�����
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSHideSmoothMove(HCSHANDLE hSmoothMove);

/*����������������������������������������������
* ��������CSSetInputStr
* �������ܣ������������ʾ�ַ���
* ����˵����
*		pcInputStr����IN����ʾ���ݵ�ָ��
*		hSmoothMove����IN�������ʵ�����
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSSetInputStr(const char* pcInputStr, HCSHANDLE hSmoothMove);

/*����������������������������������������������
* ��������CSSetInputStrEx
* �������ܣ������������ʾ�ַ���
* ����˵����
*		pcInputStr����IN����ʾ���ݵ�ָ��
*		hSmoothMove����IN�������ʵ�����
*		nchStrLen����IN�� �ַ������ȣ���СΪ�û������ַ����ĳ���
*	  ��nSpaceLen����IN���ַ�����ո���
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSSetInputStrEx(HCSHANDLE hSmoothMove, const char* pcInputStr, int nchStrLen, int nSpaceLen);

/*����������������������������������������������
* ��������CSClearInputStr
* �������ܣ������ʾ���ݣ�����ˢ����ʾ����
* ����˵����
*		hSmoothMove����IN�������ʵ�����
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSClearInputStr(HCSHANDLE hSmoothMove);


/*����������������������������������������������
* ��������CSSetOneStepPixel
* �������ܣ�����ÿ�����������ظ���
* ����˵����
*		hSmoothMove��IN���������ʵ�����
*		nOneStepPixel��IN����ÿ�����������ظ���
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSSetOneStepPixel(HCSHANDLE hSmoothMove, int nOneStepPixel);


/*����������������������������������������������
* ��������CSSetSmoothFirstShowMode
* �������ܣ�����ÿ�����������ظ���
* ����˵����
*		hSmoothMove��IN���������ʵ�����
*		bShowAllAtFirst��IN������ʼ��ʾ��ʱ���Ƿ���ʾȫ��
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSSetSmoothFirstShowMode( HCSHANDLE hSmoothMove, BOOL bShowAllAtFirst );

/*����������������������������������������������
* ��������CSSetSmoothCharInterSpaceAndLineSpace
* �������ܣ�����ÿ�����������ظ���
* ����˵����
*		hSmoothMove��IN���������ʵ�����
*		nCharInterSpace��IN��: �ַ����
*	    nLineSpace��IN��:      �ַ��о�
* ����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSSetSmoothCharInterSpaceAndLineSpace( HCSHANDLE hSmoothMove, int nCharInterSpace, int nLineSpace );

#ifdef __cplusplus
}
#endif

#endif

