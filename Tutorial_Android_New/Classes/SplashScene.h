//
//  SplashScene.hpp
//  TutorialCPP
//
//  Created by Mobile6 on 6/14/16.
//
//
#ifndef __SPLASHSCENE_H__
#define __SPLASHSCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class SplashScene: public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(SplashScene);
    static Scene* createScene();
    
    void updateScene();
};

#endif