#include <Astralbrew>

#include "map_scene.hpp"
#include "title_scene.hpp"
#include "langs.h"

class _SplashScreen : public Astralbrew::Scenes::DefaultSplashScreen {
	inline void on_end() override {
		auto* com = this->close();
		Scene* scene = new TitleScene();
		com->next(scene);
	}
};
void Astralbrew::World::init_main_scene() 
{
	set_current_language(LANG_ES);
	Astralbrew::World::__MAIN_SCENE__ = new _SplashScreen(); 
}
