#pragma once

#include <Astralbrew>
#include <climits>

using namespace Astralbrew::Memory;
using namespace Astralbrew::World;
using namespace Astralbrew;

struct MapSource
{
	const u8* source;
	int tiles_width;
	int tiles_height;
};

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

class Rectangle
{
public:
	int left, top, width, height;
	Rectangle();
	Rectangle(int left, int top, int width, int height);		
};

class MapViewer
{
public:
	const Map* map;
	int map_id;
	Address* map_address;
	Camera* camera = nullptr;		
	
	int scrollX = SHRT_MIN, scrollY = SHRT_MIN;
	
	int text_scroll_x = 0;
	int text_scroll_y = 0;
	
	void scroll8x(int px);
	void scroll8y(int py);
		
	void invalidate();
	void set_scroll(int x, int y);
public:
	MapViewer(const Map* map, int map_id);
	
	void set_camera(Camera* cam);
	Camera* get_camera() const;
	
	//void scroll(int dx, int dy);
	
	~MapViewer();
};