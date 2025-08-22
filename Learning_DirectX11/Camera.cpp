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
	// 역행렬을 이렇게 구한다는데, 성능이 좋으려나? 왜 이렇게 할까. 123123 에 Rot 전치하고, 4 에 <Vi,t> 3번하면 되는데

	//S_MatView = GetTransform()->GetWorldMatrix().Invert(); 
	// ※ NDC랑 좌표계가 같으니, 카메라의 SRT 메트릭스를 역행렬 때려도 구할 수 있긴 한데, 연산 성능이 좋지 않아 보인다



	if (_type == ProjectionType::Perspective)
	{
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 4.f, 800.f / 600.f, 1.f, 100.f);
		// 인자 순서대로, d의 세타(1 / tan(세타) 의 세타), ratio, near, far
		// ※ FOV 가 d 내에 세타인지 이제 알았네..
	}
	else
	{
		S_MatProjection = ::XMMatrixOrthographicLH(8, 6, 0.f, 1.f);
		// 인자 순서대로, Width, Height, Near, Far
	}

}
