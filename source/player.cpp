#include "player.hpp"

#include "player_sheet.h"

#include <stdio.h>

Player::Player() : Sprite(ObjSize::SIZE_64x64, ObjBitDepth::_8bit, 1, $(player))
{	
	dmaCopy(player_sheetPal, SPRITE_PALETTE, player_sheetPalLen);	
	gfx_addr.set_value((void*)0x06010080);
	set_current_frame(0,0);	
	
	get_visual()->set_frame(0, &gfx_addr);
	get_visual()->set_crt_gfx(0);	
	set_anchor(32,32);
	//set_anchor(ANCHOR_CENTER);
	
	px = 120;
	py = 80;
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
	ax=dx; 
	ay=dy;	
	
	//printf("%s %s %s\n",vx.to_string(), vy.to_string(), vx.abs().to_string());
	
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

#include <stdio.h>
#include <gba_console.h>

void Player::update()
{		
	vx+=ax;
	vy+=ay;
	px+=vx;
	py+=vy;	
	
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


const int PLAYER_FRONT = 0;
const int PLAYER_BACK  = 1;
const int PLAYER_LEFT  = 2;
const int PLAYER_RIGHT = 3;