#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(ComPtr<ID3D11DeviceContext> deviceContext) : _deviceContext(deviceContext)
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::UpdatePipeline(PipelineInfo info)
{
	// IA - VS - RS - PS -  OM

	// IA
	_deviceContext->IASetInputLayout(info.inputLayout->GetComPtr().Get());
	_deviceContext->IASetPrimitiveTopology(info.topology);
	/*
		○ IASetVertexBuffers
		 - GPU 에 넘길 정점 데이터 버퍼를 지정
		 - (4) : 정점 하나의 크기


		○ IASetIndexBuffer
		 - 준비한 인덱스 버퍼를 할당.
		 - DXGI_FORMAT_R32_UINT : 버퍼의 요소가, UINT32 로 구성됨을 알림


		○ IASetInputLayout
		 - 정점 버퍼에 들어있는 데이터가 어떤 의미인지를, 설정했던 _inputLayout 을 통해 알려주는 함수


		○ IASetPrimitiveTopology - D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		 -> Vertex들을 주면, Vertex들로 삼각형으로 인지해달라 라는 설정
	*/


	// VS
	if (info.vertexShader)
	{
		_deviceContext->VSSetShader(info.vertexShader->GetComPtr().Get(), nullptr, 0);
	}
	/*
		○ VSSetShader
		 - 앞서 만들었던 ComPtr<ID3D11VertexShader> _vertexShader 를 사용하여, GPU 가 정점을 처리하라 라는 함수

		○ VSSetConstantBuffers
		 - (1) : hlsl 의 cbuffer 슬롯 (b[n]) 인덱스

	*/


	// RS
	if (info.rasterizerState)
	{
		_deviceContext->RSSetState(info.rasterizerState->GetComPtr().Get());
	}


	// PS
	if (info.pixelShader)
	{
		_deviceContext->PSSetShader(info.pixelShader->GetComPtr().Get(), nullptr, 0);
	}
	/*
		○ PSSetShader
		 - 앞서 만들었던 ComPtr<ID3D11PixelShader> _pixelShader 를 픽셸 셰이더로 사용하라 라는 함수

		○ PSSetShaderResources
		 - (1) : 저장할 슬롯 인덱스
		  ※ (1)에 저장하는 값 n 은, hlsl 의 Texture2D textureExample : register(n); 의 n 과 동일한 n이다.
		   -> 만약 (1)항을 3 으로 하고, hlsl 의 PS 에서 해당 텍스쳐를 사용하고 싶다면, Texture2D texure3 : reigster(3); 을 사용해야 함


	*/


	// OM
	if (info.blendState)
	{
		_deviceContext->OMSetBlendState(info.blendState->GetComPtr().Get(), info.blendState->GetBlendFactor(),
			info.blendState->GetSampleMask());
	}
	//_deviceContext->Draw(_vertices.size(), 0); -->> DrawIndex 사용으로 이코드 미사용
	/*
		○ Draw
		 - 앞선 IA - VS - RS - PS 작업을을 바탕으로, 최종 결과물을 그려달라는 함수
		 ※ (1) 은 그릴 정점의 갯수를 인수로 입력하면 됨

	*/
}

void Pipeline::SetVertexBuffer(shared_ptr<VertexBuffer> buffer)
{
	uint32 stride = buffer->GetStride();
	uint32 offset = buffer->GetOffset();
	_deviceContext->IASetVertexBuffers(0, 1, buffer->GetComPtr().GetAddressOf(), &stride, &offset);
}

void Pipeline::SetIndexBuffer(shared_ptr<IndexBuffer> buffer)
{
	_deviceContext->IASetIndexBuffer(buffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Pipeline::SetTexture(uint32 slot, uint32 scope, shared_ptr<Texture> texture)
{
	if (scope & SS_VertexShader)
	{
		_deviceContext->VSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());
	}
	if (scope & SS_PixelShader)
	{
		_deviceContext->PSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());
	}
}

void Pipeline::SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState)
{
	if (scope & SS_VertexShader)
	{
		_deviceContext->VSSetSamplers(slot, 1, samplerState->GetComPtr().GetAddressOf());
	}
	if (scope & SS_PixelShader)
	{
		_deviceContext->PSSetSamplers(slot, 1, samplerState->GetComPtr().GetAddressOf());
	}
}

void Pipeline::Draw(uint32 vertexCount, uint32 startVertexLocation)
{
	_deviceContext->Draw(vertexCount, startVertexLocation);
}

void Pipeline::DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation)
{
	_deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
