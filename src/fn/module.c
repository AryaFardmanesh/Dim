#include "module.h"

void dim_modules_init() {
	dim_modules = NULL;
	dim_modules_tail_ptr = dim_modules;
}

void dim_modules_free() {
	struct DIM_MODULES_SCOPE* current_module = dim_modules;

	while (current_module != NULL) {
		if (current_module->funcs != NULL) {
			struct DIM_MODULES_CHAIN* current_funcs = current_module->funcs;

			while (current_funcs != NULL) {
				struct DIM_MODULES_CHAIN* next = current_funcs->next;
				free(current_funcs);
				current_funcs = next;
			}
		}

		struct DIM_MODULES_SCOPE* next_module = current_module->next;
		free(current_module);
		current_module = next_module;
	}

	free(dim_modules_tail_ptr);
}

void createDimModuleScope(char* name) {
	struct DIM_MODULES_SCOPE* dim_ms = (struct DIM_MODULES_SCOPE*)malloc(sizeof(struct DIM_MODULES_SCOPE));
	dim_ms->name = name;
	dim_ms->funcs = NULL;

	if (dim_modules == NULL) {
		dim_modules = dim_ms;
	}else {
		dim_modules_tail_ptr->next = dim_ms;
		dim_modules_tail_ptr = dim_modules_tail_ptr->next;
	}
}

void appendFnToDimModule(char* dim_module_name, JSCFunction* func, char* func_name) {
	struct DIM_MODULES_SCOPE* current_module = dim_modules;

	while (current_module != NULL) {
		if (strcmp(dim_module_name, current_module->name) == 0) {
			break;
		}
		current_module = current_module->next;
	}

	if (current_module == NULL) {
		fprintf(stderr, "[DIM]: Module Error: Cannot find '%s' module in modules chian.", dim_module_name);
		return;
	}

	if (current_module->funcs == NULL) {
		struct DIM_MODULES_CHAIN* node = (struct DIM_MODULES_CHAIN*)malloc(sizeof(struct DIM_MODULES_CHAIN));

		node->func = func;
		node->name = func_name;
		node->next = NULL;

		current_module->funcs = node;
	}else {
		struct DIM_MODULES_CHAIN* current_fn_chain = current_module->funcs;

		while (current_fn_chain->next != NULL) {
			current_fn_chain = current_fn_chain->next;
		}

		struct DIM_MODULES_CHAIN* node = (struct DIM_MODULES_CHAIN*)malloc(sizeof(struct DIM_MODULES_CHAIN));

		node->func = func;
		node->name = func_name;
		node->next = NULL;

		current_fn_chain->next = node;
		current_fn_chain = current_fn_chain->next;
	}
}

bool includeDimModule(char* dim_module_name) {
	struct DIM_MODULES_SCOPE* current_module = dim_modules;

	while (current_module != NULL) {
		if (strcmp(dim_module_name, current_module->name) == 0) {
			break;
		}
		current_module = current_module->next;
	}

	if (current_module == NULL) {
		fprintf(stderr, "[DIM]: Module Error: The '%s' module is not exist.", dim_module_name);
		return false;
	}

	struct DIM_MODULES_CHAIN* funcs = current_module->funcs;

	while (funcs != NULL) {
		dim_append_fn(funcs->func, funcs->name);
		funcs = funcs->next;
	}

	return true;
}
