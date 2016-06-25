#include "Wood.h"
#include "Resources.h"

// init da madeira
bool Wood::init()
{
	if (!Sprite::init())
		return false;

	// inicializa madeira com a primeira sprite
	this->initWithSpriteFrameName(woodSpriteFile0);

	return true;
}

// ativa animação da madeira
void Wood::activateAnimation()
{
	// vetor de sprite frames
	Vector<SpriteFrame*> animFrames(3);

	auto frame0 = Sprite::createWithSpriteFrameName(woodSpriteFile0);
	auto frame1 = Sprite::createWithSpriteFrameName(woodSpriteFile1);
	auto frame2 = Sprite::createWithSpriteFrameName(woodSpriteFile2);
	animFrames.pushBack(frame0->getSpriteFrame());
	animFrames.pushBack(frame1->getSpriteFrame());
	animFrames.pushBack(frame2->getSpriteFrame());
	animFrames.pushBack(frame1->getSpriteFrame());

	// cria animação com delay de 0.2f entre cada frame
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	auto animate = Animate::create(animation);

	// executa animação da madeira
	this->runAction(RepeatForever::create(animate));
}
