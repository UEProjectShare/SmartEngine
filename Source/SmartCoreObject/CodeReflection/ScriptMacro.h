#pragma once

#define FUNCTION_DEFINITION(Func) static void Func(CCoreMinimalObject* Context, FFrame& Stack, void const* RefData)
#define FUNCTION_IMPLEMENTATION(Func) void Func(CCoreMinimalObject* Context, FFrame& Stack, void const* RefData)