#include "title_scene.hpp"
#include "map_scene.hpp"

#include <stdio.h>

#include "ice_floor.h"
#include "title.h"
#include "settings_icon.h"
#include "menu_scene.hpp"

#include "soundbank.h"
#include "soundbank_bin.h"
#include <maxmod.h>
#include "fader.h"

using namespace Astralbrew::Video;

__attribute__((section(".ewram.title_parallax.scrolls"))) short scrolls[64];
__attribute__((section(".ewram.title_parallax"))) short scrdir=1;
__attribute__((section(".ewram.title_parallax"))) short pal0;

void parallax()
{	
	if(REG_VCOUNT<31)
	{
		BG_PALETTE[0] = Astralbrew::Drawing::Colors::White;
		REG_BG3HOFS = 0;
	}		
	else if(REG_VCOUNT<94)		
	{
		int c=(94-REG_VCOUNT)/2;
		BG_PALETTE[0] = (31<<10)|(c<<5)|c;
		REG_BG3HOFS = 0;
	}
	else if(REG_VCOUNT<159)
	{
		int i=REG_VCOUNT-95;
		REG_BG3HOFS = scrolls[i];
		if(scrdir==1)
			scrolls[i]+=(i)/4+1;
		else
			scrolls[i]-=(i)/4+1;		
	}
}

void TitleScene::init()
{	
	Utils::zeroize((void*)0x06000000, 0x18000);
	
	for(int i=0;i<64;i++) scrolls[i]=0;
	Video::setMode(0);
	
	bgInit(2, BgSize::Text256x256, BgPaletteType::Pal8bit, 1, 5);
	bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 6);
	
	bgSetScroll(2,0,0);
	bgUpdate();
	
	Address transparentTile;
	vram_chr_1.reserve(&transparentTile, 32);
	
	dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, (ice_floorTilesLen-32)/2);
	
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]=0x600;
	dmaCopy(ice_floorMap, bgGetMapPtr(3), ice_floorMapLen/4);
	bgSetScroll(3,0,160);
	bgUpdate();	
	
	dmaCopy(titleTiles, bgGetTilesPtr(2), titleTilesLen);
	dmaCopy(titlePal, &SH_BG_PALETTE[16], titlePalLen);
	for(int i=0;i<titleTilesLen/2;i++)
	{
		short x = bgGetTilesPtr(2)[i];
		if(x&0x00FF) x+=0x0010;
		if(x&0xFF00) x+=0x1000;
		bgGetTilesPtr(2)[i] = x;
	}
	dmaCopy(titleMap, bgGetMapPtr(2), titleMapLen);
	

	irqEnable(IRQ_HBLANK);
	irqSet(IRQ_HBLANK, parallax);
	
		
	
	dmaCopy(ice_floorPal, SH_BG_PALETTE, (ice_floorPalLen+3)/4*4);
	
	
	objEnable1D();
	player = new Player();
	player->set_movement_bounds(0,0,8000,8000);	
	
	player->get_attribute()->set_rotation_scaling(true);
	player->get_attribute()->set_affine_matrix(0);
	OamPool::set_rotation_matrix(0, 256, 0, 0, 256);
	
	
	player->set_position(120,132);	
	player->update_position(nullptr);	
	
	player->update();
	player->update_visual();	
	
	OamPool::deploy();
	
	irqSet(IRQ_VBLANK, mmVBlank);
	irqEnable(IRQ_VBLANK);
	
	mmInitDefault((mm_addr)soundbank_bin, 4);
	mmStart(MOD_TITLE_THEME, MM_PLAY_LOOP);
	
	shpal_fade();
}
	
void TitleScene::before_frame()
{
	mmFrame();
}
	
void TitleScene::frame()
{	
	player->move(a,0);
	mvcnt++;
	if(mvcnt==600)
	{
		mvcnt=0;
		a = -a;
		scrdir = -scrdir;
	}
	
	player->update();
	player->update_visual();	
	OamPool::deploy();
}

void TitleScene::on_key_down(int keys)
{
	if(keys & KEY_START)
	{
		irqDisable(IRQ_HBLANK);
		irqSet(IRQ_HBLANK, nullptr);
		REG_BG3HOFS = 0;
		close()->next(new MenuScene());
		//close()->next(new MapScene(&MAP_STATS[0]));				
	}
}

void TitleScene::on_key_held(int keys)
{
	if(keys & KEY_UP)
	{
		int x = player->get_px();
		int y = player->get_attribute()->get_y();
		//FATAL_ERROR(sf24(y).to_string());
		if(y>64)
		{
			y--;
			player->get_attribute()->set_y(y);
			player->set_position(x,y);		
			OamPool::set_rotation_matrix(0, 256+6*(100-y), 0, 0, 256+6*(100-y));
		}		
	}
	else if(keys & KEY_DOWN)
	{
		int x = player->get_px();
		int y = player->get_attribute()->get_y();		
		if(y<100)
		{
			y++;
			player->get_attribute()->set_y(y);
			player->set_position(x,y);		
			OamPool::set_rotation_matrix(0, 256+6*(100-y), 0, 0, 256+6*(100-y));
		}
	}
}

void TitleScene::launch_map()
{	
	close()->next(new MapScene());
}

TitleScene::~TitleScene()
{
	mmPause();
	mmStop();
	irqSet(IRQ_VBLANK, 0);
	delete player;	
	//OamPool::reset();	
	
	shpal_set_black();
	shpal_fade();
}