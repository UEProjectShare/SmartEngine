#pragma once

#ifndef SMARTASSETHANDLE_EXPORTS
#define SMARTASSETHANDLE_EXPORTS
#endif

#ifdef SMARTASSETHANDLE_EXPORTS
#define SMARTASSETHANDLE_API __declspec(dllexport)
#else
#define SMARTASSETHANDLE_API __declspec(dllimport)
#endif