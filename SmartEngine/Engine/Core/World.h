#pragma once

#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../EngineType.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GCamera;
class GActorObject;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();

	GCamera* GetCamera() const { return Camera; }

	template<class T>
	T *CreateActorObject()
	{
		T *InArray = new T();
		InArray->SetOuter(this);
		
		ActorObjects.push_back(InArray);

		return InArray;
	}
	bool LineTraceBySingle(FCollisionResult& OutResult,const fvector_3d& InStart, const fvector_3d& InEnd);

protected:
	CVARIABLE()
	GCamera* Camera;

	//´æ´¢ÎÒÃÇµÄActors
	CVARIABLE()
	vector<GActorObject*> ActorObjects;
};