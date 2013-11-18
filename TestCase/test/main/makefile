
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Makefile.defs
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Include.rules

INCLUDES_LOCAL += -I $(MMCP_HOME)/include/porting/platform \
				-I $(MMCP_HOME)/include/porting/udi \
				-I $(MMCP_HOME)/include/porting/hdi \
				-I $(MMCP_HOME)/include/porting/csplus
				
VPATH		+=  ./setup
VPATH		+=  ./prepare

all:
	@$(MMCP_HOME)/test/portingInit -d; \
	premake;make -f makefile_after_premake
	
MMCP_COMPILE_RULES:=TESTKIT
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Compile.rules

