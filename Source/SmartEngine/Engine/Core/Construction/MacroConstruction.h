#pragma once
#include "ActorMeshConstruction.h"


#define CREATE_RENDER_DATA(T, ...)\
ActorMeshConstruction::CreateMeshRenderData<T>(GetMeshManage(), this, __VA_ARGS__);

#define CREATE_RENDER_DATA_BY_COMPONENT(T, Component, ...)\
ActorMeshConstruction::CreateMeshRenderDataByComponent<T>(GetMeshManage(), dynamic_cast<T*>(Component), __VA_ARGS__);

//##args