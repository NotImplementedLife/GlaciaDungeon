#include "ghost.hpp"
#include "ghost_gfx.h"

using namespace Astralbrew::Memory;

Ghost::Ghost() : ChunkEntity(ObjSize::SIZE_32x32, ObjBitDepth::_4bit, 1, class_of(GHOST))
{
	crt_gfx = gfx_addr;
	get_attribute()->set_palette_number(15);
	get_visual()->set_frame(0, &crt_gfx);
	get_visual()->set_crt_gfx(0);			
	
	set_hitbox(Hitbox(6,6,20,20));
	set_anchor(128,128);	
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
	
	if(ai)
	{		
		ai->execute_step();
		set_position(ai->get_x(), ai->get_y());
	}
}

void Ghost::attach_ai(GhostAI* ai)
{
	this->ai = ai;
	int chk = get_chunk();	
	ai->ghost_pos(pos_x(), pos_y());
	assert((chk>>16)>=0);
	ai->set_chunk(chk & 0xFFFF, chk>>16);
}

void Ghost::read_player_pos(const Player* player)
{
	if(!ai) return;
	ai->player_pos(player->get_px(), player->get_py());
}

Ghost::~Ghost()
{
	delete ai;
}