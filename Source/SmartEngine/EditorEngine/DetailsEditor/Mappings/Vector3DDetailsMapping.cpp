#include "Vector3DDetailsMapping.h"
#include "../Core/ConstructDetailsWidget.h"

bool FVector3DDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return ConstructDetailsWidget::ConstructFloat3Widget<fvector_3d>(InProperty);
}

shared_ptr<FPropertyDetailsMapping> FVector3DDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FVector3DDetailsMapping>();
}
