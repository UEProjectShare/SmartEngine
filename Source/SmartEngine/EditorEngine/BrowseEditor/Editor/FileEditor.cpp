#include "FileEditor.h"
#include "../Core/AssetEditorCommon.h"

FFileEditor::FFileEditor()
{
	UpdateFileTime = 1.f;
	CurrentFileTime = 0.f;
}

void FFileEditor::BuildEditor()
{
	ResetPaths();
}

void FFileEditor::DrawEditor(float DeltaTime)
{
	CurrentFileTime += DeltaTime;
	if (CurrentFileTime >= UpdateFileTime)
	{
		CurrentFileTime = 0.f;

		ResetPaths();
	}
	
	std::string SelectPath;
	if (ImGui::TreeNode("Content"))
	{
		SelectPath =
			FEnginePathHelper::RelativeToAbsolutePath(
				FEnginePathHelper::GetEngineContentPath()) + '/';

		for (int i = 0; i < FilePaths.size(); i++)
		{
			RecursionSpawnFileWidget(FilePaths[i]->Node, SelectPath);
		}

		ImGui::TreePop();
	}

	if (!SelectPath.empty())
	{
		for (int i = 0; i < FilePaths.size(); i++)
		{
			if (SelectPath == FilePaths[i]->FileDirectory)
			{
				CurrentSelectAssetPath = FilePaths[i];
				break;
			}
		}
	}
}

void FFileEditor::ExitEditor()
{

}

void FFileEditor::ResetPaths()
{
	FilePaths.clear();

	def_c_paths Paths;
	init_def_c_paths(&Paths);

	std::string ContentPath =
		FEnginePathHelper::RelativeToAbsolutePath(
			FEnginePathHelper::GetEngineContentPath());

	find_files(ContentPath.c_str(), &Paths, true,true);

	enum EResetFileType
	{
		RESETFILETYPE_NONE,
		RESETFILETYPE_ALLOW_FILE,//允许显示的文件
		RESETFILETYPE_PATH,//路径 文件夹
	};

	for (int i = 0; i < Paths.index; i++)
	{
#ifdef _WIN64
#else
		normalization_path(Paths.paths[i]);
#endif

		EResetFileType FileType = RESETFILETYPE_NONE;
		if (find_string(Paths.paths[i], ".smart", 0) != -1)
		{
			FileType = EResetFileType::RESETFILETYPE_ALLOW_FILE;
		}
		
		int EndPos = strlen(Paths.paths[i]) - sizeof(char);
		if (EndPos > 0)
		{
			if (Paths.paths[i][EndPos] == '/')
			{
				FileType = EResetFileType::RESETFILETYPE_PATH;
			}
		}
		else
		{
			continue;
		}

		if (FileType == EResetFileType::RESETFILETYPE_PATH ||
			FileType == RESETFILETYPE_ALLOW_FILE)
		{
			char Buff[1024] = { 0 };
			if (FileType == EResetFileType::RESETFILETYPE_ALLOW_FILE)
			{
				get_path_directory(Buff, Paths.paths[i]);
			}
			else
			{
				strcpy(Buff, Paths.paths[i]);
			}			

			bool bFind = false;
			for (auto& Tmp : FilePaths)
			{
				if (Tmp->FileDirectory == Buff)
				{
					if (FileType == EResetFileType::RESETFILETYPE_ALLOW_FILE)
					{
						Tmp->AssetPaths.push_back(Paths.paths[i]);
					}

					bFind = true;
					break;
				}
			}

			if (!bFind)
			{
				FilePaths.push_back(make_shared<FFileDirectory>());
				std::shared_ptr<FFileDirectory>& InDirectory = FilePaths[FilePaths.size() - 1];

				InDirectory->FileDirectory = Buff;
				if (FileType == EResetFileType::RESETFILETYPE_ALLOW_FILE)
				{
					InDirectory->AssetPaths.push_back(Paths.paths[i]);
				}

				//p:/adasd/asdsa/asdw/Content/xxx/aswe/asd/
				 //拆解路径
				char R[512] = { 0 };
				char L[512] = { 0 };
				split(Buff, "/Content/", L, R, false);

				///xxx/aswe/asd/

				/*
				xxx
				aswe
				asd
				*/
				vector<std::string> ArrayString;
				simple_cpp_string_algorithm::parse_into_vector_array(
					R, ArrayString, "/");

				std::shared_ptr<FFileNode>* FlieNode = &InDirectory->Node;
				for (int j = 0; j < ArrayString.size(); j++)
				{
					if (ArrayString[j].length() > 0)
					{
						*FlieNode = make_shared<FFileNode>();
						(*FlieNode)->FileContent = ArrayString[j];

						(*FlieNode)->Parent = InDirectory;//父类
						FlieNode = &(*FlieNode)->Next;
					}
				}
			}
		}
	}
}

void FFileEditor::RecursionSpawnFileWidget(const std::shared_ptr<FFileNode>& Node, std::string& OutPath)
{
	if (Node)
	{
		if (ImGui::TreeNode(Node->FileContent.c_str()))
		{
			OutPath = Node->Parent.lock()->FileDirectory;

			RecursionSpawnFileWidget(Node->Next, OutPath);

			ImGui::TreePop();
		}
	}
}