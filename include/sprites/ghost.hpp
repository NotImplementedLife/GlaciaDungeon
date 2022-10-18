#pragma once

#include "chunk_entity.hpp"
#include "ghost_ai.hpp"
#include "player.hpp"

register_class(GHOST);

class Ghost : public ChunkEntity
{
private:
	static Astralbrew::Memory::Address gfx_addr;
	int frm_cnt = 0;
	int gfx_cnt = 0;
	Astralbrew::Memory::Address crt_gfx;
	GhostAI* ai = nullptr;
public:
	Ghost();
	
	void update();
	
	void attach_ai(GhostAI* ai);
	
	void read_player_pos(const Player* player);
	
	static void loadVramData(Astralbrew::Memory::VramManager& allocator);
	
	~Ghost();
};