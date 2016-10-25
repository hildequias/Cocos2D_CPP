/*
 
 Background music:
 8bit Dungeon Level by Kevin MacLeod (incompetech.com)
 
 */

#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "Rocket.h"
#include "LineContainer.h"


USING_NS_CC;

using namespace CocosDenshion;


GameLayer::~GameLayer () {
    _planets.clear();
}

GameLayer::GameLayer() : _planets(7) {}

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
    
	//init game values
	_screenSize = Director::getInstance()->getWinSize();
    _drawing = false;
    _minLineLength = _screenSize.width * 0.07f;
    _state = kGameIntro;
    
    createGameScreen();
    
    createParticles();
    
    createStarGrid();
    
    //listen for touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //create main loop
    this->scheduleUpdate();
    
    return true;
}

void GameLayer::update (float dt) {
    
}

bool GameLayer::onTouchBegan(Touch *touch, Event *event){
	
    return true;
}

void GameLayer::onTouchMoved(Touch *touch, Event *event){
    
}

void GameLayer::onTouchEnded(Touch *touch, Event *event){
    
    if (_state == kGameIntro) {
        
        _intro->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = kGamePlay;
        //resetGame();
        return;
        
    } else if (_state == kGamePaused) {
        
        _pauseBtn->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName ("btn_pause_off.png"));
        _paused->setVisible(false);
        _state = kGamePlay;
        _running = true;
        return;
        
    } else if (_state == kGameOver) {
        
        _gameOver->setVisible(false);
        _pauseBtn->setVisible(true);
        _state = kGamePlay;
        //resetGame();
        return;
        
    }
    
    if (!_running) return;

}

void GameLayer::resetGame () {

    _rocket->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.1f));
    _rocket->setOpacity(255);
    _rocket->setVisible(true);
    _rocket->reset();
    
    _cometInterval = 4;
    _cometTimer = 0;
    _timeBetweenPickups = 0.0;
    
    _score = 0;
    _scoreDisplay->setString(String::createWithFormat("%i", _score)->getCString());
    
    _lineContainer->reset();
    
    //shuffle grid cells
    std::random_shuffle(_grid.begin(), _grid.end());
    _gridIndex = 0;
    
    resetStar();
    
    _warp->stopSystem();
    
    _running = true;
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3", true);
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->playEffect("rocket.wav", true);
    
}

void GameLayer::resetStar() {
   
}

void GameLayer::killPlayer() {

}


void GameLayer::createGameScreen () {
    
    auto bg = Sprite::create("bg.png");
    bg->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    this->addChild(bg, kBackground);
    
    
    _lineContainer = LineContainer::create();
    this->addChild(_lineContainer, 1);
    
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sprite_sheet.plist");
    _gameBatchNode = SpriteBatchNode::create("sprite_sheet.png", 100);

    this->addChild(_gameBatchNode, kForeground);
    
    _rocket = Rocket::create();
    _rocket->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.1f));
    _gameBatchNode->addChild(_rocket, kForeground, kSpriteRocket);
    

    //add planets
    auto planet = GameSprite::createWithFrameName("planet_1.png");
    planet->setPosition(Vec2(_screenSize.width * 0.25f,
                            _screenSize.height * 0.8f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets.pushBack(planet);
    
    planet = GameSprite::createWithFrameName("planet_2.png");
    planet->setPosition(Vec2(_screenSize.width * 0.8f,
                            _screenSize.height * 0.45f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets.pushBack(planet);
    
    planet = GameSprite::createWithFrameName("planet_3.png");
    planet->setPosition(Vec2(_screenSize.width * 0.75f,
                            _screenSize.height * 0.8f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets.pushBack(planet);
    
    planet = GameSprite::createWithFrameName("planet_4.png");
    planet->setPosition(Vec2(_screenSize.width * 0.5f,
                            _screenSize.height * 0.5f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets.pushBack(planet);
    
    planet = GameSprite::createWithFrameName("planet_5.png");
    planet->setPosition(Vec2(_screenSize.width * 0.18f,
                            _screenSize.height * 0.45f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets.pushBack(planet);
    
    planet = GameSprite::createWithFrameName("planet_6.png");
    planet->setPosition(Vec2(_screenSize.width * 0.8f,
                            _screenSize.height * 0.15f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets.pushBack(planet);
    
    planet = GameSprite::createWithFrameName("planet_7.png");
    planet->setPosition(Vec2(_screenSize.width * 0.18f,
                            _screenSize.height * 0.1f));
    _gameBatchNode->addChild(planet, kBackground, kSpritePlanet);
    _planets.pushBack(planet);
    
    auto scoreLabel = Sprite::createWithSpriteFrameName("label_score.png");
    scoreLabel->setPosition(Vec2(_screenSize.width * 0.4f, _screenSize.height * 0.95));
    _gameBatchNode->addChild(scoreLabel, kBackground);
    
    _scoreDisplay = Label::createWithBMFont("font.fnt", "0", TextHAlignment::LEFT);
    _scoreDisplay->setWidth( _screenSize.width * 0.5f);
    _scoreDisplay->setAnchorPoint(Vec2(0,0.5));
    _scoreDisplay->setPosition(Vec2(_screenSize.width * 0.48f, _screenSize.height * 0.95f));
    this->addChild(_scoreDisplay, kBackground);
    
    _pauseBtn = GameSprite::createWithFrameName("btn_pause_off.png");
    _pauseBtn->setVisible(false);
    _pauseBtn->setPosition(Vec2(_screenSize.width * 0.06f, _screenSize.height * 0.95f));
    _gameBatchNode->addChild(_pauseBtn, kBackground);
    
    
    _intro = Sprite::createWithSpriteFrameName("logo.png");
    _intro->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    auto play = Sprite::createWithSpriteFrameName("label_play.png");
    play->setPosition(Vec2(_intro->boundingBox().size.width * 0.5f, -_intro->boundingBox().size.height * 0.5f));
    _intro->addChild(play);
    _gameBatchNode->addChild(_intro, kForeground);
    
    _gameOver = Sprite::createWithSpriteFrameName("gameOver.png");
    _gameOver->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    _gameOver->setVisible(false);
    _gameBatchNode->addChild(_gameOver, kForeground);
    
    _paused = Sprite::createWithSpriteFrameName("label_paused.png");
    _paused->setPosition(Vec2(_screenSize.width * 0.5f, _screenSize.height * 0.55f));
    _paused->setVisible(false);
    _gameBatchNode->addChild(_paused, kForeground);
}


void GameLayer::createParticles() {

}

void GameLayer::createStarGrid() {
    //create grid
    float gridFrame = _screenSize.width * 0.1f;
    int tile = 32;
    int rows = (_screenSize.height - 4 * gridFrame)/tile;
    int cols = (_screenSize.width  - 2 * gridFrame)/tile;
    
    bool overlaps;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            auto cell = Vec2(gridFrame + c * tile, 2 * gridFrame + r * tile);
            overlaps = false;
            for ( auto planet : _planets)
            {
                if (pow(planet->getPositionX() - cell.x, 2) + pow(planet->getPositionY() - cell.y, 2) <= pow(planet->getRadius() * 1.2f, 2)) {
                    overlaps = true;
                }
            }
            if (!overlaps) _grid.push_back(cell);
        }
    }
    CCLOG("POSSIBLE STARS: %i", (int)_grid.size());
    
}