include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Makefile.defs
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Include.rules

# You can modify MYFLAGS_LOCAL,VPATH,INCLUDES_LOCAL,SUBDIRS,SOURCES,LIBS_LOCAL, don't use +=
# 编译参数，如-D，但-I的参数写在INCLUDES_LOCAL中
MYFLAGS_LOCAL  :=

# 源码搜索路径，都在当前目录则留空
VPATH	       := 

# 头文件搜索路径
INCLUDES_LOCAL := 

# 其它要链接的库，空格分开，库扩展名".$(LIBSUFFIX)"，放在"trunk\lib\平台目录"中
LIBS_LOCAL :=

# 下级测试用例目录, 空格分开
SUBDIRS := 

# 本级测试用到的源代码,$(wildcard *.c *.cpp)表示所有文件
SOURCES := cs_iframe_injectertestmain.c cs_iframe_injectertestentry.c cs_iframe_injectertestcase.c \
	iframe_480i.c iframe_576i.c iframe_720p.c  iframe_1080i.c

CFLAGS_LOCAL   := $(MYFLAGS_LOCAL)
CXXFLAGS_LOCAL := $(MYFLAGS_LOCAL)
MMCP_COMPILE_RULES:=TESTKIT
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Compile.rules

