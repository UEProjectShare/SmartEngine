#pragma once

#include "CoreObject/CoreMinimalObject.h"
#include "CodeReflection/CodeReflectionMacroTag.h"
#include "../EngineType.h"
#include "World.CodeReflection.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GCamera;
class GActorObject;

class CWorld : public CCoreMinimalObject
{
	CODEREFLECTION()
public:
	CWorld();

	GCamera* GetCamera() const { return Camera; }

	template<class T>
	T* CreateActorObject()
	{
		FCreateObjectParam ParamType;
		ParamType.Outer = this;
		T* InArray = CreateObject<T>(ParamType, new T());

		char ObjectName[128] = { 0 };
		sprintf(ObjectName, "%s_%d",
			InArray->GetName().c_str(),
			ActorObjects.size());

		InArray->Rename(ObjectName);
		
		ActorObjects.push_back(InArray);

		return InArray;
	}
	
	bool LineTraceBySingle(FCollisionResult& OutResult,const fvector_3d& InStart, const fvector_3d& InEnd);

	const vector<GActorObject*>& GetActors() const { return ActorObjects; }

protected:
	CVARIABLE()
	GCamera* Camera;

	//´æ´¢ÎÒÃÇµÄActors
	CVARIABLE()
	vector<GActorObject*> ActorObjects;
};