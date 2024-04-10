#include "IntDetailsMapping.h"
#include "../Core/ConstructDetailsWidget.h"

bool FIntDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return ConstructDetailsWidget::ConstructIntWidget<int>(InProperty);
}

shared_ptr<FPropertyDetailsMapping> FIntDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FIntDetailsMapping>();
}
