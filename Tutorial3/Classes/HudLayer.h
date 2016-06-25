#ifndef __HUDLAYER_H__
#define __HUDLAYER_H__

#include "cocos2d.h"
using namespace cocos2d;

class HudLayer : public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(HudLayer);
    
    void initScore();
    void initPauseMenu();
    void initCoins();
    
    void pauseCallback(Ref* sender);
    
    Label* scoreLabel;
    Label* lifeLabel;
    
    
private:
    Size size;
};

#endif