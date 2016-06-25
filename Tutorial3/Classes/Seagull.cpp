#include "Seagull.h"
#include "Resources.h"

// init da gaivota
bool Seagull::init()
{
	if (!Sprite::init())
		return false;

	this->initWithSpriteFrameName(seagullSpriteFile0);

	size = Director::getInstance()->getWinSize();

	return true;
}

// ativa animação de movimento
void Seagull::activateMovement()
{
	// vetor de sprite frames
	Vector<SpriteFrame*> animFrames(4);

	// cria sprites de referência
	auto frame1 = Sprite::createWithSpriteFrameName(seagullSpriteFile0);
	auto frame2 = Sprite::createWithSpriteFrameName(seagullSpriteFile1);
	auto frame3 = Sprite::createWithSpriteFrameName(seagullSpriteFile2);
	auto frame4 = Sprite::createWithSpriteFrameName(seagullSpriteFile3);

	// adiciona frames no vetor
	animFrames.pushBack(frame1->getSpriteFrame());
	animFrames.pushBack(frame2->getSpriteFrame());
	animFrames.pushBack(frame3->getSpriteFrame());
	animFrames.pushBack(frame4->getSpriteFrame());

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	auto animate = Animate::create(animation);
	auto moveRight = MoveTo::create(2.0f, Vec2(size.width - (this->getContentSize().width / 2), this->getPosition().y));
	auto moveLeft = MoveTo::create(2.0f, Vec2((this->getContentSize().width / 2), this->getPosition().y));
	auto flipCallback = CallFunc::create(CC_CALLBACK_0(Seagull::flip, this));

	this->runAction(RepeatForever::create(animate));
	this->runAction(RepeatForever::create(Sequence::create(moveRight, flipCallback, moveLeft, flipCallback, NULL)));
}

void Seagull::flip()
{
	this->setFlippedX(!this->isFlippedX());
}