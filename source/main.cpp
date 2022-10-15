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
#include "ice_floor.h"

class MainScene : public Scene
{	
private:
	VramManager vram_chr_1 = VramManager::from_char_block(1);
	VramManager vram_obj = VramManager(0x06011000, 0x7000, 200);
	
	Player* player;			
	
	MapSource map_source = map1;
	Map* map;
	MapViewer* viewer;
	
	Address ice_tiles;
	
	Camera camera;
public:	
	virtual void init() override
	{	
		Video::setMode(0);		
		//consoleDemoInit();
		
		bgInit(1, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 2);
		bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 1, 1);
		
		bgSetPriority(1, 3);
		bgSetPriority(3, 1);
		
		Address transparentTile;
		vram_chr_1.reserve(&transparentTile, 32);
		
		vram_chr_1.reserve(&ice_tiles, ice_tilesTilesLen);
		ice_tiles.write(ice_tilesTiles, ice_tilesTilesLen);
				
		dmaCopy(ice_tilesPal, BG_PALETTE, (ice_tilesPalLen+3)/4*4);
		
		dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, ice_floorTilesLen-32);
		dmaCopy(ice_floorMap, bgGetMapPtr(1), ice_floorMapLen);
		dmaCopy(ice_floorPal, &BG_PALETTE[128], ice_floorPalLen);
				
		for(int i=0;i<1024;i++)
		{
			bgGetMapPtr(1)[i]&=0x0FFF;
			bgGetMapPtr(1)[i]|=0x8000;			
		}
		
		
		
		//for(int i=0;i<16;i++) {
			//bgGetTilesPtr(3)[16+i]=0x1111;
			//bgGetTilesPtr(3)[32+i]=0x2222;
		//}		
			
		BG_PALETTE[0] = Astralbrew::Drawing::Colors::White;
		BG_PALETTE[1] = Astralbrew::Drawing::Colors::Black;
		//BG_PALETTE[2] = Astralbrew::Drawing::Colors::Blue;
		
		map = new Map(map_source);
		viewer = new MapViewer(map, 3);
		
		objEnable1D();			
		
		player = new Player();								
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
	}
	
	virtual void on_key_down(int keys) override
	{		
		if(keys & KEY_START) restart();
	}
	
	void restart() {
		close()->next(new MainScene());
	}
	
	virtual void frame() override
	{		
		if(player->has_fallen())
		{
			close()->next(new MainScene());
		}
				
		player->update();
		player->update_position(&camera);				
		
		viewer->set_scroll(camera.get_x(), camera.get_y());		
		bgSetScroll(1, camera.get_x() & 0xFF, camera.get_y() & 0xFF);
		bgUpdate();								
		
		if(!player->check_feet(map))
		{
			player->enable_falling();
			//FATAL_ERROR("Game over");
		}
				
		OamPool::deploy();
	}
	
	~MainScene() 
	{
		delete player;
		delete map;
		delete viewer;
	}
};

astralbrew_launch(MainScene)


