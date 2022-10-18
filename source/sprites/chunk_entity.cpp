#include "chunk_entity.hpp"


int ChunkEntity::get_chunk() const
{
	return chunk;
}

void ChunkEntity::set_chunk(int chunk_id)
{
	chunk = chunk_id;
}

void ChunkEntity::set_chunk(int chunk_x, int chunk_y)
{
	chunk = (chunk_y<<16) | chunk_x;
}

bool ChunkEntity::is_in_chunk(int chunk_id) const
{
	return get_chunk() == chunk_id;
}

bool ChunkEntity::is_in_chunk(int chunk_x, int chunk_y) const
{
	return get_chunk() == ((chunk_y<<16) | chunk_x);
}

ChunkEntity::ChunkEntity(ObjSize size, ObjBitDepth bit_depth, u16 frames_count, obj_class_t obj_class)
	: Sprite(size, bit_depth, frames_count, obj_class) { }
