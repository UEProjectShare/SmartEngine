#pragma once 
#include "../../../Core/Engine.h"
#include "RenderingEngine.CodeReflection.h"

class CRenderingEngine : public CEngine
{
	CODEREFLECTION()
public:
	void SetMainWindowsHandle(HWND InNewMainWindowsHandle);

protected:
	HWND MainWindowsHandle;
};