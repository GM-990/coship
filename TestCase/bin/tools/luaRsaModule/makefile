# This is a makefile for UNIX

# extension for object files
O = o

# commands
CC = gcc
LIB = ar

# Set to rsaref20 or RSAEuro to choice rsalib
RSAMODULE = rsaref20

EXPORT = csrsa.so

CFLAG = -shared -fPIC

# The places to look for include files (in order).
INCL =  -I. -I$(RSAINC) -I$(LUAINC) -D$(RSAMODULE)

# The places to look for lua module
LUAINC = lua-5.1.4/src
LUABUILD = lua-5.1.4/src
LUAEXPORT = lua-5.1.4/src/lua

# The places to look for rsaref or RSAEuro module
RSAINC = $(RSAMODULE)/source/
RSABUILD = $(RSAMODULE)/install/
RSALIB = $(RSAMODULE)/install/rsaref.a

$(EXPORT) : luaRsaModule.o $(RSALIB) $(LUAEXPORT)
	$(CC) -o $@ luaRsaModule.o $(RSALIB) $(CFLAG) $(INCL)

$(RSALIB):
	make -C $(RSABUILD)
	
$(LUAEXPORT):
	make -C $(LUABUILD) linux
	
release:$(EXPORT) base64.lua MmcpKeyGenerator.lua openssl-1.0.0/apps/openssl
	cp base64.lua release
	cp MmcpKeyGenerator.lua release
	cp $(EXPORT) release
	cp $(LUAEXPORT) release
	cp openssl-1.0.0/apps/openssl release

%.o : %.c
	$(CC) -c $< $(INCL) $(CFLAG)

clean:
	-rm *.o
	-rm *.a
	-rm *.so
	#make -C $(LUABUILD) clean
	#make -C $(RSABUILD) clean
