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
		�� D3D11_RASTERIZER_DESC
		 - �ַ� �ٰ����� ��� �׸��� �� ���� ����

		�� FillMode
		 - �ﰢ���� ��� ä���� �� ����
		 - D3D11_FILL_SOLID		: ���θ� ������ ä��. (�Ϲ����� 3D ������)
		 - D3D11_FILL_WIREFRAME	: ��(�׵θ�)�� �׸�

		�� CullMode
		 - �ﰢ���� ��� �鿡�� �� �� ���������� ����
		 - D3D11_CULL_BACK	: �ﰢ�� �޸� ����
		 - D3D11_CULL_NONE	: ��� �ﰢ�� �׸���

		�� FrontCounterClockwise
		 - ����������� �ε��� ������ ��ġ�� �ﰢ���� �ո� ���� ���������� ����
	*/
}
