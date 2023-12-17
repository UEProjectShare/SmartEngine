#include "Material.hlsl"
#include "Light.hlsl"

cbuffer ObjectConstBuffer :register(b0)//b0->b14
{
	float4x4 WorldMatrix;
}

cbuffer ViewportConstBuffer : register(b1)//b0->b14
{
	float4 ViewportPosition;
	float4x4 ViewProjectionMatrix;
}

cbuffer MaterialConstBuffer : register(b2)//b0->b14
{	
	int MaterialType;
	float4 BaseColor; 
	float MaterialRoughness;
	float4x4 TransformInformation;
}

cbuffer LightConstBuffer : register(b3)//b0->b14
{
	Light SceneLights[16];
}

struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
	float3 UTangent: TANGENT;
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
	float3 UTangent : TANGENT;
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)
{
	MeshVertexOut Out;

	//��������
	Out.WorldPosition = mul(float4(MV.Position, 1.f), WorldMatrix);

	//�任����μ����ռ�
	Out.Position = mul(Out.WorldPosition, ViewProjectionMatrix);
	Out.Color.rgb = MV.Normal.rgb;

	if (MaterialType == 13)
	{
		Out.Normal = MV.Normal;
	}
	else
	{
		//ת����
		Out.Normal = mul(MV.Normal, (float3x3)WorldMatrix);
	}
	
	//����
	Out.UTangent = MV.UTangent;

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) :SV_TARGET
{	
	FMaterial Material;
	Material.BaseColor = BaseColor;

	//BaseColor
	if (MaterialType == 12)
	{
		return Material.BaseColor;
	}
	else if (MaterialType == 13)
	{
		return float4(MVOut.Normal, 1.f);
	}
	else if (MaterialType == 14)
	{
		return float4(MVOut.Normal,1.f);
	}

	float4 AmbientLight = { 0.15f, 0.15f, 0.25f, 1.0f };
	float3 ModelNormal = normalize(MVOut.Normal);

	float DotValue = 0;

	float4 LightStrengths = { 0.f, 0.f, 0.f, 1.f };
	float4 Specular = { 0.f, 0.f, 0.f, 1.f };

	for (int i = 0; i < 16; i++)
	{
		if (length(SceneLights[i].LightIntensity.xyz) > 0.f)
		{
			float3 NormalizeLightDirection = normalize(GetLightDirection(SceneLights[i], MVOut.WorldPosition));

			float4 LightStrength = ComputeLightStrength(SceneLights[i], ModelNormal, MVOut.WorldPosition, NormalizeLightDirection);

			//������
			if (MaterialType == 0)
			{
				DotValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f);
			}
			//��������
			else if (MaterialType == 1)
			{
				float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
				DotValue = max((DiffuseReflection * 0.5f + 0.5f), 0.0);//[-1,1] => [0,1]
			}
			//phong
			else if (MaterialType == 2)
			{
				float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);

				DotValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f);

				float MaterialShininess = 1.f - saturate(MaterialRoughness);
				float M = MaterialShininess * 100.f;

				Specular = (M + 2.0f) * pow(max(dot(ViewDirection, ReflectDirection), 0.f), M) / 3.1415926;
			}
			else if (MaterialType == 3)//blinn-phong 
			{
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
				float3 HalfDirection = normalize(ViewDirection + NormalizeLightDirection);

				DotValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f);

				float MaterialShininess = 1.f - saturate(MaterialRoughness);
				float M = MaterialShininess * 100.f;

				//c=(m+2.f/PI) ��һ��ϵ�� �������ϸ�����Ƶ�
				Specular = (M + 2.0f) * pow(max(dot(ModelNormal, HalfDirection), 0.f), M) / 3.1415926;
			}
			else if (MaterialType == 4)//Wrap ����ģ��Ƥ����Ч��
			{
				//	float WrapValue = 1.f;//�������ز���

				float WrapValue = 1.6f;

				float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
				DotValue = max((DiffuseReflection + WrapValue) / (1.f + WrapValue), 0.0);//[-1,1] => [0,1]
			}
			else if (MaterialType == 5)//Minnaert ģ������������Ч��
			{
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);

				float DotLight = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f);
				float DotView = max(dot(ModelNormal, ViewDirection), 0.0);

				float MaterialShininess = 1.f - saturate(MaterialRoughness);
				float M = MaterialShininess * 10.f;
				DotValue = saturate(DotLight * pow(DotLight * DotView, M));
			}
			else if (MaterialType == 6)//Banded ��ͨ
			{
				if (i == 0)
				{
					//�����������˼��
					float DiffuseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;

					float Layered = 4.f;

					DotValue = floor(DiffuseReflection * Layered) / Layered;
				}
			}
			else if (MaterialType == 7)//Banded ��ͨ
			{
				if (i == 0)
				{
					float4 Color2 = { 245.f / 255.f,88.f / 255.f,.0f,1.f };

					//�ƹ���ֵ
					float LightDotValue = dot(ModelNormal, NormalizeLightDirection);

					float DiffuseReflection = (LightDotValue + 1.f) * 0.5f;

					float Layered = 7.f;
					DotValue = floor(DiffuseReflection * Layered) / Layered;

					Material.BaseColor = lerp(Color2, Material.BaseColor, LightDotValue);
				}
			}
			else if (MaterialType == 8)//���� Banded ��ͨ
			{
				if (i == 0)
				{
					//�����������˼��
					float DiffuseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;

					//�ֲ�˼��
					float Layered = 4.f;
					DotValue = floor(DiffuseReflection * Layered) / Layered;

					//�����Ч��
					float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
					float3 F0 = { 0.05f,0.05f,0.05f };
					Specular.xyz = FresnelSchlickMethod(F0, ModelNormal, ViewDirection, 3).xyz;

					//����
					float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));

					//�߹�
					if (DotValue > 0.f)
					{
						float MaterialShininess = 1.f - saturate(MaterialRoughness);
						float M = MaterialShininess * 60.f;

						Specular = Specular + pow(max(dot(ViewDirection, ReflectDirection), 0.f), M) / 0.0314f;
					}
				}
			}
			else if (MaterialType == 9)// Back Lightģ��
			{
				//phong
				float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);

				//wrap
				float WrapValue = 1.2f;
				float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
				DotValue = max((DiffuseReflection + WrapValue) / (1.f + WrapValue), 0.0);//[-1,1] => [0,1]

				//�߹�
				if (DotValue > 0.f)
				{
					float MaterialShininess = 1.f - saturate(MaterialRoughness);
					float M = MaterialShininess * 100.f;

					Specular = pow(max(dot(ViewDirection, ReflectDirection), 0.f), M);
				}

				//ģ��͸��Ч��
				float SSSValue = 1.3f;
				float TransmissionIntensity = 2.f;
				float TransmissionScope = 1.5f;

				float3 LightNormalizeValue = -normalize(ModelNormal * SSSValue + NormalizeLightDirection);
				DotValue = DotValue + pow(saturate(dot(LightNormalizeValue, ViewDirection)), TransmissionScope) * TransmissionIntensity;
			}
			else if (MaterialType == 10)// kajiya-kayģ��
			{
				//�������� - ���������ʱ���ٲ���
			}
			else if (MaterialType == 11)
			{
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);

				float NormalLight = saturate(pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f));//������
				float NormalView = saturate(dot(ModelNormal, ViewDirection));

				float Phiri =
					length(ViewDirection - ModelNormal * NormalView) *
					length(NormalizeLightDirection - ModelNormal * NormalLight);

				float ACosNormalView = acos(NormalView);//[0,1]
				float ACosNormalLight = acos(NormalLight);

				float Alpha = max(ACosNormalView, ACosNormalLight);
				float Beta = min(ACosNormalView, ACosNormalLight);

				float MyRoughness = pow(MaterialRoughness, 2);

				float A = 1 - 0.5f * (MyRoughness / (MyRoughness + 0.33f));
				float B = 0.45f * (MyRoughness / (MyRoughness + 0.09f));

				DotValue = NormalLight * (A + B * max(0, Phiri) * sin(Alpha) * tan(Beta));
			}
			//�����
			else if (MaterialType == 100)
			{
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
				float3 F0 = { 0.1f, 0.1f, 0.1f };
				Specular.xyz = FresnelSchlickMethod(F0, ModelNormal, ViewDirection, 3).xyz;
			}

			LightStrengths += LightStrength * DotValue * float4(SceneLights[i].LightIntensity, 1.f);
			LightStrengths.w = 1.f;
		}
	}

	//������ɫ����
	MVOut.Color = LightStrengths * 
		(Material.BaseColor //������
		+ Specular * Material.BaseColor)+ //�߹�
		AmbientLight * Material.BaseColor; //��ӹ�
	
	return MVOut.Color;
}