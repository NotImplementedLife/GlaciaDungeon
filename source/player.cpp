#include "player.hpp"

#include "player_sheet.h"

Player::Player() : Sprite(ObjSize::SIZE_64x64, ObjBitDepth::_8bit, 1, $(player))
{
	dmaCopy(player_sheetPal, SPRITE_PALETTE, player_sheetPalLen);	
	gfx_addr.set_value((void*)0x06010080);
	set_current_frame(0,0);	
	
	get_visual()->set_frame(0, &gfx_addr);
	get_visual()->set_crt_gfx(0);
	set_anchor(32, 32);
}

void Player::set_current_frame(int orientation, int pos_index)
{
	this->orientation = orientation;
	this->pos_index = pos_index;
	
	u8* src = ((u8*)player_sheetTiles) + 64*64*(7*orientation + pos_index);
	gfx_addr.write(src, 64*64);
}


const int PLAYER_FRONT = 0;
const int PLAYER_BACK  = 1;
const int PLAYER_LEFT  = 2;
const int PLAYER_RIGHT = 3;