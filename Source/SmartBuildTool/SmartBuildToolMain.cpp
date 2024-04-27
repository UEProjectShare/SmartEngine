#include "SmartBuildTool.h"
#include "CodeReflection/CollectClassInfoFun/CollectClassInfo.h"
#include "CodeReflection/IntermediateFile/SpawnIntermediateFile.h"

int main()
{
	std::cout << "Start Generate Reflection Code" << endl;

	if (0)
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
	}
	else
	{
		//生成反射代码的位置
		const string CodeReflectionPath = FEnginePathHelper::RelativeToAbsolutePath(FEnginePathHelper::GetEngineCodeReflectionPath());
		//要遍历的源文件位置
		const string SourcePath = FEnginePathHelper::RelativeToAbsolutePath(FEnginePathHelper::GetEngineSourcePath());

		//先移除所有反射代码
		remove_dir_all_files(CodeReflectionPath.c_str());

		def_c_paths Paths;
		init_def_c_paths(&Paths);

		find_files(SourcePath.c_str(), &Paths, true, false);

		//先收集它的模块
		map<string, vector<string>> Modulars;
		for (int i = 0; i < Paths.index; i++)
		{
			if (find_string(Paths.paths[i], ".vcxproj", 0) != -1)
			{
				//单位化路径
				normalization_path(Paths.paths[i]);

				char Buff[1024] = { 0 };
				get_path_directory(Buff, Paths.paths[i]);

				Modulars.insert(make_pair(Buff, vector<string>()));
			}
		}

		for (int i = 0; i < Paths.index; i++)
		{
			if (find_string(Paths.paths[i], ".h", 0) != -1)
			{
				//单位化路径
				normalization_path(Paths.paths[i]);

				std::vector<std::string> StringArray;
				simple_cpp_helper_file::load_file_to_strings(Paths.paths[i], StringArray);
			
				if (IsCheckAllowCodeReflection(StringArray))
				{
					std::cout << "Generating Reflection Code:" << Paths.paths[i] << endl;
					
					//收集类型
					FClassAnalysis ClassAnalysis;
					
					vector<string>* PathReflectionCodeCpp = nullptr;
					for (auto& Tmp : Modulars)
					{
						//E:\RenZhaiEngine\Source\RenZhaiBuildTool\CodeReflection\xx.j
						if (find_string(Paths.paths[i], Tmp.first.c_str(),0) != -1)
						{
							ClassAnalysis.ModularPath = Tmp.first;
							PathReflectionCodeCpp = &Tmp.second;
							break;
						}
					}
					CollectClassInfo::Collection(Paths.paths[i], ClassAnalysis);

					//构建.h和.cpp代码
					std::vector<std::string> OutAnalysisRawH;
					std::vector<std::string> OutAnalysisRawCPP;
					IntermediateFile::Builder(ClassAnalysis, OutAnalysisRawH, OutAnalysisRawCPP);
				
					//C://dasd//asd//12.h
					char Buff[1024] = { 0 };
					get_path_clean_filename(Buff, Paths.paths[i]);

					remove_char_end(Buff, 'h');
					remove_char_end(Buff, '.');

					string PathH = CodeReflectionPath + "/" + Buff + ".CodeReflection.h";
					string PathCPP = CodeReflectionPath + "/" + Buff + ".CodeReflection.cpp";

					simple_cpp_helper_file::save_file_to_strings(PathH, OutAnalysisRawH);
					simple_cpp_helper_file::save_file_to_strings(PathCPP, OutAnalysisRawCPP);
					
					if (PathReflectionCodeCpp)
					{
						//绝对路径
						if (1)
						{
							PathReflectionCodeCpp->push_back(
								simple_cpp_string_algorithm::printf(
									"#include \"%s\"", PathCPP.c_str()));
						}
						else //相对路径
						{
							PathReflectionCodeCpp->push_back(
								simple_cpp_string_algorithm::printf(
									"#include \"%s.CodeReflection.cpp\"", Buff));
						}
					}
				}
			}
		}

		//链接
		for (auto &Tmp : Modulars)
		{
			if (Tmp.second.size() > 0)
			{
				string LinkCodeReflection = simple_cpp_string_algorithm::printf(
				"%sLinkCodeReflection.build.cpp", Tmp.first.c_str());

				simple_cpp_helper_file::save_file_to_strings(LinkCodeReflection, Tmp.second);
			}
		}
	}

	std::cout << "End Generate Reflection Code" << endl;

	return 0;
}