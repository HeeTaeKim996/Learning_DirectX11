#pragma once

struct PipelineInfo
{
	shared_ptr<InputLayout> inputLayout;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<BlendState> blendState;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};


class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> deviceContext);
	~Pipeline();

	void UpdatePipeline(PipelineInfo info);

	void SetVertexBuffer(shared_ptr<VertexBuffer> buffer);
	void SetIndexBuffer(shared_ptr<IndexBuffer> buffer);

	template<typename T>
	inline void SetConstantBuffer(uint32 slot, uint32 scope, shared_ptr<ConstantBuffer<T>> buffer)
	{
		if (scope & SS_VertexShader)
		{
			_deviceContext->VSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
		}
		if (scope & SS_PixelShader)
		{
			_deviceContext->PSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
		}
		/*
			- slot : WinAPI 의 테이블 인덱스 번호와 같은 역할. 
				     GPU 에서 관리하는 ConstantBuffer 테이블에서 해당 인덱스의 콘스턴트 버퍼를 세팅할 때 사용하는 식별자
			- VSSetConstantBuffers 의 (2) 항인 1 : slot 의 인덱스부터, 몇개의 컨스턴트 버퍼를 바인딩할지. 보통 1.
		*/
	}


	void SetTexture(uint32 slot, uint32 scope, shared_ptr<Texture> texture);
	void SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState);


	void Draw(uint32 vertexCount, uint32 startVertexLocation);
	void DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation);

private:
	ComPtr<ID3D11DeviceContext> _deviceContext;

};

