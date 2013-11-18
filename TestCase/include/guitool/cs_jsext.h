#ifndef __CS_JSEXT_H__
#define __CS_JSEXT_H__

#include "mmcp_typedef.h"
#include "object.h"

namespace KJS {
	class JSCoship : public JSObject {
	public:
		virtual bool getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot) {return false;};
		virtual JSValue *getValueProperty(ExecState *exec, int token) const {return jsNull();};
	};
}

// 构造 JSCoship 供Guitool 使用，注意：是否操作由内部进行，外部仅负责申请内存
typedef KJS::JSCoship* (*JSObjFactory)();

// 注册一个根节点
int CSGTRegisterJSExtern(HCSHANDLE hHandle, JSObjFactory root);

#ifdef __cplusplus
extern "C" {
#endif

//此函数只能在浏览器线程里面使用
char * CSGTDupCompleteURL(const char* url, KJS::ExecState* exec);

void CSGTSetDrawFocusRing(KJS::ExecState *exec, int nSet);

#ifdef __cplusplus
}
#endif

#endif
