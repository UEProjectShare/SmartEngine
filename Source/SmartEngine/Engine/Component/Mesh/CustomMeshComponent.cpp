#include "CustomMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Core/Construction/MacroConstruction.h"

#if THIRD_PARTY_LIBRARY
#include "MeshImportExport.h"

#pragma comment(lib, "SmartMeshImportExportTool.lib")

#endif // THIRD_PARTY_LIBRARY

CCustomMeshComponent::CCustomMeshComponent()
{

}

void CCustomMeshComponent::CreateMesh(
	FMeshRenderingData& MeshData, 
	const string& InPath,
	const FIEParam& InParam)
{
	//C:/dd/dd/c/x.obj
	//x.obj
	char Buff[1024] = { 0 };
	get_path_clean_filename(Buff, InPath.c_str());

	if (find_string(Buff, ".obj", 0) != -1 ||
		find_string(Buff, ".OBJ", 0) != -1)
	{
		//拿到文件大小
		const unsigned int FileSize = get_file_size_by_filename(InPath.c_str());

		//根据文件大小创建buff
		char* Buff = new char[FileSize + 1];
		//必须要初始化
		memset(Buff, 0, FileSize + 1);

		//提取buff
		get_file_buf(InPath.c_str(), Buff);

		if (!LoadObjFromBuff(Buff, FileSize, MeshData))
		{

		}

		delete Buff;
	}
	else if (find_string(Buff, ".fbx", 0) != -1 ||
		find_string(Buff, ".FBX", 0) != -1)
	{
		char PathBuff[1024] = { 0 };
		get_full_path(PathBuff,1024, InPath.c_str());

		LoadFBXFromBuff(PathBuff, MeshData, InParam);
	}
}

bool CCustomMeshComponent::LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData)
{
	if (InBuffSize > 0)
	{
		MeshData.SectionDescribe.push_back(FMeshSection());
		FMeshSection& Section = MeshData.SectionDescribe[MeshData.SectionDescribe.size() - 1];

		stringstream BuffStream(InBuff);
		char TmpLine[256] = { 0 };
		string MidTmpTag;

		vector<XMFLOAT3> Position;
		vector<XMFLOAT3> Normal;
		vector<XMFLOAT3> UTangent;
		vector<XMFLOAT2> TexCoord;//纹理坐标

		for (; !BuffStream.eof();)
		{
			memset(TmpLine, 0, 256);

			//读取一行数据
			BuffStream.getline(TmpLine, 256);
			if (strlen(TmpLine) > 0)
			{
				if (TmpLine[0] == 'v')
				{
					stringstream LineStream(TmpLine);
					LineStream >> MidTmpTag;

					if (TmpLine[1] == 'n')
					{
						//拿到位置
						Normal.push_back(XMFLOAT3());
						XMFLOAT3& Float3InNormal = Normal[Normal.size() - 1];

						//解析了位置
						LineStream >> Float3InNormal.x;
						LineStream >> Float3InNormal.y;
						LineStream >> Float3InNormal.z;
					}
					else if (TmpLine[1] == 't')
					{	
						TexCoord.push_back(XMFLOAT2());
						XMFLOAT2& Float2InTexCoord = TexCoord[TexCoord.size() - 1];

						LineStream >> Float2InTexCoord.x;
						LineStream >> Float2InTexCoord.y;
					}
					else
					{
						//拿到位置
						Position.push_back(XMFLOAT3());
						XMFLOAT3& Float3InPos = Position[Position.size() - 1];

						//解析了位置
						LineStream >> Float3InPos.x;
						LineStream >> Float3InPos.y;
						LineStream >> Float3InPos.z;
					}
				}
				else if (TmpLine[0] == 'f')
				{
					stringstream LineStream(TmpLine);
					LineStream >> MidTmpTag;

					char SaveLineString[256] = { 0 };
					char TmpBuff[256] = { 0 };
					for (size_t i = 0; i < 3; i++)
					{
						memset(SaveLineString, 0, 256);

						//输入一行数据
						LineStream >> SaveLineString;

						//找到索引的位置
						int StringPosA = find_string(SaveLineString, "/", 0);
						memset(TmpBuff, 0, 256);
						char* VPosIndex = string_mid(SaveLineString, TmpBuff, 0, StringPosA);

						//放到索引容器里面
						MeshData.Data.IndexData.push_back(atoi(VPosIndex) - 1);

						//纹理Index
						int StringPosB = find_string(SaveLineString, "/", StringPosA + 1);
						memset(TmpBuff, 0, 256);
						char* TexcoordIndex = string_mid(SaveLineString, TmpBuff, StringPosA + 1, StringPosB - (StringPosA + 1));

						//法线index
						memset(TmpBuff, 0, 256);
						char* NormalIndex = string_mid(SaveLineString, TmpBuff, StringPosB + 1, strlen(SaveLineString) - (StringPosB + 1));
					}
				}
			}
		}

		MeshData.Data.VertexData.resize(static_cast<int>(Position.size()));
		for (size_t i = 0; i < Position.size(); i++)
		{
			MeshData.Data.VertexData[i].Position = Position[i];
			MeshData.Data.VertexData[i].Normal = Normal[i];
			MeshData.Data.VertexData[i].TexCoord = TexCoord[i];
			MeshData.Data.VertexData[i].Color = XMFLOAT4(Colors::White);

			//if (i > 1)
			//{
			//	XMFLOAT3 LastPos = Position[i - 1];
			//	XMFLOAT3 Pos = Position[i];
			//
			//	fvector_3d LastPos3D = EngineMath::ToVector3d(LastPos);
			//	fvector_3d PosVector3D = EngineMath::ToVector3d(Pos);
			//
			//	fvector_3d NewDir = LastPos3D - PosVector3D;
			//	MeshData.VertexData[i].UTangent = XMFLOAT3(NewDir.x, NewDir.y, NewDir.z);
			//}
			//else
			//{
			//	MeshData.VertexData[i].UTangent = XMFLOAT3(0.f, 1.f, 0.f);
			//}
		}

		Section.IndexSize = MeshData.Data.IndexData.size();
		Section.VertexSize = MeshData.Data.VertexData.size();

		SpawnDefaultMaterial();

		return true;
	}

	return false;
}

void CCustomMeshComponent::BuildKey(size_t& OutHashKey, const std::string& InPath, const FIEParam& InParam)
{
	std::hash<string> FloatHash;
	
	OutHashKey = 3;
	OutHashKey += FloatHash(InPath);
}

bool CCustomMeshComponent::LoadFBXFromBuff(const string& InPath, FMeshRenderingData& MeshData,const FIEParam& InParam)
{
#if THIRD_PARTY_LIBRARY
	FIERenderData RenderData;
	AssetImport::LoadMeshData(InPath.c_str(), EIEMeshType::IE_FBX, RenderData,InParam);

	for (auto &TmpModel: RenderData.ModelData)
	{
		auto FindSection = [&](int InMatrealIndex)->FMeshSection*
		{
			for (auto& Tmp : MeshData.SectionDescribe)
			{
				if (Tmp.MaterialIndex == InMatrealIndex)
				{
					return &Tmp;
				}
			}

			MeshData.SectionDescribe.push_back(FMeshSection());
			FMeshSection& Section = MeshData.SectionDescribe[MeshData.SectionDescribe.size() - 1];
			
			Section.MaterialIndex = InMatrealIndex;

			return &Section;
		};

		//模型处理
		for (auto& MeshTmp: TmpModel.MeshData)
		{
			for (auto &VertexTmp: MeshTmp.VertexData)
			{
				//三角形赋值
				for (int i = 0 ;i < 3; i++)
				{
					 MeshData.Data.VertexData.push_back(FVertex());
					 FVertex& InVertex = MeshData.Data.VertexData[MeshData.Data.VertexData.size() - 1];

					 InVertex.Position.x = VertexTmp.Vertexs[i].Position.x;
					 InVertex.Position.y = VertexTmp.Vertexs[i].Position.y;
					 InVertex.Position.z = VertexTmp.Vertexs[i].Position.z;

					 InVertex.Normal.x = VertexTmp.Vertexs[i].Normal.x;
					 InVertex.Normal.y = VertexTmp.Vertexs[i].Normal.y;
					 InVertex.Normal.z = VertexTmp.Vertexs[i].Normal.z;

					 InVertex.UTangent.x = VertexTmp.Vertexs[i].Tangent.x;
					 InVertex.UTangent.y = VertexTmp.Vertexs[i].Tangent.y;
					 InVertex.UTangent.z = VertexTmp.Vertexs[i].Tangent.z;

					 InVertex.TexCoord.x = VertexTmp.Vertexs[i].UV.x;
					 InVertex.TexCoord.y = VertexTmp.Vertexs[i].UV.y;
				}

				//Section
				if (FMeshSection* Section = FindSection(VertexTmp.MaterialID))
				{
					Section->IndexSize += 3;
					Section->VertexSize += 3;
				}
			}

			//拷贝
			MeshData.Data.IndexData.insert(
				MeshData.Data.IndexData.end(),
				MeshTmp.IndexData.begin(),
				MeshTmp.IndexData.end());
		}

		//多材质
		BUILD_OBJECT_PARAMETERS_BY_COMPONENT(, this);
		if (vector<CMaterial*>* InMaterials = GetMaterials())
		{
			for (auto& MaterialTmp : TmpModel.MaterialMap)
			{
				InMaterials->push_back(CreateObject<CMaterial>(Param, new CMaterial()));
				if (CMaterial* InMaterial = (*InMaterials)[InMaterials->size() - 1])
				{
					InMaterial->Rename(MaterialTmp.second.Name);

					auto& Diffuse = MaterialTmp.second.DiffuseColor;
					InMaterial->SetBaseColor(fvector_4d(Diffuse.Buffer()[0], Diffuse.Buffer()[1], Diffuse.Buffer()[2], 1.f));
				
					auto& Specular = MaterialTmp.second.Specular;
					InMaterial->SetSpecular(fvector_3d(static_cast<float>(Specular.Buffer()[0]), static_cast<float>(Specular.Buffer()[1]), static_cast<float>(Specular.Buffer()[2])));

					auto& Reflectivity = MaterialTmp.second.Reflectivity;
					InMaterial->SetRefractiveValue(*reinterpret_cast<float*>(Reflectivity.Buffer()));
				
					//shader设置
					if (MaterialTmp.second.ShadingModel == "Lambert")
					{
						InMaterial->SetMaterialType(EMaterialType::Lambert);
					}
					else if (MaterialTmp.second.ShadingModel == "Phong")
					{
						InMaterial->SetMaterialType(EMaterialType::Phong);
					}

					if (!MaterialTmp.second.DiffuseColor.TextureFilename.empty())
					{


						InMaterial->SetBaseColor(MaterialTmp.second.DiffuseColor.TextureFilename);
					}
				}
			}
		}
	}
#endif
	return true;
}