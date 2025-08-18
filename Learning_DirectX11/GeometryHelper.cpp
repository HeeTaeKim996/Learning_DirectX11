#include "pch.h"
#include "GeometryHelper.h"

#if 1 // 주석

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	vector<VertexTextureData> vertices;
	vertices.resize(4);

	vertices[0].position = Vec3(-0.5f, -0.5f, 0);
	vertices[0].uv = Vec2(0.f, 1.f);

	vertices[1].position = Vec3(-0.5f, 0.5f, 0);
	vertices[1].uv = Vec2(0.f, 0.f);

	vertices[2].position = Vec3(0.5f, -0.5f, 0);
	vertices[2].uv = Vec2(1.f, 1.f);

	vertices[3].position = Vec3(0.5f, 0.5f, 0);
	vertices[3].uv = Vec2(1.f, 0.f);

	geometry->SetVerticies(vertices);


	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(indices);

	/*
	1  3
	0  2

	형태로 indices (시계방향으로. 밑에서 위로 되도록. 0 1 2 , 2 1 3
	*/

	/*
	○ position 과 uv 의 기준축
	 - position 의 원점은 화면 정중앙. x 증가는 우측. y 증가는 위쪽
	 - uv 의 원즘은 화면 왼쪽 위. x 증가는 우측. y 증가내느 아래쪽
	*/
}

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	vector<VertexColorData> vertices;
	vertices.resize(4);

	vertices[0].position = Vec3(-0.5f, -0.5f, 0);
	vertices[0].color = color;
	

	vertices[1].position = Vec3(-0.5f, 0.5f, 0);
	vertices[1].color = color;
	

	vertices[2].position = Vec3(0.5f, -0.5f, 0);
	vertices[2].color = color;
	

	vertices[3].position = Vec3(0.5f, 0.5f, 0);
	vertices[3].color = color;
	
	geometry->SetVerticies(vertices);


	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(indices);
}


#endif // 주석
