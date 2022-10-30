#include "menu_scene.hpp"

#include "title_scene.hpp"
#include "map_scene.hpp"
#include "map_select_scene.hpp"
#include "langs.h"
#include "save_file.hpp"

#include "soundbank.h"
#include "soundbank_bin.h"
#include <maxmod.h>

using namespace Astralbrew::Video;
#include "ice_floor.h"

namespace 
{
	__attribute__((section(".ewram.menu_parallax.scrolls"))) short scrolls[64];
	__attribute__((section(".ewram.menu__parallax"))) short scrdir=1;	
	
	void parallax()
	{			
		if(REG_VCOUNT<16 || REG_VCOUNT == 160)
		{
			BG_PALETTE[0] = (25<<10)|(26<<5)|26;
			REG_BG3HOFS = 0;
		}		
		else if(REG_VCOUNT<94)		
		{
			int c=(94-REG_VCOUNT)/3;
			BG_PALETTE[0] = (25<<10)|(c<<5)|c;
			REG_BG3HOFS = 0;
		}
		else if(REG_VCOUNT<159)
		{
			int i=REG_VCOUNT-95;
			REG_BG3HOFS = scrolls[i];
			if(scrdir==1)
				scrolls[i]+=(i)/6+1;
			else
				scrolls[i]-=(i)/6+1;		
		}
	}
}


int menu_master_sound_cnt = 0;

void load_menu_master_sound()
{
	mmSetModuleVolume(1024);
	if(menu_master_sound_cnt==0)
	{
		irqSet(IRQ_VBLANK, mmVBlank);		
		irqEnable(IRQ_VBLANK);	
		mmInitDefault((mm_addr)soundbank_bin, 1);
		mmStart(MOD_MENU_THEME, MM_PLAY_LOOP);		
	}
	menu_master_sound_cnt = 1;
}

void unload_menu_master_sound()
{
	if(menu_master_sound_cnt==0) return;
	menu_master_sound_cnt = 0;
	mmPause();
	mmStop();
	irqSet(IRQ_VBLANK, nullptr);	
}

LanguageSelectScene::LanguageSelectScene(bool title_next) : SimpleListScene(), title_next(title_next)
{
	int crt_lang = CURRENT_LANGUAGE;
	for(int i=0;i<LANGUAGES_COUNT;i++)
	{
		set_current_language(i);
		add_item(get_message(LMSG_LANG_NAME));
	}			
	set_current_language(crt_lang);
}
	
void LanguageSelectScene::init() 
{
	Utils::zeroize((void*)0x06000010, 0x18000-0x10); 
	SimpleListScene::init();	
	bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 6);
	
	BG_PALETTE[0] = 25<<10;
	
	dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, (ice_floorTilesLen-32)/2);
	dmaCopy(ice_floorPal, &BG_PALETTE[16], (ice_floorPalLen+3)/4*4);
	
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]=0x600;
	dmaCopy(ice_floorMap, bgGetMapPtr(3), ice_floorMapLen/4);
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]|=0x1000;
	bgSetScroll(3,0,140);
	bgUpdate();	
	select(CURRENT_LANGUAGE);		
		
}

void LanguageSelectScene::before_frame()
{
	if(mmActive()) mmFrame();
}
	
void LanguageSelectScene::on_selection_done(int index) 
{	
	set_current_language(index);
	SAVE_FILE.data().language = index;
	SAVE_FILE.save();
	if(title_next)
	{		
		close()->next(new TitleScene());
	}
	else
	{
		close()->next(new MenuScene(3));
	}			
}
	
void LanguageSelectScene::on_cancel_triggered() 
{
	if(title_next) return;		
	close()->next(new MenuScene());		
}

LanguageSelectScene::~LanguageSelectScene()
{	
}


#include "credits_scene.hpp"


OptionsScene::OptionsScene()
{
	add_item(get_message(LMSG_MENU_LANGUAGE));	
	add_item(get_message(LMSG_MENU_CREDITS));
}

void OptionsScene::init()
{
	Utils::zeroize((void*)0x06000010, 0x18000-0x10); 
	SimpleListScene::init();	
	bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 6);
	
	BG_PALETTE[0] = 25<<10;
	
	dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, (ice_floorTilesLen-32)/2);
	dmaCopy(ice_floorPal, &BG_PALETTE[16], (ice_floorPalLen+3)/4*4);
	
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]=0x600;
	dmaCopy(ice_floorMap, bgGetMapPtr(3), ice_floorMapLen/4);
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]|=0x1000;
	bgSetScroll(3,0,140);
	bgUpdate();
	
}

void OptionsScene::on_selection_done(int index)
{
	if(index==0) 
	{
		close()->next(new LanguageSelectScene());
	}			
	else if(index==1)
	{
		close()->next(new CreditsScene(false));
	}
}

void OptionsScene::on_cancel_triggered()
{
	close()->next(new MenuScene());				
}

void OptionsScene::before_frame() 
{
	mmFrame();		
}

OptionsScene::~OptionsScene()
{
	
}

MenuScene::MenuScene(int sel) : SimpleListScene(), sel(sel)
{
	add_item(get_message(LMSG_MENU_NEW_GAME));	
	add_item(get_message(LMSG_MENU_CONTINUE), SAVE_FILE.data().current_level != -1);
	add_item(get_message(LMSG_MENU_LEVEL_SEL));
	add_item(get_message(LMSG_MENU_OPTIONS));		
}

void MenuScene::init()
{
	for(int i=0;i<64;i++) scrolls[i]=0;
	Utils::zeroize((void*)0x06000010, 0x18000-0x10); 
	SimpleListScene::init();	
	bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 6);
	
	BG_PALETTE[0] = 25<<10;
	
	dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, (ice_floorTilesLen-32)/2);
	dmaCopy(ice_floorPal, &BG_PALETTE[16], (ice_floorPalLen+3)/4*4);
	
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]=0x600;
	dmaCopy(ice_floorMap, bgGetMapPtr(3), ice_floorMapLen/4);
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]|=0x1000;
	bgSetScroll(3,0,140);
	bgUpdate();		
	
	load_menu_master_sound();
	irqEnable(IRQ_HBLANK);
	irqSet(IRQ_HBLANK, parallax);
	if(sel==0 && SAVE_FILE.data().current_level != -1)
	{
		select(1); // continue
	}
	else select(sel);	
}

void MenuScene::before_frame()
{
	if(mmActive()) mmFrame();
}

void MenuScene::on_selection_done(int index)
{	
	if(index>=2) pass_down_sound = true;
	if(index!=3) {
		irqDisable(IRQ_HBLANK);
		irqSet(IRQ_HBLANK, nullptr);
	}
	switch(index)
	{
		case 0: 
		{
			SAVE_FILE.init_data();
			SAVE_FILE.data().language = CURRENT_LANGUAGE;
			close()->next(new MapScene(&MAP_STATS[0])); break;
		}
		case 1: close()->next(new MapScene(&MAP_STATS[SAVE_FILE.data().current_level])); break;
		case 2: mmSetModuleVolume(0); close()->next(new MapSelectScene()); break;
		case 3: close()->next(new OptionsScene()); break;
	}	
}

void MenuScene::on_cancel_triggered() 
{
	mmSetModuleVolume(0);
	mmPause();
	mmStop();
	close()->next(new TitleScene());
}

MenuScene::~MenuScene()
{	
	if(!pass_down_sound)
		unload_menu_master_sound();	
}