#pragma once

#if 0 // 주석
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
	// ※ ComPtr : 스마트 포인터 Dx11 버전. #include <wrl.h> / using namespace Micosoft::WRL; 로 사용
	// ※ Device, DeviceContext : 커맨드센터
	// 
	// ○ Device
	//  - 커맨드 센터. 생산 역할
	// 
	// ○ DeviceContext
	//  - Device 에서 만들어진 것들과, Device를 연동하는 역할
	// 
	// 
	// 
	// ※ IDXGI : GPU는 매년 발전하기 때문에, directx 가 빠르게 업데이트되지만, 화면 출력 부분의 DXGI 는 발전이 더디기 때문에,
	//           마소가 발전이 더딘 DXGI 는 directX 가 아닌 DXGI 에서 관리
	// ○ SwapChain	: WinAPI의 BitBlt, StetchBlt 처럼, 후면버퍼에 미리 그린후, 전면버퍼에 빠르게 복사하는 방법
	//   ※ 후면 버퍼의 갯수가 2개 이상일 수도 있고. 전면버퍼와 후면버퍼가 서로 번갈아 작동하는 등. SwapChain의 방법은 다양함
	// 


	// RTV (RenderTargetView)
	ComPtr<ID3D11RenderTargetView> _renderTargetView; // ※ RenderTargetView : 간략히, 후면 버퍼를 묘사하는 역할


	// Misc
	D3D11_VIEWPORT _viewPort = { 0 }; // ※ 화면의 크기를 묘사하는 역할
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f }; // R G B A



private:
	// Geometry
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;



	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;


	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;



};


#endif // 주석



































































































#if 1 // Practice
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
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;


	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;


	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;


};






















#endif // Practice