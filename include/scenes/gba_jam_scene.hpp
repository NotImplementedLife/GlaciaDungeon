#pragma once

#define GBAJAM

#include <Astralbrew>

using namespace Astralbrew;
using namespace Astralbrew::World;

class GBAJamScene : public Scene
{
private:
	int count=0;	
public:
	virtual void init() override;
	virtual void frame() override;
};