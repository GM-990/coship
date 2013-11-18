#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "global.h"
#include "rsaref.h"
#include "rsa.h"

#define L_Printf(x)			//printf x

static lua_Integer csluaL_checkinteger (lua_State *L, int narg,const char* des) 
{
  lua_Integer d = lua_tointeger(L, narg);
  if (d == 0 && !lua_isnumber(L, narg))  /* avoid extra test when d is not 0 */
    luaL_error(L, "bad argument #%d to 'pairs' (integer expected, got %s) <%s>",narg,luaL_typename(L, narg),des);
  return d;
}

static void dumpString(const char* p,int len,const char* pDes)
{
	if(p)
	{
		int i;
		L_Printf(("dumpString [%s](len=%d)",pDes,len));
		for(i=0;i<len;i++)
		{
			if(i % 16 ==0) L_Printf(("\n"));
			L_Printf(("0x%02x ",p[i]&0xff));
		}
		L_Printf(("\n"));
	}
}

#define getstr_fromtable(L,tableoffset,key,strout,len,maxlen)	\
	{\
		lua_pushstring(L,key);\
		lua_gettable(L,tableoffset);\
		strout = lua_tolstring(L,-1,&len);\
		assert(strout && "failed to get "key" from talbe");\
		assert(len <= maxlen);\
		if(len > maxlen || !strout) return -1;\
	}

#define DUMP_KEY(st,enm)	dumpString(st->enm,sizeof(st->enm),#st"->"#enm)

static int getRsaPublicKey(R_RSA_PUBLIC_KEY* pKey,lua_State *L,int keyoffset)
{
	assert(pKey != 0);
	assert(L != 0);
	assert(keyoffset > 0);
	if(pKey == 0 || L == 0 || keyoffset <= 0) return -1;
		
	luaL_checktype(L,keyoffset,LUA_TTABLE);
	
	//get num of bits
	lua_pushstring(L,"bits");
	lua_gettable(L,keyoffset);
	int numbits = csluaL_checkinteger(L,-1,"get bits from public key failed");
	assert(numbits >= MIN_RSA_MODULUS_BITS);
	assert(numbits <= MAX_RSA_MODULUS_BITS);		
	assert(numbits % 64 == 0);
	L_Printf(("numbits = %d\n",numbits));
	
	int numbytes = ((numbits + 7) / 8);
	assert(numbytes <= MAX_RSA_MODULUS_LEN);
	if(numbytes > MAX_RSA_MODULUS_LEN) return -1;
	L_Printf(("numbytes = %d\n",numbytes));
	
	//get module
	int modulelen = 0;
	const unsigned char * module;
	getstr_fromtable(L,keyoffset,"modulus",module,modulelen,numbytes);

	//get publicExponent
	int publen = 0;
	const unsigned char * pub;		
	getstr_fromtable(L,keyoffset,"publicExponent",pub,publen,numbytes);
	
	memset(pKey,0,sizeof(R_RSA_PUBLIC_KEY));
	pKey->bits = numbits;
	memcpy(pKey->modulus+MAX_RSA_MODULUS_LEN-modulelen,module,modulelen);
	memcpy(pKey->exponent+MAX_RSA_MODULUS_LEN-publen,pub,publen);
	
	L_Printf(("pKey->bits=%d\n",pKey->bits));
	DUMP_KEY(pKey,modulus);
	DUMP_KEY(pKey,exponent);

	return 0;	
}

static int getRsaPrivateKey(R_RSA_PRIVATE_KEY* pKey,lua_State *L,int keyoffset)
{
	assert(pKey != 0);
	assert(L != 0);
	assert(keyoffset > 0);
	if(pKey == 0 || L == 0 || keyoffset <= 0) return -1;
	
	//get num of bits
	lua_pushstring(L,"bits");
	lua_gettable(L,keyoffset);
	int numbits = csluaL_checkinteger(L,-1,"get bits from private key failed");
	assert(numbits >= MIN_RSA_MODULUS_BITS);
	assert(numbits <= MAX_RSA_MODULUS_BITS);		
	assert(numbits % 64 == 0);
	
	int numbytes = ((numbits + 7) / 8);
	assert(numbytes <= MAX_RSA_MODULUS_LEN);
	if(numbytes > MAX_RSA_MODULUS_LEN) return -1;
	L_Printf(("numbytes = %d\n",numbytes));

	int primelen = ((((numbits + 1) / 2) + 7) / 8);
	assert(primelen <= MAX_RSA_PRIME_LEN);
	if(primelen > MAX_RSA_PRIME_LEN) return -1;
	L_Printf(("primelen = %d\n",primelen));
	
	//get module
	int modulelen = 0;
	const unsigned char * module;
	getstr_fromtable(L,keyoffset,"modulus",module,modulelen,numbytes);

	//get publicExponent
	int publen = 0;
	const unsigned char * pub;		
	getstr_fromtable(L,keyoffset,"publicExponent",pub,publen,numbytes);
	
	//get private exponent
	int privatelen = 0;
	const unsigned char * private;
	getstr_fromtable(L,keyoffset,"privateExponent",private,privatelen,numbytes);

	//get prime1
	int prime1len = 0;
	const unsigned char * prime1;
	getstr_fromtable(L,keyoffset,"prime1",prime1,prime1len,primelen);

	//get prime2
	int prime2len = 0;
	const unsigned char * prime2;
	getstr_fromtable(L,keyoffset,"prime2",prime2,prime2len,primelen);

	//get exponent1
	int exponent1len = 0;
	const unsigned char * exponent1;
	getstr_fromtable(L,keyoffset,"exponent1",exponent1,exponent1len,primelen);

	//get exponent2
	int exponent2len = 0;
	const unsigned char * exponent2;
	getstr_fromtable(L,keyoffset,"exponent2",exponent2,exponent2len,primelen);

	//get coefficient
	int coefficientlen = 0;
	const unsigned char * coefficient;
	getstr_fromtable(L,keyoffset,"coefficient",coefficient,coefficientlen,primelen);

	memset(pKey,0,sizeof(R_RSA_PRIVATE_KEY));
	pKey->bits = numbits;
	memcpy(pKey->modulus+MAX_RSA_MODULUS_LEN-modulelen,module,modulelen);
	memcpy(pKey->publicExponent+MAX_RSA_MODULUS_LEN-publen,pub,publen);
	memcpy(pKey->exponent+MAX_RSA_MODULUS_LEN-privatelen,private,privatelen);
	memcpy(pKey->prime[0]+MAX_RSA_PRIME_LEN-prime1len,prime1,prime1len);
	memcpy(pKey->prime[1]+MAX_RSA_PRIME_LEN-prime2len,prime2,prime2len);
	memcpy(pKey->primeExponent[0]+MAX_RSA_PRIME_LEN-exponent1len,exponent1,exponent1len);
	memcpy(pKey->primeExponent[1]+MAX_RSA_PRIME_LEN-exponent2len,exponent2,exponent2len);
	memcpy(pKey->coefficient+MAX_RSA_PRIME_LEN-coefficientlen,coefficient,coefficientlen);

	L_Printf(("pKey->bits=%d\n",pKey->bits));
	DUMP_KEY(pKey,modulus);
	DUMP_KEY(pKey,publicExponent);
	DUMP_KEY(pKey,exponent);
	DUMP_KEY(pKey,prime[0]);
	DUMP_KEY(pKey,prime[1]);
	DUMP_KEY(pKey,primeExponent[0]);
	DUMP_KEY(pKey,primeExponent[1]);
	DUMP_KEY(pKey,coefficient);

	return 0;
}


static unsigned char s_dst[MAX_RSA_MODULUS_LEN];

static int rsa_privateEncrypt(lua_State *L)
{
	int len = 0;
	const unsigned char * input = luaL_checklstring(L,1,&len);

	R_RSA_PRIVATE_KEY prvKey;
	if(getRsaPrivateKey(&prvKey,L,2) != 0) return 0;

	memset(s_dst,0,sizeof(s_dst));
	int dstlen = 0;
	if(0 != RSAPrivateEncrypt(s_dst,&dstlen,(unsigned char*)input,len,&prvKey)) return 0;
	assert(dstlen <= sizeof(s_dst));

	lua_pushlstring(L,s_dst,dstlen);
	return 1;	
}

static int rsa_privateDecrypt(lua_State *L)
{
	int len = 0;
	const unsigned char * input = luaL_checklstring(L,1,&len);

	R_RSA_PRIVATE_KEY prvKey;
	if(getRsaPrivateKey(&prvKey,L,2) != 0) return 0;

	memset(s_dst,0,sizeof(s_dst));
	int dstlen = 0;
	if(0 != RSAPrivateDecrypt(s_dst,&dstlen,(unsigned char*)input,len,&prvKey)) return 0;
	assert(dstlen <= sizeof(s_dst));

	lua_pushlstring(L,s_dst,dstlen);
	return 1;	
}

static int rsa_publicEncrypt(lua_State *L)
{
	int len = 0;
	const unsigned char * input = luaL_checklstring(L,1,&len);

	R_RANDOM_STRUCT rnd;
	#if defined(RSAEuro)
	R_RandomCreate(&rnd);
	#endif

	R_RSA_PUBLIC_KEY pubKey;
	if(getRsaPublicKey(&pubKey,L,2) != 0) return 0;
	
	memset(s_dst,0,sizeof(s_dst));
	int dstlen = 0;
	if(0 != RSAPublicEncrypt(s_dst,&dstlen,(unsigned char*)input,len,&pubKey,&rnd)) return 0;
	assert(dstlen <= sizeof(s_dst));

	lua_pushlstring(L,s_dst,dstlen);
	return 1;
}

static int rsa_publicDecrypt(lua_State *L)
{
	int len = 0;
	const unsigned char * input = luaL_checklstring(L,1,&len);

	R_RSA_PUBLIC_KEY pubKey;
	if(getRsaPublicKey(&pubKey,L,2) != 0) return 0;
	
	memset(s_dst,0,sizeof(s_dst));
	int dstlen = 0;
	if(0 != RSAPublicDecrypt(s_dst,&dstlen,(unsigned char*)input,len,&pubKey)) return 0;
	assert(dstlen <= sizeof(s_dst));

	lua_pushlstring(L,s_dst,dstlen);
	return 1;
}

static int rsa_test(lua_State *L)
{
	printf("MAX_RSA_MODULUS_LEN = %d\n",MAX_RSA_MODULUS_LEN);
}

static const luaL_Reg rsalib[] = {
  {"privateEncrypt", rsa_privateEncrypt},
  {"privateDecrypt", rsa_privateDecrypt},
  {"publicEncrypt", rsa_publicEncrypt},
  {"publicDecrypt", rsa_publicDecrypt},
  {"test", rsa_test},
  {NULL, NULL}
};


LUALIB_API int luaopen_csrsa (lua_State *L) //这个函数也是lua扩展函数类型的，也会在lua中调用它
{
  luaL_register(L, "csrsa", rsalib);
  return 1;
}

/*
reuqire("xxx") --这句会触发寻找xxx.so的动态库，并自动调用luaopen_xxx，而luaL_register时传入的模块名是没有关系的。

或者
local f = package.loadlib ("./csrsa.so", "luaopen_csrsa") --注意这里就带.so的完整路径以及注册函数的入口名称了，灵活，使用也麻烦些。
f()
*/


