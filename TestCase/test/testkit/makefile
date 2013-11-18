
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Makefile.defs
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Include.rules

MYFLAGS_LOCAL  :=
INCLUDES_LOCAL := -I$(MMCP_HOME)/test/testkit -I$(MMCP_HOME)/test/main
SUBDIRS := 
SOURCES := $(wildcard *.c *.cpp)

CFLAGS_LOCAL   := $(MYFLAGS_LOCAL)
CXXFLAGS_LOCAL := $(MYFLAGS_LOCAL)

all:default_target_kit

MMCP_COMPILE_RULES:=TESTKIT
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Compile.rules
