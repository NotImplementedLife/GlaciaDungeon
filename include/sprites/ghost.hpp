#pragma once

#include "chunk_entity.hpp"

register_class(GHOST);

class Ghost : public ChunkEntity
{
private:
	static Astralbrew::Memory::Address gfx_addr;
	int frm_cnt = 0;
	int gfx_cnt = 0;
	Astralbrew::Memory::Address crt_gfx;
public:
	Ghost();
	
	void update();
	
	static void loadVramData(Astralbrew::Memory::VramManager& allocator);
};