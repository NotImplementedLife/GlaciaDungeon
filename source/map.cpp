#include "map.hpp"

using namespace Astralbrew::Video;

Map::Map(const MapSource& map_source) : map_source(map_source)
{
	
}

int Map::operator() (int x, int y) const
{	
	if(x<0 || y<0 || x>=8*map_source.tiles_width || y>=8*map_source.tiles_height)
		return 0;
	x/=8;
	y/=8;
	return map_source.source[y * map_source.tiles_width + x];
}
	
	
int Map::get_tile(int tx, int ty) const
{
	if(tx<0 || ty<0 || tx>=map_source.tiles_width || ty>=map_source.tiles_height)
		return 0;
	return map_source.source[ty * map_source.tiles_width + tx];
}


Rectangle::Rectangle(int left, int top, int width, int height)
	:left(left), top(top), width(width), height(height) {}
	
Rectangle::Rectangle() : Rectangle(0,0,0,0) {}

	
MapViewer::MapViewer(const Map* map, int map_id) : map(map), map_id(map_id)
{
	map_address = new Address();
	map_address->set_value(bgGetMapPtr(map_id));
	
	set_scroll(120,80);
	invalidate();
}

void MapViewer::set_camera(Camera* cam)
{
	camera = cam;
}

Camera* MapViewer::get_camera() const
{
	return camera;
}

/*void MapViewer::scroll(int dx, int dy)
{
	
}*/

void MapViewer::scroll8x(int px)
{
	px%=8;
	
	scrollX += px;	
	
	int left = (scrollX-120);
	int top  = (scrollY-80);
	
	int tileX = (256+left%256)%256;
	int tileY = (256+top%256)%256;
	
	bgSetScroll(map_id, tileX, tileY);
	
	tileX/=8;
	tileY/=8;
	
	u16* mptr = (u16*)map_address->get_value();
	
	int x0 = (px>=0)?31:0;
	int x1 = (px>=0)?30:1;	
	
	for(int y=0;y<23;y++) 
	{
		u16* row = &mptr[((tileY+y)%32)*32];				
		row[(tileX+x0)%32] = map->get_tile((left-7*(left<0))/8+x0,(top-7*(top<0))/8+y);
		row[(tileX+x1)%32] = map->get_tile((left-7*(left<0))/8+x1,(top-7*(top<0))/8+y);		
	}		
}

void MapViewer::scroll8y(int py)
{
	py%=8;
	
	scrollY += py;	
	
	int left = (scrollX-120);
	int top  = (scrollY-80);
	
	int tileX = (256+left%256)%256;
	int tileY = (256+top%256)%256;
	
	bgSetScroll(map_id, tileX, tileY);
	
	tileX/=8;
	tileY/=8;
	
	u16* mptr = (u16*)map_address->get_value();
	
	int y0 = (py>=0)?20:0;
	int y1 = (py>=0)?19:1;
	
	u16* row0 = &mptr[((tileY+y0)%32)*32];				
	u16* row1 = &mptr[((tileY+y1)%32)*32];					
	
	for(int x=0;x<31;x++) 
	{		
		row0[(tileX+x)%32] = map->get_tile((left-7*(left<0))/8+x,(top-7*(top<0))/8+y0);
		row1[(tileX+x)%32] = map->get_tile((left-7*(left<0))/8+x,(top-7*(top<0))/8+y1);
	}		
	
}



void MapViewer::set_scroll(int x, int y)
{
	scrollX = x;
	scrollY = y;
}

void MapViewer::invalidate()
{
	int left = (scrollX-120);
	int top  = (scrollY-80);
	
	int tileX = (256+left%256)%256;
	int tileY = (256+top%256)%256;
	
	bgSetScroll(map_id, tileX, tileY);
	
	tileX/=8;
	tileY/=8;
	
	u16* mptr = (u16*)map_address->get_value();
	
	for(int y=0;y<31;y++) 
	{
		u16* row = &mptr[((tileY+y)%32)*32];
		for(int x=0;x<31;x++)
		{
			//mptr[((tileY+y)%32)*32 + (tileX+x)%32] = map->get_tile((left-7*(left<0))/8+x,(top-7*(top<0))/8+y); 
			row[(tileX+x)%32] = map->get_tile((left-7*(left<0))/8+x,(top-7*(top<0))/8+y);
		}
	}
}

MapViewer::~MapViewer()
{
	delete map_address;
}