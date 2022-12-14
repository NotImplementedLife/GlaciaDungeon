#include "map_scene.hpp"

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

#include "ice_tiles.h"
#include "ice_floor.h"
#include "finish_portal.h"
#include "arrow.h"

#include "firefly.hpp"
#include "ghost.hpp"
#include "save_file.hpp"

#include "qmath.h"

#include "digits.h"
#include "end-level-screen.h"
#include "langs.h"

#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "title_scene.hpp"
#include "credits_scene.hpp"

#include "fader.h"
#include "qrand.h"

MosaicIncreaser::MosaicIncreaser() : ScheduledTask(5, 16) {}

void MosaicIncreaser::action()
{
	bgSetMosaicSize(value, value);
	//printf("%i\n",value);
	value++;
}

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

MapScene::MapScene(const MapData* md, bool chillin) : mapdata(md), chillin(chillin) { }	

void MapScene::load_mapstat(const MapData* md)
{		
	chunk_provider = ChunkProvider(md->name);
	map_source = md->source;
	player->place(md->start_x, md->start_y);
	finish_portal->set_position(md->finish_x, md->finish_y);
	finish_x = md->finish_x;
	finish_y = md->finish_y;
	//printf("\e[1;1H\e[2J%s\n",md->name);
	mapdata = md;
}

void MapScene::next_map()
{	
	int i = mapdata-MAP_STATS;	
	
	bool chillin_bak = chillin;
	if(i==MAP_STATS_COUNT-1)
	{
		close()->next(new CreditsScene());
	}		
	close()->next(new MapScene(&MAP_STATS[i+1], chillin_bak));
}

void MapScene::init()
{			
	
	Video::setMode(0);		
	//consoleDemoInit();
	
	bgInit(2, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 3);
	bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 0, 4);
	
	bgSetPriority(2, 3);
	bgSetPriority(3, 1);
	
	bgSetMosaicSize(1,1);
	bgMosaicEnable(1);
	bgMosaicEnable(3);
	
	bgSetAlpha(0,8,2,8);
	
	ice_tiles.set_value((void*)(0x06000000+32));
	ice_tiles.write(ice_tilesTiles, ice_tilesTilesLen);
	
	Address transparentTile;
	vram_chr_1.reserve(&transparentTile, 32);	
			
	dmaCopy(ice_tilesPal, SH_BG_PALETTE, (ice_tilesPalLen+3)/4*4);
	
	dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, ice_floorTilesLen-32);
	dmaCopy(ice_floorMap, bgGetMapPtr(2), ice_floorMapLen);
	dmaCopy(ice_floorPal, &SH_BG_PALETTE[128], ice_floorPalLen);	
	
	
	vram_obj.reserve(&portal_tiles, finish_portalTilesLen);
	portal_tiles.write(finish_portalTiles, finish_portalTilesLen);
	dmaCopy(finish_portalPal, &SH_SPRITE_PALETTE[0x90], finish_portalPalLen);			
	
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
	dmaCopy(arrowPal, &SH_SPRITE_PALETTE[0xD0], arrowPalLen);		
	
	
	Ghost::loadVramData(vram_obj);
	Firefly::loadVramData(vram_obj);
	
			
	for(int i=0;i<1024;i++)
	{
		bgGetMapPtr(2)[i]&=0x0FFF;
		bgGetMapPtr(2)[i]|=0x8000;			
	}		
	
	
	vram_obj.reserve(&digits_tiles, digitsTilesLen);
	digits_tiles.write(digitsTiles, digitsTilesLen);
	dmaCopy(digitsPal, &SH_SPRITE_PALETTE[0xE0], digitsPalLen);
	
	
	if(!chillin)
	{
		for(int i=0;i<6;i++)
		{
			digits_addr[i].set_value((u8*)digits_tiles.get_value());
			digits[i] = Sprite::quick16(&digits_addr[i], ObjSize::SIZE_8x8, ANCHOR_CENTER);	
			digits[i]->get_attribute()->set_palette_number(0xE);
			digits[i]->set_position(8+8*(i+i/3),8);
			digits[i]->update_visual();
			digits[i]->update_position(nullptr);		
		}	
		digits_addr[5].set_value((u8*)digits_tiles.get_value() + 32*10);
		digits[5]->set_position(8*4,8);
		digits[5]->update_visual();
		digits[5]->update_position(nullptr);		
	}
	else
	{
		for(int i=0;i<6;i++) digits[i]=nullptr;
	}
		
		
	
	
	
	
	//for(int i=0;i<16;i++) {
		//bgGetTilesPtr(3)[16+i]=0x1111;
		//bgGetTilesPtr(3)[32+i]=0x2222;
	//}		
		
	SH_BG_PALETTE[0] = Astralbrew::Drawing::Colors::White;
	//SH_BG_PALETTE[1] = Astralbrew::Drawing::Colors::Black;
	//SH_BG_PALETTE[2] = Astralbrew::Drawing::Colors::Blue		
	
	objEnable1D();					
	
	player = new Player();							
	player->get_attribute()->set_priority(0);
	
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
	
	if(mapdata)
		load_mapstat(mapdata);
	else
		load_mapstat(random_map());
	
	map = new Map(map_source);
	viewer = new MapViewer(map, 3);				
	
	
	camera.set_bounds(map->px_width(), map->px_height());
	camera.follow(player);
	
	player->set_movement_bounds(0,0,map->px_width(), map->px_height());		

	portal_updater = new PortalUpdater(finish_portal, portal_tiles);
	schedule_task(portal_updater);			
	
	irqSet(IRQ_VBLANK, mmVBlank);
	irqEnable(IRQ_VBLANK);
		
	mmInitDefault((mm_addr)soundbank_bin, 2);
	mmStart(MOD_MAP_THEME, MM_PLAY_LOOP);	
	
	frame();

	shpal_fade();
}	

void MapScene::before_frame()
{
	mmFrame();	
}

void MapScene::increment_timer()
{
	if(chillin) return;
	ss++;
	s0++;
	if(s0==10) s0=0, s1++;
	
	if(ss==60)
	{		
		mm++;
		
		m0++;
		if(m0==10) m0=0, m1++;
		if(m1==10) m1=0, m2++;
		
		digits_addr[0].set_value((u8*)digits_tiles.get_value() + 32*m2);		
		digits_addr[1].set_value((u8*)digits_tiles.get_value() + 32*m1);
		digits_addr[2].set_value((u8*)digits_tiles.get_value() + 32*m0);
		
		digits[0]->update_visual();
		digits[1]->update_visual();
		digits[2]->update_visual();
		
		ss=0;
		s0=s1=0;
	}	
	
	digits_addr[3].set_value((u8*)digits_tiles.get_value() + 32*s1);		
	digits_addr[4].set_value((u8*)digits_tiles.get_value() + 32*s0);
		
	digits[3]->update_visual();
	digits[4]->update_visual();	
	
}

void MapScene::update_arrow() 
{			
	int x = finish_x - player->get_px();
	int y = finish_y - player->get_py();
	
	if(-70<x && x<70 && -70<y && y<70)
	{
		if(is_in_circle(x,y, 70))		
		{
			arrow->set_position(-40,-40);
			arrow->update_position(nullptr);
			return;
		}
	}					
	
	int sc = norm_coords(x, -y);
			
	short _cos = sc & 0xFFFF;
	short _sin = (sc>>16) & 0xFFFF;		
			
	int cx = player->get_attribute()->get_x()+32;
	int cy = player->get_attribute()->get_y()+32;
	
	int ax = cx+ _cos/4; // 64*_cos/256
	int ay = cy- _sin/4;
			
	OamPool::set_rotation_matrix(1, _cos, -_sin, _sin, _cos);
	
	arrow->set_position(ax, ay);
	arrow->update_position(nullptr);
}

void MapScene::on_key_held(int keys)
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

void MapScene::on_key_down(int keys)
{		
	if(keys & KEY_START) {
		open_reports(3);
		//close()->next(new MapScene());
	}
}

void MapScene::restart() {
	bool chillin_bak = chillin;
	const MapData* mapdata_bak = mapdata;
		
	close()->next(new MapScene(mapdata_bak, chillin_bak));
}

bool MapScene::player_near_portal()
{
	int x = player->get_px() - finish_portal->pos_x();
	int y = player->get_py()+20 - finish_portal->pos_y();
	return is_in_circle(x,y,10);
}

bool MapScene::player_touches_ghost(Sprite* g) const
{
	int x = player->get_px() - g->pos_x();
	int y = player->get_py() - g->pos_y();
	return is_in_circle(x,y,12);
}

void MapScene::frame()
{					
	if(player->has_fallen())
	{
		open_reports(1); 
	}
				
	if(player_near_portal())
	{			
		open_reports(0);
		//next_map();			
	}
			
	player->update();
	player->update_position(&camera);					
	finish_portal->update_position(&camera);
	update_arrow();
	
	increment_timer();
	
	viewer->set_scroll(camera.get_x(), camera.get_y());
	
	int chk_x = viewer->get_scroll_x()/128;
	int chk_y = viewer->get_scroll_y()/128;
	for(int i=0;i<chunk_entities.size();i++) 
	{				
		if(chunk_entities[i]->is_of_class($(FIREFLY)))
		{
			int dx = abs(player->get_px()-chunk_entities[i]->pos_x());
			int dy = abs(player->get_py()-chunk_entities[i]->pos_y());
			if(dx+dy>512)
			{
				ChunkEntity* entity = chunk_entities[i];			
				chunk_entities.remove(entity);
				delete entity;							
				ff_cnt--;
			}
		}
		else
		{
			int chk_id = chunk_entities[i]->get_chunk();
			if(chk_id==-1) continue;
			int e_x = chk_id & 0xFFFF;
			int e_y = chk_id >> 16;		
			if(abs(chk_x-e_x)>=2 || abs(chk_y-e_y)>=2)
			{			
				// entity no longer in chunk, remove it
				ChunkEntity* entity = chunk_entities[i];			
				chunk_entities.remove(entity);
				delete entity;							
				
				chunk_provider.unregister_chunk(e_x, e_y);				
			}	
		}		
	}
	
	for(int dy=-1;dy<=1;dy++)
	{		
		int cy = chk_y+dy;
		if(cy<0) continue;
		for(int dx=-1;dx<=1;dx++)
		{			
			int cx = chk_x+dx;			
			if(cx<0) continue;
			if(chunk_provider.register_chunk(cx,cy))
			{				
				if(chunk_provider.chunk_has_enemies(cx,cy))
				{
					int cnt = 1 + qrand() % 3;
					for(int i=0;i<cnt;i++)
					{
						Ghost* ghost = new Ghost();
						ghost->set_chunk(chk_x+dx, chk_y+dy);
						ghost->set_position((chk_x+dx)*128+64, (chk_y+dy)*128+64);
						
						int s = qrand()%5;
						if(s<2)
							ghost->attach_ai(new SquareAI());
						else
							ghost->attach_ai(new CircleAI());
						
						chunk_entities.push_back(ghost);
						
					}
				}
			}			
		}
	}
		
		
	if(ff_cnt<2)
	{
		int px = player->get_px();
		int py = player->get_py();
		
		int rx = 130+(qrand()&31);
		int ry = 90+(qrand()&31);
			
		if(qrand()&3) rx=-rx;
		if(qrand()&3) ry=-ry;			
			
		px+=rx, py+=ry;
			
		Firefly* firefly = new Firefly(px, py);		
		firefly->set_chunk(-1);						
		chunk_entities.push_back(firefly);			
		ff_cnt++;			
	}
	
	for(int i=0;i<chunk_entities.size();i++)
	{					
		if(chunk_entities[i]->is_of_class($(GHOST)))
		{						
			((Ghost*)chunk_entities[i])->read_player_pos(player);
			if(player_touches_ghost(chunk_entities[i]))
			{				
				open_reports(2);
			}
		}
		chunk_entities[i]->update();
		chunk_entities[i]->update_visual();
		chunk_entities[i]->update_position(&camera);
	}	
	
	bgSetScroll(2, camera.get_x() & 0xFF, camera.get_y() & 0xFF);
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

#include "title_scene.hpp"

void MapScene::open_reports(int code)
{
	player->get_attribute()->set_x(-64);
	OamPool::deploy();
	bgInit(1, BgSize::Text256x512, BgPaletteType::Pal4bit, 0, 1);
	dmaCopy(end_level_screenTiles, (void*)0x06003000, end_level_screenTilesLen);
	dmaCopy(end_level_screenMap, bgGetMapPtr(1), end_level_screenMapLen);
	dmaCopy(end_level_screenPal, &BG_PALETTE[0x90], end_level_screenPalLen);
	
	//while(1);
	
	short* buff = (short*)bgGetMapPtr(1);
	for(int i=0;i<640;i++) buff[i]+=0x0180;
	
	if(code==0)
	{
		BG_PALETTE[0x93] = Colors::Green;
	}
	else if(code<3)
	{
		BG_PALETTE[0x91] = Colors::Red;
	}
	else if(code==3)
	{
		BG_PALETTE[0x93] = Colors::Yellow;	
	}
	
	
	bgSetScroll(1,0,-160);
	bgSetScroll(0,0,-160);
	bgUpdate();
	
	bgInit(0, BgSize::Text256x256, BgPaletteType::Pal4bit, 0, 5);
	vwf.set_render_space((void*)0x06006480,10,22);
	VwfEngine::prepare_map(vwf, bgGetMapPtr(0), 32, 4, 6, 0xA);
	vwf.clear(BgPaletteType::Pal4bit);
	
	BG_PALETTE[0xA1] = Colors::DarkGreen;
	BG_PALETTE[0xA2] = Colors::Red;
	BG_PALETTE[0xA3] = Colors::White;
	BG_PALETTE[0xA4] = Colors::Yellow;
	
	if(code==0) 
	{
		mmStop();
		mmStart(MOD_LEVEL_COMPLETE_THEME, MM_PLAY_ONCE);
		mmSetModuleTempo(1536);
		vwf.put_text(get_message(LMSG_LEVEL_COMPLETE), Pal4bit, SolidColorBrush(0x1));	
	}	
	else if(code<3)
	{
		mmStop();
		mmStart(MOD_GAME_OVER_THEME, MM_PLAY_ONCE);		
		vwf.put_text(get_message(LMSG_GAME_OVER), Pal4bit, SolidColorBrush(0x2));
				
		if(code==1)
			vwf.put_text(get_message(LMSG_GAME_OVER_PLAYER_FALL), Pal4bit, SolidColorBrush(0x3));
		else if(code==2)
			vwf.put_text(get_message(LMSG_GAME_OVER_GHOST_ATK), Pal4bit, SolidColorBrush(0x3));				
	}
	else if(code==3) 
	{
		vwf.put_text(get_message(LMSG_PAUSED), Pal4bit, SolidColorBrush(0x4));		
	}	
	
	for(int i=0;i<40;i++)
	{
		VBlankIntrWait();
		mmFrame();
		bgScroll(1,0,4);
		bgScroll(0,0,4);
		bgUpdate();
		if(i==10)
		{		
			if(code==0)
			{
				if(chillin)
					vwf.put_text(get_message(LMSG_NEXT_LEVEL_MSG), Pal4bit, SolidColorBrush(0x4));
			}
			else if(code<3)				
				vwf.put_text(get_message(LMSG_GAME_OVER_OPTIONS), Pal4bit, SolidColorBrush(0x4));
			else if(code==3) 
			{			
				vwf.put_text(get_message(LMSG_PAUSED_OPTIONS), Pal4bit, SolidColorBrush(0x4));		
			}						
		}
	}				
	
	
	reports = true;
	
	if(!chillin)
	{
		for(int i=0;i<6;i++)		
			digits[i]->get_attribute()->set_priority(0);
	}
	
	bool digits_moving = true;
	
	if(chillin) digits_moving = false;
	while(1)
	{
		VBlankIntrWait();
		mmFrame();
		
		scanKeys();
		int keys = keysDown();
		
		if(code==3) {
			if(keys & KEY_A) 
			{
				for(int i=0;i<40;i++)
				{					
					VBlankIntrWait();
					mmFrame();
					bgScroll(1,0,-4);
					bgScroll(0,0,-4);
					bgUpdate();		

					if(i>=35) {
						for(int j=0;j<32;j++)
							bgGetMapPtr(0)[(12+(i-35))*32+j]=0;
					}
				}				
				vwf.clear(Pal4bit);
				return;
			}
			else if(keys & KEY_B)
			{
				vwf.clear(Pal4bit);
				restart();
			}
			else if (keys & KEY_SELECT)
			{
				vwf.clear(Pal4bit);
				close()->next(new TitleScene());
			}
			
			continue;
		}
		
		if(code!=0)
		{			
			if(keys & KEY_A) restart(); 
			else if(keys & KEY_B)
			{
				close()->next(new TitleScene());
			}
		}
		else 
		{
			if(digits_moving)
			{
				for(int i=0;i<6;i++)
				{
					int x = digits[i]->get_attribute()->get_x();
					int y = digits[i]->get_attribute()->get_y();
					if(y<93)
					{
						digits[i]->get_attribute()->set_x(x+2);
						digits[i]->get_attribute()->set_y(y+2);
					} 
					else if(i==5)
					{
						if(!chillin) 
						{
							int j = (mapdata-MAP_STATS);
							unsigned mm_ss = (mm<<16)|ss;
							//assert(SAVE_FILE.data().maps_mmss[i]!=0);						
							if(mm_ss < SAVE_FILE.data().maps_mmss[j])
							{							
								SAVE_FILE.data().maps_mmss[j] = mm_ss;							
								vwf.put_text(get_message(LMSG_HI_SCORE), Pal4bit, SolidColorBrush(0x4));
							}					
							else						
								vwf.put_text("\n", Pal4bit, SolidColorBrush(0x4));
							
							if(j<MAP_STATS_COUNT-1)
							{
								if(SAVE_FILE.data().current_level==-1 || SAVE_FILE.data().current_level<=j)
									SAVE_FILE.data().current_level = j+1;
							}
							else
								SAVE_FILE.data().current_level = MAP_STATS_COUNT-1;
							SAVE_FILE.save();						
							
							digits_moving = false;
						}						
						vwf.put_text(get_message(LMSG_NEXT_LEVEL_MSG), Pal4bit, SolidColorBrush(0x4));
					}
				}
			}
			else
			{
				if(keys & KEY_A) next_map();
			}
			OamPool::deploy();
		}
	}
}
 

MapScene::~MapScene() 
{	
	mmSetModuleVolume(0);
	mmPause();
	mmStop();	
	VBlankIntrWait();
	mmFrame();
	
	shpal_set_black();
	shpal_fade();
	
	delete portal_updater;
	delete finish_portal;
	delete player;
	delete arrow;
	delete map;
	delete viewer;
	for(int i=0;i<chunk_entities.size();i++)
		delete chunk_entities[i];
	for(int i=0;i<6;i++)
		delete digits[i];		
	irqSet(IRQ_VBLANK, 0);	
	OamPool::reset();	
		
}
