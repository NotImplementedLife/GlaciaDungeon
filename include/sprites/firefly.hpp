#pragma once

#include "chunk_entity.hpp"

using namespace Astralbrew;
using namespace Astralbrew::Memory;

//register_class(FIREFLY);

class Firefly : public ChunkEntity
{
private:
	static Address gfx_addr;
	Address crt_gfx;
	Address glow_gfx;	

	sf24 px, py;
	sf24 vx=0, vy=0;
	
	Sprite* glow;	
public:
	Firefly(short px, short py);
	
	void update();
	
	static void loadVramData(Astralbrew::Memory::VramManager& allocator);
	
	void update_position(Camera* camera) override;
	
	~Firefly();
};

