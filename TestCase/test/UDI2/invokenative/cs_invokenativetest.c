#include "cs_invokenativetest.h"

#define CHECK_FIR_SEC_VALUE(FIRVALUE, SECVALUE) \
	{	\
		if ((unsigned int)(*(unsigned int *)(&FIRVALUE)) != TEST_INTVALUE ||(unsigned int)(*(unsigned int *)(&SECVALUE)) != TEST_INTVALUE) \
		{	\
			CSTCPrint("检测前两个参数错误\n");	\
			return ENVALIDRETVALUE;	\
		}	\
		CSTCPrint("下面描述的错误信息里面参数的位置是指去掉前面两个参数以后的位置\n");	\
		CSTCPrint("例如:提示第1个参数错误，实际指第三个参数错误\n");	\
	}
int Invokenative_TestInt(int env, int classObj, int nInt)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return nInt;
}
CSUDI_INT64 Invokenative_TestLong(int env, int classObj, CSUDI_INT64 lLong)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return lLong;
}

float Invokenative_TestFloat(int env, int classObj, float fFloat)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}

	
	return fFloat;
}

double Invokenative_TestDouble(int env, int classObj, double dDouble)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return dDouble;
}

int Invokenative_TestIFL
	(int env, int classObj, int nInt, float fFloat, CSUDI_INT64 lLong)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return nInt;
}
int Invokenative_TestIFD
	(int env, int classObj, int nInt, float fFloat, double dDouble)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return nInt;
}
int Invokenative_TestIDL
	(int env, int classObj, int nInt, double dDouble, CSUDI_INT64 lLong)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return nInt;
}
int Invokenative_TestIDF
	(int env, int classObj, int nInt, double dDouble, float fFloat)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	return nInt;
}
int Invokenative_TestILF
	(int env, int classObj, int nInt, CSUDI_INT64 lLong, float fFloat)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return nInt;
}
int Invokenative_TestILD
	(int env, int classObj, int nInt, CSUDI_INT64 lLong, double dDouble)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return nInt;
}

/************************************7--12********************************************/
CSUDI_INT64  Invokenative_TestLDF
	(int env, int classObj, CSUDI_INT64 lLong, double dDouble, float fFloat)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return lLong;
}
CSUDI_INT64  Invokenative_TestLDI
	(int env, int classObj, CSUDI_INT64 lLong, double dDouble, int nInt)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return lLong;
}
CSUDI_INT64  Invokenative_TestLFD
	(int env, int classObj, CSUDI_INT64 lLong, float fFloat, double dDouble)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return lLong;
}
CSUDI_INT64  Invokenative_TestLFI
	(int env, int classObj, CSUDI_INT64 lLong, float fFloat, int nInt)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return lLong;
}
CSUDI_INT64  Invokenative_TestLIF
	(int env, int classObj, CSUDI_INT64 lLong, int nInt, float fFloat)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return lLong;
}
CSUDI_INT64  Invokenative_TestLID
	(int env, int classObj, CSUDI_INT64 lLong, int nInt, double dDouble)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return lLong;
}

/************************************13--18********************************************/
float Invokenative_TestFID
	(int env, int classObj, float fFloat, int nInt, double dDouble)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return fFloat;
}
float Invokenative_TestFIL
	(int env, int classObj, float fFloat, int nInt, CSUDI_INT64 lLong)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}	

	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return fFloat;
}
float Invokenative_TestFDL
	(int env, int classObj, float fFloat, double dDouble, CSUDI_INT64 lLong)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return fFloat;
}
float Invokenative_TestFDI
	(int env, int classObj, float fFloat, double dDouble, int nInt)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return fFloat;
}
float Invokenative_TestFLI
	(int env, int classObj, float fFloat, CSUDI_INT64 lLong, int nInt)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return fFloat;
}
float Invokenative_TestFLD
	(int env, int classObj, float fFloat, CSUDI_INT64 lLong, double dDouble)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return fFloat;
}

/************************************19--24********************************************/
double  Invokenative_TestDIF
	(int env, int classObj, double dDouble, int nInt, float fFloat)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return dDouble;
}
double  Invokenative_TestDIL
	(int env, int classObj, double dDouble, int nInt, CSUDI_INT64 lLong)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return dDouble;
}
double  Invokenative_TestDFL
	(int env, int classObj, double dDouble, float fFloat, CSUDI_INT64 lLong)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return dDouble;
}
double  Invokenative_TestDFI
	(int env, int classObj, double dDouble, float fFloat, int nInt)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return dDouble;
}
double  Invokenative_TestDLI
	(int env, int classObj, double dDouble, CSUDI_INT64 lLong, int nInt)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return dDouble;
}
double  Invokenative_TestDLF
	(int env, int classObj, double dDouble, CSUDI_INT64 lLong, float fFloat)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return dDouble;
}

/************************************25--28********************************************/
int  Invokenative_TestIIIII
	(int env, int classObj, int nFirInt, int nSecInt, int nThrInt, int nForInt, int nFifInt)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&nFirInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nSecInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nThrInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nForInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nFifInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return 0;
}
int  Invokenative_TestLLLLL
	(int env, int classObj, CSUDI_INT64 lFirLong, CSUDI_INT64 lSecLong, CSUDI_INT64 lThrLong, CSUDI_INT64 lForLong, CSUDI_INT64 lFifLong)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lFirLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lFirLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lSecLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lSecLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lThrLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lThrLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lForLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lForLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lFifLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lFifLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}
int  Invokenative_TestFFFFF
	(int env, int classObj, float fFirFloat, float fSecFloat, float fThrFloat, float fForFloat, float fFifFloat)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFirFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fSecFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fThrFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fForFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fFifFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}
int  Invokenative_TestDDDDD
	(int env, int classObj, double dFirDouble, double dSecDouble, double dThrDouble, double dForDouble, double dFifDouble)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dFirDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dFirDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dSecDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dSecDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dThrDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dThrDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dForDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dForDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dFifDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dFifDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}


/************************************29--32********************************************/
int Invokenative_TestFFLLD
	(int env, int classObj, float fFirFloat, float fSecFloat, CSUDI_INT64 lFirLong, CSUDI_INT64 lSecLong, double dDouble)
{

	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFirFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fSecFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lFirLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lFirLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lSecLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lSecLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}
int Invokenative_TestFIFLFD
	(int env, int classObj, float fFirFloat, int nInt, float fSecFloat, CSUDI_INT64 lLong, float fThrFloat, double dDouble)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFirFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fSecFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&fThrFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第6个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}
int Invokenative_TestLFDFIFF
	(int env, int classObj, CSUDI_INT64 lLong, float fFirFloat, double dDouble,float fSecFloat, int nInt, float fThrFloat, float fForFloat)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&fFirFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if((unsigned int)(*(unsigned int *)(&dDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&fSecFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}	
	
	if ((unsigned int)(*(unsigned int *)(&nInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&fThrFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第6个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&fForFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第7个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}
int Invokenative_TestFFDDIILL
	(int env, int classObj, float fFirFloat, float fSecFloat, double dFirDouble, double dSecDouble, int nFirInt, int nSecInt, CSUDI_INT64 lFirLong, CSUDI_INT64 lSecLong)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFirFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&fSecFloat)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if((unsigned int)(*(unsigned int *)(&dFirDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dFirDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&dSecDouble)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dSecDouble))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&nFirInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&nSecInt)) != TEST_INTVALUE)
	{
		CSTCPrint("第6个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&lFirLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lFirLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第7个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	if ((unsigned int)(*(unsigned int *)(&lSecLong)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lSecLong))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第8个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}

int Invokenative_TestL16L
	(int env, int classObj, CSUDI_INT64 lLong1, CSUDI_INT64 lLong2, CSUDI_INT64 lLong3, CSUDI_INT64 lLong4, CSUDI_INT64 lLong5, CSUDI_INT64 lLong6, CSUDI_INT64 lLong7, CSUDI_INT64 lLong8, 
	CSUDI_INT64 lLong9, CSUDI_INT64 lLong10, CSUDI_INT64 lLong11, CSUDI_INT64 lLong12, CSUDI_INT64 lLong13, CSUDI_INT64 lLong14, CSUDI_INT64 lLong15, CSUDI_INT64 lLong16)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&lLong1)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong1))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong2)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong2))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong3)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong3))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong4)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong4))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong5)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong5))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong6)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong6))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第6个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong7)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong7))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第7个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong8)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong8))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第8个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong9)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong9))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第9个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong10)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong10))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第10个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong11)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong11))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第11个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong12)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong12))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第12个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong13)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong13))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第13个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong14)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong14))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第14个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong15)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong15))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第15个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&lLong16)) != TEST_LONGVALUE_LOW && (unsigned int)(*(((unsigned int *)(&lLong16))+1)) != TEST_LONGVALUE_HIGH)
	{
		CSTCPrint("第16个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}


int Invokenative_TestF16F
	(int env, int classObj, float fFloat1, float fFloat2, float fFloat3, float fFloat4, float fFloat5, float fFloat6, float fFloat7, float fFloat8, 
	float fFloat9, float fFloat10, float fFloat11, float fFloat12, float fFloat13, float fFloat14, float fFloat15, float fFloat16)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&fFloat1)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat2)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat3)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat4)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat5)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat6)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第6个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat7)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第7个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat8)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第8个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat9)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第9个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat10)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第10个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat11)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第11个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat12)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第12个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat13)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第13个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat14)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第14个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat15)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第15个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&fFloat16)) != TEST_FLOATVALUE)
	{
		CSTCPrint("第16个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}

int Invokenative_TestD16D
	(int env, int classObj, double dDouble1, double dDouble2, double dDouble3, double dDouble4, double dDouble5, double dDouble6, double dDouble7,double dDouble8, 
	double dDouble9, double dDouble10, double dDouble11, double dDouble12, double dDouble13, double dDouble14, double dDouble15, double dDouble16)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dDouble1)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble1))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble2)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble2))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble3)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble3))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble4)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble4))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble5)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble5))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble6)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble6))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第6个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble7)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble7))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第7个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble8)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble8))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第8个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble9)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble9))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第9个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble10)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble10))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第10个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble11)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble11))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第11个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble12)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble12))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第12个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble13)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble13))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第13个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble14)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble14))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第14个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble15)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble15))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第15个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble16)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble16))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第16个参数错误\n");
		return ENVALIDRETVALUE;
	}
	return 0;
}


int Invokenative_TestD20D
	(int env, int classObj, double dDouble1, double dDouble2, double dDouble3, double dDouble4, double dDouble5, double dDouble6, double dDouble7,double dDouble8, 
	double dDouble9, double dDouble10, double dDouble11, double dDouble12, double dDouble13, double dDouble14, double dDouble15, double dDouble16,
	double dDouble17, double dDouble18, double dDouble19, double dDouble20)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&dDouble1)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble1))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble2)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble2))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble3)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble3))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble4)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble4))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble5)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble5))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble6)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble6))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第6个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble7)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble7))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第7个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble8)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble8))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第8个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble9)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble9))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第9个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble10)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble10))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第10个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble11)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble11))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第11个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble12)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble12))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第12个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble13)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble13))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第13个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble14)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble14))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第14个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble15)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble15))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第15个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble16)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble16))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第16个参数错误\n");
		return ENVALIDRETVALUE;
	}

	if ((unsigned int)(*(unsigned int *)(&dDouble17)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble17))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第7个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble18)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble18))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第8个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble19)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble19))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第9个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&dDouble20)) != TEST_DOUBLEVALUE_LOW && (unsigned int)(*(((unsigned int *)(&dDouble20))+1)) != TEST_DOUBLEVALUE_HIGH)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	
	return 0;
}

int Invokenative_TestI30I
	(int env, int classObj, int nInt1, int nInt2, int nInt3, int nInt4, int nInt5, int nInt6, int nInt7,int nInt8, 
	int nInt9, int nInt10, int nInt11, int nInt12, int nInt13, int nInt14, int nInt15, int nInt16,
	int nInt17, int nInt18, int nInt19, int nInt20, int nInt21, int nInt22, int nInt23, int nInt24,
	int nInt25, int nInt26, int nInt27,int nInt28, int nInt29, int nInt30)
{
	CHECK_FIR_SEC_VALUE(env, classObj);
	
	if ((unsigned int)(*(unsigned int *)(&nInt1)) != TEST_INTVALUE)
	{
		CSTCPrint("第1个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt2)) != TEST_INTVALUE)
	{
		CSTCPrint("第2个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt3)) != TEST_INTVALUE)
	{
		CSTCPrint("第3个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt4)) != TEST_INTVALUE)
	{
		CSTCPrint("第4个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt5)) != TEST_INTVALUE)
	{
		CSTCPrint("第5个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt6)) != TEST_INTVALUE)
	{
		CSTCPrint("第6个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt7)) != TEST_INTVALUE)
	{
		CSTCPrint("第7个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt8)) != TEST_INTVALUE)
	{
		CSTCPrint("第8个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt9)) != TEST_INTVALUE)
	{
		CSTCPrint("第9个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt10)) != TEST_INTVALUE)
	{
		CSTCPrint("第10个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt11)) != TEST_INTVALUE)
	{
		CSTCPrint("第11个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt12)) != TEST_INTVALUE)
	{
		CSTCPrint("第12个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt13)) != TEST_INTVALUE)
	{
		CSTCPrint("第13个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt14)) != TEST_INTVALUE)
	{
		CSTCPrint("第14个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt15)) != TEST_INTVALUE)
	{
		CSTCPrint("第15个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt16)) != TEST_INTVALUE)
	{
		CSTCPrint("第16个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt17)) != TEST_INTVALUE)
	{
		CSTCPrint("第17个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt18)) != TEST_INTVALUE)
	{
		CSTCPrint("第18个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt19)) != TEST_INTVALUE)
	{
		CSTCPrint("第19个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt20)) != TEST_INTVALUE)
	{
		CSTCPrint("第20个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt21)) != TEST_INTVALUE)
	{
		CSTCPrint("第21个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt22)) != TEST_INTVALUE)
	{
		CSTCPrint("第22个参数错误\n");
		return ENVALIDRETVALUE;
	}if ((unsigned int)(*(unsigned int *)(&nInt23)) != TEST_INTVALUE)
	{
		CSTCPrint("第23个参数错误\n");
		return ENVALIDRETVALUE;
	}if ((unsigned int)(*(unsigned int *)(&nInt24)) != TEST_INTVALUE)
	{
		CSTCPrint("第24个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt25)) != TEST_INTVALUE)
	{
		CSTCPrint("第25个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt26)) != TEST_INTVALUE)
	{
		CSTCPrint("第26个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt27)) != TEST_INTVALUE)
	{
		CSTCPrint("第27个参数错误\n");
		return ENVALIDRETVALUE;
	}if ((unsigned int)(*(unsigned int *)(&nInt28)) != TEST_INTVALUE)
	{
		CSTCPrint("第28个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt29)) != TEST_INTVALUE)
	{
		CSTCPrint("第29个参数错误\n");
		return ENVALIDRETVALUE;
	}
	if ((unsigned int)(*(unsigned int *)(&nInt30)) != TEST_INTVALUE)
	{
		CSTCPrint("第30个参数错误\n");
		return ENVALIDRETVALUE;
	}

	return 0;
}

