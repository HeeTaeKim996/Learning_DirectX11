#pragma once
template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
		: _device(device), _deviceContext(deviceContext)
	{
	}
	~ConstantBuffer() {}
	
	ComPtr<ID3D11Buffer> GetComPtr() { return _constantBuffer; }

	inline void Create()
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
		CHECK(hr);

		/*
			○ D3D11_USAGE_DYNAMIC
			 - CPU_Write + GPU_Read
		*/
	}

	void CopyData(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		::memcpy(subResource.pData, &data, sizeof(data));
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

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<ID3D11Buffer> _constantBuffer;
};

