#pragma once
#include "Core/IEPropertyObject.h"

//���� ���ϲ㴫��
struct MESHIMPORTEXPORTTOOL_API FIEParam
{
	FIEParam()
		:bOriginalCoordinate(true)
	{}

	bool bOriginalCoordinate;
};

//��������
struct MESHIMPORTEXPORTTOOL_API FIEVertex
{
	fvector_3d Position;
	fvector_3d Color;
	fvector_3d Normal;
	fvector_3d Tangent;
	fvector_2d UV;
	fvector_3d Binormal;
};

//������ȡ������һ����������
struct MESHIMPORTEXPORTTOOL_API FIETriangle
{
	FIETriangle()
		: MaterialID(0)
	{}

	FIEVertex Vertexs[3];
	int MaterialID;
};

//ģ������
struct MESHIMPORTEXPORTTOOL_API FIEMesh
{
	FIEMesh()
	{}
	std::vector<FIETriangle> VertexData;
	std::vector<uint16_t> IndexData;
};

//��������
struct MESHIMPORTEXPORTTOOL_API FIEMaterial
{
	FIEMaterial()
	{
		ADD_MEMBER_PROPERTY_POOL_IE(DiffuseColor);
		ADD_MEMBER_PROPERTY_POOL_IE(Ambient);
		ADD_MEMBER_PROPERTY_POOL_IE(Emissive);
		ADD_MEMBER_PROPERTY_POOL_IE(Specular);
		ADD_MEMBER_PROPERTY_POOL_IE(Shininess);
		ADD_MEMBER_PROPERTY_POOL_IE(Opacity);
		ADD_MEMBER_PROPERTY_POOL_IE(Reflectivity);
	}
	DEFINING_MEMBER_PROPERTY_POOL_IE

	DEFINING_MEMBER_PROPERTY_IE(double, 3, DiffuseColor);
	DEFINING_MEMBER_PROPERTY_IE(double, 3, Ambient);
	DEFINING_MEMBER_PROPERTY_IE(double, 3, Emissive);
	DEFINING_MEMBER_PROPERTY_IE(double, 3, Specular);
	DEFINING_MEMBER_PROPERTY_IE(double, 1, Shininess);
	DEFINING_MEMBER_PROPERTY_IE(double, 1, Opacity);
	DEFINING_MEMBER_PROPERTY_IE(double, 1, Reflectivity);

public:
	std::string ShadingModel;
	std::string Name;
};

//Model
struct MESHIMPORTEXPORTTOOL_API FIEModel
{
	std::vector<FIEMesh> MeshData;
	std::map<int, FIEMaterial> MaterialMap;
};

//��Ⱦ����
struct MESHIMPORTEXPORTTOOL_API FIERenderData
{
	std::vector<FIEModel> ModelData;
};