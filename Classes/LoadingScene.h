//
//  LoadingScene.h
//  PlaneDemo
//
//  Created by Computer on 14/11/6.
//
//

#ifndef __PlaneDemo__LoadingScene__
#define __PlaneDemo__LoadingScene__

#include <stdio.h>
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include <SimpleAudioEngine.h>

using namespace cocos2d;
using namespace CocosDenshion;

class LoadingScene : public cocos2d::Layer
{
private:
    ProgressTimer* ptimer;
    
public:
    static cocos2d::Scene* LodingSceneCreate();
    
    virtual bool init();
    
    CREATE_FUNC(LoadingScene);
    
public:
    virtual void update(float dt);
    
    void loadingcallback();
};

#endif /* defined(__PlaneDemo__LoadingScene__) */
