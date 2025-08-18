#include "pch.h"
#include "Graphics.h"

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

	_graphics = make_shared<Graphics>(hwnd);
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());

	_geometry = make_shared<Geometry<VertexTextureData>>();
	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), 
		_graphics->GetDeviceContext());

	// Geometry
	{
		{ // Vertex Data
			GeometryHelper::CreateRectangle(_geometry);
		}


		{ // Vertex Buffer
			_vertexBuffer->Create(_geometry->GetVertices());
		}


		{ // Index Buffer
			_indexBuffer->Create(_geometry->GetIndices());
		}
	}

	// VertexShader
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");


	// InputLayout
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());


	// PixelShader
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");



	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	CreateSRV();

	_constantBuffer->Create();
}

void Game::Update()
{
	{ // _transformData

		_localPosition.x += 0.0001f;

		Matrix matScale = Matrix::CreateScale(_localScale / 3.f);
		Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
		matRotation *= Matrix::CreateRotationY(_localRotation.y);
		matRotation *= Matrix::CreateRotationZ(_localRotation.z);
		Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

		Matrix matWorld = matScale * matRotation * matTranslation;
		_transformData.matWorld = matWorld;
	}

	_constantBuffer->CopyData(_transformData);
}

void Game::Render()
{
	_graphics->RenderBegin();


	{ // IA - VS - RS - PS -  OM

		uint32 stride = sizeof(VertexTextureData);
		uint32 offset = 0;

		auto _deviceContext = _graphics->GetDeviceContext(); // 임시로 이렇게 처리

		// IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, 
			&offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout->GetComPtr().Get());
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
		_deviceContext->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer->GetComPtr().GetAddressOf());



		/*
			○ VSSetShader
			 - 앞서 만들었던 ComPtr<ID3D11VertexShader> _vertexShader 를 사용하여, GPU 가 정점을 처리하라 라는 함수
		
			○ VSSetConstantBuffers
			 - (1) : hlsl 의 cbuffer 슬롯 (b[n]) 인덱스
		
		*/




		// RS
		_deviceContext->RSSetState(_rasterizerState.Get());






		// PS
		_deviceContext->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);
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

		_deviceContext->DrawIndexed(_geometry->GetIndexCount(), 0, 0);


	}


	_graphics->RenderEnd();
}




void Game::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;

	HRESULT hr = _graphics->GetDevice()->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
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

	_graphics->GetDevice()->CreateSamplerState(&desc, _samplerState.GetAddressOf());

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

	_graphics->GetDevice()->CreateBlendState(&desc, _blendState.GetAddressOf());

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

	hr = ::CreateShaderResourceView(_graphics->GetDevice().Get(), img.GetImages(), img.GetImageCount(), md,
		_shaderResourceView.GetAddressOf());
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

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	CreateSRV();
	CreateConstantBuffer();
}

void Game::Update()
{
	{ // _transformData
		_transformData.offset.x += 0.001f;
		_transformData.offset.y += 0.001f;
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
	_deviceContext->Unmap(_constantBuffer.Get(), 0);
}

void Game::Render()
{
	RenderBegin();

	{ // IA - VS - RS - PS - OM

		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;


		// IA
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());


		// RS
		_deviceContext->RSSetState(_rasterizerState.Get());


		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());


		// OM
		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);
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

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &desc,
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
		_vertices[0].uv = Vec2(0.f, 1.f);

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0);
		_vertices[1].uv = Vec2(0.f, 0.f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0);
		_vertices[2].uv = Vec2(1.f, 1.f);

		_vertices[3].position = Vec3(0.5f, 0.5f, 0);
		_vertices[3].uv = Vec2(1.f, 0.f);
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

	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(),
		_inputLayout.GetAddressOf());
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
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr,
		_pixelShader.GetAddressOf());
	CHECK(hr);
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
}

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// TODO : 커스텀

	_device->CreateSamplerState(&desc, _samplerState.GetAddressOf());
}

void Game::CreateBlendState()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// TODO : 커스텀

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState(&desc, _blendState.GetAddressOf());
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	HRESULT hr = ::LoadFromWICFile(L"Harry.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md,
		_shaderResourceView.GetAddressOf());
	CHECK(hr);
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
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version,
	ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, name.c_str(),
		version.c_str(), compileFlag, 0, blob.GetAddressOf(), nullptr);
	CHECK(hr);
}

#endif // Practice