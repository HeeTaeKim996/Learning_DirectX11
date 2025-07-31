#include "pch.h"


#if 1 // �ּ�
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
		�� Map �� Unmap
		 - GPU ���ҽ��� CPU �� �����Ͽ� �����͸� ������ �� �ֵ��� �ϴ� API

		�� Map
		 - CPU �� GPU ���ҽ� ���� ����
		 - (3) : D3D11_MAP_WRITE_DISCARD : CPU �� �� �����͸� ���� ���� ��
		 - (5) : CPU ���� ������ �޸� ������

		�� Unmap
		 - CPU �� GPU ���ҽ� ���� ����
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
			�� IASetVertexBuffers
			 - GPU �� �ѱ� ���� ������ ���۸� ����
			 - (4) : ���� �ϳ��� ũ��


			�� IASetIndexBuffer
			 - �غ��� �ε��� ���۸� �Ҵ�.
			 - DXGI_FORMAT_R32_UINT : ������ ��Ұ�, UINT32 �� �������� �˸�


			�� IASetInputLayout
			 - ���� ���ۿ� ����ִ� �����Ͱ� � �ǹ�������, �����ߴ� _inputLayout �� ���� �˷��ִ� �Լ�


			�� IASetPrimitiveTopology - D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			 -> Vertex���� �ָ�, Vertex��� �ﰢ������ �����ش޶� ��� ����
		
		
		*/



		


		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());



		/*
			�� VSSetShader
			 - �ռ� ������� ComPtr<ID3D11VertexShader> _vertexShader �� ����Ͽ�, GPU �� ������ ó���϶� ��� �Լ�
		
			�� VSSetConstantBuffers
			 - (1) : hlsl �� cbuffer ���� (b[n]) �ε���
		
		*/




		// RS
		_deviceContext->RSSetState(_rasterizerState.Get());






		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		/*
			�� PSSetShader
			 - �ռ� ������� ComPtr<ID3D11PixelShader> _pixelShader �� �ȼ� ���̴��� ����϶� ��� �Լ�

			�� PSSetShaderResources
			 - (1) : ������ ���� �ε���
			  �� (1)�� �����ϴ� �� n ��, hlsl �� Texture2D textureExample : register(n); �� n �� ������ n�̴�.
			   -> ���� (1)���� 3 ���� �ϰ�, hlsl �� PS ���� �ش� �ؽ��ĸ� ����ϰ� �ʹٸ�, Texture2D texure3 : reigster(3); �� ����ؾ� ��

		
		*/








		// OM

		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);


		
		//_deviceContext->Draw(_vertices.size(), 0); -->> DrawIndex ������� ���ڵ� �̻��
		/*
			�� Draw
			 - �ռ� IA - VS - RS - PS �۾����� ��������, ���� ������� �׷��޶�� �Լ�
			 �� (1) �� �׸� ������ ������ �μ��� �Է��ϸ� ��
		
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

void Game::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);

	/*	�� _swaChain->Present
		 - �ռ� RenderBegin���� �׸� �ĸ���۸�, ������ۿ� ������ �����ϴ� �Լ�
	
	
	*/
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // �� ZeroMemory == memtset(&desc, 0, sizeof(desc); ==>> (2)���� 0 ����
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

void Game::CreateRenderTargetView()
{

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());

	/*
		�� _swapChain -> GetBuffer ..
		 - _swapChain �� 0�� ���� �� backBuffer �� �Ҵ�

		�� CreateRenderTargetView ..
		 - backBuffer �� ��������, GPU �� ������ �� �� �ִ� View �� ������, _renderTargetView �� ����
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

		  ���·� indices (�ð��������. �ؿ��� ���� �ǵ���. 0 1 2 , 2 1 3 
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
			�� position �� uv �� ������
			 - position �� ������ ȭ�� ���߾�. x ������ ����. y ������ ����
			 - uv �� ������ ȭ�� ���� ��. x ������ ����. y �������� �Ʒ���
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


		/*	�� DRD11_BUFFER_DESC
			 - CPU �� RAM �� �޸𸮸� �����ϴ� �ݸ�, GPU �� VRAM �� �޸𸮸� ����.
			  ( �� vec ���� �޸𸮵��� ��� CPU �� �޸� ����� )
			 - GPU�� VRAN�� �޸𸮸� �����Ϸ���, D3D11_BUFFER_DESC ���� ������, �޸𸮸� �Ҵ��ϰ�, �޸� ó�� �ʿ�
			 - D3D11_BUFFER_DESC �� GPU �� VRAM �� VertexBuffer ���� �����ϱ� ���� ���� ����ü

			�� desc.Usage
			 - D3D11_BUFFER_DESC �޸𸮿� �����ϴ� ����� ���� �÷���
			 - D3D11_USAGE_IMUTABLE �̸�, D3D11_BUFFER_DESC �� GPU �� �б⸸ ����. ����� �ʱ�ȭ �� ó�� ����. CPU�� ���� �Ұ�
			 - �׿ܿ� D3D11_USAGE_DEFAULT �̸� GPU �� �б�/���� ����. CPU �� ���� ���� ����
		
			�� desc.BindFlags
			 - D3D11_BUFFER_DESC �� ��� �뵵�� �����ϴ� �÷���
			 - D3D11_BIND_VERTEX_BUFFER		: ���� �� �׸��� �� �޸� ���
		


			�� CreateBuffer
			 - ���� ���� D3D11_BUFFER_DESC (���� ���� ����ü) �� D3D11_SUBRESOURCE_DATA ( _vertices �� ���� ����ü) �� �Ѱ��༭,
			   (3) �� ComPtr<ID3D11Buffer> �� _vertexBuffer ��, GPU �� VRAM �� ������ ������ �����͸� �ѱ�
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
		�� D3D11_INPUT_ELEMENT_DESC
		 - ���� _device->CreateBuffer �� ���Ǵ�, D3D11_SUBRESOURCE_DATA �� �ǵ����� struct �� �����ϴ� ����ü
		 - (1) : ���̴� ������ hlsl ������, VS_INPUT ����ü�� : ������ �ø�ƽ(Sementic) �� ��Ī�Ǵ� �ؽ�Ʈ�� �Է�
		 - (3) : struct �ʵ����� ����
		 - (4) : ����ü����, �ش� �ʵ����� ���۵Ǵ� ������ ��ġ. �� �ڵ忡���� POSITION �� DXGI_FORMAT_R32G32B32 ��
				  32 32 32 ==  4 + 4 + 4 = 12����Ʈ �̹Ƿ�, COLOR �� ���� ������ ��ġ�� 12
		 - (5) : ���� �������� ����. 
				  - D3D11_INPUT_PER_VERTEX_DATA		: �� �������� �ϳ��� �����ϴ� ������
	
	*/
}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);
	
	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, 
		_vertexShader.GetAddressOf());
	CHECK(hr);

	/*
		�� LoadShaderFromFile
		 - �Ʒ� ������ ����� ���� �Լ�
		 - (1) : 98.Shaders ���� ���� ��ġ��, Default.hlsl ���ϸ�
		 - (2) : ��Ʈ�� ����Ʈ(�Լ�). Default.hlsl ��Ŭ�� - �Ӽ� - �Ϲ� - ������ �̸� �� ������ �Լ���� ��ġ �ʿ�
		 - (3) : ����. Default.hlsl ��Ŭ�� - �Ӽ� - �Ϲ� - ���̴��� �� ������ ����, Ư�� ���Ŀ� ���� �Է��ؾ���. 
		         (� ���������� ��Ȯ�� ������, �ƹ�ư ������ 5.0 ������ ����..)
		 
		 - �� �Լ� �����, ComPtr<ID3DBlob> _vsblob �� ��� ������ ���� 



		�� CreateVertexShader
		 - ���Լ��� �Ҵ�� ComPtr<ID3DBlob> ��, ComPtr<ID3D11VertexShader> _vertexShader �� ������ �����ϴ� �Լ�
	*/

}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr,
		_pixelShader.GetAddressOf());
	CHECK(hr);

	/* �� ���� ������ �ּ� ����� ������ ���. ���� DirectX11 ������ ������������ VS ����. ������ ������ ������������ PS ����
	
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

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// TODO : Ŀ����..

	_device->CreateSamplerState(&desc, _samplerState.GetAddressOf());

	/*
		�� D3D11_SAMPLER_DESC
		 - hlsh �� PS �� textureEX.Sample(samplerEx, input....); �� Sample �� ����
	
	*/
}

void Game::CreateBlendState()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// TODO : Ŀ����..
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState(&desc, _blendState.GetAddressOf());

	/*
		�� BlendState
		 - PS �� PixelShading �׸���, �۵�. (OM ������ ��� ���� �ߵ�)
		 - �ȼ� ���̴����� ���� ����� ����, ���� �� �� ��� ���������� ����
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
		�� DirectX::TextMetadata
		 - �̹����� ���� ( width, height, dpeth, format ��..)

		�� DirectX::ScratchImage
		 - �̹����� �ȼ� ������

		�� ::LoadFromWICFile
		 - (1)�� �̹����� ���ڵ��Ͽ�, md, img �� ������ �Ҵ�
		
		�� ::CreateShaderResourceView
		 - img, md ������ ComPtr<ID3D11ShaderResourceView> _shaderResourceView �� �Ҵ�
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
		�� D3D11_USAGE_DYNAMIC
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



#endif // �ּ�





















































































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