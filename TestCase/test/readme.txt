建立测试用例指南
王麟, 2008.7.26













一、说明
	a.更详细说明请参考《MMCP测试工程使用方法.doc》(位置：mmcp\trunk\test\doc)
	b.进行任何操作之前需要执行setupenv设置MMCP工作环境
	c.如果编译过程不正常，请更新trunk/include, trunk/build, trunk/bin, trunk/test/main, trunk/test/testkit，并确保库是最
新的
	d.make某测试模块（如abc）后生成的程序为: "abc/exec/平台名称/abctest.out"程序
	e.测试用例函数必须以CSTC_做为前缀


二、编译与运行
	1. [推荐]任意测试模块， 如test, test/dtv, test/dtv/epg等:
		a. 进入测试模块目录
		b. make, make run （显示测试菜单），或者：
		c. mmcptest -ca（相当make, make run，但不显示测试代码，自动启动测试）

	2. 测试二级模块，即test下的dtv、protocol、graphic、porting还可以（以dtv为例）
		a. 进入mmcp/trunk/test目录
		b. make dtv
		c. cd dtv

	3. dtvm模块比较特殊，请在mmcp/trunk/test下make dtvm

	4. 自动更新、编译、运行所有测试用例
		a.进入任意目录
		b.mmcptest -a，自动启动测试，不显示菜单

	注意：在某目录下make出的测试程序，将包含其下所有子目录的测试代码。


三、自动生成测试框架
	
	1.cstfg(Coship Test Framework Generator)是测试框架代码自动生成工具
	2.在trunk/bin目录下，setupevn后可在任何目录下直接执行。
	3.生成的测试代码分为2种：
		a. 带有测试用例代码的，如epg，
		b. 不带测试代码，仅仅是一个目录，如dtv
		c.在cstfg提问“What do you want to generate”的时候，可以进行选择生成哪种测试代码

四、测试框架集成过程
现在需要在abc目录下建立测试用例，则：
1.进入abc目录
2.执行cstfg，根据提示生成框架代码
3.在abc目录下make, make生成的可执行程序为“abc目录/exec/平台名称/abctest.out”
4.完善用例代码(.c/.cpp)；编译链接通过；执行cstfg，第1步中选3，通过测试代码自动重新生成cs_*testcase.h和cs_*testentry.c文件；再次编译连接并运行
5.将abc关联到上级模块：
	a. 进入在abc的上层目录，执行cstfg
	b. 在cstfg提问“What do you want to generate”的时候，选择2，将自动扫描top的子目录，生成测试框架
	c. make，生成的可执行程序为“top目录/exec/平台名称/toptest.out”，
	d. 执行toptest.out可以看到菜单中有ABC
注意：上面第4步要求测试用例的函数必须符合《MMCP测试工程使用方法.doc》的命名规范；如果生成不正常，请手动修改cs_*testcase.h和cs_*testentry.c文件。


五、需要测试者判断结果的测试代码编写要求
	对于测试结果需要与测试者交互的测试用例，其测试代码应该包括以下几个部分：
	a) 测试开始前，提示测试者将要发生什么（如提示：现在将播放一段音视频测试AV同步）
	b) 调用CSTKWaitAnyKey等待测试者输入，用户输入任意按键后向下执行
	c) 执行测试代码（如：播放一段音视频）
	d) 程序输出提示，告诉测试者在正常情况下应该发现（如提示：是否有同步的音视频输出？）
	e) 调用CSTKWaitYes（返回TRUE表示肯定），根据返回值判断测试是否成功

六、测试集
	目前一些测试代码中，并非以单个测试用例为一个测试函数，而是一组测试用例。这种情况下，一个测试函数将测试若干个用例，此时如果某个测试用例失败，可以：
	1. 调用CSTK_ASSERT，普通断言
	2. 调用CSTK_ASSERT_FAILED，带返回值的断言
	两个断言可参见cs_testkit.h说明
	

