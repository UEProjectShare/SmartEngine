#pragma once
#include "../../../../../Engine/EngineMinimal.h"

class FBlueprintConnection : public std::enable_shared_from_this<FBlueprintConnection>
{
public:
	virtual void DrawConnection(const fvector_2d& InStartPosition, const fvector_2d& InEndPosition) = 0;
	
	virtual float GetThickness() const { return 5.f; }
	
	virtual float GetSubsection() const { return 40.f; }
	
	virtual ImU32 GetColor() const { return IM_COL32(255, 255, 255, 255); }
};