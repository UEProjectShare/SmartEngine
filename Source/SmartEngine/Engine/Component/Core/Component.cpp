#include "Component.h"

CComponent::CComponent()
{
    Parent = nullptr;
}

void CComponent::AddChild(CComponent* InChild)
{
    Children.push_back(InChild);
}
