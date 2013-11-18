//=============================================================================
//  Name: TvpProcess.h
//  Memo:
// ----------------------------------------------------------------------------
//   Copyright: Copyright 2011 - 2011 Landi Inc
// ----------------------------------------------------------------------------
//  Date           Version        Author         Remark
//  04/18/2011     1.00           Dengxc
//=============================================================================

#ifndef TVPPROCESS_H_
#define TVPPROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif

// 交易敏感信息
typedef struct  // 敏感数据结构
{
            unsigned char aucAccountOut[28+1];     // 转出账户号
            unsigned char aucAccountIn[28+1];      // 转入账户号
            unsigned char aucTransPassword[8+1];   // 交易密码
            unsigned char aucCardValidDate[4+1];   // 卡有效期
            unsigned char aucDynamicPassword[8+1]; // 动态密码
            unsigned char aucRandomNum[6+1];       // 随机数
            unsigned char aucCVN2[3+1];            // CVN2值
}TVP_TRANSINFO;

// 返回结构信息
typedef struct
{
    unsigned char response_code[2+1]; 						// 响应吗
    unsigned char trans_out_xml[4096];
}TVP_ANSWER_INFO;

int process_Txn(char * tpp_addr, int tpp_port, int timeout, unsigned char *trans_in_xml, TVP_TRANSINFO *trans_Info, TVP_ANSWER_INFO *answer_Info, int(*callback_fun)(int, char*));

#ifdef __cplusplus
}
#endif

#endif // TVPPROCESS_H_
