#include "player.hpp"

#include "player_sheet.h"

#include <stdio.h>

static const int feet_pos[112] 
{
	30 % 64 - 32, 49 % 64 - 32, 
	32 % 64 - 32, 49 % 64 - 32,
	94 % 64 - 32, 49 % 64 - 32,
	96 % 64 - 32, 49 % 64 - 32,
	158 % 64 - 32, 49 % 64 - 32,
	160 % 64 - 32, 49 % 64 - 32,
	222 % 64 - 32, 42 % 64 - 32,
	224 % 64 - 32, 54 % 64 - 32,
	286 % 64 - 32, 54 % 64 - 32,
	288 % 64 - 32, 43 % 64 - 32,
	342 % 64 - 32, 49 % 64 - 32,
	371 % 64 - 32, 46 % 64 - 32,
	397 % 64 - 32, 48 % 64 - 32,
	426 % 64 - 32, 48 % 64 - 32,
	
	30 % 64 - 32, 112 % 64 - 32,
	32 % 64 - 32, 112 % 64 - 32,
	94 % 64 - 32, 112 % 64 - 32,
	96 % 64 - 32, 112 % 64 - 32,
	158 % 64 - 32, 112 % 64 - 32,
	160 % 64 - 32, 112 % 64 - 32,
	222 % 64 - 32, 108 % 64 - 32,
	224 % 64 - 32, 117 % 64 - 32,
	286 % 64 - 32, 118 % 64 - 32,
	287 % 64 - 32, 109 % 64 - 32,
	332 % 64 - 32, 110 % 64 - 32,
	360 % 64 - 32, 112 % 64 - 32,
	404 % 64 - 32, 111 % 64 - 32,
	433 % 64 - 32, 110 % 64 - 32,
	
	31 % 64 - 32, 174 % 64 - 32,
	31 % 64 - 32, 176 % 64 - 32,
	95 % 64 - 32, 174 % 64 - 32,
	95 % 64 - 32, 176 % 64 - 32,
	159 % 64 - 32, 174 % 64 - 32,
	159 % 64 - 32, 176 % 64 - 32,
	216 % 64 - 32, 176 % 64 - 32,
	233 % 64 - 32, 174 % 64 - 32,
	280 % 64 - 32, 175 % 64 - 32,
	296 % 64 - 32, 176 % 64 - 32,
	350 % 64 - 32, 187 % 64 - 32,
	350 % 64 - 32, 170 % 64 - 32,
	415 % 64 - 32, 183 % 64 - 32,
	414 % 64 - 32, 170 % 64 - 32,
	
	31 % 64 - 32, 237 % 64 - 32,
	31 % 64 - 32, 240 % 64 - 32,
	95 % 64 - 32, 237 % 64 - 32,
	95 % 64 - 32, 240 % 64 - 32,
	159 % 64 - 32, 237 % 64 - 32,
	159 % 64 - 32, 240 % 64 - 32,
	213 % 64 - 32, 240 % 64 - 32,
	230 % 64 - 32, 239 % 64 - 32,
	278 % 64 - 32, 238 % 64 - 32,
	295 % 64 - 32, 240 % 64 - 32,
	352 % 64 - 32, 233 % 64 - 32,
	351 % 64 - 32, 245 % 64 - 32,
	415 % 64 - 32, 233 % 64 - 32,
	415 % 64 - 32, 251 % 64 - 32,
};

Player::Player() : Sprite(ObjSize::SIZE_64x64, ObjBitDepth::_8bit, 1, $(player))
{	
	dmaCopy(player_sheetPal, SPRITE_PALETTE, player_sheetPalLen);	
	gfx_addr.set_value((void*)0x06010080);
	set_current_frame(0,0);		
	
	get_attribute()->set_priority(0);		
	
	get_visual()->set_frame(0, &gfx_addr);
	get_visual()->set_crt_gfx(0);	
	set_anchor(128,128);	
	//set_anchor(ANCHOR_CENTER);	
	
	px = 120;
	py = 80;
	
	update_visual();
}

void Player::set_movement_bounds(int x, int y, int w, int h)
{
	bndx = x;
	bndy = y;
	bndw = w;
	bndh = h;
}

void Player::set_current_frame(int orientation, int pos_index)
{
	this->orientation = orientation;
	this->pos_index = pos_index;
	
	u8* src = ((u8*)player_sheetTiles) + 64*64*(7*orientation + pos_index);
	gfx_addr.write(src, 64*64);
}

#include <stdio.h>

void Player::move(sf24 dx, sf24 dy)
{
	if(falling_scale>0) return;
	ax=dx; 
	ay=dy;		
	
	if(vx==0 && vy==0)
		crt_frames = frames_start_skate;
	
	if(dx<0) 
	{
		if(vx.abs()>vy.abs())
		{			
			set_current_frame(PLAYER_LEFT, pos_index);										
		}
		return;
	}		
	if(dx>0) 
	{	
		if(vx.abs()>vy.abs())
			set_current_frame(PLAYER_RIGHT, pos_index);					
		return;
	}			
	if(dy<0)
	{
		if(vy.abs()>vx.abs())
			set_current_frame(PLAYER_BACK, pos_index);					
		return;		
	}		
	if(dy>0)
	{
		if(vy.abs()>vx.abs())
			set_current_frame(PLAYER_FRONT, pos_index);
		return;		
	}					
}


sf24 to_0(sf24 val, sf24 amount) {
		if(amount<0) amount=-amount;
		if(val<0) {
			val+=amount;
			if(val>0) val=0;
		}
		else if(val>0){
			val-=amount;
			if(val<0) val=0;
		}
		return val;
	}
	
void Player::frameset_standby() 
{
	if(crt_frames == frames_standby) return;
	crt_frames = frames_standby;
	crt_frames_len = 2;
	crt_frame_index = 0;		
	cooldown = 50;
	crt_cooldown = 0;
}

void Player::frameset_normal()
{
	if(crt_frames == frames_normal_skate) return;
	crt_frames = frames_normal_skate;
	crt_frames_len = 4;
	crt_frame_index = 0;	
	cooldown = 30;
	crt_cooldown = 0;		
}

void Player::frameset_start()
{
	if(crt_frames == frames_start_skate) return;
	crt_frames = frames_start_skate;
	crt_frames_len = 2;
	crt_frame_index = 0;	
	cooldown = 10;
	crt_cooldown = 0;	
}

void Player::frameset_rage()
{
	if(crt_frames == frames_rage_skate) return;
	crt_frames = frames_rage_skate;
	crt_frames_len = 2;
	crt_frame_index = 0;				
}

bool Player::check_feet(const Map* map)
{
	bool fx = map->operator()(feetx[0], feety[0])==0;
	bool fy = map->operator()(feetx[1], feety[1])==0;
	if(!fx) 
	{
		fall_x = feetx[0];
		fall_y = feety[0];
	}
	else if(!fy)
	{
		fall_x = feetx[1];
		fall_y = feety[1];
	}
	
	return  fx && fy;
}

#include <stdio.h>
#include <gba_console.h>

void Player::update()
{			
	if(falling_scale)
	{						
		ax = ay = 0;				
		//vx = 0;
		//vy = 0;
		if(falling_scale<1024)
		{
			falling_scale=falling_scale*50;		
			falling_scale=falling_scale/49;
			OamPool::set_rotation_matrix(0, falling_scale, 0, 0, falling_scale);						
		}						
		else 
		{
			fell = true;
		}			
	}
	else {
		//printf("\e[1;1H\e[2Jvx=%s\nvy=%s", vx.to_string(), vy.to_string());
	}
	vx+=ax;
	vy+=ay;		
	
	px+=vx;
	py+=vy;

	if(falling_scale)
	{
		vx = vx*sf24(0,250);
		vy = vy*sf24(0,250);
	}	
	
	if(px<bndx) 
	{
		px=bndx, vx=0;
	}
	else if(px>=bndx+bndw)
	{
		px = bndx+bndw-1, vx=0;
	}
	
	if(py<bndy) 
	{
		py=bndy, vy=0;
	}
	else if(py>=bndy+bndh)
	{
		py = bndy+bndh-1, vy=0;
	}	
	
	vx = to_0(vx, sf24(0,5));
	vy = to_0(vy, sf24(0,5));
	
	ax = to_0(ax, ax*sf24(0,128));
	ay = to_0(ay, ay*sf24(0,128));
	
	const sf24 almost_stop_speed(0,130);
		
	if((vx.abs()>almost_stop_speed) || (vy.abs()>almost_stop_speed)) {
		frameset_normal();
	}
	else if(vx==0 && vy==0)
	{
		frameset_standby();
	}
	else 
	{
		frameset_start();
	}	
	
	feetx[0] = (s16)px + feet_pos[28*orientation + 4*pos_index + 0];
	feety[0] = (s16)py + feet_pos[28*orientation + 4*pos_index + 1];
	
	feetx[1] = (s16)px + feet_pos[28*orientation + 4*pos_index + 2];		
	feety[1] = (s16)py + feet_pos[28*orientation + 4*pos_index + 3];
	
	
			
	set_position(px,py);

	if(crt_cooldown>0) {
		crt_cooldown++;
		if(crt_cooldown>=cooldown) 
			crt_cooldown=0;
		return;
	}
	crt_cooldown++;
	set_current_frame(orientation, crt_frames[crt_frame_index]);
	
	crt_frame_index++;
	if(crt_frame_index>=crt_frames_len) 
		crt_frame_index = 0;		
}

void Player::enable_falling()
{	
	if(falling_scale>0) return;
	
	falling_scale = 1*256;
	// enable rotscal for sprite
	OamPool::set_rotation_matrix(0, falling_scale, 0, 0, falling_scale);
	
	u16* attr0 = &((u16*)get_attribute())[0];
	u16* attr1 = &((u16*)get_attribute())[1];
		
	*attr0|=(1<<8);	
	*attr0&=~(1<<9);
		
	*attr1&=~(31<<9);		
	
	//sf24 v = vx*vx+vy*vy;
	
	if(vx<0 && vy.abs()<=vx.abs()*sf24(0,64)) 
	{
		vx = -sf24(0, 160);
		vy=0;
	}
	else if(vx>0 && vy.abs()<=vx.abs()*sf24(0,64)) 
	{
		vx = sf24(0, 160);
		vy=0;
	}
	else if(vy<0 && vx.abs()<=vy.abs()*sf24(0,64)) 
	{
		vy = sf24(0,100);
		vx=0;
	}
	else if(vy>0 && vx.abs()<=vy.abs()*sf24(0,64)) 
	{
		vy = sf24(1, 89);
		vx=0;
	}		
	else if(orientation==PLAYER_FRONT) vy = sf24(1,128), vx=0;
	else if(orientation==PLAYER_BACK) vy = -sf24(1,0), vx=0;
	else if(orientation==PLAYER_LEFT) vx = -sf24(1,0), vy=0;
	else if(orientation==PLAYER_RIGHT) vx = sf24(1,128), vy=0;
	
}

const int PLAYER_FRONT = 0;
const int PLAYER_BACK  = 1;
const int PLAYER_LEFT  = 2;
const int PLAYER_RIGHT = 3;