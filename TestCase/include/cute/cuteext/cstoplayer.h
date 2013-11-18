#ifndef CSTOPLAYER_H
#define CSTOPLAYER_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum _CSTLCodeType_E
{
    EM_TL_ASCII,
    EM_TL_UNICODE
} CSTLCodeType_E;

/**@brief 顶层窗体属性*/
typedef struct _CSTopLayerProperty_S
{
    const char *m_pBgImg; // 设置窗体背景图片路径 必须使用ascII
    int m_nBgColor;       // 以ARGB方式设置窗体背景色 （在不设置背景图片的时候有效）
    int m_nWidth;         // 设置窗体宽
    int m_nHeight;        // 设置窗体高
    int m_nXPos;          // 设置窗体相对屏幕左上角X轴坐标
    int m_nYPos;          // 设置窗体相对屏幕左上角Y轴坐标

    int m_nDelayTime;     // 在调用CSTLShowTopLayer显示toplayer后，设置窗体显示延时间，单位为ms，如果是0表示立即显示窗体
    int m_nInterval;      // 在调用CSTLShowTopLayer显示toplayer后，设置窗体闪烁的间隔时间，单位为ms，0表示不闪烁，非0以设置的时间间隔闪烁
    int m_nDuration;      // 在调用CSTLShowTopLayer显示toplayer后，设置顶层窗口持续显示时间，单位为ms，0表示持续显示

    int  m_nFontSize;     // 显示的字体大小
    int  m_nTextColor;    // 以RGB方式保存要显示的字体颜色

    const char *m_pText;  // 设置需要显示的文字
    int  m_nTextLen;      // 设置要显示文本的字符串长度，m_pText编码为unicode时，m_nTextLen为字符字节数
    int  m_nTextXPos;     // 设置要显示的文本相对窗体的X坐标
    int  m_nTextYPos;     // 设置要显示的文本相对窗体的Y坐标
    
    int  m_nFontBold;     // 显示字体是否为粗体，0表示字体为非粗体；1表示字体为粗体
    int  m_nFontItalic;   // 显示字体是否为斜体，0表示字体为非斜体；1表示字体为斜体
    CSTLCodeType_E m_codeType;  // 指定编码格式，对m_text有效
} CSTopLayerProperty_S;


/**
@brief 创建窗体，设置窗体属性

@param[in] pProperty 设置窗体的属性
@return 返回创建的顶层窗体ID号 0表示创建失败
@note 调用该函数不会显示窗体，只有在调用CSTLSetVisible函数后才会显示
*/
unsigned int CSTLCreateLayer(CSTopLayerProperty_S *psProperty);

/**
@brief 设置窗体之间的Z序

@param[in] unLayerId  窗体的ID号
@param[in] nLayerIndex   指定窗体显示的层
@note
*/
void CSTLSetLayerZorder(unsigned int unLayerId, unsigned int unLayerIndex);

/**
@brief 按照窗体ID号显示或者隐藏该窗体

@param[in] unLayerId 窗体的ID号
@param[in] nVisible 标示是显示还是隐藏该窗体,0表示不显示；1表示显示
*/
void CSTLSetVisible(unsigned int unLayerId, int nVisible);

/**
@brief 在窗体指定位置画图

@param[in] unLayerId 顶层窗体的ID号
@param[in] nXPos 图片显示在相对于顶层窗体的X轴坐标
@param[in] nYPos 图片显示在相对于顶层窗体的Y轴坐标
@param[in] nWidth  图片在窗体上显示的宽
@param[in] nHeight 图片在窗体上显示的高
@param[in] pPath   需要显示的图片路径，编码为ascII
@note
*/
void CSTLDrawImage(unsigned int unLayerId, int nXPos, int nYPos, int nWidth, int nHeight, const char *pPath);

/**
@brief 在窗体指定位置显示文字

@param[in] unLayerId 顶层窗体的ID号
@param[in] nXPos 文本显示在相对于顶层窗体的X轴坐标
@param[in] nYPos 文本显示在相对于顶层窗体的Y轴坐标
@param[in] nText  需要显示的文字，编码类型为CSTLCreateLayer设置的属性一致
@param[in] nTextLen 设置传入的文本字符串长度，如果是unicode编码，需要设置传入的文本字符字节数
@note
*/
void CSTLDrawText(unsigned int unLayerId, int nXPos, int nYPos, const char *pText, int nTextLen);

/**
@brief 按照顶层窗体的ID号销毁该窗体
@param[in] unLayerId 顶层窗体的ID号
*/
void CSTLDestroyLayer(unsigned int unLayerId);

#ifdef __cplusplus
}
#endif

#endif // CSTOPLAYER_H
