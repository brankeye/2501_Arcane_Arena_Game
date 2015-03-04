#pragma once

// DirectXTK includes
#include "Effects.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"

using namespace DirectX;

class Rect
{
public:
	Rect(void);
	~Rect(void);
	
	static void drawRect( float, float, float, float, XMVECTOR, ID3D11DeviceContext*, std::unique_ptr<BasicEffect>&, ID3D11InputLayout* ); // ( x, y, width, height, color )
};

