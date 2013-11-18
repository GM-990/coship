#include <./prepare/prepare_qt.h>
#include <./udi_init/bcm7403/N8606/udi_init.h>
#include <./udi_init/bcm7403/N8606E/udi_init.h>
#include <./udi_init/mtk8652/demo/udi_init.h>
#include <./udi_init/bcm7406/demo/udi_init.h>
#include <./udi_init/bcm7019/demo/udi_init.h>
#include <./udi_init/st71xxos21/demo/udi_init.h>
#include <./udi_init/bcm7309/N7300/udi_init.h>
#include <./udi_init/hi3716H/demo/udi_init.h>
#include <./udi_init/bcm7468/demo/udi_init.h>
#include <./udi_init/pnx8473/demo/udi_init.h>
#include <./udi_init/bcm75xx/demo/udi_init.h>
#include <./udi_init/hi3716M/demo/udi_init.h>
#include <./udi_init/st7100/ChengDuXingWang/udi_init.h>
#include <./udi_init/st7100/NingBoGaoQing/udi_init.h>
#include <./udi_init/st7100/TaiWanTop/udi_init.h>
#include <./udi_init/st71xxlinux/demo/udi_init.h>
#include <./udi_init/linux/x86/udi_init.h>
#include <./udi_init/st7111/demo/udi_init.h>
#include <./udi_init/st5105/HuNan_mmcp/udi_init.h>
#include <./udi_init/win32/udi_init.h>
#include <./udi_init/cx24501/demo/udi_init.h>
#include <./udi_init/hi3110E/demo/udi_init.h>
#include <./udi_init/st5197/demo/udi_init.h>
#include <./udi_init/bcm7401/demo/udi_init.h>
#include <./udi_init/SupraXD_180/demo/udi_init.h>
#include <./udi_init/stih237_linux/demo/udi_init.h>
#include <./version.h>
#include <./cs_maintestentry.h>
#include <./CSAppRC.h>
#include <./setup.h>
#include <./appconfig.h>
#include <./setup_dtv.h>
#include <./irdecode.h>
#include <./udi_init.h>

struct NativeFunctionSymbols { 
	char *FuncName;
	void* (*Function)();
};

const struct NativeFunctionSymbols function_symbols[] = {
	{0, 0}
};
