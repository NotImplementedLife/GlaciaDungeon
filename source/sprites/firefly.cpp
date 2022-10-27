#include "firefly.hpp"

#include "fader.h"

__attribute__((section(".ewram.firefly_gfx_addr"))) Address Firefly::gfx_addr;

Firefly::Firefly() : ChunkEntity(ObjSize::SIZE_16x16, ObjBitDepth::_4bit, 1, class_of(FIREFLY))
{
	crt_gfx = gfx_addr;
	get_attribute()->set_palette_number(0xC);
	get_visual()->set_frame(0, &crt_gfx);
	get_visual()->set_crt_gfx(0);					
	set_anchor(128,128);		
}
	
void Firefly::update()
{
	
}

#include "firefly_gfx.h"

void Firefly::loadVramData(Astralbrew::Memory::VramManager& allocator)
{
	allocator.reserve(&gfx_addr, firefly_gfxTilesLen);
	gfx_addr.write(firefly_gfxTiles, firefly_gfxTilesLen);	
	dmaCopy(firefly_gfxPal, &SH_SPRITE_PALETTE[0xC0], 32);
}

Firefly::~Firefly()
{
	
}