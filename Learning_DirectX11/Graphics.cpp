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

	/*	○ OMSetRenderTargets
		 - 후면버퍼를 렌더 타겟으로 설정
		 - (1) : 후변버퍼 수

		 ○ ClearRenderTargetView
		 - WinAPI 의 BeginPaint의 마지막 작업인 WM_ERASEBKGND 와 유사하게, 배경을 모두 지정한 색상으로 덮어버려, 초기화하는 작업
		  ※ 초기화하는 대상은 전면버퍼가 아닌 후면버퍼

		 ○ RSSetViewports
		  - viewPort에 설정한 디스플레이 정보들을 전달. 인자에 정보를 전달받는 주체가 없다는 것에서 알 수 있듯이,
			주체는 고유한 타입으로, Device 컨텍스트에 귀속된, 레스터라이저(Stage) 라 함.
	*/
}

void Graphics::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);

	/*	○ _swaChain->Present
		 - 앞선 RenderBegin에서 그린 후면버퍼를, 전면버퍼에 빠르게 복사하는 함수


	*/
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // ※ ZeroMemory == memtset(&desc, 0, sizeof(desc); ==>> (2)항이 0 고정
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
			○ Numerator 와 Denominator
			 - 주사율의 분자(Numerator) 와 분모(Denominator). 위는 60Hz

			○ SampleDesc.Count
			 - 계단 현상이 일어났을 때, 화면 해상도를 키운 후, 점들간 보간 으로 계단 현상을 줄이는데, Count 는 1일시 보간 미사용
			   (높일수록 더 넓은 화면 해상도로 만든 후 보간)

			○ BufferUsage
			 - desc (DXGI_SWAP_CHAIN_DESC ) 의 사용 용도.
			 - DXGI_USAGE_RENDER_TARGET_OUTPUT 일 경우, 렌더링 작업에서 최종 결과물을 그려주는 역할로 desc 를 사용하겠다는 의미

			○ BufferCount
			 - 후면버퍼의 개수. WinAPI 의 BitBlt 에서 hdc 와 MemDC 사용한 것처럼, 후면버퍼 (MemDC) 의 갯수
		*/
	}



	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &desc,
		_swapChain.GetAddressOf(), _device.GetAddressOf(), nullptr, _deviceContext.GetAddressOf());
	/*
		 - (2) : 렌더링 때 하드웨어인 그래픽카드를 사용. 만약 컴퓨터에 그래픽카드가 없다면, 다른 플래그 사용 필요
		 - (7) : 매크로 (버전)
		 - (8) : 앞선 코드에서 DXGI_SWP_CHAIN_DESC 를 설정 후 넘겨줌
		 ※ (8), (9), (11) : ** 를 리턴할 때 GetAddressOf 를 공통으로 사용.

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
		○ _swapChain -> GetBuffer ..
		 - _swapChain 의 0번 버퍼 를 backBuffer 에 할당

		○ CreateRenderTargetView ..
		 - backBuffer 를 바탕으로, GPU 가 렌더링 할 수 있는 View 를 생성해, _renderTargetView 에 저장
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


#endif // 주석


