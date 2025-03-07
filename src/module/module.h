/************************************************************************************************************************
*	Dim Engine 2025 - Modules System                                                                               	*
*	Developer(s):                                                                                                 	*
*		- Arya Fardmanesh                                                                                     	*
*	Description: This file is for implementing modularization of JavaScript internal functions.                   	*
*	Implementation: The way it is implemented is that we have a struct for modules, each module struct contains a	*
*	name and a pointer to the functions in it. These structs are connected together as a linked list. To load a   	*
*	module, we need to find its name in the module list and then load its functions into the QuickJS kernel.      	*
************************************************************************************************************************/

#ifndef DIM_MODULE_H
#define DIM_MODULE_H

// #define DEBUG

#include "../../quickjs/quickjs.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct DIM_MODULE_FUNCS {
	char* name;
	JSCFunction* function;
	struct DIM_MODULE_FUNCS* next;
};

struct DIM_MODULE_LIST {
	char* name;
	bool loaded;
	struct DIM_MODULE_FUNCS* functions;
	struct DIM_MODULE_LIST* next;
};

struct DIM_MODULE_LIST* _dim_module;
struct DIM_MODULE_LIST* _dim_module_end_ptr;

void dim_module_init();
void dim_module_free();

struct DIM_MODULE_FUNCS* dim_module_create_func(char* name, JSCFunction* function);
struct DIM_MODULE_LIST* dim_module_create_module(char* name, struct DIM_MODULE_FUNCS* functions);

struct DIM_MODULE_LIST* dim_module_search(char* name);

void dim_module_append_func(char* module_name, struct DIM_MODULE_FUNCS* function);
void dim_module_append_module(struct DIM_MODULE_LIST* module);

void dim_load_fn(JSCFunction* func, const char* name);
void dim_module_load(char* name);
void load_modules_in_dim_module();
void load_native_functions();

// ==================== Define Functions ====================
void dim_module_init() {
	_dim_module = NULL;
	_dim_module_end_ptr = _dim_module;

	// Load all modules in '_dim_module'.
	load_modules_in_dim_module();

	// Load outer module functions.
	load_native_functions();
}

void dim_module_free() {
	#ifdef DEBUG
	printf("DEBUG: DIM_MODULE_FREE Start.\n");
	#endif

	struct DIM_MODULE_LIST* current_module = _dim_module;

	while (current_module != NULL) {
		struct DIM_MODULE_LIST* next_module = current_module->next;
		struct DIM_MODULE_FUNCS* module_function = current_module->functions;

		#ifdef DEBUG
		printf("DEBUG: DIM_MODULE_FREE: Select '%s' module to free.\n", current_module->name);
		printf("DEBUG: DIM_MODULE_FREE: Start free the modules function.\n");
		#endif

		while (module_function != NULL) {
			struct DIM_MODULE_FUNCS* module_function_next = module_function->next;
			free(module_function);

			#ifdef DEBUG
			printf("DEBUG: DIM_MODULE_FREE: freed '%s::%s' function.\n", current_module->name, module_function->name);
			#endif

			module_function = module_function_next;
		}

		#ifdef DEBUG
		printf("DEBUG: DIM_MODULE_FREE: The '%s' module freed.\n", current_module->name);
		#endif

		free(current_module);
		current_module = next_module;
	}

	#ifdef DEBUG
	printf("DEBUG: DIM_MODULE_FREE Finish.\n");
	#endif
}

struct DIM_MODULE_FUNCS* dim_module_create_func(char* name, JSCFunction* function) {
	struct DIM_MODULE_FUNCS* node = (struct DIM_MODULE_FUNCS*)malloc(sizeof(struct DIM_MODULE_FUNCS));

	node->name = name;
	node->function = function;
	node->next = NULL;

	return node;
}

struct DIM_MODULE_LIST* dim_module_create_module(char* name, struct DIM_MODULE_FUNCS* functions) {
	struct DIM_MODULE_LIST* node = (struct DIM_MODULE_LIST*)malloc(sizeof(struct DIM_MODULE_LIST));

	node->name = name;
	node->loaded = false;
	node->functions = functions;
	node->next = NULL;

	return node;
}

struct DIM_MODULE_LIST* dim_module_search(char* name) {
	struct DIM_MODULE_LIST* current_module = _dim_module;

	while (current_module != NULL) {
		if (strcmp(current_module->name, name) == 0) {
			break;
		}

		current_module = current_module->next;
	}

	return current_module;
}

void dim_module_append_func(char* module_name, struct DIM_MODULE_FUNCS* function) {
	#ifdef DEBUG
	printf("DEBUG: DIM_MODULE_APPEND_FUNC: Start add '%s' function to '%s' module\n", function->name, module_name);
	#endif

	struct DIM_MODULE_LIST* target_module = dim_module_search(module_name);

	if (target_module == NULL) {
		fprintf(stderr, "[DIM]: InternalError: Cannot find the '%s' module.\n", module_name);
		return;
	}

	if (target_module->functions == NULL) {
		target_module->functions = function;

		#ifdef DEBUG
		printf("DEBUG: DIM_MODULE_APPEND_FUNC: Appended successfully.\n");
		#endif

		return;
	}

	struct DIM_MODULE_FUNCS* current_function = target_module->functions;

	while (current_function->next != NULL) {
		current_function = current_function->next;
	}

	current_function->next = function;

	#ifdef DEBUG
	printf("DEBUG: DIM_MODULE_APPEND_FUNC: Appended successfully.\n");
	#endif
}

void dim_module_append_module(struct DIM_MODULE_LIST* module) {
	#ifdef DEBUG
	printf("DEBUG: DIM_MODULE_APPEND_MODULE: Start append new module with '%s' name.\n", module->name);
	#endif

	if (_dim_module == NULL) {
		_dim_module = module;
		_dim_module_end_ptr = _dim_module;

		#ifdef DEBUG
		printf("DEBUG: DIM_MODULE_APPEND_MODULE: Appended successfully.\n");
		#endif

		return;
	}

	_dim_module_end_ptr->next = module;
	_dim_module_end_ptr = module;

	#ifdef DEBUG
	printf("DEBUG: DIM_MODULE_APPEND_MODULE: Appended successfully.\n");
	#endif
}

void dim_load_fn(JSCFunction* func, const char* name) {
	// Load the function in QuickJS kernel.
	// Actually set the function in global object in JavaScript.

	JSValue global_obj = JS_GetGlobalObject(context);
	JS_SetPropertyStr(context, global_obj, name, JS_NewCFunction(context, func, name, 0));
	JS_FreeValue(context, global_obj);
}

void dim_module_load(char* name) {
	#ifdef DEBUG
	printf("DEBUG: DIM_MODULE_LOAD: Start load the '%s' module.\n", name);
	#endif

	struct DIM_MODULE_LIST* target_module = dim_module_search(name);

	if (target_module == NULL) {
		fprintf(stderr, "[DIM]: Error: Cannot find the '%s' module.\n", name);
		return;
	}

	if (target_module->loaded == true) {
		#ifdef DEBUG
		printf("DEBUG: DIM_MODULE_LOAD: The '%s' module is already loaded.\n", name);
		#endif

		return;
	}else {
		target_module->loaded = true;
	}

	struct DIM_MODULE_FUNCS* current_function = target_module->functions;

	while (current_function != NULL) {
		#ifdef DEBUG
		printf(
			"--- INFO ---\n"
			"\tFUNCTION_NAME='%s'\n"
			"\tSTRCUT_PTR='%p'\n"
			"\tFUNCTION_PTR='%p'\n"
			"\tNEXT_PTR='%p'\n"
			"--- INFO ---\n",
			current_function->name,
			current_function,
			current_function->function,
			current_function->next
		);
		#endif

		dim_load_fn(current_function->function, current_function->name);
		current_function = current_function->next;
	}

	#ifdef DEBUG
	printf("DEBUG: DIM_MODULE_LOAD: Module loaded successfully.\n");
	#endif
}

#include "../libs/stdio.h"
#include "../libs/string.h"

void load_modules_in_dim_module() {
	stdio_module_init();
	string_module_init();
}
// ==================== Define Functions ====================

// ==================== Define Native Functions ====================
static JSValue dim_fn_include(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
	if (argc < 1) {
		return JS_ThrowTypeError(ctx, "[DIM]: ArgumentError: The 'include' function expected one argument.\n");
	}

	char* libname = (char*)JS_ToCString(ctx, argv[0]);
	dim_module_load(libname);
	JS_FreeCString(context, libname);

	return JS_UNDEFINED;
}

void load_native_functions() {
	dim_load_fn(dim_fn_include, "include");
}
// ==================== Define Native Functions ====================

#endif
