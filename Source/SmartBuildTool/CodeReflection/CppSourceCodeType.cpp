#include "CppSourceCodeType.h"

bool IsCheckAllowCodeReflection(const std::vector<std::string>& InContent)
{
	int Count = 0;
	for (auto& Tmp : InContent)
	{
		if (simple_cpp_string_algorithm::string_contain(Tmp, "CODEREFLECTION") ||
			simple_cpp_string_algorithm::string_contain(Tmp, ".CodeReflection.h"))
		{
			Count++;
		}
	}

	return Count >= 2;
}
