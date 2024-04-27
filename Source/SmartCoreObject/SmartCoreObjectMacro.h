#pragma once

#ifndef SMARTENGINECORE_EXPORTS
#define SMARTENGINECORE_EXPORTS
#endif

#ifdef SMARTENGINECORE_EXPORTS
#define SMARTCOREOBJECT_API __declspec(dllexport)
#else
#define SMARTCOREOBJECT_API __declspec(dllimport)
#endif