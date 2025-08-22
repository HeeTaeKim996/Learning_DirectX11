#include "pch.h"
#include "ShaderBase.h"


/*------------------------------
			 Shader
------------------------------*/
ShaderBase::ShaderBase(ComPtr<ID3D11Device> device) : _device(device)
{
}

ShaderBase::~ShaderBase()
{
}

void ShaderBase::LoadShaderFromFile(const wstring& path, const string& name, const string& version)
{
	_path = path;
	_name = name;


	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, name.c_str(),
		version.c_str(), compileFlag, 0, _blob.GetAddressOf(), nullptr);
	CHECK(hr);
}




/*------------------------------
		  Vertex Shader
------------------------------*/
VertexShader::VertexShader(ComPtr<ID3D11Device> device) : Super(device)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);

	HRESULT hr = _device->CreateVertexShader(_blob->GetBufferPointer(),
		_blob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
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



/*------------------------------
		  Pixel Shader
------------------------------*/

PixelShader::PixelShader(ComPtr<ID3D11Device> device) : Super(device)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);

	HRESULT hr = _device->CreatePixelShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr,
		_pixelShader.GetAddressOf());
	CHECK(hr);

	/* ○ 위에 설명한 주석 내용과 동일한 양식. 위는 DirectX11 렌더링 파이프라인의 VS 과정. 지금은 렌더링 파이프라인의 PS 과정
	*/
}
