#pragma once

#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GCamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();

	GCamera* GetCamera() const { return Camera; }

protected:
	CVARIABLE()
	GCamera* Camera;
};