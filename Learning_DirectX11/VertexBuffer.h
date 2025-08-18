#pragma once

#if 1 // 주석
class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D11Device> device);
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() { return _vertexBuffer; }
	uint32 GetStride() { return _stride; }
	uint32 GetOffset() { return _offset; }
	uint32 GetCount() { return _count; }

	
public:
	template<typename T>
	inline void Create(const vector<T>& vertices)
	{
		_stride = sizeof(T);
		_count = static_cast<uint32>(vertices.size());

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(_stride * _count);

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data();


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


private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _vertexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};
#endif // 주석

