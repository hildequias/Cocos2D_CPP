//
//  GameLayer.cpp
//  SkyDefense
//
//  Created by Hildequias Junior on 9/21/16.
//
//

#include "GameLayer.h"

using namespace cocos2d;


GameLayer::~GameLayer () {
    
    
    CC_SAFE_RELEASE(_growBomb);
    CC_SAFE_RELEASE(_rotateSprite);
    CC_SAFE_RELEASE(_shockwaveSequence);
    CC_SAFE_RELEASE(_swingHealth);
    CC_SAFE_RELEASE(_groundHit);
    CC_SAFE_RELEASE(_explosion);
    
    _clouds.clear();
    _meteorPool.clear();
    _healthPool.clear();
    _fallingObjects.clear();
    
}

GameLayer::GameLayer () :  _meteorPool(50), _healthPool(20),  _fallingObjects(40), _clouds(4)
{}

Scene* GameLayer::scene()
{
    auto scene = Scene::create();
    
    auto layer = GameLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    //get screen size
    _screenSize = Director::getInstance()->getWinSize();
    
    _running = false;
    
    //create game screen elements
    this->createGameScreen();
    
    //create object pools
    this->createPools();
    this->createActions();
    
    //listen for touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //create main loop
    this->scheduleUpdate();
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
    return true;
    
    return true;
}

bool GameLayer::onTouchBegan(Touch * touch, Event * event){
    
    return true;
}

void GameLayer::update (float dt) {
    
}

void GameLayer::stopGame() {
    
    
}

void GameLayer::createGameScreen() {
    
    //add bg
    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg);
    
}

void GameLayer::createPools() {
    
}


void GameLayer::createActions() {
    
}


