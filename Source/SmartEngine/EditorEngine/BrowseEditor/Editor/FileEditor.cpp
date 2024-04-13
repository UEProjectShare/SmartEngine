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

	const std::string ContentPath =
		FEnginePathHelper::RelativeToAbsolutePath(
			FEnginePathHelper::GetEngineContentPath());

	find_files(ContentPath.c_str(), &Paths, true);

	for (int i = 0; i < Paths.index; i++)
	{
		normalization_path(Paths.paths[i]);

		char Buff[1024] = { 0 };
		get_path_directory(Buff, Paths.paths[i]);

		bool bFind = false;
		for (const auto& Tmp : FilePaths)
		{
			if (Tmp->FileDirectory == Buff)
			{
				Tmp->AssetPaths.push_back(Paths.paths[i]);

				bFind = true;
				break;
			}
		}

		if (!bFind)
		{
			FilePaths.push_back(make_shared<FFileDirectory>());
			const std::shared_ptr<FFileDirectory>& InDirectory = FilePaths[FilePaths.size() - 1];

			InDirectory->FileDirectory = Buff;
			InDirectory->AssetPaths.push_back(Paths.paths[i]);

			//p:/adasd/asdsa/asdw/Content/xxx/aswe/asd/
			 //²ð½âÂ·¾¶
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

			std::shared_ptr<FFileNode>* FileNode = &InDirectory->Node;
			for (int j = 0; j < ArrayString.size(); j++)
			{
				if (ArrayString[j].length() > 0)
				{
					*FileNode = make_shared<FFileNode>();
					(*FileNode)->FileContent = ArrayString[j];

					(*FileNode)->Parent = InDirectory;//¸¸Àà
					FileNode = &(*FileNode)->Next;
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