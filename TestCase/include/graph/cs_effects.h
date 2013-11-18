/**@defgroup Effects �ṩC��2Dͼ����Ч�任����, ����������Ч�����ţ�����ȣ��������Ч���ҳ���ȡ�
@brief �ṩC��2Dͼ����Ч�任����, ����������Ч�����ţ�����ȣ��������Ч���ҳ���ȡ�

@version 0.1 2009/02/13 ��ʼ�汾
@{
*/
#ifndef _CS_EFFECT_H_
#define _CS_EFFECT_H_

#include "mwtypes.h"
#include "mwapi.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**********************************************************************************************
 *										��������
 **********************************************************************************************/

#define MODULE_EFFECT	"CSEffect"

/**********************************************************************************************
 *									ö��/�ṹ����
 **********************************************************************************************/
/**@brief effectģ��ķ������ͣ�OK��ʾ�ɹ���FAILED��ʾʧ��*/
typedef enum
{
	EM_EFFECT_SUCCUSS = 0,	/**< success */
	EM_EFFECT_FAILED = -1		/**< failed */
}CSEFTRet_E;


/**@brief ������Ч�ķ���*/
typedef enum _CSEFTDirectionType_E
{
	EM_EFFECT_DIRECTION_TYPE_LEFT,            /**< ����*/
	EM_EFFECT_DIRECTION_TYPE_TOP,              /**< ����*/
	EM_EFFECT_DIRECTION_TYPE_RIGHT,         /**< ����*/
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,    /**< ����*/
	EM_EFFECT_DIRECTION_TYPE_CENTER,      /**< �м�*/

	EM_EFFECT_DIRECTION_TYPE_LT,                 /**< ����*/
	EM_EFFECT_DIRECTION_TYPE_LB,                 /**< ����*/
	EM_EFFECT_DIRECTION_TYPE_RT,                 /**< ����*/
	EM_EFFECT_DIRECTION_TYPE_RB,                 /**< ����*/

	EM_EFFECT_DIRECTION_TYPE_IN,                  /**< ����*/
	EM_EFFECT_DIRECTION_TYPE_OUT,              /**< ����*/

	EM_EFFECT_DIRECTION_TYPE_CLOCKWISE,        			  /**< ˳ʱ�뷽��*/
	EM_EFFECT_DIRECTION_TYPE_COUNTER_CLOCKWISE,      /**< ��ʱ�뷽��*/

	EM_EFFECT_DIRECTION_TYPE_HORIZON,   /**< ˮƽ*/
	EM_EFFECT_DIRECTION_TYPE_VERTICAL,  /**< ��ֱ*/

	EM_EFFECT_DIRECTION_TYPE_CENTER_BOTH,  /**< ���м�������*/
	EM_EFFECT_DIRECTION_TYPE_BOTH_CENTER,  /**< ���������м�*/

	EM_EFFECT_DIRECTION_TYPE_FAR_NEAR,  /**< ��Զ����*/
	EM_EFFECT_DIRECTION_TYPE_NEAR_FAR,   /**< �ɽ���Զ*/
	EM_EFFECT_DIRECTION_TYPE_MAXNUMBER   /**< �����Ŀ*/	
}CSEFTDirectionType_E;

/**@brief ������ЧԴ������

Ŀǰ֧��pixmap��image��ʽ
pixmap�����ڴ��ڵ�һ���ͼ����image��Ӧһ��ͼƬ����MicroWin������֮��Ӧ�����ݽṹ.
*/
typedef enum _CSEFTSourceType_E
{
	EM_EFFECT_SOURCE_TYPE_IMAGE,            /**< Image��ʽ*/
	EM_EFFECT_SOURCE_TYPE_PIXMAP            /**< Pixmap��ʽ*/
}CSEFTSourceType_E;

/**@brief ��Ч�任ʱ���������Դ��ʽ

��������Դ���ͣ�ͼƬ��ʽ��Pixmap��ʽ
��ͼƬ/Pixmap��ID��
*/
typedef struct _CSEFTSource_S
{
	CSEFTSourceType_E	m_eSourceType;		/**< ����Դ��ʽ*/
	int					m_nSourceID;		/**< ����Դ���*/
}CSEFTSource_S;

/**@brief ͨ����Ч�任ʱ��ͼƬ��Ԫ������任��Ϣ

ͨ��ָ����ͬ����ʼ����ֹ�����꣬��ʵ���ƶ���Ч
ͨ��ָ����ͬ�������ȼ��߶ȣ���ʵ��������Ч
*/
typedef struct _CSEFTFrame_S
{
    GR_RECT     m_sClip;        /**< ԭʼͼƬ�Ĳü�����*/
	GR_RECT		m_sRect;		/**< ��ʾ��Ŀ������*/
	BYTE		m_ucAlpha;		/**< ͸���ȱ仯�����Ϣ*/
	BYTE		m_ucZOrder;		/**< ������Ч�仯��ͼƬ���ڲ��*/
	WORD		m_wInterval;		/**< ���е���һ�켣��ʱ��*/
}CSEFTFrame_S;

/**@brief ͨ����Ч�任ʱ��ͼƬ��Ԫ������Ϣ

ͨ��ָ����ͬ��Z��ֵ����ʵ����Ч�ķֲ���ʾЧ��
Z��ֵԽ��ͼƬ��ʾʱԽλ���ϲ㡣
*/
typedef struct _CSEFTUnit_S
{
	CSEFTSource_S	m_sDataSource;		/**< ����Դ*/
	CSEFTFrame_S*	m_psFrames;		/**< ��Ч���˶��켣*/
	int				m_nFrameCount;		/**< ��Ч���˶��켣Ԫ�ظ���*/
}CSEFTUnit_S;

typedef struct
{
	int nCol;								/**<����*/
	int nRow;								/**<����*/
	BOOL bHasReflect;						/**<�Ƿ��е�Ӱ*/
	int nVisibleCol;						/**<�ɼ���ͼƬ����*/
}CSFlatwallLayoutParam_S;
/**********************************************************************************************
 *									�������ܽӿ�
 **********************************************************************************************/

/**
@brief �ƶ�����������ʾͼƬ

��ָ��ͼƬ��ָ��ʱ���ڣ���̬ƽ���ش�Դ�����ƶ���Ŀ������֧���Զ�������ά������
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] dst  Ŀ�������
@param[in] src  ��ʼ������
@param[in] nTime �任����ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ���ɡ�
@param[in] psDataSource Ŀ��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTMove (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_POINT * dst, GR_POINT * src, 
	int nTime, int nCount, CSEFTSource_S *psDataSource);


/**
@brief �ƶ�����������ʾͼƬ

��ָ��ͼƬ��ָ��ʱ���ڣ���̬�����ٶȵش�Դ�����ƶ���Ŀ������֧���Զ�������ά������
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] dst  Ŀ�������
@param[in] src  ��ʼ������
@param[in] nTime �任����ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ���ɡ�
@param[in] psDataSource Ŀ��ͼƬ��Դ
@param[in] eStyle ����˶��ķ�ʽ������١����ٵȵ�
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTMoveEase (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_POINT * dst, GR_POINT * src, 
			   int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);




/**
@brief ���Ž�����ʾͼƬ

��ָ��ͼƬ��ָ��ʱ���ڣ���̬ƽ���شӿ�ʼָ����С�仯ΪĿ��ָ����С��
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] psDst  ͼƬĿ��任����
@param[in] psSrc  ͼƬ��ʼ�任����
@param[in] nTime �任����ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ���ɡ�
@param[in] psDataSource  Ŀ��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTZoom (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_RECT *psDst, GR_RECT *psSrc,
	int nTime, int nCount, CSEFTSource_S *psDataSource);
	
	
	
	
/**
@brief ���Ž�����ʾͼƬ

��ָ��ͼƬ��ָ��ʱ���ڣ���̬���شӿ�ʼָ����С�仯ΪĿ��ָ����С��
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] psDst  ͼƬĿ��任����
@param[in] psSrc  ͼƬ��ʼ�任����
@param[in] nTime �任����ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ���ɡ�
@param[in] psDataSource  Ŀ��ͼƬ��Դ
@param[in] eStyle  ����˶��ķ�ʽ������١����ٵȵ�
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTZoomEase (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_RECT *psDst, GR_RECT *psSrc,
			   int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);
			   
			   
/**
@brief ͸��������ʾͼƬ

��ָ��ͼƬ��ָ��ʱ���ڣ���̬ƽ���ش���ʼ͸���ȱ仯��Ŀ��͸���ȣ��뱳����ϣ���
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] unFrom  ��ʼAlphaֵ��0-255��
@param[in] unTo  Ŀ��Alphaֵ��0-255��
@param[in] nTime �任����ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ���ɡ�
@param[in] psDataSource  Ŀ��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTTransparent (int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	UCHAR unFrom, UCHAR unTo, int nTime, int nCount,  CSEFTSource_S *psDataSource);

/**
@brief Alpha�����ʾͼƬ

��ָ���Ķ���ͼƬ����Alpha��ϣ���ָ��ÿ��ͼƬ������ʱ��ռ�Ļ����������
���ʱ����һ��ͼƬ������ײ㣬����ͼƬ�������ϻ�ϸ��ǡ�ͼƬ���մ�С�Ե�һ��ͼƬ��СΪ׼��
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] pnImageIDArray  ��ִ��Alpha��ϵ�ͼƬ��ID����
@param[in] pucAlphaArray  ����Alpha��ϵ�ͼƬ�Ļ����������
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTBlend(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, GR_IMAGE_ID *pnImageIDArray, UCHAR *pucAlphaArray);

/**
@brief ��������ͼƬ

����ָ��ͼƬ�ľ���任�����ͼƬ��
���������������ͼƬ�����ı�ԭʼͼƬ���ݡ�
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x   ͼƬ��ʾ��x����
@param[in] y   ͼƬ��ʾ��y����
@param[in] psDataSource  Ŀ��ͼƬ��Դ
@param[out] ��
@return �ɹ������µľ��񲿷��γɵ�ͼƬID(��ֵ),ʧ�ܷ��ظ�ֵ
@note ��
*/
int CSEFTMirror(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, CSEFTSource_S *psDataSource);


/**
@brief ��ȡ��ӰͼƬ

��ȡָ��ͼƬ�ĵ�Ӱ�任�����ͼƬ��
�������ص�Ӱ����ͼƬ��ID�����ı�ԭʼͼƬ���ݡ�
��ָ���µ�ͼƬ���ԭʼͼƬ�ĸ߶ȱȣ�����ֱ�����ѹ����������ӰʱAlphaֵ�ı任��Χ��
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] nHeightPercent  ��ӰͼƬ��ԭʼͼƬ�İٷֱȣ�ȡֵ0-100��
@param[in] ucFromAlpha  ��ʼAlphaֵ��0-255��
@param[in] ucToAlpha  Ŀ��Alphaֵ��0-255��
@param[in] psDataSource  Ŀ��ͼƬ��Դ
@param[out] ��
@return �ɹ������µĵ�Ӱ�����γɵ�ͼƬID(��ֵ),ʧ�ܷ��ظ�ֵ
@note ��
*/
GR_IMAGE_ID CSEFTReflect (int client_id, GR_DRAW_ID id, GR_GC_ID gc, 
	int nHeightPercent, UCHAR ucFromAlpha, UCHAR ucToAlpha, CSEFTSource_S *psDataSource);

/**
@brief ��ӰЧ����ʾͼƬ

��ָ����ͼƬ����ӰЧ����ʾ������
������ʾͼƬ��ͬʱ�������ҷ����·���ʾ�����ɫ����
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] nShadowWidth  ��Ӱ����Ŀ��
@param[in] psDataSource  Ŀ��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTShadow (int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, int nShadowWidth, CSEFTSource_S *psDataSource);


/**
@brief Բ����ʾͼƬ

��ָ����ͼƬ���ĸ��ǣ���ָ���İ뾶Բ�ǻ�������ʾ��ָ���豸��ָ�������ϡ�
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] nRoundRadius  Բ�ǰ뾶��С
@param[in] psDataSource  Ŀ��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTRound (int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, int nRoundRadius, CSEFTSource_S *psDataSource);


/**********************************************************************************************
 *								����Ч�ӿ�
 **********************************************************************************************/

/**
@brief ������Ч

��ָ����ͼƬ�Զ���Ч����ʾ������
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  �����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_HORIZON
	EM_EFFECT_DIRECTION_TYPE_VERTICAL
@param[in] nNum �����Ĵ���
@param[in] nRange �����ķ���,������ƫ������λ�õľ���
@param[in] nTime ��������ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ���ɡ�
@param[in] psDataSource �趶��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTShake(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDirection, int nNum, int nRange, int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief ������Ч

��ָ����ͼƬ�Թ���Ч����ʾ������
��������Ч��ͼ����Դ��ĸ����������һ��������
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  �����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM
@param[in] nTime ����ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTShrink��Ӧ��
*/
int CSEFTRoll(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, CSEFTDirectionType_E eEftDir, 
	int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief ������Ч

��ָ����ͼƬ������Ч�����ؽ�ȥ��
��������Ч��ͼ����Դ��ĸ����������һ��������ȥ��
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  �����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM
@param[in] nTime ����ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ������ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTRoll��Ӧ��
*/
int CSEFTShrink(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, CSEFTDirectionType_E eEftDir, 
	int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief ��ʧ��Ч

����͸���ȴӴ�Сֱ����ʧ
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] nTime ����ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ģ��ͼƬ���Ե����Ĺ��̣�����ָ��͸���ȱ仯���䡣��CSEFTAppear��Ӧ��
*/
int CSEFTDisappear(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief ������Ч

����͸���ȴ�С����ֱ����ȫ��ʾ
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] nTime ����ʱ��
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ģ��ͼƬ�������ԵĹ��̣�����ָ��͸���ȱ仯���䡣��CSEFTDisAppear��Ӧ��
*/
int CSEFTAppear(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief ������Ч

�����𽥱��ͬʱ�䵭��ֱ����ʧ
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ģ�����̻����ź���ʧ�Ĺ��̡�
*/
int CSEFTPuff(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief ������Ч

��ָ�������ָ��������뵽��Ļ��ָ��λ��
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  �������ʼ����ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
	EM_EFFECT_DIRECTION_TYPE_LT,
	EM_EFFECT_DIRECTION_TYPE_LB,
	EM_EFFECT_DIRECTION_TYPE_RT,
	EM_EFFECT_DIRECTION_TYPE_RB  
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTFlyOut��Ӧ,ͼƬ����ʾ�ᳬ��ͼƬԭ����ռ�����С��
*/
int CSEFTFlyIn(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir, int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief �ɳ���Ч

�����ԭ��λ�÷ɳ��ɼ���Χ
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  �ɳ��ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
	EM_EFFECT_DIRECTION_TYPE_LT,
	EM_EFFECT_DIRECTION_TYPE_LB,
	EM_EFFECT_DIRECTION_TYPE_RT,
	EM_EFFECT_DIRECTION_TYPE_RB  
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTFlyIn��Ӧ, ͼƬ����ʾ�ᳬ��ͼƬԭ����ռ�����С��
*/
int CSEFTFlyOut(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);

/**
@brief ������Ч

������ָ�������ƶ�����
ͼƬ����ʾ���ᳬ��ͼƬԭ����ռ�����С
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  ����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTCutOut��Ӧ.
*/
int CSEFTCutIn(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);




/**
@brief ������Ч

������ָ�������ƶ�����
ͼƬ����ʾ���ᳬ��ͼƬԭ����ռ�����С
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  ����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[in] eStyle ����˶��ķ�ʽ������١����ٵȵ�
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTCutOut��Ӧ.
*/
int CSEFTCutInEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
			   CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);


/**
@brief �г���Ч

������ָ�������ƶ���ʧ
ͼƬ����ʾ���ᳬ��ͼƬԭ����ռ�����С
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  ����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTCutIn��Ӧ.
*/
int CSEFTCutOut(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);





/**
@brief �г���Ч

������ָ�������ƶ���ʧ
ͼƬ����ʾ���ᳬ��ͼƬԭ����ռ�����С
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  ����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[in] eStyle ����˶��ķ�ʽ������١����ٵȵ�
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTCutIn��Ӧ.
*/
int CSEFTCutOutEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
				CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);

/**
@brief ��Ҷ����Ч

�������������Ҷ��Ч��
ͼƬ����ʾ���ᳬ��ͼƬԭ����ռ�����С
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  ����ķ���ȡ����ֵ
EM_EFFECT_DIRECTION_TYPE_VERTICAL,
EM_EFFECT_DIRECTION_TYPE_HORIZON,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@@param[in] nPiece  ��Ҷ����ҳ��
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTCutOut��Ӧ.
*/
int CSEFTShutter(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
				 CSEFTDirectionType_E eEftDir,  int nTime, int nCount,int nPiece, CSEFTSource_S *psDataSource);


/**
@brief ��ͼƬ����ʾˮ����Ч��

��
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] xOff  2DԪ���������Ļ(0,0)��x��ƫ��
@param[in] yOff  2DԪ���������Ļ(0,0)��y��ƫ��
@param[in] gc  ��ͼ������ID
@param[in] x  �Ŷ����ĵ�x����
@param[in] y  �Ŷ����ĵ�y����
@param[in] nRadius  �Ŷ��İ뾶������1~30��
@param[in] nStrength �Ŷ���ǿ�ȣ�����1~200��
@param[in] psDataSource  Ŀ��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTWaterWave(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int xOff, int yOff, int x, int y, int nRadius, int nStrength,
			   CSEFTSource_S *psDataSource);


/**
@brief ������ͼƬ�Է�ҳЧ��չʾ��ע����ҳͼƬ�Ĵ�С����һ��

��
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾ��ƫ�ƺ�����
@param[in] y  ͼƬ��ʾ��ƫ��������
@param[in] psDataSource1  ��ǰҳ��ͼƬ
@param[in] psDataSource2 ��һҳ��ͼƬ

@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTFlipPage(int client_id, GR_DRAW_ID id, GR_GC_ID gc,int x, int y,
				  CSEFTSource_S *psDataSource1, CSEFTSource_S *psDataSource2);





/**********************************************************************************************
 *								�����Ч�ӿ�
 **********************************************************************************************/
 
/**
@brief ����������Ч

�����д�С��͸���ȵı任
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] psDst  ͼƬĿ��任����
@param[in] psSrc  ͼƬ��ʼ�任����
@param[in] nStartOpacity ͼƬ��ʼ͸���ȣ�ȡֵ(0-255)
@param[in] nEndOpacity ͼƬ����͸���ȣ�ȡֵ(0-255)
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTBlendZoom (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_RECT *psDst, GR_RECT *psSrc,
	   UCHAR nStartOpacity, UCHAR nEndOpacity,
	   int nTime, int nCount, CSEFTSource_S *psDataSource);



/**
@brief ����������Ч

�����д�С��͸���ȵı任
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] psDst  ͼƬĿ��任����
@param[in] psSrc  ͼƬ��ʼ�任����
@param[in] nStartOpacity ͼƬ��ʼ͸���ȣ�ȡֵ(0-255)
@param[in] nEndOpacity ͼƬ����͸���ȣ�ȡֵ(0-255)
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[in] eStyle ����˶��ķ�ʽ������١����ٵȵ�
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSEFTBlendZoomEase (int client_id, GR_DRAW_ID id, GR_GC_ID gc, GR_RECT *psDst, GR_RECT *psSrc,
					UCHAR nStartOpacity, UCHAR nEndOpacity,
					int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);


/**
@brief ��ҳ��Ч

һ��ͼƬ�ֳ����룬����ͼƬ������ָ�������ƶ�
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  ����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[in] nStartOpacity  ������һҳ����ʼ͸����
@param[in] nEndOpacity ������һҳ����ֹ͸����
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note 
*/

int CSEFTFlip(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
			   CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource,int nStartOpacity,int nEndOpacity);


/**
@brief ��ҳ��Ч

һ��ͼƬ�ֳ����룬����ͼƬ������ָ�������ƶ�
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  ����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[in] nStartOpacity  ������һҳ����ʼ͸����
@param[in] nEndOpacity ������һҳ����ֹ͸����
@param[in] eStyle ����˶��ķ�ʽ������١����ٵȵ�
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note 
*/

int CSEFTFlipEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
			   CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource,int nStartOpacity,int nEndOpacity,CSANIMATIONSTYLE_E eStyle);

/**
@brief ͼƬ��ʾ������Ч

һ��ͼƬ��ָ������������ʾ���������(�߶�)Խ��Խ���ʼ��ȫ

@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  ����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTShowOut��Ӧ.
*/
int CSEFTShowIn(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);




/**
@brief ͼƬ��ʾ������Ч

һ��ͼƬ��ָ������������ʾ���������(�߶�)Խ��Խ���ʼ��ȫ

@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir  ����ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[in] eStyle ����˶��ķ�ʽ������١����ٵȵ�
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTShowOut��Ӧ.
*/
int CSEFTShowInEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
			   CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);

/**
@brief ͼƬ��ʾ�Ƴ���Ч

	һ��ͼƬ��ָ������������ʧ���������(�߶�)Խ��ԽС
	
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir	�Ƴ��ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTShowIn��Ӧ.
*/
int CSEFTShowOut(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
	CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource);





/**
@brief ͼƬ��ʾ�Ƴ���Ч

	һ��ͼƬ��ָ������������ʧ���������(�߶�)Խ��ԽС
	
@param[in] client_id Client IDֵ
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] x  ͼƬ��ʾx����
@param[in] y  ͼƬ��ʾy����
@param[in] eEftDir	�Ƴ��ķ���ȡ����ֵ
	EM_EFFECT_DIRECTION_TYPE_LEFT,
	EM_EFFECT_DIRECTION_TYPE_TOP,
	EM_EFFECT_DIRECTION_TYPE_RIGHT,
	EM_EFFECT_DIRECTION_TYPE_BOTTOM,
@param[in] nTime �任���ʱ�䣬��ÿ�α任������ʱ�䡣
@param[in] nCount �任��������һ�α任��Ϊ���ٴ����
@param[in] psDataSource ��任��ͼƬ��Դ
@param[in] eStyle ����˶��ķ�ʽ������١����ٵȵ�
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��CSEFTShowIn��Ӧ.
*/
int CSEFTShowOutEase(int client_id, GR_DRAW_ID id, GR_GC_ID gc, int x, int y, 
				CSEFTDirectionType_E eEftDir,  int nTime, int nCount, CSEFTSource_S *psDataSource, CSANIMATIONSTYLE_E eStyle);



/**
@brief ͨ����Ч�ӿ�

ͨ����Ч��� ����λ�ã������С��͸���ȱ任������ε�ͬʱ�任��
����Ե��ǵ���ͼƬ��Դ�ı任��֧�ֱ����Զ�ά����
@param[in] nClientID Client IDֵ
@param[in] nDrawID  ����ID
@param[in] nGCID  ��ͼ������ID
@param[in] psUnit �任�켣��֡���ݣ�
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note fixme:��ʱ������Z��
*/
CSEFTRet_E CSEFTGeneral (int nClientID, GR_DRAW_ID nDrawID, GR_GC_ID nGCID,
	CSEFTUnit_S *psUnit);

/**
@brief ��ʼ������Ч�ӿ�

ִ�в�����Чǰ������ô˽ӿ������б�ʶ
@param[in] ��
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note:��
*/
CSEFTRet_E CSEFTBeginParallel();

/**
@brief ֹͣ������Ч�ӿ�

�粻��Ҫ���в�����Ч�ˣ����øýӿ�ֹͣ������Ч����ʱ������Ч����ִ��
@param[in] pvReserve Ԥ����չ������ΪNULL
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note:��
*/
CSEFTRet_E CSEFTStopParallel(void* pvReserve);

/**
@brief����������Ч�ӿ�

����������Ч�ı�־
@param[in] nClientID Client IDֵ
@param[in] nDrawID  ����ID
@param[in] nGCID  ��ͼ������ID
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note:��
*/
CSEFTRet_E CSEFTEndParallel(int nClientID, GR_DRAW_ID nDrawID, GR_GC_ID nGCID);

/**
@brief������Чִ�еĲü�����

������Чִ�еĲü����������������������Ч��ִ�лᱻ�����ڸ������ڣ�
��������Ĳ��ֻᱻ�ü�����û������������Ч��λ�ú������򲻻ᱻ���ơ�
@param[in] psClip ��Ч���õĲü�����
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note:��
*/
CSEFTRet_E CSEFTSetClip(GR_RECT *psClip);

/**
@brief CSCreateImageFlatWall

�û�����ָ����ImageFlatWallģ�Ͳ������С��С�ÿ��ͼƬ�Ŀ��ߣ��Ƿ��е�Ӱ����Ļ�Ŀ��ߡ�
@param[in] client_id
@param[in] id  ����ID
@param[in] gc  ��ͼ������ID
@param[in] psLayoutParam  ��Ч���Ű����
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSFlatWallInit(int client_id,  GR_DRAW_ID id,GR_GC_ID gc,CSFlatwallLayoutParam_S *psLayoutParam);

/**
@brief CSFlatWallAddImage

����ͼƬ��
@param[in] file  �ļ�·��
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSFlatWallAddImage(char *file);

/**
@brief CSFlatWallShow

��ʾƽ����Ч��
@param[in]��
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSFlatWallShow();

/**
@brief CSFlatWallOnKeyProcess

ƽ����Ч�İ�����Ӧ
@param[in] type
@param[in] keyCode
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/

int CSFlatWallOnKeyProcess(GR_EVENT_TYPE type,int keyCode);


/**
@brief CSFlatWallUnInit

ƽ����Ч����Դ�ͷ�
@param[in] ��
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note ��
*/
int CSFlatWallUnInit();


typedef enum {
	EM_GLOBALEFFECT_NONE = 0,     ///<û����Ч
	EM_GLOBALEFFECT_MOVE ,     ///<�ƶ���Ч
	EM_GLOBALEFFECT_ZOOM ,         ///<������Ч
	EM_GLOBALEFFECT_TRANSPARENT,         ///<͸����Ч
	EM_GLOBALEFFECT_CUTIN,         ///<�����г���Ч
	EM_GLOBALEFFECT_SHUTTER,   ///<��Ҷ����Ч
	EM_GLOBALEFFECT_WATERWAVE,   ///<��Ҷ����Ч
	EM_GLOBALEFFECT_PAGEFLIP,   ///<��Ҷ����Ч
	EM_GLOBALEFFECT_COUNT   ///< �������ã�����¼��Ч����
} CSGLOABLEFFECTSTYLE_E;

/**
@brief����ȫ����Ч�ķ��

������Чִ�еĲü����������������������Ч��ִ�лᱻ�����ڸ������ڣ�
��������Ĳ��ֻᱻ�ü�����û������������Ч��λ�ú������򲻻ᱻ���ơ�
@param[in] emEFTType ��Ч������
@param[out] ��
@return �ɹ�����0,ʧ�ܷ��ط�0ֵ
@note:��
*/
CSEFTRet_E CSEFTSetGlobalStyle(CSGLOABLEFFECTSTYLE_E emEFTType);


/**
@brief������Ч�ĳ�ʼ��

@param[in] ��
@param[out] ��
@return ��
@note:��
*/
void CSEFTInit();

#ifdef  __cplusplus
}
#endif

/** @} */

#endif


