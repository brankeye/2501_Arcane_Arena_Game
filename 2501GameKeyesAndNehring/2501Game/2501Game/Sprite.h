#pragma once

#include <directxmath.h>
#include <d3d11.h>

#include "./Inc/SpriteBatch.h"
#include "Effects.h"

using namespace DirectX;

class Sprite
{
public:
	Sprite(ID3D11Resource* spriteSheetResource, ID3D11ShaderResourceView* spriteSheet, 
		int numColumns, int numRows, int anAnimationFrameRate);

public:
	void Draw(SpriteBatch* spriteBatch, float gameTime, float alpha, XMFLOAT2 loc, float scale);
	void Draw(SpriteBatch* spriteBatch, float gameTime, float alpha, XMFLOAT2 loc, float scale, float xScale, float yScale);
	void         SetPosition(XMFLOAT2);
	void         SetPosition(float, float);

public:
    XMFLOAT2 speed; //X,Y speed components in pixels/update
    float rotation;  //rotation to be applied in radians
    XMFLOAT2 rotationOrigin; //location around which to rotate
	int getHeight();
	int getWidth();

protected:
	ID3D11ShaderResourceView* spriteSheet;

	int rows; //number of rows in sprite sheet
    int columns; //number of columns in sprite sheet
    int frameWidth; //the width of a single sprite image on sprite sheet
    int frameHeight; //height of a single sprite image on sprite sheet

    int currentRow;
    int currentColumn;
        
    int animationFrameRate; //rate at which sprite sheet frames should be advanced
	int lastRecordedTime; //number of milliseconds that had elapsed the last time we changed frames
};