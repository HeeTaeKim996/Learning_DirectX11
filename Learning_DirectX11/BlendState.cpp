#include "pch.h"
#include "BlendState.h"




BlendState::BlendState(ComPtr<ID3D11Device> device) : _device(device)
{
}

BlendState::~BlendState()
{
}

void BlendState::Create(D3D11_RENDER_TARGET_BLEND_DESC blendDesc, float factor)
{
	_blendFactor = factor;

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0] = blendDesc;

	HRESULT hr = _device->CreateBlendState(&desc, _blendState.GetAddressOf());

	/*
	○ BlendState
	 - PS 가 PixelShading 그린후, 작동. (OM 에서의 출력 직전 발동)
	 - 픽셀 셰이더에서 새로 출력한 색과, 기존 색 을 어떻게 조합할지를 결정
	*/
}




