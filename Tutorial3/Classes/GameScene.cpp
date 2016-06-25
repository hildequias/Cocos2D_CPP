#include "GameScene.h"
#include "MenuScene.h"
#include "Resources.h"
#include "Constants.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

GameScene* GameScene::instance = NULL;
HudLayer* GameScene::hudLayer = NULL;
//PauseLayer* GameScene::pauseLayer = NULL;

GameScene::GameScene()
: isPaused(false)
{
    instance = this;
}

GameScene::~GameScene()
{
    
}

GameScene* GameScene::getInstance()
{
    if (instance == NULL)
        instance = new GameScene();
    
    return instance;
}

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    
    scene->addChild(layer);
    
    auto hLayer = HudLayer::create();
    hudLayer = hLayer;
    
//    auto pLayer = PauseLayer::create();
//    pauseLayer = pLayer;
    
    scene->addChild(hudLayer);
    //scene->addChild(pauseLayer);
//    pauseLayer->setVisible(false);
    
    return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
		return false;

	// tamanho da tela
	size = Director::getInstance()->getWinSize();

	SimpleAudioEngine::getInstance()->playBackgroundMusic(ingameMusicFile);

	// inicializando input
	initInput();

	// inicializando background
	initBackground();

	// adicionando o lindo mar
	initSea();

    // inicia o Parallax
    initMountain();
    
    // inicia as nuvens
    initClouds();
    
	// criando sprite da madeira
	wood = Wood::create();
	this->addChild(wood);

	// criando sprite do marinheiro
	sailor = Sailor::create();
	sailor->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(sailor);
	sailor->activateIdleAnimation();

	// seta posição da madeira abaixo do marinheiro e ativa animação da madeira
	wood->setPosition(Vec2((size.width / 2) - 20, sailor->getPosition().y - (sailor->getContentSize().height / 2)));
	wood->activateAnimation();

	// gaivota
	seagull = Seagull::create();
	seagull->setPosition(Vec2(-100, size.height * 0.80f));
	this->addChild(seagull);
	seagull->activateMovement();

	// função que ativa update (gameloop)
	scheduleUpdate();

	return true;
}

void GameScene::initInput()
{
	// registrando listener para touch
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// inicializando estado da tela pressionada
	isTouchDown = false;

	// inicializando posições x e y 
	initialTouchPosition[0] = 0;
	initialTouchPosition[1] = 0;
}

void GameScene::initBackground()
{
	// criando sprite de fundo
	auto background = Sprite::create(backgroundFile1);
	background->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(background);
}

void GameScene::initMountain()
{
    mountainParallax = CCParallaxScrollNode::create();
    
    auto mountainSprite1 = Sprite::create(mountainFile);
    auto mountainSprite2 = Sprite::create(mountainFile);
    
    mountainParallax->addInfiniteScrollXWithZ(0, Vec2(1, 1), Vec2(0, 0), mountainSprite1, mountainSprite2, NULL);
    mountainParallax->setPosition(Vec2(0, size.height * 0.37f));
    
    this->addChild(mountainParallax);
}

void GameScene::initClouds()
{
    cloudParallax = CCParallaxScrollNode::create();
    
    auto cloud1 = Sprite::create(cloudFile0);
    auto cloud2 = Sprite::create(cloudFile1);
    auto cloud3 = Sprite::create(cloudFile2);
    
    cloudParallax->addInfiniteScrollXWithZ(0, Vec2(1, 1), Vec2(0, 0), cloud1, cloud2, cloud3, NULL);
    cloudParallax->setPosition(Vec2(0, size.height * 0.70f));
    this->addChild(cloudParallax);
    
}

void GameScene::initSea()
{
	Vector<SpriteFrame*> seaAnimFrames(8);

	auto seaSprite = Sprite::create();

	auto seaSprite0 = Sprite::createWithSpriteFrameName(seaSpriteFile0);
	auto seaSprite1 = Sprite::createWithSpriteFrameName(seaSpriteFile1);
	auto seaSprite2 = Sprite::createWithSpriteFrameName(seaSpriteFile2);
	auto seaSprite3 = Sprite::createWithSpriteFrameName(seaSpriteFile3);
	auto seaSprite4 = Sprite::createWithSpriteFrameName(seaSpriteFile4);
	auto seaSprite5 = Sprite::createWithSpriteFrameName(seaSpriteFile5);
	auto seaSprite6 = Sprite::createWithSpriteFrameName(seaSpriteFile6);
	auto seaSprite7 = Sprite::createWithSpriteFrameName(seaSpriteFile7);

	seaAnimFrames.pushBack(seaSprite0->getSpriteFrame());
	seaAnimFrames.pushBack(seaSprite1->getSpriteFrame());
	seaAnimFrames.pushBack(seaSprite2->getSpriteFrame());
	seaAnimFrames.pushBack(seaSprite3->getSpriteFrame());
	seaAnimFrames.pushBack(seaSprite4->getSpriteFrame());
	seaAnimFrames.pushBack(seaSprite5->getSpriteFrame());
	seaAnimFrames.pushBack(seaSprite6->getSpriteFrame());
	seaAnimFrames.pushBack(seaSprite7->getSpriteFrame());

	auto seaAnimation = Animation::createWithSpriteFrames(seaAnimFrames, 0.15f);
	auto animateSea = Animate::create(seaAnimation);

	seaSprite->setAnchorPoint(Vec2(0, 0));
	seaSprite->setPosition(Vec2(0, 0));
	this->addChild(seaSprite);
	seaSprite->runAction(RepeatForever::create(animateSea));
}


// usuário tocou na tela
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

// usuário tirou dedo da tela
void GameScene::onTouchEnded(Touch* touch, Event* pEvent)
{
	//CCLOG("Touch Ended");

	isTouchDown = false;
}

// usuário arrastou dedo na tela
void GameScene::onTouchMoved(Touch* touch, Event* pEvent)
{
	//CCLOG("Touch Moved");

	currentTouchPosition[0] = touch->getLocation().x;
	currentTouchPosition[1] = touch->getLocation().y;
}

// usuário cancelou toque
void GameScene::onTouchCancelled(Touch* touch, Event* pEvent)
{
	onTouchEnded(touch, pEvent);
}

// atualiza apenas gestos (swipe)
void GameScene::updateSwipeInput(float dt)
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
		else if (initialTouchPosition[1] - currentTouchPosition[1] > size.height * 0.05f)
		{
			CCLOG("SWIPED DOWN");

			isTouchDown = false;
		}
		else if (initialTouchPosition[1] - currentTouchPosition[1] < -size.height * 0.05f)
		{
			CCLOG("SWIPED UP");

			isTouchDown = false;
		}
	}
}

// Fun‹o para sair da Scene
void GameScene::quitGame()
{
//    pauseLayer->setVisible(false);
    Director::getInstance()->resume();
    unscheduleUpdate();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(MenuScene::createScene());
}

// update geral da cena
void GameScene::update(float dt)
{
    // Divide a velocidade da montanha pelo numero de pixel por segundo
    if (!isPaused)
    {
        updateSwipeInput(dt);
        
        cloudParallax->updateWithVelocity(Vec2(-VELOCITY_CLOUDS / PTM_RATIO, 0), dt);
        mountainParallax->updateWithVelocity(Vec2(-VELOCITY_MOUNTAIN / PTM_RATIO, 0), dt);
    }
}
