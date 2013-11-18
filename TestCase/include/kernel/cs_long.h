
#ifndef _CS_LONG_H_
#define _CS_LONG_H_

#include	<limits.h>
#include	<cs_types.h>  
#ifdef __cplusplus
extern "C" {
#endif


#ifdef	MMCP_SUPPORT_LL

#define ll_setZero(a)		((a) = ll_zero_const)
#define ll_inc(a,b)          	((a) += (b))
#define ll_dec(a,b)          	((a) -= (b))

#define ull2ll(a)			((long long)(a))
#define ll2ull(a)			((unsigned long long)(a))

#define initint2ll(a)	    		((long long)(a))
#define initintint2ll(a, b) 	(((long long)(a)<<32) | (DWORD)(b))
	
#define ll_high(a)	    		((int)((a)>>32))
#define ll_low(a)	    		((int)(a))
#define int2ll(a)	    		((long long)(a))
#define int2ull(a)			((unsigned long long)(a))
#define ull2uint(a)			((unsigned int)(a))
#define uint2ll(a)	    		((long long)(unsigned int)(a))
#define uint2ull(a)	    		((unsigned long long)(unsigned int)(a))	
#define intint2ll(a,b)		(((long long)(a)<<32) | (unsigned int)(b))
#define ll2int(a)			((int)(a))
#define ll_add(a, b)		((a) + (b))
#define ll_and(a, b)		((a) & (b))
#define ll_div(a, b)			((a) / (b))
#define ll_mul(a, b)		((a) * (b))
#define ll_neg(a)			(-(a))
#define ll_not(a)			(~(a))
#define ll_or(a, b)			((a) | (b))
#define ll_shl(a, n)			((a) << (n))
#define ll_shr(a, n)			((a) >> (n))
#define ll_sub(a, b)		((a) - (b))
#define ll_ushr(a, n)		((uint64)(a) >> (n))
#define ll_xor(a, b)			((a) ^ (b))
#define ll_rem(a, b)		((a) % (b))
#define ll2float(a)			((float)(a))
#define ll2double(a)		((unsigned int)(a))
#define float2ll(f)			((long long) (f))
#define double2ll(f)		((long long)(f))
#define ll_ltz(ll)			((ll)<0)
#define ll_gtz(ll)			((ll)>0)
#define ll_lez(ll)			((ll)<=0)
#define ll_gez(ll)			((ll)>=0)
#define ll_eqz(a)			((a) == 0)
#define ll_ne(a, b)			((a) != (b))
#define ll_eq(a,b)			((a)==(b))
#define ll_ge(a, b)			((a) >= (b))
#define ll_le(a, b)			((a) <= (b))
#define ll_lt(a, b)			((a) < (b))
#define ll_gt(a, b)			((a) > (b))

#define ll_zero_const		initint2ll(0)
#define ll_one_const		initint2ll(1)

#define ll_min_const		initintint2ll(LONG_MIN, 0)
#define ll_max_const		initintint2ll(LONG_MAX, ULONG_MAX)

#define ll_uint2long(a,b) 	((a) = (unsigned long long)(unsigned long)b)
#define ll_int2long(a,b)  	((a) = (signed long)b)
#define ll_long2uint(a,b) 	(b = (unsigned int)(a))

#else	/* MMCP_SUPPORT_LL */
//===========================================================================//
//禁止使用该注释框之内的接口，应该使用宏定义中的接口						 //
//===========================================================================//
extern	int64	ll_zero_const;
extern	int64	ll_one_const;

extern	int64	ll_min_const;
extern	int64	ll_max_const;

extern 	int64	ull2ll_fct(uint64 a);
extern 	uint64	ll2ull_fct(int64 a);

extern  int64 int2ll_fct(int a);
extern  uint64 int2ull_fct(int a);
extern  int64 uint2ll_fcn(DWORD a);
extern  uint64 uint2ull_fcn(DWORD a);
extern  int64 intint2ll_fcn(int a, DWORD b);
extern  int64 ll_add_fcn(int64 a, int64 b);
extern  int64 ll_and_fcn(int64 a, int64 b);

extern  int64 ll_neg_fcn(int64 a);
extern  int64 ll_not_fcn(int64 a);
extern  int64 ll_or_fcn(int64 a, int64 b);
extern  int64 ll_shl_fcn(int64 a, DWORD n);
extern  int64 ll_shr_fcn(int64 a, DWORD n);
extern  int64 ll_sub_fcn(int64 a, int64 b);
extern  int64 ll_ushr_fcn(int64 a, DWORD n);
extern  int64 ll_xor_fcn(int64 a, int64 b);
extern  int64 double2ll_fcn(double f);

extern	int64	ll_mul_fct(int64 a, int64 b);
extern	int64	ll_div_fct(int64 a, int64 b);
extern	int64	ll_rem_fct(int64 a, int64 b);
//===================================================================================//
#define MAX_LENGTH_LONG 21

extern char  *ll2str(int64 aLong, char *resultString);
extern int64 str2ll(const char *str, char **endptr, int base);

#define ll_setZero(a)        	((a).high = (a).low = 0)
#define ll_inc(a,b)          	((a).low += (b).low, \
                             		(a).high += (b).high + ((a).low < (b).low));
#define ll_dec(a,b)          	((a).high -= ((b).high + ((a).low < (b).low)), \
                             		(a).low  -= (b).low)

#define ull2ll(a)			(ull2ll_fct((a)))
#define ll2ull(a)			(ll2ull_fct((a)))

#define initintint2ll(a, b)      {(DWORD)(b),(int)(a)}
#define initint2ll(a)			initintint2ll(0, a)
	
#define ll_high(a)			((int)((a).high))

#define ll_low(a)			((DWORD)((a).low))

#define int2ll(a)        		(int2ll_fct((a)))
#define int2ull(a)			(int2ull_fct((a)))
#define ull2uint(a)			((unsigned int)(a).low)
#define uint2ll(a)      		(uint2ll_fcn((a)))
#define uint2ull(a)			(uint2ull_fcn((a)))
#define intint2ll(a, b)   		(intint2ll_fcn((a),(b)))
#define ll2int(a)			((int)(a).low)

#define ll_add(a, b)     		(ll_add_fcn((a), (b)))
#define ll_and(a, b)     		(ll_and_fcn((a), (b)))

#define ll_div(a, b)			(ll_div_fct((a), (b)))
	 
#define ll_mul(a, b)		(ll_mul_fct((a), (b)))


#define ll_neg(a)        		(ll_neg_fcn((a)))
#define ll_not(a)        		(ll_not_fcn((a)))
#define ll_or(a, b)     		(ll_or_fcn((a), (b)))
#define ll_shl(a, n)     		(ll_shl_fcn((a), (n)))
#define ll_shr(a, n)     		(ll_shr_fcn((a), (n)))
#define ll_sub(a, b)     		(ll_sub_fcn((a), (b)))
#define ll_ushr(a, n)    		(ll_ushr_fcn((a), (n)))
#define ll_xor(a, b)     		(ll_xor_fcn((a), (b)))

#define ll_rem(a, b)		(ll_rem_fct((a), (b)))

#define ll2float(a)			((float)ll2double(a))
#define ll2double(a)		((DWORD)((a).high * ((DWORD)UINT_MAX + 1) +	\
								   (a).low))

#define float2ll(f)			(double2ll((DWORD)(f)))


#define double2ll(f)     		(double2ll_fcn((f)))


#define ll_ltz(a)			((a).high < 0)
#define ll_gtz(a)			(((a).high > 0)||(((a).high==0)&&(a).low>0))//modified by yinliming
#define ll_lez(a)			((a).high <= 0)
#define ll_gez(a)			((a).high >= 0)
#define ll_eqz(a)			(((a).high == 0) && ((a).low == 0))
									
#define ll_eq(a, b)			(((a).high == (b).high) &&	\
						 ((a).low == (b).low))
									
#define ll_ne(a,b)			(((a).high != (b).high) ||	\
						 ((a).low != (b).low))
									
#define ll_ge(a, b)			(((a).high > (b).high) || (((a).high == (b).high) && ((a).low >= (b).low)))
									
#define ll_le(a, b)			(((a).high < (b).high) ||		\
						(((a).high == (b).high) && ((a).low <= (b).low)))
									
#define ll_lt(a, b)			(((a).high < (b).high) ||		\
						(((a).high == (b).high) && ((a).low < (b).low)))
									
#define ll_gt(a, b)			(((a).high > (b).high) ||		\
						(((a).high == (b).high) && ((a).low > (b).low)))

#define ll_uint2long(a,b) 	((a).high = 0, (a).low = b)
#define ll_int2long(a,b) 	 {(a).low = b;\
						(a).high = ((signed long)b) >> 31;}
#define ll_long2uint(a,b) 	(b = (unsigned int)((a).low))
	 
#endif	/* MMCP_SUPPORT_LL */

#ifdef __cplusplus
}
#endif


#endif /*_CS_LONG_H_*/
