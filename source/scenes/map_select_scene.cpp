#include "map_select_scene.hpp"

#include "map.hpp"
#include "mapstats.hpp"
#include "map_scene.hpp"

#include "a_select.h"

using namespace Astralbrew;
using namespace Astralbrew::Video;
using namespace Astralbrew::Objects;


void hblank_palette()
{	
	if(REG_VCOUNT<29)
	{
		int c=3+REG_VCOUNT;
		BG_PALETTE[0] = (31<<10)|(c<<5)|c;				
	}
	else if(REG_VCOUNT<132)
	{
		BG_PALETTE[0] = Astralbrew::Drawing::Colors::White;		
	}		
	else if(REG_VCOUNT<160)		
	{
		int c=3+(160-REG_VCOUNT);
		BG_PALETTE[0] = (31<<10)|(c<<5)|c;		
	}
	
	
}

void MapSelectScene::init()
{
	Utils::zeroize((void*)0x06000000, 0x18000);
	Video::setMode(4);
	bgInit(2, BgSize::RotS256x256, BgPaletteType::Pal8bit, 0, 0);
	BG_PALETTE[0] = Drawing::Colors::Black;
	BG_PALETTE[1] = Drawing::Colors::White;
	BG_PALETTE[2] = Drawing::Colors::Blue;
	BG_PALETTE[3] = RGB5(0,15,31);
	BG_PALETTE[4] = Drawing::Colors::Green;
	BG_PALETTE[5] = Drawing::Colors::Red;
	
	
	Address adr;
	adr.set_value((void*)0x06014000);
	
	dmaCopy(a_selectTiles, adr.get_value(), a_selectTilesLen);
	dmaCopy(a_selectPal, SPRITE_PALETTE, a_selectPalLen);
	
	
	
	objEnable1D();	
			
	a_select = Sprite::quick16(&adr, ObjSize::SIZE_64x32, ANCHOR_BOTTOM_RIGHT);
	a_select->set_position(230,155);
	a_select->update_visual();
	a_select->update_position(nullptr);
	a_select->get_attribute()->set_priority(0);
	
	irqEnable(IRQ_HBLANK);
	irqSet(IRQ_HBLANK, hblank_palette);	
	
	generate_map();
}

void MapSelectScene::frame()
{		
	a_select->set_position(230,155+cnt/10);
	a_select->update_position(nullptr);
	cnt+=sgn;
	if(cnt==22) sgn=-1;
	else if(cnt==-20) sgn=1;	
	OamPool::deploy();
}

void MapSelectScene::set_tile(int x, int y, int id)
{	
	u16* gfx = bgGetMapPtr(2);
	if(id==0xFF)
	{
		gfx[y*120 + x/2] = gfx[(y+1)*120+x/2] = 0x0404;
		return;
	}
	if(id==0xFE)
	{
		gfx[y*120 + x/2] = gfx[(y+1)*120+x/2] = 0x0505;
		return;
	}
	if(id==0)
	{
		gfx[y*120 + x/2] = gfx[(y+1)*120+x/2] = 0x0202+((x/2+y/2)%2)*0x0101;
		return;
	}	
	gfx[y*120 + x/2] = gfx[(y+1)*120+x/2] = 0x0101;
		
}

void MapSelectScene::on_key_down(int keys)
{
	if(keys & KEY_LEFT)
	{
		if(index>0) 
		{
			index--;
			generate_map();
		}
	}
	else if(keys & KEY_RIGHT)
	{
		if(index<MAP_STATS_COUNT-1)
		{
			index++;
			generate_map();
		}			
	}
	else if(keys & (KEY_START | KEY_A))
	{
		irqDisable(IRQ_HBLANK);
		irqSet(IRQ_HBLANK, nullptr);
		
		int bak_index = index;
		close()->next(new MapScene(&MAP_STATS[bak_index]));
	}
}

void MapSelectScene::generate_map()
{
	const MapData& md = MAP_STATS[index];
	Map map(md.source);
	int start_x = md.start_x/8;
	int start_y = md.start_y/8;
	
	for(int y=-half_h;y<half_h;y++)
	{
		for(int x=-half_w;x<half_w;x++)
		{
			if(start_x+x>=0 && start_x+x<map.tiles_width() &&
			   start_y+y>=0 && start_y+y<map.tiles_height())
			{
				set_tile(120+x, 80+y, map.get_tile(start_x+x,start_y+y));
			}
			else if (start_x+x==md.finish_x/8 && start_y+y==md.finish_y/8)
			{
				set_tile(120+x, 80+y, 0xFE);
			}
			else
			{
				set_tile(120+x, 80+y, 1);
			}
		}
	}
	set_tile(120, 80, 0xFF);	
}

MapSelectScene::~MapSelectScene()
{
	delete a_select;
	OamPool::reset();
	Utils::zeroize((void*)0x06000000, 0x18000);
}