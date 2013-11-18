
struct NativeFunctionSymbols { 
	char *FuncName;
	void* (*Function)();
};

const struct NativeFunctionSymbols function_symbols[] = {
	{0, 0}
};
