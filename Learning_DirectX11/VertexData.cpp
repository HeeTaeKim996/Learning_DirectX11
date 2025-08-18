#include "pch.h"
#include "VertexData.h"

vector<D3D11_INPUT_ELEMENT_DESC> VertexTextureData::descs =
{

		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}

		/*
		○ D3D11_INPUT_ELEMENT_DESC
		 - 위의 _device->CreateBuffer 에 사용되는, D3D11_SUBRESOURCE_DATA 의 실데이터 struct 를 서술하는 구조체
		 - (1) : 셰이더 파일인 hlsl 에서의, VS_INPUT 구조체의 : 우측인 시멘틱(Sementic) 과 매칭되는 텍스트를 입력
		 - (3) : struct 필드멤버의 형식
		 - (4) : 구조체에서, 해당 필드멤버가 시작되는 포인터 위치. 위 코드에서는 POSITION 이 DXGI_FORMAT_R32G32B32 로
				  32 32 32 ==  4 + 4 + 4 = 12바이트 이므로, COLOR 의 시작 포인터 위치는 12
				  ※ D3D11_APPEND_ALIGNED_ELEMENT : (4)항에 입력될 정수를 자동 계산

		 - (5) : 정점 데이터의 종류.
				  - D3D11_INPUT_PER_VERTEX_DATA		: 각 정점마다 하나씩 존재하는 데이터

		*/
};

vector<D3D11_INPUT_ELEMENT_DESC> VertexColorData
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
	D3D11_INPUT_PER_VERTEX_DATA, 0},
};
