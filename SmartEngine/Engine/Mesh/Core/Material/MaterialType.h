#pragma once

enum EMaterialType
{
	Lambert = 0,			//ֽ��
	HalfLambert,			//�ֲڱ���
	Phong,					//������Ч��
	BlinnPhong,				//������Ч��
	Wrap,					//ģ��Ƥ��Ч��
	Minnaert,				//������� ����
	Banded,					//������ͨЧ��
	GradualBanded,			//����Ŀ�ͨ����
	FinalBanded,			//�ں��˺ܶ�Ч���Ŀ�ͨ����
	Back,					//������Ч��
	AnisotropyKajiyaKay,	//GDC 2004�� KajiyaKay ͷ����Ⱦģ��
	OrenNayar,				//GDC ģ��ֲڱ���
	Fresnel = 100,			//����ȥ����Ч�����������
	Max,
};