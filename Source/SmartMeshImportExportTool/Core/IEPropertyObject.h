#pragma once

#include "MeshImportExportTool.h"

struct FIEPropertyObject
{
	std::string PropertyName;

	std::string TextureType;
	std::string TextureName;
	std::string TextureFilename;
};
//
//struct FIEPropertyObjectFloat3 :public FIEPropertyObject
//{
//	FIEPropertyObjectFloat3()
//	{
//		memset(Data, 0, sizeof(float) * 3);
//	}
//	FIEPropertyObjectFloat3(const std::string & InName)
//	{
//		PropertyName = InName; 
//		memset(Data, 0, sizeof(float) * 3);
//	}
//	FIEPropertyObjectFloat3& operator=(const char* InContent)
//	{
//		PropertyName = InContent;
//
//		return *this;
//	}
//
//	float* Buffer() { return Data; }
//protected:
//	float Data[3];
//};

//typedef long long int64;
DEFINE_PROPERTY_IE(int, 4);
DEFINE_PROPERTY_IE(int, 3);
DEFINE_PROPERTY_IE(int, 2);
DEFINE_PROPERTY_IE(int, 1);
DEFINE_PROPERTY_IE(char, 4);
DEFINE_PROPERTY_IE(char, 3);
DEFINE_PROPERTY_IE(char, 2);
DEFINE_PROPERTY_IE(char, 1);
DEFINE_PROPERTY_IE(float, 4);
DEFINE_PROPERTY_IE(float, 3);
DEFINE_PROPERTY_IE(float, 2);
DEFINE_PROPERTY_IE(float, 1);
DEFINE_PROPERTY_IE(double, 4);
DEFINE_PROPERTY_IE(double, 3);
DEFINE_PROPERTY_IE(double, 2);
DEFINE_PROPERTY_IE(double, 1);

