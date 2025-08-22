#include "pch.h"
#include "Texture.h"

Texture::Texture(ComPtr<ID3D11Device> device) : Super(ResourceType::Texture), _device(device)
{
}

Texture::~Texture()
{
}

void Texture::Create(const wstring& path)
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md,
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
