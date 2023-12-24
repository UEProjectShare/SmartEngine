#pragma once
#include "ActorMeshConstruction.h"


#define CREATE_RENDER_DATA(T, ...)\
ActorMeshConstruction::CreateMeshRenderData<T>(GetMeshManage(), this, __VA_ARGS__);

//##args