#include "chunk_provider.hpp"

ChunkProvider::ChunkProvider() : ChunkProvider(0) 
{
	for(int i=0;i<8;i++) chunks[i][0]=0;	
}

ChunkProvider::ChunkProvider(int seed) : seed(seed)
{	
	for(int i=0;i<8;i++) chunks[i][0]=0;
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
	for(int i=0;i<8;i++) chunks[i][0]=0;
}

bool ChunkProvider::chunk_has_enemies(int cx, int cy) const
{
	return (cx*seed+cy+1) % 17 == 10;
}

int ChunkProvider::exists(int chk_id) const
{
	int h = chk_id&7; 
	for(int i=1;i<=chunks[h][0];i++)
		if(chunks[h][i]==chk_id) return 1;
	return 0;
}

bool ChunkProvider::register_chunk(int cx, int cy)
{
	int chk_id = (cy<<16)|cx;
	if(exists(chk_id)) return false;
	
	int h = chk_id&7; 
	chunks[h][++chunks[h][0]] = chk_id;
	
	return true;
}

void ChunkProvider::unregister_chunk(int cx, int cy)
{
	int chk_id = (cy<<16)|cx;
	
	int h = chk_id & 7;
	for(int i=1;i<=chunks[h][0];i++)
	{
		if(chunks[h][i]==chk_id)
		{
			for(int j=i;j<chunks[h][0];j++)
			{
				chunks[h][j]=chunks[h][j+1];
			}
			chunks[h][0]--;
			return;
		}
	}
	
	//chunks.remove(chk_id);
}