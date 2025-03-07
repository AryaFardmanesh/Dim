#ifndef DIM_H
#define DIM_H

#include "../quickjs/quickjs.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define DIM_VERSION "1.0.0"

JSRuntime* runtime;
JSContext* context;
JSValue executeResult;
bool status;

char* script;

void dim_init();
void dim_free();

void handleError();

void dim_exe();

#include "module/module.h"

#endif
