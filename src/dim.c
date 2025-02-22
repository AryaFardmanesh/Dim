#include "dim.h"

void dim_init(char* program) {
	runtime = JS_NewRuntime();
	context = JS_NewContext(runtime);
	status = true;
	script = program;

	dim_modules_init();
	dim_load_fn();
}

void dim_free() {
	JS_FreeValue(context, executeResult);
	JS_FreeContext(context);
	JS_FreeRuntime(runtime);
	dim_modules_free();
	free(script);
}

void handleError() {
	if (JS_IsException(executeResult)) {
		status = false;
		JSValue exception = JS_GetException(context);
		const char *error = JS_ToCString(context, exception);
		fprintf(stderr, "[DIM]: Error: %s\n", error);
		JS_FreeCString(context, error);
		JS_FreeValue(context, exception);
	}
}

void dim_exe() {
	executeResult = JS_Eval(context, script, strlen(script), "<input>", JS_EVAL_TYPE_GLOBAL);
	handleError();
}

int main(int argc, char** argv) {
	// Get the JavaScript file.
	if (argc < 2) {
		fprintf(stderr, "[DIM]: Error: No file specified.");
		exit(EXIT_FAILURE);
	}

	const char* address = argv[1];

	FILE* fptr = fopen(address, "r");
	if (fptr == NULL) {
		fprintf(stderr, "[DIM]: Error: Cannot read the specified file.");
		exit(EXIT_FAILURE);
	}

	char ch;
	size_t fileSize = 1;
	char* fileContent = (char*)malloc(fileSize);

	while ((ch = fgetc(fptr)) != EOF) {
		fileContent[fileSize - 1] = ch;
		fileContent = (char*)realloc(fileContent, ++fileSize);
	}
	fileContent[fileSize - 1] = (char)0x0;

	fclose(fptr);

	// Execute the JavaScript file
	dim_init(fileContent);
	dim_exe();
	dim_free();

	return EXIT_SUCCESS;
}
