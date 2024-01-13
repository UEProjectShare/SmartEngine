#include "CustomMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"
#if THIRD_PARTY_LIBRARY
#include "../../../SDK/FBX/FBXSDK/FBXSDK.h"

#pragma comment(lib, "FBXSDK.lib")

#endif // THIRD_PARTY_LIBRARY

CCustomMeshComponent::CCustomMeshComponent()
{

}

void CCustomMeshComponent::CreateMesh(FMeshRenderingData& MeshData, const string& InPath)
{
	//C:/dd/dd/c/x.obj
	//x.obj
	char Buff[1024] = { 0 };
	get_path_clean_filename(Buff, InPath.c_str());

	if (find_string(Buff, ".obj", 0) != -1 ||
		find_string(Buff, ".OBJ", 0) != -1)
	{
		//�õ��ļ���С
		const unsigned int FileSize = get_file_size_by_filename(InPath.c_str());

		//�����ļ���С����buff
		char* Buff = new char[FileSize + 1];
		//����Ҫ��ʼ��
		memset(Buff, 0, FileSize + 1);

		//��ȡbuff
		get_file_buf(InPath.c_str(), Buff);

		if (!LoadObjFromBuff(Buff, FileSize, MeshData))
		{

		}

		delete Buff;
	}
	else if (find_string(Buff, ".fbx", 0) != -1 ||
		find_string(Buff, ".FBX", 0) != -1)
	{
		LoadFBXFromBuff(InPath, MeshData);
	}
}

bool CCustomMeshComponent::LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData)
{
	if (InBuffSize > 0)
	{
		stringstream BuffStream(InBuff);
		char TmpLine[256] = { 0 };
		string MidTmpTag;

		vector<XMFLOAT3> Position;
		vector<XMFLOAT3> Normal;
		vector<XMFLOAT3> UTangent;
		vector<XMFLOAT2> TexCoord;//��������

		for (; !BuffStream.eof();)
		{
			memset(TmpLine, 0, 256);

			//��ȡһ������
			BuffStream.getline(TmpLine, 256);
			if (strlen(TmpLine) > 0)
			{
				if (TmpLine[0] == 'v')
				{
					stringstream LineStream(TmpLine);
					LineStream >> MidTmpTag;

					if (TmpLine[1] == 'n')
					{
						//�õ�λ��
						Normal.push_back(XMFLOAT3());
						XMFLOAT3& Float3InNormal = Normal[Normal.size() - 1];

						//������λ��
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
						//�õ�λ��
						Position.push_back(XMFLOAT3());
						XMFLOAT3& Float3InPos = Position[Position.size() - 1];

						//������λ��
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

						//����һ������
						LineStream >> SaveLineString;

						//�ҵ�������λ��
						int StringPosA = find_string(SaveLineString, "/", 0);
						memset(TmpBuff, 0, 256);
						char* VPosIndex = string_mid(SaveLineString, TmpBuff, 0, StringPosA);

						//�ŵ�������������
						MeshData.IndexData.push_back(atoi(VPosIndex) - 1);

						//����Index
						int StringPosB = find_string(SaveLineString, "/", StringPosA + 1);
						memset(TmpBuff, 0, 256);
						char* TexcoordIndex = string_mid(SaveLineString, TmpBuff, StringPosA + 1, StringPosB - (StringPosA + 1));

						//����index
						memset(TmpBuff, 0, 256);
						char* NormalIndex = string_mid(SaveLineString, TmpBuff, StringPosB + 1, strlen(SaveLineString) - (StringPosB + 1));
					}
				}
			}
		}

		MeshData.VertexData.resize(static_cast<int>(Position.size()));
		for (size_t i = 0; i < Position.size(); i++)
		{
			MeshData.VertexData[i].Position = Position[i];
			MeshData.VertexData[i].Normal = Normal[i];
			MeshData.VertexData[i].TexCoord = TexCoord[i];
			MeshData.VertexData[i].Color = XMFLOAT4(Colors::White);

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

		return true;
	}

	return false;
}

bool CCustomMeshComponent::LoadFBXFromBuff(const string& InPath, FMeshRenderingData& MeshData)
{
#if THIRD_PARTY_LIBRARY
	FFBXRenderData RenderData;
	FFBXAssetImport().LoadMeshData(InPath.c_str(), RenderData);

	for (auto &TmpModel: RenderData.ModelData)
	{
		for (auto & MeshTmp: TmpModel.MeshData)
		{
			for (auto &VertexTmp: MeshTmp.VertexData)
			{
				for (int i =0 ;i < 3;i++)
				{
					MeshData.VertexData.push_back(FVertex());
					FVertex& InVertex = MeshData.VertexData[MeshData.VertexData.size() - 1];

					InVertex.Position.x = VertexTmp.Vertexs[i].Position.X;
					InVertex.Position.y = VertexTmp.Vertexs[i].Position.Y;
					InVertex.Position.z = VertexTmp.Vertexs[i].Position.Z;

					InVertex.Normal.x = VertexTmp.Vertexs[i].Normal.X;
					InVertex.Normal.y = VertexTmp.Vertexs[i].Normal.Y;
					InVertex.Normal.z = VertexTmp.Vertexs[i].Normal.Z;

					InVertex.UTangent.x = VertexTmp.Vertexs[i].Tangent.X;
					InVertex.UTangent.y = VertexTmp.Vertexs[i].Tangent.Y;
					InVertex.UTangent.z = VertexTmp.Vertexs[i].Tangent.Z;

					InVertex.TexCoord.x = VertexTmp.Vertexs[i].UV.X;
					InVertex.TexCoord.y = VertexTmp.Vertexs[i].UV.Y;
				}				
			}

			//����
			MeshData.IndexData = MeshTmp.IndexData;
		}
	}
#endif
	return true;
}

void CCustomMeshComponent::BuildKey(size_t& OutHashKey, const std::string& InPath)
{
	const std::hash<string> FloatHash;
	
	OutHashKey = 3;
	OutHashKey += FloatHash(InPath);
}
