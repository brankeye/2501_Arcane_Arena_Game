#include "Rect.h"

Rect::Rect(void)
{
}

Rect::~Rect(void)
{
}

void Rect::drawRect( float x, float y, float width, float height, XMVECTOR color, ID3D11DeviceContext* g_pImmediateContext, std::unique_ptr<BasicEffect>& g_BatchEffect, ID3D11InputLayout* g_pBatchInputLayout ) // ( x, y, width, height, color )
{
	g_BatchEffect->Apply( g_pImmediateContext );
    g_pImmediateContext->IASetInputLayout( g_pBatchInputLayout );
	std::unique_ptr<PrimitiveBatch<VertexPositionColor>> g_Batch;
	g_Batch.reset( new PrimitiveBatch<VertexPositionColor>( g_pImmediateContext ) );
	g_Batch->Begin();

	XMFLOAT2 pos1 = XMFLOAT2(x, y);                  // top left
    XMFLOAT2 pos2 = XMFLOAT2(x + width, y);          // top right
	XMFLOAT2 pos3 = XMFLOAT2(x + width, y - height); // bottom right
    XMFLOAT2 pos4 = XMFLOAT2(x, y - height);         // bottom left

	VertexPositionColor v1 = VertexPositionColor(XMLoadFloat2(&pos1), color);
    VertexPositionColor v2 = VertexPositionColor(XMLoadFloat2(&pos2), color);
	VertexPositionColor v3 = VertexPositionColor(XMLoadFloat2(&pos3), color);
	VertexPositionColor v4 = VertexPositionColor(XMLoadFloat2(&pos4), color);

	g_Batch->DrawQuad( v1, v2, v3, v4 );

	g_Batch->End();
}