#include "pch.h"
#include "Graphics.h"


#if 1 
Graphics::Graphics(HWND hwnd)
{
	_hwnd = hwnd;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}
Graphics::~Graphics()
{

}
void Graphics::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewPort);

	/*	�� OMSetRenderTargets
		 - �ĸ���۸� ���� Ÿ������ ����
		 - (1) : �ĺ����� ��

		 �� ClearRenderTargetView
		 - WinAPI �� BeginPaint�� ������ �۾��� WM_ERASEBKGND �� �����ϰ�, ����� ��� ������ �������� �������, �ʱ�ȭ�ϴ� �۾�
		  �� �ʱ�ȭ�ϴ� ����� ������۰� �ƴ� �ĸ����

		 �� RSSetViewports
		  - viewPort�� ������ ���÷��� �������� ����. ���ڿ� ������ ���޹޴� ��ü�� ���ٴ� �Ϳ��� �� �� �ֵ���,
			��ü�� ������ Ÿ������, Device ���ؽ�Ʈ�� �ͼӵ�, �����Ͷ�����(Stage) �� ��.
	*/
}

void Graphics::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);

	/*	�� _swaChain->Present
		 - �ռ� RenderBegin���� �׸� �ĸ���۸�, ������ۿ� ������ �����ϴ� �Լ�


	*/
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // �� ZeroMemory == memtset(&desc, 0, sizeof(desc); ==>> (2)���� 0 ����
	{
		desc.BufferDesc.Width = GWinSizeX;
		desc.BufferDesc.Height = GWinSizeY;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		/*
			�� Numerator �� Denominator
			 - �ֻ����� ����(Numerator) �� �и�(Denominator). ���� 60Hz

			�� SampleDesc.Count
			 - ��� ������ �Ͼ�� ��, ȭ�� �ػ󵵸� Ű�� ��, ���鰣 ���� ���� ��� ������ ���̴µ�, Count �� 1�Ͻ� ���� �̻��
			   (���ϼ��� �� ���� ȭ�� �ػ󵵷� ���� �� ����)

			�� BufferUsage
			 - desc (DXGI_SWAP_CHAIN_DESC ) �� ��� �뵵.
			 - DXGI_USAGE_RENDER_TARGET_OUTPUT �� ���, ������ �۾����� ���� ������� �׷��ִ� ���ҷ� desc �� ����ϰڴٴ� �ǹ�

			�� BufferCount
			 - �ĸ������ ����. WinAPI �� BitBlt ���� hdc �� MemDC ����� ��ó��, �ĸ���� (MemDC) �� ����
		*/
	}



	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &desc,
		_swapChain.GetAddressOf(), _device.GetAddressOf(), nullptr, _deviceContext.GetAddressOf());
	/*
		 - (2) : ������ �� �ϵ������ �׷���ī�带 ���. ���� ��ǻ�Ϳ� �׷���ī�尡 ���ٸ�, �ٸ� �÷��� ��� �ʿ�
		 - (7) : ��ũ�� (����)
		 - (8) : �ռ� �ڵ忡�� DXGI_SWP_CHAIN_DESC �� ���� �� �Ѱ���
		 �� (8), (9), (11) : ** �� ������ �� GetAddressOf �� �������� ���.

	*/

	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);

	/*
		�� _swapChain -> GetBuffer ..
		 - _swapChain �� 0�� ���� �� backBuffer �� �Ҵ�

		�� CreateRenderTargetView ..
		 - backBuffer �� ��������, GPU �� ������ �� �� �ִ� View �� ������, _renderTargetView �� ����
	*/
}

void Graphics::SetViewport()
{
	_viewPort.TopLeftX = 0.f;
	_viewPort.TopLeftY = 0.f;
	_viewPort.Width = static_cast<float>(GWinSizeX);
	_viewPort.Height = static_cast<float>(GWinSizeY);
	_viewPort.MinDepth = 0.f;
	_viewPort.MaxDepth = 1.f;
}


#endif // �ּ�


