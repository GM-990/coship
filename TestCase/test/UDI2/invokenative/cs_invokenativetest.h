#ifndef _CS_INVOKENATIVE_TEST_H_
#define _CS_INVOKENATIVE_TEST_H_

#include "cs_testkit.h"
#include "udi2_invokeNative.h"
#include "udi2_typedef.h"
#include "udiplus_os.h"
#include "udiplus_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENVALIDRETVALUE (0xFFFFFFFF)

#define TEST_INTVALUE (0xf8a432eb)				//-123456789

#define TEST_LONGVALUE_LOW (0x82167eeb)
#define TEST_LONGVALUE_HIGH (0xeeddef0b)		//-1234567890123456789

#define TEST_FLOATVALUE (0xc2f63f36)			//-123.123456

#define TEST_DOUBLEVALUE_LOW (0x547e6b40)
#define TEST_DOUBLEVALUE_HIGH (0xc19d6f34)	//-1234546789.123456

int Invokenative_TestInt(int env, int classObj, int nInt);
CSUDI_INT64 Invokenative_TestLong(int env, int classObj, CSUDI_INT64 lLong);
float Invokenative_TestFloat(int env, int classObj, float fFloat);
double Invokenative_TestDouble(int env, int classObj, double dDouble);

int Invokenative_TestIFL
	(int env, int classObj, int nInt, float fFloat, CSUDI_INT64 lLong);
int Invokenative_TestIFD
	(int env, int classObj, int nInt, float fFloat, double dDouble);
int Invokenative_TestIDL
	(int env, int classObj, int nInt, double dDouble, CSUDI_INT64 lLong);
int Invokenative_TestIDF
	(int env, int classObj, int nInt, double dDouble, float fFloat);
int Invokenative_TestILF
	(int env, int classObj, int nInt, CSUDI_INT64 lLong, float fFloat);
int Invokenative_TestILD
	(int env, int classObj, int nInt, CSUDI_INT64 lLong, double dDouble);

CSUDI_INT64  Invokenative_TestLDF
	(int env, int classObj, CSUDI_INT64 lLong, double dDouble, float fFloat);
CSUDI_INT64  Invokenative_TestLDI
	(int env, int classObj, CSUDI_INT64 lLong, double dDouble, int nInt);
CSUDI_INT64  Invokenative_TestLFD
	(int env, int classObj, CSUDI_INT64 lLong, float fFloat, double dDouble);
CSUDI_INT64  Invokenative_TestLFI
	(int env, int classObj, CSUDI_INT64 lLong, float fFloat, int nInt);
CSUDI_INT64  Invokenative_TestLIF
	(int env, int classObj, CSUDI_INT64 lLong, int nInt, float fFloat);
CSUDI_INT64  Invokenative_TestLID
	(int env, int classObj, CSUDI_INT64 lLong, int nInt, double dDouble);

float Invokenative_TestFID
	(int env, int classObj, float fFloat, int nInt, double dDouble);
float Invokenative_TestFIL
	(int env, int classObj, float fFloat, int nInt, CSUDI_INT64 lLong);
float Invokenative_TestFDL
	(int env, int classObj, float fFloat, double dDouble, CSUDI_INT64 lLong);
float Invokenative_TestFDI
	(int env, int classObj, float fFloat, double dDouble, int nInt);
float Invokenative_TestFLI
	(int env, int classObj, float fFloat, CSUDI_INT64 lLong, int nInt);
float Invokenative_TestFLD
	(int env, int classObj, float fFloat, CSUDI_INT64 lLong, double dDouble);

double  Invokenative_TestDIF
	(int env, int classObj, double dDouble, int nInt, float fFloat);
double  Invokenative_TestDIL
	(int env, int classObj, double dDouble, int nInt, CSUDI_INT64 lLong);
double  Invokenative_TestDFL
	(int env, int classObj, double dDouble, float fFloat, CSUDI_INT64 lLong);
double  Invokenative_TestDFI
	(int env, int classObj, double dDouble, float fFloat, int nInt);
double  Invokenative_TestDLI
	(int env, int classObj, double dDouble, CSUDI_INT64 lLong, int nInt);
double  Invokenative_TestDLF
	(int env, int classObj, double dDouble, CSUDI_INT64 lLong, float fFloat);

int  Invokenative_TestIIIII
	(int env, int classObj, int nFirInt, int nSecInt, int nThrInt, int nForInt, int nFifInt);
int  Invokenative_TestLLLLL
	(int env, int classObj, CSUDI_INT64 lFirLong, CSUDI_INT64 lSecLong, CSUDI_INT64 lThrLong, CSUDI_INT64 lForLong, CSUDI_INT64 lFifLong);
int  Invokenative_TestFFFFF
	(int env, int classObj, float fFirFloat, float fSecFloat, float fThrFloat, float fForFloat, float fFifFloat);
int  Invokenative_TestDDDDD
	(int env, int classObj, double dFirDouble, double dSecDouble, double dThrDouble, double dForDouble, double dFifDouble);


int Invokenative_TestFFLLD
	(int env, int classObj, float fFirFloat, float fSecFloat, CSUDI_INT64 lFirLong, CSUDI_INT64 lSecLong, double dDouble);
int Invokenative_TestFIFLFD
	(int env, int classObj, float fFirFloat, int nInt, float fSecFloat, CSUDI_INT64 lLong, float fThrFloat, double dDouble);
int Invokenative_TestLFDFIFF
	(int env, int classObj, CSUDI_INT64 lLong, float fFirFloat, double dDouble,float fSecFloat, int nInt, float fThrFloat, float fForFloat);
int Invokenative_TestFFDDIILL
	(int env, int classObj, float fFirFloat, float fSecFloat, double dFirDouble, double dSecDouble, int nFirInt, int nSecInt, CSUDI_INT64 lFirLong, CSUDI_INT64 lSecLong);
int Invokenative_TestL16L
	(int env, int classObj, CSUDI_INT64 lLong1, CSUDI_INT64 lLong2, CSUDI_INT64 lLong3, CSUDI_INT64 lLong4, CSUDI_INT64 lLong5, CSUDI_INT64 lLong6, CSUDI_INT64 lLong7, CSUDI_INT64 lLong8, 
	CSUDI_INT64 lLong9, CSUDI_INT64 lLong10, CSUDI_INT64 lLong11, CSUDI_INT64 lLong12, CSUDI_INT64 lLong13, CSUDI_INT64 lLong14, CSUDI_INT64 lLong15, CSUDI_INT64 lLong16);
int Invokenative_TestF16F
	(int env, int classObj, float fFloat1, float fFloat2, float fFloat3, float fFloat4, float fFloat5, float fFloat6, float fFloat7, float fFloat8, 
	float fFloat9, float fFloat10, float fFloat11, float fFloat12, float fFloat13, float fFloat14, float fFloat15, float fFloat16);
int Invokenative_TestD16D
	(int env, int classObj, double dDouble1, double dDouble2, double dDouble3, double dDouble4, double dDouble5, double dDouble6, double dDouble7,double dDouble8, 
	double dDouble9, double dDouble10, double dDouble11, double dDouble12, double dDouble13, double dDouble14, double dDouble15, double dDouble16);

int Invokenative_TestD20D
	(int env, int classObj, double dDouble1, double dDouble2, double dDouble3, double dDouble4, double dDouble5, double dDouble6, double dDouble7,double dDouble8, 
	double dDouble9, double dDouble10, double dDouble11, double dDouble12, double dDouble13, double dDouble14, double dDouble15, double dDouble16,
	double dDouble17, double dDouble18, double dDouble19, double dDouble20);

int Invokenative_TestI30I
	(int env, int classObj, int nInt1, int nInt2, int nInt3, int nInt4, int nInt5, int nInt6, int nInt7,int nInt8, 
	int nInt9, int nInt10, int nInt11, int nInt12, int nInt13, int nInt14, int nInt15, int nInt16,
	int nInt17, int nInt18, int nInt19, int nInt20, int nInt21, int nInt22, int nInt23, int nInt24,
	int nInt25, int nInt26, int nInt27,int nInt28, int nInt29, int nInt30);


#ifdef __cplusplus
}
#endif

#endif/*_CS_OSG_TEST_H_*/ 

