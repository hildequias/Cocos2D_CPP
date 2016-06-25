#include "SplashScene.h"
#include "MenuScene.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Scene* SplashScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SplashScene::create();

	scene->addChild(layer);
	return scene;
}

bool SplashScene::init()
{
	if (!Layer::init())
		return false;

	// Tamanho da tela
	Size size = Director::getInstance()->getWinSize();

	// Carrega texturas na memória
	loadTextures();

	// Carrega sons na memória
	loadSounds();

	// sprite de splash
	auto splash = Sprite::create(splashFile);
	splash->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(splash);

	// criando callback, delay de 3 segundos e sequência
	auto loading = CallFunc::create(CC_CALLBACK_0(SplashScene::updateScene, this));
	auto delay = DelayTime::create(3.0f);
	auto sequence = Sequence::create(delay, loading, NULL);
	
	// executa a sequencia criada
	this->runAction(sequence);

	return true;
}

void SplashScene::loadTextures()
{
	// Carregando as texturas em memória
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(sailorIdlePlistFile);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(extrasPlistFile);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(seaPlistFile);
}

void SplashScene::loadSounds()
{
	SimpleAudioEngine::getInstance()->preloadEffect(buttonFxFile);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(themeMusicFile);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(ingameMusicFile);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(gameoverMusicFile);
}

// função para transição entre splash e menu (será chamada após 3 segundos)
void SplashScene::updateScene()
{
	Director::getInstance()->replaceScene(MenuScene::createScene());
}
