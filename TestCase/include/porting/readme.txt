目录说明：

udi2:
平台驱动接口2.0版本，其中包括所有应用（中间件）需要使用的驱动操作接口
对外移植中间件时只需实现该目录下的所有接口即可。

udi2plus:
udi2目录的补充，但这一部分的实现是平台无关的。中间件可以使用。

udiadapter:
udi2.0,1.0使用同一套代码的辅助适配器，帮助在不同的情况下，引用不同的头文件。
注意：这里仅保护简单的头文件引用及函数名称define，如仅接口名称变化，但不改变名称的。
对于调用复杂的过程不进行封装，如接口变化

csplus:
平台驱动接口2.0版本的初始化部分及内部模块部分（如I2C），当芯片厂商为coship Porting一个新平台时除需实现udi2外还需实现该目录下所有头文件。

udi,platform,hdi
平台驱动1.0版本，该部分的头文件部分内容由udi2.0定义，使用1.0的平台使用该部分头文件。