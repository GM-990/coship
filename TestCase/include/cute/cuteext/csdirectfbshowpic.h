#ifndef CSDIRECTFBSHOWPIC_H
#define CSDIRECTFBSHOWPIC_H

#ifdef __cplusplus
extern "C"{
#endif
/**
@brief  ��������:
                ʹ��DirectFB��ʾͼƬ

@param[in]
const char* pcPicPath ͼ���ļ�·�������Զ�ɾ��·��ǰ�õ�"/workdir"

@return
int     QTδ��ʼ��              ���� -4
        �������ָ��Ϊ��        ���� -3
        �����·���ļ�������    ���� -2
        ����·��Ϊ"/workdir"    ���� -1
        �ɹ�                    ���� 0

@exception
@note
*/
int CSDfbShowPicture(const char* pcPicPath);

/**
@brief  ��������:
                ���ʹ��CSDfbShowPicture��ʾ��ͼ�����ݲ��ͷ��ڴ�

@param[in]
        ��
@return
int      �ɹ�                   ����0

@exception
@note
*/
int CSDfbShowClear(void);
#ifdef __cplusplus
}
#endif
#endif // CSDIRECTFBSHOWPIC_H
