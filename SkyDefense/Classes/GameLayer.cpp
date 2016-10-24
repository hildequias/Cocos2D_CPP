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
    
    //release all retained actions
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

#pragma mark - Detecting Swipes
bool GameLayer::onTouchBegan(Touch * touch, Event * event){
    
    if (!_running) {
        if (_introMessage->isVisible()) {
            
            _introMessage->setVisible(false);
            this->resetGame();
            
        } else if (_gameOverMessage->isVisible()) {
            
            SimpleAudioEngine::getInstance()->stopAllEffects();
            _gameOverMessage->setVisible(false);
            this->resetGame();
            
        }
        return true;
    }
    
    if (touch) {
        
        if (_bomb->isVisible()) {
            
            _bomb->stopAllActions();
            auto child = _bomb->getChildByTag(kSpriteHalo);
            child->stopAllActions();
            child = _bomb->getChildByTag(kSpriteSparkle);
            child->stopAllActions();
            
            if (_bomb->getScale() > 0.25f) {
                
                _shockWave->setScale(0.1f);
                _shockWave->setPosition(_bomb->getPosition());
                _shockWave->setVisible(true);
                _shockWave->setOpacity(255);
                _shockWave->runAction(ScaleTo::create(0.5f, _bomb->getScale() * 2.0f));
                _shockWave->runAction( _shockwaveSequence->clone());
                SimpleAudioEngine::getInstance()->playEffect("bombRelease.wav");
                
            } else {
                SimpleAudioEngine::getInstance()->playEffect("bombFail.wav");
            }
            _bomb->setVisible(false);
            _shockwaveHits = 0;
            
        } else {
            
            Point tap = touch->getLocation();
            _bomb->stopAllActions();
            _bomb->setScale(0.1f);
            _bomb->setPosition(tap);
            _bomb->setVisible(true);
            _bomb->setOpacity(50);
            _bomb->runAction( _growBomb->clone() );
            
            auto child = _bomb->getChildByTag(kSpriteHalo);
            child->runAction( _rotateSprite->clone());
            child = _bomb->getChildByTag(kSpriteSparkle);
            child->runAction( _rotateSprite->clone());
        }
        
        return true;
    }
    
    return false;
}

void GameLayer::update (float dt) {
    
    if (!_running) return;
    
    int count;
    int i;
    
    //update timers
    _meteorTimer += dt;
    if (_meteorTimer > _meteorInterval) {
        _meteorTimer = 0;
        this->resetMeteor();
    }
    
    _healthTimer += dt;
    if (_healthTimer > _healthInterval) {
        _healthTimer = 0;
        this->resetHealth();
    }
    
    _difficultyTimer += dt;
    if (_difficultyTimer > _difficultyInterval) {
        _difficultyTimer = 0;
        this->increaseDifficulty();
    }
    
    if (_bomb->isVisible()) {
        if (_bomb->getScale() > 0.3f) {
            if (_bomb->getOpacity() != 255)
                _bomb->setOpacity(255);
        }
    }
    
    float diffx;
    float diffy;
    
    //check collision with shockwave
    if (_shockWave->isVisible()) {
        count = (int) _fallingObjects.size();
        
        for (i = count-1; i >= 0; i--) {
            auto sprite =  _fallingObjects.at(i);
            diffx = _shockWave->getPositionX() - sprite->getPositionX();
            diffy = _shockWave->getPositionY() - sprite->getPositionY();
            if (pow(diffx, 2) + pow(diffy, 2) <= pow(_shockWave->getBoundingBox().size.width * 0.5f, 2)) {
                sprite->stopAllActions();
                sprite->runAction( _explosion->clone());
                SimpleAudioEngine::getInstance()->playEffect("boom.wav");
                if (sprite->getTag() == kSpriteMeteor) {
                    _shockwaveHits++;
                    _score += _shockwaveHits * 13 + _shockwaveHits * 2;
                }
                //play sound
                _fallingObjects.erase(i);
            }
        }
        
        _scoreDisplay->setString(String::createWithFormat("%i", _score)->getCString());
    }
    
    //move clouds
    count = (int) _clouds.size();
    
    for (i = 0; i < count; i++) {
        auto sprite = _clouds.at(i);
        sprite->setPositionX(sprite->getPositionX() + dt * 20);
        if (sprite->getPositionX() > _screenSize.width + sprite->getBoundingBox().size.width * 0.5f)
            sprite->setPositionX(-sprite->getBoundingBox().size.width * 0.5f);
    }
}

void GameLayer::stopGame() {
    _running = false;
    //stop all actions currently running (meteors, health drops, animations,
    int i;
    int count = (int) _fallingObjects.size();
    
    for (i = count-1; i >= 0; i--) {
        auto sprite = _fallingObjects.at(i);
        sprite->stopAllActions();
        sprite->setVisible(false);
        _fallingObjects.erase(i);
    }
    if (_bomb->isVisible()) {
        _bomb->stopAllActions();
        _bomb->setVisible(false);
        auto child = _bomb->getChildByTag(kSpriteHalo);
        child->stopAllActions();
        child = _bomb->getChildByTag(kSpriteSparkle);
        child->stopAllActions();
    }
    if (_shockWave->isVisible()) {
        _shockWave->stopAllActions();
        _shockWave->setVisible(false);
    }
}

void GameLayer::resetGame(void) {
    
    _score = 0;
    _energy = 100;
    
    //reset timers and "speeds"
    _meteorInterval = 2.5;
    _meteorTimer = _meteorInterval * 0.99f;
    _meteorSpeed = 10;//in seconds to reach ground

    _healthInterval = 20;
    _healthTimer = 0;
    _healthSpeed = 15;//in seconds to reach ground
    
    _difficultyInterval = 60;
    _difficultyTimer = 0;
    _running = true;

    //reset labels
    _energyDisplay->setString(std::to_string((int) _energy) + "%");
    _scoreDisplay->setString(std::to_string((int) _score));
}

void GameLayer::createGameScreen() {
    
    //add bg
    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg);
    
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprite_sheet.plist");
    _gameBatchNode = SpriteBatchNode::create("sprite_sheet.png");
    this->addChild(_gameBatchNode);
    
    //First, the city:
    for (int i = 0; i < 2; i++) {
        
        auto sprite = Sprite::createWithSpriteFrameName("city_dark.png");
        sprite->setAnchorPoint(Vec2(0.5,0));
        sprite->setPosition(_screenSize.width * (0.25f + i * 0.5f),0);
        
        _gameBatchNode->addChild(sprite, kMiddleground);
        
        sprite = Sprite::createWithSpriteFrameName("city_light.png");
        sprite->setAnchorPoint(Vec2(0.5,0));
        sprite->setPosition(Vec2(_screenSize.width * (0.25f + i * 0.5f), _screenSize.height * 0.1f));
        
        _gameBatchNode->addChild(sprite, kBackground);
    }
    
    //add trees
    for (int i = 0; i < 3; i++) {
        
        auto sprite = Sprite::createWithSpriteFrameName("trees.png");
        sprite->setAnchorPoint(Vec2(0.5f, 0.0f));
        sprite->setPosition(Vec2(_screenSize.width * (0.2f + i * 0.3f),0));
        
        _gameBatchNode->addChild(sprite, kForeground);
    }
    
    _scoreDisplay = Label::createWithBMFont("font.fnt", "0");
    _scoreDisplay->setAnchorPoint(Vec2(1,0.5));
    _scoreDisplay->setPosition(Vec2(_screenSize.width * 0.8f, _screenSize.height * 0.94f));
    this->addChild(_scoreDisplay);
    
    _energyDisplay = Label::createWithBMFont("font.fnt", "100%",TextHAlignment::RIGHT);
    _energyDisplay->setPosition(Vec2(_screenSize.width * 0.3f, _screenSize.height * 0.94f));
    this->addChild(_energyDisplay);
    
    auto icon = Sprite::createWithSpriteFrameName("health_icon.png");
    icon->setPosition( Vec2(_screenSize.width * 0.15f, _screenSize.height * 0.94f) );
    
    _gameBatchNode->addChild(icon, kBackground);
    
    for (int i = 0; i < 4; i++) {
        
        float cloud_y = i % 2 == 0 ? _screenSize.height * 0.4f : _screenSize.height * 0.5f;
        auto cloud = Sprite::createWithSpriteFrameName("cloud.png");
        
        cloud->setPosition(Vec2 (_screenSize.width * 0.1f + i * _screenSize.width * 0.3f, cloud_y));
        _gameBatchNode->addChild(cloud, kBackground);
        _clouds.pushBack(cloud);
    }
    
    _bomb = Sprite::createWithSpriteFrameName("bomb.png");
    _bomb->getTexture()->generateMipmap();
    _bomb->setVisible(false);
    auto size = _bomb->getContentSize();
    
    //add sparkle inside bomb sprite
    auto sparkle = Sprite::createWithSpriteFrameName("sparkle.png");
    sparkle->setPosition(Vec2(size.width * 0.72f, size.height * 0.72f));
    _bomb->addChild(sparkle, kMiddleground, kSpriteSparkle);
    
    //add halo inside bomb sprite
    auto halo = Sprite::createWithSpriteFrameName("halo.png");
    halo->setPosition(Vec2(size.width * 0.4f, size.height * 0.4f));
    
    _bomb->addChild(halo, kMiddleground, kSpriteHalo);
    _gameBatchNode->addChild(_bomb, kForeground);
    
    _shockWave = Sprite::createWithSpriteFrameName("shockwave.png");
    _shockWave->getTexture()->generateMipmap();
    _shockWave->setVisible(false);
    _gameBatchNode->addChild(_shockWave);
    
    _introMessage = Sprite::createWithSpriteFrameName("logo.png");
    _introMessage->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.6f));
    _introMessage->setVisible(true);
    this->addChild(_introMessage, kForeground);
    
    _gameOverMessage = Sprite::createWithSpriteFrameName("gameover.png");
    _gameOverMessage->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.65f));
    _gameOverMessage->setVisible(false);
    this->addChild(_gameOverMessage, kForeground);
}

void GameLayer::createPools() {
    
    int i;
    _meteorPoolIndex = 0;
    
    for (i = 0; i < 50; i++) {
        
        auto sprite = Sprite::createWithSpriteFrameName("meteor.png");
        sprite->setVisible(false);
        _gameBatchNode->addChild(sprite, kMiddleground, kSpriteMeteor);
        _meteorPool.pushBack(sprite);
    }
    
    _healthPoolIndex = 0;
    
    for (i = 0; i < 20; i++) {
        
        auto sprite = Sprite::createWithSpriteFrameName("health.png");
        sprite->setVisible(false);
        sprite->setAnchorPoint(Vec2(0.5f, 0.8f));
        
        _gameBatchNode->addChild(sprite, kMiddleground, kSpriteHealth);
        _healthPool.pushBack(sprite);
    }
}


void GameLayer::createActions() {
    //swing action for health drops
    
    auto easeSwing = Sequence::create(EaseInOut::create(RotateTo::create(1.2f, -10), 2),
                                      EaseInOut::create(RotateTo::create(1.2f, 10), 2),
                                      nullptr);//mark the end of a sequence with a nullptr
    
    _swingHealth = RepeatForever::create((ActionInterval *) easeSwing);
    _swingHealth->retain();
    
    //action sequence for shockwave: fade out, callback when done
    _shockwaveSequence = Sequence::create(FadeOut::create(1.0f), CallFunc::create(std::bind(&GameLayer::shockwaveDone, this)), nullptr);
    _shockwaveSequence->retain();
    
    //action to grow bomb
    _growBomb = ScaleTo::create(6.0f, 1.0);
    _growBomb->retain();
    
    //action to rotate sprites
    auto rotate = RotateBy::create(0.5f , -90);
    _rotateSprite = RepeatForever::create( rotate );
    _rotateSprite->retain();
    
    auto animation = Animation::create();
    int i;
    
    for(i = 1; i <= 10; i++) {
        
        auto name = String::createWithFormat("boom%i.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name->getCString());
        animation->addSpriteFrame(frame);
    }
    
    animation->setDelayPerUnit(1 / 10.0f);
    animation->setRestoreOriginalFrame(true);
    
    _groundHit = Sequence::create(
                     MoveBy::create(0, Vec2(0,_screenSize.height * 0.12f)),
                     Animate::create(animation), CallFuncN::create(CC_CALLBACK_1(GameLayer::animationDone,this)), nullptr);
    _groundHit->retain();
    
    animation = Animation::create();
    
    for(int i = 1; i <= 7; i++) {
        
        auto name = String::createWithFormat("explosion_small%i.png", i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name->getCString());
        animation->addSpriteFrame(frame);
    }
    
    animation->setDelayPerUnit(0.5 / 7.0f);
    animation->setRestoreOriginalFrame(true);
    
    _explosion = Sequence::create(Animate::create(animation), CallFuncN::create(CC_CALLBACK_1(GameLayer::animationDone, this)), nullptr);
    
    _explosion->retain();
}

void GameLayer::animationDone (Node * pSender) {
    pSender->setVisible(false);
}

void GameLayer::shockwaveDone() {
    _shockWave->setVisible(false);
}

void GameLayer::resetMeteor(void) {
    
    if (_fallingObjects.size() > 30) return;
    
    auto meteor = _meteorPool.at(_meteorPoolIndex);
    _meteorPoolIndex++;
    if (_meteorPoolIndex == _meteorPool.size()) _meteorPoolIndex = 0;
    
    
    int meteor_x = rand() % (int) (_screenSize.width * 0.8f) + _screenSize.width * 0.1f;
    int meteor_target_x = rand() % (int) (_screenSize.width * 0.8f) + _screenSize.width * 0.1f;
    
    meteor->stopAllActions();
    meteor->setPosition(Vec2(meteor_x, _screenSize.height + meteor->getBoundingBox().size.height * 0.5));
    
    //create action
    auto  rotate = RotateBy::create(0.5f ,  -90);
    auto  repeatRotate = RepeatForever::create( rotate );
    auto  sequence = Sequence::create (
                                       MoveTo::create(_meteorSpeed, Vec2(meteor_target_x, _screenSize.height * 0.15f)),
                                       CallFunc::create(std::bind(&GameLayer::fallingObjectDone, this, meteor) ),
                                       nullptr);
    
    meteor->setVisible ( true );
    meteor->runAction(repeatRotate);
    meteor->runAction(sequence);
    _fallingObjects.pushBack(meteor);
}

void GameLayer::fallingObjectDone (Node * pSender) {
    
    
    _fallingObjects.erase(_fallingObjects.find( (Sprite *)pSender));
    pSender->stopAllActions();
    pSender->setRotation(0);
    
    if (pSender->getTag() == kSpriteMeteor) {
        changeEnergy(-15);
        pSender->runAction( _groundHit->clone() );
        //play explosion sound
        SimpleAudioEngine::getInstance()->playEffect("boom.wav");
    } else {
        pSender->setVisible(false);
        
        if (_energy == 100) {
            
            _score += 25;
            _scoreDisplay->setString(String::createWithFormat("%i", _score)->getCString());
            
        } else {
            changeEnergy(10);
        }
        //play health bonus sound
        SimpleAudioEngine::getInstance()->playEffect("health.wav");
    }
}

void GameLayer::changeEnergy(float value) {
    _energy += value;
    if (_energy <= 0) {
        _energy = 0;
        this->stopGame();
        SimpleAudioEngine::getInstance()->playEffect("fire_truck.wav");
        //show GameOver
        _gameOverMessage->setVisible(true);
    }
    
    if (_energy > 100) _energy = 100;
    
    _energyDisplay->setString(String::createWithFormat("%i%% ", (int)_energy)->getCString());
}

void GameLayer::increaseDifficulty () {
    
    _meteorInterval -= 0.15f;
    if (_meteorInterval < 0.25f) _meteorInterval = 0.25f;
    _meteorSpeed -= 1;
    if (_meteorSpeed < 4) _meteorSpeed = 4;
    
    _healthSpeed -= 1;
    if (_healthSpeed < 8) _healthSpeed = 8;
    
}


void GameLayer::resetHealth(void) {
    
    if (_fallingObjects.size() > 30) return;
    
    auto health = _healthPool.at(_healthPoolIndex);
    _healthPoolIndex++;
    if (_healthPoolIndex == _healthPool.size()) _healthPoolIndex = 0;
    
    
    int health_x = rand() % (int) (_screenSize.width * 0.8f) + _screenSize.width * 0.1f;
    int health_target_x = rand() % (int) (_screenSize.width * 0.8f) + _screenSize.width * 0.1f;
    
    health->stopAllActions();
    health->setPosition(Vec2(health_x, _screenSize.height + health->getBoundingBox().size.height * 0.5));
    
    //create action
    auto  sequence = Sequence::create(
                                      MoveTo::create(_healthSpeed, Vec2(health_target_x, _screenSize.height * 0.15f)),
                                      CallFunc::create(std::bind(&GameLayer::fallingObjectDone, this, health) ),
                                      nullptr);
    
    health->setVisible ( true );
    health->runAction( _swingHealth->clone());
    health->runAction(sequence);
    _fallingObjects.pushBack(health);
}

