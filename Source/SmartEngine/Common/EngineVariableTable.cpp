#include "EngineVariableTable.h"
#include "../Engine/EngineMacro.h"
#include "CoreMacro.h"

int ActorSelected = 0;
class GActorObject* SelectedObject = nullptr;
class CMeshComponent* SelectAxisComponent = nullptr;
bool bOperationHandleSelect = false;
#if EDITOR_ENGINE
class GMoveArrow* MoveArrow = nullptr;
class GScalingArrow* ScalingArrow = nullptr;
class GRotatorArrow* RotatorArrow = nullptr;
#endif // EDITOR_ENGINE