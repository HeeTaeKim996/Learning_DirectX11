#pragma once
#include "Geometry.h"
#include "VertexData.h"

#if 1 // 주석
class GeometryHelper
{
public:
	static void CreateRectangle(shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color);

};

#endif // 주석

