#pragma once

#if 1 // �ּ�
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


private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _vertexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};
#endif // �ּ�

