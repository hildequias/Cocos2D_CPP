#include "Sailor.h"
#include "Resources.h"

// init do marinheiro
bool Sailor::init()
{
	if (!Sprite::init())
		return false;

	this->initWithSpriteFrameName(sailorSpriteArray[0]);

	return true;
}

// ativa anima��o de IDLE
void Sailor::activateIdleAnimation()
{
	// vetor de sprite frames
	Vector<SpriteFrame*> animFrames(25);

	for (int i = 0; i < 26; i++)
	{
		// cria sprites de refer�ncia
		Sprite* frame = Sprite::createWithSpriteFrameName(sailorSpriteArray[i]);

		// adiciona frames no vetor
		animFrames.pushBack(frame->getSpriteFrame());
	}

	// cria anima��o com delay de 0.05f entre cada frame
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	auto animate = Animate::create(animation);

	// para e limpa qualquer a��o realizada anteriormente pelo objeto 
	this->stopAllActions();
	this->cleanup();

	// seta espelho no eixo X
	this->setFlippedX(false);

	// cria a��o para repetir 
	this->runAction(RepeatForever::create(animate));
}