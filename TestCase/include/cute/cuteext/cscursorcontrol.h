#ifndef CSCURSORCONTROL_H
#define CSCURSORCONTROL_H

#ifdef __cplusplus
#include "cutepublic.h"
#include "cutesingleton.h"

CSCUTE_PRIVATE_CLASS(CSCursorControl)

//**********************| C++ interface |**********************
class CSCUTE_DECL_EXPORT CSCursorControl: public CSCuteSingleton<CSCursorControl>
{
    CSCUTE_DECLARE_PRIVATE(CSCursorControl)
public:
    CSCursorControl();
    ~CSCursorControl();

/**
@brief  ��������:
        ʹ���Զ����ع�깦��
        Ĭ�Ϲ�걣����ʾʱ��Ϊ2S������Ч�ʵĿ��ǻ���0-200ms�����
       �����ڰ汾�ĸı��Ĭ��ֵ�������ܻ�ı䣬�����Ҫ��ʹ��setKeepTime���ù�걣����ʾ��ʱ�䣩

@param[in]
int nFlag   �Զ����ع�깦�ܵĿ��ر�־��0Ϊ�رգ�1Ϊ����

@return
int         ���õ�nFlag����ֵ��֮ǰ���õ�ֵһ�£�����   0
            ����Ϊ�޷�ʶ��Ĳ�����Ŀǰֻʶ��0��1������  -1
            ���óɹ�����                            1

@exception
@note
*/
    int enableAutoHideCursor(int nFlag);

/**
@brief  ��������:
        ���ù�걣����ʾ��ʱ�䣬Ŀǰ������ʱ��ֵС��300ms���Զ���Ϊ������ʾʱ��Ϊ300ms���Զ����ع�깦�ܿ�����

@param[in]
int nTime   ��걣����ʾ��ʱ�䣬��λΪms

@return
int         �������Ϊ��ֵ����            -1
            ���õ�ֵ�뵱ǰʹ�õ�ֵ��ͬ����   0
            ���óɹ�����                  1

@exception
@note
*/
    int setKeepTime(int nTime);

/**
@brief  ��������:
        ���ع��

@param[in]
            ��

@return
int         ִ�гɹ�����                            1
            �������Ѿ�������״̬�������κζ�������    0

@exception
@note
*/
    int hideCursor();

/**
@brief  ��������:
        ��ʾ���

@param[in]
            ��

@return
int         ִ�гɹ�����                            1
            �������Ѿ�����ʾ״̬�������κζ�������    0

@exception
@note
*/
    int showCursor();

/**
@brief  ��������:
        �ƶ���꣨Ŀǰ��������ƽ̨�������޷��������У�

@param[in]
int nX      ���ù���ƶ�λ�õ�x��������ֵ
int nY      ���ù���ƶ�λ�õ�y��������ֵ

@return
int         ִ�гɹ�����                      1
            ������������뵱ǰ���λ��һ�·���   0

@exception
@note
*/
    int moveCursor(int nX, int nY);

/**
@brief  ��������:
        ��ȡ��ǰ���λ��

@param[out]
int nX      ��ȡ��ǰ���λ�õ�x��������ֵ
int nY      ��ȡ��ǰ���λ�õ�y��������ֵ

@return
int         ����ָ��pnX��pnYΪNULL����  -1
            ִ�гɹ�����                1

@exception
@note
*/
    int getCursorPos(int* pnX, int* pnY);

/**
=============================================================
    3 static public members inherited from CSCuteSingleton
=============================================================

@brief  ��������:
        ��ù��������Ψһʵ�������ʵ���������ڽ�����һ������������򷵻�ʵ��ָ��

@param[in]
        ��

@return
CSCursorControl*  ���ع�����Ψһʵ��ָ��

@exception
@note

    static CSCursorControl* getInstance();

@brief  ��������:
        ��ù��������Ψһʵ����ָ�룬���ʵ�������ڲ��ᴴ��������NULL

@param[in]
        ��

@return
CSCursorControl*  ���ع�����Ψһʵ��ָ��

@exception
@note

    static CSCursorControl* instance();


@brief  ��������:
        �ͷŹ�������Ψһʵ��

@param[in]
        ��

@return
int         ʵ�����ڲ��ɹ�ɾ������     1
            ʵ�������ڷ���           0

@exception
@note
    static int releaseInstance();
*/
};
#endif

#ifdef __cplusplus
CSCUTE_BEGIN_C_HEADER
#endif

//**********************| C interface |**********************
/**
@brief  ��������:
        ʹ���Զ����ع�깦�ܣ�Ĭ�Ϲ�걣����ʾʱ��Ϊ2S������Ч�ʵĿ��ǻ���0-200ms�����
       �����ڰ汾�ĸı��Ĭ��ֵ�������ܻ�ı䣬�����Ҫ��ʹ��CSQTSetKeepTime���ù�걣����ʾ��ʱ�䣩

@param[in]
int nFlag   �Զ����ع�깦�ܵĿ��ر�־��0Ϊ�رգ�1Ϊ����

@return
int         ���õ�nFlag����ֵ��֮ǰ���õ�ֵһ�£�����   0
            ����Ϊ�޷�ʶ��Ĳ�����Ŀǰֻʶ��0��1������  -1
            ���óɹ�����                            1

@exception
@note
*/
    int CSCuteEnableAutoHideCursor(int nFlag);

/**
@brief  ��������:
        ���ù�걣����ʾ��ʱ�䣬Ŀǰ������ʱ��ֵС��300ms���Զ���Ϊ������ʾʱ��Ϊ300ms���Զ����ع�깦�ܿ�����

@param[in]
int nTime   ��걣����ʾ��ʱ�䣬��λΪms

@return
int         �������Ϊ��ֵ����            -1
            ���õ�ֵ�뵱ǰʹ�õ�ֵ��ͬ����   0
            ���óɹ�����                  1

@exception
@note
*/
    int CSCuteSetKeepTime(int nTime);

/**
@brief  ��������:
        ���ع��

@param[in]
            ��

@return
int         ִ�гɹ�����                            1
            �������Ѿ�������״̬�������κζ�������    0

@exception
@note
*/
    int CSCuteHideCursor();

/**
@brief  ��������:
        ��ʾ���

@param[in]
            ��

@return
int         ִ�гɹ�����                            1
            �������Ѿ�����ʾ״̬�������κζ�������    0

@exception
@note
*/
    int CSCuteShowCursor();

/**
@brief  ��������:
        �ƶ���꣨Ŀǰ��������ƽ̨�������޷��������У�

@param[in]
int nX      ���ù���ƶ�λ�õ�x��������ֵ
int nY      ���ù���ƶ�λ�õ�y��������ֵ

@return
int         ִ�гɹ�����                      1
            ������������뵱ǰ���λ��һ�·���   0

@exception
@note
*/
    int CSCuteMoveCursor(int nX, int nY);

/**
@brief  ��������:
        ��ȡ��ǰ���λ��

@param[out]
int nX      ��ȡ��ǰ���λ�õ�x��������ֵ
int nY      ��ȡ��ǰ���λ�õ�y��������ֵ

@return
int         ����ָ��pnX��pnYΪNULL����  -1
            ִ�гɹ�����                1

@exception
@note
*/
    int CSCuteGetCursorPos(int* pnX, int* pnY);

/**
@brief  ��������:
        �ͷ�ʹ�ù����ƹ�����ռ�õ���Դ

@param[in]
        ��

@return
int         �ɹ��ͷ���Դ����                 1
            û����Դ��ռ��û�б�Ҫ�ͷŷ���     0

@exception
@note
*/
    int CSCuteReleaseCursorControl();

#ifdef __cplusplus
CSCUTE_END_C_HEADER
#endif

#endif // CSCURSORCONTROL_H
