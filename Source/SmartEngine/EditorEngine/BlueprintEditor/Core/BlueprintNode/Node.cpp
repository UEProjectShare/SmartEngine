#include "Node.h"
#include "../../BlueprintConfigInfo.h"

FNode::FNode()
{
}

bool FNode::IsInTheScope(const fvector_2d& InCurrentPosition) const
{
    fvector_2d StartPosition = GetPosition();
    fvector_2d EndPosition = StartPosition + NodeSize;

    if (StartPosition.x <= InCurrentPosition.x && StartPosition.y <= InCurrentPosition.y &&
        InCurrentPosition.x <= EndPosition.x && InCurrentPosition.y <= EndPosition.y)
    {
        return true;
    }

    return false;
}

void FNode::ZoomSize(float InRatio)
{

}

void FNode::ZoomOffset(float InRatio)
{
    OffsetPosition = StandardOffsetPosition* InRatio;
}
