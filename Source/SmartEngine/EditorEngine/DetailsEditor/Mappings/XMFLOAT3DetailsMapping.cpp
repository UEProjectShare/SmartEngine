#include "XMFLOAT3DetailsMapping.h"
#include "../Core/ConstructDetailsWidget.h"

bool FXMFLOAT3DetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return ConstructDetailsWidget::ConstructFloat3Widget<XMFLOAT3>(InProperty);
}

shared_ptr<FPropertyDetailsMapping> FXMFLOAT3DetailsMapping::MakeDetailsMapping()
{
	return make_shared<FXMFLOAT3DetailsMapping>();
}
