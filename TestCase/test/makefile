include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Makefile.defs
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Include.rules

# You can modify MYFLAGS_LOCAL,VPATH,INCLUDES_LOCAL,SUBDIRS,SOURCES,LIBS_LOCAL, don't use +=
# 编译参数，如-D，但-I的参数写在INCLUDES_LOCAL中
MYFLAGS_LOCAL  :=

# 源码搜索路径，都在当前目录则留空
VPATH	       := $(MMCP_HOME)/test/main

# 头文件搜索路径
INCLUDES_LOCAL := 

# 其它要链接的库，空格分开，库扩展名".$(LIBSUFFIX)"，放在"trunk\lib\平台目录"中
LIBS_LOCAL :=

# 下级测试用例目录, 空格分开
SUBDIRS :=  testkit kernel  mediaplayer
#SUBDIRS :=  protocol guitool   testkit codec kernel  graph\
#		graphics dtv mediaplayer pvr  ca
##暂时为空或者无法自动编译的目录
##classes  integration jvm dtvm	midp
# 本级测试用到的源代码,$(wildcard *.c *.cpp)表示所有文件
SOURCES := cs_mmcptestmain.c cs_mmcptestentry.c

CFLAGS_LOCAL   := $(MYFLAGS_LOCAL)
CXXFLAGS_LOCAL := $(MYFLAGS_LOCAL)
MMCP_COMPILE_RULES:=TESTKIT
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Compile.rules
config:
	@$(MMCP_HOME)/test/portingInit

