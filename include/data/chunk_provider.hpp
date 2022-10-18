#pragma once

#include <Astralbrew>

class ChunkProvider
{
private:
	int seed;
	Astralbrew::Vector<int> chunks;	
public:
	ChunkProvider();
	ChunkProvider(int seed);
	ChunkProvider(const char* sseed);
	bool chunk_has_enemies(int cx, int cy) const;
	
	bool register_chunk(int cx, int cy);
	void unregister_chunk(int cx, int cy);
};