#include "ThirdPartyLibrary/FBX/include/fbxsdk.h"
#include "FBXAssist.h"

#pragma comment(lib, "libfbxsdk.lib")
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libfbxsdk-mt.lib")
//#pragma comment(lib, "libxml2-md.lib")
//#pragma comment(lib, "libxml2-mt.lib")
//#pragma comment(lib, "zlib-md.lib")
//#pragma comment(lib, "zlib-mt.lib")

//FBX基础API
void InitializeSdkObjects(class FbxManager*& InManager, FbxScene*& InScene);
bool LoadScene(FbxManager* InManager, FbxDocument* InScene, const char* InFilename);
void DestroySdkObjects(FbxManager* InManager);

//和模型相关
void RecursiveLoadMesh(FbxNode* InNode, FIERenderData& OutData, const FIEParam& InParam);
void GetMesh(FbxNode* InNode, FIEModel& InModel, const FIEParam& InParam);
void GetPolygons(FbxMesh* InMesh, FIEMesh& OutData);

//材质相关
void LinkMaterial(const FbxGeometry* InGeometry, std::map<int, FIEMaterial>& OutMap);
void LinkMaterialTexture(const FbxGeometry* InGeometry,std::map<int, FIEMaterial>& OutMap);
void MaterialMappings(FbxMesh* InGeometry, FIEMesh& OutMesh);

/// <summary>
/// 具体实现
/// </summary>
/// <param name="InManager"></param>
/// <param name="InScene"></param>
/// 
void InitializeSdkObjects(FbxManager*& InManager, FbxScene*& InScene)
{
	InManager = FbxManager::Create();

	FbxIOSettings* FBXIO = FbxIOSettings::Create(InManager, IOSROOT);
	InManager->SetIOSettings(FBXIO);

	FbxString FBXPath = FbxGetApplicationDirectory();
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

void GetPolygons(FbxMesh* InMesh, FIEMesh& OutData, const FIEParam& InParam)
{
	int PolygonCount = InMesh->GetPolygonCount();
	FbxVector4* ControlPoints = InMesh->GetControlPoints();

	int VertexID = 0;
	for (int i = 0; i < PolygonCount; i++)//图元
	{
		OutData.VertexData.push_back(FIETriangle());
		FIETriangle& InTriangle = OutData.VertexData[OutData.VertexData.size() - 1];

		int PolygonSize = InMesh->GetPolygonSize(i);
		for (int j = 0; j < PolygonSize; j++)
		{
			int ControlPointIndex = InMesh->GetPolygonVertex(i, j);

			int Indexs[3] = { 0,1,2 };
			int CoordinateZSymbol = 1;
			if (!InParam.bOriginalCoordinate)
			{
				Indexs[1] = 2;
				Indexs[2] = 1;

				CoordinateZSymbol = -1;
			}

			//Coordinates
			//拿到位置
			//FbxVector4 Coordinates;
			//获取位置
			{
				//考虑缩放
				FbxDouble3 Scaling = InMesh->GetNode()->LclScaling;

				InTriangle.Vertexs[j].Position.x = ControlPoints[ControlPointIndex].mData[Indexs[0]] * Scaling[Indexs[0]];
				InTriangle.Vertexs[j].Position.y = ControlPoints[ControlPointIndex].mData[Indexs[1]] * Scaling[Indexs[1]];
				InTriangle.Vertexs[j].Position.z = CoordinateZSymbol * ControlPoints[ControlPointIndex].mData[Indexs[2]] * Scaling[Indexs[2]];
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
						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ControlPointIndex);

						InTriangle.Vertexs[j].UV.x = UV.mData[0];
						InTriangle.Vertexs[j].UV.y = 1.f - UV.mData[1];//V是反的 DX 和 OpenGL不一样
					}
					else if (ReferenceMode == fbxsdk::FbxLayerElement::eIndexToDirect)
					{
						int ID = TextureUV->GetIndexArray().GetAt(ControlPointIndex);

						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].UV.x = UV.mData[0];
						InTriangle.Vertexs[j].UV.y = 1.f - UV.mData[1];//V是反的 DX 和 OpenGL不一样
					}
				}
				else if (ModeType == fbxsdk::FbxLayerElement::eByPolygonVertex)
				{
					int TextureUVIndex = InMesh->GetTextureUVIndex(i, j);
					FbxVector2 UV = TextureUV->GetDirectArray().GetAt(TextureUVIndex);
					switch (ReferenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						FbxVector2 UV = TextureUV->GetDirectArray().GetAt(ControlPointIndex);

						InTriangle.Vertexs[j].UV.x = UV.mData[0];
						InTriangle.Vertexs[j].UV.y = 1.f - UV.mData[1];//V是反的 DX 和 OpenGL不一样
						break;
					}
					}
				}
			}

			//法线
			for (int l = 0; l < InMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* Normal = InMesh->GetElementNormal(l);
				auto NormalReferenceMode = Normal->GetReferenceMode();

				if (Normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (NormalReferenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(VertexID);

						InTriangle.Vertexs[j].Normal.x = NormalPoint.mData[Indexs[0]];
						InTriangle.Vertexs[j].Normal.y = NormalPoint.mData[Indexs[1]];
						InTriangle.Vertexs[j].Normal.z = NormalPoint.mData[Indexs[2]];

						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						int ID = Normal->GetIndexArray().GetAt(VertexID);

						FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].Normal.x = NormalPoint.mData[Indexs[0]];
						InTriangle.Vertexs[j].Normal.y = NormalPoint.mData[Indexs[1]];
						InTriangle.Vertexs[j].Normal.z = NormalPoint.mData[Indexs[2]];
					}
					}
				}
				else if (Normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					switch (NormalReferenceMode)
					{
					case fbxsdk::FbxLayerElement::eDirect:
					{
						FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(ControlPointIndex);
						InTriangle.Vertexs[j].Normal.x = NormalPoint.mData[Indexs[0]];
						InTriangle.Vertexs[j].Normal.y = NormalPoint.mData[Indexs[1]];
						InTriangle.Vertexs[j].Normal.z = NormalPoint.mData[Indexs[2]];
						break;
					}
					case fbxsdk::FbxLayerElement::eIndexToDirect:
					{
						int ID = Normal->GetIndexArray().GetAt(ControlPointIndex);
						FbxVector4 NormalPoint = Normal->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].Normal.x = NormalPoint.mData[Indexs[0]];
						InTriangle.Vertexs[j].Normal.y = NormalPoint.mData[Indexs[1]];
						InTriangle.Vertexs[j].Normal.z = NormalPoint.mData[Indexs[2]];
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
						FbxVector4 TangentValue = Tangent->GetDirectArray().GetAt(VertexID);

						InTriangle.Vertexs[j].Tangent.x = TangentValue.mData[Indexs[0]];
						InTriangle.Vertexs[j].Tangent.y = TangentValue.mData[Indexs[1]];
						InTriangle.Vertexs[j].Tangent.z = TangentValue.mData[Indexs[2]];
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int ID = Tangent->GetIndexArray().GetAt(VertexID);

						FbxVector4 TangentValue = Tangent->GetDirectArray().GetAt(ID);
						InTriangle.Vertexs[j].Tangent.x = TangentValue.mData[Indexs[0]];
						InTriangle.Vertexs[j].Tangent.y = TangentValue.mData[Indexs[1]];
						InTriangle.Vertexs[j].Tangent.z = TangentValue.mData[Indexs[2]];
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

void GetIndex(FIEMesh& InMesh)
{
	uint16_t VertexDataSize = InMesh.VertexData.size() * 3.f;
	InMesh.IndexData.resize(InMesh.VertexData.size() * 3.f);

	for (uint16_t i = 0; i < VertexDataSize; i++)
	{
		InMesh.IndexData[i] = i;
	}
}

void GetMesh(FbxNode* InNode, FIEModel& InModel, const FIEParam& InParam)
{
	FbxMesh* NodeMesh = (FbxMesh*)InNode->GetNodeAttribute();

	InModel.MeshData.push_back(FIEMesh());
	FIEMesh& InMesh = InModel.MeshData[InModel.MeshData.size() - 1];

	GetPolygons(NodeMesh, InMesh, InParam);

	GetIndex(InMesh);

	LinkMaterial(NodeMesh, InModel.MaterialMap);

	LinkMaterialTexture(NodeMesh, InModel.MaterialMap);

	MaterialMappings(NodeMesh, InMesh);
}

void RecursiveLoadMesh(FbxNode* InNode, FIERenderData& OutData, const FIEParam& InParam)
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
			OutData.ModelData.push_back(FIEModel());
			FIEModel& InModel = OutData.ModelData[OutData.ModelData.size() - 1];
			GetMesh(InNode, InModel, InParam);
		}
		else if (AttributeType == fbxsdk::FbxNodeAttribute::eSkeleton)
		{

		}
	}

	for (int i = 0; i < InNode->GetChildCount(); i++)
	{
		RecursiveLoadMesh(InNode->GetChild(i), OutData, InParam);
	}
}

void DestroySdkObjects(FbxManager* InManager)
{
	if (InManager)
	{
		InManager->Destroy();
	}
}

void LinkMaterial(const FbxGeometry* InGeometry, std::map<int, FIEMaterial>& OutMap)
{
	int MaterialCount = 0;
	FbxNode* Node = nullptr;

	if (InGeometry)
	{
		Node = InGeometry->GetNode();
		if (Node)
		{
			MaterialCount = Node->GetMaterialCount();
		}
	}

	if (MaterialCount > 0)
	{
		FbxPropertyT<FbxDouble3> KFbxDouble3;
		FbxPropertyT<FbxDouble> KFbxDouble1;
		FbxColor TheColor;

		for (size_t i = 0; i < MaterialCount; i++)
		{
			OutMap.insert({i, FIEMaterial()});
			FIEMaterial& InMap = OutMap[i];

			FbxSurfaceMaterial* FBXMaterialInst = Node->GetMaterial(i);

			//材质名字
			InMap.Name = FBXMaterialInst->GetName();

			if (FBXMaterialInst->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				KFbxDouble3 = static_cast<FbxSurfacePhong*>(FBXMaterialInst)->Ambient;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Ambient.Update(&TheColor);

				KFbxDouble3 = static_cast<FbxSurfacePhong*>(FBXMaterialInst)->Diffuse;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.DiffuseColor.Update(&TheColor);

				KFbxDouble3 = static_cast<FbxSurfacePhong*>(FBXMaterialInst)->Specular;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Specular.Update(&TheColor);

				KFbxDouble3 = static_cast<FbxSurfacePhong*>(FBXMaterialInst)->Emissive;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Emissive.Update(&TheColor);

				KFbxDouble1 = static_cast<FbxSurfacePhong*>(FBXMaterialInst)->TransparencyFactor;
				auto Value = 1.0 - KFbxDouble1.Get();
				InMap.Opacity.Update(&Value);

				KFbxDouble1 = static_cast<FbxSurfacePhong*>(FBXMaterialInst)->Shininess;
				Value = KFbxDouble1.Get();
				InMap.Shininess.Update(&Value);

				KFbxDouble1 = static_cast<FbxSurfacePhong*>(FBXMaterialInst)->ReflectionFactor;
				Value = KFbxDouble1.Get();
				InMap.Reflectivity.Update(&Value);
			}
			else if (FBXMaterialInst->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				KFbxDouble3 = static_cast<FbxSurfaceLambert*>(FBXMaterialInst)->Ambient;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Ambient.Update(&TheColor);

				KFbxDouble3 = static_cast<FbxSurfaceLambert*>(FBXMaterialInst)->Diffuse;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.DiffuseColor.Update(&TheColor);

				KFbxDouble3 = static_cast<FbxSurfaceLambert*>(FBXMaterialInst)->Emissive;
				TheColor.Set(KFbxDouble3.Get()[0], KFbxDouble3.Get()[1], KFbxDouble3.Get()[2]);
				InMap.Emissive.Update(&TheColor);

				KFbxDouble1 = static_cast<FbxSurfaceLambert*>(FBXMaterialInst)->TransparencyFactor;
				auto Value = 1.0 - KFbxDouble1.Get();
				InMap.Opacity.Update(&Value);
			}
			else
			{
				//"Unknown type of Material";
			}

			//Shading类型
			InMap.ShadingModel = FBXMaterialInst->ShadingModel.Get().Buffer();
		}
	}
}

void MaterialMappings(FbxMesh* InFbxMesh, FIEMesh& OutMesh)
{
	int MaterialCount = 0;
	FbxNode* Node = nullptr;

	if (InFbxMesh)
	{
		Node = InFbxMesh->GetNode();
		if (Node)
		{
			MaterialCount = Node->GetMaterialCount();
		}
	}

	for (size_t i = 0; i < InFbxMesh->GetElementMaterialCount(); i++)
	{
		if (FbxGeometryElementMaterial* FBXGElementMaterial = InFbxMesh->GetElementMaterial(i))
		{
			int IndexArrayCount = FBXGElementMaterial->GetIndexArray().GetCount();
			for (size_t j = 0; j < IndexArrayCount; j++)
			{
				//材质ID
				auto ValueIndex = FBXGElementMaterial->GetIndexArray().GetAt(j);
				OutMesh.VertexData[j].MaterialID = ValueIndex;
			}
		}
	}
}

void LinkMaterialTexture(const FbxGeometry* InGeometry, std::map<int, FIEMaterial>& OutMap)
{
	int MaterialIndex = -1;
	FbxProperty Property;

	if (InGeometry->GetNode() == nullptr)
	{
		return;
	}

	int SurfaceNumber = InGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (MaterialIndex = 0; MaterialIndex < SurfaceNumber; MaterialIndex++)
	{
		if (FbxSurfaceMaterial* InSurfaceMaterial = InGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(MaterialIndex))
		{
			FIEMaterial &InMaterial = OutMap[MaterialIndex];

			int i = 0;
			FBXSDK_FOR_EACH_TEXTURE(i)
			{
				const char* FindName = FbxLayerElement::sTextureChannelNames[i];
				Property = InSurfaceMaterial->FindProperty(FindName);

				if (Property.IsValid())
				{
					int TextureCount = Property.GetSrcObjectCount<FbxTexture>();
					if (FIEPropertyObject* InProperty = InMaterial.Find(FindName))
					{
						for (int j = 0; j < TextureCount; j++)
						{
							//分层纹理
							if (FbxLayeredTexture* LayeredTexture = Property.GetSrcObject<FbxLayeredTexture>(j))
							{
								//分层纹理
							}
							else
							{
								if (FbxTexture* Texture = Property.GetSrcObject<FbxTexture>(j))
								{
									InProperty->TextureName = Texture->GetName();
								
									if (FbxFileTexture* FileTexture = FbxCast<FbxFileTexture>(Texture))
									{
										InProperty->TextureType = "File Texture";
										InProperty->TextureFilename = FileTexture->GetFileName();
									}
									else if(FbxProceduralTexture* ProceduralTexture = FbxCast<FbxProceduralTexture>(Texture))
									{
										InProperty->TextureType = "Procedural Texture";
									}

									//Texture->GetScaleU();
									//Texture->GetScaleV();

									//Texture->GetTranslationU();
									//Texture->GetTranslationV();

									//Texture->GetSwapUV();

									//Texture->GetRotationU();
									//Texture->GetRotationV();
									//Texture->GetRotationW();

									//Texture->GetCroppingLeft();
									//Texture->GetCroppingTop();
									//Texture->GetCroppingRight();
									//Texture->GetCroppingBottom();
								}
							}
						}
					}
				}
			}
		}
	}
}

namespace FBXAssetImport
{
	void LoadMeshData(const char* InPath, FIERenderData& OutData, const FIEParam& InParam)
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
				RecursiveLoadMesh(Node->GetChild(i), OutData, InParam);
			}
		}

		//干掉
		DestroySdkObjects(SdkManager);
	}
}


//int main()
//{
//	//
//	FFBXRenderData Out;
//	std::string FBXPath = "P:/FBXTest/Heart.fbx";
//
//	FFBXAssetImport().LoadMeshData(FBXPath, Out);
//
//	return 0;
//}
//
