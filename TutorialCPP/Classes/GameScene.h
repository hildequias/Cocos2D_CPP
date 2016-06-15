//
//  GameScene.hpp
//  TutorialCPP
//
//  Created by Mobile6 on 6/14/16.
//
//

#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameScene: public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(GameScene);
    static Scene* createScene();
    
    bool onTouchBegan(Touch* touch, Event* pEvent);
    void onTouchEnded(Touch* touch, Event* pEvent);
    void onTouchMoved(Touch* touch, Event* pEvent);
    void onTouchCancelled(Touch *touch, Event* pEvent);
    
    virtual void update(float dt);
    
    void quitCallback(Ref* sender); 
    void updateInput(float dt);
    
private:
    Size size;
    
    float currentTouchPosition[1];
    float initialTouchPosition[1];
    bool isTouchDown;
};

#endif
