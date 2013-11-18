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

// ���� JSCoship ��Guitool ʹ�ã�ע�⣺�Ƿ�������ڲ����У��ⲿ�����������ڴ�
typedef KJS::JSCoship* (*JSObjFactory)();

// ע��һ�����ڵ�
int CSGTRegisterJSExtern(HCSHANDLE hHandle, JSObjFactory root);

#ifdef __cplusplus
extern "C" {
#endif

//�˺���ֻ����������߳�����ʹ��
char * CSGTDupCompleteURL(const char* url, KJS::ExecState* exec);

void CSGTSetDrawFocusRing(KJS::ExecState *exec, int nSet);

#ifdef __cplusplus
}
#endif

#endif
