#include <Astralbrew>

#include "map_scene.hpp"
#include "title_scene.hpp"
#include "langs.h"
#include "menu_scene.hpp"
#include "save_file.hpp"

using namespace Astralbrew::Scenes;

class _SplashScreen : public Astralbrew::Scenes::DefaultSplashScreen {
	inline void on_end() override {
		auto* com = this->close();		
		Scene* scene = new TitleScene();
		com->next(scene);
	}
};

void Astralbrew::World::init_main_scene() 
{
	SAVE_FILE.load();
	set_current_language(SAVE_FILE.data().language);
	Astralbrew::World::__MAIN_SCENE__ = new _SplashScreen(); 
}