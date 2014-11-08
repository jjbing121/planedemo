#include "HelloWorldScene.h"

USING_NS_CC;

cocos2d::Scene* HelloWorld::createScene(Vector<Sprite*> getTransportArray)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create(getTransportArray);

    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size globalsize = Director::getInstance()->getWinSize();
    
    // notification center callback
    CCLOG("gettransportarray -> %d", (int)prelog_sprite_list.size());
    
    // 背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("diqiuyi.mp3");
    
    // 背景图
    Sprite* bg = Sprite::create("paperbg.png");
    bg->setPosition(globalsize.width/2, globalsize.height/2);
    bg->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(bg);
    
    // 纸飞机
    this->addChild(prelog_sprite_list.at(0));
    // 子弹
    this->addChild(prelog_sprite_list.at(1));
    
    // 敌机
    j=2;
    schedule(schedule_selector(HelloWorld::load_enemy), 2);
    
    // 记分板
    boardl = Label::createWithSystemFont("0", "Courier", 45);
    boardl->setPosition(globalsize.width-100, globalsize.height-50);
    boardl->setAnchorPoint(Vec2(0.5,0.5));
    boardl->setSystemFontSize(45);
    boardl->setTextColor(Color4B(70, 70, 70, 80));
    this->addChild(boardl);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, prelog_sprite_list.at(0));

    schedule(schedule_selector(HelloWorld::bulletshoot), 0.5f);  // 设置子弹射速
    schedule(schedule_selector(HelloWorld::updatebullet));       // 设置子弹碰撞
    schedule(schedule_selector(HelloWorld::updateplane), 0.5f);  // 设置飞机碰撞
    
    return true;
}

/*virtual*/ bool HelloWorld::onTouchBegan(Touch *t, Event *e)
{
    if (prelog_sprite_list.at(0)->getBoundingBox().containsPoint(t->getLocation())) {
        return true;
    }
    return false;
};

/*virtual*/ void HelloWorld::onTouchMoved(Touch *t, Event *e)
{
    if (prelog_sprite_list.at(0)!=NULL) {
        prelog_sprite_list.at(0)->setPosition(Vec2(t->getLocation().x, t->getLocation().y));
    }
};

/*virtual*/ void HelloWorld::onTouchEnded(Touch *t, Event *e)
{
    CCLOG("touch ended.");
};

/*virtual*/ void HelloWorld::bulletshoot(float directime)
{
    if (prelog_sprite_list.at(1) && prelog_sprite_list.at(0)) {
        prelog_sprite_list.at(1)->setPosition(prelog_sprite_list.at(0)->getPosition());
        MoveTo* bullet_mv = MoveTo::create(0.5, prelog_sprite_list.at(1)->getPosition()+Vec2(0,200));
        prelog_sprite_list.at(1)->runAction(bullet_mv);
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("bulletvoice.wav");
    }
}
/*virtual*/ void HelloWorld::updatebullet(float directime)
{
    for (int i=2; i<prelog_sprite_list.size(); i++) {
        
        if (prelog_sprite_list.at(i)) {
            
            if (prelog_sprite_list.at(1) && prelog_sprite_list.at(i)!=NULL && prelog_sprite_list.at(1)->getBoundingBox().intersectsRect(prelog_sprite_list.at(i)->getBoundingBox())) {
                CCLOG("bounding box ok.");
                FadeOut* enemy_out = FadeOut::create(1);
                prelog_sprite_list.at(i)->runAction(enemy_out);
                prelog_sprite_list.at(i)->stopAllActions();
                prelog_sprite_list.at(i)->removeFromParentAndCleanup(true);
//                prelog_sprite_list.at(i)=NULL;
                prelog_sprite_list.erase(i);
                int tmpmath = atoi(boardl->getString().c_str());
                char gmath[6];
                memset(gmath, 0, 6);
                sprintf(gmath, "%d", tmpmath+100);
                boardl->setString(gmath);
            }
        }
    }
};

/*virtual*/ void HelloWorld::updateplane(float directime)
{
    for (int i=2; i<prelog_sprite_list.size(); i++) {
        
        if (prelog_sprite_list.at(i)) {
            
            if (prelog_sprite_list.at(0) && prelog_sprite_list.at(i)!=NULL && prelog_sprite_list.at(0)->getBoundingBox().intersectsRect(prelog_sprite_list.at(i)->getBoundingBox())) {
                CCLOG("plane touched.");
                Hide* plane_action = Hide::create();
                prelog_sprite_list.at(0)->runAction(plane_action);

                prelog_sprite_list.at(0)->stopAllActions();
                prelog_sprite_list.at(0)->removeFromParentAndCleanup(true);
                //        prelog_sprite_list.at(0)=NULL;
                prelog_sprite_list.erase(0);
                prelog_sprite_list.at(1)->stopAllActions();
                prelog_sprite_list.at(1)->removeFromParentAndCleanup(true);
                //        prelog_sprite_list.at(1)=NULL;
                prelog_sprite_list.erase(1);
            }
        }
    }
}

HelloWorld* HelloWorld::create(Vector<Sprite*> getSpriteArray)
{
    HelloWorld* pRet = new HelloWorld();
    pRet->prelog_sprite_list = getSpriteArray;
    pRet->loadlog_sprite_list.operator=(getSpriteArray);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void HelloWorld::load_enemy(float addtime)
{
    CCLOG("j => %d, size => %d, newsize => %d", j, (int)prelog_sprite_list.size(), (int)loadlog_sprite_list.size());
    if (j<loadlog_sprite_list.size()) {
        this->addChild(loadlog_sprite_list.at(j));
        j++;
    }
    
}