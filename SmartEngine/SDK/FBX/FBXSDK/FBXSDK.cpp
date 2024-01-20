#include "include/fbxsdk.h"
#include "FBXSDK.h"

#pragma comment(lib, "libfbxsdk.lib")
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libfbxsdk-mt.lib")

void InitializeSdkObjects(FbxManager*& InManager, FbxScene*& InScene)
{
	InManager = FbxManager::Create();

	FbxIOSettings* FBXIO = FbxIOSettings::Create(InManager, IOSROOT);
	InManager->SetIOSettings(FBXIO);

	const FbxString FBXPath = FbxGetApplicationDirectory();
	InManager->LoadPluginsDirectory(FBXPath);

	InScene = FbxScene::Create(InManager, "OK My");
}

bool LoadScene(FbxManager* InManager, FbxDocument* InScene, const char* InFilename)
{
	FFBXVersion SDKVersion;
	FbxManager::GetFileFormatVersion(
		SDKVersion.Major,
		SDKVersion.Minor,
		SDKVersion.Revision);

	FbxImporter* FBXImporterPtr = FbxImporter::Create(InManager, "");

	bool bReturn = FBXImporterPtr->Initialize(
		InFilename,
		-1,
		InManager->GetIOSettings());

	FFBXVersion FileVersion;
	FBXImporterPtr->GetFileVersion(
		FileVersion.Major,
		FileVersion.Minor,
		FileVersion.Revision);

	if (!bReturn)
	{
		return false;
	}

	if (FBXImporterPtr->IsFBX())
	{
		//打印数据
	}

	bReturn = FBXImporterPtr->Import(InScene);
	if (bReturn &&
		FBXImporterPtr->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		//密码
	}

	//删除
	FBXImporterPtr->Destroy();

	return true;
}

void GetPolygons(FbxMesh* InMesh, FFBXMesh& OutData)
{
	const int PolygonCount = InMesh->GetPolygonCount();
	const FbxVector4* ControlPoints = InMesh->GetControlPoints();

	int VertexID = 0;
	for (int i = 0; i < PolygonCount; i++)//图元
	{
		OutData.VertexData.push_back(FFBXTriangle());
		FFBXTriangle& InTriangle = OutData.VertexData[OutData.VertexData.size() - 1];

		const int PolygonSize = InMesh->GetPolygonSize(i);
		for (int j = 0; j < PolygonSize; j++)
		{
			const int ControlPointIndex = InMesh->GetPolygonVertex(i, j);

			//Coordinates
			//拿到位置
			//FbxVector4 Coordinates;
			//获取位置
			{
				//考虑缩放
				FbxDouble3 Scaling = InMesh->GetNode()->LclScaling;

				InTriangle.Vertexs[j].Position.X = ControlPoints[ControlPointIndex].mData[0] * Scaling[0];
				InTriangle.Vertexs[j].Position.Y = ControlPoints[ControlPointIndex].mData[2] * Scaling[2];
				InTriangle.Vertexs[j].Position.Z = -ControlPoints[ControlPointIndex].mData[1] * Scaling[1];
			}

			//顶点颜色
			for (int l = 0; l < InMesh->GetElementVertexColorCount(); l++) {}

			//UV
			for (int l = 0; l < InMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* TextureUV = InMesh->GetElementUV(l);
				auto ModeType = TextureUV->GetMappingMode();

				auto ReferenceMode = TextureUV->GetReferenceMode();
				if (ModeType == fbxsdk::FbxLayerElement::eByControlPoint)
				{
					if (ReferenceMode == fbxsdk::FbxLayerElement::eDirect)
					{
						const FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ControlPointIndex);

						InTriangle.Vertexs[j].UV.X = UV.mData[0];
						InTriangle.Vertexs[j].UV.Y = 1.f - UV.mData[1];//V是反的 DX 和 OpenGL不一样
					}
					else if (ReferenceMode == fbxsdk::FbxLayerElement::eIndexToDirect)
					{
						const int ID = TextureUV->GetIndexArray().GetAt(ControlPointIndex);

						const FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].UV.X = UV.mData[0];
						InTriangle.Vertexs[j].UV.Y = 1.f - UV.mData[1];//V是反的 DX 和 OpenGL不一样
					}
				}
				else if (ModeType == fbxsdk::FbxLayerElement::eByPolygonVertex)
				{
					const int TextureUVIndex = InMesh->GetTextureUVIndex(i, j);
					FbxVector2 UV = TextureUV->GetDirectArray().GetAt(TextureUVIndex);
					switch (ReferenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ControlPointIndex);

						InTriangle.Vertexs[j].UV.X = UV.mData[0];
						InTriangle.Vertexs[j].UV.Y = 1.f - UV.mData[1];//V是反的 DX 和 OpenGL不一样
						break;
					}
					}
				}
			}

			//法线
			for (int l = 0; l < InMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* Normal = InMesh->GetElementNormal(l);
				const auto NormalReferenceMode = Normal->GetReferenceMode();

				if (Normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (NormalReferenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						const FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(VertexID);

						InTriangle.Vertexs[j].Normal.X = NormalPoint.mData[0];
						InTriangle.Vertexs[j].Normal.Y = NormalPoint.mData[2];
						InTriangle.Vertexs[j].Normal.Z = NormalPoint.mData[1];

						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						const int ID = Normal->GetIndexArray().GetAt(VertexID);

						const FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].Normal.X = NormalPoint.mData[0];
						InTriangle.Vertexs[j].Normal.Y = NormalPoint.mData[2];
						InTriangle.Vertexs[j].Normal.Z = NormalPoint.mData[1];
					}
					}
				}
				else if (Normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					switch (NormalReferenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						const FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(ControlPointIndex);
						InTriangle.Vertexs[j].Normal.X = NormalPoint.mData[0];
						InTriangle.Vertexs[j].Normal.Y = NormalPoint.mData[2];
						InTriangle.Vertexs[j].Normal.Z = NormalPoint.mData[1];
						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						const int ID = Normal->GetIndexArray().GetAt(ControlPointIndex);
						const FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].Normal.X = NormalPoint.mData[0];
						InTriangle.Vertexs[j].Normal.Y = NormalPoint.mData[2];
						InTriangle.Vertexs[j].Normal.Z = NormalPoint.mData[1];
						break;
					}
					}
				}
			}

			//切线 T
			for (int l = 0; l < InMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* Tangent = InMesh->GetElementTangent(l);

				if (Tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (Tangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						const FbxVector4 TangentValue = Tangent->GetDirectArray().GetAt(VertexID);

						InTriangle.Vertexs[j].Tangent.X = TangentValue.mData[0];
						InTriangle.Vertexs[j].Tangent.Y = TangentValue.mData[2];
						InTriangle.Vertexs[j].Tangent.Z = TangentValue.mData[1];
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int ID = Tangent->GetIndexArray().GetAt(VertexID);

						const FbxVector4 TangentValue = Tangent->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].Tangent.X = TangentValue.mData[0];
						InTriangle.Vertexs[j].Tangent.Y = TangentValue.mData[2];
						InTriangle.Vertexs[j].Tangent.Z = TangentValue.mData[1];
						break;
					}
					}
				}
			}

			//B
			for (int l = 0; l < InMesh->GetElementBinormalCount(); ++l)
			{
				FbxGeometryElementBinormal* Binormal = InMesh->GetElementBinormal(l);

				if (Binormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (Binormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						Binormal->GetDirectArray().GetAt(VertexID);
						break;
					}
					}
				}
			}

			VertexID++;
		}
	}
}

void GetMaterial() {}

void GetIndex(FFBXMesh& InMesh)
{
	const uint16_t VertexDataSize = InMesh.VertexData.size() * 3.f;
	InMesh.IndexData.resize(InMesh.VertexData.size() * 3.f);

	for (uint16_t i = 0; i < VertexDataSize; i++)
	{
		InMesh.IndexData[i] = i;
	}
}

void GetMesh(FbxNode* InNode, FFBXModel& InModel)
{
	FbxMesh* NodeMesh = static_cast<FbxMesh*>(InNode->GetNodeAttribute());

	InModel.MeshData.push_back(FFBXMesh());
	FFBXMesh& InMesh = InModel.MeshData[InModel.MeshData.size() - 1];

	GetPolygons(NodeMesh, InMesh);

	GetIndex(InMesh);

	//GetMaterial();
}

void RecursiveLoadMesh(FbxNode* InNode, FFBXRenderData& OutData)
{
	//XML
	if (InNode->GetNodeAttribute() == nullptr)
	{
		//NULL Node
	}
	else
	{
		FbxNodeAttribute::EType AttributeType = (InNode->GetNodeAttribute()->GetAttributeType());

		if (AttributeType == fbxsdk::FbxNodeAttribute::eMesh)
		{
			OutData.ModelData.push_back(FFBXModel());
			FFBXModel& InModel = OutData.ModelData[OutData.ModelData.size() - 1];
			GetMesh(InNode, InModel);
		}
		else if (AttributeType == fbxsdk::FbxNodeAttribute::eSkeleton)
		{

		}
	}

	for (int i = 0; i < InNode->GetChildCount(); i++)
	{
		RecursiveLoadMesh(InNode->GetChild(i), OutData);
	}
}

void DestroySdkObjects(FbxManager* InManager)
{
	if (InManager)
	{
		InManager->Destroy();
	}
}

void FFBXAssetImport::LoadMeshData(const char* InPath, FFBXRenderData& OutData)
{
	//创建基础管理和场景
	FbxManager* SdkManager = nullptr;
	FbxScene* Scene = nullptr;

	//初始化场景对象
	InitializeSdkObjects(SdkManager, Scene);

	//读取FBX模型
	FbxString FBXPath(InPath);
	bool bResult = LoadScene(SdkManager, Scene, FBXPath.Buffer());

	//FbxAxisSystem::DirectX.ConvertScene(Scene);

	//XML 解析我们的FBX
	if (FbxNode* Node = Scene->GetRootNode())
	{
		for (int i = 0; i < Node->GetChildCount(); i++)
		{
			RecursiveLoadMesh(Node->GetChild(i), OutData);
		}
	}

	//干掉
	DestroySdkObjects(SdkManager);
}

//int main()
//{
//	FFBXRenderData Out;
//	std::string FBXPath = "C:/SmartEngine/SmartEngine/AssetHeart.fbx";
//
//	FFBXAssetImport().LoadMeshData(FBXPath.data(), Out);
//
//	return 0;
//}
