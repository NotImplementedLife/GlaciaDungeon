#pragma once

#include <Astralbrew>

using namespace Astralbrew::Scenes;

void unload_menu_master_sound();

class LanguageSelectScene : public SimpleListScene
{
private:
	bool title_next;
public:
	LanguageSelectScene(bool title_next = false);
	void init() override;	
	void on_selection_done(int index) override;
	void on_cancel_triggered() override;
	void before_frame() override;
	~LanguageSelectScene();
};

class MenuScene : public SimpleListScene
{
private:
	bool pass_down_sound = false;
	int sel=0;
public:
	MenuScene(int sel=0);
	void init() override;
	void on_selection_done(int index) override;	
	void on_cancel_triggered() override;
	void before_frame() override;
	~MenuScene();
};
