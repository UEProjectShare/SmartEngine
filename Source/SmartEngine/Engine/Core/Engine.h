#pragma once

#include "CoreObject/CoreMinimalObject.h"

#if defined(_WIN32)
#include "WinMainCommandParameters.h"
#endif // 1

#include "Engine.CodeReflection.h"

class CEngine : public CCoreMinimalObject
{
	CODEREFLECTION()
public:
	CEngine();

	virtual int PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif // 1
	) = 0;

	virtual int Init(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif // 1
	) = 0;
	
	virtual int PostInit() = 0;

	void Tick(float DeltaTime) override {};

	virtual void OnResetSize(int InWidth, int InHeight) = 0;

	virtual int PreExit() = 0;
	
	virtual int Exit() = 0;
	
	virtual int PostExit() = 0;
};