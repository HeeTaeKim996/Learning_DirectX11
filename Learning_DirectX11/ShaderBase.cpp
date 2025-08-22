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

	/* �� ���� ������ �ּ� ����� ������ ���. ���� DirectX11 ������ ������������ VS ����. ������ ������ ������������ PS ����
	*/
}
