#ifndef __COPYRIGHTNOTICE_H__
#define __COPYRIGHTNOTICE_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
   @brief 设置盗版黑屏调用函数.
   Detail Info: 该函数定时将当前屏幕黑屏一次, 并显示用户设置的提示文字.
   @param[in] pcInfoStr: 提示文字内容
   @param[in] nInterval: 每次黑屏间隔时间
   @param[in] nDuration: 每次黑屏持续时间
   @param[in] nInfoStrLen: 提示文字内容长度
   @param[in] nCodeType: 字符编码格式
   @param[in] nTextSize: 提示文字的大小
   @param[in] nBgColor: 背景色(黑屏还是其它颜色屏)
   @param[in] nTextColor: 提示文字颜色
   @param[out] 无
   @return
   -1: 错误
    0: 执行正确
   @note nCodeType字符编码格式目前只支持ASCII
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

