#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FContainerDetailsMapping : public FPropertyDetailsMapping
{
public:
	FContainerDetailsMapping();

	bool UpdateDetailsWidget(CPropertyObject* InProperty) override;
	
	virtual	void UpdateWidget(CPropertyObject* InProperty);

	virtual	void UpdateEditorContainerProperty(CPropertyObject* InProperty);
protected:
	std::string ContainerName;
};