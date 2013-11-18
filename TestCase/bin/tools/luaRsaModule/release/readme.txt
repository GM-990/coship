MMCP序列号签名工具使用手册

本工具理论上可在任意x86 Linux终端上执行，已在FC6上测试通过。

1、生成签名
./MmcpKeyGenerator.lua -key priv.pem -in maclist.txt，详细请参考./MmcpKeyGenerator.lua --help
生成签名前，请注意核对其头部的信息是否正确，包括manufacturer，customer，stbtype，stbtypeid。
生成签名时，需输入生成密钥是的密码。

2、生成密钥
openssl genrsa -des3 -out xxx.pem 1024
其中1024表示密钥位数，请选择1024位。
生成过程中会要求输入密钥本地加密密码，请牢记此密码
生成后的密钥文件为xxx.pem，请自行改名，并保存好该文件。

3、导出公钥
openssl rsa -in xxx.pem -out yyy.pem -pubout
其中xxx.pem为密钥，可能已经改名。
yyy.pem为生成的公钥，请自行改名，并将其提供给集成人员导入到校验代码中。
导出公钥过程中需输入生成密钥时的密码。

4、注意事项：
请妥善保存好每个项目的密钥文件，并确保在项目中使用了对应的公钥！
请妥善保存好每个密钥的本地密码，该密码如遗忘，不可恢复！
请妥善保存好每个项目对应的信息，如stbtypeid等，生成过程切不可出错，否则会目标终端造成启动失败！
若执行MmcpKeyGenerator.lua脚本时提示：
-bash: ./MmcpKeyGenerator.lua: /usr/sbin/lua: bad interpreter: 没有那个文件或目录
请将本目录下lua拷贝到/usr/sbin/目录下，并增加可执行权限。