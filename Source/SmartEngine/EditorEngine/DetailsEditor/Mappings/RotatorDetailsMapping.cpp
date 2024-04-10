#include "RotatorDetailsMapping.h"
#include "../Core/ConstructDetailsWidget.h"

bool FRotatorDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return ConstructDetailsWidget::ConstructFloat3Widget<XMFLOAT3>(InProperty);
}

shared_ptr<FPropertyDetailsMapping> FRotatorDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FRotatorDetailsMapping>();
}
