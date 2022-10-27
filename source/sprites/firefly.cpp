#include "firefly.hpp"

#include "fader.h"
#include "regs.hpp"

__attribute__((section(".ewram.firefly_gfx_addr"))) Address Firefly::gfx_addr;

Firefly::Firefly(short px, short py) : ChunkEntity(ObjSize::SIZE_16x16, ObjBitDepth::_4bit, 1, class_of(FIREFLY))
{	
	crt_gfx = gfx_addr;
	glow_gfx = gfx_addr + 4*0x20;	
	get_attribute()->set_palette_number(0xC);
	get_visual()->set_frame(0, &crt_gfx);
	get_visual()->set_crt_gfx(0);					
	set_anchor(128,128);		
	this->px = sf24(px);
	this->py = sf24(py);
	
	glow = Sprite::quick16(&glow_gfx, ObjSize::SIZE_16x16, 128, 128);	
	
	glow->get_attribute()->set_palette_number(0xC);	
	
	*((s16*)glow->get_attribute()) |= 1<<10;	
	
	glow->get_attribute()->set_priority(2);	
	
	glow->update_visual();	
}

#include "qrand.h"
	
void Firefly::update()
{
	if((qrand()&3)==0)
	{		
		int x = (qrand()&0xF)-8;
		int y = (qrand()&0xF)-8;
		vx += x<0 ? sf24(-1,x) : sf24(0,x);
		if(vx<-sf24(0,128)) vx=-sf24(0,128); else if(vx>sf24(0,128)) vx=sf24(0,128);		
		vy += y<0 ? sf24(-1,y) : sf24(0,y);
		if(vy<-sf24(0,128)) vy=-sf24(0,128); else if(vy>sf24(0,128)) vy=sf24(0,128);
		
	}
	px+=vx;
	py+=vy;
	set_position(px, py);	
	glow->set_position(px, py);		
}

void Firefly::update_position(Camera* camera)
{
	glow->update_position(camera);
	Sprite::update_position(camera);
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
	delete glow;		
}