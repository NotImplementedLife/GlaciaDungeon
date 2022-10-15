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

#include "map0.maps.h"
#include "map1.maps.h"

#include "ice_tiles.h"

class MainScene : public Scene
{	
private:
	VramManager vram_chr_1 = VramManager::from_char_block(1);
	VramManager vram_obj = VramManager(0x06011000, 0x7000, 200);
	
	Player* player;	
	bool auto_mode = false;
	int framecnt = 0;
	
	MapSource map_source = map1;
	Map* map;
	MapViewer* viewer;
	
	Address ice_tiles;
	
	Camera camera;
public:	
	virtual void init() override
	{	
		Video::setMode(0);		
		consoleDemoInit();
		
		bgInit(3, BgSize::Text256x256 , BgPaletteType::Pal4bit, 1, 1);
		
		Address transparentTile;
		vram_chr_1.reserve(&transparentTile, 32);
		
		vram_chr_1.reserve(&ice_tiles, ice_tilesTilesLen);
		ice_tiles.write(ice_tilesTiles, ice_tilesTilesLen);
		
		//for(int i=0;i<16;i++) {
			//bgGetTilesPtr(3)[16+i]=0x1111;
			//bgGetTilesPtr(3)[32+i]=0x2222;
		//}

		dmaCopy(ice_tilesPal, BG_PALETTE, ice_tilesPalLen);
			
		BG_PALETTE[0] = Astralbrew::Drawing::Colors::Black;
		//BG_PALETTE[2] = Astralbrew::Drawing::Colors::Blue;
		
		map = new Map(map_source);
		viewer = new MapViewer(map, 3);
		
		objEnable1D();			
		
		player = new Player();		
		
		player->set_position(120,80);
		player->update_position(&camera);
		player->update_visual();				
		player->get_attribute()->set_priority(0);		
		
		camera.follow(player);		
				
		player->set_movement_bounds(0,0,map->px_width(), map->px_height());
		
		camera.set_bounds(map->px_width(), map->px_height());
	}	
	
	virtual void on_key_up(int keys) override
	{		
	}
	
	virtual void on_key_held(int keys) override
	{		
		if(keys & KEY_LEFT)
		{										
			player->move(-sf24(0,32),0);
		}
		else if(keys & KEY_RIGHT)
		{
			player->move(sf24(0,32),0);
		}			
		if(keys & KEY_UP)
		{						
			player->move(0,-sf24(0,32));
		}
		else if(keys & KEY_DOWN)
		{						
			player->move(0,sf24(0,32));
		}
		bgUpdate();								
	}
	
	virtual void on_key_down(int keys) override
	{		
				
	}
	
	virtual void frame() override
	{		
		if(auto_mode && framecnt%8==0)
		{
			int pos_index = player->get_pos_index();
			int orientation = player->get_orientation();			
			player->set_current_frame((orientation+1)%4, (pos_index+1)%7);			
		}
		
		viewer->set_scroll(camera.get_x(), camera.get_y());
		player->update();
		player->update_position(&camera);
		framecnt++;
		OamPool::deploy();
	}
};

astralbrew_launch(MainScene)


