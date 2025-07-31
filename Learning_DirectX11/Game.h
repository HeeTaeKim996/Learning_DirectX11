#pragma once

#if 1 // �ּ�
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	void RenderBegin();
	void RenderEnd();



private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewPort();


private:
	void CreateGeometry();
	void CreateInputLayout();

	void CreateVS();
	void CreatePS();

	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();
	void CreateSRV();


	void CreateConstantBuffer();


	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);



private:
	HWND _hwnd;
	uint32 _width = 0;
	uint32 _height = 0;


private:
	// Device & SwapChain

	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	// �� ComPtr : ����Ʈ ������ Dx11 ����. #include <wrl.h> / using namespace Micosoft::WRL; �� ���
	// �� Device, DeviceContext : Ŀ�ǵ弾��
	// 
	// �� Device
	//  - Ŀ�ǵ� ����. ���� ����
	// 
	// �� DeviceContext
	//  - Device ���� ������� �͵��, Device�� �����ϴ� ����
	// 
	// 
	// 
	// �� IDXGI : GPU�� �ų� �����ϱ� ������, directx �� ������ ������Ʈ������, ȭ�� ��� �κ��� DXGI �� ������ ����� ������,
	//           ���Ұ� ������ ���� DXGI �� directX �� �ƴ� DXGI ���� ����
	// �� SwapChain	: WinAPI�� BitBlt, StetchBlt ó��, �ĸ���ۿ� �̸� �׸���, ������ۿ� ������ �����ϴ� ���
	//   �� �ĸ� ������ ������ 2�� �̻��� ���� �ְ�. ������ۿ� �ĸ���۰� ���� ������ �۵��ϴ� ��. SwapChain�� ����� �پ���
	// 


	// RTV (RenderTargetView)
	ComPtr<ID3D11RenderTargetView> _renderTargetView; // �� RenderTargetView : ������, �ĸ� ���۸� �����ϴ� ����


	// Misc
	D3D11_VIEWPORT _viewPort = { 0 }; // �� ȭ���� ũ�⸦ �����ϴ� ����
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f }; // R G B A



private:
	// Geometry
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;



	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;




	// RS
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;



	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;


	// SRV
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;


	// SamplerState
	ComPtr<ID3D11SamplerState> _samplerState = nullptr;

	// BlendState
	ComPtr<ID3D11BlendState> _blendState = nullptr;


private:
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;

};


#endif // �ּ�



































































































#if 0 // Practice
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hWnd);
	void Update();
	void Render();


private:
	void RenderBegin();
	void RenderEnd();




private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewPort();


private:
	void CreateGeometry();
	void CreateInputLayout();

	void CreateVS();
	void CreatePS();

	void CreateSRV();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);


private:
	HWND _hwnd;
	uint32 _width = 0;
	uint32 _height = 0;


private:
	// Device & SwapChain

	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;


	// RTV (Render Target View)
	ComPtr<ID3D11RenderTargetView> _renderTargetView;


	// Misc
	D3D11_VIEWPORT _viewPort = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

	
private:
	// Geometry
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;


	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;


	// SRV
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;
};




#endif // Practice