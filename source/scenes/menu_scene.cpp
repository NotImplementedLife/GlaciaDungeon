#include "menu_scene.hpp"

#include "title_scene.hpp"
#include "map_scene.hpp"
#include "langs.h"
#include "save_file.hpp"

class LanugageSelectScene : public SimpleListScene
{
public:
	LanugageSelectScene() : SimpleListScene()
	{
		int crt_lang = CURRENT_LANGUAGE;
		for(int i=0;i<LANGUAGES_COUNT;i++)
		{
			set_current_language(i);
			add_item(get_message(LMSG_LANG_NAME));
		}			
		set_current_language(crt_lang);
	}
	
	void init() override
	{
		SimpleListScene::init();
		select(CURRENT_LANGUAGE);
	}
	
	void on_selection_done(int index) override
	{
		set_current_language(index);
		SAVE_FILE.data().language = index;
		SAVE_FILE.save();
		close()->next(new MenuScene());
	}
	
	void on_cancel_triggered() override
	{
		close()->next(new MenuScene());
	}
};


MenuScene::MenuScene() : SimpleListScene()
{
	add_item(get_message(LMSG_MENU_NEW_GAME));
	add_item(get_message(LMSG_MENU_CONTINUE), false);
	add_item(get_message(LMSG_MENU_LANGUAGE));
}

void MenuScene::on_selection_done(int index)
{
	switch(index)
	{
		case 0: close()->next(new MapScene(&MAP_STATS[0])); break;
		case 1: close()->next(new MapScene(&MAP_STATS[0])); break;
		case 2: close()->next(new LanugageSelectScene()); break;
	}	
}

void MenuScene::on_cancel_triggered() 
{
	close()->next(new TitleScene());
}