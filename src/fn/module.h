#ifndef DIM_MODULE_H
#define DIM_MODULE_H

#include "../dim.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct DIM_MODULES_CHAIN {
	JSCFunction* func;
	const char* name;
	struct DIM_MODULES_CHAIN* next;
};

struct DIM_MODULES_SCOPE {
	char* name;
	struct DIM_MODULES_CHAIN* funcs;
	struct DIM_MODULES_SCOPE* next;
};

struct DIM_MODULES_SCOPE* dim_modules;
struct DIM_MODULES_SCOPE* dim_modules_tail_ptr;

void dim_modules_init();
void dim_modules_free();

void createDimModuleScope(char* name);
void appendFnToDimModule(char* dim_module_name, JSCFunction* func, char* func_name);
bool includeDimModule(char* dim_module_name);

#endif
