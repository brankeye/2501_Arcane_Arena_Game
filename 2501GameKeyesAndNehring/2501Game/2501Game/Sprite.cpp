#include "Sprite.h"
#include "./Inc/CommonStates.h"

Sprite::Sprite(ID3D11Resource* aSpriteSheetResource, ID3D11ShaderResourceView* aSpriteSheet, int numColumns, int numRows, int anAnimationFrameRate)
{

	spriteSheet = aSpriteSheet;

	// Get height and width of spritesheet
	auto txt = reinterpret_cast<ID3D11Texture2D*>( aSpriteSheetResource );
	D3D11_TEXTURE2D_DESC desc;
	txt->GetDesc( &desc );

	float spriteSheetWidth = desc.Width;
	float spriteSheetHeight = desc.Height;

    currentRow = 0;
    currentColumn = 0;
    rows = numRows;
    columns = numColumns;
    frameWidth = spriteSheetWidth / columns;
    frameHeight = spriteSheetHeight / rows;
    animationFrameRate = anAnimationFrameRate;
    lastRecordedTime = 0;

    rotation = 0;
    rotationOrigin = XMFLOAT2(0, 0);
}



void Sprite::Draw(SpriteBatch* spriteBatch, float gameTime, float alpha, XMFLOAT2 loc, float scale)
{	
	RECT sourceFrame = { 
		currentColumn * frameWidth, 
		currentRow * frameHeight, 
		currentColumn * frameWidth + frameWidth, 
		currentRow * frameHeight + frameHeight
	};
	SpriteEffects directionFlip = SpriteEffects_None;
   // if (speed.x < 0) directionFlip = SpriteEffects_FlipHorizontally;
	

	spriteBatch->Draw( spriteSheet, loc, &sourceFrame, Colors::White * alpha, rotation, rotationOrigin, scale, directionFlip, 0.0f );
}

void Sprite::Draw(SpriteBatch* spriteBatch, float gameTime, float alpha, XMFLOAT2 loc, float scale, float xScale, float yScale)
{	
	RECT sourceFrame = { 
		0,
		0, 
		frameWidth*xScale, 
		frameHeight*yScale
	};
	SpriteEffects directionFlip = SpriteEffects_None;
   // if (speed.x < 0) directionFlip = SpriteEffects_FlipHorizontally;
	

	spriteBatch->Draw( spriteSheet, loc, &sourceFrame, Colors::White * alpha, rotation, rotationOrigin, scale, directionFlip, 0.0f );
}


int Sprite::getWidth(){
	return frameWidth;
}

int Sprite::getHeight(){
	return frameHeight;
}