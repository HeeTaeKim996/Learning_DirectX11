#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Super(ComponentType::Camera)
{

}

Camera::~Camera()
{

}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
		
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	// ������� �̷��� ���Ѵٴµ�, ������ ��������? �� �̷��� �ұ�. 123123 �� Rot ��ġ�ϰ�, 4 �� <Vi,t> 3���ϸ� �Ǵµ�

	//S_MatView = GetTransform()->GetWorldMatrix().Invert(); 
	// �� NDC�� ��ǥ�谡 ������, ī�޶��� SRT ��Ʈ������ ����� ������ ���� �� �ֱ� �ѵ�, ���� ������ ���� �ʾ� ���δ�



	if (_type == ProjectionType::Perspective)
	{
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 4.f, 800.f / 600.f, 1.f, 100.f);
		// ���� �������, d�� ��Ÿ(1 / tan(��Ÿ) �� ��Ÿ), ratio, near, far
		// �� FOV �� d ���� ��Ÿ���� ���� �˾ҳ�..
	}
	else
	{
		S_MatProjection = ::XMMatrixOrthographicLH(8, 6, 0.f, 1.f);
		// ���� �������, Width, Height, Near, Far
	}

}
