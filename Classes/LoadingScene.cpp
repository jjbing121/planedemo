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
    cache->addSpriteFramesWithFile("planeblist.plist", "planeblist.png");
    
    // 预加载资源music
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("diqiuyi.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("bulletvoice.mp3");

    // 自身飞机
    Sprite* selfplane = Sprite::createWithSpriteFrameName("PaperPlane.png");
    selfplane->setPosition(loading_size.width/2, 0);
    selfplane->setAnchorPoint(Vec2(0.5, 0.5));
    MoveBy* selfintoscene = MoveBy::create(1, Vec2(0, loading_size.height/8));
    selfplane->runAction(selfintoscene);
    SpriteArray.pushBack(selfplane);
    // 子弹加载
    Sprite* selfbullet = Sprite::createWithSpriteFrameName("planebullet.png");
    selfbullet->setPosition(selfplane->getPosition());
    selfbullet->setAnchorPoint(Vec2(0.5, 0.5));
    selfbullet->setScale(0.5);
    SpriteArray.pushBack(selfbullet);
    // 记分加载
    Label* scoreboard = Label::createWithSystemFont("0", "Courier", 45);
    scoreboard->setPosition(loading_size.width-100, loading_size.height-50);
    scoreboard->setAnchorPoint(Vec2(0.5,0.5));
    scoreboard->setSystemFontSize(45);
    scoreboard->setTextColor(Color4B(70, 70, 70, 80));
    SpriteArray.pushBack((Sprite*)scoreboard);
    // 敌机加载
    for (int i=0; i<10; i++) {
        Sprite* enemytmp = Sprite::createWithSpriteFrameName("PaperPlane_01.png");
        // 敌机随机位置出现
        int random_location = CCRANDOM_0_1()*loading_size.width;
        enemytmp->setPosition(random_location, loading_size.height+200);
        enemytmp->setAnchorPoint(Vec2(0.5, 0.5));
        Sequence* enemy_action = Sequence::create(RotateBy::create(0.1, 180), MoveTo::create(3, Vec2(loading_size.width/2, loading_size.height-1200)), NULL);
        enemytmp->runAction(enemy_action);
        SpriteArray.pushBack(enemytmp);
    }

    return true;
};

/*virtual*/ void LoadingScene::update(float dt)
{
    float percent_get = ptimer->getPercentage();
    
    percent_get += 5;
    
    if (percent_get <= 100) {
        ptimer->setPercentage(percent_get);
    }
    else
    {
        
        Director::getInstance()->replaceScene(HelloWorld::createScene(SpriteArray));
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
