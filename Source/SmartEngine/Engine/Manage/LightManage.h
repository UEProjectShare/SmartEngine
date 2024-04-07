#pragma once
#include "CoreObject/CoreMinimalObject.h"
#include "LightManage.CodeReflection.h"

class CLightComponent;
class CLightManage : public CCoreMinimalObject
{
	CODEREFLECTION()
	
	friend struct FGeometryMap;
public:
	void AddLight(CLightComponent* InLightStance);

	const vector<CLightComponent*>& GetLights() const { return Lights; }
protected:
	vector<CLightComponent*> Lights;
};