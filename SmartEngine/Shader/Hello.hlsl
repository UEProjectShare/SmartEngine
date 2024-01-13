#include "Material.hlsl"
#include "PBR.hlsl"
#include "SkyFunction.hlsl"
#include "ShadowFunction.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
	float3 UTangent: TANGENT;
	float2 TexCoord: TEXCOORD;
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
	float3 UTangent : TANGENT;
	float2 TexCoord: TEXCOORD;
};

MeshVertexOut VertexShaderMain(MeshVertexIn MV)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialIndex];

	MeshVertexOut Out;

	//颜色
	Out.Color = MV.Color;

	//世界坐标
	Out.WorldPosition = mul(float4(MV.Position, 1.f), WorldMatrix);

	//变换到齐次剪辑空间
	Out.Position = mul(Out.WorldPosition, ViewProjectionMatrix);

	if (MatConstBuffer.MaterialType == 13)
	{
		Out.Normal = MV.Normal;
	}
	else
	{
		//转法线
		Out.Normal = mul(MV.Normal, (float3x3)WorldMatrix);
	}

	//切线
	Out.UTangent = mul(MV.UTangent, (float3x3)WorldMatrix);

	//ui坐标
	float4 MyTexCoord = mul(float4(MV.TexCoord, 0.0f, 1.f), ObjectTextureTransform);
	Out.TexCoord = mul(MyTexCoord, MatConstBuffer.TransformInformation).xy;

	return Out;
}

float4 PixelShaderMain(MeshVertexOut MVOut) :SV_TARGET
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialIndex];

	if (MatConstBuffer.MaterialType == 101)
	{
		return float4(SimpleShadowMap.Sample(TextureSampler, MVOut.TexCoord).rrr, 1.f);
	}
	
	FMaterial Material;

	//获取BaseColor
	Material.BaseColor = GetMaterialBaseColor(MatConstBuffer, MVOut.TexCoord);

	//BaseColor
	if (MatConstBuffer.MaterialType == 12)
	{
		float4 Specular = GetMaterialSpecular(MatConstBuffer, MVOut.TexCoord);
		return Material.BaseColor * Specular + Material.BaseColor + 0.1f;
	}
	else if (MatConstBuffer.MaterialType == 13)
	{
		return float4(MVOut.Normal, 1.f);
	}
	else if (MatConstBuffer.MaterialType == 14)
	{
		return float4(MVOut.Normal,1.f);
	}

	float4 AmbientLight = { 0.15f, 0.15f, 0.25f, 1.0f };
	float3 ModelNormal = normalize(MVOut.Normal);

	//获取法线 如果设置了法线贴图就获取法线贴图
	ModelNormal = GetMaterialNormals(MatConstBuffer, MVOut.TexCoord,ModelNormal,MVOut.UTangent);

	float DotValue = 0;

	float4 FinalColor = { 0.f,0.f,0.f,1.f };

	for (int i = 0; i < 16; i++)
	{
		if (length(SceneLights[i].LightIntensity.xyz) > 0.f)
		{
			//拿到Specular
			float4 Specular = GetMaterialSpecular(MatConstBuffer, MVOut.TexCoord);

			float3 NormalizeLightDirection = normalize(GetLightDirection(SceneLights[i],MVOut.WorldPosition.xyz));

			float4 LightStrength = ComputeLightStrength(SceneLights[i], ModelNormal, MVOut.WorldPosition.xyz, NormalizeLightDirection);

			//兰伯特
			if (MatConstBuffer.MaterialType == 0)
			{
				DotValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0),2.f);
			}
			//半兰伯特
			else if (MatConstBuffer.MaterialType == 1)
			{
				float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
				DotValue = max((DiffuseReflection * 0.5f + 0.5f), 0.0);//[-1,1] => [0,1]
			}
			//phong
			else if (MatConstBuffer.MaterialType == 2)
			{
				float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);

				DotValue = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f);

				float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
				float M = MaterialShininess * 100.f;

				Specular *= saturate((M + 2.0f) * pow(max(dot(ViewDirection, ReflectDirection), 0.f), M) / 3.1415926);
			}
			//blinn-phong 
			else if (MatConstBuffer.MaterialType == 3)
			{
				//当前的公式已经对blinn-phong 模型做了很大的变形
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
				float3 HalfDirection = normalize(ViewDirection + NormalizeLightDirection);

				//先半兰博特化 再减去0.2f曝光的，再平方，变得更柔和
				DotValue = pow(max(0.0,(dot(ModelNormal, NormalizeLightDirection) * 0.5f + 0.5f) - 0.2f),2);

				float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
				float M = MaterialShininess * 100.f;

				//c=(m+2.f/PI) 归一化系数 后面会详细讲解推导
				Specular *= saturate((M + 2.0f) * pow(max(dot(HalfDirection, ModelNormal), 0.f), M) / 3.1415926);
			}
			else if (MatConstBuffer.MaterialType == 4)//Wrap 早期模拟皮肤的效果
			{
				//	float WrapValue = 1.f;//半兰伯特材质

				float WrapValue = 2.6f;

				float DiffuseReflection = dot(ModelNormal, NormalizeLightDirection);
				DotValue = max((DiffuseReflection + WrapValue) / (1.f + WrapValue), 0.0);//[-1,1] => [0,1]
			}
			else if (MatConstBuffer.MaterialType == 5)//Minnaert 模拟月球和天鹅绒效果
			{
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);

				float DotLight = pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f);
				float DotView = max(dot(ModelNormal, ViewDirection), 0.0);

				float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
				float M = MaterialShininess * 10.f;
				DotValue = saturate(DotLight * pow(DotLight * DotView, M));
			}
			else if (MatConstBuffer.MaterialType == 6)//Banded 卡通
			{
				if (i == 0)
				{
					//融入半兰伯特思想
					float DiffuseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;

					float Layered = 4.f;

					DotValue = floor(DiffuseReflection * Layered) / Layered;
				}
			}
			else if (MatConstBuffer.MaterialType == 7)//Banded 卡通
			{
				if (i == 0)
				{
					float4 Color2 = { 245.f / 255.f,88.f / 255.f,.0f,1.f };

					//灯光点乘值
					float LightDotValue = dot(ModelNormal, NormalizeLightDirection);

					float DiffuseReflection = (LightDotValue + 1.f) * 0.5f;

					float Layered = 7.f;
					DotValue = floor(DiffuseReflection * Layered) / Layered;

					Material.BaseColor = lerp(Color2, Material.BaseColor, LightDotValue);
				}
			}
			else if (MatConstBuffer.MaterialType == 8)//最终 Banded 卡通
			{
				if (i == 0)
				{
					//融入半兰伯特思想
					float DiffuseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;

					//分层思想
					float Layered = 4.f;
					DotValue = floor(DiffuseReflection * Layered) / Layered;

					//菲尼尔效果
					float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
					float3 F0 = { 0.05f,0.05f,0.05f };
					Specular.xyz = FresnelSchlickMethod(F0, ModelNormal, ViewDirection, 3).xyz;

					//反射
					float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));

					//高光
					if (DotValue > 0.f)
					{
						float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
						float M = MaterialShininess * 60.f;

						Specular += saturate(pow(max(dot(ViewDirection, ReflectDirection), 0.f), M) / 0.0314f);
					}
				}
			}
			else if (MatConstBuffer.MaterialType == 9)// Back Light模型
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
					float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
					float M = MaterialShininess * 100.f;

					Specular = saturate(pow(max(dot(ViewDirection, ReflectDirection), 0.f), M));
				}

				//模拟透射效果
				float SSSValue = 1.3f;
				float TransmissionIntensity = 2.f;
				float TransmissionScope = 1.5f;

				float3 LightNormalizeValue = -normalize(ModelNormal * SSSValue + NormalizeLightDirection);
				DotValue = DotValue + pow(saturate(dot(LightNormalizeValue, ViewDirection)), TransmissionScope) * TransmissionIntensity;
			}
			else if (MatConstBuffer.MaterialType == 10)// kajiya-kay模型
			{
				//各项异性 - 讲解纹理的时候再补充

			}
			else if (MatConstBuffer.MaterialType == 11)//OrenNayar
			{
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);

				float NormalLight = saturate(pow(max(dot(ModelNormal, NormalizeLightDirection), 0.0), 2.f));//兰伯特
				float NormalView = saturate(dot(ModelNormal, ViewDirection));

				float Phiri =
					length(ViewDirection - ModelNormal * NormalView) +
					length(NormalizeLightDirection - ModelNormal * NormalLight);

				float ACosNormalView = acos(NormalView);//[0,1]
				float ACosNormalLight = acos(NormalLight);

				float Alpha = max(ACosNormalView, ACosNormalLight);
				float Beta = min(ACosNormalView, ACosNormalLight);

				float MyRoughness = pow(MatConstBuffer.MaterialRoughness, 2);

				float A = 1 - 0.5f * (MyRoughness / (MyRoughness + 0.33f));
				float B = 0.45f * (MyRoughness / (MyRoughness + 0.09f));

				DotValue = NormalLight * (A + B * max(0, Phiri) * sin(Alpha) * tan(Beta));
			}
			else if (MatConstBuffer.MaterialType == 15)//透明物体
			{
				////当前的公式已经对blinn-phong 模型做了很大的变形
				//float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
				//float3 HalfDirection = normalize(ViewDirection + NormalizeLightDirection);
				//
				////先半兰博特化 再减去0.2f曝光的，再平方，变得更柔和
				////DotValue = pow(max(0.0, (dot(ModelNormal, NormalizeLightDirection) * 0.5f + 0.5f) - 0.2f), 2);
				//
				//float MaterialShininess = 1.f - saturate(MatConstBuffer.MaterialRoughness);
				//float M = MaterialShininess * 100.f;
				//
				////c=(m+2.f/PI) 归一化系数 后面会详细讲解推导
				//Specular *= saturate((M + 2.0f) * pow(max(dot(HalfDirection, ModelNormal) * DotValue, 0.f), M) / 3.1415926);
			}
			else if (MatConstBuffer.MaterialType == 20)//PBR
			{
				float3 L = NormalizeLightDirection;
				float3 V = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
				float3 H = normalize(V + L);
				float3 N = ModelNormal;

				float PI = 3.1415926;

				float Roughness = MatConstBuffer.MaterialRoughness;
				float3 Metallic = MatConstBuffer.Metallicity;

				float4 D = GetDistributionGGX(N, H, Roughness);

				float3 F0 = 0.04f;
				F0 = lerp(F0, MatConstBuffer.BaseColor.rgb, Metallic);
				float4 F = float4(FresnelSchlickMethod(F0, N, V, 5), 1.0f);

				float4 G = GSmith(N, V, L, Roughness);

				float LoH = saturate(dot(L, H));
				float NoV = saturate(dot(N, V));
				float NoL = saturate(dot(N, L));

				float3 FIndirect = GetIndirectLight(LoH, F0, Roughness);
				float3 IndirKS = GetDirectLight(NoV,F0, Roughness);

				float4 Kd = 1 - float4(FIndirect,1.f);
				Kd *=(1-float4(IndirKS,1.f))*(1 - float4(Metallic, 1.f));

				float3 Diffuse = Kd.rgb * GetDiffuseLambert(MatConstBuffer.BaseColor.rgb);

				float4 Value = (D * F * G) / (4 * (NoV * NoL));
				Specular = float4(Value.rgb, 1.f);

				float3 Radiance = LightStrength.xyz;
				float3 MyColor = (Diffuse + Specular.xyz) * NoL * Radiance;

				//IBL
				return float4(MyColor.xyz, 1.0f);
			}
			//菲尼尔
			else if (MatConstBuffer.MaterialType == 100)
			{
				//另一种菲尼尔方法
				float3 ViewDirection = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
				DotValue = pow(1.f - max(dot(ModelNormal, ViewDirection), 0.0), 2.f);

				//Schlick 菲尼尔方法
				//float3 F0 = { 0.1f,0.1f,0.1f };
				//Specular.xyz = FresnelSchlickMethod(F0, ModelNormal, ViewDirection, 3).xyz;
			}

			float4 Diffuse = Material.BaseColor;;
			Specular = saturate(Specular);
			
			//float ShadowFactor = GetShadowFactor(MVOut.WorldPosition, SceneLights[i].ShadowTransform);
			//float ShadowFactor = GetShadowFactor_PCF_Sample4(MVOut.WorldPosition, SceneLights[i].ShadowTransform);
			float ShadowFactor = GetShadowFactor_PCF_Sample9(MVOut.WorldPosition, SceneLights[i].ShadowTransform);
			
			FinalColor += ShadowFactor * (saturate((Diffuse + Specular) * LightStrength * DotValue));
		}
	}

	//最终颜色贡献
	MVOut.Color = FinalColor + //最终颜色
		AmbientLight * Material.BaseColor; //间接光

	switch (MatConstBuffer.MaterialType)
	{
		case 2:
		case 3:
		case 9:	
		{
			//计算反射
			float3 ReflectionColor = GetReflectionColor(MatConstBuffer, ModelNormal, MVOut.WorldPosition.xyz);
			MVOut.Color.xyz += ReflectionColor;
			break;
		}
		case 15: //透明杯子
		{
			//先计算折射
			float3 NewRefract = GetRefract(ModelNormal, MVOut.WorldPosition.xyz, MatConstBuffer.Refraction);
			float3 SampleRefractColor = GetReflectionSampleColor(ModelNormal, NewRefract);

			//计算反射
			float3 NewReflect = GetReflect(ModelNormal, MVOut.WorldPosition.xyz);
			float3 SampleReflectionColor = GetReflectionSampleColor(ModelNormal, NewReflect);
			
			//算A通道
			float3 V = normalize(ViewportPosition.xyz - MVOut.WorldPosition.xyz);
			float Shininess = GetShininess(MatConstBuffer);
			float3 FresnelFactor = FresnelSchlickFactor(MatConstBuffer, ModelNormal, V);

			float3 Color = lerp(SampleRefractColor , SampleReflectionColor, pow(Shininess * FresnelFactor,2));

			MVOut.Color.xyz += Color;
			break;
		}
	}

	if (MatConstBuffer.MaterialType == 15)
	{
		//透明的
		MVOut.Color.a = MatConstBuffer.Transparency;
	}
	else
	{
		//透明的
		MVOut.Color.a = Material.BaseColor.a;
	}
	
	//计算雾
	MVOut.Color = GetFogValue(MVOut.Color, MVOut.WorldPosition);

	return MVOut.Color;
}