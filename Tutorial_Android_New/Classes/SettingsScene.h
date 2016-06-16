//
//  SettingsScene.hpp
//  TutorialCPP
//
//  Created by Mobile6 on 6/14/16.
//
//
#ifndef __SETTINGSSCENE_H__
#define __SETTINGSSCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class SettingsScene: public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(SettingsScene);
    static Scene* createScene();
};

#endif
