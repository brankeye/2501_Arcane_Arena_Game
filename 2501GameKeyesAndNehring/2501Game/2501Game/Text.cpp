#include "Text.h"

Text::Text(void)
{
	float x, y, size;
	XMVECTOR color;
}

Text::Text(float xT, float yT, float sizeT, XMVECTOR colorT)
{
	x = xT;
	y = yT;
	size = sizeT;
	color = colorT;
}

void Text::drawText( const std::wstring& str, float x, float y, float size, XMVECTOR color, ID3D11DeviceContext* g_pImmediateContext, ID3D11Device* g_pd3dDevice )
{
	std::unique_ptr<SpriteBatch> c_Sprites;
    std::unique_ptr<SpriteFont> c_Font;
	c_Sprites.reset( new SpriteBatch( g_pImmediateContext ) );
	c_Font.reset( new SpriteFont( g_pd3dDevice, L"arial.spritefont" ) );
	c_Sprites->Begin( SpriteSortMode_Deferred );
	c_Font->DrawString( c_Sprites.get(), str.c_str(), XMFLOAT2( x, y ), color, 0.0f, XMFLOAT2( 0, 0 ), size, SpriteEffects_None, 0.0f );
	c_Sprites->End();
}
