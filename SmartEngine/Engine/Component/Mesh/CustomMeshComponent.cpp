#include "CustomMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CCustomMeshComponent::CCustomMeshComponent()
{

}

void CCustomMeshComponent::CreateMesh(FMeshRenderingData& MeshData, const string& InPath)
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

bool CCustomMeshComponent::LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData)
{
	if (InBuffSize > 0)
	{
		stringstream BuffStream(InBuff);
		char TmpLine[256] = { 0 };
		string MidTmpTag;

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
						//以后再写
					}
					else if (TmpLine[1] == 't')
					{
						//以后再写
					}
					else
					{
						//先添加一个
						MeshData.VertexData.push_back(FVertex(
							XMFLOAT3(), XMFLOAT4(Colors::White)));

						//拿到添加后的位置
						int TopIndex = MeshData.VertexData.size() - 1;
						XMFLOAT3& Float3InPos = MeshData.VertexData[TopIndex].Position;

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
						MeshData.IndexData.push_back(atoi(VPosIndex) - 1);

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

		return true;
	}

	return false;
}
