OSG模块测试用例测试说明：

海斯平台：
一、请仔细阅读平台配置文件test_config_PLATFORM.cfg
    （1）海斯平台只支持CS_OSGUDI2_SURPORT_ARGB_8888颜色模式
    （2）CS_OSGUDI2_SURPORT_SURFACE_1280_720和CS_OSGUDI2_SURPORT_SURFACE_1920_1080主要用来测试CSUDIOSGCreateSurface接口，
    	 海斯平台为标清，所以这两个为0
    （3）海斯平台为标清，所以CS_OSGUDI2_SURPORT_HD_SD_DISPLAY为0
    （4）CS_OSGUDI2_SURFACE_MAX_WIDTH和CS_OSGUDI2_SURFACE_MAX_HEIGHT至少是平台支持大小以上，此处为720*576以上
    （5）海斯平台CS_UDI2OSG_SD_DISPLAY_WIDTH和CS_UDI2OSG_SD_DISPLAY_HEIGHT分别为720*576
    （6）海斯平台支持ARGB_8888颜色模式，因此CS_UDI2OSG_DISPLAY_PIXEL_FORMAT为6
    （7）图片硬解码接口尚未实现，因此CS_UDI2OSG_DECODE_IFRAME等选项都为0
    
二、本测试用例集只有CSTC_OSG_IT_Blit_Alpha_0001用例为半自动测试，需要与测试人员交互，其它用例均为自动测试！