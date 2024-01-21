#pragma once

#ifndef RENZHAIENGINECORE_EXPORTS
#define RENZHAIENGINECORE_EXPORTS
#endif

#ifdef RENZHAIENGINECORE_EXPORTS
#define RENZHAIENGINECORE_API __declspec(dllexport)
#else
#define RENZHAIENGINECORE_API __declspec(dllimport)
#endif // EEE