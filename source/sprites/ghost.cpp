#include "ghost.hpp"
#include "ghost_gfx.h"

using namespace Astralbrew::Memory;

Ghost::Ghost() : ChunkEntity(ObjSize::SIZE_32x32, ObjBitDepth::_4bit, 1, class_of(GHOST))
{
	crt_gfx = gfx_addr;
	get_attribute()->set_palette_number(15);
	get_visual()->set_frame(0, &crt_gfx);
	get_visual()->set_crt_gfx(0);			
	/*dmaCopy(player_sheetPal, SPRITE_PALETTE, player_sheetPalLen);	
	gfx_addr.set_value((void*)0x06010080);
	set_current_frame(0,0);		
	
	get_attribute()->set_priority(0);		
	
	get_visual()->set_frame(0, &gfx_addr);
	get_visual()->set_crt_gfx(0);			
	set_anchor(128,128);	
	//set_anchor(ANCHOR_CENTER);	
	
	px = 120;
	py = 80;
	
	update_visual();*/
}

__attribute__((section(".ewram.ghost_gfx_addr"))) Address Ghost::gfx_addr;

void Ghost::loadVramData(VramManager& allocator)
{	
	allocator.reserve(&gfx_addr, ghost_gfxTilesLen);
	gfx_addr.write(ghost_gfxTiles, ghost_gfxTilesLen);
	
	dmaCopy(ghost_gfxPal, &SPRITE_PALETTE[0xF0], 32);
}

void Ghost::update()
{
	frm_cnt++;
	if(frm_cnt==5)
	{
		frm_cnt=0;
		
		crt_gfx.set_value((void*)((int)gfx_addr.get_value()+32*16*gfx_cnt));
		gfx_cnt++;
		if(gfx_cnt>=5) gfx_cnt = 0;		
	}	
}