#ifndef __SAILOR_H__
#define __SAILOR_H__

#include "cocos2d.h"
using namespace cocos2d;

class Sailor : public Sprite
{
public:
	virtual bool init();
	CREATE_FUNC(Sailor);

	void activateIdleAnimation();
};

#endif