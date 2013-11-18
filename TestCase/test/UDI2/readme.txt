一、设置环境变量
. bin/setup --im  (选择平台，要求上传的用例至少在linux\st5105\bcm7309都编译通过）

二、利用cstfg命令生成测试用例框架
例如：
[903126@localhost udi2]$ cd panel
[903126@localhost panel]$ cstfg 
 -----------------------------------------------------------
|    Welcome to use Coship Test Framework Generator v1.2    |
 -----------------------------------------------------------
pwd: /home/903126/hdd/mmcp/mmcp_Eastwin200_udi2.0/test/udi2/panel

Please select:
    1 - Clear and generate test framework code
    2 - Only generate test framework code
    3 - Generate test entry by test case code(CSTC_ functions)
    q - Quit
Enter your anwser(default: 1):2         【此处选2】

Enter module name(default:panel):PANEL  【输入模块名】
Module name is: PANEL

What do you want to generate:
    1 - Test framework code with demo testcase files
    2 - Test framework code for directory(autoscan sub dir)
    q - Quit
Enter your anwser(default: 1):2         【如果没有子目录，可以选1】

三、执行make命令，确保生成的测试框架编译通过

四、生成说明文件
   mktestdb .; mktestdoc testdb.lua

