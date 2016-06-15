//
//  MenuScene.hpp
//  TutorialCPP
//
//  Created by Mobile6 on 6/14/16.
//
//

#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class MenuScene: public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(MenuScene);
    static Scene* createScene();
    
    void updateScene();
    void settingsCallback(Ref* sender);
    void playCallback(Ref* sender);
    void rankingCallback(Ref* sender);
};

#endif