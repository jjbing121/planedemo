//
//  MenuScene.h
//  PlaneDemo
//
//  Created by Computer on 14/11/6.
//
//

#ifndef __PlaneDemo__MenuScene__
#define __PlaneDemo__MenuScene__

#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "LoadingScene.h"
#include <SimpleAudioEngine.h>

using namespace cocos2d;
using namespace CocosDenshion;

class MenuScene : public cocos2d::Layer
{
private:
    Sprite* menubutton_start;
    Sprite* menubutton_after;
public:
    static cocos2d::Scene* MenuSceneCreate();
    
    virtual bool init();
    
    CREATE_FUNC(MenuScene);
    
public:
    // menubutton click
    virtual bool onMenuButtonClick(Touch *touch, Event *unused_event);
};

#endif /* defined(__PlaneDemo__MenuScene__) */
