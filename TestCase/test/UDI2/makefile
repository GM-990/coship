include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Makefile.defs
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Include.rules

# You can modify MYFLAGS_LOCAL,VPATH,INCLUDES_LOCAL,SUBDIRS,SOURCES,LIBS_LOCAL, don't use +=
# 编译参数，如-D，但-I的参数写在INCLUDES_LOCAL中
MYFLAGS_LOCAL  :=

# 源码搜索路径，都在当前目录则留空
VPATH	       := 

CURRENT_PATH=`pwd`

# 头文件搜索路径
INCLUDES_LOCAL := 

# 其它要链接的库，空格分开，库扩展名".$(LIBSUFFIX)"，放在"trunk\lib\平台目录"中
LIBS_LOCAL := $(MMCP_HOME)/lib/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/libdtv.$(LIBSUFFIX) \
		$(MMCP_HOME)/lib/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/libkernel.$(LIBSUFFIX) 

# 下级测试用例目录, 空格分开
SUBDIRS := audio os osg input section player panel demux fs tuner debug toolset \
		flash injecter eeprom screen video descramble aout smartcard network ittest  performance usb bootshow ota hdmi record hid

# 本级测试用到的源代码,$(wildcard *.c *.cpp)表示所有文件
SOURCES := $(wildcard *.c *.cpp)

CFLAGS_LOCAL   := $(MYFLAGS_LOCAL)
CXXFLAGS_LOCAL := $(MYFLAGS_LOCAL)
MMCP_COMPILE_RULES:=TESTKIT
include $(MMCP_HOME)/build/$(MMCP_PLATFORM)/Compile.rules

EXPORT_PORT_TEST_LIB := $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/$(MMCP_PLATFORM)_$(TARGET)_test.$(LIBSUFFIX)

INPUT_PORT_TEST_LIB := $(MMCP_HOME)/test/testkit/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/libtestkittest.$(LIBSUFFIX) \
	      ./libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/lib$(TARGET)test.$(LIBSUFFIX)  \
	      $(MMCP_HOME)/lib/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/libUDIPlus.$(LIBSUFFIX) \
	      $(LIBS_LOCAL)
	   


$(EXPORT_PORT_TEST_LIB) : $(INPUT_PORT_TEST_LIB)
	@echo build $(EXPORT_PORT_TEST_LIB) $^...
	$(MMCP_BUILD_LIBRARY)
	@echo build $(EXPORT_PORT_TEST_LIB) over
	
doc:
	@mktestdb .
	@mktestdoc testdb.lua
	@cp $(MMCP_HOME)/test/udi2/cs_$(TARGET)testentry.h  $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/udi2/
	@rm $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/udi2/include/ -rf
	@mkdir -p $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/udi2/include/
	#@cp $(MMCP_HOME)/include/porting/udi2/UDI2*.doc  $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/udi2/include/
	@cp $(MMCP_HOME)/include/porting/udi2/*.h  $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/udi2/include/
	@rm $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/testlib/include/ -rf
	@mkdir -p $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/testlib/include/
	@cp $(MMCP_HOME)/include/porting/udi2/*.h  $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/testlib/include/
	#@cp $(MMCP_HOME)/include/porting/udi2/UDI2*.doc   $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/testlib/include/
	@cp $(MMCP_HOME)/test/udi2/document  $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/udi2/ -R
	@cp $(MMCP_HOME)/test/udi2/UDI2*.doc  $(MMCP_HOME)/test/udi2/libs/$(MMCP_PLATFORM)/$(MMCP_COMPILING_MODE)/udi2/


testlib: $(EXPORT_PORT_TEST_LIB)
