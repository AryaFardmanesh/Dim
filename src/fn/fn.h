#ifndef FN_H
#define FN_H

void dim_append_fn(JSCFunction* func, const char* name);
void dim_load_fn();

#include "../dim.h"
#include "module.h"
#include "modules/stdio.c"

#include <stdio.h>
#include <string.h>

// ========== JS Native Functions ==========
static JSValue dim_fn_include(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
	if (argc < 1) {
		return JS_ThrowTypeError(ctx, "DIM::ERROR $ the 'print' function expected one argument.");
	}

	char* libname = (char*)JS_ToCString(ctx, argv[0]);
	includeDimModule(libname);

	return JS_UNDEFINED;
}
// ========== JS Native Functions ==========

void dim_append_fn(JSCFunction* func, const char* name) {
	JSValue global_obj = JS_GetGlobalObject(context);
	JS_SetPropertyStr(context, global_obj, name, JS_NewCFunction(context, func, name, 0));
	JS_FreeValue(context, global_obj);
}

void dim_load_fn() {
	dim_append_fn(dim_fn_include, "include");

	dim_lib_stdio();
}

#endif
