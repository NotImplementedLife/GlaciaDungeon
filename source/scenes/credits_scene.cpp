#include "credits_scene.hpp"

#include <maxmod.h>
#include "ice_tiles.h"
#include "ice_floor.h"

#include "credits_bg.mapdata.h"
#include "fader.h"
using namespace Astralbrew::Video;
using namespace Astralbrew::Drawing;

#include "menu_scene.hpp"
#include "title_scene.hpp"

#include "soundbank_bin.h"
#include "soundbank.h"


const char* const CreditsScene::messages = 
	"Credits\0"
	"Author : NotImplementedLife\0"
	"Graphics : \n    Me\0"
	"Code : \n    Also me\0"
	"Instrument samples : \n    ModArchive\n    - only_lonely.mod\n    - p_and_n.mod    \n    - bjorn.mod    \n    - stor_och_liten.mod\0"
	"Composition : \n    Original\n(that's why it sounds bad :3)\0"
	"Motivation to work on this game:\n    GBAJam2022 yuhoo!!!\0"
	"Engine : \n   Astralbrew\0"
	"Glacia Dungeon\n\n          2022\0"
	"A game by\0"
	"NotImpLife\0"
	"part of collection\0"
	"EsotericDevZone\n\n           2022\0"
	" \0"
	" \0"
	"~ Belu oek Oku ~ \0"
	" \0"
	" \0"
	" \0";


CreditsScene::CreditsScene(bool title_next)
{
	this->title_next = title_next;
}

void CreditsScene::init()
{	
	msgindex = messages;
		
	irqDisable(IRQ_HBLANK);	
	
	bgSetAlpha(0,0,0,0);
	
	Video::setMode(0);		
	//consoleDemoInit();
	
	bgInit(2, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 3);
	bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 0, 4);
	
	bgSetPriority(2, 3);
	bgSetPriority(3, 1);
	
	ice_tiles.set_value((void*)(0x06000000+32));
	ice_tiles.write(ice_tilesTiles, ice_tilesTilesLen);
	dmaCopy(ice_tilesPal, SH_BG_PALETTE, (ice_tilesPalLen+3)/4*4);
	
	dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, ice_floorTilesLen-32);
	dmaCopy(ice_floorMap, bgGetMapPtr(2), ice_floorMapLen);
	dmaCopy(ice_floorPal, &SH_BG_PALETTE[128], ice_floorPalLen);	
	
	SH_BG_PALETTE[0] = Astralbrew::Drawing::Colors::White;	
	
	map = new Map(ms);
	viewer = new MapViewer(map, 3);				
	
	camera.set_bounds(map->px_width(), map->px_height());
	
	camera.set_x(map->px_width()/2);
	camera.set_y(80);
	
	bgInit(0, BgSize::Text256x256, BgPaletteType::Pal4bit, 0, 5);
	vwf.set_render_space((void*)0x06004800,14,30);
	VwfEngine::prepare_map(vwf, bgGetMapPtr(0), 32, 1, 0, 0xA);
	vwf.clear(BgPaletteType::Pal4bit);
	
	SH_BG_PALETTE[0xA1] = Colors::DarkGreen;
	SH_BG_PALETTE[0xA2] = Colors::Red;
	SH_BG_PALETTE[0xA3] = Colors::White;
	SH_BG_PALETTE[0xA4] = Colors::Yellow;	
	
	frame();


	shpal_fade();	
	
	irqSet(IRQ_VBLANK, mmVBlank);
	if(!mmActive())
	{
		mmInitDefault((mm_addr)soundbank_bin, 2);
		mmStart(MOD_MAP_THEME, MM_PLAY_LOOP);	
	}
}

void CreditsScene::before_frame()
{	
	if(mmActive()) mmFrame();
}

#include <math.h>

int strwidth(const char* str)
{
	int mw = 0;
	int cw = 0;
	
	for(;*str;++str) 
	{
		if(*str=='\n') 
		{
			if(cw>mw)
			{
				mw = cw;
				cw = 0;
			}
		}			
		else cw++;
	}	
	
	return cw>mw ? cw:mw;
}

void CreditsScene::frame()
{
	viewer->set_scroll(camera.get_x(), camera.get_y());
	x=(int)(sin(M_PI*t/512)*(map->px_width()/2-120)) + map->px_width()/2;
	t++;
	if(t==1024) t=0;	
	y+=sgn==1 ? sf24(0,128) : -sf24(0,128);
	camera.set_x(x);
	camera.set_y(y);
	
	if(y>=map->px_height()-400 && t==0) 
	{
		sgn=-1;
	}
	else if(y<400 && t==0)
	{
		sgn=1;
	}
	
	bgSetScroll(2, camera.get_x() & 0xFF, camera.get_y() & 0xFF);
	
	if(msgcounter>0)
	{
		msgcounter--;
	}	
	else 
	{
		if(*msgindex)
		{		
			vwf.clear(Pal4bit);
			
			bgSetScroll(0,strwidth(msgindex)*3-120,-32 - rand()%40);
			
			vwf.put_text(msgindex, Pal4bit, SolidColorBrush(0x4));
			
			msgindex+=strlen(msgindex)+1;
						
			msgcounter=200;
		}
		else 
		{
			if(title_next)
				close()->next(new TitleScene());
			else 
				close()->next(new OptionsScene());
			}
	}
	bgUpdate();		
		
}

void CreditsScene::on_key_down(int keys)
{
	if(keys & KEY_A)
	{
		if(title_next)
			close()->next(new TitleScene());
		else 
			close()->next(new OptionsScene());
	}
}

CreditsScene::~CreditsScene() 
{	
	bgSetScroll(0,0,0);
	bgUpdate();	
	irqEnable(IRQ_HBLANK);		
	delete map;
	delete viewer;
}