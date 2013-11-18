include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Makefile.defs
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Include.rules

MODULE_NAME := libudi2service

EXPORTS := $(MODULE_NAME).$(LIBSUFFIX)

EXPORTSCOPYOUT := TRUE

CFLAGS_LOCAL   := 
CXXFLAGS_LOCAL := 

VPATH	       := 
INCLUDES_LOCAL := -I . \
	-I include \
	-I porting \
	-I $(MMCP_HOME)/include/porting/udi \
	-I $(MMCP_HOME)/include/porting/udi2 \
	-I $(MMCP_HOME)/include/porting/platform \
	-I $(MMCP_HOME)/include/porting/deprecated \
	-I $(MMCP_HOME)/include/porting/csplus \
	-I $(MMCP_HOME)/include/graph \
	-I $(MMCP_HOME)/include/kernel \
	-I $(MMCP_HOME)/include/dtv \
	-I $(MMCP_HOME)/include/dtv/MemoryPool \
	-I $(MMCP_HOME)/include/dtv/Section \
	-I $(MMCP_HOME)/include/guitool \
	-I $(MMCP_HOME)/include/kernel

SOURCES  := $(wildcard *.c *.cpp)


include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Compile.rules

