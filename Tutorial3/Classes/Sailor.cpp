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

// ativa animação de IDLE
void Sailor::activateIdleAnimation()
{
	// vetor de sprite frames
	Vector<SpriteFrame*> animFrames(25);

	for (int i = 0; i < 26; i++)
	{
		// cria sprites de referência
		Sprite* frame = Sprite::createWithSpriteFrameName(sailorSpriteArray[i]);

		// adiciona frames no vetor
		animFrames.pushBack(frame->getSpriteFrame());
	}

	// cria animação com delay de 0.05f entre cada frame
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	auto animate = Animate::create(animation);

	// para e limpa qualquer ação realizada anteriormente pelo objeto 
	this->stopAllActions();
	this->cleanup();

	// seta espelho no eixo X
	this->setFlippedX(false);

	// cria ação para repetir 
	this->runAction(RepeatForever::create(animate));
}