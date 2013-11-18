/**@defgroup CSUART[CSUART]Í¬ÖŞÏîÄ¿×¨ÓÃUARTÇı¶¯½Ó¿Ú
@brief ±¾Ä£¿éÖ÷Òª¶¨ÒåÁËÍ¬ÖŞÏîÄ¿×¨ÓÃÆ½Ì¨ÎŞ¹ØUARTÇı¶¯½Ó¿Ú
@brief UART (Universal Asynchronous Receiver/Transmitter) Í¨ÓÃÒì²½ÊÕ·¢Æ÷¡£»ùÓÚudiµÄMMCPÖĞ¼ä²ã²»»áÓÃµ½Õâ¸öÄ£¿é£¬µ«ÊÇÔÚ¾ßÌåµÄ»ùÓÚMMCPµÄÏîÄ¿µÄÅäÖÃÎÄ¼şÖĞÓ¦¸Ã»áÓÃµ½¡£
 
@version 0.2 2008/10/17 ÆÀÉóºóµÄµÚÒ»¸ö°æ±¾
@version 0.1 2008/10/16 ÑİÊ¾°æ±¾
@{
*/

#ifndef _CS_UART_H_
#define _CS_UART_H_

#include "cs_typedef.h"
#include "CSHDI_typedef.h"
#include "cs_assert.h"
#include "stdarg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief ´®¿Ú×Ô¶¨Òå²ÎÊı*/
typedef struct  _T_ST_Custom_Param
{
	DWORD dwCommClock; /**´®¿ÚËùÓÃµÄÊ±ÖÓÆµÂÊ*/
}T_ST_Custom_Param;


/**@brief ´®¿Ú²ÎÊıÀàĞÍ*/
typedef enum
{
	UART_STOP_BIT_NUM, 	/**< Type:T_UartStopBitNum */
	UART_DATA_LENGTH, 	/**< Type:T_UartDataLengthı */
	UART_PARITY, 			/**<  Type:T_UartParityType£ */
	UART_BAUD_RATE 		/**<  Type:DWORD */
}T_UartParamType;


/**@brief ´®¿Ú²ÎÊıÖĞµÄÍ£Ö¹Î»¶¨Òå*/
typedef enum
{ 
	UART_ONE_STOP_BIT, 	/**<1Î»Í£Ö¹Î»*/
	UART_TWO_STOP_BIT	/**< 2Î»Í£Ö¹Î»*/
}T_UartStopBitNum;


/**@brief ´®¿Ú²ÎÊıÖĞµÄÊı¾İ³¤¶È¶¨Òå*/
typedef enum
{
	UART_7_BIT_DATA,	/**< Êı¾İÎ»Îª7bit*/
	UART_8_BIT_DATA	/**< Êı¾İÎ»Îª8bit*/
}T_UartDataLength;


/**@brief ´®¿Ú²ÎÊıÖĞµÄÆæÅ¼Ğ£Ñé¶¨Òå*/
typedef enum
{
	UART_NO_PARITY,		/**< ÎŞĞ£Ñé*/
	UART_ODD_PARITY,		/**< ÆæĞ£Ñé*/
	UART_EVEN_PARITY		/**< Å¼Ğ£Ñé*/
}T_UartParityType;


/**@brief ´®¿Ú²ÎÊıÖĞµÄ¹¤×÷Ä£Ê½¶¨Òå,µ÷ÓÃCSSetUartModeÊ¹ÓÃ,CSSetUartModeÒÑ´ÓÍ·ÎÄ¼şÖĞÉ¾³ı*/
typedef enum
{
	UART_PRINT_MODE,		/**< ĞèÌí¼ÓËµÃ÷*/
	UART_DATA_MODE,		/**< ĞèÌí¼ÓËµÃ÷*/
	UART_OTHER_MODE		/**< ĞèÌí¼ÓËµÃ÷*/
}T_UartModeType;


/**
@brief ¶ÔµÚnUartIndexÂ·´®¿Ú½øĞĞ³õÊ¼»¯¡£ÔÚÊ¹ÓÃ´®¿ÚÇ°±ØĞë¶Ô¸ÃÂ·´®¿Ú½øĞĞ³õÊ¼»¯£¬Èç¹ûÓÃ»§ÒªÉèÖÃcommon clockµÄÖµÔò±ØĞëÔÚ³õÊ¼»¯´®¿ÚÇ°µ÷ÓÃº¯ÊıCSHDIUartSetCustomParam¡£

@param[in] nUartIndex  Òª³õÊ¼»¯´®¿ÚµÄË÷Òı¡£¶ÔÓÚ²»Í¬µÄÆ½Ì¨£¬´®¿ÚµÄ×ÜÊı²»Ò»Ñù£¬ÓÃ×÷´®¿Ú¹¦ÄÜµÄASCÒ²²»Í¬£¬ĞèÊÓ¾ßÌåÓ²¼şÉè¼Æ¶ø¶¨£¬ÔÚĞÂµÄÓ²¼şÆ½Ì¨ÉÏ½øĞĞ¿ª·¢Ê±£¬ÇëÏòÓ²¼ş¹¤³ÌÊ±Ê¦»òÕßÏòÇı¶¯¹¤³ÌÊ¦Ñ¯ÎÊ¾ßÌåµÄÈ¡Öµ¡£
@param[in] dwBaud ´®¿ÚµÄ´«ÊäËÙÂÊ¡£Ò»°ãÎª115200bps
@return
- CSHDI_ERROR_BAD_PARAMETER£ºÊäÈë²ÎÊı²»ºÏ·¨¡£
- CSHDI_ERROR_ALREADY_INITIALIZED£º¸ÃÂ·´®¿ÚÔÚÖ®Ç°ÒÑ¾­±»³õÊ¼»¯¹ıÁË¡£
- CSHDI_FAILURE£º´®¿Ú³õÊ¼»¯Ê§°Ü¡£
- CSHDI_SUCCESS£º´®¿Ú³õÊ¼»¯³É¹¦¡£
@note ´®¿Ú³õÊ¼»¯ĞèÔÚgpio³õÊ¼»¯Ö®ºó¡£
*/
CSHDI_Error_t CSHDIUartInitial  ( int  nUartIndex, UINT dwBaud);


/**
@brief ÉèÖÃÄ¬ÈÏµÄ´®¿Ú¡£

@param[in] nUartIndex ÒªÉèÖÃ³ÉÄ¬ÈÏ´®¿ÚµÄ´®¿ÚË÷Òı¡£
@return Èç¹û³É¹¦Ôò·µ»ØÓÃ»§Éè¶¨µÄ´®¿ÚË÷Òı£¬Èç¹û²»³É¹¦£¬Ôò·µ»ØÓÃ»§ÏÈÇ°Éè¶¨µÄÄ¬ÈÏ´®¿ÚµÄË÷Òı£¬Èç¹ûÓÃ»§ÏÈÇ°Ã»ÓĞÉèÖÃ¹ıÄ¬ÈÏµÄ´®¿Ú£¬Ôò·µ»ØÇı¶¯³õÊ¼»¯Ê±Ä¬ÈÏµÄ´®¿ÚË÷Òı(2)¡£
@note ÉèÖÃÄ¬ÈÏ´®¿ÚĞèÔÚ¶ÁĞ´²Ù×÷Ç°¡£
*/
int CSHDIUartSetDefault ( int  nUartIndex);


/**
@brief ´Ó´®¿ÚnUartIndex¶ÁÈ¡u32Length¸öÊı¾İ£¬¶ÁÈ¡µÄÊı¾İ´æ·Åµ½ pucBuffer£¬³¬Ê±Ê±¼äÎªu32TimeOut¡£

@param[in] nUartIndex Òª²Ù×÷µÄ´®¿ÚË÷Òı¡£
@param[in] pucBuffer ÓÃÓÚ´æ·Å¶ÁÈ¡µ½Êı¾İµÄ»º´æ¡£
@param[in] u32Length Òª¶ÁÈ¡Êı¾İµÄ³¤¶È¡£
@param[in] u32TimeOut ³¬Ê±Ê±¼ä£¬Á½¸ö×Ö·ûÖ®¼äµÄ³¬Ê±Ê±¼ä£¬µ¥Î»Îªms¡£
@return ·µ»ØÊµ¼Ê¶Áµ½µÄÊı¾İµÄ³¤¶È¡£
*/
ULONG  CSHDIUartReadFromWithTimeout ( int  nUartIndex ,PBYTE pucBuffer, ULONG u32Length, ULONG u32TimeOut);

/**
@brief Ïò´®¿ÚnUartIndex·¢ËÍu32Length¸öÊı¾İ£¬Òª·¢ËÍµÄÊı¾İ´æ·ÅÔÚ pucBufferÖĞ£¬³¬Ê±Ê±¼äÎªu32TimeOut¡£

@param[in] nUartIndex Òª²Ù×÷µÄ´®¿ÚË÷Òı¡£
@param[in] pucBuffer ´æ·ÅÒª·¢ËÍÊı¾İµÄ»º´æ¡£
@param[in] u32Length Òª·¢ËÍÊı¾İµÄ³¤¶È¡£
@param[in] u32TimeOut ³¬Ê±µÄÊ±¼ä£¬Á½¸ö×Ö·ûÖ®¼äµÄ³¬Ê±Ê±¼ä£¬µ¥Î»Îªms¡£
@return ·µ»ØÊµ¼Ê·¢ËÍµÄÊı¾İµÄ³¤¶È¡£
*/
ULONG  CSHDIUartWriteToWithTimeout( int  nUartIndex ,PBYTE pucBuffer, ULONG u32Length, ULONG u32TimeOut);

/**
@brief ´ÓÄ¬ÈÏ´®¿Ú¶ÁÈ¡u32Length¸öÊı¾İ£¬¶ÁÈ¡µÄÊı¾İ´æ·ÅÔÚ pucBufferÖĞ£¬³¬Ê±Ê±¼äÎªu32TimeOut¡£

@param[in] pucBuffer ÓÃÀ´´æ·Å¶Áµ½Êı¾İµÄ»º´æ¡£
@param[in] u32Length Òª¶ÁÈ¡Êı¾İµÄ³¤¶È¡£
@param[in] u32TimeOut ³¬Ê±µÄÊ±¼ä£¬Á½¸ö×Ö·ûÖ®¼äµÄ³¬Ê±Ê±¼ä£¬µ¥Î»Îªms¡£
@return ·µ»ØÊµ¼Ê¶Áµ½µÄÊı¾İµÄ³¤¶È¡£
*/
ULONG  CSHDIUartReadWithTimeout ( PBYTE pucBuffer, ULONG u32Length, ULONG u32TimeOut);

/**
@brief ÏòÄ¬ÈÏ´®¿Ú·¢ËÍu32Length¸öÊı¾İ£¬Òª·¢ËÍµÄÊı¾İ´æ·ÅÔÚ pucBufferÖĞ£¬³¬Ê±Ê±¼äÎªu32TimeOut¡£

@param[in] pucBuffer ´æ·ÅÒª·¢ËÍÊı¾İµÄ»º´æ¡£
@param[in] u32Length Òª·¢ËÍÊı¾İµÄ³¤¶È¡£
@param[in] u32TimeOut ³¬Ê±µÄÊ±¼ä£¬Á½¸ö×Ö·ûÖ®¼äµÄ³¬Ê±Ê±¼ä£¬µ¥Î»Îªms¡£
@return ·µ»ØÊµ¼Ê·¢ËÍµÄÊı¾İµÄ³¤¶È¡£
*/
ULONG  CSHDIUartWriteWithTimeout( PBYTE pucBuffer, ULONG u32Length, ULONG u32TimeOut);

/**
@brief Í¨¹ı´®¿ÚnUartIndex´òÓ¡Ò»´®×Ö·û´®¡£

@param[in] nUartIndex Òª²Ù×÷µÄ´®¿ÚË÷Òı¡£
@param[in] pucInFormat Òª´òÓ¡µÄ×Ö·û´®¡£
@return ÎŞ
*/
void  CSHDIUartPrintTo( int  nUartIndex, PCHAR pucInFormat,...);

/**
@brief Í¨¹ıÄ¬ÈÏ´®¿Ú´òÓ¡Ò»´®×Ö·û´®¡£

@param[in] pucInFormat Òª´òÓ¡µÄ×Ö·û´®¡£
@return ÎŞ
*/
void  CSHDIUartPrint(PCHAR pucInFormat,...);

/**
@briefÍ¨¹ıÄ¬ÈÏ´®¿Ú´òÓ¡Ò»´®×Ö·û´®¡£

@param[in] pucInFormat Òª´òÓ¡µÄ×Ö·û´®¡£
@param[in] ap ¿É±ä²ÎÊıÁĞ±í¡£
@return ÎŞ
@note ²»½¨ÒéÊ¹ÓÃ¸Ã½Ó¿Ú
*/
void  CSHDIUartVSPrint(PCHAR pucInFormat,va_list *ap);

/**
@brief »ñÈ¡UARTÄ£¿é°æ±¾ĞÅÏ¢£¬ÏÔÊ¾¸ñÊ½Îª:
  [Ä£¿éÃû]-[Ä£¿éÀàĞÍ]-[°æ±¾ºÅ],ÈçUART_OS_T_01.00.00, UART±íÊ¾Ä£¿éÃû,OS±íÊ¾+osÇı¶¯£¬NOOS±íÊ¾-osÇı¶¯£¬T±íÊ¾test°æ±¾,01.00.00Îª°æ±¾ºÅ

@param[out] pchVer ´æ·Å°æ±¾ĞÅÏ¢µÄ»º³åÇøÊ×µØÖ·
@param[in] nSize  »º³åÇø³¤¶È
@return 
   1> Èç¹ûµÚÒ»¸ö²ÎÊıpchVerÎª¿Õ£¬·µ»Ø°æ±¾ĞÅÏ¢µÄÊµ¼Ê³¤¶È
   2> Èç¹ûµÚÒ»¸ö²ÎÊı²»Îª¿Õ£¬ÇÒº¯Êıµ÷ÓÃ³É¹¦£¬Ôò·µ»ØÊµ
        ¼Ê»ñÈ¡µÄ°æ±¾ĞÅÏ¢³¤¶È
   3> Èç¹ûÊ§°Ü£¬·µ»Ø0
*/

int CSUARTGetVersion( PCHAR pucVer, int nSize );

/**
@brief ´Ó´®¿ÚnUartIndex¶ÁÈ¡u32Length¸öÊı¾İ£¬¶ÁÈ¡µÄÊı¾İ´æ·ÅÔÚ pucBufferÖĞ£¬³¬Ê±Ê±¼äÎªÄ¬ÈÏ³¬Ê±Ê±¼ä£¬
       Ä¬ÈÏ³¬Ê±Ê±¼ä¿ÉÒÔÍ¨¹ıSetDefaultTimeOutÉèÖÃ, Èç²»ÉèÖÃ£¬Ä¬ÈÏ³¬Ê±Ê±¼äÎª1000ms¡£

@param[in] nUartIndex Òª²Ù×÷µÄ´®¿ÚË÷Òı¡£
@param[in] pucBuffer ´æ·Å¶ÁÈ¡Êı¾İµÄ»º´æ¡£
@param[in] u32Length Òª¶ÁÈ¡Êı¾İµÄ³¤¶È¡£
@return ·µ»ØÊµ¼Ê¶ÁÈ¡µÄÊı¾İµÄ³¤¶È¡£
*/
ULONG  CSHDIUartReadFrom ( int  nUartIndex ,PBYTE pucBuffer, ULONG u32Length);

/**
@brief Ïò´®¿ÚnUartIndex·¢ËÍu32Length¸öÊı¾İ£¬Òª·¢ËÍµÄÊı¾İ´æ·ÅÔÚ pucBufferÖĞ£¬³¬Ê±Ê±¼äÎªÄ¬ÈÏ³¬Ê±Ê±¼ä£¬
	Ä¬ÈÏ³¬Ê±Ê±¼ä¿ÉÒÔÍ¨¹ıSetDefaultTimeOutÉèÖÃ, Èç²»ÉèÖÃ£¬Ä¬ÈÏ³¬Ê±Ê±¼äÎª1000ms¡£

@param[in] nUartIndex Òª²Ù×÷µÄ´®¿ÚË÷Òı¡£
@param[in] pucBuffer ´æ·ÅÒª·¢ËÍÊı¾İµÄ»º´æ¡£
@param[in] u32Length Òª·¢ËÍÊı¾İµÄ³¤¶È
@return·µ»ØÊµ¼Ê·¢ËÍµÄÊı¾İµÄ³¤¶È¡£
*/
ULONG  CSHDIUartWriteTo( int  nUartIndex ,PBYTE pucBuffer, ULONG u32Length);

/**
@brief ´ÓÄ¬ÈÏ´®¿Ú¶ÁÈ¡u32Length¸öÊı¾İ£¬¶ÁÈ¡µÄÊı¾İ´æ·ÅÔÚ pucBufferÖĞ£¬³¬Ê±Ê±¼äÎªÄ¬ÈÏ³¬Ê±Ê±¼ä£¬
	Ä¬ÈÏ´®¿ÚÍ¨¹ıCSHDIUartSetDefaultÉèÖÃ£¬Èç²»ÉèÖÃ£¬Ä¬ÈÏ´®¿ÚÎª2,
	Ä¬ÈÏ³¬Ê±Ê±¼ä¿ÉÒÔÍ¨¹ıSetDefaultTimeOutÉèÖÃ, Èç²»ÉèÖÃ£¬Ä¬ÈÏ³¬Ê±Ê±¼äÎª1000ms¡£

@param[in] pucBuffer ´æ·Å¶ÁÈ¡Êı¾İµÄ»º´æ¡£
@param[in] u32Length Òª¶ÁÈ¡Êı¾İµÄ³¤¶È¡£
@return ·µ»ØÊµ¼Ê¶ÁÈ¡µÄÊı¾İµÄ³¤¶È¡£
*/
ULONG  CSHDIUartRead ( PBYTE pucBuffer, ULONG u32Length);

/**
@brief ÏòÄ¬ÈÏ´®¿ÚĞ´Ö¸¶¨³¤¶ÈµÄÊı¾İ,³¬Ê±Ê±¼äÎªÄ¬ÈÏ³¬Ê±Ê±¼ä£¬
	Ä¬ÈÏ´®¿ÚÍ¨¹ıCSHDIUartSetDefaultÉèÖÃ£¬Èç²»ÉèÖÃ£¬Ä¬ÈÏ´®¿ÚÎª2,
	Ä¬ÈÏ³¬Ê±Ê±¼ä¿ÉÒÔÍ¨¹ıSetDefaultTimeOutÉèÖÃ, Èç²»ÉèÖÃ£¬Ä¬ÈÏ³¬Ê±Ê±¼äÎª1000ms¡£

@param[in] pucBuffer ´æ·ÅÒªĞ´Êı¾İµÄ»º´æ¡£
@param[in] u32Length ÒªĞ´Êı¾İµÄ³¤¶È¡£
@return ·µ»ØÊµ¼ÊĞ´ÈëµÄÊı¾İµÄ³¤¶È¡£
*/
ULONG  CSHDIUartWrite( PBYTE pucBuffer, ULONG u32Length);

/**
@brief ÉèÖÃ×Ô¶¨Òå²ÎÊı£¬¶ÔÓÚSTÆ½Ì¨ÎªÉèÖÃ´®¿ÚËùÓÃµÄÊ±ÖÓÆµÂÊ

@param[in] pParam ×Ô¶¨Òå²ÎÊı£¬STÆ½Ì¨ÎªÉèÖÃ´®¿ÚËùÓÃµÄÊ±ÖÓÆµÂÊ£¬Ê¹ÓÃT_ST_Custom_Param
@return none
*/
void CSHDIUartSetCustomParam( PVOID pParam);


/**
@brief SetDefaultTimeOutÎª²úÆ·Æ½Ì¨µÚ¶ş´ú½Ó¿ÚÖĞµÄº¯Êı£¬ÊµÏÖÖĞÖ±½Óµ÷ÓÃCSHDIUartSetDefaultTimeOut*

@param[in] dwValueTimeOut ³¬Ê±Ê±¼ä, unit(ms)
@return None
*/
void SetDefaultTimeOut( DWORD dwValueTimeOut);

/**
@brief ÉèÖÃÖ¸¶¨´®¿ÚµÄ²ÎÊı£¬¾ßÌå¼ûT_UartParamType¶¨Òå

@param[in] nUartIndex ´®¿ÚË÷Òı
@param[in] enmType ²ÎÊıÀàĞÍ
@param[in] pVaule ²ÎÊıÄÚÈİµÄÊ×µØÖ·
@return 
	CSHDI_SUCCESS:ÉèÖÃ³É¹¦
	CSHDI_FAILURE:ÉèÖÃÊ§°Ü
	CSHDI_ERROR_BAD_PARAMETER: ²ÎÊı´íÎó
*/
CSHDI_Error_t CSHDIUartSetParam(int  nUartIndex, T_UartParamType enmType, void *pVaule);

/**
@brief »ñÈ¡Ö¸¶¨´®¿ÚµÄ²ÎÊı

@param[in] nUartIndex ´®¿ÚË÷Òı
@param[in] enmType ²ÎÊıÀàĞÍ
@param[out] pVaule  ´æ·Å²ÎÊıÖµµÄÊ×µØÖ·
@return  
	CSHDI_SUCCESS:ÉèÖÃ³É¹¦
	CSHDI_FAILURE:ÉèÖÃÊ§°Ü
	CSHDI_ERROR_BAD_PARAMETER: ²ÎÊı´íÎó
*/
CSHDI_Error_t CSHDIUartGetParam(int  nUartIndex, T_UartParamType enmType, void *pVaule);


/**
@brief ¹Ø±ÕÖ¸¶¨´®¿Ú,¹Ø±Õºó±ØĞëÍ¨¹ıµ÷ÓÃ³õÊ¼»¯½Ó¿Ú·½¿É»Ö¸´´®¿Ú¹¤×÷¡£

@param[in] nUartIndex ´®¿ÚË÷Òı
@return
	CSHDI_SUCCESS: ¹Ø±Õ³É¹¦
	CSHDI_FAILURE:  ¹Ø±ÕÊ§°Ü
	CSHDI_ERROR_BAD_PARAMETER: ²ÎÊı´íÎó
*/
CSHDI_Error_t CSHDIUartTerm(int nUartIndex);


/**
@brief Çå³ıÒÑ¾­´ÓuartÊÕµ½£¬ÉĞ´æÔÚ»º³åÇøÖĞÃ»ÓĞ±»CSUDIUartRead¶Á×ßµÄÊı¾İ

@param[in] nUartIndex Òª²Ù×÷µÄ´®¿ÚË÷Òı¡£

@return
	CSHDI_SUCCESS:Çå³ı³É¹¦
	CSHDI_FAILURE: Çå³ıÊ§°Ü
	CSHDI_ERROR_BAD_PARAMETER: ²ÎÊı´íÎó
*/
CSHDI_Error_t CSHDIUartClear(int nUartIndex);

#ifdef __cplusplus
}
#endif

/** @} */

#endif



