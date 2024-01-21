//Copyright (C) RenZhai.2022.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../Common/Core/GamePlay/PlayerStateBase.h"
#include "../../MMOARPGGameType.h"
#include "MMOARPGType.h"
#include "MMOARPGPlayerState.generated.h"

/**
 *
 */
UCLASS()
class SMARTBUILDTOOL_API AMMOARPGPlayerState : public APlayerStateBase
{
	GENERATED_BT_BODY()

	UPROPERTY(CodeType = Resources)
	int Num;

	UPROPERTY(CodeType = Resources)
	bool PlayerState;

public:
	UFUNCTION(CodeType = Event)
	void Hello(int c, float b);

	UFUNCTION(CodeType = Describe)
	void Hello1();
};
