#pragma once
#include <memory>

class Graphics;
class SceneManager;
class Pipeline;
class InputManager;
class TimeManager;
class ResourceManager;

#if 1 // 주석
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

	shared_ptr<InputManager> GetInputManager() { return _input; }
	shared_ptr<TimeManager> GetTimeManager() { return _time; }
	shared_ptr<SceneManager> GetSceneManager() { return _scene; }
	shared_ptr<ResourceManager> GetResourceManager() { return _resource; }
	shared_ptr<Pipeline> GetPipeline() { return _pipeline; }

private:

	HWND _hwnd;

	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;


private:
	shared_ptr<InputManager> _input;
	shared_ptr<TimeManager> _time;
	shared_ptr<SceneManager> _scene;
	shared_ptr<ResourceManager> _resource;
};

extern unique_ptr<Game> GGame;




#endif // 주석



































































































#if 0 // Practice
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

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version,
		ComPtr<ID3DBlob>& blob);

private:
	HWND _hwnd;
	uint32 _width = 0;
	uint32 _height = 0;

private:
	// Device & SwanChain
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// RTV
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


#endif // Practice