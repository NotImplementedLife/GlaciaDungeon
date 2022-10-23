#include "menu_scene.hpp"

#include "title_scene.hpp"
#include "map_scene.hpp"
#include "langs.h"
#include "save_file.hpp"

using namespace Astralbrew::Video;
#include "ice_floor.h"

LanugageSelectScene::LanugageSelectScene(bool title_next) : SimpleListScene(), title_next(title_next)
{
	int crt_lang = CURRENT_LANGUAGE;
	for(int i=0;i<LANGUAGES_COUNT;i++)
	{
		set_current_language(i);
		add_item(get_message(LMSG_LANG_NAME));
	}			
	set_current_language(crt_lang);
}
	
void LanugageSelectScene::init() 
{
	Utils::zeroize((void*)0x06000010, 0x18000-0x10); 
	SimpleListScene::init();	
	bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 6);
	
	BG_PALETTE[0] = Astralbrew::Drawing::Colors::Blue;
	
	dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, (ice_floorTilesLen-32)/2);
	dmaCopy(ice_floorPal, &BG_PALETTE[16], (ice_floorPalLen+3)/4*4);
	
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]=0x600;
	dmaCopy(ice_floorMap, bgGetMapPtr(3), ice_floorMapLen/4);
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]|=0x1000;
	bgSetScroll(3,0,140);
	bgUpdate();	
	select(CURRENT_LANGUAGE);		
}
	
void LanugageSelectScene::on_selection_done(int index) 
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
		close()->next(new MenuScene());
	}			
}
	
void LanugageSelectScene::on_cancel_triggered() 
{
	if(title_next) return;	
	close()->next(new MenuScene());		
}

MenuScene::MenuScene() : SimpleListScene()
{
	add_item(get_message(LMSG_MENU_NEW_GAME));	
	add_item(get_message(LMSG_MENU_CONTINUE), SAVE_FILE.data().current_level != -1);
	add_item(get_message(LMSG_MENU_LEVEL_SEL));
	add_item(get_message(LMSG_MENU_LANGUAGE));
}

void MenuScene::init()
{
	Utils::zeroize((void*)0x06000010, 0x18000-0x10); 
	SimpleListScene::init();	
	bgInit(3, BgSize::Text256x256, BgPaletteType::Pal4bit, 2, 6);
	
	BG_PALETTE[0] = Astralbrew::Drawing::Colors::Blue;
	
	dmaCopy(((u8*)ice_floorTiles)+32, (int*)0x06008000, (ice_floorTilesLen-32)/2);
	dmaCopy(ice_floorPal, &BG_PALETTE[16], (ice_floorPalLen+3)/4*4);
	
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]=0x600;
	dmaCopy(ice_floorMap, bgGetMapPtr(3), ice_floorMapLen/4);
	for(int i=0;i<1024;i++) bgGetMapPtr(3)[i]|=0x1000;
	bgSetScroll(3,0,140);
	bgUpdate();		
}

void MenuScene::on_selection_done(int index)
{
	switch(index)
	{
		case 0: close()->next(new MapScene(&MAP_STATS[0])); break;
		case 1: close()->next(new MapScene(&MAP_STATS[SAVE_FILE.data().current_level])); break;
		case 2: break;
		case 3: close()->next(new LanugageSelectScene()); break;
	}	
}

void MenuScene::on_cancel_triggered() 
{
	close()->next(new TitleScene());
}