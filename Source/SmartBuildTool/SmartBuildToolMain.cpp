#include "SmartBuildTool.h"
#include "CodeReflection/CollectClassInfoFun/CollectClassInfo.h"
#include "CodeReflection/IntermediateFile/SpawnIntermediateFile.h"

int main()
{
	char Path[] = "../SmartBuildTool/TestObject.h";
	constexpr char PathH[] = "../SmartBuildTool/TestObject.G.h";
	constexpr char PathCPP[] = "../SmartBuildTool/TestObject.G.cpp";
	normalization_path(Path);
	FClassAnalysis ClassAnalysis;
	CollectClassInfo::Collection(Path, ClassAnalysis);

	std::vector<std::string> OutAnalysisRawH;
	std::vector<std::string> OutAnalysisRawCPP;
	IntermediateFile::Builder(ClassAnalysis, OutAnalysisRawH, OutAnalysisRawCPP);

	simple_cpp_helper_file::save_file_to_strings(PathH, OutAnalysisRawH);
	simple_cpp_helper_file::save_file_to_strings(PathCPP, OutAnalysisRawCPP);
	return 0;
}