#include "pch.h"


#if 1 // 주석
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewPort();


	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	CreateSRV();
	CreateConstantBuffer();
}

void Game::Update()
{
	{ // _transformData

		_transformData.offset.x = 0.001f;
		_transformData.offset.y = 0.001f;
	}


	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
	_deviceContext->Unmap(_constantBuffer.Get(), 0);

	/*
		○ Map 과 Unmap
		 - GPU 리소스에 CPU 가 접근하여 데이터를 수정할 수 있도록 하는 API

		○ Map
		 - CPU 의 GPU 리소스 조작 시작
		 - (3) : D3D11_MAP_WRITE_DISCARD : CPU 가 새 데이터를 전부 덮어 씀
		 - (5) : CPU 에서 조작할 메모리 포인터

		○ Unmap
		 - CPU 의 GPU 리소스 조작 종료
	*/
}

void Game::Render()
{
	RenderBegin();


	{ // IA - VS - RS - PS -  OM

		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;


		// IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);




		/*
			○ IASetVertexBuffers
			 - GPU 에 넘길 정점 데이터 버퍼를 지정
			 - (4) : 정점 하나의 크기


			○ IASetIndexBuffer
			 - 준비한 인덱스 버퍼를 할당.
			 - DXGI_FORMAT_R32_UINT : 버퍼의 요소가, UINT32 로 구성됨을 알림


			○ IASetInputLayout
			 - 정점 버퍼에 들어있는 데이터가 어떤 의미인지를, 설정했던 _inputLayout 을 통해 알려주는 함수


			○ IASetPrimitiveTopology - D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			 -> Vertex들을 주면, Vertex들로 삼각형으로 인지해달라 라는 설정
		
		
		*/



		


		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());



		/*
			○ VSSetShader
			 - 앞서 만들었던 ComPtr<ID3D11VertexShader> _vertexShader 를 사용하여, GPU 가 정점을 처리하라 라는 함수
		
			○ VSSetConstantBuffers
			 - (1) : hlsl 의 cbuffer 슬롯 (b[n]) 인덱스
		
		*/




		// RS
		_deviceContext->RSSetState(_rasterizerState.Get());






		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		/*
			○ PSSetShader
			 - 앞서 만들었던 ComPtr<ID3D11PixelShader> _pixelShader 를 픽셸 셰이더로 사용하라 라는 함수

			○ PSSetShaderResources
			 - (1) : 저장할 슬롯 인덱스
			  ※ (1)에 저장하는 값 n 은, hlsl 의 Texture2D textureExample : register(n); 의 n 과 동일한 n이다.
			   -> 만약 (1)항을 3 으로 하고, hlsl 의 PS 에서 해당 텍스쳐를 사용하고 싶다면, Texture2D texure3 : reigster(3); 을 사용해야 함

		
		*/








		// OM

		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);


		
		//_deviceContext->Draw(_vertices.size(), 0); -->> DrawIndex 사용으로 이코드 미사용
		/*
			○ Draw
			 - 앞선 IA - VS - RS - PS 작업을을 바탕으로, 최종 결과물을 그려달라는 함수
			 ※ (1) 은 그릴 정점의 갯수를 인수로 입력하면 됨
		
		*/

		_deviceContext->DrawIndexed(_indices.size(), 0, 0);


	}


	RenderEnd();
}



void Game::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewPort);
	
	/*	○ OMSetRenderTargets
		 - 후면버퍼를 렌더 타겟으로 설정
		 - (1) : 후변버퍼 수

		 ○ ClearRenderTargetView
		 - WinAPI 의 BeginPaint의 마지막 작업인 WM_ERASEBKGND 와 유사하게, 배경을 모두 지정한 색사으로 덮어버려, 초기화하는 작업
		  ※ 초기화하는 대상은 전면버퍼가 아닌 후면버퍼

		 ○ RSSetViewports
		  - viewPort에 설정한 디스플레이 정보들을 전달. 인자에 정보를 전달받는 주체가 없다는 것에서 알 수 있듯이,
		    주체는 고유한 타입으로, Device 컨텍스트에 귀속된, 레스터라이저(Stage) 라 함.
	*/
}

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);

	/*	○ _swaChain->Present
		 - 앞선 RenderBegin에서 그린 후면버퍼를, 전면버퍼에 빠르게 복사하는 함수
	
	
	*/
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // ※ ZeroMemory == memtset(&desc, 0, sizeof(desc); ==>> (2)항이 0 고정
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
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

void Game::CreateRenderTargetView()
{

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());

	/*
		○ _swapChain -> GetBuffer ..
		 - _swapChain 의 0번 버퍼 를 backBuffer 에 할당

		○ CreateRenderTargetView ..
		 - backBuffer 를 바탕으로, GPU 가 렌더링 할 수 있는 View 를 생성해, _renderTargetView 에 저장
	*/
}

void Game::SetViewPort()
{
	_viewPort.TopLeftX = 0.f;
	_viewPort.TopLeftY = 0.f;
	_viewPort.Width = static_cast<float>(_width);
	_viewPort.Height = static_cast<float>(_height);
	_viewPort.MinDepth = 0.f;
	_viewPort.MaxDepth = 1.f;
}

void Game::CreateGeometry()
{
	{ // Vertex Data

		/*
		  1  3
		  0  2

		  형태로 indices (시계방향으로. 밑에서 위로 되도록. 0 1 2 , 2 1 3 
		*/


		_vertices.resize(4);
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0);
		//_vertices[0].color = Color(1.0f, 0.f, 0.f, 1.f);
		_vertices[0].uv = Vec2(0.f, 1.f);
		

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0);
		//_vertices[1].color = Color(0.0f, 1.f, 0.f, 1.f);
		_vertices[1].uv = Vec2(0.f, 0.f);


		_vertices[2].position = Vec3(0.5f, -0.5f, 0);
		//_vertices[2].color = Color(0.0f, 0.f, 1.f, 1.f);
		_vertices[2].uv = Vec2(1.f, 1.f);


		_vertices[3].position = Vec3(0.5f, 0.5f, 0);
		//_vertices[3].color = Color(1.0f, 1.f, 1.f, 1.f);
		_vertices[3].uv = Vec2(1.f, 0.f);



		/* 
			○ position 과 uv 의 기준축
			 - position 의 원점은 화면 정중앙. x 증가는 우측. y 증가는 위쪽
			 - uv 의 원즘은 화면 왼쪽 위. x 증가는 우측. y 증가내느 아래쪽
		*/
	}



	{ // Vertex Buffer
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();
		

		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);


		/*	○ DRD11_BUFFER_DESC
			 - CPU 는 RAM 에 메모리를 저장하는 반면, GPU 는 VRAM 에 메모리를 저장.
			  ( 위 vec 같은 메모리들은 모두 CPU 에 메모리 저장됨 )
			 - GPU의 VRAN에 메모리를 저장하려면, D3D11_BUFFER_DESC 같은 공간에, 메모리를 할당하고, 메모리 처리 필요
			 - D3D11_BUFFER_DESC 는 GPU 의 VRAM 에 VertexBuffer 등을 저장하기 위한 설정 구조체

			○ desc.Usage
			 - D3D11_BUFFER_DESC 메모리에 접근하는 방법에 관한 플래그
			 - D3D11_USAGE_IMUTABLE 이면, D3D11_BUFFER_DESC 에 GPU 는 읽기만 가능. 쓰기는 초기화 후 처리 못함. CPU는 접근 불가
			 - 그외에 D3D11_USAGE_DEFAULT 이면 GPU 는 읽기/쓰기 가능. CPU 는 간접 접근 가능
		
			○ desc.BindFlags
			 - D3D11_BUFFER_DESC 의 사용 용도를 지정하는 플래그
			 - D3D11_BIND_VERTEX_BUFFER		: 정점 을 그리는 데 메모리 사용
		


			○ CreateBuffer
			 - 위에 만든 D3D11_BUFFER_DESC (버퍼 서술 구조체) 와 D3D11_SUBRESOURCE_DATA ( _vertices 를 담은 구조체) 를 넘겨줘서,
			   (3) 인 ComPtr<ID3D11Buffer> 인 _vertexBuffer 에, GPU 의 VRAM 에 생성된 버퍼의 포인터를 넘김
		*/
	}

	

	{ // Index Data

		_indices = { 0, 1, 2, 2, 1, 3 };
	}

	{ // Index Buffer

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();

		 HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		 CHECK(hr);
	}
	
	
}

void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);


	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());



	/*
		○ D3D11_INPUT_ELEMENT_DESC
		 - 위의 _device->CreateBuffer 에 사용되는, D3D11_SUBRESOURCE_DATA 의 실데이터 struct 를 서술하는 구조체
		 - (1) : 셰이더 파일인 hlsl 에서의, VS_INPUT 구조체의 : 우측인 시멘틱(Sementic) 과 매칭되는 텍스트를 입력
		 - (3) : struct 필드멤버의 형식
		 - (4) : 구조체에서, 해당 필드멤버가 시작되는 포인터 위치. 위 코드에서는 POSITION 이 DXGI_FORMAT_R32G32B32 로
				  32 32 32 ==  4 + 4 + 4 = 12바이트 이므로, COLOR 의 시작 포인터 위치는 12
		 - (5) : 정점 데이터의 종류. 
				  - D3D11_INPUT_PER_VERTEX_DATA		: 각 정점마다 하나씩 존재하는 데이터
	
	*/
}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);
	
	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, 
		_vertexShader.GetAddressOf());
	CHECK(hr);

	/*
		○ LoadShaderFromFile
		 - 아래 정의한 사용자 정의 함수
		 - (1) : 98.Shaders 폴더 내에 위치한, Default.hlsl 파일명
		 - (2) : 엔트리 포인트(함수). Default.hlsl 우클릭 - 속성 - 일반 - 진입점 이름 에 설정한 함수명과 일치 필요
		 - (3) : 버전. Default.hlsl 우클릭 - 속성 - 일반 - 셰이더모델 에 설정한 값을, 특정 형식에 맞춰 입력해야함. 
		         (어떤 형식인지는 정확히 모르지만, 아무튼 지정한 5.0 버전에 맞춤..)
		 
		 - 위 함수 결과로, ComPtr<ID3DBlob> _vsblob 에 결과 포인터 전달 



		○ CreateVertexShader
		 - 위함수로 할당된 ComPtr<ID3DBlob> 을, ComPtr<ID3D11VertexShader> _vertexShader 에 내용을 전달하는 함수
	*/

}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr,
		_pixelShader.GetAddressOf());
	CHECK(hr);

	/* ○ 위에 설명한 주석 내용과 동일한 양식. 위는 DirectX11 렌더링 파이프라인의 VS 과정. 지금은 렌더링 파이프라인의 PS 과정
	
	*/

}

void Game::CreateRasterizerState()
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

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// TODO : 커스텀..

	_device->CreateSamplerState(&desc, _samplerState.GetAddressOf());

	/*
		○ D3D11_SAMPLER_DESC
		 - hlsh 의 PS 의 textureEX.Sample(samplerEx, input....); 의 Sample 을 설정
	
	*/
}

void Game::CreateBlendState()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// TODO : 커스텀..
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState(&desc, _blendState.GetAddressOf());

	/*
		○ BlendState
		 - PS 가 PixelShading 그린후, 작동. (OM 에서의 출력 직전 발동)
		 - 픽셀 셰이더에서 새로 출력한 색과, 기존 색 을 어떻게 조합할지를 결정
	*/
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	HRESULT hr = ::LoadFromWICFile(L"Harry.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);

	/*
		○ DirectX::TextMetadata
		 - 이미지의 형식 ( width, height, dpeth, format 등..)

		○ DirectX::ScratchImage
		 - 이미지의 픽셀 데이터

		○ ::LoadFromWICFile
		 - (1)의 이미지를 디코딩하여, md, img 에 데이터 할당
		
		○ ::CreateShaderResourceView
		 - img, md 정보를 ComPtr<ID3D11ShaderResourceView> _shaderResourceView 에 할당
	*/
}

void Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	CHECK(hr);

	/*
		○ D3D11_USAGE_DYNAMIC
		 - CPU_Write + GPU_Read
	*/
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, name.c_str(), version.c_str(), 
		compileFlag, 0, blob.GetAddressOf(), nullptr);
	CHECK(hr);


}



#endif // 주석





















































































#if 0 // Practice
#include "Game.h"

Game::Game()
{

}

Game::~Game()
{

}


void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewPort();


	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();

	CreateSRV();
}


void Game::Update()
{

}

void Game::Render()
{
	RenderBegin();



	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;


		// IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);


		// RS


		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());


		// OM
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}
	RenderEnd();
}


void Game::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewPort);
}


void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}


void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
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
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain
	(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &desc,
		_swapChain.GetAddressOf(), _device.GetAddressOf(), nullptr, _deviceContext.GetAddressOf());
	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
}


void Game::SetViewPort()
{
	_viewPort.TopLeftX = 0.f;
	_viewPort.TopLeftY = 0.f;
	_viewPort.Width = static_cast<float>(_width);
	_viewPort.Height = static_cast<float>(_height);
	_viewPort.MinDepth = 0.f;
	_viewPort.MaxDepth = 1.f;
}


void Game::CreateGeometry()
{
	{ // Vertex Data

		_vertices.resize(4);

		_vertices[0].position = Vec3(-0.5f, -0.5f, 0);
		_vertices[0].uv = Vec2(0, 1.f);

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0);
		_vertices[1].uv = Vec2(0, 0.f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0);
		_vertices[2].uv = Vec2(1.f, 1.f);

		_vertices[3].position = Vec3(0.5f, 0.5f, 0);
		_vertices[3].uv = Vec2(1.f, 0);
	}



	{ // Vertex Buffer
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _vertices.data();

		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}



	{ // Index Data

		_indices = { 0, 1, 2, 2, 1, 3 };
	}

	{ // Index Buffer
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());


		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = _indices.data();

		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		CHECK(hr);
	}
}


void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}


void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr,
		_vertexShader.GetAddressOf());
	CHECK(hr);
}


void Game::CreatePS()
{
	LoadShaderFromFile(L"default.hlsl", "PS", "ps_5_0", _psBlob);

	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr,
		_pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	HRESULT hr = ::LoadFromWICFile(L"Harry.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, name.c_str(), version.c_str(),
		compileFlag, 0, blob.GetAddressOf(), nullptr);
	CHECK(hr);
}


#endif // Practice