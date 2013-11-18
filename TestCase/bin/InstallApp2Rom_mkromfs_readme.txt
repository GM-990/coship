1.InstallApp2Rom
该文件是linux环境下安装应用的工具，主要是安装目录或者文件到
$MMCP_HOME/javaresources目录下面

假如.../aaa 该目录下面有aaa1 aaa2两个目录，aaa1下面有TestTimer.class文件

用法一：cd .../aaa;
命令行 InstallApp2Rom -m aaa1; 将aaa1拷贝到$MMCP_HOME/javaresources目录下面，
同时删除*.java,*.h, CVS, .svn,Makefile,.sh,cvmtest,.local，.dtvm，.mhp,.zh,.en, makefile,
等类型的文件，其他不需要的文件可手动删除也可添加到InstallApp2Rom中自动删除，其主要目的是
留下.class文件
命令行 InstallApp2Rom -m aaa2;同aaa1；-m选项安装当前目录的应用到$MMCP_HOME/javaresources

用法二：cd .../aaa 
命令行 InstallApp2Rom -a 将aaa1和aaa2同时拷贝到$MMCP_HOME/javaresources下面，其他的同用法一

用法三:InstallApp2Rom -d 将默认的应用拷贝到$MMCP_HOME/javaresources下面，由于目前还没有相应的
需求，此方法暂时没有拷贝内容。

用法四：InstallApp2Rom -b 同用法三



2.mkromfs
此文件是将$MMCP_HOME/javaresources下面的内容生成romdata[0-100].c文件放到$MMCP_HOME/generated目录下，
$MMCP_HOME/generated目录下的Makefile已经写好，在此目录下编译时生成cs_generated.a或者
cs_generated.lib文件

3.romfs的使用方法，链接时包含cs_generated.a或者cs_generated.lib文件，
CSFS目前不需要初始化就可以使用
int error;
static char *global_argv[] = {"mmcp"/*, "-Xtrace:32", "-Xms2048k", "-Xgc:youngGen=1024k"*/, 
	"-Xss256k", "-Xbootclasspath=/workdir/MMCP_resources/class/aaa1",
	"-Djava.class.path=/workdir/MMCP_resources/class/aaa1", "TestTimer"}; 
static int global_argc = 5;

error = CSFSMkdir("/workdir", 0);

error = CSFSMkdir("/workdir/MMCP_resources", 0);

error = CSFSMkdir("/workdir/MMCP_resources/class", 0);

error = CSFSMount("rom0", "/workdir/MMCP_resources/class", "romfs", 0, 0);

ansiJavaMain(global_argc, global_argv);
