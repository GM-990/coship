经分析加解密接口输入PEM会引入更多的模块，如私钥肯定是要本地加密的，如果要直接引入PEM格式，则又要加入3DES等对称加密算法。当然也可以通过OpenSSL的rsa -text选项输出私钥明文。但PEM的明文解析也比较困难。
因此这里采取了一些更简单的手法，输入采用R_RSA_PRIVATE_KEY结构类似的table为lua模块传输key，而这些数据同样可以通过OpenSSL的rsa -text输出，然后用lua解析，相关代码之前已经写好。
（openssl rsa -in p2048.pem -text）

这次接着上次的工作，假设key解析已经完成，RsaModule.lua把key从hexstring转换成hex形式（这种形式在c中解析最简单），并形成table传给lua扩展块。
这次的重点在lua扩展模块，之前由于这样需要修改lua的elf文件一直极力避免。但这次实验发现动态链接库可以很好的解决这个问题。

makefile为我们演示了lua的
动态链接扩展
动态链接库与.a链接（rsaref.a库是静态编译的）

另外这也是一个基本可以直接使用的模块。

经实验，RSAEuro可以扩展到2048bits（这里已经修改），但rsaref不能，区别应该在大数算法上。

另外，这里的RSAEuro，rsaref20，lua-5.1.4三个开源项目下下来尽量最小改变。

需发布文件：
base64.lua
MmcpKeyGenerator.lua
csrsa.so
openssl