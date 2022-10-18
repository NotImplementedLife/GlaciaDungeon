#pragma once
#include <Astralbrew>

using namespace Astralbrew::Entity;
using namespace Astralbrew::Objects;

class ChunkEntity : public Sprite
{
private:
	int chunk;
	
public:
	int get_chunk() const;
	void set_chunk(int chunk_id);
	void set_chunk(int chunk_x, int chunk_y);
	
	bool is_in_chunk(int chunk_id) const;
	bool is_in_chunk(int chunk_x, int chunk_y) const;
	
	ChunkEntity(ObjSize size, ObjBitDepth bit_depth, u16 frames_count, obj_class_t obj_class);
	
	virtual void update() = 0;
};
