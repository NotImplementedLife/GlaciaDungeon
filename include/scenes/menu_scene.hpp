#pragma once

#include <Astralbrew>

using namespace Astralbrew::Scenes;

class MenuScene : public SimpleListScene
{
public:
	MenuScene();
	void on_selection_done(int index) override;	
	void on_cancel_triggered() override;
};
