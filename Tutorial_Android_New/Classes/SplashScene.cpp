//
//  SplashScene.cpp
//  TutorialCPP
//
//  Created by Mobile6 on 6/14/16.
//
//

#include "SplashScene.h"
#include "MenuScene.h"

USING_NS_CC;

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
    
    Size size = Director::getInstance()->getWinSize();
    
    auto splash = Sprite::create("splash.png");
    splash->setPosition(Vec2(size.width/2, size.height/2));
    this->addChild(splash);
    
    auto loading = CallFunc::create(CC_CALLBACK_0(SplashScene::updateScene, this));
    auto delay = DelayTime::create(3.0f);
    auto sequence = Sequence::create(delay, loading, NULL);
    this->runAction(sequence);
    
    return true;
}

void SplashScene::updateScene() //.cpp
{
    Director::getInstance()->replaceScene(TransitionJumpZoom::create(1.5, MenuScene::createScene()));
}