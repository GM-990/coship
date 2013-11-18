OS模块测试用例测试说明：


1、请仔细阅读平台配置文件test_config_PLATFORM.cfg

2、配置CS_OSUDI2_SUPPORT_SUSPEND

 （1）、确定用于测试的平台是否已经实现了CSUDIOSThreadSuspend和CSUDIOSThreadResume接口

 （2）、如果实现了这两个接口则CS_OSUDI2_SUPPORT_SUSPEND的值为1，否则为0

3、配置CS_OSUDI2_CPU_COUNT：平台CPU个数

4、配置CS_OSUID2_RAM_SIZE：平台总的RAM空间大小
