#include "MenuScene.h"
#include "SettingsScene.h"
#include "GameScene.h"
#include "Resources.h"

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();

	scene->addChild(layer);
	return scene;
}

bool MenuScene::init()
{
	if (!Layer::init())
		return false;

    // tocar musica com loop
    SimpleAudioEngine::getInstance()->playBackgroundMusic(themeMusicFile, true);
    
    
	// tamanho e configuração de fonte
	Size size = Director::getInstance()->getWinSize();
	TTFConfig ttfConfig(font_riffic, 64.0f);

	// background do menu
	auto backgroundSprite = Sprite::create(backgroundFile1);
	backgroundSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(backgroundSprite);

	// label de título
	auto label = Label::createWithTTF(ttfConfig, "GAME NAME", TextHAlignment::CENTER);
	label->setPosition(Vec2(size.width / 2, size.height * 0.75f));
	label->setColor(Color3B::BLACK);
	this->addChild(label);

	// item de settings
	auto settingsItem = MenuItemImage::create(
		uiSettingsNormal,
		uiSettingsPressed,
		this,
		menu_selector(MenuScene::settingsCallback));
	settingsItem->setPosition(Vec2(size.width * 0.25f, size.height * 0.25f));

	// item jogar
	auto playItem = MenuItemImage::create(
		uiPlayNormal,
		uiPlayPressed,
		this,
		menu_selector(MenuScene::playCallback));
	playItem->setPosition(Vec2(size.width / 2, size.height * 0.25f));

	// item ranking
	auto rankingItem = MenuItemImage::create(
		uiRankingNormal,
		uiRankingPressed,
		this,
		menu_selector(MenuScene::rankingCallback));
	rankingItem->setPosition(Vec2(size.width * 0.75f, size.height * 0.25f));

	// menu com opções
	auto menu = Menu::create(settingsItem, playItem, rankingItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

// transita para Settings
void MenuScene::settingsCallback(Ref* sender)
{
	Director::getInstance()->replaceScene(SettingsScene::createScene());
}

// transita para a GameScene
void MenuScene::playCallback(Ref* sender)
{
	Director::getInstance()->replaceScene(GameScene::createScene());
}

// transita para o ranking
void MenuScene::rankingCallback(Ref* sender)
{

}
