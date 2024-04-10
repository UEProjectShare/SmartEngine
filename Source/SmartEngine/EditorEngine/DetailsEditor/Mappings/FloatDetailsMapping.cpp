#include "FloatDetailsMapping.h"
#include "../Core/ConstructDetailsWidget.h"

bool FFloatDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return ConstructDetailsWidget::ConstructFloatWidget<float>(InProperty);
}

shared_ptr<FPropertyDetailsMapping> FFloatDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FFloatDetailsMapping>();
}
