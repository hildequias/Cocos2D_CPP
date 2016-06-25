#ifndef __SEAGULL_H__
#define __SEAGULL_H__

#include "cocos2d.h"
using namespace cocos2d;

class Seagull : public Sprite
{
public:
	virtual bool init();
	CREATE_FUNC(Seagull);

	void activateMovement();

	void flip();
private:
	Size size;
};

#endif