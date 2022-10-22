#include "menu_scene.hpp"

#include "title_scene.hpp"
#include "map_scene.hpp"
#include "langs.h"

class LanugageSelectScene : public SimpleListScene
{
public:
	LanugageSelectScene() : SimpleListScene()
	{
		get_items().push_back(new ListOption("English"));	
		get_items().push_back(new ListOption("Espanol"));
		get_items().push_back(new ListOption("Romana"));	
		get_items().push_back(new ListOption("Russkiy"));
	}
	
	void init() override
	{
		SimpleListScene::init();
		select(CURRENT_LANGUAGE);
	}
	
	void on_selection_done(int index) override
	{
		set_current_language(index);
		close()->next(new MenuScene());
	}
	
	void on_cancel_triggered() override
	{
		close()->next(new MenuScene());
	}
};


MenuScene::MenuScene() : SimpleListScene()
{
	get_items().push_back(new ListOption("New game"));	
	get_items().push_back(new ListOption("Continue"));
	get_items().push_back(new ListOption("Language"));	
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