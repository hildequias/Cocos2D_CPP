#ifndef __WOOD_H__
#define __WOOD_H__

#include "cocos2d.h"

using namespace cocos2d;

class Wood : public Sprite
{
public:
	virtual bool init();
	CREATE_FUNC(Wood);

	void activateAnimation();
};

#endif