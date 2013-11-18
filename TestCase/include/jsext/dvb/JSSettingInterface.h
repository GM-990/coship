
#ifndef _JS_SETTING_INTERFACE_H_
#define _JS_SETTING_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================
//函数原型:JSSetTimeZone()
//输入参数:fTimeZone  时区值
//输出参数:无
//返回值:  无
//功能 : 设置时区值
//注意事项:
=========================================================== */
void JSSetTimeZone( float fTimeZone);

/*==========================================================
//函数原型:JSSetSummerTime()
//输入参数:nSummerTime  夏令时
//输出参数:无
//返回值:  无
//功能 : 设置夏令时
//注意事项:
=========================================================== */
void JSSetSummerTime( int nSummerTime );

/*==========================================================
//函数原型:JSGetSummerTimeZone()
//输入参数:无
//输出参数:无
//返回值:  时区值  +夏令时
//功能 :  获取当前设定时区的时区值+夏令时
//注意事项:
=========================================================== */
float JSGetSummerTimeZone();

#ifdef __cplusplus
}
#endif

#endif
