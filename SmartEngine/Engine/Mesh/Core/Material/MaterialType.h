#pragma once

enum EMaterialType
{
	//局部光照模型
	Lambert = 0,			//纸张
	HalfLambert,			//粗糙表面
	Phong,					//金属的效果
	BlinnPhong,				//金属的效果
	Wrap,					//模拟皮肤效果
	Minnaert,				//做天鹅绒 月球
	Banded,					//基础卡通效果
	GradualBanded,			//渐变的卡通材质
	FinalBanded,			//融合了很多效果的卡通材质
	Back,					//做玉器效果
	AnisotropyKajiyaKay,	//GDC 2004年 KajiyaKay 头发渲染模型
	OrenNayar,				//GDC 模拟粗糙表面

	//单独显示对应效果
	BaseColor,				//本身的颜色
	Normal,					//显示法线
	WorldNormal,			//显示世界法线

	PBR = 20,				//基于物理的显示

	Fresnel = 100,			//单独去看的效果，不算材质
	Max,
};

enum EMaterialDisplayStatusType
{
	DefaultDisplay,//默认显示
	PointDisplay,//显示点
	WireframeDisplay,//线框
	TriangleDisplay = 4,//三角形显示
};