#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Sailor.h"
#include "Wood.h"
#include "Seagull.h"
#include "CCParallaxScrollNode.h"
#include "PauseLayer.h"
#include "HudLayer.h"

using namespace cocos2d;

class GameScene : public Layer
{
public:
    GameScene();
    ~GameScene();
    static GameScene* instance;
    static GameScene* getInstance();
    
    virtual bool init();
    CREATE_FUNC(GameScene);
    static Scene* createScene();
    bool onTouchBegan(Touch* touch, Event* pEvent);
    void onTouchEnded(Touch* touch, Event* pEvent);
    void onTouchMoved(Touch* touch, Event* pEvent);
    void onTouchCancelled(Touch *touch, Event* pEvent);
    virtual void update(float dt);
    
    void initInput();
	void initBackground();
	void initSea();
    void initMountain();
    void initClouds();
    
    void pauseGame();
    void quitGame();
    
	void updateSwipeInput(float dt);

	Sailor* sailor;
	Wood* wood;
	Seagull* seagull;
    
    CCParallaxScrollNode* mountainParallax;
    CCParallaxScrollNode* cloudParallax;
    
    static HudLayer* hudLayer;
    
private:
	Size size;

	float currentTouchPosition[1];
	float initialTouchPosition[1];
	bool isTouchDown;
    bool isPaused;
};

#endif