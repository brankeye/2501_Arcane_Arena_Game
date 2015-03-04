#pragma once

// DirectXTK includes

#include "SpriteFont.h"

using namespace DirectX;

class Text
{
public:
	Text(void);
	Text(float x, float y, float size, XMVECTOR color);

	float x, y, size;
	XMVECTOR color;

	static void drawText( const std::wstring&, float, float, float, XMVECTOR, ID3D11DeviceContext*, ID3D11Device* );
};

