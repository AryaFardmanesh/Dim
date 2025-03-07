#ifndef DIM_STRING_MOD_H
#define DIM_STRING_MOD_H

#define MODULE_NAME "string"

#include "../module/module.h"
#include "../dim.h"
#include <string.h>
#include <stdlib.h>

static JSValue string_strlen(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
	if (argc < 1) {
		return JS_ThrowTypeError(ctx, "[DIM]: ArgumentError: The 'string_strcpy' function expected one argument.\n");
	}

	char* value = (char*)JS_ToCString(ctx, argv[0]);
	int32_t string_len = (int32_t)strlen(value);
	JS_FreeCString(context, value);

	return JS_NewInt32(context, string_len);
}

static JSValue string_strcpy(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
	if (argc < 1) {
		return JS_ThrowTypeError(ctx, "[DIM]: ArgumentError: The 'strlen' function expected one argument.\n");
	}

	char* value = (char*)JS_ToCString(ctx, argv[0]);
	char* new_str = (char*)malloc(strlen(value));
	strcpy(new_str, value);
	JS_FreeCString(context, value);

	return JS_NewString(context, new_str);
}

void string_module_init() {
	struct DIM_MODULE_LIST* module = dim_module_create_module(MODULE_NAME, NULL);
	dim_module_append_module(module);

	struct DIM_MODULE_FUNCS* string_strlen_s = dim_module_create_func("strlen", string_strlen);
	dim_module_append_func(MODULE_NAME, string_strlen_s);

	struct DIM_MODULE_FUNCS* string_strcpy_s = dim_module_create_func("strcpy", string_strcpy);
	dim_module_append_func(MODULE_NAME, string_strcpy_s);
}

#undef MODULE_NAME

#endif
