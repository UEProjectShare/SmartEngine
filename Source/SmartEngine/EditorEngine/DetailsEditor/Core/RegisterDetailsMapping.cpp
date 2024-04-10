#include "RegisterDetailsMapping.h"

map<std::string, shared_ptr<FClassDetailsMapping>> FRegisterDetailsMapping::ClassDetailsMappings;
map<std::string, shared_ptr<FPropertyDetailsMapping>> FRegisterDetailsMapping::PropertyDetailsMappings;

void FRegisterDetailsMapping::RegisterClassDetails(const std::string& InClassName, shared_ptr<FClassDetailsMapping> InMapping)
{
	if (InMapping && !InClassName.empty())
	{
		ClassDetailsMappings.insert(std::pair<string, shared_ptr<FClassDetailsMapping>>(InClassName, InMapping));
	}
}

void FRegisterDetailsMapping::RegisterPropertyDetails(const std::string& InPropertyName, shared_ptr<FPropertyDetailsMapping> InMapping)
{
	if (InMapping && !InPropertyName.empty())
	{
		PropertyDetailsMappings.insert(std::pair<string, shared_ptr<FPropertyDetailsMapping>>(InPropertyName, InMapping));
	}
}

shared_ptr<FClassDetailsMapping> FRegisterDetailsMapping::FindClass(const std::string& InClassName)
{
	const auto iterator = ClassDetailsMappings.find(InClassName);
	if (iterator != ClassDetailsMappings.end())
	{
		return iterator->second;
	}

	return nullptr;
}

shared_ptr<FPropertyDetailsMapping> FRegisterDetailsMapping::FindProperty(const std::string& InPropertyName)
{
	const auto iterator = PropertyDetailsMappings.find(InPropertyName);
	if (iterator != PropertyDetailsMappings.end())
	{
		return iterator->second;
	}

	return nullptr;
}

bool FRegisterDetailsMapping::UpdateClassWidget(CCoreMinimalObject* InObject)
{
	if (InObject)
	{
		for (auto& Tmp : InObject->GetNativeClass().InheritClassTypes)
		{
			//需要考虑继承关系 映射继承的内容
			if (const shared_ptr<FClassDetailsMapping> ClassDetailsMapping = FindClass(Tmp))
			{
				ClassDetailsMapping->UpdateDetailsWidget(InObject);
			}
		}
	}

	return false;
}

bool FRegisterDetailsMapping::UpdatePropertyWidget(CPropertyObject* InProperty)
{
	if (InProperty)
	{
		std::map<std::string, std::vector<CPropertyObject*>> Categories;
		auto AddCategory = [&](CPropertyObject* InNewProperty, const std::string& InCategoryName)
		{
			const auto CategoriesIt = Categories.find(InCategoryName);
			if (CategoriesIt != Categories.end())
			{
				CategoriesIt->second.push_back(InNewProperty);
			}
			else
			{
				Categories.insert({InCategoryName, std::vector<CPropertyObject*>()});

				Categories[InCategoryName].push_back(InNewProperty);
			}
		};

		//收集我们的类别
		while (InProperty)
		{
			auto MateValue = InProperty->GetNativeClass().Metas.find("Category");
			if (MateValue != InProperty->GetNativeClass().Metas.end())
			{
				AddCategory(InProperty, MateValue->second);
			}

			InProperty = dynamic_cast<CPropertyObject*>(InProperty->Next);
		}

		ImGui::Separator();
		for (auto& Tmp : Categories)
		{		
			if (ImGui::CollapsingHeader(Tmp.first.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (const auto& SubTmp : Tmp.second)
				{
					if (const auto InDetailsProperty = FindProperty(SubTmp->GetType().c_str()))
					{
						if (InDetailsProperty->UpdateDetailsWidget(SubTmp))
						{
							if (CCoreMinimalObject* InObject = dynamic_cast<CCoreMinimalObject*>(SubTmp->GetOuter()))
							{
								InObject->UpdateEditorPropertyDetails(SubTmp);
							}
						}
					}
				}
			}		
		}
		ImGui::Separator();

		return true;
	}
	
	return false;
}