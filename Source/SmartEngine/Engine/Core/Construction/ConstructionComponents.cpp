#include "ConstructionComponents.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Component/Core/Component.h"
#include "../../Actor/Core/ActorObject.h"
#include "../../Component/TransformationComponent.h"

namespace ConstructionComponent
{
	void UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent)
	{
		if (InParentComponent != InComponent)//����Attach�Լ�
		{
			//���õ�����
			InParentComponent->AddChild(InComponent);

			//���ø�����˭
			InComponent->SetParentComponent(InParentComponent);
		}
	}

	void ConstructionComponents(CCoreMinimalObject* InOuter, CCoreMinimalObject* NewObject)
	{
		if (CComponent* InComponent = dynamic_cast<CComponent*>(NewObject))
		{
			if (InOuter)
			{
				//Outer��һ�����
				if (CComponent* InOuterComponent = dynamic_cast<CComponent*>(InOuter))
				{
					UpdateConstructionComponents(InComponent, InOuterComponent);
				}
				//Outer��һ������
				else if (const GActorObject* InOuterActor = dynamic_cast<GActorObject*>(InOuter))
				{
					if (InOuterActor->GetRootComponent())
					{
						UpdateConstructionComponents(InComponent, InOuterActor->GetRootComponent());
					}
				}
			}
		}
	}
}