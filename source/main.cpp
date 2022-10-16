#include <Astralbrew>
#include <gba.h>
#include <stdio.h>
#include <math.h>

using namespace Astralbrew;
using namespace Astralbrew::World;
using namespace Astralbrew::Drawing;
using namespace Astralbrew::Memory;
using namespace Astralbrew::Video;
using namespace Astralbrew::Objects;
using namespace Astralbrew::Entity;

#include "player.hpp"
#include "map.hpp"

#include "mapstats.hpp"

#include "map0.maps.h"
#include "map1.maps.h"
#include "map2.maps.h"

#include "ice_tiles.h"
#include "ice_floor.h"
#include "finish_portal.h"
#include "arrow.h"

class MosaicIncreaser : public ScheduledTask
{
private:
	int value = 1;
public:
	MosaicIncreaser() : ScheduledTask(5, 16) {}
	void action() override
	{
		bgSetMosaicSize(value, value);
		//printf("%i\n",value);
		value++;
	}
};

class PortalUpdater : public ScheduledTask
{
private:
	Sprite* portal;
	Address tile_addr;
	Address crt_addr;
	int spr_frame = 0;	
public:
	PortalUpdater(Sprite* portal, Address addr) : ScheduledTask(10, 0), portal(portal), tile_addr(addr) { }
	void action() override
	{		
		crt_addr.set_value((void*)((int)tile_addr.get_value()+4*4*64*spr_frame));
		portal->get_visual()->set_frame(0, &crt_addr);
		spr_frame++;
		spr_frame&=3;
		
		portal->update_visual();
	}
};

class MainScene : public Scene
{	
private:
	VramManager vram_chr_1 = VramManager::from_char_block(1);
	VramManager vram_obj = VramManager(0x06011000, 0x7000, 200);
	
	Player* player;			
	Sprite* finish_portal;
	Sprite* arrow;
	
	MapSource map_source = map2;
	Map* map;
	MapViewer* viewer;
	
	Address ice_tiles;
	Address portal_tiles;
	Address arrow_tiles;
	
	Camera camera;
	
	bool falling = false;	
	
	MosaicIncreaser mosaic_increaser;
	PortalUpdater* portal_updater;
	
	int finish_x, finish_y;
public:	
	void load_mapstat(const MapData& md)
	{		
		map_source = md.source;
		player->place(md.start_x, md.start_y);
		finish_portal->set_position(md.finish_x, md.finish_y);
		finish_x = md.finish_x;
		finish_y = md.finish_y;
		printf("\e[1;1H\e[2J%s\n",md.name);
	}

	virtual void init() override
	{			
		
		Video::setMode(0);		
		consoleDemoInit();
		
		bgInit(1, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 5);
		bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 1, 6);
		
		bgSetPriority(1, 3);
		bgSetPriority(3, 1);
		
		bgSetMosaicSize(1,1);
		bgMosaicEnable(1);
		bgMosaicEnable(3);
		
		Address transparentTile;
		vram_chr_1.reserve(&transparentTile, 32);
		
		vram_chr_1.reserve(&ice_tiles, ice_tilesTilesLen);
		ice_tiles.write(ice_tilesTiles, ice_tilesTilesLen);
				
		dmaCopy(ice_tilesPal, BG_PALETTE, (ice_tilesPalLen+3)/4*4);
		
		dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, ice_floorTilesLen-32);
		dmaCopy(ice_floorMap, bgGetMapPtr(1), ice_floorMapLen);
		dmaCopy(ice_floorPal, &BG_PALETTE[128], ice_floorPalLen);
		
		vram_obj.reserve(&portal_tiles, finish_portalTilesLen);
		portal_tiles.write(finish_portalTiles, finish_portalTilesLen);
		dmaCopy(finish_portalPal, &SPRITE_PALETTE[0x90], finish_portalPalLen);		
		
		u16* dst = (u16*)portal_tiles.get_value();
		for(int i=0;i<finish_portalTilesLen;i++)
		{
			unsigned short x = dst[i];			
			if(x&(0xFF)) x+=0x90;
			if(x&(0xFF00)) x+=0x9000;
			dst[i]=x;
		}
		
		
		vram_obj.reserve(&arrow_tiles, arrowTilesLen);
		arrow_tiles.write(arrowTiles, arrowTilesLen);
		dmaCopy(arrowPal, &SPRITE_PALETTE[0xD0], arrowPalLen);		
		
				
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
		//BG_PALETTE[2] = Astralbrew::Drawing::Colors::Blue		
		
		objEnable1D();					
		
		player = new Player();						
		
		finish_portal = Sprite::quick256(&portal_tiles, ObjSize::SIZE_32x32, ANCHOR_CENTER);
		finish_portal->update_visual();
		finish_portal->update_position(&camera);
		
		arrow = Sprite::quick16(&arrow_tiles, ObjSize::SIZE_32x32, ANCHOR_CENTER);
		arrow->get_attribute()->set_palette_number(0xD);
		arrow->update_visual();		
		arrow->get_attribute()->set_rotation_scaling(true);		
		arrow->get_attribute()->set_affine_matrix(1);		
		arrow->get_attribute()->set_double_size(true);
		arrow->set_hitbox(Hitbox(0,0,64,64));
		arrow->set_anchor(128,128);
		
		arrow->update_position(&camera);
		
		load_mapstat(random_map());
		
		map = new Map(map_source);
		viewer = new MapViewer(map, 3);
		
		
		
		
		camera.set_bounds(map->px_width(), map->px_height());
		camera.follow(player);
		
		player->set_movement_bounds(0,0,map->px_width(), map->px_height());		

		portal_updater = new PortalUpdater(finish_portal, portal_tiles);
		schedule_task(portal_updater);		
	}	
	
	float a=0;
	
	/*int sin(int a) // uses pi~=256/81, that means for a=256, sin(a) = sin(pi) = 0
	{
		static const long long A = 120LL * 81 * 81;
		static const long long B = 6LL * 120 * 81*81*81*81;
		static const long long C = 6LL * 120 * 81*81*81*81*81;
		
		int b1 = a;
		int b2 = a*a;
		int b3 = b1 * b2;
		int b5 = b3 * b2;
		return (int)(256LL*(b5-A*b3+B*b1)/C);
	}*/
	
	/*int cos(int a) 
	{
		
	}*/
	
	void update_arrow() 
	{			
		int x = finish_x - player->get_px();
		int y = finish_y - player->get_py();
		
		if(-70<x && x<70 && -70<y && y<70)
		{
			if((x*x+y*y)<4900)
			{
				arrow->set_position(-40,-40);
				arrow->update_position(nullptr);
				return;
			}
		}		
		
		float a = atan2(-y,x);
	
		int _sin, _cos;		
		_sin=(int)(256*sin(a));
		_cos=(int)(256*cos(a));		
				
		int cx = player->get_attribute()->get_x()+32;
		int cy = player->get_attribute()->get_y()+32;
		
		int ax = cx+(50*_cos)/256;
		int ay = cy-(50*_sin)/256;
				
		OamPool::set_rotation_matrix(1, _cos, -_sin, _sin, _cos);
		
		arrow->set_position(ax, ay);
		arrow->update_position(nullptr);
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
		finish_portal->update_position(&camera);
		update_arrow();
		
		viewer->set_scroll(camera.get_x(), camera.get_y());		
		bgSetScroll(1, camera.get_x() & 0xFF, camera.get_y() & 0xFF);
		bgUpdate();								
		
		if(!falling && !player->check_feet(map))
		{
			falling = true;
			player->enable_falling();											
			//FATAL_ERROR("Game over");
			
			schedule_task(&mosaic_increaser);
		}		
				
		OamPool::deploy();
	}
	
	~MainScene() 
	{
		delete portal_updater;
		delete finish_portal;
		delete player;
		delete arrow;
		delete map;
		delete viewer;
	}
};

astralbrew_launch_with_splash(MainScene)


