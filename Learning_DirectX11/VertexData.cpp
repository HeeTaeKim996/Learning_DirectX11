#include "pch.h"
#include "VertexData.h"

vector<D3D11_INPUT_ELEMENT_DESC> VertexTextureData::descs =
{

		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}

		/*
		�� D3D11_INPUT_ELEMENT_DESC
		 - ���� _device->CreateBuffer �� ���Ǵ�, D3D11_SUBRESOURCE_DATA �� �ǵ����� struct �� �����ϴ� ����ü
		 - (1) : ���̴� ������ hlsl ������, VS_INPUT ����ü�� : ������ �ø�ƽ(Sementic) �� ��Ī�Ǵ� �ؽ�Ʈ�� �Է�
		 - (3) : struct �ʵ����� ����
		 - (4) : ����ü����, �ش� �ʵ����� ���۵Ǵ� ������ ��ġ. �� �ڵ忡���� POSITION �� DXGI_FORMAT_R32G32B32 ��
				  32 32 32 ==  4 + 4 + 4 = 12����Ʈ �̹Ƿ�, COLOR �� ���� ������ ��ġ�� 12
				  �� D3D11_APPEND_ALIGNED_ELEMENT : (4)�׿� �Էµ� ������ �ڵ� ���

		 - (5) : ���� �������� ����.
				  - D3D11_INPUT_PER_VERTEX_DATA		: �� �������� �ϳ��� �����ϴ� ������

		*/
};

vector<D3D11_INPUT_ELEMENT_DESC> VertexColorData
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
	D3D11_INPUT_PER_VERTEX_DATA, 0},
};
