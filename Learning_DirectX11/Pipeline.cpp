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
		�� IASetVertexBuffers
		 - GPU �� �ѱ� ���� ������ ���۸� ����
		 - (4) : ���� �ϳ��� ũ��


		�� IASetIndexBuffer
		 - �غ��� �ε��� ���۸� �Ҵ�.
		 - DXGI_FORMAT_R32_UINT : ������ ��Ұ�, UINT32 �� �������� �˸�


		�� IASetInputLayout
		 - ���� ���ۿ� ����ִ� �����Ͱ� � �ǹ�������, �����ߴ� _inputLayout �� ���� �˷��ִ� �Լ�


		�� IASetPrimitiveTopology - D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		 -> Vertex���� �ָ�, Vertex��� �ﰢ������ �����ش޶� ��� ����
	*/


	// VS
	if (info.vertexShader)
	{
		_deviceContext->VSSetShader(info.vertexShader->GetComPtr().Get(), nullptr, 0);
	}
	/*
		�� VSSetShader
		 - �ռ� ������� ComPtr<ID3D11VertexShader> _vertexShader �� ����Ͽ�, GPU �� ������ ó���϶� ��� �Լ�

		�� VSSetConstantBuffers
		 - (1) : hlsl �� cbuffer ���� (b[n]) �ε���

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
		�� PSSetShader
		 - �ռ� ������� ComPtr<ID3D11PixelShader> _pixelShader �� �ȼ� ���̴��� ����϶� ��� �Լ�

		�� PSSetShaderResources
		 - (1) : ������ ���� �ε���
		  �� (1)�� �����ϴ� �� n ��, hlsl �� Texture2D textureExample : register(n); �� n �� ������ n�̴�.
		   -> ���� (1)���� 3 ���� �ϰ�, hlsl �� PS ���� �ش� �ؽ��ĸ� ����ϰ� �ʹٸ�, Texture2D texure3 : reigster(3); �� ����ؾ� ��


	*/


	// OM
	if (info.blendState)
	{
		_deviceContext->OMSetBlendState(info.blendState->GetComPtr().Get(), info.blendState->GetBlendFactor(),
			info.blendState->GetSampleMask());
	}
	//_deviceContext->Draw(_vertices.size(), 0); -->> DrawIndex ������� ���ڵ� �̻��
	/*
		�� Draw
		 - �ռ� IA - VS - RS - PS �۾����� ��������, ���� ������� �׷��޶�� �Լ�
		 �� (1) �� �׸� ������ ������ �μ��� �Է��ϸ� ��

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
