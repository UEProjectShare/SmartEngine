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
	float3 LightIntensity;
	float3 LightDirection;;
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

	//世界坐标
	Out.WorldPosition = mul(float4(MV.Position, 1.f), WorldMatrix);

	//变换到齐次剪辑空间
	Out.Position = mul(Out.WorldPosition, ViewProjectionMatrix);
	Out.Color.rgb = MV.Normal.rgb;

	//转法线
	Out.Normal = mul(MV.Normal, (float3x3)WorldMatrix);

	//切线
	Out.UTangent = MV.UTangent;

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) : SV_TARGET
{
	float4 AmbientLight = { 0.15f, 0.15f, 0.25f, 1.0f };

	float3 ModelNormal = normalize(MVOut.Normal);
	float3 NormalizeLightDirection = normalize(-LightDirection);

	FMaterial Material;
	Material.BaseColor = BaseColor;

	float DotValue = 0;
	float4 Specular = { 0.f, 0.f, 0.f, 1.f };

	//兰伯特
	if (MaterialType == 0)
	{
		DotValue = max(dot(ModelNormal, NormalizeLightDirection), 0.0);
	}
	//半兰伯特
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

		DotValue = max(dot(ModelNormal, NormalizeLightDirection), 0.0);

		if (DotValue > 0.f)
		{
			float MaterialShininess = 1.f - saturate(MaterialRoughness);
			float M = MaterialShininess * 100.f;

			Specular = pow(max(dot(ViewDirection, ReflectDirection),0.f), M);
		}
	}
	else if (MaterialType == 3)//blinn-phong 
	{
		float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
		float3 HalfDirection = normalize(ViewDirection + NormalizeLightDirection);

		DotValue = max(dot(ModelNormal, NormalizeLightDirection), 0.0);

		if (DotValue > 0.f)
		{
			float MaterialShininess = 1.f - saturate(MaterialRoughness);
			float M = MaterialShininess * 100.f;

			Specular = pow(max(dot(ModelNormal, HalfDirection), 0.f), M);
		}
	}
	else if (MaterialType == 4)//Wrap 早期模拟皮肤的效果
	{
	//	float WrapValue = 1.f;//半兰伯特材质

		float WrapValue = 1.6f;

		float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
		DotValue = max((DiffuseReflection + WrapValue) / (1.f + WrapValue), 0.0);//[-1,1] => [0,1]
	}
	else if (MaterialType == 5)//Minnaert 模拟月球和天鹅绒效果
	{
		float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
	
		float DotLight = max(dot(ModelNormal, NormalizeLightDirection), 0.0);
		float DotView = max(dot(ModelNormal, ViewDirection), 0.0);

		float MaterialShininess = 1.f - saturate(MaterialRoughness);
		float M = MaterialShininess * 10.f;
		DotValue = saturate(DotLight * pow(DotLight * DotView, M));
	}
	else if (MaterialType == 6)//Banded 卡通
	{
		//融入半兰伯特思想
		float DiffuseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;
	
		float Layered = 4.f; 

		DotValue = floor(DiffuseReflection * Layered) / Layered;
	}
	else if (MaterialType == 7)//Banded 卡通
	{
		float4 Color2 = {245.f/255.f, 88.f/255.f, 0.f, 1.f};

		//灯光点乘值
		float LightDotValue = dot(ModelNormal, NormalizeLightDirection);

		float DiffuseReflection = (LightDotValue + 1.f) * 0.5f;

		float Layered = 7.f;
		DotValue = floor(DiffuseReflection * Layered) / Layered;

		Material.BaseColor = lerp(Color2, Material.BaseColor, LightDotValue);
	}
	else if (MaterialType == 8)//最终 Banded 卡通
	{
		//融入半兰伯特思想
		float DiffuseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;

		//分层思想
		float Layered = 4.f;
		DotValue = floor(DiffuseReflection * Layered) / Layered;

		//菲尼尔效果
		float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
		float3 F0 = { 0.05f, 0.05f, 0.05f };
		Specular.xyz = FresnelSchlickMethod(F0, ModelNormal, ViewDirection, 3).xyz;
	
		//反射
		float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));

		//高光
		if (DotValue > 0.f)
		{
			float MaterialShininess = 1.f - saturate(MaterialRoughness);
			float M = MaterialShininess * 60.f;

			Specular = Specular + pow(max(dot(ViewDirection, ReflectDirection), 0.f), M) / 0.0314f;
		}
	}
	else if (MaterialType == 9)// Back Light模型
	{
		//phong
		float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
		float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);

		//wrap
		float WrapValue = 1.2f;
		float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
		DotValue = max((DiffuseReflection + WrapValue) / (1.f + WrapValue), 0.0);//[-1,1] => [0,1]

		//高光
		if (DotValue > 0.f)
		{
			float MaterialShininess = 1.f - saturate(MaterialRoughness);
			float M = MaterialShininess * 100.f;

			Specular = pow(max(dot(ViewDirection, ReflectDirection), 0.f), M);
		} 

		//模拟透射效果
		float SSSValue = 1.3f;
		float TransmissionIntensity = 2.f;
		float TransmissionScope = 1.5f;

		float3 LightNormalizeValue = -normalize(ModelNormal * SSSValue + NormalizeLightDirection);
		DotValue = DotValue + pow(saturate(dot(LightNormalizeValue, ViewDirection)), TransmissionScope) * TransmissionIntensity;
	}
	else if (MaterialType == 10)// kajiya-kay模型
	{
		//各项异性 - 讲解纹理的时候再补充
	}
	else if (MaterialType == 11)
	{
		float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
		
		float NormalLight = saturate(dot(ModelNormal, NormalizeLightDirection));//兰伯特
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
	//菲尼尔
	else if (MaterialType == 100)
	{
		float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
		float3 F0 = { 0.1f, 0.1f, 0.1f };
		Specular.xyz = FresnelSchlickMethod(F0, ModelNormal, ViewDirection, 3).xyz;
	}
	
	//最终颜色贡献
	MVOut.Color = Material.BaseColor * DotValue + //漫反射
		AmbientLight * Material.BaseColor + //间接光
		Specular * Material.BaseColor;//高光
	
	return MVOut.Color;
}