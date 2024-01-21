#pragma once

#include "../../SmartEngineCoreMacro.h"
#include "../../SmartEngineCore.h"

struct RENZHAIENGINECORE_API FEnginePathHelper
{
	static std::string GetEngineRootPath();

	static std::string GetEngineBinariesPath();

	static std::string GetEngineLogsPath();

	static std::string GetEngineContentPath();

	static std::wstring GetEngineShadersPath();

	static std::string RelativeToAbsolutePath(const std::string &InPath);
};