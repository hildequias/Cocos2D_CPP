#include "HudLayer.h"
#include "Resources.h"
#include "GameScene.h"

// init do marinheiro
bool HudLayer::init()
{
    if (!Layer::init())
        return false;
    
    size = Director::getInstance()->getWinSize();
    
    initScore();
    initPauseMenu();
    
    return true;
}

void HudLayer::initScore()
{
    auto chestSprite = Sprite::create(chestFile);
    chestSprite->setPosition(Vec2(size.width * 0.05f, size.height * 0.88f));
    this->addChild(chestSprite);
    chestSprite->setScale(0.5f);
    
    TTFConfig ttfConfigTreasure(font_riffic, 36.0f);
    
    scoreLabel = Label::createWithTTF(ttfConfigTreasure, "0", TextHAlignment::CENTER);
    scoreLabel->setPosition(Vec2(chestSprite->getPosition().x + chestSprite->getContentSize().width / 2 + 5, size.height * 0.88f));
    scoreLabel->setColor(Color3B::WHITE);
    scoreLabel->enableOutline(Color4B::BLACK, 2);
    this->addChild(scoreLabel);
}

void HudLayer::initPauseMenu()
{
    // criando item de pausa
    auto pauseItem = MenuItemImage::create(
                                           uiPauseNormal,
                                           uiPausePressed,
                                           this,
                                           menu_selector(HudLayer::pauseCallback));
    pauseItem->setPosition(Vec2(size.width * 0.90f, size.height * 0.88f));
    pauseItem->setScale(0.75f);
    
    // criando menu e adicionando pausa
    auto menu = Menu::create(pauseItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void HudLayer::pauseCallback(Ref* sender)
{
//    GameScene::getInstance()->pauseGame();
}