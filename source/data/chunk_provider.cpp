#include "chunk_provider.hpp"

ChunkProvider::ChunkProvider() : ChunkProvider(0) { }

ChunkProvider::ChunkProvider(int seed) : seed(seed)
{	
}

ChunkProvider::ChunkProvider(const char* sseed)
{
	this->seed = 0;
	int c = 1;
	while(*sseed) {
		this->seed += c * (*sseed);
		c *= (*sseed);
		sseed++;
	}
}

bool ChunkProvider::chunk_has_enemies(int cx, int cy) const
{
	return (cx*seed+cy+1) % 17 == 10;
}

bool ChunkProvider::register_chunk(int cx, int cy)
{
	int chk_id = (cy<<16)|cx;
	if(chunks.exists(chk_id)) return false;
	chunks.push_back(chk_id);	
	return true;
}

void ChunkProvider::unregister_chunk(int cx, int cy)
{
	int chk_id = (cy<<16)|cx;
	chunks.remove(chk_id);
}