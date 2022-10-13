#include <Astralbrew>
#include <gba.h>
#include <stdio.h>

using namespace Astralbrew;
using namespace Astralbrew::World;
using namespace Astralbrew::Drawing;
using namespace Astralbrew::Memory;
using namespace Astralbrew::Video;
using namespace Astralbrew::Objects;
using namespace Astralbrew::Entity;

#include "player.hpp"
#include "map.hpp"

static const u8 src[] {
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
	2,1,1,2,2,2,1,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,
	2,2,1,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,
	2,1,1,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,
	2,2,1,2,2,2,2,1,2,2,2,2,2,1,1,2,2,2,2,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,
	2,1,1,2,2,2,2,1,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,
	2,2,1,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,
	2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,	
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
};

class MainScene : public Scene
{	
private:
	VramManager vram_chr_1 = VramManager::from_char_block(1);
	VramManager vram_obj = VramManager::from_char_block(4);
	
	Player* player;	
	bool auto_mode = false;
	int framecnt = 0;
	
	MapSource map_source = {src, 44, 10};
	Map* map;
	MapViewer* viewer;
	
public:	
	virtual void init() override
	{
		Video::setMode(0);						
		
		//consoleDemoInit();
		//printf("\n          Ice Skater\n        position  test\n\n");
		//printf("UP  /DOWN  Change Orientation\n");
		//printf("LEFT/RIGHT Change Position\n");		
		
		bgInit(3, BgSize::Text256x256 , BgPaletteType::Pal4bit, 1, 1);
		
		for(int i=0;i<16;i++) {
			bgGetTilesPtr(3)[16+i]=0x1111;
			bgGetTilesPtr(3)[32+i]=0x2222;
		}
		
		BG_PALETTE[1] = 0x001f;
		BG_PALETTE[2] = 0x7fff;
		
		map = new Map(map_source);
		viewer=new MapViewer(map, 3);
		
				
		
		objEnable1D();		
		
		
		player = new Player();
		
		player->set_position(120-32, 110-32);
		player->update_position(nullptr);
		player->update_visual();				
	}
	
	bool scroll=false;
	
	virtual void on_key_up(int keys) override
	{
		if(keys & KEY_L) scroll=false;
	}
	
	virtual void on_key_held(int keys) override
	{
			
	}
	
	virtual void on_key_down(int keys) override
	{
		if(keys & KEY_L) scroll=true;		
		
		if(scroll) {
			if(keys & KEY_LEFT)
			{		
				viewer->set_scroll(viewer->scrollX-1, viewer->scrollY);
				viewer->invalidate();
				bgUpdate();
			}
			else if(keys & KEY_RIGHT)
			{		
				viewer->set_scroll(viewer->scrollX+1, viewer->scrollY);
				viewer->invalidate();
				bgUpdate();
			}
			
			return;
		}	
		
		if(keys & KEY_START) auto_mode = !auto_mode;
		if(auto_mode) return;
		int pos_index = player->get_pos_index();
		int orientation = player->get_orientation();
		if(keys & KEY_LEFT)
		{		
			if(pos_index > 0)
			{
				player->set_current_frame(orientation, pos_index-1);
			}			
		}		
		else if(keys & KEY_RIGHT)
		{
			if(pos_index < 6)
			{
				player->set_current_frame(orientation, pos_index+1);
			}
		}
		
		if(keys & KEY_UP)
		{		
			if(orientation > 0)
			{
				player->set_current_frame(orientation-1, pos_index);
			}			
		}		
		else if(keys & KEY_DOWN)
		{
			if(orientation < 3)
			{
				player->set_current_frame(orientation+1, pos_index);
			}
		}
	}
	
	virtual void frame() override
	{
		if(auto_mode && framecnt%8==0)
		{
			int pos_index = player->get_pos_index();
			int orientation = player->get_orientation();			
			player->set_current_frame((orientation+1)%4, (pos_index+1)%7);
			
		}
		framecnt++;
		OamPool::deploy();
	}
};

astralbrew_launch(MainScene)


