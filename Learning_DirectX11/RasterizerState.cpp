#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device) : _device(device)
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::Create()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;

	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);

	/*
		○ D3D11_RASTERIZER_DESC
		 - 주로 다각형을 어떻게 그릴지 에 대한 설정

		○ FillMode
		 - 삼각형을 어떻게 채울지 를 결정
		 - D3D11_FILL_SOLID		: 내부를 색으로 채움. (일반적인 3D 렌더링)
		 - D3D11_FILL_WIREFRAME	: 선(테두리)만 그림

		○ CullMode
		 - 삼각형을 어느 면에서 볼 때 무시할지를 결정
		 - D3D11_CULL_BACK	: 삼각형 뒷면 무시
		 - D3D11_CULL_NONE	: 모든 삼각형 그리기

		○ FrontCounterClockwise
		 - 어느방향으로 인덱스 정점이 배치된 삼각형을 앞면 으로 간주할지를 결정
	*/
}
