//
//  MenuScene.cpp
//  TutorialCPP
//
//  Created by Mobile6 on 6/14/16.
//
//

#include "MenuScene.h"
#include "SettingsScene.h"
#include "GameScene.h"

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
    
    Size size = Director::getInstance()->getWinSize();
    TTFConfig ttfConfig("fonts/Marker Felt.ttf", 48.0f);
    
    auto backgroundSprite = Sprite::create("background1.png");
    backgroundSprite->setPosition(Vec2(size.width/2, size.height/2));
    this->addChild(backgroundSprite);
    
    auto label = Label::createWithTTF(ttfConfig, "GAME NAME", TextHAlignment::CENTER);
    label->setPosition(Vec2(size.width / 2, size.height * 0.75f));
    label->setColor(Color3B::BLACK);
    this->addChild(label);
    
    auto settingsItem = MenuItemImage::create(
                                              "settings_normal.png",
                                              "settings_pressed.png",
                                              this,
                                              menu_selector(MenuScene::settingsCallback));
    
    settingsItem->setPosition(Vec2(size.width * 0.25f, size.height * 0.25f));
    
    auto playItem = MenuItemImage::create(
                                          "play_normal.png",
                                          "play_pressed.png",
                                          this,
                                          menu_selector(MenuScene::playCallback));
    
    playItem->setPosition(Vec2(size.width / 2, size.height * 0.25f));
    
    auto rankingItem = MenuItemImage::create(
                                             "ranking_normal.png",
                                             "ranking_pressed.png",
                                             this,
                                             menu_selector(MenuScene::rankingCallback));
                                             
    rankingItem->setPosition(Vec2(size.width * 0.75f, size.height * 0.25f));
    
    auto menu = Menu::create(settingsItem, playItem, rankingItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    
    return true;
}

void MenuScene::settingsCallback(Ref* sender)
{
    Director::getInstance()->replaceScene(SettingsScene::createScene());
}

void MenuScene::playCallback(Ref* sender)
{
    Director::getInstance()->replaceScene(GameScene::createScene());
}

void MenuScene::rankingCallback(Ref* sender)
{
    
}