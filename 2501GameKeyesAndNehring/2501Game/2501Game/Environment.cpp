//
//
//

#include "Environment.h"

Environment::Environment(XMFLOAT2 pos, Sprite* m, Sprite* w, Sprite* c){
	map = m;
	water = w;
	clouds = c;
	screenRes = pos;
	waterheight = 0;
	cloudwidth = 0;
	drawing = true;
}

Environment::~Environment(void){
}

void Environment::Draw(SpriteBatch* spriteBatch, long int gameTime){
	if(drawing){
		waterheight+= 0.10f;
		if(waterheight >= screenRes.y){
			waterheight-=screenRes.y;
		}
		water->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(screenRes.x/2-128,waterheight), 1.0f);
		water->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(screenRes.x/2-128,waterheight-screenRes.y), 1.0f);

		map->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(0,0), 1.0f);

		cloudwidth+= 0.05f;
		if(cloudwidth >= screenRes.x){
			cloudwidth-=screenRes.x;
		}
		clouds->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(cloudwidth,0), 1.0f);
		clouds->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(cloudwidth-screenRes.x,0), 1.0f);
		
	}
}

bool Environment::IsDrawing(){
	return drawing;
}