#pragma once

#include "chunk_entity.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Memory;

register_class(FIREFLY);

class Firefly : public ChunkEntity
{
private:
	static Address gfx_addr;
	Address crt_gfx;

	sf24 vx=0, vy=0;
public:
	Firefly();
	
	void update();
	
	static void loadVramData(Astralbrew::Memory::VramManager& allocator);
	
	~Firefly();
};

