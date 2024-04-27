#pragma once
#include "../Interface/BlueprintConnection.h"

class FBlueprintBezierQuadraticConnection : public FBlueprintConnection
{
public:
	void DrawConnection(const fvector_2d& InStartPosition, const fvector_2d& InEndPosition) override;
};