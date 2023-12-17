#pragma once
#include "../Core/CoreObject/CoreMinimalObject.h"

class CLightComponent;
class CLightManage : public CCoreMinimalObject
{
	friend struct FGeometryMap;
public:
	void AddLight(CLightComponent* InLightStance);

protected:
	vector<CLightComponent*> Lights;
};