#ifndef DIM_STDIO_MOD_H
#define DIM_STDIO_MOD_H

#define MODULE_NAME "stdio"

#include "../module/module.h"
#include "../dim.h"
#include <stdio.h>
#include <stdlib.h>

static JSValue stdio_echo(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
	if (argc < 1) {
		return JS_ThrowTypeError(ctx, "[DIM]: ArgumentError: The 'echo' function expected one argument.\n");
	}

	char* value = (char*)JS_ToCString(ctx, argv[0]);
	printf("%s", value);
	JS_FreeCString(context, value);

	return JS_UNDEFINED;
}

static JSValue stdio_input(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
	if (argc >= 1) {
		char* value = (char*)JS_ToCString(ctx, argv[0]);
		printf("%s", value);
		JS_FreeCString(context, value);
	}

	size_t str_size = 1;
	char* str = (char*)malloc(str_size);
	char ch = (char)0x0;

	while ((int)(ch = (char)getchar()) != (int)'\n') {
		str[str_size - 1] = ch;
		str = (char*)realloc(str, ++str_size);
	}

	str[str_size - 1] = (char)0x0;

	return JS_NewString(context, (const char*)str);
}

void stdio_module_init() {
	struct DIM_MODULE_LIST* module = dim_module_create_module(MODULE_NAME, NULL);
	dim_module_append_module(module);

	struct DIM_MODULE_FUNCS* stdio_echo_struct = dim_module_create_func("echo", stdio_echo);
	dim_module_append_func(MODULE_NAME, stdio_echo_struct);

	struct DIM_MODULE_FUNCS* stdio_input_struct = dim_module_create_func("input", stdio_input);
	dim_module_append_func(MODULE_NAME, stdio_input_struct);
}

#undef MODULE_NAME

#endif
