1.InstallApp2Rom
���ļ���linux�����°�װӦ�õĹ��ߣ���Ҫ�ǰ�װĿ¼�����ļ���
$MMCP_HOME/javaresourcesĿ¼����

����.../aaa ��Ŀ¼������aaa1 aaa2����Ŀ¼��aaa1������TestTimer.class�ļ�

�÷�һ��cd .../aaa;
������ InstallApp2Rom -m aaa1; ��aaa1������$MMCP_HOME/javaresourcesĿ¼���棬
ͬʱɾ��*.java,*.h, CVS, .svn,Makefile,.sh,cvmtest,.local��.dtvm��.mhp,.zh,.en, makefile,
�����͵��ļ�����������Ҫ���ļ����ֶ�ɾ��Ҳ����ӵ�InstallApp2Rom���Զ�ɾ��������ҪĿ����
����.class�ļ�
������ InstallApp2Rom -m aaa2;ͬaaa1��-mѡ�װ��ǰĿ¼��Ӧ�õ�$MMCP_HOME/javaresources

�÷�����cd .../aaa 
������ InstallApp2Rom -a ��aaa1��aaa2ͬʱ������$MMCP_HOME/javaresources���棬������ͬ�÷�һ

�÷���:InstallApp2Rom -d ��Ĭ�ϵ�Ӧ�ÿ�����$MMCP_HOME/javaresources���棬����Ŀǰ��û����Ӧ��
���󣬴˷�����ʱû�п������ݡ�

�÷��ģ�InstallApp2Rom -b ͬ�÷���



2.mkromfs
���ļ��ǽ�$MMCP_HOME/javaresources�������������romdata[0-100].c�ļ��ŵ�$MMCP_HOME/generatedĿ¼�£�
$MMCP_HOME/generatedĿ¼�µ�Makefile�Ѿ�д�ã��ڴ�Ŀ¼�±���ʱ����cs_generated.a����
cs_generated.lib�ļ�

3.romfs��ʹ�÷���������ʱ����cs_generated.a����cs_generated.lib�ļ���
CSFSĿǰ����Ҫ��ʼ���Ϳ���ʹ��
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
