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

// ����������Ϣ
typedef struct  // �������ݽṹ
{
            unsigned char aucAccountOut[28+1];     // ת���˻���
            unsigned char aucAccountIn[28+1];      // ת���˻���
            unsigned char aucTransPassword[8+1];   // ��������
            unsigned char aucCardValidDate[4+1];   // ����Ч��
            unsigned char aucDynamicPassword[8+1]; // ��̬����
            unsigned char aucRandomNum[6+1];       // �����
            unsigned char aucCVN2[3+1];            // CVN2ֵ
}TVP_TRANSINFO;

// ���ؽṹ��Ϣ
typedef struct
{
    unsigned char response_code[2+1]; 						// ��Ӧ��
    unsigned char trans_out_xml[4096];
}TVP_ANSWER_INFO;

int process_Txn(char * tpp_addr, int tpp_port, int timeout, unsigned char *trans_in_xml, TVP_TRANSINFO *trans_Info, TVP_ANSWER_INFO *answer_Info, int(*callback_fun)(int, char*));

#ifdef __cplusplus
}
#endif

#endif // TVPPROCESS_H_
