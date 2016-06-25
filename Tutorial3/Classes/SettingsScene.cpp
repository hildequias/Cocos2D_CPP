#include "SettingsScene.h"
#include "MenuScene.h"
#include "Resources.h"

Scene* SettingsScene::createScene()
{
	// criando a cena
	auto scene = Scene::create();
	auto layer = SettingsScene::create();

	scene->addChild(layer);
	return scene;
}

bool SettingsScene::init()
{
	if (!Layer::init())
		return false;

	// armazenando o tamanho da tela 
	Size size = Director::getInstance()->getWinSize();

	TTFConfig ttfConfig(font_riffic, 48.0f);

	// adicionando background 
	auto backgroundSprite = Sprite::create(backgroundFile1);
	backgroundSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(backgroundSprite);

	// label Settings
	auto labelSettings = Label::createWithTTF(ttfConfig, "SETTINGS", TextHAlignment::CENTER);
	labelSettings->setPosition(Vec2(size.width / 2, size.height * 0.80f));
	labelSettings->setColor(Color3B::BLACK);
	this->addChild(labelSettings);

	// adicionando painel de settings
	auto panelSprite = Sprite::create(panelFile);
	panelSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(panelSprite);

	// items de music e toggle
	auto musicOnItem = MenuItemImage::create(uiMusicNormal, uiMusicPressed);
	auto musicOffItem = MenuItemImage::create(uiMusicDisabled, uiMusicDisabled);

	auto musicToggle = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(SettingsScene::musicCallback, this),
		musicOnItem,
		musicOffItem,
		NULL);
	musicToggle->setPosition(Vec2(size.width * 0.30f, size.height * 0.60f));

	// items de sound effects e toggle
	auto sfxOnItem = MenuItemImage::create(uiSoundFxNormal, uiSoundFxPressed);
	auto sfxOffItem = MenuItemImage::create(uiSoundFxDisabled, uiSoundFxDisabled);

	auto sfxToggle = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(SettingsScene::sfxCallback, this),
		sfxOnItem,
		sfxOffItem,
		NULL);
	sfxToggle->setPosition(Vec2(size.width * 0.30f, size.height * 0.45f));

	// labels para os bot�es
	auto musicLabel = Label::createWithTTF(ttfConfig, "MUSIC", TextHAlignment::CENTER);
	musicLabel->setPosition(Vec2(size.width * 0.60f, musicToggle->getPosition().y));
	musicLabel->setColor(Color3B::BLACK);
	this->addChild(musicLabel);

	auto sfxLabel = Label::createWithTTF(ttfConfig, "SFX", TextHAlignment::CENTER);
	sfxLabel->setPosition(Vec2(size.width * 0.60f, sfxToggle->getPosition().y));
	sfxLabel->setColor(Color3B::BLACK);
	this->addChild(sfxLabel);

	// cria item de voltar
	auto backItem = MenuItemImage::create(
		uiBackNormal,
		uiBackPressed,
		this,
		menu_selector(SettingsScene::backCallback));
	backItem->setPosition(Vec2(size.width * 0.15f, size.height * 0.15f));

	// cria menu e adiciona item
	auto menu = Menu::create(musicToggle, sfxToggle, backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void SettingsScene::musicCallback(Ref* sender)
{
}

void SettingsScene::sfxCallback(Ref* sender)
{
}

void SettingsScene::backCallback(Ref* sender)
{

	// retorna para MenuScene
	Director::getInstance()->replaceScene(MenuScene::createScene());
}
