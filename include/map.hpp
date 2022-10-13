#pragma once

#include <Astralbrew>
#include <climits>

using namespace Astralbrew::Memory;
using namespace Astralbrew::World;
using namespace Astralbrew;

#include "map_source.h"

class Map
{
private:
	const MapSource& map_source;
public:
	Map(const MapSource& map_source);
	
	int operator() (int x, int y) const;	
	
	inline int tiles_width() { return map_source.tiles_width; }
	inline int tiles_height() { return map_source.tiles_height; }
	
	inline int px_width() { return tiles_width()<<3; }
	inline int px_height() { return tiles_height()<<3; }
	
	int get_tile(int tx, int ty) const;
};

class MapViewer
{
private:
	const Map* map;
	int map_id;
	Address* map_address;		
	
	int scrollX = SHRT_MIN, scrollY = SHRT_MIN;	
	
	void set_scroll_crds(int x, int y);
	
	void scroll8x(int px);
	void scroll8y(int py);	
		
	void invalidate();
public:
	MapViewer(const Map* map, int map_id);	
	
	void scroll(int dx, int dy);	
	
	void set_scroll(int x, int y);
	
	~MapViewer();
};