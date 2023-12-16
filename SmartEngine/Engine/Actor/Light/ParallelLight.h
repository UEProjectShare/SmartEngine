#pragma once
#include "../Core/ActorObject.h"
#include "../../Component/Light/ParallelLightComponent.h"

//G -> Game
class GParallelLight : public GActorObject
{
	CVARIABLE()
	CParallelLightComponent* ParallelLightComponent;
public:


};