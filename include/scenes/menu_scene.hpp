#pragma once

#include <Astralbrew>

using namespace Astralbrew::Scenes;

class LanugageSelectScene : public SimpleListScene
{
private:
	bool title_next;
public:
	LanugageSelectScene(bool title_next = false);
	void init() override;
	void on_selection_done(int index) override;
	void on_cancel_triggered() override;
};

class MenuScene : public SimpleListScene
{
public:
	MenuScene();
	void init() override;
	void on_selection_done(int index) override;	
	void on_cancel_triggered() override;
};
