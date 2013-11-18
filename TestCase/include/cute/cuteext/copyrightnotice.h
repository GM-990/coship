#ifndef __COPYRIGHTNOTICE_H__
#define __COPYRIGHTNOTICE_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
   @brief ���õ���������ú���.
   Detail Info: �ú�����ʱ����ǰ��Ļ����һ��, ����ʾ�û����õ���ʾ����.
   @param[in] pcInfoStr: ��ʾ��������
   @param[in] nInterval: ÿ�κ������ʱ��
   @param[in] nDuration: ÿ�κ�������ʱ��
   @param[in] nInfoStrLen: ��ʾ�������ݳ���
   @param[in] nCodeType: �ַ������ʽ
   @param[in] nTextSize: ��ʾ���ֵĴ�С
   @param[in] nBgColor: ����ɫ(��������������ɫ��)
   @param[in] nTextColor: ��ʾ������ɫ
   @param[out] ��
   @return
   -1: ����
    0: ִ����ȷ
   @note nCodeType�ַ������ʽĿǰֻ֧��ASCII
   */
typedef struct _CSCopyRightNoticeInfo_S {
    char *pcInfoStr;
    int nInterval;
    int nDuration;
    int nInfoStrLen;
    int nCodeType;
    int nTextSize;
    unsigned int nBgColor;
    unsigned int nTextColor;
}CSCopyRightNoticeInfo_S;

int CSCopyrightNotice(CSCopyRightNoticeInfo_S *info);

#ifdef __cplusplus
}
#endif

#endif

