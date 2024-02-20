#pragma once
#include "CoreObject/CoreMinimalObject.h"

class CLightComponent;
class CLightManage : public CCoreMinimalObject
{
	friend struct FGeometryMap;
public:
	void AddLight(CLightComponent* InLightStance);

	const vector<CLightComponent*>& GetLights() const { return Lights; }
protected:
	vector<CLightComponent*> Lights;
};