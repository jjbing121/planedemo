//
//  LoadingScene.cpp
//  PlaneDemo
//
//  Created by Computer on 14/11/6.
//
//

#include "LoadingScene.h"

cocos2d::Scene* LoadingScene::LodingSceneCreate()
{
    auto scene = Scene::create();
    
    auto layer = LoadingScene::create();
    
    scene->addChild(layer);
    
    return scene;
};

/*virtual*/ bool LoadingScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    Size loading_size = Director::getInstance()->getVisibleSize();
    
    Sprite* loading_screen = Sprite::create("paperbg.png");
    loading_screen->setPosition(loading_size.width/2, loading_size.height/2);
    loading_screen->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(loading_screen);
    
    Sprite* processbar_str = Sprite::create("processbutton_start.png");
    processbar_str->setPosition(loading_size.width/2, loading_size.height/4);
    processbar_str->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(processbar_str, 1);
    
    Sprite* processbar_end = Sprite::create("processbutton_after.png");

    ptimer = ProgressTimer::create(processbar_end);
    ptimer->setType(ProgressTimer::Type::BAR);
    ptimer->setMidpoint(Vec2(0, 0.5));
    ptimer->setBarChangeRate(Vec2(1, 0));
    ptimer->setPercentage(0);
    ptimer->setPosition(loading_size.width/2, loading_size.height/4);
    ptimer->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(ptimer, 2);
    
    scheduleUpdate();
    
    // 预加载资源planelist
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("planelist.plist", "planelist.png");
    
    // 预加载资源music
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("diqiuyi.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("bulletvoice.mp3");

    return true;
};

/*virtual*/ void LoadingScene::update(float dt)
{
    float percent_get = ptimer->getPercentage();
    
    percent_get += 0.5f;
    
    if (percent_get <= 100) {
        ptimer->setPercentage(percent_get);
    }
    else
    {
//        Director::getInstance()->replaceScene(HelloWorld::createScene());
        Director::getInstance()->pushScene(HelloWorld::createScene());
    }
};
/*virtual*/ void LoadingScene::loadingcallback()
{
    CCLOG("loading ok.");
};

void LoadingScene::plistimagecallback(Texture2D* texture)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("planelist.plist", texture);
};