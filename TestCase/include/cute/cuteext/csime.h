#ifndef CSIME_H
#define CSIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"

typedef enum _CSIMECodeType_E
{
    EM_IME_ASCII,
    EM_IME_UNICODE
} CSIMECodeType_E;

/**
@brief ���뷨ģʽ
*/
typedef enum _CSIMEModeFlags_E
{
    EM_IMEMODE_CHINESE = 1,         // ����ģʽ
    EM_IMEMODE_ENGLISH = 2,         // Ӣ��ģʽ
    EM_IMEMODE_VIRTUALKETBOARD = 4, // �������ģʽ
    EM_IMEMODE_USER = 8,            // �û��Զ��嵥�ʱ�ģʽ
    EM_IMEMODE_TOOLBAR = 16         // toolBar��ʾģʽ
} CSIMEModeFlags_E;

typedef unsigned char imeModeFlags;

/**
@brief ͨ�����뷨�ӿڽ��ַ����ִ��ݸ�EW600����ϵͳ
@param[in] eType ������ı����뷽ʽ
@param[in] pcText ��Ҫ��ʾ���ı�, ���뷽ʽȡ����eType����
@param[in] nTextLen ��Ҫ��ʾ���ı�����, eTypeΪEM_IME_ASCII����nTextLenΪ�ַ�����;
           eTypeΪEM_IME_UNICODE����nTextLenΪ�ַ�����
@note
*/
void CSIMESendText(CSIMECodeType_E eType, const char *pcText, int nTextLen);

/**
@brief �ýӿڹ��û��������뷨ģʽ��
@param[in] ucImeMode �ò�����ʾ���뷨ģʽ,ΪCSIMEModeFlags_Eö�����͵����
@param[in] pvPath ֻҪ�û�������EM_IMEMOD_USERģʽ��Ϊ���ʱ��ļ���·��������ģʽΪNULL
@note
Ĭ��Ϊ����ģʽ��Ӣ��ģʽ���������ģʽ����ģʽ
���뷨�л�������ģʽ˳��Ϊ������ģʽ��Ӣ��ģʽ���û��Զ��嵥�ʱ�ģʽ,�������ģʽ

�û������Զ���ѡ��һ�ֻ�������ģʽ���������£�
��ʹ��Ӣ����ģʽ���û��Զ��嵥�ʱ�ģʽ��
CSIMESetMode(EM_IMEMODE_ENGLISH | EM_IMEMODE_USER)
ʹ�����ġ�Ӣ��ģʽ�����뷨��ʾ��ǰģʽtoolbar:
CSIMESetMode(EM_IMEMODE_CHINESE | EM_IMEMODE_ENGLISH | EM_IMEMODE_TOOLBAR)

�û��Զ��嵥�ʱ������ļ���ʽ���壺
��һ���Ǳ��뷽ʽ��encoding=ascii; ÿ������(1-9)�Ⱥ�֮�������Ϊ��������Ӧ���ַ����ݣ�ÿ�б�����";"��β
�磺
encoding=ascii;
1=! @ # $ ;
2=a b c ch;
3=....
*/
void CSIMESetMode(imeModeFlags ucImeMode, const char *pcPath);

/**
@brief �ýӿڹ��û��Զ������뷨����������ֵ
@param[in] nInputMethodKey �û��Զ�������뷨����������ֵ
@param[in] bEnable TRUEΪnInputMethodKey��Ӧ����������Ч��FALSEΪ��Ч
@note
    �û�����ע���Զ������뷨������������Խ���ɫ��CSUDIINPUT_KEY_REDע��Ϊ���뷨����������Ĭ��ΪCSUDI_VK_INPUT
    �˽ӿڿ�֧�ֶ�ע�ᣬ�����ֵ��Ӧ��bEnableΪTRUE������ͬʱ��Ϊ����������Ĭ�����뷨������CSUDI_VK_INPUTҲͬʱ��Ч
*/
void CSIMECustomizeInputKey(int nInputMethodKey, BOOL bEnable);


/**
@brief �ýӿڹ��û��������뷨�ʿ��ļ��ľ��Ե�ַ
@param[in] pcDictPath �û��Զ�������뷨�ʿ��ļ��ľ��Ե�ַ
@param[in] nPathLen�ʿ��ļ��ľ��Ե�ַ�ĳ���
@note
     ��ͬ�Ĺ��̣�dict_pinyin.dat�ľ��Ե�ַ���ܲ�ͬ���û���ͨ���˽ӿ����ôʿ��ļ��ĵ�ַ
     Ĭ�ϵ�ַΪ/usr/local/etc/data�������ô˽ӿڻ�ͨ�������ļ����ã�������Ĭ�ϵ�ַ
*/
void CSIMESetDictPinyinPath(const char* pcDictPath, int nPathLen);

#ifdef __cplusplus
}
#endif
#endif
