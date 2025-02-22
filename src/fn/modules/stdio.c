#include <stdio.h>
#include "../module.h"

static JSValue dim_stdio_print(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
	if (argc < 1) {
		return JS_ThrowTypeError(ctx, "[DIM]: Error: The 'print' function expected one argument.");
	}

	printf("%s", JS_ToCString(context, argv[0]));

	return JS_UNDEFINED;
}

static JSValue dim_stdio_input(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
	if (argc < 1) {
		printf("%s", JS_ToCString(context, argv[0]));
	}

	size_t line_size = 1;
	char ch = (char)0x0;
	char* line = (char*)malloc(line_size);

	while (1) {
		ch = getchar();
		if (ch == '\n') {
			break;
		}
		line[line_size - 1] = ch;
		line = (char*)realloc(line, ++line_size);
	}

	line[line_size - 1] = (char)0x0;

	return JS_NewString(context, line);
}

void dim_lib_stdio() {
	createDimModuleScope("stdio");

	appendFnToDimModule("stdio", dim_stdio_print, "print");
	appendFnToDimModule("stdio", dim_stdio_input, "input");
}
