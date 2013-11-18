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
@brief 输入法模式
*/
typedef enum _CSIMEModeFlags_E
{
    EM_IMEMODE_CHINESE = 1,         // 中文模式
    EM_IMEMODE_ENGLISH = 2,         // 英文模式
    EM_IMEMODE_VIRTUALKETBOARD = 4, // 虚拟键盘模式
    EM_IMEMODE_USER = 8,            // 用户自定义单词表模式
    EM_IMEMODE_TOOLBAR = 16         // toolBar提示模式
} CSIMEModeFlags_E;

typedef unsigned char imeModeFlags;

/**
@brief 通过输入法接口将字符文字传递给EW600窗口系统
@param[in] eType 传入的文本编码方式
@param[in] pcText 需要显示的文本, 编码方式取决于eType定义
@param[in] nTextLen 需要显示的文本长度, eType为EM_IME_ASCII，则nTextLen为字符长度;
           eType为EM_IME_UNICODE，则nTextLen为字符个数
@note
*/
void CSIMESendText(CSIMECodeType_E eType, const char *pcText, int nTextLen);

/**
@brief 该接口供用户设置输入法模式，
@param[in] ucImeMode 该参数表示输入法模式,为CSIMEModeFlags_E枚举类型的组合
@param[in] pvPath 只要用户设置了EM_IMEMOD_USER模式，为单词表文件的路径，其他模式为NULL
@note
默认为中文模式，英文模式，虚拟键盘模式三种模式
输入法切换的语言模式顺序为：中文模式，英文模式，用户自定义单词表模式,虚拟键盘模式

用户可以自定义选择一种或多种组合模式，例子如下：
仅使用英文文模式和用户自定义单词表模式：
CSIMESetMode(EM_IMEMODE_ENGLISH | EM_IMEMODE_USER)
使用中文、英文模式和输入法提示当前模式toolbar:
CSIMESetMode(EM_IMEMODE_CHINESE | EM_IMEMODE_ENGLISH | EM_IMEMODE_TOOLBAR)

用户自定义单词表配置文件格式定义：
第一行是编码方式：encoding=ascii; 每行数字(1-9)等号之后的内容为数字所对应的字符内容，每行必须以";"结尾
如：
encoding=ascii;
1=! @ # $ ;
2=a b c ch;
3=....
*/
void CSIMESetMode(imeModeFlags ucImeMode, const char *pcPath);

/**
@brief 该接口供用户自定义输入法启动按键键值
@param[in] nInputMethodKey 用户自定义的输入法启动按键键值
@param[in] bEnable TRUE为nInputMethodKey对应的启动键有效，FALSE为无效
@note
    用户可以注册自定义输入法启动键，如可以将红色键CSUDIINPUT_KEY_RED注册为输入法启动按键，默认为CSUDI_VK_INPUT
    此接口可支持多注册，如果键值对应的bEnable为TRUE都可以同时作为启动按键，默认输入法启动键CSUDI_VK_INPUT也同时有效
*/
void CSIMECustomizeInputKey(int nInputMethodKey, BOOL bEnable);


/**
@brief 该接口供用户设置输入法词库文件的绝对地址
@param[in] pcDictPath 用户自定义的输入法词库文件的绝对地址
@param[in] nPathLen词库文件的绝对地址的长度
@note
     不同的工程，dict_pinyin.dat的绝对地址可能不同，用户可通过此接口设置词库文件的地址
     默认地址为/usr/local/etc/data。不调用此接口或不通过配置文件配置，将采用默认地址
*/
void CSIMESetDictPinyinPath(const char* pcDictPath, int nPathLen);

#ifdef __cplusplus
}
#endif
#endif
