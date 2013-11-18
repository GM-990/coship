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

/**@brief ���㴰������*/
typedef struct _CSTopLayerProperty_S
{
    const char *m_pBgImg; // ���ô��屳��ͼƬ·�� ����ʹ��ascII
    int m_nBgColor;       // ��ARGB��ʽ���ô��屳��ɫ ���ڲ����ñ���ͼƬ��ʱ����Ч��
    int m_nWidth;         // ���ô����
    int m_nHeight;        // ���ô����
    int m_nXPos;          // ���ô��������Ļ���Ͻ�X������
    int m_nYPos;          // ���ô��������Ļ���Ͻ�Y������

    int m_nDelayTime;     // �ڵ���CSTLShowTopLayer��ʾtoplayer�����ô�����ʾ��ʱ�䣬��λΪms�������0��ʾ������ʾ����
    int m_nInterval;      // �ڵ���CSTLShowTopLayer��ʾtoplayer�����ô�����˸�ļ��ʱ�䣬��λΪms��0��ʾ����˸����0�����õ�ʱ������˸
    int m_nDuration;      // �ڵ���CSTLShowTopLayer��ʾtoplayer�����ö��㴰�ڳ�����ʾʱ�䣬��λΪms��0��ʾ������ʾ

    int  m_nFontSize;     // ��ʾ�������С
    int  m_nTextColor;    // ��RGB��ʽ����Ҫ��ʾ��������ɫ

    const char *m_pText;  // ������Ҫ��ʾ������
    int  m_nTextLen;      // ����Ҫ��ʾ�ı����ַ������ȣ�m_pText����Ϊunicodeʱ��m_nTextLenΪ�ַ��ֽ���
    int  m_nTextXPos;     // ����Ҫ��ʾ���ı���Դ����X����
    int  m_nTextYPos;     // ����Ҫ��ʾ���ı���Դ����Y����
    
    int  m_nFontBold;     // ��ʾ�����Ƿ�Ϊ���壬0��ʾ����Ϊ�Ǵ��壻1��ʾ����Ϊ����
    int  m_nFontItalic;   // ��ʾ�����Ƿ�Ϊб�壬0��ʾ����Ϊ��б�壻1��ʾ����Ϊб��
    CSTLCodeType_E m_codeType;  // ָ�������ʽ����m_text��Ч
} CSTopLayerProperty_S;


/**
@brief �������壬���ô�������

@param[in] pProperty ���ô��������
@return ���ش����Ķ��㴰��ID�� 0��ʾ����ʧ��
@note ���øú���������ʾ���壬ֻ���ڵ���CSTLSetVisible������Ż���ʾ
*/
unsigned int CSTLCreateLayer(CSTopLayerProperty_S *psProperty);

/**
@brief ���ô���֮���Z��

@param[in] unLayerId  �����ID��
@param[in] nLayerIndex   ָ��������ʾ�Ĳ�
@note
*/
void CSTLSetLayerZorder(unsigned int unLayerId, unsigned int unLayerIndex);

/**
@brief ���մ���ID����ʾ�������ظô���

@param[in] unLayerId �����ID��
@param[in] nVisible ��ʾ����ʾ�������ظô���,0��ʾ����ʾ��1��ʾ��ʾ
*/
void CSTLSetVisible(unsigned int unLayerId, int nVisible);

/**
@brief �ڴ���ָ��λ�û�ͼ

@param[in] unLayerId ���㴰���ID��
@param[in] nXPos ͼƬ��ʾ������ڶ��㴰���X������
@param[in] nYPos ͼƬ��ʾ������ڶ��㴰���Y������
@param[in] nWidth  ͼƬ�ڴ�������ʾ�Ŀ�
@param[in] nHeight ͼƬ�ڴ�������ʾ�ĸ�
@param[in] pPath   ��Ҫ��ʾ��ͼƬ·��������ΪascII
@note
*/
void CSTLDrawImage(unsigned int unLayerId, int nXPos, int nYPos, int nWidth, int nHeight, const char *pPath);

/**
@brief �ڴ���ָ��λ����ʾ����

@param[in] unLayerId ���㴰���ID��
@param[in] nXPos �ı���ʾ������ڶ��㴰���X������
@param[in] nYPos �ı���ʾ������ڶ��㴰���Y������
@param[in] nText  ��Ҫ��ʾ�����֣���������ΪCSTLCreateLayer���õ�����һ��
@param[in] nTextLen ���ô�����ı��ַ������ȣ������unicode���룬��Ҫ���ô�����ı��ַ��ֽ���
@note
*/
void CSTLDrawText(unsigned int unLayerId, int nXPos, int nYPos, const char *pText, int nTextLen);

/**
@brief ���ն��㴰���ID�����ٸô���
@param[in] unLayerId ���㴰���ID��
*/
void CSTLDestroyLayer(unsigned int unLayerId);

#ifdef __cplusplus
}
#endif

#endif // CSTOPLAYER_H
