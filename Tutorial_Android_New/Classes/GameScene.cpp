//
//  GameScene.cpp
//  TutorialCPP
//
//  Created by Mobile6 on 6/14/16.
//
//

#include "GameScene.h"
#include "MenuScene.h"

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
        return false;
    
    size = Director::getInstance()->getWinSize();
    TTFConfig ttfConfig("fonts/Marker Felt.ttf", 40.0f);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan      = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved      = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded      = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchCancelled  = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    isTouchDown = false;
    
    initialTouchPosition[0] = 0;
    initialTouchPosition[1] = 0;
    
    auto label = Label::createWithTTF(ttfConfig, "Game is running...", TextHAlignment::CENTER);
    label->setPosition(Vec2(size.width / 2, size.height * 0.75f));
    label->setColor(Color3B::WHITE);
    this->addChild(label);
    
    auto sprite = Sprite::create("sailor.png");
    sprite->setPosition(Vec2(size.width / 2, size.height / 2));
    this->addChild(sprite);
    
    auto pauseItem = MenuItemImage::create(
                                           "pause_normal.png",
                                           "pause_pressed.png",
                                           this,
                                           menu_selector(GameScene::quitCallback));
    
    pauseItem->setPosition(Vec2(size.width * 0.85f, size.height * 0.90f));
    
    auto menu = Menu::create(pauseItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    scheduleUpdate();
    
    return true;
}

#pragma region input
bool GameScene::onTouchBegan(Touch* touch, Event* pEvent)
{
    //CCLOG("Touch Began");
    
    initialTouchPosition[0] = touch->getLocation().x;
    initialTouchPosition[1] = touch->getLocation().y;
    currentTouchPosition[0] = touch->getLocation().x;
    currentTouchPosition[1] = touch->getLocation().y;
    
    isTouchDown = true;
    
    return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* pEvent)
{
    //CCLOG("Touch Ended");
    
    isTouchDown = false;
}

void GameScene::onTouchMoved(Touch* touch, Event* pEvent)
{
    //CCLOG("Touch Moved");
    
    currentTouchPosition[0] = touch->getLocation().x;
    currentTouchPosition[1] = touch->getLocation().y;
}

void GameScene::onTouchCancelled(Touch* touch, Event* pEvent)
{
    onTouchEnded(touch, pEvent);
}

#pragma endregion input
void GameScene::quitCallback(Ref* sender)
{
    unscheduleUpdate();
    Director::getInstance()->replaceScene(MenuScene::createScene());
}

void GameScene::updateInput(float dt)
{
    if (true == isTouchDown)
    {
        if (initialTouchPosition[0] - currentTouchPosition[0] > size.width * 0.05f)
        {
            CCLOG("SWIPED LEFT");
            
            isTouchDown = false;
        }
        else if (initialTouchPosition[0] - currentTouchPosition[0] < -size.width * 0.05f)
        {
            CCLOG("SWIPED RIGHT");
            
            isTouchDown = false;
        }
        else if (initialTouchPosition[1] - currentTouchPosition[1] > size.width * 0.05f)
        {
            CCLOG("SWIPED DOWN");
            
            isTouchDown = false;
        }
        else if (initialTouchPosition[1] - currentTouchPosition[1] < -size.width * 0.05f)
        {
            CCLOG("SWIPED UP");
            
            isTouchDown = false;
        }
    }
}

void GameScene::update(float dt)
{
    updateInput(dt);
}